//===================================================================
//
//Zheng zhikai  95-10-4
//===================================================================
#include "prehead.h"

#include "core.h"
#include "cpu.h"
#include "cobj.h"
#include "corerw.h"
#include "csysobj.h"
#include "cerror.h"
#include "cmodu.h"
#include "calgo.h"

#define _RETURN_FALSE_IF_INVALID(_wrd_)							\
	if( !((gMTInfo.lpBase)[_wrd_].Status & MTUBIT_VALID) )		\
	{  															\
		CORE_ASSERT( FALSE );									\
		SET_LAST_ERROR( ERR_Invalid_MTU );						\
		return FALSE;											\
	}															


#define _MTU_SET_DIRTY( _ELE_ )									\
		{_lpMtu->Dirty |= MTUBIT_DIRTY_##_ELE_;}


static WRD ModuGetFreeRoom(void);
static void ModuReleaseRoom(WRD _wrd);

//local functions for module paste
static void _ModuAdjustReferenceTree( WRD wrdDes_, WRD wrdSrc_, WRD wrdRootDes_, WRD wrdRootSrc_ );
static WRD _ModuPasteTree( WRD wrdParent_, WRD wrdCopy_ );
static WRD _ModuPaste( WRD wrdParent_, WRD wrdCopy_ );
static BOOL _ModuSynRef( WRD wrdRootSrc_, WRD wrdRefSrc_, WRD wrdRootDes_, WRD* pwrdRefDes_ );
static void _DefaultNewTag( LPSTR lpszDstTag_, LPCSTR lpszSrcTag_ );

static BOOL _Modu_delete(WRD wrd_);
static BOOL _ModuTreeCheckReference( WRD wrdRoot_, WRD wrdTest_ );

void _near _ModuSetChild(WRD wrd_, WRD wrdChild_);
void _near _ModuSetNext(WRD wrd_, WRD wrdNext_);

extern MTInfo       gMTInfo;
extern BOOL         gbRunTime;

extern DWORD        gdwLastError;
extern CoreInfo		gCoreInfo;

struct {
	WORD    element;
	char    szName[16];
	VSOF	vsof;
	DIRTY	dirty;	//dirty mask
	WORD    type;
} static ssNameSet[] =
{
	//public writeable data 
	{MTU_TAG,        "Tag",      VSOF_LPCOBJ_STR,	MTUBIT_DIRTY_TAG,		TET_PRIMARY },
	{MTU_COMMENT,    "Comment",  VSOF_LPCOBJ_STR,	MTUBIT_DIRTY_COMMENT,	TET_PRIMARY },
	//public readonly data
	{MTU_IDALGO,     "Algo",     VSOF_IDALGO,		MTUBIT_DIRTY_IDALGO,	TET_PRIMARY|TET_READONLY },
	{MTU_NUMPARAM,   "NumParam", VSOF_WORD,			MTUBIT_DIRTY_NUMPARAM,	TET_PRIMARY|TET_READONLY },
	{MTU_PARENT,     "Parent",   VSOF_WRDMTU,		MTUBIT_DIRTY_PARENT,	TET_PRIMARY|TET_READONLY },
	{MTU_NEXT,       "Next",     VSOF_WRDMTU,		MTUBIT_DIRTY_NEXT,		TET_PRIMARY|TET_READONLY },
	{MTU_CHILD,      "Child",    VSOF_WRDMTU,		MTUBIT_DIRTY_CHILD,		TET_PRIMARY|TET_READONLY },
	//private data
	{MTU_STATUS,     "Status",   VSOF_STATUS_MTU,	MTUBIT_DIRTY_STATUS,	TET_PRIMARY|TET_READONLY|TET_PRIVATE },
	{MTU_PARAM,      "Param",    VSOF_FPTR,			MTUBIT_DIRTY_PARAM,		TET_PRIMARY|TET_READONLY|TET_PRIVATE },
	{MTU_DIRTY,      "Dirty",    VSOF_DIRTY_MTU,	MTUBIT_DIRTY_DIRTY,		TET_PRIMARY|TET_READONLY|TET_PRIVATE },
};
#define _NUM_NAMESET	( sizeof(ssNameSet)/sizeof(ssNameSet[0]) )

//...................................................................
//Description:
//  Creates a modu-unit in Modu Table as the last unit at
//  the modu-link owned by wParant_.
//Note:
//  Because always needs wParent_, the function can't create the
//  first Modu("Root") in modu table. the "Root" is added
//  directly to the table(this is done in InitCore(...) ).
//Entry:
//  LPCSTR  szTag_
//      Points to a string specifying the modu tag. if szTag_
//      begins with '#', it value is ignore and the effect is
//      as the same as szTag_ == NULL.
//  WRD     wParent_
//      Identifies the parent node of the unit to be allocated.
//      If wParant_ is NULL, returns NULL.
//  IDALGO  idAlgo_
//      Specifies the algorithm id.
//  LONG    lData_
//      Calller specifies data for passing to AlgoPICCreate()
//
//Returns:
//      the subscription of modu-unit if the function succeeds.
//      Otherwise, it is NULL.
//...................................................................
_DLLEXP WRD _DLLFUNC ModuCreate( LPCSTR    lpszTag_,
							WRD     wParent_,
							IDALGO  idAlgo_,
							LONG    lData_)
{
	LPMTU       _ptu;
	WRD         _i;
	WRD         _wMe, _wBrother;
	LPPU        _lpPrm = NULL;
	LPCOBJ      _lpCobj = NULL;
	BOOL        _bOk;
	WORD        _wNumParam;

	CLEAR_LAST_ERROR;

	if( !gMTInfo.lpBase )
		return 0;

	if( !IsAlgo(idAlgo_) )
	{
		SET_LAST_ERROR( ERR_Invalid_Algo );
		return 0;   //illegal algo. id;
	}

	//check the legality of the wParant_
	if(  (wParent_ >= gMTInfo.wAdding) || 
	   (((gMTInfo.lpBase)[wParent_].Status & MTUBIT_VALID) == 0) )
	{
		SET_LAST_ERROR( ERR_Invalid_Parent_Node );
		return 0;
	}

	if(lpszTag_ && (*lpszTag_) )        //otherwise szTag_ is NULL string
	{
		//do not use ModuCheckTag to check for legality of
		//tag because it is slight difference with the function.
		if( !IsTagLegal(lpszTag_) )
			return 0;

		//check on whether it's an unique tag in the modu-table
		for(_i=1; _i<gMTInfo.wAdding; _i++)
		{
			LPMTU   _lpMtu;

			_lpMtu = gMTInfo.lpBase + _i;

			if( pMODU_GET_TAG(_lpMtu) == NULL )
				continue;

			if( CmpTag( pMODU_GET_TAG(_lpMtu), lpszTag_, CORE_MAX_TAG_LENGTH ) == 0 )
			{
				SET_LAST_ERROR( ERR_Not_Unique_Tag );
				return 0;
			}
		}

		_lpCobj = AllocTag( lpszTag_, &_bOk );
		if( !_bOk )
			return 0;
	}
	else
		_lpCobj = NULL;

	_wMe = ModuGetFreeRoom();
	if( !_wMe ) return 0; //if no free room

	_wNumParam = AlgoGetNumParam( idAlgo_ );

	if(_wNumParam)
	{
		_lpPrm = ParamAlloc( _wNumParam );
		if(_lpPrm == NULL)
		{
			SET_LAST_ERROR( ERR_Frag_Allocate_Failure );
			goto _BAD;
		}
	}

	_ptu = gMTInfo.lpBase + _wMe;
	memset( _ptu, 0, sizeof(MTU) ); //clear the new unit

	//copy data
	_ptu->lpTag = _lpCobj;
	_ptu->Algo = idAlgo_;
	_ptu->NumParam = _wNumParam;
	_ptu->lpParam = _lpPrm;
	_ptu->Status = MTUBIT_VALID;
	_ptu->Dirty = MTUBIT_DIRTY_ALL;
	_ptu->Parent = wParent_;

	//update the modu tree
	_wBrother = (gMTInfo.lpBase)[wParent_].Child;
	if(_wBrother == NULL)
	{
		_ModuSetChild( wParent_, _wMe);
	}
	else
	{
		while( (gMTInfo.lpBase)[_wBrother].Next )
			_wBrother = (gMTInfo.lpBase)[_wBrother].Next;

		CORE_ASSERT( (gMTInfo.lpBase)[_wBrother].Parent == wParent_ );
		CORE_ASSERT( (gMTInfo.lpBase)[_wBrother].Next == NULL );

		_ModuSetNext(_wBrother, _wMe);
	}

	if( !AlgoPICCreate( idAlgo_, _wMe, lData_) )
	{
		ModuDelete( _wMe );
		return 0;
	}

	DispatchCoreMessage( TBX_MT );

	return _wMe;

_BAD:
	if(_lpPrm)
		FreeParameter(_lpPrm, _wNumParam );
	if(_lpCobj)
		CobjFree(_lpCobj);
	ModuReleaseRoom( _wMe );
	return 0;
}

//...................................................................
//Description:
//  Create a temp. modu by copying from wrd_
//...................................................................
_DLLEXP WRD _DLLFUNC ModuCopyTemp( WRD wrd_ )
{
	LPMTU       _lpMtuTmp, _lpMtuSrc;
	WRD         _wMe;
	LPPU        _lpPrmTmp = NULL;
	LPCOBJ      _lpCobjTmpTag = NULL;
	LPCOBJ      _lpCobjTmpComment = NULL;
	BOOL        _bOk;

	CLEAR_LAST_ERROR;

	_wMe = ModuGetFreeRoom();
	if( !_wMe ) return 0; //if no free room

	_lpMtuSrc = gMTInfo.lpBase + wrd_;

	//duplicate the tag, and comment
	_lpCobjTmpTag = CobjCopy( _lpMtuSrc->lpTag, &_bOk );
	if( !_bOk )	goto _BAD;
	_lpCobjTmpComment = CobjCopy( _lpMtuSrc->lpComment, &_bOk );
	if( !_bOk )	goto _BAD;

	//do not check for legality of the tag
	//do not check for legality of the algo
	//do not check for legality of the wParant

	if(_lpMtuSrc->NumParam)
	{
		WORD	_i;

		_lpPrmTmp = ParamAlloc( _lpMtuSrc->NumParam );
		if(_lpPrmTmp == NULL)
		{
			SET_LAST_ERROR( ERR_Frag_Allocate_Failure );
			goto _BAD;
		}

		memcpy(_lpPrmTmp, _lpMtuSrc->lpParam, _lpMtuSrc->NumParam * sizeof(PU) );
		//increase the reference counter of all cobjects
		for( _i=0; _i<_lpMtuSrc->NumParam; _i++ )
		{
			if( (_lpPrmTmp[_i]).prefix == (PF_IMM|IMM_LPCOBJ) )
				CobjLock( (_lpPrmTmp[_i]).du.zzk__LPCOBJ );
		}
	}
	else
	{
		CORE_ASSERT(!_lpMtuSrc->lpParam);
		_lpPrmTmp = NULL;
	}

	_lpMtuTmp = (gMTInfo.lpBase) + _wMe;

	//copy old data
	memcpy( _lpMtuTmp, _lpMtuSrc, sizeof(MTU) );    //clear the new unit

	//update new data
	_lpMtuTmp->lpParam = _lpPrmTmp;
	_lpMtuTmp->lpTag = _lpCobjTmpTag;
	_lpMtuTmp->lpComment = _lpCobjTmpComment;
	_lpMtuTmp->Status |= MTUBIT_VALID|MTUBIT_TEMP;

	//NOTE: do NOT update modu tree link

	//Note:
	//Do NOT call AlgoPICCreate(...) to initialize the new modu
	//because that function will reset the parameters of the meter

	return _wMe;

_BAD:
	CobjFree( _lpCobjTmpTag );
	CobjFree( _lpCobjTmpComment );

	if( _lpPrmTmp )
		FreeParameter( _lpPrmTmp, _lpMtuSrc->NumParam );

	ModuReleaseRoom( _wMe );
	return FALSE;
}

//...................................................................
//Description:
//...................................................................
_DLLEXP BOOL _DLLFUNC ModuDeleteTemp(WRD wrd_)
{
	LPMTU   _lpModu = &((gMTInfo.lpBase)[wrd_]);

	CLEAR_LAST_ERROR;

	if(   (_lpModu->Status & MTUBIT_VALID)
	   && (_lpModu->Status & MTUBIT_TEMP) )
	{
		CobjFree( _lpModu->lpTag );
		_lpModu->lpTag = NULL;

		CobjFree( _lpModu->lpComment );
		_lpModu->lpComment = NULL;

		//free paramters memory resource
		if( _lpModu->lpParam )
			FreeParameter( _lpModu->lpParam, _lpModu->NumParam );

		ModuReleaseRoom(wrd_);

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
_DLLEXP BOOL _DLLFUNC ModuDelete(WRD wrd_)
{
	BOOL	_ret;

	CLEAR_LAST_ERROR;

	if( !ModuIsValid(wrd_) )
	{
		CORE_ASSERT( FALSE );
		return FALSE;
	}

	if( wrd_ == MODU_ROOT )
	{
		CORE_ASSERT( FALSE );
		return FALSE;
	}

	if( _ModuTreeCheckReference( wrd_, wrd_ ) )
	{
		SET_LAST_ERROR( ERR_Referenced_MTU );
		return FALSE;   //the MTU is referenced by other xTUs
	}

	_ret = _Modu_delete( wrd_ );
	if( _ret ) 
		DispatchCoreMessage( TBX_MT );

	return _ret;
}

//...................................................................
//Description:
//	This is an inner function for deleting modu;
//...................................................................
static BOOL _Modu_delete(WRD wrd_)
{
	WRD     _wrdPrev;
	LPMTU   _lpModu = gMTInfo.lpBase + wrd_;

	/////////////////////////
	//free its children first

	while( _lpModu->Child )
		_Modu_delete( _lpModu->Child );

	/////////////////////////
	//then free self

	AlgoPICDelete( _lpModu->Algo, wrd_ );

	//property tree structure
	_wrdPrev = ModuGetPrevious(wrd_);
	if( _wrdPrev == NULL )  //this is the first child
	{
		//whether there are other children or not, this code is correct.
		_ModuSetChild(_lpModu->Parent, _lpModu->Next);
	}
	else
		_ModuSetNext(_wrdPrev, _lpModu->Next);

	//free paramters memory resource
	if(_lpModu->lpParam)
	{
		FreeParameter( _lpModu->lpParam, _lpModu->NumParam );
		_lpModu->lpParam = NULL;
	}

	_lpModu->Status &= (~MTUBIT_VALID);  //clear VALID bit
	_lpModu->Dirty |= MTUBIT_DIRTY_ALL;

	CobjFree( _lpModu->lpTag );
	_lpModu->lpTag = NULL;

	CobjFree( _lpModu->lpComment );
	_lpModu->lpComment = NULL;

	return TRUE;
}

//...................................................................
//Description:
//	This function checks if modus in a modu-tree: wrdTest_, are
//	referenced by the modu which is out of the tree: wrdRoot_;
//...................................................................
static BOOL _ModuTreeCheckReference( WRD wrdRoot_, WRD wrdTest_ )
{
	COREREF _core_ref;
	WRD		_wrdChild;

	memset( &_core_ref, 0, sizeof(_core_ref) );

_AGAIN:
	if( GetReference( TB_MT, wrdTest_, TRUE /*skip myself*/, &_core_ref ) )
	{
		if( _core_ref.wTable != TB_MT )
			return TRUE;
		else
		{
			//check if the referenced modu is in this tree
			if( !ModuIsInTree( wrdRoot_, _core_ref.wrd ) )
				return TRUE;
		}

		//continue to search
		goto _AGAIN;
	}

	for( _wrdChild = ModuGetChild( wrdTest_ );
		 _wrdChild;
		 _wrdChild = ModuGetNext( _wrdChild ) )
	{
		if( _ModuTreeCheckReference( wrdRoot_, _wrdChild ) )
			return TRUE;
	}

	return FALSE;
}

//...................................................................
//Description:
//	This function checks if a modu is in a given tree
//...................................................................
_DLLEXP BOOL _DLLFUNC ModuIsInTree( WRD wrdRoot_, WRD wrdTest_ )
{
	do
	{
		if( wrdTest_ == wrdRoot_ )
			return TRUE;
		wrdTest_ = ModuGetParent( wrdTest_ );
	} while( wrdTest_ );

	return FALSE;
}

//...................................................................
//Description:
//  Undo a modu specified by wModu_ using its undo buffer: wrdBuffer_
//and then Delete the undo buffer modu.
//...................................................................
_DLLEXP BOOL _DLLFUNC ModuUndo( WRD wrdModu_, WRD wrdBuffer_ )
{
	LPMTU   _lpModu, _lpBuffer;

	if( !wrdModu_ || !wrdBuffer_ )
	{
		SET_LAST_ERROR( ERR_Invalid_Entries );
		return FALSE;
	}
	_lpModu = gMTInfo.lpBase + wrdModu_;
	_lpBuffer = gMTInfo.lpBase + wrdBuffer_;

	if( _lpModu->lpParam )     //free the dirty parameters
		FreeParameter( _lpModu->lpParam, _lpModu->NumParam );

	//free the dirty tag, and comment
	CobjFree( _lpModu->lpTag );
	CobjFree( _lpModu->lpComment );

	//undo the parameters of the meter and other elements
	//NOTE: the tag and parameters of the undo buffer meter
	//      is sent to the wrdModu_
	memcpy( _lpModu, _lpBuffer, sizeof( MTU) );
	_lpModu->Status &= (~MTUBIT_TEMP);    //clear TEMP bit

	//delete the undo buffer MTU
	memset( _lpBuffer, 0, sizeof(MTU) );
	ModuReleaseRoom( wrdBuffer_ );

	return TRUE;
}

//...................................................................
//Description:
//  This function only checks the MTUBIT_VALID bit;
//...................................................................
_DLLEXP BOOL _DLLFUNC ModuIsValid( WRD wrd_ )
{
	if(  ( wrd_ < gMTInfo.wCount )
	   &&( (gMTInfo.lpBase + wrd_)->Status & MTUBIT_VALID ) )
		return TRUE;
	else 
		return FALSE;
}

//...................................................................
//Description:
//  Checks the legality of a MTU even that is invalid, including:
//      . legality of szTag naming (ERROR CODE: MTU_ERR_TAGNAMING)
//      . a unique of tag in the table (ERROR CODE: MTU_ERR_TAGUNIQUE)
//      . legality of Algo (ERROR CODE: MTU_ERR_ALGO)
//      . legality of the wParant (ERROR CODE: MTU_ERR_PARENT)
//
//Entry:
//  MTU FAR* lp_
//      Points to the MTU to be checked. if lp_ == NULL, returns
//      FALSE and no error code.
//Return:
//      if Modu is legality, returns TRUE; otherwise, returns
//      FALSE.
//...................................................................
_DLLEXP BOOL _DLLFUNC ModuCheckLegality(WRD wrd_)
{
	LPMTU _lp = gMTInfo.lpBase + wrd_;

	CLEAR_LAST_ERROR;
	_RETURN_FALSE_IF_INVALID(wrd_)

	if( !ModuCheckTag(wrd_) )
		return FALSE;

	//the algorithm no found
	if( !IsAlgo(_lp->Algo) )
	{
		SET_LAST_ERROR( ERR_Invalid_Algo );
		return FALSE;
	}

	//check the legality of the wParant_
	if(  ( ((gMTInfo.lpBase)[_lp->Parent].Status & MTUBIT_VALID) == 0 )
	   ||(_lp->Parent >= gMTInfo.wAdding) )
	{
		if( wrd_ != MODU_ROOT )   //MODU_ROOT has no parent node
		{
			SET_LAST_ERROR( ERR_Invalid_Parent_Node );
			return FALSE;
		}
	}

	//add the following check
	//  . the number of parameter
	//  . the datalink legality
	//those checks should be fulfilled by the corresponding algorithm
	if( !AlgoCallParamProc( _lp->Algo, PIC_CHECKPARAM, wrd_, 0, 0 ) )
		return FALSE;

	return TRUE;
}

//...................................................................
//Description:
//Entry:
//Return:
//...................................................................
_DLLEXP BOOL _DLLFUNC ModuCheckTag(WRD wModu_)
{
	LPMTU       _lp = gMTInfo.lpBase + wModu_;
	WRD         _i;

	CLEAR_LAST_ERROR;

	if( pMODU_GET_TAG(_lp) == NULL )
		return TRUE;

	if( !IsTagLegal( pMODU_GET_TAG(_lp) ) )
		return FALSE;

	//check on whether it's an unique tag in the modu-table
	for(_i=1; _i<gMTInfo.wAdding; _i++)
	{
		LPMTU   _lpMtu;

		_lpMtu = gMTInfo.lpBase + _i;

		if(  !(_lpMtu->Status & MTUBIT_VALID)
		   ||(_lpMtu->Status & MTUBIT_TEMP) )
			continue;

		//check for whether itself
		if( _lpMtu == _lp)
			continue;

		if( !CmpTag( pMODU_GET_TAG(_lpMtu), pMODU_GET_TAG(_lp), CORE_MAX_TAG_LENGTH ) )
		{
			SET_LAST_ERROR( ERR_Not_Unique_Tag );
			return FALSE;
		}
	}

	return TRUE;
}

//...................................................................
//Description:
//Entry:
//Return:
//...................................................................
_DLLEXP WRD _DLLFUNC ModuGetWRD(LPCSTR szTag_)
{
	WRD _i;

	if(*szTag_ == '#')
	{
		WRD _wrd = TagToWRD(szTag_);
		if( _wrd == NULL)
			return 0;
		else
		{
			if( (gMTInfo.lpBase[_wrd]).Status & MTUBIT_VALID )
				return _wrd;
			else
				return 0;
		}
	}

	for(_i=1; _i<gMTInfo.wAdding; _i++)
	{
		if( !CmpTag( MODU_GET_TAG(_i),
					 szTag_,
					 CORE_MAX_TAG_LENGTH) )
			return _i;
	}

	return 0;
}

//...................................................................
//Description:
//Entry:
//Return:
//	0:			failed, no name returns
//	1:			successful with real name
//	2:			successful with pseudo name
//Note:
//  ONLY valid MTU has a tag!
//...................................................................
_DLLEXP int _DLLFUNC ModuGetTag(WRD wrd_, LPSTR szTag_, WORD wSize_)
{
	int		_ret = 0;

	CLEAR_LAST_ERROR;

	if( wrd_ >= gMTInfo.wCount )
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
		LPMTU 	_lpMtu;
		_lpMtu = gMTInfo.lpBase + wrd_;

		if( _lpMtu->Status & MTUBIT_VALID )
		{
			if( (_lpMtu->lpTag) && *(_lpMtu->lpTag->data) )
			{
				StringCopyn( szTag_, pMODU_GET_TAG(_lpMtu), wSize_ );
				_ret = 1;	//real name
			}
			else
			{
				WRDToTag( wrd_, szTag_, wSize_ );
				_ret = 2;	//pseudo name
			}
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
//...................................................................
_DLLEXP STATUS _DLLFUNC ModuGetStatus(WRD wrd_)
{
	CORE_ASSERT( wrd_ < gMTInfo.wCount );
	return (gMTInfo.lpBase)[wrd_].Status;
}

//...................................................................
//...................................................................
_DLLEXP DIRTY _DLLFUNC ModuGetDirty(WRD wrd_)
{
	CORE_ASSERT( gMTInfo.lpBase[wrd_].Status & MTUBIT_VALID );
	return (gMTInfo.lpBase)[wrd_].Dirty;
}

//...................................................................
//...................................................................
_DLLEXP IDALGO _DLLFUNC ModuGetAlgo(WRD wrd_)
{
	CORE_ASSERT( gMTInfo.lpBase[wrd_].Status & MTUBIT_VALID );
	return (gMTInfo.lpBase)[wrd_].Algo;
}

//...................................................................
//...................................................................
_DLLEXP WORD _DLLFUNC ModuGetNumParam(WRD wrd_)
{
	CORE_ASSERT( gMTInfo.lpBase[wrd_].Status & MTUBIT_VALID );
	return (gMTInfo.lpBase)[wrd_].NumParam;
}

//...................................................................
//...................................................................
_DLLEXP WRD _DLLFUNC ModuGetParent(WRD wrd_)
{
	CORE_ASSERT( gMTInfo.lpBase[wrd_].Status & MTUBIT_VALID );
	return (gMTInfo.lpBase)[wrd_].Parent;
}

//...................................................................
//...................................................................
_DLLEXP WRD _DLLFUNC ModuGetChild(WRD wrd_)
{
	CORE_ASSERT( gMTInfo.lpBase[wrd_].Status & MTUBIT_VALID );
	return (gMTInfo.lpBase)[wrd_].Child;
}

//...................................................................
//...................................................................
_DLLEXP WRD _DLLFUNC ModuGetNext(WRD wrd_)
{
	CORE_ASSERT( gMTInfo.lpBase[wrd_].Status & MTUBIT_VALID );
	return (gMTInfo.lpBase)[wrd_].Next;
}

//...................................................................
//...................................................................
_DLLEXP LPPU _DLLFUNC ModuGetParameter(WRD wrd_)
{
	CORE_ASSERT( gMTInfo.lpBase[wrd_].Status & MTUBIT_VALID );
	return (gMTInfo.lpBase)[wrd_].lpParam;
}

//...................................................................
//Description:
//
//...................................................................
_DLLEXP WRD _DLLFUNC ModuGetPrevious( WRD wrd_ )
{
	WRD     _wModu;

	//assert that the MTU is valid
	CORE_ASSERT( (gMTInfo.lpBase)[wrd_].Status & MTUBIT_VALID );
	_RETURN_FALSE_IF_INVALID(wrd_)

	_wModu = (gMTInfo.lpBase)[wrd_].Parent;  //get the parent node
	_wModu = (gMTInfo.lpBase)[_wModu].Child; //get the first node

	if(_wModu == wrd_)
		return 0;   //its the first face

	while(_wModu)
	{
		if( wrd_ == (gMTInfo.lpBase)[_wModu].Next )
			return _wModu;
		else
			_wModu = (gMTInfo.lpBase)[_wModu].Next;
	}

	CORE_ASSERT(0);
	return 0;
}

//...................................................................
//...................................................................
_DLLEXP WRD _DLLFUNC ModuGetLast(WRD wrd_)
{
	WRD _wNext = wrd_;

	while( (gMTInfo.lpBase + _wNext)->Next )
		_wNext = (gMTInfo.lpBase + _wNext)->Next;

	return _wNext;
}


//...................................................................
//...................................................................
_DLLEXP WORD _DLLFUNC ModuGetElementType( WORD element_ )
{
	int     _i;

	for(_i=0; _i<_NUM_NAMESET; _i++)
		if(element_ == ssNameSet[_i].element)
			return ssNameSet[_i].type;

	return 0;
}

//...................................................................
//...................................................................
_DLLEXP BOOL _DLLFUNC ModuGetElementName(WORD element_, LPSTR szName_, WORD wSize_)
{
	unsigned        _i;

	CLEAR_LAST_ERROR;

	if( !szName_ )
	{
		SET_LAST_ERROR( ERR_Invalid_Entries );
		return FALSE;
	}

	for(_i=0; _i<_NUM_NAMESET; _i++)
	{
		if(element_ == ssNameSet[_i].element)
		{
			StringCopyn(szName_, ssNameSet[_i].szName, wSize_);
			return TRUE;
		}
	}

	//at debug version, we trap this case!
	CORE_ASSERT(0);
	return FALSE;
}

//...................................................................
//Description:
//  Retrieves ID of the element by its ASCII name
//...................................................................
_DLLEXP WORD _DLLFUNC ModuGetElement(LPSTR szName_)
{
	unsigned        _i, _chars;

	if( !szName_ )
	{
		SET_LAST_ERROR( ERR_Invalid_Entries );
		return FALSE;
	}

	if( (*szName_ == 'P') || (*szName_ == 'p') ) //"Param" or "Parent"
		_chars = 4;
	else
		_chars = 3;

	for(_i=0; _i<_NUM_NAMESET; _i++)
	{
		if( !strnicmp(ssNameSet[_i].szName, szName_, _chars) )
			return ssNameSet[_i].element;
	}

	return 0;
}

//...................................................................
//Decription:
//  Set the modu tag if the new tag is legal after checked;
//Note:
//  if the new tag begins with '#', returns FALSE;
//...................................................................
_DLLEXP BOOL _DLLFUNC ModuSetTag(WRD wrd_, LPCSTR szTag_)
{
	LPMTU	_lpMtu = gMTInfo.lpBase + wrd_;
	BOOL    _bOk;
	LPCOBJ  _lpTagOld, _lpTagNew;

	_RETURN_FALSE_IF_INVALID(wrd_)

	_lpTagNew = CoreCheckAndFormatTag( szTag_, &_bOk );
	if( !_bOk )
		return FALSE;

	_lpTagOld = _lpMtu->lpTag;
	if( !CmpTag( GET_COBJ_DATA(_lpTagNew), GET_COBJ_DATA(_lpTagOld), CORE_MAX_TAG_LENGTH ) )
		return TRUE;	//the old and new tag are identify
	else
	{
		_lpMtu->lpTag = _lpTagNew;

		if( !ModuCheckLegality( wrd_ ) )
		{
			//the new tag is illegal
			CobjFree( _lpTagNew );
			_lpMtu->lpTag = _lpTagOld;
			return FALSE;
		}
		else	//here, legal tag
		{
			_lpMtu->Dirty |= MTUBIT_DIRTY_TAG;

			//set tag successful, and delete the old tag
			CobjFree( _lpTagOld );
			return TRUE;
		}
	}
}

//...................................................................
//...................................................................
_DLLEXP void _DLLFUNC ModuToggleExpand( WRD wrd_ )
{
	//assert that the MTU is valid
	CORE_ASSERT( (gMTInfo.lpBase)[wrd_].Status & MTUBIT_VALID );

	//toggle the expanded bit
	(gMTInfo.lpBase)[wrd_].Status ^= (MTUBIT_EXPANDED);

	return;
}

//...................................................................
//Description:
//  this function should use with care;
//...................................................................
_DLLEXP void _DLLFUNC ModuSetStatus( WRD wrd_, STATUS status_ )
{
	//assert that the MTU is valid
	CORE_ASSERT( (gMTInfo.lpBase)[wrd_].Status & MTUBIT_VALID );

	(gMTInfo.lpBase)[wrd_].Status = status_;

	return;
}

//...................................................................
//...................................................................
_DLLEXP BOOL _DLLFUNC ModuInsertParamGroup( WRD wrd_, WORD wInsertBefore_ )
{
	LPMTU   _lpMtu = gMTInfo.lpBase + wrd_;
	WORD    _wNumInsert, _i;

	if(  (wrd_ >= gMTInfo.wAdding)
	   ||( !(_lpMtu->Status & MTUBIT_VALID) )
	   ||( wInsertBefore_ >= _lpMtu->NumParam ) )
	{
		CORE_ASSERT( FALSE );
		return FALSE;
	}

	_wNumInsert = (WORD)AlgoCallParamProc( _lpMtu->Algo, PIC_CANINSERT,
							   wrd_, wInsertBefore_, 0 );
	if( !_wNumInsert )
		return FALSE;

	if( !ModuInsertParam( wrd_, wInsertBefore_, _wNumInsert ) )
		return FALSE;

	for(_i=0; _i<_wNumInsert; _i++ )
	{
		AlgoCallParamProc(	_lpMtu->Algo, 
							PIC_INITIALIZEPARAM,
							wrd_, 
							(WORD)(wInsertBefore_ + _i), 
							0 );
	}
	return TRUE;
}

//...................................................................
//Note:
//  This function only allocate the memory and reset the parameter
//  pointer in MTU, don't initialize the new parameters;
//...................................................................
_DLLEXP BOOL _DLLFUNC ModuInsertParam( WRD wrd_, WORD wInsertBefore_, WORD wNumInsert_ )
{
	LPPU    _lpPrmNew;
	LPMTU   _lpMtu = gMTInfo.lpBase + wrd_;

	CORE_ASSERT( wNumInsert_ );
	if(  (wrd_ >= gMTInfo.wAdding)
	   ||( !(_lpMtu->Status & MTUBIT_VALID) )
	   ||( wInsertBefore_ >= _lpMtu->NumParam ) )
	{
		CORE_ASSERT( FALSE );
		return FALSE;
	}

	_lpPrmNew = ParamAlloc( (_lpMtu->NumParam + wNumInsert_) );
	if( !_lpPrmNew )
		return FALSE;

	memmove( _lpPrmNew,
			 _lpMtu->lpParam,
			 sizeof(PU)* wInsertBefore_ );

	memmove( _lpPrmNew + wInsertBefore_ + wNumInsert_,
			 _lpMtu->lpParam + wInsertBefore_,
			 sizeof(PU)* (_lpMtu->NumParam - wInsertBefore_) );

	FreeParameter( _lpMtu->lpParam, _lpMtu->NumParam );   //at first, free the old param block
	_lpMtu->lpParam = _lpPrmNew;
	_lpMtu->NumParam = _lpMtu->NumParam + wNumInsert_;
	_lpMtu->Dirty = MTUBIT_DIRTY_ALL;

	return TRUE;
}

//...................................................................
//...................................................................
_DLLEXP BOOL _DLLFUNC ModuAppendParamGroup( WRD wrd_ )
{
	LPMTU   _lpMtu = gMTInfo.lpBase + wrd_;
	WORD    _wNumParamOld;
	WORD    _wNumAppend;
	WORD    _i;

	if(  (wrd_ >= gMTInfo.wAdding)
	   ||( !(_lpMtu->Status & MTUBIT_VALID) ) )
	{
		CORE_ASSERT( FALSE );
		return FALSE;
	}

	_wNumAppend = (WORD)AlgoCallParamProc( _lpMtu->Algo, PIC_CANAPPEND,
							   wrd_, 0, 0 );
	if( !_wNumAppend )
		return FALSE;

	_wNumParamOld = ModuGetNumParam( wrd_ );
	if( !ModuAppendParam( wrd_, _wNumAppend ) )
		return FALSE;

	for(_i=0; _i<_wNumAppend; _i++ )
	{
		AlgoCallParamProc( _lpMtu->Algo, PIC_INITIALIZEPARAM,
					   wrd_, (WORD)(_wNumParamOld + _i), 0 );
	}
	return TRUE;
}

//...................................................................
//Note:
//  This function only allocate the memory and reset the parameter
//  pointer in MTU, don't initialize the new parameters;
//...................................................................
_DLLEXP BOOL _DLLFUNC ModuAppendParam( WRD wrd_, WORD wNumAppend_ )
{
	LPPU    _lpPrmNew;
	LPMTU   _lpMtu = gMTInfo.lpBase + wrd_;

	CORE_ASSERT( wNumAppend_ );
	if(  (wrd_ >= gMTInfo.wAdding)
	   ||( !(_lpMtu->Status & MTUBIT_VALID) ) )
	{
		CORE_ASSERT( FALSE );
		return FALSE;
	}

	_lpPrmNew = ParamAlloc( (_lpMtu->NumParam + wNumAppend_) );
	if( !_lpPrmNew )
		return FALSE;

	memmove( _lpPrmNew,
			 _lpMtu->lpParam,
			 sizeof(PU)* _lpMtu->NumParam );

	FreeParameter( _lpMtu->lpParam, _lpMtu->NumParam );   //at first, free the old param block
	_lpMtu->lpParam = _lpPrmNew;
	_lpMtu->NumParam = _lpMtu->NumParam + wNumAppend_;
	_lpMtu->Dirty = MTUBIT_DIRTY_ALL;

	return TRUE;
}

//...................................................................
//...................................................................
_DLLEXP BOOL _DLLFUNC ModuDeleteParamGroup( WRD wrd_, WORD wFirstDelete_ )
{
	LPMTU   _lpMtu = gMTInfo.lpBase + wrd_;
	WORD    _wNumDelete;

	if(  (wrd_ >= gMTInfo.wAdding)
	   ||( !(_lpMtu->Status & MTUBIT_VALID) ) )
	{
		CORE_ASSERT( FALSE );
		return FALSE;
	}

	_wNumDelete = (WORD)AlgoCallParamProc( _lpMtu->Algo, PIC_CANDELETE,
							   wrd_, wFirstDelete_, 0 );

	if( !_wNumDelete )
		return FALSE;

	return ModuDeleteParam( wrd_, wFirstDelete_, _wNumDelete );
}

//...................................................................
//Note:
//  This function only delete the memory and reset the parameter
//  pointer in MTU;
//...................................................................
_DLLEXP BOOL _DLLFUNC ModuDeleteParam( WRD wrd_, WORD wFirstDelete_, WORD wNumDelete_ )
{
	LPMTU   _lpMtu = gMTInfo.lpBase + wrd_;
	WORD    _wFirstRest;
	int     _rests;
	LPPU    _lpPrmNew;

	CORE_ASSERT( wNumDelete_ );

	if(  (wrd_ >= gMTInfo.wAdding)
	   ||( !(_lpMtu->Status & MTUBIT_VALID) ) )
	{
		CORE_ASSERT( FALSE );
		return FALSE;
	}

	_wFirstRest = wNumDelete_ + wFirstDelete_;
	_rests = _lpMtu->NumParam - _wFirstRest;

	if( _rests < 0 )
	{
		CORE_ASSERT( FALSE );
		return FALSE;
	}

	_lpPrmNew = ParamAlloc( (_lpMtu->NumParam - wNumDelete_) );
	if( !_lpPrmNew )
		return FALSE;

	memmove( _lpPrmNew,
			 _lpMtu->lpParam,
			 sizeof(PU)* wFirstDelete_ );

	memmove( _lpPrmNew + wFirstDelete_,
			 _lpMtu->lpParam + _wFirstRest,
			 sizeof(PU)* _rests );

	FreeParameter( _lpMtu->lpParam, _lpMtu->NumParam );   //at first, free the old param block
	_lpMtu->lpParam = _lpPrmNew;
	_lpMtu->NumParam = wFirstDelete_ + _rests;
	_lpMtu->Dirty = MTUBIT_DIRTY_ALL;

	return TRUE;
}

//...................................................................
//Description:
//Note:
//  The old parameters is freed by this function;
//...................................................................
_DLLEXP BOOL _DLLFUNC ModuChangeAlgo(WRD wrd_, IDALGO id_ )
{
	LPMTU   _lpMtu;
	LPPU    _lpPrmNew;
	WORD    _wNumParamNew;
	COREREF _core_ref;

	MTU		_mtuOld, _mtuNew;

	_RETURN_FALSE_IF_INVALID(wrd_)

	_lpMtu = (gMTInfo.lpBase) + wrd_;

	if(  (wrd_ >= gMTInfo.wAdding)
	   ||( !(_lpMtu->Status & MTUBIT_VALID) ) )
	{
		CORE_ASSERT( FALSE );
		SET_LAST_ERROR( ERR_Invalid_Entries );
		return FALSE;
	}

	if( !IsAlgo(id_) ) {
		SET_LAST_ERROR( ERR_Invalid_Algo );
		return 0;   //illegal algo. id;
	}

	memset( &_core_ref, 0, sizeof(_core_ref) );
	if( GetReference( TB_MT, wrd_, TRUE /*skip myself*/, &_core_ref ) )
	{
		if( _core_ref.wTable != TB_MT )
			return 0;	//the MTU is referenced by others
	}

	_wNumParamNew = AlgoGetNumParam( id_ );
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

	//keep MTU head aside
	memcpy(&_mtuOld, _lpMtu, sizeof(MTU) );

	//replace it with the new settings
	_lpMtu->lpParam = _lpPrmNew;
	_lpMtu->NumParam = _wNumParamNew;
	_lpMtu->Algo = id_;
	_lpMtu->Status = MTUBIT_VALID;
	_lpMtu->Dirty = MTUBIT_DIRTY_ALL;
	_lpMtu->UserData = NULL;

	//to initialize it
	if( !AlgoPICCreate( id_, wrd_, 0) )
	{
		//if failed, then recover it
		memcpy(_lpMtu, &_mtuOld, sizeof(MTU) );

		//release the memory for new paramters
		FreeParameter( _lpPrmNew, _wNumParamNew );
		return FALSE;
	}
	else
	{
		//backup the new head
		memcpy(&_mtuNew, _lpMtu, sizeof(MTU) );

		//temporarily change it to the old setting again
		memcpy(_lpMtu, &_mtuOld, sizeof(MTU) );

		//send PIC_DELETE message to the algorithm
		AlgoPICDelete( _lpMtu->Algo, wrd_ );
		FreeParameter( _lpMtu->lpParam, _lpMtu->NumParam );	

		//at the last, set the new head again
		memcpy(_lpMtu, &_mtuNew, sizeof(MTU) );

		return TRUE;
	}
}

//...................................................................
//Description:
//  Set Flag_ bit TRUE and does not change other bits
//...................................................................
_DLLEXP BOOL _DLLFUNC ModuSetPUFlagBit( WRD wrd_, WORD wOrder_, WORD Flag_ )
{
	LPMTU	_lpMtu;

	_RETURN_FALSE_IF_INVALID(wrd_)

	_lpMtu = (gMTInfo.lpBase) + wrd_;

	if( wOrder_ == (WORD)-1 )
	{
		unsigned _i;
		CORE_ASSERT( _lpMtu->lpParam );

		for( _i=0; _i<_lpMtu->NumParam; _i++)
			(_lpMtu->lpParam)[_i].flags |= Flag_;

		return TRUE;
	}
	else
	{
		if(   _lpMtu->lpParam
		   && (_lpMtu->NumParam>wOrder_) )
		{
			(_lpMtu->lpParam)[wOrder_].flags |= Flag_;
			return TRUE;
		}
		else
		{
			CORE_ASSERT( FALSE );
			return FALSE;
		}
	}
}

//...................................................................
//Description:
//  Clear bFlag bit and does not change others
//...................................................................
_DLLEXP BOOL _DLLFUNC ModuClearPUFlagBit( WRD wrd_, WORD  wOrder_, WORD Flag_ )
{
	LPMTU	_lpMtu;

	_RETURN_FALSE_IF_INVALID(wrd_)

	_lpMtu = (gMTInfo.lpBase) + wrd_;

	if( wOrder_ == (WORD)-1 )
	{
		unsigned _i;

		CORE_ASSERT( _lpMtu->lpParam );

		for( _i=0; _i<_lpMtu->NumParam; _i++)
			(_lpMtu->lpParam)[_i].flags &= (~Flag_);

		return TRUE;
	}
	else
	{
		if(  _lpMtu->lpParam
		   &&( _lpMtu->NumParam > wOrder_) )
		{
			(_lpMtu->lpParam)[ wOrder_].flags &= (~Flag_);
			return TRUE;
		}
		else
		{
			CORE_ASSERT( FALSE );
			return FALSE;
		}
	}
}

//...................................................................
//Description:
//  clear dirty flags
//Return:
//  none
//...................................................................
_DLLEXP void _DLLFUNC ModuClearDirty(WRD wrd_)
{
	LPMTU	_lpMtu;
	LPPU _lppu;

	_lpMtu = (gMTInfo.lpBase) + wrd_;
	_lpMtu->Dirty &= (~MTUBIT_DIRTY_ALL);
	if( !(_lpMtu->Status & MTUBIT_VALID) )
	{  						
		//Don't check for legality, ref to CoreClearDirty()
		//CORE_ASSERT( FALSE );
		return;											
	}															

	_lppu = _lpMtu->lpParam;
	if( _lppu )
	{
		WORD    _i;
		for(_i=0;
			_i<_lpMtu->NumParam;
			_i++, _lppu++ )
		{
			_lppu->flags &= ( ~PU_DIRTY );
		}
	}

	return ;
}

//...................................................................
//...................................................................
_DLLEXP LPSTR _DLLFUNC ModuGetData(LPSTR lpsz_, int size_, WRD wrd_, WORD order_)
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

	_pu.prefix = PF_TBX|TBX_MT|order_;
	_pu.du.zzk__WRD = wrd_;
	_ret = PUGetValueString( &_pu, &_gvs, ssNameSet[_i].vsof );

	return _ret? lpsz_: NULL;
}

//...................................................................
//...................................................................
_DLLEXP BOOL _DLLFUNC ModuSetData(LPCSTR lpsz_, WRD wrd_, WORD order_)
{
	PU			_pu;
	DWORD		_ret;
	
	int			_i;

	if(  ( wrd_ >= gMTInfo.wCount )
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

	_pu.prefix = PF_TBX|TBX_MT|order_;
	_pu.du.zzk__WRD = wrd_;
	_ret = PUSetValueString( &_pu, lpsz_, ssNameSet[_i].vsof );

	return _ret? TRUE: FALSE;
}

//...................................................................
//Description:
//  Retrieves the WRD of "StepModu"
//...................................................................
_DLLEXP WRD _DLLFUNC GetStepRoot()
{
	return MODU_STEPROOT;
}

//////////////////////////////////////////////////////
//inner function

//...................................................................
//inner function
//Note:
//  This function should be used with care.
//...................................................................
void _near _ModuSetChild(WRD wrd_, WRD wrdChild_)
{
	(gMTInfo.lpBase)[wrd_].Child = wrdChild_;
	(gMTInfo.lpBase)[wrd_].Dirty |= MTUBIT_DIRTY_CHILD;
}

//...................................................................
//inner function
//Note:
//  This function should be used with care.
//...................................................................
void _near _ModuSetNext(WRD wrd_, WRD wrdNext_)
{
	(gMTInfo.lpBase)[wrd_].Next = wrdNext_;
	(gMTInfo.lpBase)[wrd_].Dirty |= MTUBIT_DIRTY_NEXT;
}

//...................................................................
//Description:
//...................................................................
static WRD ModuGetFreeRoom()
{
	WRD     _i;

	CLEAR_LAST_ERROR;

	for(_i=1; _i<gMTInfo.wAdding; _i++)
	{
		if( (gMTInfo.lpBase + _i)->Status == 0 )
			break;
	}

	if( _i < gMTInfo.wAdding )
		return _i;
	else    // _i == gMTInfo.wAdding
	{
		if( gMTInfo.wAdding < gMTInfo.wCount )
		{
			gMTInfo.wAdding ++;
			return _i;
		}
		else    //no free room
		{
			SET_LAST_ERROR( ERR_MTU_No_Free_Room );
			return 0;
		}
	}
}

//...................................................................
//...................................................................
static void ModuReleaseRoom(WRD wrd_)
{
	if( wrd_ >= gMTInfo.wAdding )
	{
		CORE_ASSERT( FALSE );
		return ;
	}

	memset( gMTInfo.lpBase + wrd_, 0, sizeof(MTU) );

	//if the MTU[adding-1] is free, decrease Adding
	while(  ((gMTInfo.lpBase + gMTInfo.wAdding - 1)->Status == 0 )
		  &&((gMTInfo.lpBase + gMTInfo.wAdding - 1)->Dirty == 0 )
		  &&( gMTInfo.wAdding > 1 ) )
		gMTInfo.wAdding -- ;

	return;
}

_DLLEXP BOOL _DLLFUNC ModuPaste( WRD wrdParent_, WRD wrdCopy_, BOOL bTree_ )
{
	WRD		_wrd;

	if( !ModuIsValid(wrdParent_) ) return 0;
	if( !ModuIsValid(wrdCopy_) ) return 0;

	if( bTree_ )
		_wrd = _ModuPasteTree( wrdParent_, wrdCopy_ );
	else
		_wrd = _ModuPaste( wrdParent_, wrdCopy_ );

	//adjust references
	_ModuAdjustReferenceTree( _wrd, wrdCopy_, _wrd, wrdCopy_ );

	return _wrd;
}

static void _ModuAdjustReferenceTree( WRD wrdDes_, WRD wrdSrc_, WRD wrdRootDes_, WRD wrdRootSrc_ )
{
	WRD		_wrdSrc, _wrdDes;
	WRD		_wrdRef;
	WORD	_i;
	WORD	_num_param = ModuGetNumParam( wrdDes_ );
	LPPU	_lpPUDes, _lpPUSrc;

	for( _i=0, _lpPUDes = ModuGetParameter(wrdDes_), _lpPUSrc = ModuGetParameter(wrdSrc_);
		 _i<_num_param; 
		 _i++, _lpPUDes++, _lpPUSrc++ )
	{
		if(  ( _lpPUSrc->prefix == (PF_TBX|TBX_MT) )
		   ||( (_lpPUSrc->prefix & PF_MASK) == PF_MPX ) )
		{
			if( !ModuIsInTree( wrdRootSrc_, _lpPUSrc->du.zzk__WRD ) )
				continue;

			if( !_ModuSynRef( wrdRootSrc_, _lpPUSrc->du.zzk__WRD, wrdRootDes_, &_wrdRef ) )
				continue;
			
			CORE_ASSERT( _lpPUSrc->prefix == _lpPUDes->prefix );
			_lpPUDes->du.zzk__WRD = _wrdRef;
		}
	}

	for(_wrdSrc = ModuGetChild( wrdSrc_ ), _wrdDes = ModuGetChild( wrdDes_ );
		_wrdSrc && _wrdDes;
		_wrdSrc = ModuGetNext( _wrdSrc ), _wrdDes = ModuGetNext( _wrdDes ) )
	{
		_ModuAdjustReferenceTree( _wrdDes, _wrdSrc, wrdRootDes_, wrdRootSrc_ );
	}

	return;
}

static WRD _ModuPasteTree( WRD wrdParent_, WRD wrdCopy_ )
{
	WRD		_wrd, _wrdPaste;
	BOOL	_ok = TRUE;

	_wrdPaste = _ModuPaste( wrdParent_, wrdCopy_ );
	if( !_wrdPaste )
		return 0;

	for(_wrd = ModuGetChild( wrdCopy_ );
		_wrd;
		_wrd = ModuGetNext( _wrd ) )
	{
		if( !_ModuPasteTree( _wrdPaste, _wrd ) )
		{
			_ok = FALSE;
			break;
		}
	}

	if( !_ok )
	{
		_Modu_delete( _wrdPaste );
		return 0;
	}
	else
		return _wrdPaste;
}

static WRD _ModuPaste( WRD wrdParent_, WRD wrdCopy_ )
{
	char	_szNewTag[CORE_MAX_TAG_LENGTH];
	LPMTU	_lpMtuParent, _lpMtuCopy, _lpMtuPaste;
	WRD		_wrdPaste = 0;
	WORD	_i;
	LPPU	_lpPU, _lpPUSrc;
	BOOL	_bOk;	//cobject copy result
	LPCOBJ	_lpComment = NULL;

	_lpMtuParent = gMTInfo.lpBase + wrdParent_;
	_lpMtuCopy = gMTInfo.lpBase + wrdCopy_;

	_DefaultNewTag( _szNewTag, (LPSTR)GET_COBJ_DATA(_lpMtuCopy->lpTag) );

	_wrdPaste = ModuCreate( _szNewTag, wrdParent_, _lpMtuCopy->Algo, 0 );
	if( !_wrdPaste )
		return 0;
	_lpMtuPaste =  gMTInfo.lpBase + _wrdPaste;
	_lpMtuPaste->Status = _lpMtuCopy->Status;

	CORE_ASSERT( !(_lpMtuPaste->Status & MTUBIT_TEMP) );
	CORE_ASSERT( _lpMtuPaste->Dirty == (DIRTY)-1 );

	if( _lpMtuPaste->NumParam != _lpMtuCopy->NumParam )
	{
		LPPU	_lpPrmNew = ParamAlloc( _lpMtuCopy->NumParam );
		if( !_lpPrmNew )
		{
			_Modu_delete( _wrdPaste );
			ModuReleaseRoom( _wrdPaste );
			return 0;
		}

		ParamFree( _lpMtuPaste->lpParam ); 
		_lpMtuPaste->lpParam = _lpPrmNew;
		_lpMtuPaste->NumParam = _lpMtuCopy->NumParam;
	}

	memcpy(  _lpMtuPaste->lpParam,
			 _lpMtuCopy->lpParam,
			 sizeof(PU)* _lpMtuCopy->NumParam );

	//paste the parameters

	//in the first loop, clear the LPCOBJ pointers in order to avoid deleting core objects
	//at the case that an error occurred
	for( _i=0, _lpPU = _lpMtuPaste->lpParam;
		 _i<_lpMtuPaste->NumParam; 
		 _i++, _lpPU++ )
	{
		if( _lpPU->prefix == (PF_IMM|IMM_LPCOBJ) )
			_lpPU->du.zzk__LPCOBJ = NULL;
	}

	//then, copy core objects
	for( _i=0, _lpPU = _lpMtuPaste->lpParam, _lpPUSrc = _lpMtuCopy->lpParam;
		 _i<_lpMtuPaste->NumParam; 
		 _i++, _lpPU++, _lpPUSrc++ )
	{
		if( _lpPU->prefix == (PF_IMM|IMM_LPCOBJ) )
		{
			LPCOBJ	_lpCobj = CobjCopy( _lpPUSrc->du.zzk__LPCOBJ, &_bOk );
			if( !_bOk )
			{
				//delete this
				_Modu_delete( _wrdPaste );
				ModuReleaseRoom( _wrdPaste );
				return 0;
			}
			else
			{
				_lpPU->du.zzk__LPCOBJ = _lpCobj;
			}
		}
	}

	//at last, copy module comment
	_lpComment = CobjCopy( _lpMtuCopy->lpComment, &_bOk );
	if( !_bOk )
	{
		//delete this
		_Modu_delete( _wrdPaste );
		ModuReleaseRoom( _wrdPaste );
		return 0;
	}
	else
	{
		CORE_ASSERT( !_lpMtuPaste->lpComment );
		_lpMtuPaste->lpComment = _lpComment;
	}

	if( !AlgoPICPaste( _lpMtuPaste->Algo, _wrdPaste ) )
	{
		ModuDelete( _wrdPaste );
		return 0;
	}

	return _wrdPaste;
}

//here, there are two cases permitted: 
//	1.	the source tree and the destination tree are identical
//	2.	the destination tree is no children, while the source tree is a tree
//
static BOOL _ModuSynRef( WRD wrdRootSrc_, WRD wrdRefSrc_, WRD wrdRootDes_, WRD* pwrdRefDes_ )
{
	WRD		_wrdSrc, _wrdDes;

	if( wrdRootSrc_ == wrdRefSrc_ )
	{
		*pwrdRefDes_ = wrdRootDes_;
		return TRUE;
	}

	for(_wrdSrc = ModuGetChild( wrdRootSrc_ ), _wrdDes = ModuGetChild( wrdRootDes_ );
		_wrdSrc && _wrdDes ;
		_wrdSrc = ModuGetNext( _wrdSrc ), _wrdDes = ModuGetNext( _wrdDes ) )
	{
		if( _ModuSynRef( _wrdSrc, wrdRefSrc_, _wrdDes, pwrdRefDes_ ) )
			return TRUE;
	}
	return FALSE;
}

//recalculate a new unique tag
static void _DefaultNewTag( LPSTR lpszDstTag_, LPCSTR lpszSrcTag_ )
{
	char	_szLeft [CORE_MAX_TAG_LENGTH],
			_szMid  [CORE_MAX_TAG_LENGTH],
			_szRight[CORE_MAX_TAG_LENGTH],
			_szTemp [CORE_MAX_TAG_LENGTH * 3];
	
	LPSTR _lpsz, _lpszLast;

	int	_i = 0;
	int _pos = 0;
	
	_szLeft[0] = _szMid[0] = _szTemp[0] = _szTemp[0] = 0;

	// find first non-space character
	_lpsz = (LPSTR)lpszSrcTag_;
	while ( isspace(*_lpsz) )
		_lpsz++;
	StringCopyn( _szTemp, _lpsz, CORE_MAX_TAG_LENGTH );

	// find beginning of trailing spaces by starting at beginning 
	_lpsz = _szTemp;
	_lpszLast = NULL;
	while (*_lpsz != '\0')
	{
		if (isspace(*_lpsz))
		{
			if (_lpszLast == NULL)
				_lpszLast = _lpsz;
		}
		else
			_lpszLast = NULL;
		_lpsz++;
	}
	if (_lpszLast != NULL)
		*_lpszLast = '\0';

	// find the first digit char
	_lpsz = strpbrk(_szTemp, "0123456789");

	if( !_lpsz )
	{
		//no digit char in tag string
		strcpy( _szLeft, _szTemp );
		do{
			sprintf( _szTemp, "%s%d", _szLeft, _i++ );
		} while( !IsNewLegalModuTag( _szTemp ) );
	}
	else
	{
		LPSTR _lpsz2 = _lpsz;
		while( *_lpsz2 && (isdigit(*_lpsz2)) )	
			_lpsz2++;

		StringCopyn( _szLeft,  _szTemp, _lpsz - _szTemp + 1 );
		StringCopyn( _szMid,   _lpsz, _lpsz2 - _lpsz + 1 ); 
		StringCopyn( _szRight, _lpsz2, CORE_MAX_TAG_LENGTH );	//copy to the end of string

		_i = atoi( (LPSTR)(LPCSTR)_szMid );

		do{
			sprintf( _szTemp, "%s%d%s", _szLeft, _i++, _szRight );
		} while( !IsNewLegalModuTag( _szTemp ) );
	}

	if( strlen( _szTemp )+1 >= CORE_MAX_TAG_LENGTH )
		*lpszDstTag_ = 0;
	else
		strcpy( lpszDstTag_, _szTemp );

	return;
}

_DLLEXP BOOL _DLLFUNC IsNewLegalModuTag( LPCSTR lpszTag_ )
{
	int _i;

	if( !IsTagLegal( lpszTag_ ) )
		return FALSE;

	//check on whether it's an unique tag in the module-table
	for(_i=1; _i<gMTInfo.wAdding; _i++)
	{
		LPMTU	_lpMTU = gMTInfo.lpBase + _i;

		if( _lpMTU->Status & MTUBIT_TEMP )
			continue;	//don't check temp MTU
		if(  (_lpMTU->Status & MTUBIT_VALID )
		   &&(_lpMTU->lpTag) )
		{
			if( CmpTag( lpszTag_,
						pMODU_GET_TAG(_lpMTU),
						CORE_MAX_TAG_LENGTH ) == 0)
			{
				return FALSE;
			}
		}
	}

	return TRUE;
}

//...................................................................
//...................................................................
_DLLEXP BOOL _DLLFUNC ModuMove(WRD wrd_, WRD wrdParentNew_, WRD wrdPrevNew_ )
{
	WRD _wrd;

	if( !((gMTInfo.lpBase)[wrd_].Status & MTUBIT_VALID) )
		return FALSE;

	if( !((gMTInfo.lpBase)[wrdParentNew_].Status & MTUBIT_VALID) )
		return FALSE;

	if( wrd_ == MODU_ROOT )
	{
		CORE_ASSERT( FALSE );
		return FALSE;
	}

	if( wrdPrevNew_ )
		if( ModuGetParent( wrdPrevNew_ ) != wrdParentNew_ )
		{
			CORE_ASSERT( FALSE );
			return FALSE;
		}

	//at the first, detach the wrd_ from the tree
	_wrd = ModuGetPrevious(wrd_);
	if(_wrd == NULL)
	{
		_wrd = ModuGetParent(wrd_);
		(gMTInfo.lpBase)[_wrd].Child = (gMTInfo.lpBase)[wrd_].Next;
		(gMTInfo.lpBase)[_wrd].Dirty |= MTUBIT_DIRTY_CHILD;
	}
	else
	{
		(gMTInfo.lpBase)[_wrd].Next = (gMTInfo.lpBase)[wrd_].Next;
		(gMTInfo.lpBase)[_wrd].Dirty |= MTUBIT_DIRTY_NEXT;
	}

	//second, place the wrd_ to the new position
	if( wrdPrevNew_ )
	{
		//backup the old next node
		_wrd = (gMTInfo.lpBase)[wrdPrevNew_].Next;
		(gMTInfo.lpBase)[wrdPrevNew_].Next = wrd_;
		(gMTInfo.lpBase)[wrdPrevNew_].Dirty |= MTUBIT_DIRTY_NEXT;
	}
	else	//as the first child
	{
		_wrd = (gMTInfo.lpBase)[wrdParentNew_].Child;
		(gMTInfo.lpBase)[wrdParentNew_].Child = wrd_;
		(gMTInfo.lpBase)[wrdParentNew_].Dirty = MTUBIT_DIRTY_PARENT;
	}
	(gMTInfo.lpBase)[wrd_].Parent = wrdParentNew_;
	(gMTInfo.lpBase)[wrd_].Next = _wrd;
	(gMTInfo.lpBase)[wrd_].Dirty |= (MTUBIT_DIRTY_NEXT|MTUBIT_DIRTY_PARENT);

	return TRUE;
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
_DLLEXP BOOL _DLLFUNC ModuChangeDataLink( WRD wrd_, WORD order_, LPCSTR lpszDataLink_ )
{
	//get the old value string first
	char				_szBuffer[512];
	PU					_puOld, _puNew;
	LPPU				_lpParam; 
	GETVALUESTRING		_gvs;
	DWORD				_ret_gvs;
	ENCODE				_encode;
	IDALGO				_idAlgo = ModuGetAlgo( wrd_ );
	WORD				_from, _to;

	_lpParam = ModuGetParameter( wrd_ );
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
	_ret_gvs = AlgoGetValueString(_idAlgo, wrd_, order_, &_gvs);

	if( !AlgoEncode(_idAlgo, wrd_, order_, &_encode) )
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
			AlgoSetValueString(_idAlgo, wrd_, order_, _szBuffer);
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
			CobjFree( _lpParam[order_].du.zzk__LPCOBJ );
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
_DLLEXP BOOL _DLLFUNC PUReadModuTable(SimplePU FAR* to_, LPPU from_)
{
	SimplePU 	_tmp;
	LPMTU		_lpMtu;

	if( (from_->du.zzk__WRD == 0) || (from_->du.zzk__WRD >= gMTInfo.wAdding) )
		return FALSE;

	_lpMtu = gMTInfo.lpBase + from_->du.zzk__WRD;

	switch(from_->prefix & 0x00FF)
	{
		case MTU_IDALGO:
			_tmp.prefix = PF_IMM|IMM_IDALGO;
			_tmp.du.zzk__IDALGO = _lpMtu->Algo;
			break;

		case MTU_STATUS:
			_tmp.prefix = PF_IMM|IMM_STATUS;
			_tmp.du.zzk__STATUS = _lpMtu->Status;
			break;

		case MTU_TAG:
			_tmp.prefix = PF_IMM|IMM_LPCOBJ;
			_tmp.du.zzk__LPCOBJ = _lpMtu->lpTag;
			/*
				after calling PUCast, _tmp.du.zzk__LPCOBJ will be cleared.
			*/
			break;
	
		case MTU_COMMENT:
			_tmp.prefix = PF_IMM|IMM_LPCOBJ;
			_tmp.du.zzk__LPCOBJ = _lpMtu->lpComment;
			/*
				after calling PUCast, _tmp.du.zzk__LPCOBJ will be cleared.
			*/
			break;
	
		case MTU_PARENT:
			_tmp.prefix = PF_IMM|IMM_WRDMTU;
			_tmp.du.zzk__WRD = _lpMtu->Parent;
			break;

		case MTU_CHILD:
			_tmp.prefix = PF_IMM|IMM_WRDMTU;
			_tmp.du.zzk__WRD = _lpMtu->Child;
			break;

		case MTU_NEXT:
			_tmp.prefix = PF_IMM|IMM_WRDMTU;
			_tmp.du.zzk__WRD = _lpMtu->Next;
			break;

		case MTU_PARAM:
			_tmp.prefix = PF_IMM|IMM_FPTR;
			_tmp.du.zzk__FPTR = _lpMtu->lpParam;
			break;

		case MTU_NUMPARAM:
			_tmp.prefix = PF_IMM|IMM_WORD;
			_tmp.du.zzk__WORD = _lpMtu->NumParam;
			break;

		case MTU_DIRTY:
			_tmp.prefix = PF_IMM|IMM_DIRTY;
			_tmp.du.zzk__DIRTY = _lpMtu->lpParam;
			break;

		default:
			return FALSE;
	}

	return PUCast(to_, &_tmp);
}

//...................................................................
//...................................................................
_DLLEXP BOOL _DLLFUNC PUWriteModuTable(LPPU to_, SimplePU FAR* from_)
{
	//SimplePU 	_tmp;
	LPMTU		_lpMtu;

	_lpMtu = (gMTInfo.lpBase) + to_->du.zzk__WRD;

	switch(to_->prefix & 0x00FF)
	{
		case MTU_TAG:
			/* Note:
			There, from->du.zzk__LPCOBJ must be a Cobject directly,
			and a reference to another string cobject is elligal,
			notice that the following code can't find this case:
				_tmp.prefix = PF_IMM|IMM_REAL;
				PUCast(&_tmp, from_);
			here, TRUE can be returned when from_ is a reference to a cobj.
			*/
			if( IS_STRING_COBJ( from_->du.zzk__LPCOBJ ) )
			{
				//need not check here, ModuSetTag will do everything 
				if( ModuSetTag(to_->du.zzk__WRD, PU_GET_COBJ_STRING(from_) ) )
				{
					//if success, delete the imported cobject
					CobjFree( from_->du.zzk__LPCOBJ );
					from_->du.zzk__LPCOBJ = NULL;

					//dirty has been set in ModuSetTag
					//	_MTU_SET_DIRTY( TAG )
					return TRUE;
				}
				else
					return FALSE;
			}
			else
			{
				LOGERR1("PU data is not a String:PU=%lp", from_);
				return FALSE;
			}
			break;

		case MTU_COMMENT:
			if( IS_STRING_COBJ( from_->du.zzk__LPCOBJ ) )
			{
				if( !CobjCompareString( _lpMtu->lpComment, from_->du.zzk__LPCOBJ ) )
					return TRUE;

				CobjFree ( _lpMtu->lpComment );
				_lpMtu->lpComment = from_->du.zzk__LPCOBJ;
				from_->du.zzk__LPCOBJ = 0;

				_MTU_SET_DIRTY( COMMENT )
				return TRUE;
			}
			else
			{
				LOGERR1("PU data is not a String:PU=%lp", from_);
				return FALSE;
			}

		default:
			return FALSE;
	}

	return TRUE;
}

_DLLEXP BOOL _DLLFUNC PUModuDirty( LPPU lppu_ )
{
	int		_i;
	WORD	_ele;

	CORE_ASSERT( ((lppu_->prefix) & TBX_MASK) == TBX_MT );
	if( lppu_->du.zzk__WRD  >= gMTInfo.wAdding )
	{
		CORE_ASSERT( FALSE );
		return FALSE;
	}

	_ele = lppu_->prefix & XT_MASK;

	for(_i=0; _i<_NUM_NAMESET; _i++)
		if( _ele == ssNameSet[_i].element )
		{
			return (gMTInfo.lpBase + lppu_->du.zzk__WRD)->Dirty & ssNameSet[_i].dirty;
		}

	CORE_ASSERT( FALSE );	//should NOT go here
	return FALSE;	//no found
}

//...................................................................
//Entries:
//	bPublicDirty_
//		If this is TRUE, the private dirty bits are excluded.
//...................................................................
_DLLEXP BOOL _DLLFUNC IsModuTableDirty( BOOL bPublicDirty_ )
{
	LPMTU	_lpMtu;
	LPPU	_lppu;
	DIRTY	_mask = MTUBIT_DIRTY_ALL;
	WRD		_wrd;

	if( bPublicDirty_ )
		_mask &= ~( MTUBIT_DIRTY_DIRTY );

	for(_wrd=1; _wrd<gMTInfo.wAdding; _wrd++)
	{
		_lpMtu = (gMTInfo.lpBase) + _wrd;
		_lppu = _lpMtu->lpParam;

		if( _lpMtu->Dirty & _mask )
			return TRUE;

		_lppu = _lpMtu->lpParam;
		if( _lppu )
		{
			WORD    _i;
			for(_i=0;
				_i<_lpMtu->NumParam;
				_i++, _lppu++ )
			{
				if( _lppu->flags & PU_DIRTY )
					return TRUE;
			}
		}	//end if
	}//end for

	return FALSE;
}
