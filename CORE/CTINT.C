//===================================================================
//
//Zheng zhikai	95-10-4
//===================================================================

#include "prehead.h"

#include "core.h"
#include "cpu.h"
#include "cobj.h"
#include "csysobj.h"
#include "ctint.h"
#include "cerror.h"
#include "corerw.h"
#include "calarm.h"

static void TintReleaseRoom(WRD _wrd);
static WRD TintGetFreeRoom(void);
static BOOL TintInitializeTrace( WRD wrd_ );

extern TTInfo		gTTInfo;

extern DWORD		gdwLastError;
extern CoreInfo		gCoreInfo;

struct {
	WORD 	element;
	char 	szName[16];
	VSOF	vsof;
	DIRTY	dirty;	//dirty mask
	WORD	type;	//access type
} static ssNameSet[] =
{
	{TTU_VALUE, 		"Value",		VSOF_REAL,			TTUBIT_DIRTY_VALUE,			TET_PRIMARY },
	{TTU_MIN,			"Min",			VSOF_REAL,			TTUBIT_DIRTY_MIN,			TET_PRIMARY },
	{TTU_MAX,			"Max",			VSOF_REAL,			TTUBIT_DIRTY_MAX,			TET_PRIMARY },
	{TTU_HIHI,			"HiHi",			VSOF_REAL,			TTUBIT_DIRTY_HIHI,			TET_PRIMARY },
	{TTU_HI,			"Hi",			VSOF_REAL,			TTUBIT_DIRTY_HI,			TET_PRIMARY },
	{TTU_LO,			"Lo",			VSOF_REAL,			TTUBIT_DIRTY_LO,			TET_PRIMARY },
	{TTU_LOLO,			"LoLo",			VSOF_REAL,			TTUBIT_DIRTY_LOLO,			TET_PRIMARY },
	{TTU_LOGBAND,		"LogBand",		VSOF_REAL,			TTUBIT_DIRTY_LOGBAND,		TET_PRIMARY },
	{TTU_ALARMBAND,		"AlmBand",		VSOF_REAL,			TTUBIT_DIRTY_ALARMBAND,		TET_PRIMARY },
	{TTU_TAG,			"Tag",			VSOF_LPCOBJ_STR,	TTUBIT_DIRTY_TAG,			TET_PRIMARY },
	{TTU_UNIT,			"Unit",			VSOF_LPCOBJ_STR,	TTUBIT_DIRTY_UNIT,			TET_PRIMARY },
	{TTU_COMMENT,		"Comment",		VSOF_LPCOBJ_STR,	TTUBIT_DIRTY_COMMENT,		TET_PRIMARY },
	{TTU_TRACETIME,		"TraceTime",	VSOF_DWORD_MS,		TTUBIT_DIRTY_TRACETIME,		TET_PRIMARY },
	{TTU_USERDATA,		"UserData",		VSOF_DWORD_HEX,		TTUBIT_DIRTY_USERDATA,		TET_PRIMARY },
	//private primary elements
	{TTU_STATUS,		"Status",		VSOF_STATUS_TTU,	TTUBIT_DIRTY_STATUS,		TET_PRIMARY|TET_READONLY|TET_PRIVATE },
	{TTU_ALARM,			"_alarm",		VSOF_ALARM_TTU,		TTUBIT_DIRTY_ALARM,			TET_PRIMARY|TET_READONLY|TET_PRIVATE },
	{TTU_DIRTY,			"Dirty",		VSOF_DIRTY_TTU,		TTUBIT_DIRTY_DIRTY,			TET_PRIMARY|TET_READONLY|TET_PRIVATE },
	{TTU_LASTLOG,		"LastLog",		VSOF_REAL,			TTUBIT_DIRTY_LASTLOG,		TET_PRIMARY|TET_READONLY|TET_PRIVATE },
	{TTU_TRACEDATA,		"TraceData",	VSOF_FPTR,			TTUBIT_DIRTY_TRACEDATA,		TET_PRIMARY|TET_READONLY|TET_PRIVATE },
	//here, primary data end

	{TTU_SPAN,			"Span",			VSOF_REAL,			TTUBIT_DIRTY_VALUE|TTUBIT_DIRTY_MAX|TTUBIT_DIRTY_MIN,		TET_READONLY },

	{TTU_ENABLELOLO,	"EnableLoLo",	VSOF_BOOL,			TTUBIT_DIRTY_STATUS,		TET_READWRITE },
	{TTU_ENABLELO,		"EnableLo",		VSOF_BOOL,			TTUBIT_DIRTY_STATUS,		TET_READWRITE },
	{TTU_ENABLEHI,		"EnableHi",		VSOF_BOOL,			TTUBIT_DIRTY_STATUS,		TET_READWRITE },
	{TTU_ENABLEHIHI,	"EnableHiHi",	VSOF_BOOL,			TTUBIT_DIRTY_STATUS,		TET_READWRITE },
	{TTU_TRACE,			"Trace",		VSOF_BOOL,			TTUBIT_DIRTY_STATUS,		TET_READWRITE },
	{TTU_LOGHIS,		"LogHis",		VSOF_BOOL,			TTUBIT_DIRTY_STATUS,		TET_READWRITE },
	{TTU_VALID,			"Valid",		VSOF_BOOL,			TTUBIT_DIRTY_STATUS,		TET_READONLY|TET_PRIVATE },

	//Alarm on: ALM_xxx; data type: BOOL
	{TTU_ALM,			"Alm",			VSOF_BOOL,			TTUBIT_DIRTY_ALARM,			TET_READONLY },
	{TTU_ALMHIHI,		"AlmHiHi",		VSOF_BOOL,			TTUBIT_DIRTY_ALARM,			TET_READONLY },
	{TTU_ALMHI,			"AlmHi",		VSOF_BOOL,			TTUBIT_DIRTY_ALARM,			TET_READONLY },
	{TTU_ALMLO,			"AlmLo",		VSOF_BOOL,			TTUBIT_DIRTY_ALARM,			TET_READONLY },
	{TTU_ALMLOLO,		"AlmLoLo",		VSOF_BOOL,			TTUBIT_DIRTY_ALARM,			TET_READONLY },

	//data accessing defines: Alarm without(No) Ack: ANA_xxx
	//data type: BOOL
	{TTU_ANA,			"Ana",			VSOF_BOOL,			TTUBIT_DIRTY_ALARM,		TET_READONLY },
	{TTU_ANAHIHI,		"AnaHiHi",		VSOF_BOOL,			TTUBIT_DIRTY_ALARM,		TET_READONLY },
	{TTU_ANAHI,			"AnaHi",		VSOF_BOOL,			TTUBIT_DIRTY_ALARM,		TET_READONLY },
	{TTU_ANALO,			"AnaLo",		VSOF_BOOL,			TTUBIT_DIRTY_ALARM,		TET_READONLY },
	{TTU_ANALOLO,		"AnaLoLo",		VSOF_BOOL,			TTUBIT_DIRTY_ALARM,		TET_READONLY },

	//data accessing defines: ack
	//data type: BOOL
	{TTU_ACK,			"Ack",			VSOF_BOOL,			TTUBIT_DIRTY_ALARM,		TET_READWRITE },
	{TTU_ACKHIHI,		"AckHiHi",		VSOF_BOOL,			TTUBIT_DIRTY_ALARM,		TET_READONLY },
	{TTU_ACKHI,			"AckHi",		VSOF_BOOL,			TTUBIT_DIRTY_ALARM,		TET_READONLY },
	{TTU_ACKLO,			"AckLo",		VSOF_BOOL,			TTUBIT_DIRTY_ALARM,		TET_READONLY },
	{TTU_ACKLOLO,		"AckLoLo",		VSOF_BOOL,			TTUBIT_DIRTY_ALARM,		TET_READONLY },

	//a STATUS type data to identify which kind of alarm is enabled. 
	//and if any alarm enabled, this value is non-zero
	{TTU_ENABLEALM,		"EnableAlm",	VSOF_WORD_HEX,		TTUBIT_DIRTY_STATUS,		TET_READONLY },
};


#define _NUM_NAMESET	( sizeof(ssNameSet)/sizeof(ssNameSet[0]) )
//...................................................................
//Description:
//	Creates a Tint-unit in the Tint Table;
//Return:
//		the subscription of tint-unit if the function succeeds;
//		Otherwise, it is NULL;
//...................................................................
_DLLEXP WRD _DLLFUNC TintCreate(LPCSTR	lpszTag_,
						TINT	rValue_,
						TINT	rMin_,
						TINT	rMax_)
{
	WRD			_wrd;
	LPCOBJ		_lpCobj = NULL;
	BOOL		_bOk;

	CLEAR_LAST_ERROR;

	_wrd = TintGetFreeRoom();
	if( !_wrd )
	{
		//if no free room, function TintGetFreeRoom(...) set gdwLastError
		return 0;
	}

	memset( gTTInfo.lpBase + _wrd, 0, sizeof(TTU) );

	//allocate the memory for the tag
	_lpCobj = AllocTag( lpszTag_, &_bOk );
	if( !_bOk )		//allocate the tag failed
		return FALSE;

	(gTTInfo.lpBase + _wrd)->lpTag = _lpCobj;
	(gTTInfo.lpBase + _wrd)->Value = rValue_;
	(gTTInfo.lpBase + _wrd)->Min = rMin_;
	(gTTInfo.lpBase + _wrd)->Max = rMax_;

	if( !TintCheckLegality(_wrd) )
	{
		if( (gTTInfo.lpBase + _wrd)->lpTag )
			CobjFree( (gTTInfo.lpBase + _wrd)->lpTag );

		//this function does not change gdwLastError
		TintReleaseRoom(_wrd);
		return 0;
	}

	//Note: here using "=" , not "|="
	(gTTInfo.lpBase + _wrd)->Status = TTUBIT_VALID;
	(gTTInfo.lpBase + _wrd)->Dirty = TTUBIT_DIRTY_ALL;

	DispatchCoreMessage( TBX_TT );
	return _wrd;
}

//...................................................................
//Description:
//	Deletes a TTU;
//Entry:
//	WRD wrd_
//		the subscription of the TTU being freed;
//...................................................................
_DLLEXP BOOL _DLLFUNC TintDelete(WRD wrd_)
{
	LPTTU	_lpTtu;

	CLEAR_LAST_ERROR;
	_lpTtu = gTTInfo.lpBase + wrd_;

	if( wrd_ >= gTTInfo.wCount )
	{
		CORE_ASSERT(FALSE);
		SET_LAST_ERROR( ERR_Invalid_Entries );
		return NULL;
	}

	if( !(_lpTtu->Status & TTUBIT_VALID) )
	{
		CORE_ASSERT( FALSE );
		SET_LAST_ERROR( ERR_Invalid_Entries );
		return FALSE;
	}

	if( GetReference(TB_TT, wrd_, TRUE, NULL) )
	{
		SET_LAST_ERROR( ERR_Referenced_TTU );
		return FALSE;	//the TTU is referenced by other xTUs
	}

	_lpTtu->Status &= (~TTUBIT_VALID);	//clear VALID bit
	_lpTtu->Dirty |= TTUBIT_DIRTY_ALL;

	//free memory occupied by this unit
	FragFree( _lpTtu->lpTraceData );

	CobjFree( _lpTtu->lpTag );
	CobjFree( _lpTtu->lpUnit );
	CobjFree( _lpTtu->lpComment );
	_lpTtu->lpTag = _lpTtu->lpUnit =_lpTtu->lpComment = NULL;
	_lpTtu->lpTraceData = NULL;

	DispatchCoreMessage( TBX_TT );
	return TRUE;
}

//...................................................................
//Description:
//...................................................................
static WRD TintGetFreeRoom()
{
	WRD		_i;

	CLEAR_LAST_ERROR;

	for(_i=1; _i<gTTInfo.wAdding; _i++)
	{
		//Note: the following code is WRONG
		//	if( ((gTTInfo.lpBase + _i)->Status & TTUBIT_VALID) == 0 )
		//
		if(  ((gTTInfo.lpBase + _i)->Status == 0 )
		   &&((gTTInfo.lpBase + _i)->Dirty == 0 ) )
			break;
	}

	if( _i < gTTInfo.wAdding )
		return _i;
	else	// _i == gTTInfo.wAdding
	{
		if( gTTInfo.wAdding < gTTInfo.wCount )
		{
			gTTInfo.wAdding ++;
			return _i;
		}
		else	//no free room
		{
			//if no free room, set last error
			SET_LAST_ERROR( ERR_TTU_No_Free_Room );
			return 0;
		}
	}
}

//...................................................................
//Description:
//	This function frees the room occuped by TTU wrd_;
//Note:
//	The function does NOT change the last error;
//...................................................................
static void TintReleaseRoom(WRD wrd_)
{
	if( wrd_ >= gTTInfo.wAdding )
	{
		CORE_ASSERT( FALSE );
		return ;
	}

	memset( gTTInfo.lpBase + wrd_, 0, sizeof(TTU) );

	//if the TTU[adding-1] is free, decrease Adding
	while(  ((gTTInfo.lpBase + gTTInfo.wAdding - 1)->Status == 0 )
		  &&((gTTInfo.lpBase + gTTInfo.wAdding - 1)->Dirty == 0 )
		  &&( gTTInfo.wAdding > 1 ) )
		gTTInfo.wAdding -- ;

	return ;
}

//...................................................................
//Description:
//  Create a temp. TTU by copying from wrd_
//...................................................................
_DLLEXP WRD _DLLFUNC TintCopyTemp( WRD wrd_ )
{
	LPTTU       _lpTtuTmp, _lpTtuSrc;
	WRD         _wMe;
	LPCOBJ      _lpCobjTmpTag = NULL;
	LPCOBJ      _lpCobjTmpUnit = NULL;
	LPCOBJ      _lpCobjTmpComment = NULL;
	BOOL        _bOk;

	CLEAR_LAST_ERROR;

	_wMe = TintGetFreeRoom();
	if( !_wMe ) return 0; //if no free room

	_lpTtuSrc = gTTInfo.lpBase + wrd_;

	//duplicate the tag, unit and comment
	_lpCobjTmpTag = CobjCopy( _lpTtuSrc->lpTag, &_bOk );
	if( !_bOk )	goto _BAD;
	_lpCobjTmpUnit = CobjCopy( _lpTtuSrc->lpUnit, &_bOk );
	if( !_bOk )	goto _BAD;
	_lpCobjTmpComment = CobjCopy( _lpTtuSrc->lpComment, &_bOk );
	if( !_bOk )	goto _BAD;

	//do not check for legality of the tag

	_lpTtuTmp = (gTTInfo.lpBase) + _wMe;

	//copy old data
	memcpy( _lpTtuTmp, _lpTtuSrc, sizeof(TTU) );    

	//update new data
	_lpTtuTmp->lpTag = _lpCobjTmpTag;
	_lpTtuTmp->lpUnit = _lpCobjTmpUnit;
	_lpTtuTmp->lpComment = _lpCobjTmpComment;
	_lpTtuTmp->Status |= TTUBIT_VALID|TTUBIT_TEMP;

	return _wMe;

_BAD:
	if( _lpCobjTmpTag ) CobjFree( _lpCobjTmpTag );
	if( _lpCobjTmpUnit ) CobjFree( _lpCobjTmpUnit );
	if( _lpCobjTmpComment ) CobjFree( _lpCobjTmpComment );

	TintReleaseRoom( _wMe );
	return FALSE;
}

//...................................................................
//Description:
//...................................................................
_DLLEXP BOOL _DLLFUNC TintDeleteTemp(WRD wrd_)
{
	LPTTU   _lpTtu = gTTInfo.lpBase + wrd_;

	CLEAR_LAST_ERROR;

	if(   (_lpTtu->Status & TTUBIT_VALID)
	   && (_lpTtu->Status & TTUBIT_TEMP) )
	{
		CobjFree( _lpTtu->lpTag );
		CobjFree( _lpTtu->lpUnit );
		CobjFree( _lpTtu->lpComment );
		_lpTtu->lpTag = _lpTtu->lpUnit =_lpTtu->lpComment = NULL;

		TintReleaseRoom(wrd_);

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
//  Undo a TTU specified by wTint_ using its undo buffer: wBuffer_
//and then Delete the undo buffer TTU.
//...................................................................
_DLLEXP BOOL _DLLFUNC TintUndo( WRD wTint_, WRD wBuffer_ )
{
	LPTTU   _lpTtu, _lpTtuBuffer;

	if( !wTint_ || !wBuffer_ )
	{
		SET_LAST_ERROR( ERR_Invalid_Entries );
		return FALSE;
	}
	_lpTtu = gTTInfo.lpBase + wTint_;
	_lpTtuBuffer = gTTInfo.lpBase + wBuffer_;

	//free the dirty tag, unit and comment
	CobjFree( _lpTtu->lpTag );	
	CobjFree( _lpTtu->lpUnit );	
	CobjFree( _lpTtu->lpComment );

	//undo the parameters of the TTU and other elements
	//NOTE: the tag of the undo buffer TTU is sent to the wTint_
	memcpy( _lpTtu, _lpTtuBuffer, sizeof( TTU) );
	_lpTtu->Status &= (~TTUBIT_TEMP);    //clear TEMP bit

	//delete the undo buffer TTU
	memset( _lpTtuBuffer, 0, sizeof(TTU) );
	TintReleaseRoom( wBuffer_ );

	return TRUE;
}

//...................................................................
//Description:
//	This function only checks the TTUBIT_VALID bit;
//...................................................................
_DLLEXP BOOL _DLLFUNC TintIsValid( WRD wrd_ )
{
	if(  ( wrd_ < gTTInfo.wCount )
	   &&( (gTTInfo.lpBase + wrd_)->Status & TTUBIT_VALID ) )
		return TRUE;
	else return FALSE;
}

//...................................................................
//Description:
//	Checks the legality of a TTU even the TTU is invald, including:
//		. legality of tag(ERROR CODE: ERR_Illegal_Tag)
//		. an unique of tag in the table(ERROR CODE: ERR_Not_Unique_Tag)
//Entry:
//	TTU FAR* lp_
//		source tint-unit. returns 0 if lp_ == NULL;
//Return:
//	TRUE/FALSE
//Note:
//	The old version checks the following rule
//		. min <= value <= max (ERROR CODE: ERR_Invalid_Min_Value_Max)
//	Now the check is deleted;
//...................................................................
_DLLEXP BOOL _DLLFUNC TintCheckLegality(WRD wrd_)
{
	CLEAR_LAST_ERROR;
	if( wrd_ >= gTTInfo.wCount )
	{
		CORE_ASSERT(FALSE);
		SET_LAST_ERROR( ERR_Invalid_Entries );
		return NULL;
	}

	if( !TintCheckTag( wrd_ ) )
		return FALSE;
	if( !TintCheckMinMax( wrd_ ) )
		return FALSE;
	if( !TintCheckAlarmThreshold( wrd_ ) )
		return FALSE;

	return TRUE;
}

//...................................................................
//Description:
//Entry:
//Return:
//...................................................................
_DLLEXP BOOL _DLLFUNC TintCheckTag(WRD wrd_)
{
	WRD		_i;
	LPTTU 	_lp = gTTInfo.lpBase + wrd_;

	CLEAR_LAST_ERROR;
	if( wrd_ >= gTTInfo.wCount )
	{
		CORE_ASSERT(FALSE);
		SET_LAST_ERROR( ERR_Invalid_Entries );
		return NULL;
	}

	if(	_lp->lpTag == NULL )
		return TRUE;

	if( !IsTagLegal( pTINT_GET_TAG(_lp) ) )
	{
		return FALSE;
	}

	//check on whether it's an unique tag in the tint-table
	for(_i=1; _i<gTTInfo.wAdding; _i++)
	{
		if( (gTTInfo.lpBase[_i]).Status & TTUBIT_TEMP )
			continue;	//don't check temp TTU
		if(  ((gTTInfo.lpBase[_i]).Status & TTUBIT_VALID )
		   &&((gTTInfo.lpBase[_i]).lpTag) )
		{
			if( CmpTag( TINT_GET_TAG(_i),
						pTINT_GET_TAG(_lp),
						CORE_MAX_TAG_LENGTH ) == 0)
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
_DLLEXP BOOL _DLLFUNC TintCheckMinMax(WRD wrd_)
{
	LPTTU 	_lpTtu = gTTInfo.lpBase + wrd_;

	CLEAR_LAST_ERROR;
	if( wrd_ >= gTTInfo.wCount )
	{
		CORE_ASSERT(FALSE);
		SET_LAST_ERROR( ERR_Invalid_Entries );
		return FALSE;
	}

	if( !( _lpTtu->Min < _lpTtu->Max ) )
	{
		SET_LAST_ERROR( ERR_MAX_and_MIN );
		return FALSE;
	}

	return TRUE;
}

//...................................................................
//Description:
//Entry:
//Return:
//...................................................................
_DLLEXP BOOL _DLLFUNC TintCheckAlarmThreshold(WRD wrd_)
{
	LPTTU 	_lpTtu = gTTInfo.lpBase + wrd_;

	CLEAR_LAST_ERROR;
	if( wrd_ >= gTTInfo.wCount )
	{
		CORE_ASSERT(FALSE);
		SET_LAST_ERROR( ERR_Invalid_Entries );
		return NULL;
	}

	//check: HiHi > (Hi, Lo, LoLo)
	if( _lpTtu->Status & TTUBIT_ENABLEHIHI )
		if( _lpTtu->Status & TTUBIT_ENABLEHI )
			if( !( _lpTtu->HiHi > _lpTtu->Hi ) )
			{
				SET_LAST_ERROR( ERR_HIHI_and_HI );
				return FALSE;
			}
		else
			if( _lpTtu->Status & TTUBIT_ENABLELO )
				if( !( _lpTtu->HiHi > _lpTtu->Lo ) )
				{
					SET_LAST_ERROR( ERR_HIHI_and_LO );
					return FALSE;
				}
			else
				if( _lpTtu->Status & TTUBIT_ENABLELOLO )
					if( !( _lpTtu->HiHi > _lpTtu->LoLo ) )
					{
						SET_LAST_ERROR( ERR_HIHI_and_LOLO );
						return FALSE;
					}

	//check: Hi > (Lo, LoLo)
	if( _lpTtu->Status & TTUBIT_ENABLEHI )
		if( _lpTtu->Status & TTUBIT_ENABLELO )
			if( !( _lpTtu->LoLo > _lpTtu->Lo ) )
			{
				SET_LAST_ERROR( ERR_HI_and_LO );
				return FALSE;
			}
		else
			if( _lpTtu->Status & TTUBIT_ENABLELOLO )
				if( !( _lpTtu->Hi > _lpTtu->LoLo ) )
				{
					SET_LAST_ERROR( ERR_HI_and_LOLO );
					return FALSE;
				}

	//check: Lo > (LoLo)
	if( _lpTtu->Status & TTUBIT_ENABLELO )
		if( _lpTtu->Status & TTUBIT_ENABLELOLO )
			if( !( _lpTtu->Lo > _lpTtu->LoLo ) )
			{
				SET_LAST_ERROR( ERR_LO_and_LOLO );
				return FALSE;
			}

	return TRUE;
}

//...................................................................
//Description:
//Entry:
//Return:
//Note:
//	if the TTU that matchs the tag szTag_ is deleted, returns 0;
//...................................................................
_DLLEXP WRD _DLLFUNC TintGetWRD(LPCSTR szTag_)
{
	WRD	_i;

	if(*szTag_ == '#')
	{
		WRD _wrd = TagToWRD(szTag_);
		if( _wrd == NULL)
			return 0;
		else
		{
			if( (gTTInfo.lpBase[_wrd]).Status & TTUBIT_VALID )
				return _wrd;
			else
				return 0;
		}
	}

	for(_i=1; _i<gTTInfo.wAdding; _i++)
	{
		if(  (gTTInfo.lpBase[_i].Status & TTUBIT_VALID )
		   &&((gTTInfo.lpBase[_i]).lpTag)  )
		{
			if( !CmpTag( TINT_GET_TAG(_i),
						 szTag_,
						 CORE_MAX_TAG_LENGTH ) )
				return _i;
		}
	}

	return 0;
}

//...................................................................
//Description:
//Entry:
//Return:
//	0:			failed, no name returns
//	1:			successful with tint name
//	2:			successful with pseudo name
//...................................................................
_DLLEXP int _DLLFUNC TintGetTag(WRD wrd_, LPSTR szTag_, WORD wSize_)
{
	int		_ret = 0;

	CLEAR_LAST_ERROR;

	if( wrd_ >= gTTInfo.wCount )
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
		LPTTU 	_lpTtu;
		_lpTtu = gTTInfo.lpBase + wrd_;

		if( _lpTtu->Status & TTUBIT_VALID )
		{
			if( (_lpTtu->lpTag) && *(_lpTtu->lpTag->data) )
			{
				StringCopyn( szTag_, pTINT_GET_TAG(_lpTtu), wSize_ );
				_ret = 1;	//tint name
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
//Note:
//	In the release version, the Status can be read even the TTU
//is invalid, this is more resionable than returns a zero when
//the TTU is invalid;
//...................................................................
_DLLEXP STATUS _DLLFUNC TintGetStatus(WRD wrd_)
{
	if( wrd_ >= gTTInfo.wCount )
	{
		CORE_ASSERT(FALSE);
		SET_LAST_ERROR( ERR_Invalid_Entries );
		return NULL;
	}

	CORE_ASSERT( gTTInfo.lpBase[wrd_].Status & TTUBIT_VALID );
	return (gTTInfo.lpBase)[wrd_].Status;
}

//...................................................................
//Note:
//	In the release version, the Status can be read even the TTU
//is invalid, this is more resionable than returns a zero when
//the TTU is invalid;
//...................................................................
_DLLEXP STATUS _DLLFUNC TintSetStatus( WRD wrd_, STATUS status_ )
{
	STATUS _ret;
	
	if( wrd_ >= gTTInfo.wCount )
	{
		CORE_ASSERT(FALSE);
		SET_LAST_ERROR( ERR_Invalid_Entries );
		return NULL;
	}

	CORE_ASSERT( gTTInfo.lpBase[wrd_].Status & TTUBIT_VALID );
	_ret = (gTTInfo.lpBase)[wrd_].Status;

	(gTTInfo.lpBase)[wrd_].Status = status_;

	return _ret;
}

//...................................................................
//Note:
//	In the release version, the value can be read even the TTU
//is invalid, this is more resionable than returns a zero when
//the TTU is invalid;
//...................................................................
_DLLEXP DIRTY _DLLFUNC TintGetDirty(WRD wrd_)
{
	if( wrd_ >= gTTInfo.wCount )
	{
		CORE_ASSERT(FALSE);
		SET_LAST_ERROR( ERR_Invalid_Entries );
		return NULL;
	}

	CORE_ASSERT( gTTInfo.lpBase[wrd_].Status & TTUBIT_VALID );
	return (gTTInfo.lpBase)[wrd_].Dirty;
}

//...................................................................
//Decription:
//	Set the Tint tag if the new tag is legal after checked;
//Note:
//	if the new tag begins with '#', returns FALSE;
//...................................................................
_DLLEXP BOOL _DLLFUNC TintSetTag(WRD wrd_, LPCSTR szTag_)
{
	BOOL		_bOk;
	LPTTU		_lpTtu = (gTTInfo.lpBase) + wrd_;

	if( !szTag_ )
	{
		if( _lpTtu->lpTag )
			_lpTtu->Dirty |= TTUBIT_DIRTY_TAG;

		CobjFree( _lpTtu->lpTag );
		_lpTtu->lpTag = NULL;
		return TRUE;
	}

	if( *szTag_ == '#' )
		return FALSE;

	if( !CmpTag( pTINT_GET_TAG(_lpTtu), szTag_, CORE_MAX_TAG_LENGTH ) )
		return TRUE;	//the old and new tag are identical

	if( _lpTtu->Status & TTUBIT_VALID )
	{
		LPCOBJ	_lpTagOld, _lpTagNew;

		if( strlen(szTag_) >= CORE_MAX_TAG_LENGTH - 1)
			return FALSE;

		//save the old tag
		_lpTagOld = _lpTtu->lpTag;
		_lpTagNew = AllocTag( szTag_, &_bOk );

		if( !_bOk )
			return FALSE;

		_lpTtu->lpTag = _lpTagNew;

		if( !TintCheckLegality( wrd_ ) )
		{
			//the new tag is illegal
			CobjFree( _lpTagNew );
			_lpTtu->lpTag = _lpTagOld;
			return FALSE;
		}
		else
		{
			//set tag successful, and delete the old tag
			CobjFree( _lpTagOld );
			_lpTtu->Dirty |= TTUBIT_DIRTY_TAG;
			return TRUE;
		}
	}

	return FALSE;
}

//...................................................................
//...................................................................
_DLLEXP TTInfo FAR* _DLLFUNC GetTintTableInformation()
{
	return &gTTInfo;
}

//...................................................................
//...................................................................
_DLLEXP WORD _DLLFUNC TintGetElementType( WORD element_ )
{
	int		_i;

	for(_i=0; _i<_NUM_NAMESET; _i++)
		if(element_ == ssNameSet[_i].element)
			return ssNameSet[_i].type;

	return 0;
}

//...................................................................
//...................................................................
_DLLEXP BOOL _DLLFUNC TintGetElementName(WORD element_, LPSTR szName_, WORD wSize_)
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
_DLLEXP WORD _DLLFUNC TintGetElement(LPSTR szName_)
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
_DLLEXP LPSTR _DLLFUNC TintGetData(LPSTR lpsz_, int size_, WRD wrd_, WORD order_)
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

	_pu.prefix = PF_TBX|TBX_TT|order_;
	_pu.du.zzk__WRD = wrd_;
	_ret = PUGetValueString( &_pu, &_gvs, ssNameSet[_i].vsof );

	return _ret? lpsz_: NULL;
}

//...................................................................
//...................................................................
_DLLEXP BOOL _DLLFUNC TintSetData(LPCSTR lpsz_, WRD wrd_, WORD order_)
{
	PU			_pu;
	DWORD		_ret;
	
	int			_i;

	if(  ( wrd_ >= gTTInfo.wCount )
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

	_pu.prefix = PF_TBX|TBX_TT|order_;
	_pu.du.zzk__WRD = wrd_;
	_ret = PUSetValueString( &_pu, lpsz_, ssNameSet[_i].vsof );

	return _ret? TRUE: FALSE;
}

//...................................................................
//Entries:
//	bPublicDirty_
//		If this is TRUE, the private dirty bits are excluded.
//...................................................................
_DLLEXP BOOL _DLLFUNC IsTintTableDirty( BOOL bPublicDirty_ )
{
	WRD		_i;
	DIRTY	_mask = TTUBIT_DIRTY_ALL;

	if( bPublicDirty_ )
		_mask &= ~( TTUBIT_DIRTY_ALARM|
					TTUBIT_DIRTY_LASTLOG|
					TTUBIT_DIRTY_TRACEDATA|
					TTUBIT_DIRTY_DIRTY);

	for(_i=1; _i<gTTInfo.wAdding; _i++)
	{
		if( (gTTInfo.lpBase + _i)->Dirty & _mask )
			return TRUE;
	}

	return FALSE;
}
//...................................................................
//...................................................................
_DLLEXP void _DLLFUNC TintClearTableDirty()
{
	WRD		_i;

	for(_i=1; _i<gTTInfo.wAdding; _i++)
		(gTTInfo.lpBase + _i)->Dirty &= (~TTUBIT_DIRTY_ALL);

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
//		Value, HiHi, Hi, Lo, LoLo, or Alarm bits are enabled/disabled
//	this function should be called;
//...................................................................
_DLLEXP ALARM _DLLFUNC TintAlarm( WRD wrd_ )
{
	LPTTU	_lpTtu;	
	ALARM	_alarm;		//temp value

	CLEAR_LAST_ERROR;
	if(  ( wrd_ >= gTTInfo.wCount )
	   ||(!TintIsValid(wrd_)) )
	{
		SET_LAST_ERROR( ERR_Invalid_Entries );
		CORE_ASSERT( FALSE );
		return FALSE;
	}

	_lpTtu = gTTInfo.lpBase + wrd_;

	//we check HiHi first
	if( _lpTtu->Status & TTUBIT_ENABLEHIHI )
	{
		//value touches HiHi
		if(  ( !(_lpTtu->Alarm & TTUBIT_ALMSTATE_HIHI) )
		   &&( _lpTtu->Value >= _lpTtu->HiHi + _lpTtu->AlarmBand ) )	//Note: here is "+"
		{
			_alarm = _lpTtu->Alarm;	//old alarm state
			_lpTtu->Alarm &= (~(TTUBIT_ACK|TTUBIT_ALMSTATE_MASK));		//clear ACK and old alarm state
			_lpTtu->Alarm |= TTUBIT_ALMSTATE_HIHI;
			
			PostAlarm( TB_TT, wrd_, _lpTtu->Alarm, _alarm );
			return _lpTtu->Alarm;
		}
		
		//value keepes HiHi
		if(  ( (_lpTtu->Alarm & TTUBIT_ALMSTATE_HIHI) )
		   &&( _lpTtu->Value >= _lpTtu->HiHi - _lpTtu->AlarmBand ) )	//Note: here is "-"
		{
			return _lpTtu->Alarm;	//do nothing
		}

		//otherwise, goto the following cases
	}

	//then we check LoLo before Lo
	if( _lpTtu->Status & TTUBIT_ENABLELOLO )
	{
		//value touches LOLO
		if(  ( !(_lpTtu->Alarm & TTUBIT_ALMSTATE_LOLO) )
		   &&( _lpTtu->Value <= _lpTtu->LoLo - _lpTtu->AlarmBand ) )	//Note: here is "-"
		{
			_alarm = _lpTtu->Alarm;	 //old alarm state
			_lpTtu->Alarm &= (~(TTUBIT_ACK|TTUBIT_ALMSTATE_MASK));		//clear ACK and old alarm state
			_lpTtu->Alarm |= TTUBIT_ALMSTATE_LOLO;
			
			PostAlarm( TB_TT, wrd_, _lpTtu->Alarm, _alarm );
			return _lpTtu->Alarm;
		}
		
		//value keepes LOLO
		if(  ( (_lpTtu->Alarm & TTUBIT_ALMSTATE_LOLO) )
		   &&( _lpTtu->Value <= _lpTtu->LoLo + _lpTtu->AlarmBand ) )	//Note: here is "+"
		{
			return _lpTtu->Alarm;	//do nothing
		}

		//otherwise, goto the following cases
	}

	//then we check Hi or Lo
	if( _lpTtu->Status & TTUBIT_ENABLELO )
	{
		//value touches LO
		if(  ( !(_lpTtu->Alarm & TTUBIT_ALMSTATE_LO) )
		   &&( _lpTtu->Value <= _lpTtu->Lo - _lpTtu->AlarmBand ) )	//Note: here is "-"
		{
			_alarm = _lpTtu->Alarm;	 //old alarm state
			_lpTtu->Alarm &= (~(TTUBIT_ACK|TTUBIT_ALMSTATE_MASK));		//clear ACK and old alarm state
			_lpTtu->Alarm |= TTUBIT_ALMSTATE_LO;
			
			PostAlarm( TB_TT, wrd_, _lpTtu->Alarm, _alarm );
			return _lpTtu->Alarm;
		}
		
		//value keepes LO
		if(  ( (_lpTtu->Alarm & TTUBIT_ALMSTATE_LO) )
		   &&( _lpTtu->Value <= _lpTtu->Lo + _lpTtu->AlarmBand ) )	//Note: here is "+"
		{
			return _lpTtu->Alarm;	//do nothing
		}

		//otherwise, goto the following cases
	}

	//check Hi here
	if( _lpTtu->Status & TTUBIT_ENABLEHI )
	{
		//value touches Hi
		if(  ( !(_lpTtu->Alarm & TTUBIT_ALMSTATE_HI) )
		   &&( _lpTtu->Value >= _lpTtu->Hi + _lpTtu->AlarmBand ) )	//Note: here is "+"
		{
			_alarm = _lpTtu->Alarm;	//old alarm state
			_lpTtu->Alarm &= (~(TTUBIT_ACK|TTUBIT_ALMSTATE_MASK));		//clear ACK and old alarm state
			_lpTtu->Alarm |= TTUBIT_ALMSTATE_HI;
			
			PostAlarm( TB_TT, wrd_, _lpTtu->Alarm, _alarm );
			return _lpTtu->Alarm;
		}
		
		//value keepes Hi
		if(  ( (_lpTtu->Alarm & TTUBIT_ALMSTATE_HI) )
		   &&( _lpTtu->Value >= _lpTtu->Hi - _lpTtu->AlarmBand ) )	//Note: here is "-"
		{
			return _lpTtu->Alarm;	//do nothing
		}

		//otherwise, goto the following cases
	}

	////////////////
	//here, value should be in normal range
	
	//value comes into normal range or alarm thresholds are disabled
	if( _lpTtu->Alarm & TTUBIT_ALMSTATE_MASK )
	{
		_alarm = _lpTtu->Alarm;	//old alarm state
		//clear alarm	
		_lpTtu->Alarm &= (~(TTUBIT_ACK|TTUBIT_ALMSTATE_MASK));		//clear ACK and old alarm state
		PostAlarm( TB_TT, wrd_, _lpTtu->Alarm, _alarm );
		return _lpTtu->Alarm;
	}

	//value keep in normal range
	//do nothing here
	return _lpTtu->Alarm;
}

static BOOL TintInitializeTrace( WRD wrd_ )
{
	LPTTU	_lpTtu;	

	CLEAR_LAST_ERROR;
	if(  ( wrd_ >= gTTInfo.wCount )
	   ||(!TintIsValid(wrd_)) )
	{
		SET_LAST_ERROR( ERR_Invalid_Entries );
		CORE_ASSERT( FALSE );
		return FALSE;
	}

	_lpTtu = gTTInfo.lpBase + wrd_;

	//first, free the old trace data
	_FreeTraceData( _lpTtu->lpTraceData );
	_lpTtu->lpTraceData = NULL;

	if( _lpTtu->Status & TTUBIT_TRACE )
	{
		DWORD		_items;
		LPTRACE		_lpTrace;

		_items = _lpTtu->TraceTime / gCoreInfo.uInterval;
		if( !_items )
			return TRUE;

		_lpTrace = (LPTRACE)FragAlloc( sizeof(TRACEHEAD) + _items * sizeof(TINT)  );
		if( !_lpTrace )
			return FALSE;

		//initialize the structure
		_lpTrace->Items = _items;
		_lpTrace->Adding = 0;
		_lpTrace->ValidItems = 0;
		_lpTrace->ItemBits = sizeof(TINT);

		//set the pointer to TTU
		_lpTtu->lpTraceData = _lpTrace;
	
		return TRUE;
	}
	else
	{
		//need do nothing
		return TRUE;
	}
}

//...................................................................
//Description:
//	This function sets teh ACK bit and posts alarm message.
//Returns:
//	If no alarms are set, returns FALSE; if alarms have been 
//	acknowledged, return TRUE;
//Note:
//	When operator acknowledges the alarms, this function will
//	be called
//...................................................................
_DLLEXP BOOL _DLLFUNC TintAckAlarm( WRD wrd_ )
{
	LPTTU	_lpTtu;	

	CLEAR_LAST_ERROR;
	if(  ( wrd_ >= gTTInfo.wCount )
	   ||(!TintIsValid(wrd_)) )
	{
		SET_LAST_ERROR( ERR_Invalid_Entries );
		CORE_ASSERT( FALSE );
		return FALSE;
	}

	_lpTtu = gTTInfo.lpBase + wrd_;
	
	if( _lpTtu->Alarm & TTUBIT_ALMSTATE_MASK )
	{
		if( !(_lpTtu->Alarm & TTUBIT_ACK) )	//not ACK yet
		{
			ALARM	_alarm = _lpTtu->Alarm;

			_lpTtu->Alarm |= TTUBIT_ACK;		//set ACK bit
			PostAlarm( TB_TT, wrd_, _lpTtu->Alarm, _alarm ); 
		}
		return TRUE;
	}
	return FALSE;	//no alarm
}

//...................................................................
//...................................................................
_DLLEXP BOOL _DLLFUNC PUReadTintTable(SimplePU FAR* to_, LPPU from_)
{
	LPTTU		_lpTtu;
	SimplePU 	_tmp;

	if( (from_->du.zzk__WRD == 0) || (from_->du.zzk__WRD >= gTTInfo.wAdding) )
		return FALSE;

	_lpTtu = gTTInfo.lpBase + from_->du.zzk__WRD;

	switch(from_->prefix & XT_MASK)
	{
		case TTU_VALUE:
			_tmp.prefix = PF_IMM|IMM_TINT;
			_tmp.du.zzk__TINT = _lpTtu->Value;
			break;

		case TTU_MIN:
			_tmp.prefix = PF_IMM|IMM_TINT;
			_tmp.du.zzk__TINT = _lpTtu->Min;
			break;

		case TTU_MAX:
			_tmp.prefix = PF_IMM|IMM_TINT;
			_tmp.du.zzk__TINT = _lpTtu->Max;
			break;

		case TTU_USERDATA:
			_tmp.prefix = PF_IMM|IMM_DWORD;
			_tmp.du.zzk__DWORD = _lpTtu->UserData;
			break;

		case TTU_SPAN:
			_tmp.prefix = PF_IMM|IMM_TINT;
			_tmp.du.zzk__TINT = _lpTtu->Max - _lpTtu->Min;
			break;

		case TTU_STATUS:
			_tmp.prefix = PF_IMM|IMM_STATUS;
			_tmp.du.zzk__STATUS = _lpTtu->Status;
			break;

		case TTU_TAG:
			_tmp.prefix = PF_IMM|IMM_LPCOBJ;
			_tmp.du.zzk__LPCOBJ = _lpTtu->lpTag;
			/*
				after calling PUCast, _tmp.du.zzk__LPCOBJ will be cleared.
			*/
			break;

		case TTU_UNIT:
			_tmp.prefix = PF_IMM|IMM_LPCOBJ;
			_tmp.du.zzk__LPCOBJ = _lpTtu->lpUnit;
			/*
				after calling PUCast, _tmp.du.zzk__LPCOBJ will be cleared.
			*/
			break;

		case TTU_COMMENT:
			_tmp.prefix = PF_IMM|IMM_LPCOBJ;
			_tmp.du.zzk__LPCOBJ = _lpTtu->lpComment;
			/*
				after calling PUCast, _tmp.du.zzk__LPCOBJ will be cleared.
			*/
			break;

		case TTU_DIRTY:
			_tmp.prefix = PF_IMM|IMM_DIRTY;
			_tmp.du.zzk__DIRTY = _lpTtu->Dirty;
			break;

		case TTU_LOGBAND:
			_tmp.prefix = PF_IMM|IMM_TINT;
			_tmp.du.zzk__TINT = _lpTtu->LogBand;
			break;

		case TTU_LASTLOG:
			_tmp.prefix = PF_IMM|IMM_TINT;
			_tmp.du.zzk__TINT = _lpTtu->LastLog;
			break;

		case TTU_ALARMBAND:
			_tmp.prefix = PF_IMM|IMM_TINT;
			_tmp.du.zzk__TINT = _lpTtu->AlarmBand;
			break;

		case TTU_ALARM:
			_tmp.prefix = PF_IMM|IMM_ALARM;
			_tmp.du.zzk__ALARM = _lpTtu->Alarm;
			break;

		case TTU_HIHI:
			_tmp.prefix = PF_IMM|IMM_TINT;
			_tmp.du.zzk__TINT = _lpTtu->HiHi;
			break;

		case TTU_HI:
			_tmp.prefix = PF_IMM|IMM_TINT;
			_tmp.du.zzk__TINT = _lpTtu->Hi;
			break;

		case TTU_LO:
			_tmp.prefix = PF_IMM|IMM_TINT;
			_tmp.du.zzk__TINT = _lpTtu->Lo;
			break;

		case TTU_LOLO:
			_tmp.prefix = PF_IMM|IMM_TINT;
			_tmp.du.zzk__TINT = _lpTtu->LoLo;
			break;

		case TTU_ALM:	//alarm state
			_tmp.prefix = PF_IMM|IMM_ALARM;
			_tmp.du.zzk__ALARM = _lpTtu->Alarm & TTUBIT_ALMSTATE_MASK;
			break;

		case TTU_ALMHIHI:
			_tmp.prefix = PF_IMM|IMM_BOOL;
			_tmp.du.zzk__BOOL = _lpTtu->Alarm & TTUBIT_ALMSTATE_HIHI;
			break;

		case TTU_ALMHI:
			_tmp.prefix = PF_IMM|IMM_BOOL;
			_tmp.du.zzk__BOOL = _lpTtu->Alarm & TTUBIT_ALMSTATE_HI;
			break;

		case TTU_ALMLO:
			_tmp.prefix = PF_IMM|IMM_BOOL;
			_tmp.du.zzk__BOOL = _lpTtu->Alarm & TTUBIT_ALMSTATE_LO;
			break;

		case TTU_ALMLOLO:
			_tmp.prefix = PF_IMM|IMM_BOOL;
			_tmp.du.zzk__BOOL = _lpTtu->Alarm & TTUBIT_ALMSTATE_LOLO;
			break;

		case TTU_ANA:
			_tmp.prefix = PF_IMM|IMM_BOOL;
			_tmp.du.zzk__BOOL = 
				(_lpTtu->Alarm & TTUBIT_ALMSTATE_MASK) && (!(_lpTtu->Alarm & TTUBIT_ACK));
			break;

		case TTU_ANAHIHI:
			_tmp.prefix = PF_IMM|IMM_BOOL;
			_tmp.du.zzk__BOOL = 
				(_lpTtu->Alarm & TTUBIT_ALMSTATE_HIHI) && (!(_lpTtu->Alarm & TTUBIT_ACK));
				break;

		case TTU_ANAHI:
			_tmp.prefix = PF_IMM|IMM_BOOL;
			_tmp.du.zzk__BOOL = 
				(_lpTtu->Alarm & TTUBIT_ALMSTATE_HI) && (!(_lpTtu->Alarm & TTUBIT_ACK));
			break;

		case TTU_ANALO:
			_tmp.prefix = PF_IMM|IMM_BOOL;
			_tmp.du.zzk__BOOL = 
				(_lpTtu->Alarm & TTUBIT_ALMSTATE_LO) && (!(_lpTtu->Alarm & TTUBIT_ACK));
			break;

		case TTU_ANALOLO:
			_tmp.prefix = PF_IMM|IMM_BOOL;
			_tmp.du.zzk__BOOL = 
				(_lpTtu->Alarm & TTUBIT_ALMSTATE_LOLO) && (!(_lpTtu->Alarm & TTUBIT_ACK));
			break;

		case TTU_ACK:
			_tmp.prefix = PF_IMM|IMM_BOOL;
			_tmp.du.zzk__BOOL = _lpTtu->Alarm & TTUBIT_ACK;
			break;

		case TTU_ACKHIHI:
			_tmp.prefix = PF_IMM|IMM_BOOL;
			_tmp.du.zzk__BOOL = 
				(_lpTtu->Alarm & TTUBIT_ALMSTATE_HIHI) && (_lpTtu->Alarm & TTUBIT_ACK);
			break;

		case TTU_ACKHI:
			_tmp.prefix = PF_IMM|IMM_BOOL;
			_tmp.du.zzk__BOOL = 
				(_lpTtu->Alarm & TTUBIT_ALMSTATE_HI) && (_lpTtu->Alarm & TTUBIT_ACK);
				break;

		case TTU_ACKLO:
			_tmp.prefix = PF_IMM|IMM_BOOL;
			_tmp.du.zzk__BOOL = 
				(_lpTtu->Alarm & TTUBIT_ALMSTATE_LO) && (_lpTtu->Alarm & TTUBIT_ACK);
				break;

		case TTU_ACKLOLO:
			_tmp.prefix = PF_IMM|IMM_BOOL;
			_tmp.du.zzk__BOOL = 
				(_lpTtu->Alarm & TTUBIT_ALMSTATE_LOLO) && (_lpTtu->Alarm & TTUBIT_ACK);
			break;

		case TTU_ENABLEALM:
			_tmp.prefix = PF_IMM|IMM_STATUS;
			_tmp.du.zzk__STATUS = _lpTtu->Status & TTUBIT_ENABLEMASK;
			break;

		case TTU_ENABLEHIHI:
			_tmp.prefix = PF_IMM|IMM_BOOL;
			_tmp.du.zzk__BOOL = _lpTtu->Status & TTUBIT_ENABLEHIHI;
			break;

		case TTU_ENABLEHI:
			_tmp.prefix = PF_IMM|IMM_BOOL;
			_tmp.du.zzk__BOOL = _lpTtu->Status & TTUBIT_ENABLEHI;
			break;

		case TTU_ENABLELO:
			_tmp.prefix = PF_IMM|IMM_BOOL;
			_tmp.du.zzk__BOOL = _lpTtu->Status & TTUBIT_ENABLELO;
			break;

		case TTU_ENABLELOLO:
			_tmp.prefix = PF_IMM|IMM_BOOL;
			_tmp.du.zzk__BOOL = _lpTtu->Status & TTUBIT_ENABLELOLO;
			break;

		case TTU_TRACETIME:
			_tmp.prefix = PF_IMM|IMM_DWORD;
			_tmp.du.zzk__DWORD = _lpTtu->TraceTime;
			break;

		case TTU_TRACEDATA:
			_tmp.prefix = PF_IMM|IMM_FPTR;
			_tmp.du.zzk__FPTR = _lpTtu->lpTraceData;
			break;

		case TTU_VALID:
			_tmp.prefix = PF_IMM|IMM_BOOL;
			_tmp.du.zzk__BOOL = _lpTtu->Status & TTUBIT_VALID;
			break;

		case TTU_LOGHIS:
			_tmp.prefix = PF_IMM|IMM_BOOL;
			_tmp.du.zzk__BOOL = _lpTtu->Status & TTUBIT_LOGHIS;
			break;

		case TTU_TRACE:
			_tmp.prefix = PF_IMM|IMM_BOOL;
			_tmp.du.zzk__BOOL = _lpTtu->Status & TTUBIT_TRACE;
			break;

		default:
			CORE_ASSERT(0);
			return FALSE;
	}

	return PUCast(to_, &_tmp);
}

//...................................................................
//...................................................................
#define _TTU_SET_DIRTY( _ELE_ )							\
		{_lpTtu->Dirty |= TTUBIT_DIRTY_##_ELE_;}

_DLLEXP BOOL _DLLFUNC PUWriteTintTable(LPPU to_, SimplePU FAR* from_)
{
	SimplePU 	_tmp;
	LPTTU		_lpTtu;
	TINT		_inner;	//temp value

	_lpTtu = (gTTInfo.lpBase) + to_->du.zzk__WRD;
	if( !(_lpTtu->Status & TTUBIT_VALID) )
	{
		LOGERR1( "Write to invalid TTU: %d", to_->du.zzk__WRD );
		return FALSE;
	}

	switch(to_->prefix & 0x00FF)
	{
		case TTU_VALUE:
			_tmp.prefix = PF_IMM|IMM_TINT;
			PUCast(&_tmp, from_);
			if( _lpTtu->Value == _tmp.du.zzk__TINT )
				return TRUE;	//same value
			_lpTtu->Value = _tmp.du.zzk__TINT;
			TintAlarm( to_->du.zzk__WRD );

			_TTU_SET_DIRTY( VALUE )
			break;

		case TTU_MIN:
			_tmp.prefix = PF_IMM|IMM_TINT;
			PUCast(&_tmp, from_);
			if( _lpTtu->Min == _tmp.du.zzk__TINT )
				return TRUE;	//same value
			if( _lpTtu->Status & TTUBIT_DISABLE_CHECK )
			{
				_lpTtu->Min = _tmp.du.zzk__TINT;
			}
			else
			{
				if( _tmp.du.zzk__TINT < _lpTtu->Max )
					_lpTtu->Min = _tmp.du.zzk__TINT;
				else
					return FALSE;	//invalid min value
			}

			_TTU_SET_DIRTY( MIN )
			break;

		case TTU_MAX:
			_tmp.prefix = PF_IMM|IMM_TINT;
			PUCast(&_tmp, from_);
			if( _lpTtu->Max == _tmp.du.zzk__TINT )
				return TRUE;	//same value
			if( _lpTtu->Status & TTUBIT_DISABLE_CHECK )
			{
				_lpTtu->Max = _tmp.du.zzk__TINT;
			}
			else
			{
				if( _tmp.du.zzk__TINT > _lpTtu->Min )
					_lpTtu->Max = _tmp.du.zzk__TINT;
				else
					return FALSE;	//invalid min value
			}

			_TTU_SET_DIRTY( MAX )
			break;

		case TTU_TAG:
			/* Note:
			There, from->du.zzk__LPCOBJ must be a Cobject directly,
			and a reference to another string cobject is elligal,
			notice that the following code can't find this case:
				_tmp.prefix = PF_IMM|IMM_TINT;
				PUCast(&_tmp, from_);
			here, TRUE can be returned when from_ is a reference to a cobj.
			*/
			if( IS_STRING_COBJ( from_->du.zzk__LPCOBJ ) )
			{
				//need not check here, TintSetTag will do everything 
				if( TintSetTag(to_->du.zzk__WRD, PU_GET_COBJ_STRING(from_) ) )
				{
					//if success, delete the imported cobject
					CobjFree( from_->du.zzk__LPCOBJ );
					from_->du.zzk__LPCOBJ = NULL;

					//dirty has been set in TintSetTag
					//	_TTU_SET_DIRTY( TAG )
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

		case TTU_UNIT:
			if( IS_STRING_COBJ( from_->du.zzk__LPCOBJ ) )
			{
				if( !CobjCompareString( _lpTtu->lpUnit, from_->du.zzk__LPCOBJ ) )
					return TRUE;

				CobjFree( _lpTtu->lpUnit );
				_lpTtu->lpUnit = from_->du.zzk__LPCOBJ;
				from_->du.zzk__LPCOBJ = 0;

				_TTU_SET_DIRTY( UNIT )
				return TRUE;
			}
			else
			{
				LOGERR1("PU data is not a String:PU=%lp", from_);
				return FALSE;
			}
			break;

		case TTU_COMMENT:
			if( IS_STRING_COBJ( from_->du.zzk__LPCOBJ ) )
			{
				if( !CobjCompareString( _lpTtu->lpComment, from_->du.zzk__LPCOBJ ) )
					return TRUE;

				CobjFree ( _lpTtu->lpComment );
				_lpTtu->lpComment = from_->du.zzk__LPCOBJ;
				from_->du.zzk__LPCOBJ = 0;

				_TTU_SET_DIRTY( COMMENT )
				return TRUE;
			}
			else
			{
				LOGERR1("PU data is not a String:PU=%lp", from_);
				return FALSE;
			}

		case TTU_HIHI:
			_tmp.prefix = PF_IMM|IMM_TINT;
			PUCast(&_tmp, from_);
			
			if( _lpTtu->HiHi == _tmp.du.zzk__TINT )
				return TRUE;

			if( _lpTtu->Status & TTUBIT_DISABLE_CHECK )
			{
				_lpTtu->HiHi = _tmp.du.zzk__TINT;
				_TTU_SET_DIRTY( HIHI )
				return TRUE;
			}

			_inner = _lpTtu->HiHi;
			_lpTtu->HiHi = _tmp.du.zzk__TINT;
			if( !TintCheckAlarmThreshold( to_->du.zzk__WRD ) )
			{
				_lpTtu->HiHi = _inner;
				return FALSE;
			}
			_TTU_SET_DIRTY( HIHI )
			TintAlarm( to_->du.zzk__WRD );
			break;

		case TTU_HI:
			_tmp.prefix = PF_IMM|IMM_TINT;
			PUCast(&_tmp, from_);
			
			if( _lpTtu->Hi == _tmp.du.zzk__TINT )
				return TRUE;

			if( _lpTtu->Status & TTUBIT_DISABLE_CHECK )
			{
				_lpTtu->Hi = _tmp.du.zzk__TINT;
				_TTU_SET_DIRTY( HI )
				return TRUE;
			}

			_inner = _lpTtu->Hi;
			_lpTtu->Hi = _tmp.du.zzk__TINT;
			if( !TintCheckAlarmThreshold( to_->du.zzk__WRD ) )
			{
				_lpTtu->Hi = _inner;
				return FALSE;
			}
			_TTU_SET_DIRTY( HI )
			TintAlarm( to_->du.zzk__WRD );
			break;

		case TTU_LO:
			_tmp.prefix = PF_IMM|IMM_TINT;
			PUCast(&_tmp, from_);

			if( _lpTtu->Lo == _tmp.du.zzk__TINT )
				return TRUE;
			
			if( _lpTtu->Status & TTUBIT_DISABLE_CHECK )
			{
				_lpTtu->Lo = _tmp.du.zzk__TINT;
				_TTU_SET_DIRTY( LO )
				return TRUE;
			}

			_inner = _lpTtu->Lo;
			_lpTtu->Lo = _tmp.du.zzk__TINT;
			if( !TintCheckAlarmThreshold( to_->du.zzk__WRD ) )
			{
				_lpTtu->Lo = _inner;
				return FALSE;
			}
			_TTU_SET_DIRTY( LO )
			TintAlarm( to_->du.zzk__WRD );
			break;

		case TTU_LOLO:
			_tmp.prefix = PF_IMM|IMM_TINT;
			PUCast(&_tmp, from_);

			if( _lpTtu->LoLo == _tmp.du.zzk__TINT )
				return TRUE;
			
			if( _lpTtu->Status & TTUBIT_DISABLE_CHECK )
			{
				_lpTtu->LoLo = _tmp.du.zzk__TINT;
				_TTU_SET_DIRTY( LOLO )
				return TRUE;
			}

			_inner = _lpTtu->LoLo;
			_lpTtu->LoLo = _tmp.du.zzk__TINT;
			if( !TintCheckAlarmThreshold( to_->du.zzk__WRD ) )
			{
				_lpTtu->LoLo = _inner;
				return FALSE;
			}
			_TTU_SET_DIRTY( LOLO )
			TintAlarm( to_->du.zzk__WRD );
			break;

		case TTU_LOGBAND:
			_tmp.prefix = PF_IMM|IMM_TINT;
			PUCast(&_tmp, from_);
			if( _lpTtu->LogBand == _tmp.du.zzk__TINT )
				return TRUE;	//same value
			_lpTtu->LogBand = _tmp.du.zzk__TINT;
			
			_TTU_SET_DIRTY( LOGBAND )
			break;

		case TTU_ALARMBAND:
			_tmp.prefix = PF_IMM|IMM_TINT;
			PUCast(&_tmp, from_);
			if( _lpTtu->AlarmBand == _tmp.du.zzk__TINT )
				return TRUE;	//same value
			_lpTtu->AlarmBand = _tmp.du.zzk__TINT;
			
			_TTU_SET_DIRTY( ALARMBAND )
			break;

		case TTU_USERDATA:
			_tmp.prefix = PF_IMM|IMM_DWORD;
			PUCast(&_tmp, from_);
			if( _lpTtu->UserData == _tmp.du.zzk__DWORD )
				return TRUE;	//same value
			_lpTtu->UserData = _tmp.du.zzk__DWORD;
			
			_TTU_SET_DIRTY( USERDATA )
			break;

		case TTU_TRACETIME:
		{
			_tmp.prefix = PF_IMM|IMM_DWORD;
			PUCast(&_tmp, from_);
			if( _lpTtu->TraceTime == _tmp.du.zzk__DWORD )
				return TRUE;	//same value
			_lpTtu->TraceTime = _tmp.du.zzk__DWORD;
			
			_TTU_SET_DIRTY( TRACETIME )

			if( IS_RUN_MODE() )
				return TintInitializeTrace( to_->du.zzk__WRD );
			
			break;
		}

		case TTU_LOGHIS:
			_tmp.prefix = PF_IMM|IMM_BOOL;
			PUCast(&_tmp, from_);
			if(    ( (_lpTtu->Status & TTUBIT_LOGHIS)? TRUE: FALSE )
				== ( _tmp.du.zzk__BOOL?TRUE:FALSE ) )
				return TRUE;	//same value

			_lpTtu->Status ^= (TTUBIT_LOGHIS);		//tag this bit
			_TTU_SET_DIRTY( STATUS )
			break;

		case TTU_TRACE:
		{
			_tmp.prefix = PF_IMM|IMM_BOOL;
			PUCast(&_tmp, from_);
			if(    ( (_lpTtu->Status & TTUBIT_TRACE)? TRUE: FALSE )
				== ( _tmp.du.zzk__BOOL?TRUE:FALSE ) )
				return TRUE;	//same value

			_lpTtu->Status ^= (TTUBIT_TRACE);		//tag this bit
			_TTU_SET_DIRTY( STATUS )
			
			if( IS_RUN_MODE() )
				return TintInitializeTrace( to_->du.zzk__WRD );
			
			break;
		}

		case TTU_ENABLEHIHI:
			_tmp.prefix = PF_IMM|IMM_BOOL;
			PUCast(&_tmp, from_);
			if(    ( (_lpTtu->Status & TTUBIT_ENABLEHIHI)? TRUE: FALSE )
				== ( _tmp.du.zzk__BOOL?TRUE:FALSE ) )
				return TRUE;	//same value

			_lpTtu->Alarm ^= (TTUBIT_ENABLEHIHI);		//tag this bit
			_TTU_SET_DIRTY( STATUS )
			
			if( !TintCheckAlarmThreshold(to_->du.zzk__WRD) )
				LOGERR1( "Alarm threshold error, TTU:0x%X", to_->du.zzk__WRD );	
			TintAlarm( to_->du.zzk__WRD );
			break;

		case TTU_ENABLEHI:
			_tmp.prefix = PF_IMM|IMM_BOOL;
			PUCast(&_tmp, from_);
			if(    ( (_lpTtu->Status & TTUBIT_ENABLEHI)? TRUE: FALSE )
				== ( _tmp.du.zzk__BOOL?TRUE:FALSE ) )
				return TRUE;	//same value

			_lpTtu->Alarm ^= (TTUBIT_ENABLEHI);		//tag this bit
			_TTU_SET_DIRTY( STATUS )
			
			if( !TintCheckAlarmThreshold(to_->du.zzk__WRD) )
				LOGERR1( "Alarm threshold error, TTU:0x%X", to_->du.zzk__WRD );	
			TintAlarm( to_->du.zzk__WRD );
			break;

		case TTU_ENABLELO:
			_tmp.prefix = PF_IMM|IMM_BOOL;
			PUCast(&_tmp, from_);
			if(    ( (_lpTtu->Status & TTUBIT_ENABLELO)? TRUE: FALSE )
				== ( _tmp.du.zzk__BOOL?TRUE:FALSE ) )
				return TRUE;	//same value

			_lpTtu->Alarm ^= (TTUBIT_ENABLELO);		//tag this bit
			_TTU_SET_DIRTY( STATUS )
			
			if( !TintCheckAlarmThreshold(to_->du.zzk__WRD) )
				LOGERR1( "Alarm threshold error, TTU:0x%X", to_->du.zzk__WRD );	
			TintAlarm( to_->du.zzk__WRD );
			break;

		case TTU_ENABLELOLO:
			_tmp.prefix = PF_IMM|IMM_BOOL;
			PUCast(&_tmp, from_);
			if(    ( (_lpTtu->Status & TTUBIT_ENABLELOLO)? TRUE: FALSE )
				== ( _tmp.du.zzk__BOOL?TRUE:FALSE ) )
				return TRUE;	//same value

			_lpTtu->Alarm ^= (TTUBIT_ENABLELOLO);		//tag this bit
			_TTU_SET_DIRTY( STATUS )
			
			if( !TintCheckAlarmThreshold(to_->du.zzk__WRD) )
				LOGERR1( "Alarm threshold error, TTU:0x%X", to_->du.zzk__WRD );	
			TintAlarm( to_->du.zzk__WRD );
			break;

		case TTU_ACK:
		{
			//this function will do everthing
			return TintAckAlarm( to_->du.zzk__WRD );
		}

		default:
			//read-only elements
			return FALSE;
	}

	return TRUE;
}

//...................................................................
//...................................................................
_DLLEXP BOOL _DLLFUNC PUTintDirty( LPPU lppu_ )
{
	int		_i;
	WORD	_ele;

	CORE_ASSERT( ((lppu_->prefix) & TBX_MASK) == TBX_TT );
	if( lppu_->du.zzk__WRD  >= gTTInfo.wAdding )
	{
		CORE_ASSERT( FALSE );
		return FALSE;
	}

	_ele = lppu_->prefix & XT_MASK;

	for(_i=0; _i<_NUM_NAMESET; _i++)
		if( _ele == ssNameSet[_i].element )
		{
			return (gTTInfo.lpBase + lppu_->du.zzk__WRD)->Dirty & ssNameSet[_i].dirty;
		}

	CORE_ASSERT( FALSE );	//should NOT go here
	return FALSE;	//no found
}
