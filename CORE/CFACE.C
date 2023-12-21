//===================================================================
//
//Zheng zhikai  95-4-21
//===================================================================

#include "prehead.h"

#include "core.h"
#include "cerror.h"
#include "cobj.h"
#include "corerw.h"
#include "cface.h"
#include "cstamp.h"
#include "csysstmp.h"

static WRD FaceGetFreeRoom(void);
static void FaceReleaseRoom(WRD _wrd);

BOOL _face_delete(WRD wrd_);
BOOL _FaceTreeCheckReference( WRD wrdRoot_, WRD wrdTest_ );

//local functions for face paste
static void _FaceAdjustReferenceTree( WRD wrdDes_, WRD wrdSrc_, WRD wrdRootDes_, WRD wrdRootSrc_ );
static WRD _FacePasteTree( WRD wrdParent_, WRD wrdCopy_ );
static WRD _FacePaste( WRD wrdParent_, WRD wrdCopy_ );
static BOOL _FaceSynRef( WRD wrdRootSrc_, WRD wrdRefSrc_, WRD wrdRootDes_, WRD* pwrdRefDes_ );

void _near _FaceSetChild(WRD wrd_, WRD wChild_);
void _near _FaceSetNext(WRD wrd_, WRD wNext_);

extern FTInfo       gFTInfo;
extern BOOL         gbRunTime;

extern DWORD        gdwLastError;
extern CoreInfo	    gCoreInfo;

WORD                gwSHSize = 4;

struct {
	WORD    element;
	char    szName[16];
	VSOF	vsof;
	DIRTY	dirty;	//dirty mask
	WORD    type;
} static ssNameSet[] =
{
	{FTU_IDSTAMP,    "Stamp",        VSOF_IDSTAMP,		FTUBIT_DIRTY_IDSTAMP,	TET_PRIMARY|TET_READONLY },
	{FTU_NUMPARAM,   "NumParam",     VSOF_WORD,			FTUBIT_DIRTY_NUMPARAM,	TET_PRIMARY|TET_READONLY },
	{FTU_PARENT,     "Parent",       VSOF_WRDFTU,		FTUBIT_DIRTY_PARENT,	TET_PRIMARY|TET_READONLY },
	{FTU_NEXT,       "Next",         VSOF_WRDFTU,		FTUBIT_DIRTY_NEXT,		TET_PRIMARY|TET_READONLY },
	{FTU_CHILD,      "Child",        VSOF_WRDFTU,		FTUBIT_DIRTY_CHILD,		TET_PRIMARY|TET_READONLY },
	//private data
	{FTU_STATUS,     "Status",       VSOF_STATUS_FTU,	FTUBIT_DIRTY_STATUS,	TET_PRIMARY|TET_READONLY|TET_PRIVATE },
	{FTU_DIRTY,      "Dirty",        VSOF_DIRTY_FTU,	FTUBIT_DIRTY_DIRTY,		TET_PRIMARY|TET_READONLY|TET_PRIVATE },
	{FTU_POSITION,   "Position",     VSOF_FPTR_RECT,	FTUBIT_DIRTY_POSITION,	TET_PRIMARY|TET_READONLY|TET_PRIVATE },
	{FTU_PARAM,      "Param",        VSOF_FPTR,			FTUBIT_DIRTY_PARAM,		TET_PRIMARY|TET_READONLY|TET_PRIVATE },
};
#define _NUM_NAMESET	( sizeof(ssNameSet)/sizeof(ssNameSet[0]) )


#define _RETURN_FALSE_IF_INVALID(_wrd_)							\
	if( !((gFTInfo.lpBase)[_wrd_].Status & FTUBIT_VALID) )		\
	{  															\
		CORE_ASSERT( FALSE );									\
		SET_LAST_ERROR( ERR_Invalid_FTU );						\
		return FALSE;											\
	}															


//...................................................................
//Description:
//  Creates a face-unit in Face Table as the last unit at
//  the face-link owned by wParant_.
//Note:
//  Because always needs wParent_, the function can't create the
//  first Face("Root") in face table. the "Root" is added to
//  the table directly by InitCore.
//Entry:
//  WRD     wParent_
//      Identifies the parent node of the unit to be allocated.
//  IDSTAMP idStamp_
//      Specifies the stamp id. of the face to be created.
//  LONG    lData_
//      Calller specified data for passing to StampPICCreate()
//Returns:
//      the subscription of face-unit if the function succeeds.
//      Otherwise, it is NULL.
//...................................................................
_DLLEXP WRD _DLLFUNC FaceCreate(WRD     wParent_,
								IDSTAMP idStamp_,
								LONG    lData_)
{
	LPFTU       _ptu;
	WRD         _wMe, _wBrother;
	LPPU        _lpPrm = NULL;
	WORD        _wNumParam;

	if( !gFTInfo.lpBase )
		return 0;

	if(!IsStamp(idStamp_))
		return 0;   //illegal stamp. id.

	//check the legality of the wParant_
	if(  (wParent_ >= gFTInfo.wAdding)
	   ||(((gFTInfo.lpBase)[wParent_].Status & FTUBIT_VALID) == 0) )
	{
		SET_LAST_ERROR( ERR_Invalid_Parent_Node );
		return FALSE;
	}

	_wMe = FaceGetFreeRoom();
	if( !_wMe ) return 0; //if no free room

	_wNumParam = StampGetNumParam( idStamp_ );
	if(_wNumParam)
	{
		_lpPrm = ParamAlloc( _wNumParam );
		if(_lpPrm == NULL)
		{
			SET_LAST_ERROR( ERR_Frag_Allocate_Failure );
			goto _BAD;
		}
	}

	_ptu = &((gFTInfo.lpBase)[_wMe]);
	memset(_ptu, 0, sizeof(FTU));   //clear the new unit

	//copy data
	_ptu->Stamp = idStamp_;
	_ptu->NumParam = _wNumParam;
	_ptu->lpParam = _lpPrm;
	_ptu->Status = FTUBIT_VALID;
	_ptu->Dirty = FTUBIT_DIRTY_ALL;
	_ptu->Parent = wParent_;

	//update the link data
	_wBrother = (gFTInfo.lpBase)[wParent_].Child;
	if(_wBrother == NULL)
		_FaceSetChild( wParent_, _wMe);
	else
	{
		while((gFTInfo.lpBase)[_wBrother].Next)
			_wBrother = (gFTInfo.lpBase)[_wBrother].Next;

		CORE_ASSERT( (gFTInfo.lpBase)[_wBrother].Parent == wParent_ );
		CORE_ASSERT( (gFTInfo.lpBase)[_wBrother].Next == NULL );

		_FaceSetNext(_wBrother, _wMe);
	}

	if( !StampPICCreate( idStamp_, _wMe, lData_ ) )
	{
		FaceDelete( _wMe );
		return NULL;
	}

	DispatchCoreMessage( TBX_FT );
	return _wMe;

_BAD:
	if(_lpPrm)
		FreeParameter( _lpPrm, _wNumParam );
	FaceReleaseRoom( _wMe );
	return 0;
}

//...................................................................
//Description:
//  Create a temp. face by copying from wrd_
//...................................................................
_DLLEXP WRD _DLLFUNC FaceCopyTemp( WRD wrd_ )
{
	LPFTU       _ptu, _lpSrc;
	WRD         _wMe;
	LPPU        _lpPrmTemp;

	CLEAR_LAST_ERROR;

	_wMe = FaceGetFreeRoom();
	if( !_wMe ) return 0; //if no free room

	_lpSrc = gFTInfo.lpBase + wrd_;

	//do not check for legality of the tag
	//do not check for legality of the stamp
	//do not check for legality of the wParant

	if(_lpSrc->NumParam)
	{
		WORD	_i;

		_lpPrmTemp = (LPPU)ParamAlloc( _lpSrc->NumParam );
		if(_lpPrmTemp == NULL)
		{
			SET_LAST_ERROR( ERR_Frag_Allocate_Failure );
			goto _BAD;
		}

		memcpy(_lpPrmTemp, _lpSrc->lpParam, _lpSrc->NumParam * sizeof(PU) );
		//increase the reference counter of all cobjects
		for( _i=0; _i<_lpSrc->NumParam; _i++ )
		{
			if( (_lpPrmTemp[_i]).prefix == (PF_IMM|IMM_LPCOBJ) )
				CobjLock( (_lpPrmTemp[_i]).du.zzk__LPCOBJ );
		}
	}
	else
	{
		CORE_ASSERT(!_lpSrc->lpParam);
		_lpPrmTemp = NULL;
	}

	_ptu = (gFTInfo.lpBase) + _wMe;

	//copy old data
	memcpy( _ptu, _lpSrc, sizeof(FTU) );    //clear the new unit

	//update new data
	_ptu->lpParam = _lpPrmTemp;
	_ptu->Status |= FTUBIT_VALID|FTUBIT_TEMP;

	//NOTE: do NOT update face tree link

	//Note:
	//Do NOT call StampPICCreate(...) to initialize the new face
	//because that function will reset the parameters of the face

	return _wMe;

_BAD:
	FaceReleaseRoom( _wMe );
	return FALSE;
}

//...................................................................
//Description:
//  Deletes the specified face in Face Table.
//Return:
//  If the face to be freed is valid, returns TRUE. Otherwise,
//  returns FALSE.
//Note:
//  The function does not clear the memory occupted by the deleted face;
//  because if we do that, the dirty checking routine can't find the
//  face table has been changed( the face is deleted and no dirty flag
//  leave ); So, we set dirty bits for the deleted face but not clear it
//  until next saving;
//...................................................................
_DLLEXP BOOL _DLLFUNC FaceDelete(WRD wrd_)
{
	BOOL	_ret;

	CLEAR_LAST_ERROR;

	if( !FaceIsValid(wrd_) )
	{
		CORE_ASSERT( FALSE );
		return FALSE;
	}

	if( wrd_ == FACE_ROOT )
	{
		CORE_ASSERT( FALSE );
		return FALSE;
	}

	if( _FaceTreeCheckReference( wrd_, wrd_ ) )
	{
		SET_LAST_ERROR( ERR_Referenced_FTU );
		return FALSE;   //the FTU is referenced by other xTUs
	}

	_ret = _face_delete( wrd_ );
	if( _ret ) DispatchCoreMessage( TBX_FT );

	return _ret;
}

/*	this is an old impliment version
_DLLEXP BOOL _DLLFUNC FaceDelete(WRD wrd_)
{
	WRD         _wPreFace;
	LPFTU       _lpFace = gFTInfo.lpBase + wrd_;

	if( !(_lpFace->Status & FTUBIT_VALID) )
		return FALSE;

	if( GetReference(TB_FT, wrd_, TRUE, NULL) )
	{
		SET_LAST_ERROR( ERR_Referenced_FTU );
		return FALSE;   //the FTU is referenced by other xTUs
	}

	StampPICDelete( _lpFace->Stamp, wrd_ );

	//adjust tree structure
	_wPreFace = FaceGetPrevious(wrd_);
	if(_wPreFace == NULL)   //this is the first child
	{
		//whether there are other children or not, this statement is correct.
		_FaceSetChild(_lpFace->Parent, _lpFace->Next);
	}
	else
		_FaceSetNext(_wPreFace, _lpFace->Next);

	//free its children
	while(_lpFace->Child) FaceDelete(_lpFace->Child);

	//free paramters memory resource
	if(_lpFace->lpParam)
	{
		WORD	_i;
		LPPU	_lpPrm = _lpFace->lpParam;

		//delete cobjects in parameter array
		for( _i=0; _i<_lpFace->NumParam; _i++ )
		{
			if( (_lpPrm[_i]).prefix == (PF_IMM|IMM_LPCOBJ) )
				CobjFree( (_lpPrm[_i]).du.zzk__COBJ );
		}

		FreeParameter( _lpFace->lpParam );
		_lpFace->lpParam = NULL;
	}

	_lpFace->Status &= (~FTUBIT_VALID);   //clear VALID bit
	_lpFace->Dirty |= FTUBIT_DIRTY_ALL;

	return TRUE;
}
*/

//...................................................................
//Description:
//	This is an inner function for deleting face;
//...................................................................
BOOL _face_delete(WRD wrd_)
{
	WRD     _wrdPrev;
	LPFTU   _lpFace = gFTInfo.lpBase + wrd_;

	/////////////////////////
	//free its children first

	while( _lpFace->Child )
		_face_delete( _lpFace->Child );

	/////////////////////////
	//then free self

	StampPICDelete( _lpFace->Stamp, wrd_ );

	//property tree structure
	_wrdPrev = FaceGetPrevious(wrd_);
	if( _wrdPrev == NULL )  //this is the first child
	{
		//whether there are other children or not, this code is correct.
		_FaceSetChild(_lpFace->Parent, _lpFace->Next);
	}
	else
		_FaceSetNext(_wrdPrev, _lpFace->Next);

	//free paramters memory resource
	if(_lpFace->lpParam)
	{
		FreeParameter( _lpFace->lpParam, _lpFace->NumParam );
		_lpFace->lpParam = NULL;
	}

	_lpFace->Status &= (~FTUBIT_VALID);  //clear VALID bit
	_lpFace->Dirty |= FTUBIT_DIRTY_ALL;

	return TRUE;
}


//...................................................................
//Description:
//...................................................................
_DLLEXP BOOL _DLLFUNC FaceDeleteTemp(WRD wrd_)
{
	LPFTU   _lpFace = &((gFTInfo.lpBase)[wrd_]);

	CLEAR_LAST_ERROR;

	if(   (_lpFace->Status & FTUBIT_VALID)
	   && (_lpFace->Status & FTUBIT_TEMP) )
	{
		//free paramters memory resource
		if((gFTInfo.lpBase)[wrd_].lpParam)
			FreeParameter( _lpFace->lpParam, _lpFace->NumParam );

		FaceReleaseRoom(wrd_);

		return TRUE;
	}
	else
	{
		CORE_ASSERT(0);
		return FALSE;
	}
}

//...................................................................
//Description:
//...................................................................
static WRD FaceGetFreeRoom()
{
	WRD     _i;

	for(_i=1; _i<gFTInfo.wAdding; _i++)
	{
		if( (gFTInfo.lpBase + _i)->Status == 0 )
			break;
	}

	if( _i < gFTInfo.wAdding )
		return _i;
	else    // _i == gFTInfo.wAdding
	{
		if( gFTInfo.wAdding < gFTInfo.wCount )
		{
			gFTInfo.wAdding ++;
			return _i;
		}
		else    //no free room
			return 0;
	}
}

//...................................................................
//...................................................................
static void FaceReleaseRoom(WRD wrd_)
{
	if( wrd_ >= gFTInfo.wAdding )
	{
		CORE_ASSERT( FALSE );
		return ;
	}

	memset( gFTInfo.lpBase + wrd_, 0, sizeof(FTU) );

	//if the RTU[adding-1] is free, decrease Adding
	while(  ((gFTInfo.lpBase + gFTInfo.wAdding - 1)->Status == 0 )
		  &&((gFTInfo.lpBase + gFTInfo.wAdding - 1)->Dirty == 0 )
		  &&( gFTInfo.wAdding > 1 ) )
		gFTInfo.wAdding -- ;

	return ;
}

//...................................................................
//Description:
//	This function checks if the faces in the face-tree that specified
//	by wrdTest_, are referenced by the face which is out of the tree
//	that specified by wrdRoot_;
//...................................................................
BOOL _FaceTreeCheckReference( WRD wrdRoot_, WRD wrdTest_ )
{
	COREREF _core_ref;
	WRD		_wrdChild;

	memset( &_core_ref, 0, sizeof(_core_ref) );

_AGAIN:
	if( GetReference( TB_FT, wrdTest_, TRUE, &_core_ref ) )
	{
		if( _core_ref.wTable != TB_FT )
			return TRUE;
		else
		{
			//check if the referenced face is in this tree
			if( !FaceIsInTree( wrdRoot_, _core_ref.wrd ) )
				return TRUE;
		}

		//continue to search
		goto _AGAIN;
	}

	for( _wrdChild = FaceGetChild( wrdTest_ );
		 _wrdChild;
		 _wrdChild = FaceGetNext( _wrdChild ) )
	{
		if( _FaceTreeCheckReference( wrdRoot_, _wrdChild ) )
			return TRUE;
	}

	return FALSE;
}

//...................................................................
//Description:
//	This function checks if a face is in a given tree
//...................................................................
_DLLEXP BOOL _DLLFUNC FaceIsInTree( WRD wrdRoot_, WRD wrdTest_ )
{
	do
	{
		if( wrdTest_ == wrdRoot_ )
			return TRUE;
		wrdTest_ = FaceGetParent( wrdTest_ );
	} while( wrdTest_ );

	return FALSE;
}

//...................................................................
//Description:
//  Undo a face specified by wFace_ using its undo buffer: wBuffer_
//and then Delete the undo buffer face.
//...................................................................
_DLLEXP BOOL _DLLFUNC FaceUndo( WRD wFace_, WRD wBuffer_ )
{
	LPFTU   _lpFace, _lpBuffer;

	if( !wFace_ || !wBuffer_ )
	{
		SET_LAST_ERROR( ERR_Invalid_Entries );
		return FALSE;
	}
	_lpFace = gFTInfo.lpBase + wFace_;
	_lpBuffer = gFTInfo.lpBase + wBuffer_;

	if( _lpFace->lpParam )      //free the dirty parameters
		FreeParameter( _lpFace->lpParam, _lpFace->NumParam );

	//undo the parameters of the face and other elements
	//NOTE: the parameter of the undo buffer face is sent
	//to the wFace_
	memcpy( _lpFace, _lpBuffer, sizeof( FTU) );
	_lpFace->Status &= (~FTUBIT_TEMP); //clear TEMP bit

	//delete the undo buffer FTU
	memset( _lpBuffer, 0, sizeof(FTU) );
	FaceReleaseRoom(wBuffer_);

	return TRUE;
}

//...................................................................
//Description:
//  This function instead the face parameter with undo buffer.
//then delete the buffer.
//Note:
//	This function is designed for history trend only;
//...................................................................
_DLLEXP BOOL _DLLFUNC FaceUpdateParameter( WRD wFace_, WRD wBuffer_ )
{
	LPPU	_lpPrm;
	WORD	_wNumParam;
	LPFTU	_lpBuffer;

	_lpPrm = FaceGetParameter( wBuffer_ );
	_wNumParam = FaceGetNumParam( wBuffer_ );
	FaceChangeParameter( wFace_, _wNumParam, _lpPrm );

	//now the parameter array of the face specified by
	//wBuffer_ has been transfered to wFace_, so deleting
	//its room directly and must not free its parameter
	//array;

	_lpBuffer = gFTInfo.lpBase + wBuffer_;

	//delete the undo buffer FTU
	memset( _lpBuffer, 0, sizeof(FTU) );
	FaceReleaseRoom( wBuffer_ );

	return TRUE;
}

//...................................................................
//Description:
//  This function only checks the FTUBIT_VALID bit;
//...................................................................
_DLLEXP BOOL _DLLFUNC FaceIsValid( WRD wrd_ )
{
	if(  ( wrd_ < gFTInfo.wCount )
	   &&( (gFTInfo.lpBase + wrd_)->Status & FTUBIT_VALID ) )
		return TRUE;
	else return FALSE;
}

_DLLEXP BOOL _DLLFUNC FacePaste( WRD wrdParent_, WRD wrdCopy_, BOOL bTree_ )
{
	WRD		_wrd;

	if( !FaceIsValid(wrdParent_) ) return 0;
	if( !FaceIsValid(wrdCopy_) ) return 0;

	if( bTree_ )
		_wrd = _FacePasteTree( wrdParent_, wrdCopy_ );
	else
		_wrd = _FacePaste( wrdParent_, wrdCopy_ );

	//adjust references
	_FaceAdjustReferenceTree( _wrd, wrdCopy_, _wrd, wrdCopy_ );

	return _wrd;
}

static void _FaceAdjustReferenceTree( WRD wrdDes_, WRD wrdSrc_, WRD wrdRootDes_, WRD wrdRootSrc_ )
{
	WRD		_wrdSrc, _wrdDes;
	WRD		_wrdRef;
	WORD	_i;
	WORD	_num_param = FaceGetNumParam( wrdDes_ );
	LPPU	_lpPUDes, _lpPUSrc;

	for( _i=0, _lpPUDes = FaceGetParameter(wrdDes_), _lpPUSrc = FaceGetParameter(wrdSrc_);
		 _i<_num_param; 
		 _i++, _lpPUDes++, _lpPUSrc++ )
	{
		if(  ( _lpPUSrc->prefix == (PF_TBX|TBX_FT) )
		   ||( (_lpPUSrc->prefix & PF_MASK) == PF_FPX ) )
		{
			if( !FaceIsInTree( wrdRootSrc_, _lpPUSrc->du.zzk__WRD ) )
				continue;

			if( !_FaceSynRef( wrdRootSrc_, _lpPUSrc->du.zzk__WRD, wrdRootDes_, &_wrdRef ) )
				continue;
			
			CORE_ASSERT( _lpPUSrc->prefix == _lpPUDes->prefix );
			_lpPUDes->du.zzk__WRD = _wrdRef;
		}
	}

	for(_wrdSrc = FaceGetChild( wrdSrc_ ), _wrdDes = FaceGetChild( wrdDes_ );
		_wrdSrc && _wrdDes;
		_wrdSrc = FaceGetNext( _wrdSrc ), _wrdDes = FaceGetNext( _wrdDes ) )
	{
		_FaceAdjustReferenceTree( _wrdDes, _wrdSrc, wrdRootDes_, wrdRootSrc_ );
	}

	return;
}

static WRD _FacePasteTree( WRD wrdParent_, WRD wrdCopy_ )
{
	WRD		_wrd, _wrdPaste;
	BOOL	_ok = TRUE;

	_wrdPaste = _FacePaste( wrdParent_, wrdCopy_ );
	if( !_wrdPaste )
		return 0;

	for(_wrd = FaceGetChild( wrdCopy_ );
		_wrd;
		_wrd = FaceGetNext( _wrd ) )
	{
		if( !_FacePasteTree( _wrdPaste, _wrd ) )
		{
			_ok = FALSE;
			break;
		}
	}

	if( !_ok )
	{
		_face_delete( _wrdPaste );
		return 0;
	}
	else
		return _wrdPaste;
}

static WRD _FacePaste( WRD wrdParent_, WRD wrdCopy_ )
{
	LPFTU	_lpFtuParent, _lpFtuCopy, _lpFtuPaste;
	WRD		_wrdPaste = 0;
	WORD	_i;
	LPPU	_lpPU, _lpPUSrc;
	BOOL	_bOk;	//cobject copy result

	_lpFtuParent = gFTInfo.lpBase + wrdParent_;
	_lpFtuCopy = gFTInfo.lpBase + wrdCopy_;

	_wrdPaste = FaceCreate( wrdParent_, _lpFtuCopy->Stamp, 0 );
	if( !_wrdPaste )
		return 0;
	_lpFtuPaste =  gFTInfo.lpBase + _wrdPaste;

	//this causes it will be update always
	SetRect( &(_lpFtuPaste->Rect), 0, 0, 32766, 32766 );
	_lpFtuPaste->Status = _lpFtuCopy->Status;

	CORE_ASSERT( !(_lpFtuPaste->Status & FTUBIT_TEMP) );
	CORE_ASSERT( _lpFtuPaste->Dirty == (DIRTY)-1 );

	if( _lpFtuPaste->NumParam != _lpFtuCopy->NumParam )
	{
		LPPU	_lpPrmNew = ParamAlloc( _lpFtuCopy->NumParam );
		if( !_lpPrmNew )
		{
			_face_delete( _wrdPaste );
			FaceReleaseRoom( _wrdPaste );
			return 0;
		}

		ParamFree( _lpFtuPaste->lpParam ); 
		_lpFtuPaste->lpParam = _lpPrmNew;
		_lpFtuPaste->NumParam = _lpFtuCopy->NumParam;
	}

	memcpy(  _lpFtuPaste->lpParam,
			 _lpFtuCopy->lpParam,
			 sizeof(PU)* _lpFtuCopy->NumParam );

	//paste the parameters

	//in the first loop, clear the LPCOBJ pointers in order to avoid deleting core objects
	//at the case that an error occurred
	for( _i=0, _lpPU = _lpFtuPaste->lpParam;
		 _i<_lpFtuPaste->NumParam; 
		 _i++, _lpPU++ )
	{
		if( _lpPU->prefix == (PF_IMM|IMM_LPCOBJ) )
			_lpPU->du.zzk__LPCOBJ = NULL;
	}

	//then, copy core objects
	for( _i=0, _lpPU = _lpFtuPaste->lpParam, _lpPUSrc = _lpFtuCopy->lpParam;
		 _i<_lpFtuPaste->NumParam; 
		 _i++, _lpPU++, _lpPUSrc++ )
	{
		if( _lpPU->prefix == (PF_IMM|IMM_LPCOBJ) )
		{
			LPCOBJ	_lpCobj = CobjCopy( _lpPUSrc->du.zzk__LPCOBJ, &_bOk );
			if( !_bOk )
			{
				//delete this
				_face_delete( _wrdPaste );
				FaceReleaseRoom( _wrdPaste );
				return 0;
			}
			else
			{
				_lpPU->du.zzk__LPCOBJ = _lpCobj;
			}
		}
	}

	if( !StampPICPaste( _lpFtuPaste->Stamp, _wrdPaste ) )
	{
		FaceDelete( _wrdPaste );
		return NULL;
	}

	return _wrdPaste;
}

//here, there are two cases permitted: 
//	1.	the source tree and the destination tree are identical
//	2.	the destination tree is no children, while the source tree is a tree
//
static BOOL _FaceSynRef( WRD wrdRootSrc_, WRD wrdRefSrc_, WRD wrdRootDes_, WRD* pwrdRefDes_ )
{
	WRD		_wrdSrc, _wrdDes;

	if( wrdRootSrc_ == wrdRefSrc_ )
	{
		*pwrdRefDes_ = wrdRootDes_;
		return TRUE;
	}

	for(_wrdSrc = FaceGetChild( wrdRootSrc_ ), _wrdDes = FaceGetChild( wrdRootDes_ );
		_wrdSrc && _wrdDes ;
		_wrdSrc = FaceGetNext( _wrdSrc ), _wrdDes = FaceGetNext( _wrdDes ) )
	{
		if( _FaceSynRef( _wrdSrc, wrdRefSrc_, _wrdDes, pwrdRefDes_ ) )
			return TRUE;
	}
	return FALSE;
}

//...................................................................
//...................................................................
_DLLEXP BOOL _DLLFUNC FaceMove(WRD wrd_, WRD wrdParentNew_, WRD wrdPrevNew_ )
{
	WRD _wrd;

	if( !((gFTInfo.lpBase)[wrd_].Status & FTUBIT_VALID) )
		return FALSE;

	if( !((gFTInfo.lpBase)[wrdParentNew_].Status & FTUBIT_VALID) )
		return FALSE;

	if( wrd_ == FACE_ROOT )
	{
		CORE_ASSERT( FALSE );
		return FALSE;
	}

	if( wrdPrevNew_ )
		if( FaceGetParent( wrdPrevNew_ ) != wrdParentNew_ )
		{
			CORE_ASSERT( FALSE );
			return FALSE;
		}

	//at the first, detach the wrd_ from the tree
	_wrd = FaceGetPrevious(wrd_);
	if(_wrd == NULL)
	{
		_wrd = FaceGetParent(wrd_);
		(gFTInfo.lpBase)[_wrd].Child = (gFTInfo.lpBase)[wrd_].Next;
		(gFTInfo.lpBase)[_wrd].Dirty |= FTUBIT_DIRTY_CHILD;
	}
	else
	{
		(gFTInfo.lpBase)[_wrd].Next = (gFTInfo.lpBase)[wrd_].Next;
		(gFTInfo.lpBase)[_wrd].Dirty |= FTUBIT_DIRTY_NEXT;
	}

	//second, place the wrd_ to the new position
	if( wrdPrevNew_ )
	{
		//backup the old next node
		_wrd = (gFTInfo.lpBase)[wrdPrevNew_].Next;
		(gFTInfo.lpBase)[wrdPrevNew_].Next = wrd_;
		(gFTInfo.lpBase)[wrdPrevNew_].Dirty |= FTUBIT_DIRTY_NEXT;
	}
	else	//as the first child
	{
		_wrd = (gFTInfo.lpBase)[wrdParentNew_].Child;
		(gFTInfo.lpBase)[wrdParentNew_].Child = wrd_;
		(gFTInfo.lpBase)[wrdParentNew_].Dirty = FTUBIT_DIRTY_PARENT;
	}
	(gFTInfo.lpBase)[wrd_].Parent = wrdParentNew_;
	(gFTInfo.lpBase)[wrd_].Next = _wrd;
	(gFTInfo.lpBase)[wrd_].Dirty |= (FTUBIT_DIRTY_NEXT|FTUBIT_DIRTY_PARENT);

	return TRUE;
}

//...................................................................
//Description:
//  Checks the legality of a FTU, including:
//      . legality of Status (ERROR CODE: FTU_ERR_STATUSVALID)
//      . legality of Stamp (ERROR CODE: FTU_ERR_STAMP)
//      . legality of the wParant (ERROR CODE: FTU_ERR_PARENT)
//  The error code is stored in a global var: giCoreErrorCode.
//
//Entry:
//  FTU FAR* lp_
//      Points to the FTU to be checked. if lp_ == NULL, returns
//      FALSE and no error code.
//Return:
//      if FTU is legality, returns TRUE; otherwise, returns
//      FALSE.
//...................................................................
_DLLEXP BOOL _DLLFUNC FaceCheckLegality(WRD wrd_)
{
	LPFTU   _lp = gFTInfo.lpBase + wrd_;

	CLEAR_LAST_ERROR;

	if( (_lp->Status & FTUBIT_VALID) == 0 )
	{
		SET_LAST_ERROR( ERR_Invalid_Entries );
		return FALSE;
	}

	//the stamp no found
	if( !IsStamp(_lp->Stamp) )
	{
		SET_LAST_ERROR( ERR_Invalid_Stamp );
		return FALSE;
	}

	//check the legality of the wParant_
	if(  ( ((gFTInfo.lpBase)[_lp->Parent].Status & FTUBIT_VALID) == 0 )
	   ||(_lp->Parent >= gFTInfo.wAdding) )
	{
		if( wrd_ != FACE_ROOT )    //FACE_ROOT has no parent node
		{
			SET_LAST_ERROR( ERR_Invalid_Parent_Node );
			return FALSE;
		}
	}

	//add the following check
	//  . the number of parameter
	//  . the datalink legality
	//those checks should be fulfilled by the corresponding stamp
	if( !StampCallParamProc( _lp->Stamp, PIC_CHECKPARAM, wrd_, 0, 0 ) )
		return FALSE;

	return TRUE;
}

//...................................................................
//Description:
//  ONLY valid face has a tag, a unique read-only tag which specified
//  by system;
//Return:
//	0:			failed, no name returns
//	1:			successful with real name
//	2:			successful with pseudo name
//	because the face unit no real name here, pseudo name always returns 
//...................................................................
_DLLEXP int _DLLFUNC FaceGetTag( WRD wrd_, LPSTR szTag_, WORD wSize_ )
{
	int		_ret = 0;

	CLEAR_LAST_ERROR;

	if( wrd_ >= gFTInfo.wCount )
	{
		CORE_ASSERT(FALSE);
		SET_LAST_ERROR( ERR_Invalid_Entries );
		return 0;
	}

	if( !szTag_ )
	{
		CORE_ASSERT( szTag_ );
		SET_LAST_ERROR( ERR_Invalid_Entries );
		return 0;
	}

	if( !wrd_ )	//0 is a valid parameter
	{
		WRDToTag( wrd_, szTag_, wSize_ );
		_ret = 2;	//pseudo name
	}
	else
	{
		LPFTU 	_lpFtu;
		_lpFtu = gFTInfo.lpBase + wrd_;

		if( _lpFtu->Status & FTUBIT_VALID )
		{
			WRDToTag( wrd_, szTag_, wSize_ );
			_ret = 2;	//pseudo name
		}
		else
		{
			CORE_ASSERT( FALSE );
			SET_LAST_ERROR( ERR_Invalid_Entries );
			return 0;
		}
	}

	return _ret;
}

//...................................................................
//Description:
//
//...................................................................
_DLLEXP WRD _DLLFUNC FaceGetPrevious( WRD wrd_ )
{
	WRD     _wFace;

	//assert that the FTU is valid
	CORE_ASSERT( (gFTInfo.lpBase)[wrd_].Status & FTUBIT_VALID );

	_wFace = (gFTInfo.lpBase)[wrd_].Parent;   //get the parent node
	_wFace = (gFTInfo.lpBase)[_wFace].Child;   //get the first node

	if(_wFace == wrd_)
		return 0;   //its the first face

	while(_wFace)
	{
		if( wrd_ == (gFTInfo.lpBase)[_wFace].Next )
			return _wFace;
		else
			_wFace = (gFTInfo.lpBase)[_wFace].Next;
	}

	CORE_ASSERT(0);
	return 0;
}

//...................................................................
//...................................................................
_DLLEXP void _DLLFUNC FaceSetStatus(WRD wrd_, STATUS status_ )
{
	//assert that the FTU is valid
	CORE_ASSERT( (gFTInfo.lpBase)[wrd_].Status & FTUBIT_VALID );

	(gFTInfo.lpBase)[wrd_].Status = status_;
	return;
}

//...................................................................
//...................................................................
_DLLEXP BOOL _DLLFUNC IsFaceSelect(WRD wrd_)
{
	//assert that the FTU is valid
	CORE_ASSERT( (gFTInfo.lpBase)[wrd_].Status & FTUBIT_VALID );

	if( (gFTInfo.lpBase)[wrd_].Status & FTUBIT_SELECT )
		return TRUE;
	else
		return FALSE;
}

//...................................................................
//...................................................................
_DLLEXP void _DLLFUNC FaceSetSelect(WRD wrd_)
{
	//assert that the FTU is valid
	CORE_ASSERT( (gFTInfo.lpBase)[wrd_].Status & FTUBIT_VALID );

	(gFTInfo.lpBase)[wrd_].Status |= FTUBIT_SELECT;
	return;
}

//...................................................................
//...................................................................
_DLLEXP void _DLLFUNC FaceClearSelect(WRD wrd_)
{
	//assert that the FTU is valid
	CORE_ASSERT( (gFTInfo.lpBase)[wrd_].Status & FTUBIT_VALID );

	(gFTInfo.lpBase)[wrd_].Status &= (~FTUBIT_SELECT);
	return;
}

//...................................................................
//Return:
//  Returns TRUE if selected, otherwise, returns FALSE;
//...................................................................
_DLLEXP BOOL _DLLFUNC FaceToggleSelect( WRD wrd_ )
{
	//assert that the FTU is valid
	CORE_ASSERT( (gFTInfo.lpBase)[wrd_].Status & FTUBIT_VALID );

	//toggle the selected bit
	(gFTInfo.lpBase)[wrd_].Status ^= (FTUBIT_SELECT);

	return (gFTInfo.lpBase)[wrd_].Status & (FTUBIT_SELECT);
}

//...................................................................
//...................................................................
_DLLEXP BOOL _DLLFUNC IsFaceBlink(WRD wrd_)
{
	//assert that the FTU is valid
	CORE_ASSERT( (gFTInfo.lpBase)[wrd_].Status & FTUBIT_VALID );

	if( (gFTInfo.lpBase)[wrd_].Status & FTUBIT_BLINK )
		return TRUE;
	else
		return FALSE;
}

//...................................................................
//...................................................................
_DLLEXP void _DLLFUNC FaceSetBlink(WRD wrd_)
{
	//assert that the FTU is valid
	CORE_ASSERT( (gFTInfo.lpBase)[wrd_].Status & FTUBIT_VALID );

	(gFTInfo.lpBase)[wrd_].Status |= FTUBIT_BLINK;
	return;
}

//...................................................................
//...................................................................
_DLLEXP void _DLLFUNC FaceClearBlink(WRD wrd_)
{
	//assert that the FTU is valid
	CORE_ASSERT( (gFTInfo.lpBase)[wrd_].Status & FTUBIT_VALID );

	(gFTInfo.lpBase)[wrd_].Status &= (~FTUBIT_BLINK);
	return;
}

//...................................................................
//...................................................................
_DLLEXP void _DLLFUNC FaceToggleExpand( WRD wrd_ )
{
	//assert that the FTU is valid
	CORE_ASSERT( (gFTInfo.lpBase)[wrd_].Status & FTUBIT_VALID );

	//toggle the expanded bit
	(gFTInfo.lpBase)[wrd_].Status ^= (FTUBIT_EXPANDED);

	return ;
}

//...................................................................
//...................................................................
_DLLEXP BOOL _DLLFUNC IsFaceRedraw(WRD wrd_)
{
	//assert that the FTU is valid
	CORE_ASSERT( (gFTInfo.lpBase)[wrd_].Status & FTUBIT_VALID );

	if( (gFTInfo.lpBase)[wrd_].Status & FTUBIT_REDRAW )
		return TRUE;
	else
		return FALSE;
}

//...................................................................
//...................................................................
_DLLEXP void _DLLFUNC FaceSetRedraw( WRD wrd_ )
{
	//assert that the FTU is valid
	CORE_ASSERT( (gFTInfo.lpBase)[wrd_].Status & FTUBIT_VALID );

	(gFTInfo.lpBase)[wrd_].Status |= FTUBIT_REDRAW;

	return ;
}

//...................................................................
//...................................................................
_DLLEXP void _DLLFUNC FaceClearRedraw( WRD wrd_ )
{
	//assert that the FTU is valid
	CORE_ASSERT( (gFTInfo.lpBase)[wrd_].Status & FTUBIT_VALID );

	(gFTInfo.lpBase)[wrd_].Status &= (~FTUBIT_REDRAW);

	return ;
}

//...................................................................
//Description:
//  Set the status bit: FTUBIT_AUTOSIZECHILDREN
//...................................................................
_DLLEXP void _DLLFUNC FaceSetAutoSizeChildren( WRD wrd_ )
{
	//assert that the FTU is valid
	CORE_ASSERT( (gFTInfo.lpBase)[wrd_].Status & FTUBIT_VALID );

	//toggle the selected bit
	(gFTInfo.lpBase)[wrd_].Status |= (FTUBIT_AUTOSIZECHILDREN);

	return ;
}

//...................................................................
//Description:
//Entry:
//Return:
//...................................................................
_DLLEXP WRD _DLLFUNC FaceGetWRD(LPCSTR szTag_)
{
	if(*szTag_ == '#')
	{
		WRD _wrd = TagToWRD(szTag_);
		if( _wrd == NULL)
			return 0;
		else    //if( (gFTInfo.lpBase[_wrd]).Status & FTUBIT_VALID )
			return _wrd;
	}

	return 0;
}

//...................................................................
//...................................................................
_DLLEXP LPRECT _DLLFUNC FaceGetRect(WRD wrd_, LPRECT lpRect_)
{
	//assert that the FTU is valid
	CORE_ASSERT( (gFTInfo.lpBase)[wrd_].Status & FTUBIT_VALID );
	CORE_ASSERT( lpRect_ );

	CopyRect( lpRect_, &(gFTInfo.lpBase[wrd_].Rect) );

	return lpRect_;
}

//...................................................................
//...................................................................
_DLLEXP STATUS _DLLFUNC FaceGetStatus( WRD wrd_ )
{
	//assert that the FTU is valid
	CORE_ASSERT( (gFTInfo.lpBase)[wrd_].Status & FTUBIT_VALID );

	return (gFTInfo.lpBase)[wrd_].Status;
}

//...................................................................
//...................................................................
_DLLEXP DIRTY _DLLFUNC FaceGetDirty( WRD wrd_ )
{
	//assert that the FTU is valid
	CORE_ASSERT( (gFTInfo.lpBase)[wrd_].Status & FTUBIT_VALID );
	return (gFTInfo.lpBase)[wrd_].Dirty;
}

//...................................................................
//...................................................................
_DLLEXP IDSTAMP _DLLFUNC FaceGetStamp(WRD wrd_)
{
	CORE_ASSERT( (gFTInfo.lpBase[wrd_].Status & FTUBIT_VALID) );

	return (gFTInfo.lpBase)[wrd_].Stamp;
}

//...................................................................
//...................................................................
_DLLEXP WORD _DLLFUNC FaceGetNumParam(WRD wrd_)
{
	CORE_ASSERT( (gFTInfo.lpBase[wrd_].Status & FTUBIT_VALID) );
	return (gFTInfo.lpBase)[wrd_].NumParam;
}

//...................................................................
//...................................................................
_DLLEXP WRD _DLLFUNC FaceGetParent(WRD wrd_)
{
	CORE_ASSERT( gFTInfo.lpBase[wrd_].Status & FTUBIT_VALID );
	return (gFTInfo.lpBase)[wrd_].Parent;
}

//...................................................................
//...................................................................
_DLLEXP WRD _DLLFUNC FaceGetChild(WRD wrd_)
{
	CORE_ASSERT( gFTInfo.lpBase[wrd_].Status & FTUBIT_VALID );
	return (gFTInfo.lpBase)[wrd_].Child;
}

//...................................................................
//...................................................................
_DLLEXP WRD _DLLFUNC FaceGetNext(WRD wrd_)
{
	CORE_ASSERT( gFTInfo.lpBase[wrd_].Status & FTUBIT_VALID );
	return (gFTInfo.lpBase)[wrd_].Next;
}

//...................................................................
//...................................................................
_DLLEXP LPPU _DLLFUNC FaceGetParameter(WRD wrd_)
{
	CORE_ASSERT( gFTInfo.lpBase[wrd_].Status & FTUBIT_VALID );
	return (gFTInfo.lpBase)[wrd_].lpParam;
}

//...................................................................
//...................................................................
_DLLEXP void _DLLFUNC FaceSetRect( WRD wrd_, LPRECT lpRect_)
{
	//assert that the FTU is valid
	CORE_ASSERT( (gFTInfo.lpBase)[wrd_].Status & FTUBIT_VALID );

	memcpy( &( (gFTInfo.lpBase)[wrd_].Rect ), lpRect_, sizeof(RECT) );
	(gFTInfo.lpBase)[wrd_].Dirty |= FTUBIT_DIRTY_POSITION;
	return;
}

//...................................................................
//...................................................................
_DLLEXP WORD _DLLFUNC FaceGetElementType( WORD element_ )
{
	int     _i;

	for(_i=0; _i<_NUM_NAMESET; _i++)
		if(element_ == ssNameSet[_i].element)
			return ssNameSet[_i].type;

	return 0;
}

//...................................................................
//...................................................................
_DLLEXP BOOL _DLLFUNC FaceGetElementName(WORD element_, LPSTR szName_, WORD wSize_)
{
	unsigned        _i;

	for(_i=0; _i<_NUM_NAMESET; _i++)
	{
		if(element_ == ssNameSet[_i].element)
		{
			StringCopyn(szName_, ssNameSet[_i].szName, wSize_);
			return TRUE;
		}
	}

	CORE_ASSERT(0);
	return FALSE;
}

//...................................................................
//...................................................................
_DLLEXP WORD _DLLFUNC FaceGetElement(LPSTR szName_)
{
	unsigned        _i;

	for(_i=0; _i<_NUM_NAMESET; _i++)
	{
		if( !strnicmp(ssNameSet[_i].szName, szName_, 4) )
			return ssNameSet[_i].element;
	}

	return 0;
}

//...................................................................
//...................................................................
_DLLEXP BOOL _DLLFUNC FaceStoreAsFirstChild(WRD wrd_)
{
	if( (wrd_ != FACE_ROOT) &&
		((gFTInfo.lpBase)[wrd_].Status & FTUBIT_VALID) )
	{
		WRD _wrd;
		_wrd = FaceGetPrevious(wrd_);
		if(_wrd == NULL)
			return TRUE;    //the FTU is the first child already

		(gFTInfo.lpBase)[_wrd].Next = (gFTInfo.lpBase)[wrd_].Next;
		(gFTInfo.lpBase)[_wrd].Dirty |= FTUBIT_DIRTY_NEXT;

		_wrd = FaceGetParent(wrd_);

		(gFTInfo.lpBase)[wrd_].Next = (gFTInfo.lpBase)[_wrd].Child;
		(gFTInfo.lpBase)[wrd_].Dirty |= FTUBIT_DIRTY_NEXT;

		(gFTInfo.lpBase)[_wrd].Child = wrd_;
		(gFTInfo.lpBase)[_wrd].Dirty |= FTUBIT_DIRTY_CHILD;
		return TRUE;
	}
	else
	{
		CORE_ASSERT(0);
		return FALSE;
	}
}

//...................................................................
//...................................................................
_DLLEXP BOOL _DLLFUNC FaceStoreAsLastChild(WRD wrd_)
{
	if( (wrd_ != FACE_ROOT) &&
		((gFTInfo.lpBase)[wrd_].Status & FTUBIT_VALID) )
	{
		WRD _wrd;

		if((gFTInfo.lpBase)[wrd_].Next == NULL)
			return TRUE;

		_wrd = FaceGetPrevious(wrd_);
		if(_wrd == NULL)
		{
			_wrd = FaceGetParent(wrd_);
			(gFTInfo.lpBase)[_wrd].Child = (gFTInfo.lpBase)[wrd_].Next;
			(gFTInfo.lpBase)[_wrd].Dirty |= FTUBIT_DIRTY_CHILD;
		}
		else
		{
			(gFTInfo.lpBase)[_wrd].Next = (gFTInfo.lpBase)[wrd_].Next;
			(gFTInfo.lpBase)[_wrd].Dirty |= FTUBIT_DIRTY_NEXT;
		}

		_wrd = FaceGetLast(wrd_);
		(gFTInfo.lpBase)[_wrd].Next = wrd_;
		(gFTInfo.lpBase)[_wrd].Dirty |= FTUBIT_DIRTY_NEXT;

		(gFTInfo.lpBase)[wrd_].Next = NULL;
		(gFTInfo.lpBase)[wrd_].Dirty |= FTUBIT_DIRTY_NEXT;
		return TRUE;
	}
	else
	{
		CORE_ASSERT(0);
		return FALSE;
	}
}

//...................................................................
//...................................................................
_DLLEXP BOOL _DLLFUNC FaceExchangeTreeNode(WRD wFTU1_, WRD wFTU2_)
{
	WRD _parent, _wrdBack, _wrdFore;
	WRD _preBack, _preFore, _nxtBack, _nxtFore;

	_wrdBack = _wrdFore = NULL;

	if(  ( (gFTInfo.lpBase)[wFTU1_].Status & FTUBIT_VALID )
	   &&( (gFTInfo.lpBase)[wFTU2_].Status & FTUBIT_VALID ) )
	{
		_parent = (gFTInfo.lpBase)[wFTU1_].Parent;

		if( _parent == (gFTInfo.lpBase)[wFTU2_].Parent )
		{
			WRD _wrd = (gFTInfo.lpBase)[_parent].Child;

			while(_wrd)
			{
				if(_wrd == wFTU1_)
				{
					_wrdBack = wFTU1_;  _wrdFore = wFTU2_;
					break;
				}
				else if(_wrd == wFTU2_)
				{
					_wrdBack = wFTU2_;  _wrdFore = wFTU1_;
					break;
				}
				_wrd = (gFTInfo.lpBase)[_wrd].Next;
			}

			CORE_ASSERT(_wrdBack && _wrdFore);

			_preBack = FaceGetPrevious(_wrdBack);
			_preFore = FaceGetPrevious(_wrdFore);
			_nxtBack = (gFTInfo.lpBase)[_wrdBack].Next;
			_nxtFore = (gFTInfo.lpBase)[_wrdFore].Next;

			if(!_preBack)   //it is the first child
			{
				(gFTInfo.lpBase)[_parent].Child = _wrdFore;  //set Parent's child
				(gFTInfo.lpBase)[_parent].Dirty |= FTUBIT_DIRTY_CHILD;
			}
			else
			{
				(gFTInfo.lpBase)[_preBack].Next = _wrdFore;  //set Previous' next
				(gFTInfo.lpBase)[_preBack].Dirty |= FTUBIT_DIRTY_NEXT;
			}

			(gFTInfo.lpBase)[_wrdBack].Next = _nxtFore;

			if(_nxtBack == _wrdFore)
			{
				(gFTInfo.lpBase)[_wrdFore].Next = _wrdBack;
				(gFTInfo.lpBase)[_preFore].Dirty |= FTUBIT_DIRTY_NEXT;
			}
			else
			{
				(gFTInfo.lpBase)[_wrdFore].Next = _nxtBack;
				(gFTInfo.lpBase)[_wrdFore].Dirty |= FTUBIT_DIRTY_NEXT;

				(gFTInfo.lpBase)[_preFore].Next = _wrdBack;
				(gFTInfo.lpBase)[_preFore].Dirty |= FTUBIT_DIRTY_NEXT;
			}

			return TRUE;
		}
		else
			return FALSE;
	}
	else
	{
		CORE_ASSERT(0);
		return FALSE;
	}
}

//...................................................................
//Description:
//  The function insert a child tree which own to wrdParent_ after the node wrdInsert_
//Note:
//  Can not insert a parent tree to its child tree !!!
//  Can not insert a tree to itself !!!
//
//Entries:
//WRD wrdParent_        the parent node for the child tree
//WRD wrdInsert_        the insert point which the child tree insert to
//...................................................................
_DLLEXP BOOL _DLLFUNC FaceInsertTree( WRD wrdParent_, WRD wrdInsert_ )
{
	WRD     _wrdChild, _wrdChildLast;

	if( (gFTInfo.lpBase)[wrdParent_].Child == 0 )
		return FALSE;   //no child tree

	_wrdChild = (gFTInfo.lpBase)[wrdParent_].Child ;
	_wrdChildLast = FaceGetLast(_wrdChild);

	(gFTInfo.lpBase)[_wrdChildLast].Next = (gFTInfo.lpBase)[wrdInsert_].Next;
	(gFTInfo.lpBase)[_wrdChildLast].Dirty |= FTUBIT_DIRTY_NEXT;

	(gFTInfo.lpBase)[wrdInsert_].Next = _wrdChild;
	(gFTInfo.lpBase)[wrdInsert_].Dirty |= FTUBIT_DIRTY_NEXT;

	(gFTInfo.lpBase)[wrdParent_].Child = 0;
	(gFTInfo.lpBase)[wrdParent_].Dirty |= FTUBIT_DIRTY_CHILD;

	return TRUE;
}

//...................................................................
//...................................................................
_DLLEXP BOOL _DLLFUNC FaceInsertParamGroup( WRD wrd_, WORD wInsertBefore_ )
{
	LPFTU   _lpFtu = gFTInfo.lpBase + wrd_;
	WORD    _wNumInsert, _i;

	if(  (wrd_ >= gFTInfo.wAdding)
	   ||( !(_lpFtu->Status & FTUBIT_VALID) )
	   ||( wInsertBefore_ >= _lpFtu->NumParam ) )
	{
		CORE_ASSERT( FALSE );
		return FALSE;
	}

	_wNumInsert = (WORD)StampCallParamProc( _lpFtu->Stamp, PIC_CANINSERT,
							   wrd_, wInsertBefore_, 0 );
	if( !_wNumInsert )
		return FALSE;

	if( !FaceInsertParam( wrd_, wInsertBefore_, _wNumInsert ) )
		return FALSE;

	for(_i=0; _i<_wNumInsert; _i++ )
	{
		StampCallParamProc( _lpFtu->Stamp, PIC_INITIALIZEPARAM,
					   wrd_, (WORD)(wInsertBefore_ + _i), 0 );
	}
	return TRUE;
}

//...................................................................
//Note:
//  This function only allocate the memory and reset the parameter
//  pointer in FTU, don't initialize the new parameters;
//...................................................................
_DLLEXP BOOL _DLLFUNC FaceInsertParam( WRD wrd_, WORD wInsertBefore_, WORD wNumInsert_ )
{
	LPPU    _lpPrmNew;
	LPFTU   _lpFtu = gFTInfo.lpBase + wrd_;

	CORE_ASSERT( wNumInsert_ );
	if(  (wrd_ >= gFTInfo.wAdding)
	   ||( !(_lpFtu->Status & FTUBIT_VALID) )
	   ||( wInsertBefore_ >= _lpFtu->NumParam ) )
	{
		CORE_ASSERT( FALSE );
		return FALSE;
	}

	_lpPrmNew = ParamAlloc( (_lpFtu->NumParam + wNumInsert_) );
	if( !_lpPrmNew )
		return FALSE;

	memmove( _lpPrmNew,
			 _lpFtu->lpParam,
			 sizeof(PU)* wInsertBefore_ );

	memmove( _lpPrmNew + wInsertBefore_ + wNumInsert_,
			 _lpFtu->lpParam + wInsertBefore_,
			 sizeof(PU)* (_lpFtu->NumParam - wInsertBefore_) );

	FreeParameter( _lpFtu->lpParam, _lpFtu->NumParam );   //at first, free the old param block
	_lpFtu->lpParam = _lpPrmNew;
	_lpFtu->NumParam = _lpFtu->NumParam + wNumInsert_;
	_lpFtu->Dirty = FTUBIT_DIRTY_ALL;

	return TRUE;
}

//...................................................................
//...................................................................
_DLLEXP BOOL _DLLFUNC FaceAppendParamGroup( WRD wrd_ )
{
	LPFTU   _lpFtu = gFTInfo.lpBase + wrd_;
	WORD    _wNumParamOld;
	WORD    _wNumAppend;
	int     _i;

	if(  (wrd_ >= gFTInfo.wAdding)
	   ||( !(_lpFtu->Status & FTUBIT_VALID) ) )
	{
		CORE_ASSERT( FALSE );
		return FALSE;
	}

	_wNumAppend = (WORD)StampCallParamProc( _lpFtu->Stamp, PIC_CANAPPEND,
							   wrd_, 0, 0 );
	if( !_wNumAppend )
		return FALSE;

	_wNumParamOld = FaceGetNumParam( wrd_ );
	if( !FaceAppendParam( wrd_, _wNumAppend ) )
		return FALSE;

	for(_i=0; _i<_wNumAppend; _i++ )
	{
		StampCallParamProc( _lpFtu->Stamp, PIC_INITIALIZEPARAM,
					   wrd_, (WORD)(_wNumParamOld + _i), 0 );
	}
	return TRUE;
}

//...................................................................
//Note:
//  This function only allocate the memory and reset the parameter
//  pointer in FTU, don't initialize the new parameters;
//...................................................................
_DLLEXP BOOL _DLLFUNC FaceAppendParam( WRD wrd_, WORD wNumAppend_ )
{
	LPPU    _lpPrmNew;
	LPFTU   _lpFtu = gFTInfo.lpBase + wrd_;

	CORE_ASSERT( wNumAppend_ );
	if(  (wrd_ >= gFTInfo.wAdding)
	   ||( !(_lpFtu->Status & FTUBIT_VALID) ) )
	{
		CORE_ASSERT( FALSE );
		return FALSE;
	}

	_lpPrmNew = ParamAlloc( (_lpFtu->NumParam + wNumAppend_) );
	if( !_lpPrmNew )
		return FALSE;

	memmove( _lpPrmNew,
			 _lpFtu->lpParam,
			 sizeof(PU)* _lpFtu->NumParam );

	FreeParameter( _lpFtu->lpParam, _lpFtu->NumParam );   //at first, free the old param block
	_lpFtu->lpParam = _lpPrmNew;
	_lpFtu->NumParam = _lpFtu->NumParam + wNumAppend_;
	_lpFtu->Dirty = FTUBIT_DIRTY_ALL;

	return TRUE;
}

//...................................................................
//...................................................................
_DLLEXP BOOL _DLLFUNC FaceDeleteParamGroup( WRD wrd_, WORD wFirstDelete_ )
{
	LPFTU   _lpFtu = gFTInfo.lpBase + wrd_;
	WORD    _wNumDelete;

	if(  (wrd_ >= gFTInfo.wAdding)
	   ||( !(_lpFtu->Status & FTUBIT_VALID) ) )
	{
		CORE_ASSERT( FALSE );
		return FALSE;
	}

	_wNumDelete = (WORD)StampCallParamProc( _lpFtu->Stamp, PIC_CANDELETE,
							   wrd_, wFirstDelete_, 0 );

	if( !_wNumDelete )
		return FALSE;

	return FaceDeleteParam( wrd_, wFirstDelete_, _wNumDelete );
}

//...................................................................
//Note:
//  This function only delete the memory and reset the parameter
//  pointer in FTU;
//...................................................................
_DLLEXP BOOL _DLLFUNC FaceDeleteParam( WRD wrd_, WORD wFirstDelete_, WORD wNumDelete_ )
{
	LPFTU   _lpFtu = gFTInfo.lpBase + wrd_;
	WORD    _wFirstRest;
	int     _rests;
	LPPU    _lpPrmNew;

	CORE_ASSERT( wNumDelete_ );

	if(  (wrd_ >= gFTInfo.wAdding)
	   ||( !(_lpFtu->Status & FTUBIT_VALID) ) )
	{
		CORE_ASSERT( FALSE );
		return FALSE;
	}

	_wFirstRest = wNumDelete_ + wFirstDelete_;
	_rests = _lpFtu->NumParam - _wFirstRest;

	if( _rests < 0 )
	{
		CORE_ASSERT( FALSE );
		return FALSE;
	}

	_lpPrmNew = ParamAlloc( (_lpFtu->NumParam - wNumDelete_) );
	if( !_lpPrmNew )
		return FALSE;

	memmove( _lpPrmNew,
			 _lpFtu->lpParam,
			 sizeof(PU)* wFirstDelete_ );

	memmove( _lpPrmNew + wFirstDelete_,
			 _lpFtu->lpParam + _wFirstRest,
			 sizeof(PU)* _rests );

	FreeParameter( _lpFtu->lpParam, _lpFtu->NumParam );   //at first, free the old param block
	_lpFtu->lpParam = _lpPrmNew;
	_lpFtu->NumParam = wFirstDelete_ + _rests;
	_lpFtu->Dirty = FTUBIT_DIRTY_ALL;

	return TRUE;
}

//...................................................................
//Description:
//Note:
//  The old parameters is freed by this function;
//...................................................................
_DLLEXP BOOL _DLLFUNC FaceChangeParameter(WRD wrd_, WORD wNumParam_, LPPU lpPU_)
{
	LPFTU	_lpFace;

	if( wrd_ >= gFTInfo.wAdding )
	{
		CORE_ASSERT( FALSE );
		return FALSE;
	}

	_lpFace = gFTInfo.lpBase + wrd_;

	if( _lpFace->Status & FTUBIT_VALID )
	{
		FreeParameter( _lpFace->lpParam, _lpFace->NumParam );
		_lpFace->NumParam = wNumParam_;
		_lpFace->lpParam = lpPU_;

		return TRUE;
	}
	else
	{
		CORE_ASSERT( FALSE );
		return FALSE;
	}
}

//...................................................................
//Description:
//Note:
//  The old parameters is freed by this function;
//...................................................................
_DLLEXP BOOL _DLLFUNC FaceChangeStamp(WRD wrd_, IDSTAMP id_)
{
	LPFTU   _lpFtu;
	LPPU    _lpPrmNew;
	WORD    _wNumParamNew;
	COREREF _core_ref;

	FTU		_ftuOld, _ftuNew;

	_RETURN_FALSE_IF_INVALID(wrd_)

	_lpFtu = (gFTInfo.lpBase) + wrd_;

	if(  (wrd_ >= gFTInfo.wAdding)
	   ||( !(_lpFtu->Status & FTUBIT_VALID) ) )
	{
		CORE_ASSERT( FALSE );
		SET_LAST_ERROR( ERR_Invalid_Entries );
		return FALSE;
	}

	if( !IsStamp(id_) ) {
		SET_LAST_ERROR( ERR_Invalid_Stamp );
		return 0;   //illegal stamp. id;
	}

	memset( &_core_ref, 0, sizeof(_core_ref) );
	if( GetReference( TB_FT, wrd_, TRUE /*skip myself*/, &_core_ref ) )
	{
		if( _core_ref.wTable != TB_FT )
			return 0;	//the FTU is referenced by others
	}

	_wNumParamNew = StampGetNumParam( id_ );
	if(_wNumParamNew)
	{
		_lpPrmNew = ParamAlloc( _wNumParamNew );
		if(_lpPrmNew == NULL)
		{
			SET_LAST_ERROR( ERR_Frag_Allocate_Failure );
			return FALSE;
		}
	}
	else
		_lpPrmNew = NULL;

	//keep FTU head aside
	memcpy(&_ftuOld, _lpFtu, sizeof(FTU) );

	//replace it with the new settings
	_lpFtu->lpParam = _lpPrmNew;
	_lpFtu->NumParam = _wNumParamNew;
	_lpFtu->Stamp = id_;
	_lpFtu->Status = FTUBIT_VALID;
	_lpFtu->Dirty = FTUBIT_DIRTY_ALL;
	_lpFtu->UserData = NULL;

	//to initialize it
	if( !StampPICCreate( id_, wrd_, 0) )
	{
		//if failed, then recover it
		memcpy(_lpFtu, &_ftuOld, sizeof(FTU) );

		//release the memory for new paramters
		FreeParameter( _lpPrmNew, _wNumParamNew );
		return FALSE;
	}
	else
	{
		//backup the new head
		memcpy(&_ftuNew, _lpFtu, sizeof(FTU) );

		//temporarily change it to the old setting again
		memcpy(_lpFtu, &_ftuOld, sizeof(FTU) );

		//send PIC_DELETE message to the stamprithm
		StampPICDelete( _lpFtu->Stamp, wrd_ );
		FreeParameter( _lpFtu->lpParam, _lpFtu->NumParam );	

		//at the last, set the new head again
		memcpy(_lpFtu, &_ftuNew, sizeof(FTU) );

		return TRUE;
	}
}

/*
_DLLEXP BOOL _DLLFUNC FaceChangeStamp(WRD wrd_, IDSTAMP id_)
	old implementation 
{
	LPFTU   _lpFtu = (gFTInfo.lpBase + wrd_);
	LPPU    _lpPrmNew;
	WORD    _i, _wNumParam;

	if(  (wrd_ >= gFTInfo.wAdding)
	   ||( !(_lpFtu->Status & FTUBIT_VALID) ) )
	{
		CORE_ASSERT( FALSE );
		SET_LAST_ERROR( ERR_Invalid_Entries );
		return FALSE;
	}

	if( !IsStamp(id_) ) {
		SET_LAST_ERROR( ERR_Invalid_Stamp );
		return 0;   //illegal stamp. id;
	}

	_wNumParam = StampGetNumParam( id_ );
	if(_wNumParam)
	{
		_lpPrmNew = ParamAlloc( _wNumParam );
		if(_lpPrmNew == NULL)
		{
			SET_LAST_ERROR( ERR_Frag_Allocate_Failure );
			return FALSE;
		}
	}
	else
		_lpPrmNew = NULL;

	FreeParameter( _lpFtu->lpParam, _lpFtu->NumParam );
	_lpFtu->Stamp = id_;
	_lpFtu->NumParam = _wNumParam;
	_lpFtu->lpParam = _lpPrmNew;

	//after linking the new parameters to the FTU,
	//we initialize parameters here;
	for(_i=0; _i<_wNumParam; _i++)
		StampCallParamProc( id_, PIC_INITIALIZEPARAM, wrd_, _i, 0 );

	return TRUE;
}
*/

//...................................................................
//...................................................................
_DLLEXP WRD _DLLFUNC FaceGetLast(WRD wrd_)
{
	WRD _wNext = wrd_;

	while( (gFTInfo.lpBase + _wNext)->Next )
		_wNext = (gFTInfo.lpBase + _wNext)->Next;

	return _wNext;
}

//...................................................................
//...................................................................
_DLLEXP LPSTR _DLLFUNC FaceGetData(LPSTR lpsz_, int size_, WRD wrd_, WORD order_)
{
	PU					_pu;
	GETVALUESTRING		_gvs;
	DWORD				_ret;
	int					_i;

	CLEAR_LAST_ERROR;

	for(_i=0; _i<_NUM_NAMESET; _i++)
		if(order_ == ssNameSet[_i].element)
			break;

	if( _i >= _NUM_NAMESET )
	{
		CORE_ASSERT(0);
		return NULL;	//no found
	}

	//initialize the struction: _gvs
	_gvs.lpszBuffer = lpsz_;
	_gvs.wSize = size_;

	_pu.prefix = PF_TBX|TBX_FT|order_;
	_pu.du.zzk__WRD = wrd_;
	_ret = PUGetValueString( &_pu, &_gvs, ssNameSet[_i].vsof );

	return _ret? lpsz_: NULL;
}

//...................................................................
//...................................................................
_DLLEXP BOOL _DLLFUNC FaceSetData(LPCSTR lpsz_, WRD wrd_, WORD order_)
{
	PU			_pu;
	DWORD		_ret;
	
	int			_i;

	if(  ( wrd_ >= gFTInfo.wCount )
	   ||( !lpsz_ ) )
	{
		SET_LAST_ERROR( ERR_Invalid_Entries );
		CORE_ASSERT( FALSE );
		return FALSE;
	}

	CLEAR_LAST_ERROR;

	for(_i=0; _i<_NUM_NAMESET; _i++)
		if(order_ == ssNameSet[_i].element)
			break;

	if( _i >= _NUM_NAMESET )
	{
		CORE_ASSERT(0);
		return FALSE;	//no found
	}

	_pu.prefix = PF_TBX|TBX_FT|order_;
	_pu.du.zzk__WRD = wrd_;
	_ret = PUSetValueString( &_pu, lpsz_, ssNameSet[_i].vsof );

	return _ret? TRUE: FALSE;
}

//...................................................................
//Note:
//  This function should be used with care.
//
//  The function always sets changed parent DIRTY since it thinks
//  that face's parent should not be changed at run-time.
//...................................................................
_DLLEXP BOOL _DLLFUNC FaceSetParent(WRD wrd_, WRD wParent_)
{
	if( (gFTInfo.lpBase)[wrd_].Status & FTUBIT_VALID )
	{
		if( (gFTInfo.lpBase)[wParent_].Status & FTUBIT_VALID )
		{
			(gFTInfo.lpBase)[wrd_].Parent = wParent_;
			(gFTInfo.lpBase)[wrd_].Dirty |= FTUBIT_DIRTY_PARENT;
			return TRUE;
		}
		else
			return FALSE;
	}

	return FALSE;
}

//...................................................................
//...................................................................
_DLLEXP BOOL _DLLFUNC FacePaint(WRD wrd_, HDC hdc_)
{
	SRTPROC _lpfn = StampGetPaintProc( (gFTInfo.lpBase + wrd_)->Stamp );

	if( _lpfn )
		return (*_lpfn)( wrd_, hdc_ );
	else
		return FALSE;
}

//...................................................................
//Description:
//  If the Face is run-time dirty, returns a non-zero double word
//Return:
//  Returns the following bit or their combination.
//      HEAD_DIRTY          0x0001  //the head of FTU is dirty
//      POSITION_DIRTY      0x0002  //the postion of FTU is dirty
//      NORMAL_DIRTY        0x0004  //others parameters are dirty
//  if head or no parameters dirty, returns
//      NO_DIRTY            0x0000  //no dirty
//...................................................................
_DLLEXP WORD _DLLFUNC FaceDirty(WRD wrd_)
{
	WORD    _ret = NO_DIRTY;
	LPPU    _lppu;
	LPFTU   _lpFtu = gFTInfo.lpBase + wrd_;

	if( _lpFtu->Dirty & FTUBIT_DIRTY_ALL )
		_ret |= HEAD_DIRTY;

	_lppu = _lpFtu->lpParam;
	if( _lppu )
	{
		WORD    _i;
		for(_i=0;
			_i<_lpFtu->NumParam;
			_i++, _lppu++ )
		{
			if( (!IS_PU_IGNOREDIRTY(_lppu)) && PUIsDirty(_lppu) )
			{
				if( _i<FACE_FIRSTPARAM )
					_ret |= POSITION_DIRTY;
				else
				{
					_ret |= NORMAL_DIRTY;

					//need not do more test!
					return _ret;
				}
			}
		}
	}
	return _ret;
}

//...................................................................
//Description:
//  clear run-time dirty flags
//Return:
//  none
//...................................................................
_DLLEXP void _DLLFUNC FaceClearDirty(WRD wrd_)
{
	LPPU _lppu;
	LPFTU   _lpFtu = (gFTInfo.lpBase + wrd_);

	_lpFtu->Dirty &= (~FTUBIT_DIRTY_ALL);
	_lpFtu->Status &= (~FTUBIT_REDRAW);

	if( !(_lpFtu->Status & FTUBIT_VALID) )
	{  						
		//Don't check for legality, ref to CoreClearDirty()
		//CORE_ASSERT( FALSE );
		return;											
	}
	
	_lppu = _lpFtu->lpParam;
	if( _lppu )
	{
		WORD    _i;
		for(_i=0;
			_i<_lpFtu->NumParam;
			_i++, _lppu++ )
		{
			_lppu->flags &= (~PU_DIRTY);
		}
	}

	return ;
}

//...................................................................
//...................................................................
_DLLEXP WORD _DLLFUNC SetSelectHandleSize(WORD wSHSize_)
{
	WORD    _oldSHS = gwSHSize;

	gwSHSize = wSHSize_;
	return _oldSHS;
}

//...................................................................
//...................................................................
_DLLEXP WORD _DLLFUNC GetSelectHandleSize()
{
	return gwSHSize;
}

//////////////////////////////////////////////////////
//inner function

//...................................................................
//inner function
//Note:
//  This function should be used with care.
//...................................................................
void _near _FaceSetNext(WRD wrd_, WRD wNext_)
{
	(gFTInfo.lpBase)[wrd_].Next = wNext_;
	(gFTInfo.lpBase)[wrd_].Dirty |= FTUBIT_DIRTY_NEXT;
}

//...................................................................
//inner function
//Note:
//  This function should be used with care.
//...................................................................
void _near _FaceSetChild(WRD wrd_, WRD wChild_)
{
	(gFTInfo.lpBase)[wrd_].Child = wChild_;
	(gFTInfo.lpBase)[wrd_].Dirty |= FTUBIT_DIRTY_CHILD;
}

//...................................................................
//Description:
//	This function change the PU's data link safely.
//Note:
//if change from ... to ...:
//	NUM		->	NUM
//	REF		->	NUM
//					set the new value with the old value string
//	NUM		->	COBJ
//	REF		->	COBJ
//					clear the new cobj to NULL
//	NUM		->	REF
//	REF		->	REF
//	COBJ	->	COBJ
//					do nothing
//	COBJ	->	NUM
//	COBJ	->	REF
//					delete the old cobj first
//
//...................................................................
#define _IS_NUM		0x0001
#define _IS_COBJ	0x0002
#define _IS_REF		0x0003
_DLLEXP BOOL _DLLFUNC FaceChangeDataLink( WRD wrd_, WORD order_, LPCSTR lpszDataLink_ )
{
	//get the old value string first
	char				_szBuffer[512];
	PU					_puOld, _puNew;
	LPPU				_lpParam; 
	GETVALUESTRING		_gvs;
	DWORD				_ret_gvs;
	ENCODE				_encode;
	IDSTAMP				_idStamp = FaceGetStamp( wrd_ );
	WORD				_from, _to;

	_lpParam = FaceGetParameter( wrd_ );
	if( !_lpParam )
	{
		CORE_ASSERT( FALSE );
		return TRUE;
	}

	_gvs.lpszBuffer = _szBuffer;
	_gvs.wSize = sizeof(_szBuffer);
	_encode.lpszBuffer = (LPSTR)lpszDataLink_;
	_encode.wAccess = 0;

	memcpy( &_puOld, _lpParam + order_, sizeof(PU) );
	_ret_gvs = StampGetValueString(_idStamp, wrd_, order_, &_gvs);

	if( !StampEncode(_idStamp, wrd_, order_, &_encode) )
		return FALSE;

	memcpy( &_puNew, _lpParam + order_, sizeof(PU) );

	if( (_puOld.prefix & PF_MASK) == PF_IMM )
	{
		_from = _IS_NUM;

		if( _puOld.prefix == (PF_IMM|IMM_LPCOBJ) )
			_from = _IS_COBJ;
	}
	else
		_from = _IS_REF;

	if( (_puNew.prefix & PF_MASK) == PF_IMM )
	{
		_to = _IS_NUM;

		if( _puNew.prefix == (PF_IMM|IMM_LPCOBJ) )
			_to = _IS_COBJ;
	}
	else
		_to = _IS_REF;

	switch( MAKELONG(_from, _to) )
	{
		case MAKELONG( _IS_NUM, _IS_NUM ):
		case MAKELONG( _IS_REF, _IS_NUM ):
		{
			//set the new value with the old value string
			StampSetValueString(_idStamp, wrd_, order_, _szBuffer);
			break;
		}

		case MAKELONG( _IS_NUM, _IS_COBJ ):
		case MAKELONG( _IS_REF, _IS_COBJ ):
		{
			_lpParam[order_].du.zzk__LPCOBJ = NULL;
			break;
		}

		case MAKELONG( _IS_COBJ, _IS_NUM ):
		case MAKELONG( _IS_COBJ, _IS_REF ):
		{
			CobjFree( _puOld.du.zzk__LPCOBJ );
			break;
		}

		case MAKELONG( _IS_NUM, _IS_REF ):
		case MAKELONG( _IS_COBJ, _IS_COBJ ):
		case MAKELONG( _IS_REF, _IS_REF ):
			//do nothing here
			break;
	}

	return TRUE;
}


//...................................................................
//...................................................................
_DLLEXP BOOL _DLLFUNC PUReadFaceTable(SimplePU FAR* to_, LPPU from_)
{
	SimplePU 	_tmp;
	LPFTU		_lpFtu;

	if( (from_->du.zzk__WRD == 0) || (from_->du.zzk__WRD >= gFTInfo.wAdding) )
		return FALSE;

	_lpFtu = gFTInfo.lpBase + from_->du.zzk__WRD;

	switch(from_->prefix & 0x00FF)
	{
		case FTU_IDSTAMP:
			_tmp.prefix = PF_IMM|IMM_IDSTAMP;
			_tmp.du.zzk__IDSTAMP = _lpFtu->Stamp;
			break;

		case FTU_STATUS:
			_tmp.prefix = PF_IMM|IMM_STATUS;
			_tmp.du.zzk__STATUS = _lpFtu->Status;
			break;

		case FTU_POSITION:
			_tmp.prefix = PF_IMM|IMM_FPTR;
			_tmp.du.zzk__FPTR = &(_lpFtu->Rect);
			break;

		case FTU_PARENT:
			_tmp.prefix = PF_IMM|IMM_WRDFTU;
			_tmp.du.zzk__WRD = _lpFtu->Parent;
			break;

		case FTU_CHILD:
			_tmp.prefix = PF_IMM|IMM_WRDFTU;
			_tmp.du.zzk__WRD = _lpFtu->Child;
			break;

		case FTU_NEXT:
			_tmp.prefix = PF_IMM|IMM_WRDFTU;
			_tmp.du.zzk__WRD = _lpFtu->Next;
			break;

		case FTU_PARAM:
			_tmp.prefix = PF_IMM|IMM_FPTR;
			_tmp.du.zzk__FPTR = _lpFtu->lpParam;
			break;

		default:
			return FALSE;
	}

	return PUCast(to_, &_tmp);
}

//...................................................................
//...................................................................
_DLLEXP BOOL _DLLFUNC PUWriteFaceTable(LPPU to_, SimplePU FAR* from_)
{
	return FALSE;
}

//...................................................................
//...................................................................
_DLLEXP BOOL _DLLFUNC PUFaceDirty( LPPU lppu_ )
{
	int		_i;
	WORD	_ele;

	CORE_ASSERT( ((lppu_->prefix) & TBX_MASK) == TBX_FT );
	if( lppu_->du.zzk__WRD  >= gFTInfo.wAdding )
	{
		CORE_ASSERT( FALSE );
		return FALSE;
	}

	_ele = lppu_->prefix & XT_MASK;

	for(_i=0; _i<_NUM_NAMESET; _i++)
		if( _ele == ssNameSet[_i].element )
		{
			return (gFTInfo.lpBase + lppu_->du.zzk__WRD)->Dirty & ssNameSet[_i].dirty;
		}

	CORE_ASSERT( FALSE );	//should NOT go here
	return FALSE;	//no found
}

//...................................................................
//Entries:
//	bPublicDirty_
//		If this is TRUE, the private dirty bits are excluded.
//...................................................................
_DLLEXP BOOL _DLLFUNC IsFaceTableDirty( BOOL bPublicDirty_ )
{
	LPFTU	_lpFtu;
	LPPU	_lppu;
	DIRTY	_mask = FTUBIT_DIRTY_ALL;
	WRD		_wrd;

	if( bPublicDirty_ )
		_mask &= ~( FTUBIT_DIRTY_DIRTY|FTUBIT_DIRTY_POSITION );

	for(_wrd=1; _wrd<gFTInfo.wAdding; _wrd++)
	{
		_lpFtu = (gFTInfo.lpBase) + _wrd;
		_lppu = _lpFtu->lpParam;

		if( _lpFtu->Dirty & _mask )
			return TRUE;

		_lppu = _lpFtu->lpParam;
		if( _lppu )
		{
			WORD    _i;
			for(_i=0;
				_i<_lpFtu->NumParam;
				_i++, _lppu++ )
			{
				if( _lppu->flags & PU_DIRTY )
					return TRUE;
			}
		}	//end if
	}//end for

	return FALSE;
}

//...................................................................
//Entries:
//Note:
//	This implementation does not check the visibility for a face window
//...................................................................
_DLLEXP BOOL _DLLFUNC IsFaceVisible( WRD wFace_ )
{
	BOOL	_visible = FALSE;
	LPPU	_lpPrm;
	WRD		_wrd;

	CORE_ASSERT( FaceIsValid( wFace_ ) );

	_wrd = wFace_;
	while( TRUE )
	{
		if( !_wrd || 
			(_wrd == FACE_ROOT) ||
			(FaceGetStamp(_wrd) == (IDSTAMP)IDS_FACEWND ) )
			return TRUE;

		_lpPrm = FaceGetParameter( _wrd );

		MFPUREAD( _lpPrm, FACE_VISIBLE, BOOL, _visible );
		if( !_visible )
			return FALSE;

		_wrd = FaceGetParent( _wrd );
	}

}
