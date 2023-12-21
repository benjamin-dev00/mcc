//===================================================================
//
//Zheng zhikai	95-10-4
//===================================================================

#include "prehead.h"

#include "core.h"
#include "cpu.h"
#include "cobj.h"
#include "csysobj.h"
#include "corerw.h"
#include "calarm.h"
#include "cbool.h"
#include "cerror.h"

static WRD BoolGetFreeRoom(void);
static void BoolReleaseRoom(WRD _wrd);
static BOOL BoolInitializeTrace( WRD wrd_ );

extern BTInfo		gBTInfo;

extern CoreInfo		gCoreInfo;
extern DWORD		gdwLastError;

struct {
	WORD 	element;
	char 	szName[16];
	VSOF	vsof;
	DIRTY	dirty;	//dirty mask
	WORD	type;
} static ssNameSet[] =
{
	{BTU_VALUE, 		"Value",		VSOF_BOOL,			BTUBIT_DIRTY_VALUE,			TET_PRIMARY },
	{BTU_TAG,			"Tag",			VSOF_LPCOBJ_STR,	BTUBIT_DIRTY_TAG,			TET_PRIMARY },
	{BTU_UNIT,			"Unit",			VSOF_LPCOBJ_STR,	BTUBIT_DIRTY_UNIT,			TET_PRIMARY },
	{BTU_COMMENT,		"Comment",		VSOF_LPCOBJ_STR,	BTUBIT_DIRTY_COMMENT,		TET_PRIMARY },
	{BTU_USERDATA,		"UserData",		VSOF_DWORD_HEX,		BTUBIT_DIRTY_USERDATA,		TET_PRIMARY },
	{BTU_TRACETIME,		"TraceTime",	VSOF_DWORD_MS,		BTUBIT_DIRTY_TRACETIME,		TET_PRIMARY },
	//private primary elements
	{BTU_STATUS,		"Status",		VSOF_STATUS_BTU,	BTUBIT_DIRTY_STATUS,		TET_PRIMARY|TET_READONLY|TET_PRIVATE },
	{BTU_ALARM,			"_alarm",		VSOF_ALARM_BTU,		BTUBIT_DIRTY_ALARM,			TET_PRIMARY|TET_READONLY|TET_PRIVATE },
	{BTU_DIRTY,			"Dirty",		VSOF_DIRTY_BTU,		BTUBIT_DIRTY_DIRTY,			TET_PRIMARY|TET_READONLY|TET_PRIVATE },
	{BTU_LASTLOG,		"LastLog",		VSOF_BOOL,			BTUBIT_DIRTY_LASTLOG,		TET_PRIMARY|TET_READONLY|TET_PRIVATE },
	{BTU_TRACEDATA,		"TraceData",	VSOF_FPTR,			BTUBIT_DIRTY_TRACEDATA,		TET_PRIMARY|TET_READONLY|TET_PRIVATE },
	//here, primary data end

	//value:
	{BTU_VALUENOT, 		"ValueNot",		VSOF_BOOL,			BTUBIT_DIRTY_VALUE,			TET_READWRITE },

	//others
	{BTU_ENABLEFALSE,	"EnableFalse",	VSOF_BOOL,			BTUBIT_DIRTY_STATUS,		TET_READWRITE },
	{BTU_ENABLETRUE,	"EnableTrue",	VSOF_BOOL,			BTUBIT_DIRTY_STATUS,		TET_READWRITE },
	{BTU_TRACE,			"Trace",		VSOF_BOOL,			BTUBIT_DIRTY_STATUS,		TET_READWRITE },
	{BTU_LOGHIS,		"LogHis",		VSOF_BOOL,			BTUBIT_DIRTY_STATUS,		TET_READWRITE },
	{BTU_VALID,			"Valid",		VSOF_BOOL,			BTUBIT_DIRTY_STATUS,		TET_READONLY|TET_PRIVATE },

	//Alarm on: ALM_xxx; data type: BOOL
	{BTU_ALM,			"Alm",			VSOF_BOOL,			BTUBIT_DIRTY_ALARM,			TET_READONLY },
	{BTU_ALMFALSE,		"AlmFalse",		VSOF_BOOL,			BTUBIT_DIRTY_ALARM,			TET_READONLY },
	{BTU_ALMTRUE,		"AlmTrue",		VSOF_BOOL,			BTUBIT_DIRTY_ALARM,			TET_READONLY },
	
	//data accessing defines: Alarm without(No) Ack: ANA_xxx
	//data type: BOOL
	{BTU_ANA,			"Ana",			VSOF_BOOL,			BTUBIT_DIRTY_ALARM,			TET_READONLY },
	{BTU_ANAFALSE,		"AnaFalse",		VSOF_BOOL,			BTUBIT_DIRTY_ALARM,			TET_READONLY },
	{BTU_ANATRUE,		"AnaTrue",		VSOF_BOOL,			BTUBIT_DIRTY_ALARM,			TET_READONLY },

	//data accessing defines: ack
	//data type: BOOL
	{BTU_ACK,			"Ack",			VSOF_BOOL,			BTUBIT_DIRTY_ALARM,			TET_READWRITE },
	{BTU_ACKFALSE,		"AckFalse",		VSOF_BOOL,			BTUBIT_DIRTY_ALARM,			TET_READONLY },
	{BTU_ACKTRUE,		"AckTrue",		VSOF_BOOL,			BTUBIT_DIRTY_ALARM,			TET_READONLY },

	//a STATUS type data to identify which kind of alarm is enabled. 
	//and if any alarm enabled, this value is non-zero
	{BTU_ENABLEALM,		"EnableAlm",	VSOF_WORD_HEX,		BTUBIT_DIRTY_STATUS,		TET_READONLY },
};

#define _NUM_NAMESET	( sizeof(ssNameSet)/sizeof(ssNameSet[0]) )
		
//...................................................................
//Description:
//	Creates a Boolean-unit in the Bool Table
//Return:
//		the subscription of the boolean-unit if the function
//		succeeds. Otherwise, it is NULL.
//...................................................................
_DLLEXP WRD _DLLFUNC BoolCreate(LPCSTR	lpszTag_, BOOL bValue_)
{
	WRD			_wrd;
	LPCOBJ		_lpCobj = NULL;
	BOOL		_bOk;
	
	CLEAR_LAST_ERROR;

	_wrd = BoolGetFreeRoom();
	if( !_wrd )
	{
		return FALSE;
	}

	memset( gBTInfo.lpBase + _wrd, 0, sizeof(BTU) );

	//allocate the memory for the tag
	_lpCobj = AllocTag( lpszTag_, &_bOk );
	if( !_bOk )		//allocate the tag failed
		return FALSE;

	(gBTInfo.lpBase + _wrd)->lpTag = _lpCobj;
	(gBTInfo.lpBase + _wrd)->Value = bValue_;

	if(!BoolCheckLegality(_wrd))
	{
		if( (gBTInfo.lpBase + _wrd)->lpTag )
			CobjFree( (gBTInfo.lpBase + _wrd)->lpTag );

		//this function does not change the Last Error
		BoolReleaseRoom(_wrd);
		return 0;
	}

	(gBTInfo.lpBase + _wrd)->Status |= (BTUBIT_VALID);
	//Note: here using "=" , not "|="
	(gBTInfo.lpBase + _wrd)->Dirty = BTUBIT_DIRTY_ALL;

	DispatchCoreMessage( TBX_BT );
	return _wrd;
}

//...................................................................
//Description:
//	Deletes a BTU;
//Entry:
//	WRD wrd_
//		the subscription of the BTU being freed
//...................................................................
_DLLEXP BOOL _DLLFUNC BoolDelete(WRD wrd_)
{
	LPBTU	_lpBtu;

	CLEAR_LAST_ERROR;
	_lpBtu = gBTInfo.lpBase + wrd_;

	if( wrd_ >= gBTInfo.wCount )
	{
		CORE_ASSERT(FALSE);
		SET_LAST_ERROR( ERR_Invalid_Entries );
		return NULL;
	}

	if( !(_lpBtu->Status & BTUBIT_VALID) )
	{
		CORE_ASSERT( FALSE );
		SET_LAST_ERROR( ERR_Invalid_Entries );
		return FALSE;
	}

	if( GetReference(TB_BT, wrd_, TRUE, NULL) )
	{
		SET_LAST_ERROR( ERR_Referenced_BTU );
		return FALSE;	//the BTU is referenced by other xTUs
	}

	_lpBtu->Status &= (~BTUBIT_VALID);	//clear VALID bit
	_lpBtu->Dirty |= BTUBIT_DIRTY_ALL;

	//free memory occupied by this unit
	FragFree( _lpBtu->lpTraceData );

	CobjFree( _lpBtu->lpTag );
	CobjFree( _lpBtu->lpUnit );
	CobjFree( _lpBtu->lpComment );
	_lpBtu->lpTag = _lpBtu->lpUnit =_lpBtu->lpComment = NULL;
	_lpBtu->lpTraceData = NULL;

	DispatchCoreMessage( TBX_BT );

	return TRUE;
}

//...................................................................
//Description:
//...................................................................
static WRD BoolGetFreeRoom()
{
	WRD		_i;

	CLEAR_LAST_ERROR;

	for(_i=1; _i<gBTInfo.wAdding; _i++)
	{
		//Note: the following code is WRONG
		//	if( ((gBTInfo.lpBase + _i)->Status & BTUBIT_VALID) == 0 )
		//
		if( (gBTInfo.lpBase + _i)->Status == 0 )
			break;
	}

	if( _i < gBTInfo.wAdding )
		return _i;
	else	// _i == gBTInfo.wAdding
	{
		if( gBTInfo.wAdding < gBTInfo.wCount )
		{
			gBTInfo.wAdding ++;
			return _i;
		}
		else	//no free room
		{
			//if no free room, set last error
			SET_LAST_ERROR( ERR_BTU_No_Free_Room );
			return 0;
		}
	}
}

//...................................................................
//Description:
//	This function frees the room occuped by BTU wrd_
//Note:
//	The function does NOT change the last error
//...................................................................
static void BoolReleaseRoom(WRD wrd_)
{
	if( wrd_ >= gBTInfo.wAdding )
	{
		CORE_ASSERT( FALSE );
		return ;
	}

	memset( gBTInfo.lpBase + wrd_, 0, sizeof(BTU) );

	//if the BTU[adding-1] is free, decrease Adding
	while(  ((gBTInfo.lpBase + gBTInfo.wAdding - 1)->Status == 0 )
		  &&((gBTInfo.lpBase + gBTInfo.wAdding - 1)->Dirty == 0 )
		  &&( gBTInfo.wAdding > 1 ) )
		gBTInfo.wAdding -- ;

	return ;
}

//...................................................................
//Description:
//  Create a temp. BTU by copying from wrd_
//...................................................................
_DLLEXP WRD _DLLFUNC BoolCopyTemp( WRD wrd_ )
{
	LPBTU       _lpBtuTmp, _lpBtuSrc;
	WRD         _wMe;
	LPCOBJ      _lpCobjTmpTag = NULL;
	LPCOBJ      _lpCobjTmpUnit = NULL;
	LPCOBJ      _lpCobjTmpComment = NULL;
	BOOL        _bOk;

	CLEAR_LAST_ERROR;

	_wMe = BoolGetFreeRoom();
	if( !_wMe ) return 0; //if no free room

	_lpBtuSrc = gBTInfo.lpBase + wrd_;

	//duplicate the tag, unit and comment
	_lpCobjTmpTag = CobjCopy( _lpBtuSrc->lpTag, &_bOk );
	if( !_bOk )	goto _BAD;
	_lpCobjTmpUnit = CobjCopy( _lpBtuSrc->lpUnit, &_bOk );
	if( !_bOk )	goto _BAD;
	_lpCobjTmpComment = CobjCopy( _lpBtuSrc->lpComment, &_bOk );
	if( !_bOk )	goto _BAD;

	//do not check for legality of the tag

	_lpBtuTmp = (gBTInfo.lpBase) + _wMe;

	//copy old data
	memcpy( _lpBtuTmp, _lpBtuSrc, sizeof(BTU) );    

	//update new data
	_lpBtuTmp->lpTag = _lpCobjTmpTag;
	_lpBtuTmp->lpUnit = _lpCobjTmpUnit;
	_lpBtuTmp->lpComment = _lpCobjTmpComment;
	_lpBtuTmp->Status |= BTUBIT_VALID|BTUBIT_TEMP;

	return _wMe;

_BAD:
	if( _lpCobjTmpTag ) CobjFree( _lpCobjTmpTag );
	if( _lpCobjTmpUnit ) CobjFree( _lpCobjTmpUnit );
	if( _lpCobjTmpComment ) CobjFree( _lpCobjTmpComment );

	BoolReleaseRoom( _wMe );
	return FALSE;
}

//...................................................................
//Description:
//...................................................................
_DLLEXP BOOL _DLLFUNC BoolDeleteTemp(WRD wrd_)
{
	LPBTU   _lpBtu = gBTInfo.lpBase + wrd_;

	CLEAR_LAST_ERROR;

	if(   (_lpBtu->Status & BTUBIT_VALID)
	   && (_lpBtu->Status & BTUBIT_TEMP) )
	{
		CobjFree( _lpBtu->lpTag );
		CobjFree( _lpBtu->lpUnit );
		CobjFree( _lpBtu->lpComment );
		_lpBtu->lpTag = _lpBtu->lpUnit =_lpBtu->lpComment = NULL;

		BoolReleaseRoom(wrd_);

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
//  Undo a BTU specified by wBool_ using its undo buffer: wBuffer_
//and then Delete the undo buffer BTU.
//...................................................................
_DLLEXP BOOL _DLLFUNC BoolUndo( WRD wBool_, WRD wBuffer_ )
{
	LPBTU   _lpBtu, _lpBuffer;

	if( !wBool_ || !wBuffer_ )
	{
		SET_LAST_ERROR( ERR_Invalid_Entries );
		return FALSE;
	}
	_lpBtu = gBTInfo.lpBase + wBool_;
	_lpBuffer = gBTInfo.lpBase + wBuffer_;

	//free the dirty tag, unit and comment
	CobjFree( _lpBtu->lpTag );	
	CobjFree( _lpBtu->lpUnit );	
	CobjFree( _lpBtu->lpComment );

	//undo the parameters of the BTU and other elements
	//NOTE: the tag of the undo buffer BTU is sent to the wBool_
	memcpy( _lpBtu, _lpBuffer, sizeof( BTU) );
	_lpBtu->Status &= (~BTUBIT_TEMP);    //clear TEMP bit

	//delete the undo buffer BTU
	memset( _lpBuffer, 0, sizeof(BTU) );
	BoolReleaseRoom( wBuffer_ );

	return TRUE;
}


//...................................................................
//Description:
//	This function only checks the BTUBIT_VALID bit;
//...................................................................
_DLLEXP BOOL _DLLFUNC BoolIsValid( WRD wrd_ )
{
	if(  ( wrd_ < gBTInfo.wCount )
	   &&( (gBTInfo.lpBase + wrd_)->Status & BTUBIT_VALID ) )
		return TRUE;
	else return FALSE;
}

//...................................................................
//Description:
//	Checks the legality of a BTU even that is invalid, including:
//		. legality of tag(ERROR CODE: ERR_Illegal_Tag)
//		. an unique of tag in the table(ERROR CODE: ERR_Not_Unique_Tag)
//Entry:
//	BTU FAR* lp_
//		source Bool-unit. returns 0 if lp_ == NULL.
//Return:
//		TRUE/FALSE
//...................................................................
_DLLEXP BOOL _DLLFUNC BoolCheckLegality(WRD wrd_)
{
	CLEAR_LAST_ERROR;
	if( wrd_ >= gBTInfo.wCount )
	{
		CORE_ASSERT(FALSE);
		SET_LAST_ERROR( ERR_Invalid_Entries );
		return NULL;
	}

	if( !BoolCheckTag( wrd_ ) )
		return FALSE;
	else
		return TRUE;
}

//...................................................................
//Description:
//Entry:
//Return:
//...................................................................
_DLLEXP BOOL _DLLFUNC BoolCheckTag(WRD wrd_)
{
	WRD		_i;
	LPBTU 	_lp;

	CLEAR_LAST_ERROR;
	if( wrd_ >= gBTInfo.wCount )
	{
		CORE_ASSERT(FALSE);
		SET_LAST_ERROR( ERR_Invalid_Entries );
		return NULL;
	}

	_lp = gBTInfo.lpBase + wrd_;

	if(	_lp->lpTag == NULL )
		return TRUE;

	if( !IsTagLegal( pBOOL_GET_TAG(_lp) ) )
	{
		return FALSE;
	}

	//check on whether it's an unique tag in the bool-table
	for(_i=1; _i<gBTInfo.wAdding; _i++)
	{
		if( (gBTInfo.lpBase[_i]).Status & BTUBIT_TEMP )
			continue;	//don't check temp BTU
		if(  ((gBTInfo.lpBase[_i]).Status & BTUBIT_VALID )
		   &&((gBTInfo.lpBase[_i]).lpTag) )
		{
			if( !CmpTag( BOOL_GET_TAG(_i), pBOOL_GET_TAG(_lp), CORE_MAX_TAG_LENGTH ) )
			{
				//check for whether itself
				if( _i == wrd_ )
					continue;
				else
				{
					SET_LAST_ERROR( ERR_Not_Unique_Tag );
					return FALSE;
				}
			}
		}
	}

	return TRUE;
}

//...................................................................
//Description:
//Entry:
//Return:
//...................................................................
_DLLEXP WRD _DLLFUNC BoolGetWRD(LPCSTR szTag_)
{
	WRD	_i;

	if(*szTag_ == '#')
	{
		WRD _wrd = TagToWRD(szTag_);
		if( _wrd == NULL)
			return 0;
		else
		{
			if( (gBTInfo.lpBase[_wrd]).Status & BTUBIT_VALID )
				return _wrd;
			else
				return 0;
		}
	}

	for(_i=1; _i<gBTInfo.wAdding; _i++)
	{
		if(  (gBTInfo.lpBase[_i].Status & BTUBIT_VALID )
		   &&((gBTInfo.lpBase[_i]).lpTag)  )
		{
			if( !CmpTag( BOOL_GET_TAG(_i),
						 szTag_,
						 CORE_MAX_TAG_LENGTH ) )
				return _i;
		}
	}

	return 0;
}

//...................................................................
//Description:
//Note:
//	Only valid BTU has tag except 0;	
//Entry:
//Return:
//	0:			failed, no name returns
//	1:			successful with Bool name
//	2:			successful with pseudo name
//...................................................................
_DLLEXP int _DLLFUNC BoolGetTag(WRD wrd_, LPSTR szTag_, WORD wSize_)
{
	int		_ret = 0;

	CLEAR_LAST_ERROR;

	if( wrd_ >= gBTInfo.wCount )
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
		LPBTU 	_lpBtu;
		_lpBtu = gBTInfo.lpBase + wrd_;

		if( _lpBtu->Status & BTUBIT_VALID )
		{
			if( (_lpBtu->lpTag) && *(_lpBtu->lpTag->data) )
			{
				StringCopyn( szTag_, pBOOL_GET_TAG(_lpBtu), wSize_ );
				_ret = 1;	//Bool name
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
_DLLEXP STATUS _DLLFUNC BoolGetStatus(WRD wrd_)
{
	if( wrd_ >= gBTInfo.wCount )
	{
		CORE_ASSERT(FALSE);
		SET_LAST_ERROR( ERR_Invalid_Entries );
		return 0;
	}

	CORE_ASSERT( gBTInfo.lpBase[wrd_].Status & BTUBIT_VALID );
	return (gBTInfo.lpBase)[wrd_].Status;
}

//...................................................................
//Note:
//	In the release version, the Status can be read even the BTU
//is invalid, this is more resionable than returns a zero when
//the BTU is invalid;
//...................................................................
_DLLEXP STATUS _DLLFUNC BoolSetStatus( WRD wrd_, STATUS status_ )
{
	STATUS _ret;
	
	if( wrd_ >= gBTInfo.wCount )
	{
		CORE_ASSERT(FALSE);
		SET_LAST_ERROR( ERR_Invalid_Entries );
		return 0;
	}

	CORE_ASSERT( gBTInfo.lpBase[wrd_].Status & BTUBIT_VALID );
	_ret = (gBTInfo.lpBase)[wrd_].Status;

	(gBTInfo.lpBase)[wrd_].Status = status_;

	return _ret;
}

//...................................................................
//...................................................................
_DLLEXP DIRTY _DLLFUNC BoolGetDirty(WRD wrd_)
{
	if( wrd_ >= gBTInfo.wCount )
	{
		CORE_ASSERT(FALSE);
		SET_LAST_ERROR( ERR_Invalid_Entries );
		return 0;
	}

	CORE_ASSERT( gBTInfo.lpBase[wrd_].Status & BTUBIT_VALID );
	return (gBTInfo.lpBase)[wrd_].Dirty;
}

//...................................................................
//Decription:
//	Set the bool tag if the new tag is legal after checked;
//Note:
//	if the new tag begins with '#', returns FALSE;
//...................................................................
_DLLEXP BOOL _DLLFUNC BoolSetTag(WRD wrd_, LPCSTR szTag_)
{
	BOOL		_bOk;
	LPBTU		_lpBtu = gBTInfo.lpBase + wrd_;

	if( !szTag_ )
	{
		if( _lpBtu->lpTag )
			_lpBtu->Dirty |= BTUBIT_DIRTY_TAG;

		CobjFree( _lpBtu->lpTag );
		_lpBtu->lpTag = NULL;
		return TRUE;
	}

	if( *szTag_ == '#' )
	{
		return FALSE;
	}

	if( _lpBtu->Status & BTUBIT_VALID )
	{
		LPCOBJ	_lpTagOld, _lpTagNew;

		if( strlen(szTag_) >= CORE_MAX_TAG_LENGTH - 1)
			return FALSE;

		//save the old tag
		_lpTagOld = _lpBtu->lpTag;
		_lpTagNew = AllocTag( szTag_, &_bOk );

		if( !_bOk )
			return FALSE;

		_lpBtu->lpTag = _lpTagNew;

		if( !BoolCheckLegality( wrd_ ) )
		{
			//the new tag is illegal
			CobjFree( _lpTagNew );
			_lpBtu->lpTag = _lpTagOld;
			return FALSE;
		}
		else
		{
			//set tag successful, and delete the old tag
			CobjFree( _lpTagOld );

			if( !IS_RUN_MODE() )
				_lpBtu->Dirty |= BTUBIT_DIRTY_TAG;
			else
				_lpBtu->Dirty |= BTUBIT_DIRTY_TAG;
			return TRUE;
		}
	}

	return FALSE;
}

//...................................................................
//...................................................................
_DLLEXP  BTInfo FAR* _DLLFUNC GetBoolTableInformation()
{
	return &gBTInfo;
}

//...................................................................
//...................................................................
_DLLEXP WORD _DLLFUNC BoolGetElementType( WORD element_ )
{
	int		_i;

	for(_i=0; _i<_NUM_NAMESET; _i++)
		if(element_ == ssNameSet[_i].element)
			return ssNameSet[_i].type;

	return 0;
}

//...................................................................
//...................................................................
_DLLEXP BOOL _DLLFUNC BoolGetElementName(WORD element_, LPSTR szName_, WORD wSize_)
{
	unsigned		_i;

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
//...................................................................
_DLLEXP WORD _DLLFUNC BoolGetElement(LPSTR szName_)
{
	unsigned		_i;

	CLEAR_LAST_ERROR;

	if( !szName_ )
	{
		SET_LAST_ERROR( ERR_Invalid_Entries );
		return FALSE;
	}

	for(_i=0; _i<_NUM_NAMESET; _i++)
	{
		if( !strnicmp(ssNameSet[_i].szName, szName_, 3) )
			return ssNameSet[_i].element;
	}

	//at debug version, we trap this case!
	CORE_ASSERT(0);
	return 0;
}

//...................................................................
//...................................................................
_DLLEXP LPSTR _DLLFUNC BoolGetData(LPSTR lpsz_, int size_, WRD wrd_, WORD order_)
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

	_pu.prefix = PF_TBX|TBX_BT|order_;
	_pu.du.zzk__WRD = wrd_;
	_ret = PUGetValueString( &_pu, &_gvs, ssNameSet[_i].vsof );

	return _ret? lpsz_: NULL;
}

//...................................................................
//...................................................................
_DLLEXP BOOL _DLLFUNC BoolSetData(LPCSTR lpsz_, WRD wrd_, WORD order_)
{
	PU			_pu;
	DWORD		_ret;
	
	int			_i;

	if(  ( wrd_ >= gBTInfo.wCount )
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

	_pu.prefix = PF_TBX|TBX_BT|order_;
	_pu.du.zzk__WRD = wrd_;
	_ret = PUSetValueString( &_pu, lpsz_, ssNameSet[_i].vsof );

	return _ret? TRUE: FALSE;
}

//...................................................................
//Entries:
//	bPublicDirty_
//		If this is TRUE, the private dirty bits are excluded.
//...................................................................
_DLLEXP BOOL _DLLFUNC IsBoolTableDirty( BOOL bPublicDirty_ )	
{
	WRD		_i;
	DIRTY	_mask = BTUBIT_DIRTY_ALL;

	if( bPublicDirty_ )
		_mask &= ~( BTUBIT_DIRTY_ALARM|
					BTUBIT_DIRTY_LASTLOG|
					BTUBIT_DIRTY_TRACEDATA|
					BTUBIT_DIRTY_DIRTY);

	for(_i=1; _i<gBTInfo.wAdding; _i++)
	{
		if( (gBTInfo.lpBase + _i)->Dirty & _mask )
			return TRUE;
	}

	return FALSE;
}

//...................................................................
//...................................................................
_DLLEXP void _DLLFUNC BoolClearTableDirty()
{
	WRD		_i;

	for(_i=1; _i<gBTInfo.wAdding; _i++)
		(gBTInfo.lpBase + _i)->Dirty &= (~BTUBIT_DIRTY_ALL);

	return;
}

//...................................................................
//Description:	
//	The function sets properly the Alarm bits depending on the value 
//	and Alarm thresholds;
//Returns:
//	Returns the updated Alarm states;
//Note:
//	When the following value changed:
//		Value or Alarm bits are enabled/disabled
//	this function should be called;
//...................................................................
_DLLEXP ALARM _DLLFUNC BoolAlarm( WRD wrd_ )
{
	LPBTU	_lpBtu;	
	ALARM	_alarm;		//temp value

	CLEAR_LAST_ERROR;
	if(  ( wrd_ >= gBTInfo.wCount )
	   ||(!BoolIsValid(wrd_)) )
	{
		SET_LAST_ERROR( ERR_Invalid_Entries );
		CORE_ASSERT( FALSE );
		return FALSE;
	}

	_lpBtu = gBTInfo.lpBase + wrd_;

	if( _lpBtu->Status & BTUBIT_ENABLEFALSE )
	{
		//value switch to FALSE
		if(  ( !(_lpBtu->Alarm & BTUBIT_ALMSTATE_FALSE) )
		   &&( !(_lpBtu->Value) ) )
		{
			_alarm = _lpBtu->Alarm;	//old alarm state
			_lpBtu->Alarm &= (~(BTUBIT_ACK|BTUBIT_ALMSTATE_MASK));		//clear ACK and old alarm state
			_lpBtu->Alarm |= BTUBIT_ALMSTATE_FALSE;
			
			PostAlarm( TB_BT, wrd_, _lpBtu->Alarm, _alarm );
			return _lpBtu->Alarm;
		}
		
		//value keepes FALSE
		if(  ( (_lpBtu->Alarm & BTUBIT_ALMSTATE_FALSE) )
		   &&( !(_lpBtu->Value) ) )
		{
			return _lpBtu->Alarm;	//do nothing
		}

		//otherwise, goto the following cases
	}

	if( _lpBtu->Status & BTUBIT_ENABLETRUE )
	{
		//value switch to TRUE
		if(  ( !(_lpBtu->Alarm & BTUBIT_ALMSTATE_TRUE) )
		   &&( (_lpBtu->Value) ) )
		{
			_alarm = _lpBtu->Alarm;	//old alarm state
			_lpBtu->Alarm &= (~(BTUBIT_ACK|BTUBIT_ALMSTATE_MASK));		//clear ACK and old alarm state
			_lpBtu->Alarm |= BTUBIT_ALMSTATE_TRUE;
			
			PostAlarm( TB_BT, wrd_, _lpBtu->Alarm, _alarm );
			return _lpBtu->Alarm;
		}
		
		//value keepes TRUE
		if(  ( (_lpBtu->Alarm & BTUBIT_ALMSTATE_TRUE) )
		   &&( (_lpBtu->Value) ) )
		{
			return _lpBtu->Alarm;	//do nothing
		}

		//otherwise, goto the following cases
	}

	////////////////
	//here, value should be normal value or alarm is disabled
	
	//value comes into normal range or alarm thresholds are disabled
	if( _lpBtu->Alarm & BTUBIT_ALMSTATE_MASK )
	{
		_alarm = _lpBtu->Alarm;	//old alarm state
		//clear alarm	
		_lpBtu->Alarm &= (~(BTUBIT_ACK|BTUBIT_ALMSTATE_MASK));		//clear ACK and old alarm state
		PostAlarm( TB_BT, wrd_, _lpBtu->Alarm, _alarm );
		return _lpBtu->Alarm;
	}

	//value keep in normal value
	//do nothing here
	return _lpBtu->Alarm;
}

//...................................................................
//Description:
//	This function sets teh ACK bit and posts alarm message.
//Returns:
//	If no alarms are set, returns FALSE; if alarms have been 
//	acknowledged, return TRUE;
//Note:
//	When operator acknowledges the alarms, this function will
//	be called.
//...................................................................
_DLLEXP BOOL _DLLFUNC BoolAckAlarm( WRD wrd_ )
{
	LPBTU	_lpBtu;	

	CLEAR_LAST_ERROR;
	if(  ( wrd_ >= gBTInfo.wCount )
	   ||(!BoolIsValid(wrd_)) )
	{
		SET_LAST_ERROR( ERR_Invalid_Entries );
		CORE_ASSERT( FALSE );
		return FALSE;
	}

	_lpBtu = gBTInfo.lpBase + wrd_;
	
	if( _lpBtu->Alarm & BTUBIT_ALMSTATE_MASK )
	{
		if( !(_lpBtu->Alarm & BTUBIT_ACK) )	//not ACK yet
		{
			ALARM	_alarm = _lpBtu->Alarm;

			_lpBtu->Alarm |= BTUBIT_ACK;		//set ACK bit
			PostAlarm( TB_BT, wrd_, _lpBtu->Alarm, _alarm ); 
		}
		return TRUE;
	}
	return FALSE;	//no alarm
}

static BOOL BoolInitializeTrace( WRD wrd_ )
{
	LPBTU	_lpBtu;	

	CLEAR_LAST_ERROR;
	if(  ( wrd_ >= gBTInfo.wCount )
	   ||(!BoolIsValid(wrd_)) )
	{
		SET_LAST_ERROR( ERR_Invalid_Entries );
		CORE_ASSERT( FALSE );
		return FALSE;
	}

	_lpBtu = gBTInfo.lpBase + wrd_;

	//first, free the old trace data
	_FreeTraceData( _lpBtu->lpTraceData );
	_lpBtu->lpTraceData = NULL;

	if( _lpBtu->Status & BTUBIT_TRACE )
	{
		DWORD		_items;
		LPTRACE		_lpTrace;

		_items = _lpBtu->TraceTime / gCoreInfo.uInterval;
		if( !_items )
			return TRUE;

		_lpTrace = (LPTRACE)FragAlloc( sizeof(TRACEHEAD) + _items * sizeof(BOOL)  );
		if( !_lpTrace )
			return FALSE;

		//initialize the structure
		_lpTrace->Items = _items;
		_lpTrace->Adding = 0;
		_lpTrace->ValidItems = 0;
		_lpTrace->ItemBits = sizeof(BOOL);

		//set the pointer to BTU
		_lpBtu->lpTraceData = _lpTrace;
	
		return TRUE;
	}
	else
	{
		//need do nothing
		return TRUE;
	}
}
//...................................................................
//...................................................................
_DLLEXP BOOL _DLLFUNC PUReadBoolTable(SimplePU FAR* to_, LPPU from_)
{
	SimplePU 	_tmp;
	LPBTU		_lpBtu;

	if( (from_->du.zzk__WRD == 0) || (from_->du.zzk__WRD >= gBTInfo.wAdding) )
		return FALSE;

	_lpBtu = gBTInfo.lpBase + from_->du.zzk__WRD;

	switch(from_->prefix & 0x00FF)
	{
		case BTU_VALUE:
			_tmp.prefix = PF_IMM|IMM_BOOL;
			_tmp.du.zzk__BOOL = _lpBtu->Value;
			break;

		case BTU_USERDATA:
			_tmp.prefix = PF_IMM|IMM_DWORD;
			_tmp.du.zzk__DWORD = _lpBtu->UserData;
			break;

		case BTU_STATUS:
			_tmp.prefix = PF_IMM|IMM_STATUS;
			_tmp.du.zzk__STATUS = _lpBtu->Status;
			break;

		case BTU_TAG:
			_tmp.prefix = PF_IMM|IMM_LPCOBJ;
			_tmp.du.zzk__LPCOBJ = _lpBtu->lpTag;
			/*
				after calling PUCast, _tmp.du.zzk__LPCOBJ will be cleared.
			*/
			break;

		case BTU_UNIT:
			_tmp.prefix = PF_IMM|IMM_LPCOBJ;
			_tmp.du.zzk__LPCOBJ = _lpBtu->lpUnit;
			/*
				after calling PUCast, _tmp.du.zzk__LPCOBJ will be cleared.
			*/
			break;

		case BTU_COMMENT:
			_tmp.prefix = PF_IMM|IMM_LPCOBJ;
			_tmp.du.zzk__LPCOBJ = _lpBtu->lpComment;
			/*
				after calling PUCast, _tmp.du.zzk__LPCOBJ will be cleared.
			*/
			break;

		case BTU_DIRTY:
			_tmp.prefix = PF_IMM|IMM_WORD;
			_tmp.du.zzk__DIRTY = _lpBtu->Dirty;
			break;

		case BTU_LASTLOG:
			_tmp.prefix = PF_IMM|IMM_BOOL;
			_tmp.du.zzk__BOOL = _lpBtu->LastLog;
			break;

		case BTU_ALARM:
			_tmp.prefix = PF_IMM|IMM_ALARM;
			_tmp.du.zzk__ALARM = _lpBtu->Alarm;
			break;

		case BTU_ALM:	//alarm state
			_tmp.prefix = PF_IMM|IMM_BOOL;
			_tmp.du.zzk__BOOL = _lpBtu->Alarm & BTUBIT_ALMSTATE_MASK;
			break;

		case BTU_ALMFALSE:	//if FALSE-ALARM enable
			_tmp.prefix = PF_IMM|IMM_BOOL;
			_tmp.du.zzk__BOOL = _lpBtu->Alarm & BTUBIT_ALMSTATE_FALSE;
			break;

		case BTU_ALMTRUE:	//alarm state
			_tmp.prefix = PF_IMM|IMM_BOOL;
			_tmp.du.zzk__BOOL = _lpBtu->Alarm & BTUBIT_ALMSTATE_TRUE;
			break;

		case BTU_ANA:
			_tmp.prefix = PF_IMM|IMM_BOOL;
			_tmp.du.zzk__BOOL = 
				(_lpBtu->Alarm & BTUBIT_ALMSTATE_MASK) && (!(_lpBtu->Alarm & BTUBIT_ACK));
			break;

		case BTU_ANAFALSE:
			_tmp.prefix = PF_IMM|IMM_BOOL;
			_tmp.du.zzk__BOOL = 
				(_lpBtu->Alarm & BTUBIT_ALMSTATE_MASK) && (!(_lpBtu->Alarm & BTUBIT_ACK));
			break;

		case BTU_ANATRUE:
			_tmp.prefix = PF_IMM|IMM_BOOL;
			_tmp.du.zzk__BOOL = 
				(_lpBtu->Alarm & BTUBIT_ALMSTATE_MASK) && (!(_lpBtu->Alarm & BTUBIT_ACK));
			break;

		case BTU_ACK:
			_tmp.prefix = PF_IMM|IMM_BOOL;
			_tmp.du.zzk__BOOL = _lpBtu->Alarm & BTUBIT_ACK;
			break;

		case BTU_ACKFALSE:
			_tmp.prefix = PF_IMM|IMM_BOOL;
			_tmp.du.zzk__BOOL = 
				(_lpBtu->Alarm & BTUBIT_ALMSTATE_FALSE) && (_lpBtu->Alarm & BTUBIT_ACK);
			break;

		case BTU_ACKTRUE:
			_tmp.prefix = PF_IMM|IMM_BOOL;
			_tmp.du.zzk__BOOL = 
				(_lpBtu->Alarm & BTUBIT_ALMSTATE_TRUE) && (_lpBtu->Alarm & BTUBIT_ACK);
			break;

		case BTU_ENABLEALM:
			_tmp.prefix = PF_IMM|IMM_STATUS;
			_tmp.du.zzk__STATUS = _lpBtu->Status & BTUBIT_ENABLEMASK;
			break;

		case BTU_ENABLEFALSE:
			_tmp.prefix = PF_IMM|IMM_BOOL;
			_tmp.du.zzk__BOOL = _lpBtu->Status & BTUBIT_ENABLEFALSE;
			break;

		case BTU_ENABLETRUE:
			_tmp.prefix = PF_IMM|IMM_BOOL;
			_tmp.du.zzk__BOOL = _lpBtu->Status & BTUBIT_ENABLETRUE;
			break;

		case BTU_TRACETIME:
			_tmp.prefix = PF_IMM|IMM_DWORD;
			_tmp.du.zzk__DWORD = _lpBtu->TraceTime;
			break;

		case BTU_TRACEDATA:
			_tmp.prefix = PF_IMM|IMM_FPTR;
			_tmp.du.zzk__FPTR = _lpBtu->lpTraceData;
			break;

		case BTU_VALID:
			_tmp.prefix = PF_IMM|IMM_BOOL;
			_tmp.du.zzk__BOOL = _lpBtu->Status & BTUBIT_VALID;
			break;

		case BTU_LOGHIS:
			_tmp.prefix = PF_IMM|IMM_BOOL;
			_tmp.du.zzk__BOOL = _lpBtu->Status & BTUBIT_LOGHIS;
			break;

		case BTU_TRACE:
			_tmp.prefix = PF_IMM|IMM_BOOL;
			_tmp.du.zzk__BOOL = _lpBtu->Status & BTUBIT_TRACE;
			break;

		case BTU_VALUENOT:
			_tmp.prefix = PF_IMM|IMM_BOOL;
			_tmp.du.zzk__BOOL = !(_lpBtu->Value);
			break;

		default:
			return FALSE;
	}

	return PUCast(to_, &_tmp);
}

#define _BTU_SET_DIRTY( _ELE_ )							\
		{_lpBtu->Dirty |= BTUBIT_DIRTY_##_ELE_;}

//...................................................................
//...................................................................
_DLLEXP BOOL _DLLFUNC PUWriteBoolTable(LPPU to_, SimplePU FAR* from_)
{
	SimplePU	_tmp;
	LPBTU		_lpBtu;

	_lpBtu = (gBTInfo.lpBase) + to_->du.zzk__WRD;
	if( !(_lpBtu->Status & BTUBIT_VALID) )
	{
		LOGERR1( "Write to invalid BTU: %d", to_->du.zzk__WRD );
		return FALSE;
	}

	switch(to_->prefix & 0x00FF)
	{
		case BTU_VALUE:
			_tmp.prefix = PF_IMM|IMM_BOOL;
			PUCast(&_tmp, from_);
			if( _lpBtu->Value == _tmp.du.zzk__BOOL )
				return TRUE;	//same value
			_lpBtu->Value = _tmp.du.zzk__BOOL;
			BoolAlarm( to_->du.zzk__WRD );

			_BTU_SET_DIRTY( VALUE )
			break;

		case BTU_TAG:
			/* Note:
			There, from->du.zzk__LPCOBJ must be a Cobject directly,
			and a reference to another string cobject is elligal,
			notice that the following code can't be used this case:
				_tmp.prefix = PF_IMM|IMM_LPCOBJ;
				PUCast(&_tmp, from_);
			here, TRUE can be returned when from_ is a reference to a cobj.
			*/
			if( IS_STRING_COBJ( from_->du.zzk__LPCOBJ ) )
			{
				//need not check here, BoolSetTag will do everything 
				if( BoolSetTag(to_->du.zzk__WRD, PU_GET_COBJ_STRING(from_) ) )
				{
					//if success, delete the imported cobject
					CobjFree( from_->du.zzk__LPCOBJ );
					from_->du.zzk__LPCOBJ = NULL;

					//dirty has been set in BoolSetTag
					//	_BTU_SET_DIRTY( TAG )
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

		case BTU_UNIT:
			if( IS_STRING_COBJ( from_->du.zzk__LPCOBJ ) )
			{
				if( !CobjCompareString( _lpBtu->lpUnit, from_->du.zzk__LPCOBJ ) )
					return TRUE;

				CobjFree( _lpBtu->lpUnit );
				_lpBtu->lpUnit = from_->du.zzk__LPCOBJ;
				from_->du.zzk__LPCOBJ = 0;

				_BTU_SET_DIRTY( UNIT )
				return TRUE;
			}
			else
			{
				LOGERR1("PU data is not a String:PU=%lp", from_);
				return FALSE;
			}
			break;

		case BTU_COMMENT:
			if( IS_STRING_COBJ( from_->du.zzk__LPCOBJ ) )
			{
				if( !CobjCompareString( _lpBtu->lpComment, from_->du.zzk__LPCOBJ ) )
					return TRUE;

				CobjFree ( _lpBtu->lpComment );
				_lpBtu->lpComment = from_->du.zzk__LPCOBJ;
				from_->du.zzk__LPCOBJ = 0;

				_BTU_SET_DIRTY( COMMENT )
				return TRUE;
			}
			else
			{
				LOGERR1("PU data is not a String:PU=%lp", from_);
				return FALSE;
			}

		case BTU_USERDATA:
			_tmp.prefix = PF_IMM|IMM_DWORD;
			PUCast(&_tmp, from_);
			if( _lpBtu->UserData == _tmp.du.zzk__DWORD )
				return TRUE;	//same value
			_lpBtu->UserData = _tmp.du.zzk__DWORD;
			
			_BTU_SET_DIRTY( USERDATA )
			break;

		case BTU_TRACETIME:
		{
			_tmp.prefix = PF_IMM|IMM_DWORD;
			PUCast(&_tmp, from_);
			if( _lpBtu->TraceTime == _tmp.du.zzk__DWORD )
				return TRUE;	//same value
			_lpBtu->TraceTime = _tmp.du.zzk__DWORD;
			
			_BTU_SET_DIRTY( TRACETIME )

			if( IS_RUN_MODE() )
				return BoolInitializeTrace( to_->du.zzk__WRD );
			
			break;
		}

		case BTU_LOGHIS:
			_tmp.prefix = PF_IMM|IMM_BOOL;
			PUCast(&_tmp, from_);
			if(    ( (_lpBtu->Status & BTUBIT_LOGHIS)? TRUE: FALSE )
				== ( _tmp.du.zzk__BOOL?TRUE:FALSE ) )
				return TRUE;	//same value

			_lpBtu->Status ^= (BTUBIT_LOGHIS);		//tag this bit
			_BTU_SET_DIRTY( STATUS )
			break;

		case BTU_TRACE:
		{
			_tmp.prefix = PF_IMM|IMM_BOOL;
			PUCast(&_tmp, from_);
			if(    ( (_lpBtu->Status & BTUBIT_TRACE)? TRUE: FALSE )
				== ( _tmp.du.zzk__BOOL?TRUE:FALSE ) )
				return TRUE;	//same value

			_lpBtu->Status ^= (BTUBIT_TRACE);		//tag this bit
			_BTU_SET_DIRTY( STATUS )
			
			if( IS_RUN_MODE() )
				return BoolInitializeTrace( to_->du.zzk__WRD );
			
			break;
		}

		case BTU_ENABLEFALSE:
			//if double-limit alarm don't permitted
			_tmp.prefix = PF_IMM|IMM_BOOL;
			PUCast(&_tmp, from_);

			if( IS_CO_BOOLDOUBLELIMITALARM() )
			{
				if(    ( (_lpBtu->Status & BTUBIT_ENABLEFALSE)? TRUE: FALSE )
					== ( _tmp.du.zzk__BOOL?TRUE:FALSE ) )
					return TRUE;	//same value
				_lpBtu->Status ^= (BTUBIT_ENABLEFALSE);		//tag this bit
			}
			else
			{	
				STATUS	_status = _tmp.du.zzk__BOOL? BTUBIT_ENABLEFALSE: NULL;

				//only a kind of alarm type is permitted at any time
				if( (_lpBtu->Status & BTUBIT_ENABLEMASK) == _status )
					return TRUE;	//same value
				
				if( _status )	//set it
					_lpBtu->Status = (_lpBtu->Status & (~BTUBIT_ENABLEMASK) ) | _status;
				else	//clear it only
					_lpBtu->Status &= (~BTUBIT_ENABLEFALSE);
			}

			_BTU_SET_DIRTY( STATUS )
			
			BoolAlarm( to_->du.zzk__WRD );
			break;

		case BTU_ENABLETRUE:
			//if double-limit alarm don't permitted
			_tmp.prefix = PF_IMM|IMM_BOOL;
			PUCast(&_tmp, from_);

			if( IS_CO_BOOLDOUBLELIMITALARM() )
			{
				if(    ( (_lpBtu->Status & BTUBIT_ENABLETRUE)? TRUE: FALSE )
					== ( _tmp.du.zzk__BOOL?TRUE:FALSE ) )
					return TRUE;	//same value
				_lpBtu->Status ^= (BTUBIT_ENABLETRUE);		//tag this bit
			}
			else
			{	
				STATUS	_status = _tmp.du.zzk__BOOL? BTUBIT_ENABLETRUE: NULL;

				//only a kind of alarm type is permitted at any time
				if( (_lpBtu->Status & BTUBIT_ENABLEMASK) == _status )
					return TRUE;	//same value

				if( _status )
					_lpBtu->Status = (_lpBtu->Status & (~BTUBIT_ENABLEMASK) ) | _status;
				else
					_lpBtu->Status &= (~BTUBIT_ENABLETRUE);
			}

			_BTU_SET_DIRTY( STATUS )
			
			BoolAlarm( to_->du.zzk__WRD );
			break;

		case BTU_ACK:
			//this function will do everthing
			return BoolAckAlarm( to_->du.zzk__WRD );

		case BTU_VALUENOT:
			_tmp.prefix = PF_IMM|IMM_BOOL;
			PUCast(&_tmp, from_);
			if( _lpBtu->Value == !(_tmp.du.zzk__BOOL) )
				return TRUE;	//same value
			_lpBtu->Value = !(_tmp.du.zzk__BOOL);
			BoolAlarm( to_->du.zzk__WRD );

			_BTU_SET_DIRTY( VALUE )
			break;

		default:
			return FALSE;
	}

	return TRUE;
}

_DLLEXP BOOL _DLLFUNC PUBoolDirty( LPPU lppu_ )
{
	int		_i;
	WORD	_ele;

	CORE_ASSERT( ((lppu_->prefix) & TBX_MASK) == TBX_BT );
	if( lppu_->du.zzk__WRD  >= gBTInfo.wAdding )
	{
		CORE_ASSERT( FALSE );
		return FALSE;
	}

	_ele = lppu_->prefix & XT_MASK;

	for(_i=0; _i<_NUM_NAMESET; _i++)
		if( _ele == ssNameSet[_i].element )
		{
			return (gBTInfo.lpBase + lppu_->du.zzk__WRD)->Dirty & ssNameSet[_i].dirty;
		}

	CORE_ASSERT( FALSE );	//should NOT go here
	return FALSE;	//no found
}