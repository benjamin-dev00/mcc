//===================================================================
//
//Zheng zhikai	95-10-4
//===================================================================

#include "prehead.h"

#include "core.h"
#include "cpu.h"
#include "cobj.h"
#include "csysobj.h"
#include "creal.h"
#include "cerror.h"
#include "corerw.h"
#include "calarm.h"

static void RealReleaseRoom(WRD _wrd);
static WRD RealGetFreeRoom(void);
static BOOL RealInitializeTrace( WRD wrd_ );

extern RTInfo		gRTInfo;

extern DWORD		gdwLastError;
extern CoreInfo 	gCoreInfo;

struct {
	WORD	element;
	char	szName[16];
	VSOF	vsof;
	DIRTY	dirty;	//dirty mask
	WORD	type;	//access type
} static ssNameSet[] =
{
	{RTU_VALUE, 		"Value",		VSOF_REAL,			RTUBIT_DIRTY_VALUE, 		TET_PRIMARY },
	{RTU_MIN,			"Min",			VSOF_REAL,			RTUBIT_DIRTY_MIN,			TET_PRIMARY },
	{RTU_MAX,			"Max",			VSOF_REAL,			RTUBIT_DIRTY_MAX,			TET_PRIMARY },
	{RTU_HIHI,			"HiHi", 		VSOF_REAL,			RTUBIT_DIRTY_HIHI,			TET_PRIMARY },
	{RTU_HI,			"Hi",			VSOF_REAL,			RTUBIT_DIRTY_HI,			TET_PRIMARY },
	{RTU_LO,			"Lo",			VSOF_REAL,			RTUBIT_DIRTY_LO,			TET_PRIMARY },
	{RTU_LOLO,			"LoLo", 		VSOF_REAL,			RTUBIT_DIRTY_LOLO,			TET_PRIMARY },
	{RTU_LOGBAND,		"LogBand",		VSOF_REAL,			RTUBIT_DIRTY_LOGBAND,		TET_PRIMARY },
	{RTU_ALARMBAND, 	"AlmBand",		VSOF_REAL,			RTUBIT_DIRTY_ALARMBAND, 	TET_PRIMARY },
	{RTU_TAG,			"Tag",			VSOF_LPCOBJ_STR,		RTUBIT_DIRTY_TAG,		TET_PRIMARY },
	{RTU_UNIT,			"Unit", 		VSOF_LPCOBJ_STR,		RTUBIT_DIRTY_UNIT,		TET_PRIMARY },
	{RTU_COMMENT,		"Comment",		VSOF_LPCOBJ_STR,		RTUBIT_DIRTY_COMMENT,	TET_PRIMARY },
	{RTU_TRACETIME, 	"TraceTime",	VSOF_DWORD_MS,		RTUBIT_DIRTY_TRACETIME, 	TET_PRIMARY },
	{RTU_USERDATA,		"UserData", 	VSOF_DWORD_HEX, 	RTUBIT_DIRTY_USERDATA,		TET_PRIMARY },
	//private primary elements
	{RTU_STATUS,		"Status",		VSOF_STATUS_RTU,	RTUBIT_DIRTY_STATUS,		TET_PRIMARY|TET_READONLY|TET_PRIVATE },
	{RTU_ALARM, 		"_alarm",		VSOF_ALARM_RTU, 	RTUBIT_DIRTY_ALARM, 		TET_PRIMARY|TET_READONLY|TET_PRIVATE },
	{RTU_DIRTY, 		"Dirty",		VSOF_DIRTY_RTU, 	RTUBIT_DIRTY_DIRTY, 		TET_PRIMARY|TET_READONLY|TET_PRIVATE },
	{RTU_LASTLOG,		"LastLog",		VSOF_REAL,			RTUBIT_DIRTY_LASTLOG,		TET_PRIMARY|TET_READONLY|TET_PRIVATE },
	{RTU_TRACEDATA, 	"TraceData",	VSOF_FPTR,			RTUBIT_DIRTY_TRACEDATA, 	TET_PRIMARY|TET_READONLY|TET_PRIVATE },
	//here, primary data end

	{RTU_VALUE_PERCENT, "Percent",		VSOF_REAL,			RTUBIT_DIRTY_VALUE, 		TET_READWRITE },

	{RTU_SPAN,			"Span", 		VSOF_REAL,			RTUBIT_DIRTY_VALUE|RTUBIT_DIRTY_MAX|RTUBIT_DIRTY_MIN,		TET_READONLY },

	{RTU_ENABLELOLO,	"EnableLoLo",	VSOF_BOOL,			RTUBIT_DIRTY_STATUS,		TET_READWRITE },
	{RTU_ENABLELO,		"EnableLo", 	VSOF_BOOL,			RTUBIT_DIRTY_STATUS,		TET_READWRITE },
	{RTU_ENABLEHI,		"EnableHi", 	VSOF_BOOL,			RTUBIT_DIRTY_STATUS,		TET_READWRITE },
	{RTU_ENABLEHIHI,	"EnableHiHi",	VSOF_BOOL,			RTUBIT_DIRTY_STATUS,		TET_READWRITE },
	{RTU_TRACE, 		"Trace",		VSOF_BOOL,			RTUBIT_DIRTY_STATUS,		TET_READWRITE },
	{RTU_LOGHIS,		"LogHis",		VSOF_BOOL,			RTUBIT_DIRTY_STATUS,		TET_READWRITE },
	{RTU_VALID, 		"Valid",		VSOF_BOOL,			RTUBIT_DIRTY_STATUS,		TET_READONLY|TET_PRIVATE },

	//Alarm on: ALM_xxx; data type: BOOL
	{RTU_ALM,			"Alm",			VSOF_BOOL,			RTUBIT_DIRTY_ALARM, 		TET_READONLY },
	{RTU_ALMHIHI,		"AlmHiHi",		VSOF_BOOL,			RTUBIT_DIRTY_ALARM, 		TET_READONLY },
	{RTU_ALMHI, 		"AlmHi",		VSOF_BOOL,			RTUBIT_DIRTY_ALARM, 		TET_READONLY },
	{RTU_ALMLO, 		"AlmLo",		VSOF_BOOL,			RTUBIT_DIRTY_ALARM, 		TET_READONLY },
	{RTU_ALMLOLO,		"AlmLoLo",		VSOF_BOOL,			RTUBIT_DIRTY_ALARM, 		TET_READONLY },

	//data accessing defines: Alarm without(No) Ack: ANA_xxx
	//data type: BOOL
	{RTU_ANA,			"Ana",			VSOF_BOOL,			RTUBIT_DIRTY_ALARM, 	TET_READONLY },
	{RTU_ANAHIHI,		"AnaHiHi",		VSOF_BOOL,			RTUBIT_DIRTY_ALARM, 	TET_READONLY },
	{RTU_ANAHI, 		"AnaHi",		VSOF_BOOL,			RTUBIT_DIRTY_ALARM, 	TET_READONLY },
	{RTU_ANALO, 		"AnaLo",		VSOF_BOOL,			RTUBIT_DIRTY_ALARM, 	TET_READONLY },
	{RTU_ANALOLO,		"AnaLoLo",		VSOF_BOOL,			RTUBIT_DIRTY_ALARM, 	TET_READONLY },

	//data accessing defines: ack
	//data type: BOOL
	{RTU_ACK,			"Ack",			VSOF_BOOL,			RTUBIT_DIRTY_ALARM, 	TET_READWRITE },
	{RTU_ACKHIHI,		"AckHiHi",		VSOF_BOOL,			RTUBIT_DIRTY_ALARM, 	TET_READONLY },
	{RTU_ACKHI, 		"AckHi",		VSOF_BOOL,			RTUBIT_DIRTY_ALARM, 	TET_READONLY },
	{RTU_ACKLO, 		"AckLo",		VSOF_BOOL,			RTUBIT_DIRTY_ALARM, 	TET_READONLY },
	{RTU_ACKLOLO,		"AckLoLo",		VSOF_BOOL,			RTUBIT_DIRTY_ALARM, 	TET_READONLY },

	//a STATUS type data to identify which kind of alarm is enabled. 
	//and if any alarm enabled, this value is non-zero
	{RTU_ENABLEALM, 	"EnableAlm",	VSOF_WORD_HEX,		RTUBIT_DIRTY_STATUS,		TET_READONLY },
};

#define _NUM_NAMESET	( sizeof(ssNameSet)/sizeof(ssNameSet[0]) )
//...................................................................
//Description:
//	Creates a Real-unit in the Real Table;
//Return:
//		the subscription of real-unit if the function succeeds;
//		Otherwise, it is NULL;
//...................................................................
_DLLEXP WRD _DLLFUNC RealCreate(LPCSTR	lpszTag_,
						REAL	rValue_,
						REAL	rMin_,
						REAL	rMax_)
{
	WRD 		_wrd;
	LPCOBJ		_lpCobj = NULL;
	BOOL		_bOk;

	CLEAR_LAST_ERROR;

	_wrd = RealGetFreeRoom();
	if( !_wrd )
	{
		//if no free room, function RealGetFreeRoom(...) set gdwLastError
		return 0;
	}

	memset( gRTInfo.lpBase + _wrd, 0, sizeof(RTU) );

	//allocate the memory for the tag
	_lpCobj = AllocTag( lpszTag_, &_bOk );
	if( !_bOk ) 	//allocate the tag failed
		return FALSE;

	(gRTInfo.lpBase + _wrd)->lpTag = _lpCobj;
	(gRTInfo.lpBase + _wrd)->Value = rValue_;
	(gRTInfo.lpBase + _wrd)->Min = rMin_;
	(gRTInfo.lpBase + _wrd)->Max = rMax_;

	if( !RealCheckLegality(_wrd) )
	{
		if( (gRTInfo.lpBase + _wrd)->lpTag )
			CobjFree( (gRTInfo.lpBase + _wrd)->lpTag );

		//this function does not change gdwLastError
		RealReleaseRoom(_wrd);
		return 0;
	}

	//Note: here using "=" , not "|="
	(gRTInfo.lpBase + _wrd)->Status = RTUBIT_VALID;
	(gRTInfo.lpBase + _wrd)->Dirty = RTUBIT_DIRTY_ALL;

	DispatchCoreMessage( TBX_RT );
	return _wrd;
}

//...................................................................
//Description:
//	Deletes a RTU;
//Entry:
//	WRD wrd_
//		the subscription of the RTU being freed;
//...................................................................
_DLLEXP BOOL _DLLFUNC RealDelete(WRD wrd_)
{
	LPRTU	_lpRtu;

	CLEAR_LAST_ERROR;
	_lpRtu = gRTInfo.lpBase + wrd_;

	if( wrd_ >= gRTInfo.wCount )
	{
		CORE_ASSERT(FALSE);
		SET_LAST_ERROR( ERR_Invalid_Entries );
		return NULL;
	}

	if( !(_lpRtu->Status & RTUBIT_VALID) )
	{
		CORE_ASSERT( FALSE );
		SET_LAST_ERROR( ERR_Invalid_Entries );
		return FALSE;
	}

	if( GetReference(TB_RT, wrd_, TRUE, NULL) )
	{
		SET_LAST_ERROR( ERR_Referenced_RTU );
		return FALSE;	//the RTU is referenced by other xTUs
	}

	_lpRtu->Status &= (~RTUBIT_VALID);	//clear VALID bit
	_lpRtu->Dirty |= RTUBIT_DIRTY_ALL;

	//free memory occupied by this unit
	FragFree( _lpRtu->lpTraceData );

	CobjFree( _lpRtu->lpTag );
	CobjFree( _lpRtu->lpUnit );
	CobjFree( _lpRtu->lpComment );
	_lpRtu->lpTag = _lpRtu->lpUnit =_lpRtu->lpComment = NULL;
	_lpRtu->lpTraceData = NULL;

	DispatchCoreMessage( TBX_RT );
	return TRUE;
}

//...................................................................
//Description:
//...................................................................
static WRD RealGetFreeRoom()
{
	WRD 	_i;

	CLEAR_LAST_ERROR;

	for(_i=1; _i<gRTInfo.wAdding; _i++)
	{
		//Note: the following code is WRONG
		//	if( ((gRTInfo.lpBase + _i)->Status & RTUBIT_VALID) == 0 )
		//
		if(  ((gRTInfo.lpBase + _i)->Status == 0 )
		   &&((gRTInfo.lpBase + _i)->Dirty == 0 ) )
			break;
	}

	if( _i < gRTInfo.wAdding )
		return _i;
	else	// _i == gRTInfo.wAdding
	{
		if( gRTInfo.wAdding < gRTInfo.wCount )
		{
			gRTInfo.wAdding ++;
			return _i;
		}
		else	//no free room
		{
			//if no free room, set last error
			SET_LAST_ERROR( ERR_RTU_No_Free_Room );
			return 0;
		}
	}
}

//...................................................................
//Description:
//	This function frees the room occuped by RTU wrd_;
//Note:
//	The function does NOT change the last error;
//...................................................................
static void RealReleaseRoom(WRD wrd_)
{
	if( wrd_ >= gRTInfo.wAdding )
	{
		CORE_ASSERT( FALSE );
		return ;
	}

	memset( gRTInfo.lpBase + wrd_, 0, sizeof(RTU) );

	//if the RTU[adding-1] is free, decrease Adding
	while(	((gRTInfo.lpBase + gRTInfo.wAdding - 1)->Status == 0 )
		  &&((gRTInfo.lpBase + gRTInfo.wAdding - 1)->Dirty == 0 )
		  &&( gRTInfo.wAdding > 1 ) )
		gRTInfo.wAdding -- ;

	return ;
}

//...................................................................
//Description:
//	Create a temp. RTU by copying from wrd_
//...................................................................
_DLLEXP WRD _DLLFUNC RealCopyTemp( WRD wrd_ )
{
	LPRTU		_lpRtuTmp, _lpRtuSrc;
	WRD 		_wMe;
	LPCOBJ		_lpCobjTmpTag = NULL;
	LPCOBJ		_lpCobjTmpUnit = NULL;
	LPCOBJ		_lpCobjTmpComment = NULL;
	BOOL		_bOk;

	CLEAR_LAST_ERROR;

	_wMe = RealGetFreeRoom();
	if( !_wMe ) return 0; //if no free room

	_lpRtuSrc = gRTInfo.lpBase + wrd_;

	//duplicate the tag, unit and comment
	_lpCobjTmpTag = CobjCopy( _lpRtuSrc->lpTag, &_bOk );
	if( !_bOk ) goto _BAD;
	_lpCobjTmpUnit = CobjCopy( _lpRtuSrc->lpUnit, &_bOk );
	if( !_bOk ) goto _BAD;
	_lpCobjTmpComment = CobjCopy( _lpRtuSrc->lpComment, &_bOk );
	if( !_bOk ) goto _BAD;

	//do not check for legality of the tag

	_lpRtuTmp = (gRTInfo.lpBase) + _wMe;

	//copy old data
	memcpy( _lpRtuTmp, _lpRtuSrc, sizeof(RTU) );	

	//update new data
	_lpRtuTmp->lpTag = _lpCobjTmpTag;
	_lpRtuTmp->lpUnit = _lpCobjTmpUnit;
	_lpRtuTmp->lpComment = _lpCobjTmpComment;
	_lpRtuTmp->Status |= RTUBIT_VALID|RTUBIT_TEMP;

	return _wMe;

_BAD:
	if( _lpCobjTmpTag ) CobjFree( _lpCobjTmpTag );
	if( _lpCobjTmpUnit ) CobjFree( _lpCobjTmpUnit );
	if( _lpCobjTmpComment ) CobjFree( _lpCobjTmpComment );

	RealReleaseRoom( _wMe );
	return FALSE;
}

//...................................................................
//Description:
//...................................................................
_DLLEXP BOOL _DLLFUNC RealDeleteTemp(WRD wrd_)
{
	LPRTU	_lpRtu = gRTInfo.lpBase + wrd_;

	CLEAR_LAST_ERROR;

	if(   (_lpRtu->Status & RTUBIT_VALID)
	   && (_lpRtu->Status & RTUBIT_TEMP) )
	{
		CobjFree( _lpRtu->lpTag );
		CobjFree( _lpRtu->lpUnit );
		CobjFree( _lpRtu->lpComment );
		_lpRtu->lpTag = _lpRtu->lpUnit =_lpRtu->lpComment = NULL;

		RealReleaseRoom(wrd_);

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
//	Undo a RTU specified by wReal_ using its undo buffer: wBuffer_
//and then Delete the undo buffer RTU.
//...................................................................
_DLLEXP BOOL _DLLFUNC RealUndo( WRD wReal_, WRD wBuffer_ )
{
	LPRTU	_lpRtu, _lpRtuBuffer;

	if( !wReal_ || !wBuffer_ )
	{
		SET_LAST_ERROR( ERR_Invalid_Entries );
		return FALSE;
	}
	_lpRtu = gRTInfo.lpBase + wReal_;
	_lpRtuBuffer = gRTInfo.lpBase + wBuffer_;

	//free the dirty tag, unit and comment
	CobjFree( _lpRtu->lpTag );	
	CobjFree( _lpRtu->lpUnit ); 
	CobjFree( _lpRtu->lpComment );

	//undo the parameters of the RTU and other elements
	//NOTE: the tag of the undo buffer RTU is sent to the wReal_
	memcpy( _lpRtu, _lpRtuBuffer, sizeof( RTU) );
	_lpRtu->Status &= (~RTUBIT_TEMP);	 //clear TEMP bit

	//delete the undo buffer RTU
	memset( _lpRtuBuffer, 0, sizeof(RTU) );
	RealReleaseRoom( wBuffer_ );

	return TRUE;
}

//...................................................................
//Description:
//	This function only checks the RTUBIT_VALID bit;
//...................................................................
_DLLEXP BOOL _DLLFUNC RealIsValid( WRD wrd_ )
{
	if(  ( wrd_ < gRTInfo.wCount )
	   &&( (gRTInfo.lpBase + wrd_)->Status & RTUBIT_VALID ) )
		return TRUE;
	else return FALSE;
}

//...................................................................
//Description:
//	Checks the legality of a RTU even the RTU is invald, including:
//		. legality of tag(ERROR CODE: ERR_Illegal_Tag)
//		. an unique of tag in the table(ERROR CODE: ERR_Not_Unique_Tag)
//Entry:
//	
//		
//Return:
//	TRUE/FALSE
//Note:
//	The old version checks the following rule
//		. min <= value <= max (ERROR CODE: ERR_Invalid_Min_Value_Max)
//	Now the check is deleted;
//...................................................................
_DLLEXP BOOL _DLLFUNC RealCheckLegality(WRD wrd_)
{
	CLEAR_LAST_ERROR;
	if( wrd_ >= gRTInfo.wCount )
	{
		CORE_ASSERT(FALSE);
		SET_LAST_ERROR( ERR_Invalid_Entries );
		return NULL;
	}

	if( !RealCheckTag( wrd_ ) )
		return FALSE;
	if( !RealCheckMinMax( wrd_ ) )
		return FALSE;
	if( !RealCheckAlarmThreshold( wrd_ ) )
		return FALSE;

	return TRUE;
}

//...................................................................
//Description:
//Entry:
//Return:
//...................................................................
_DLLEXP BOOL _DLLFUNC RealCheckTag(WRD wrd_)
{
	WRD 	_i;
	LPRTU	_lp = gRTInfo.lpBase + wrd_;

	CLEAR_LAST_ERROR;
	if( wrd_ >= gRTInfo.wCount )
	{
		CORE_ASSERT(FALSE);
		SET_LAST_ERROR( ERR_Invalid_Entries );
		return NULL;
	}

	if( _lp->lpTag == NULL )
		return TRUE;

	if( !IsTagLegal( pREAL_GET_TAG(_lp) ) )
	{
		return FALSE;
	}

	//check on whether it's an unique tag in the real-table
	for(_i=1; _i<gRTInfo.wAdding; _i++)
	{
		if( (gRTInfo.lpBase[_i]).Status & RTUBIT_TEMP )
			continue;	//don't check temp RTU
		if(  ((gRTInfo.lpBase[_i]).Status & RTUBIT_VALID )
		   &&((gRTInfo.lpBase[_i]).lpTag) )
		{
			if( CmpTag( REAL_GET_TAG(_i),
						pREAL_GET_TAG(_lp),
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
_DLLEXP BOOL _DLLFUNC RealCheckMinMax(WRD wrd_)
{
	LPRTU	_lpRtu = gRTInfo.lpBase + wrd_;

	CLEAR_LAST_ERROR;
	if( wrd_ >= gRTInfo.wCount )
	{
		CORE_ASSERT(FALSE);
		SET_LAST_ERROR( ERR_Invalid_Entries );
		return FALSE;
	}

	if( !( _lpRtu->Min < _lpRtu->Max ) )
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
_DLLEXP BOOL _DLLFUNC RealCheckAlarmThreshold(WRD wrd_)
{
	LPRTU	_lpRtu = gRTInfo.lpBase + wrd_;

	CLEAR_LAST_ERROR;
	if( wrd_ >= gRTInfo.wCount )
	{
		CORE_ASSERT(FALSE);
		SET_LAST_ERROR( ERR_Invalid_Entries );
		return NULL;
	}

	//check: HiHi > (Hi, Lo, LoLo)
	if( _lpRtu->Status & RTUBIT_ENABLEHIHI )
		if( _lpRtu->Status & RTUBIT_ENABLEHI )
			if( !( _lpRtu->HiHi > _lpRtu->Hi ) )
			{
				SET_LAST_ERROR( ERR_HIHI_and_HI );
				return FALSE;
			}
		else
			if( _lpRtu->Status & RTUBIT_ENABLELO )
				if( !( _lpRtu->HiHi > _lpRtu->Lo ) )
				{
					SET_LAST_ERROR( ERR_HIHI_and_LO );
					return FALSE;
				}
			else
				if( _lpRtu->Status & RTUBIT_ENABLELOLO )
					if( !( _lpRtu->HiHi > _lpRtu->LoLo ) )
					{
						SET_LAST_ERROR( ERR_HIHI_and_LOLO );
						return FALSE;
					}

	//check: Hi > (Lo, LoLo)
	if( _lpRtu->Status & RTUBIT_ENABLEHI )
		if( _lpRtu->Status & RTUBIT_ENABLELO )
			if( !( _lpRtu->Hi > _lpRtu->Lo ) )
			{
				SET_LAST_ERROR( ERR_HI_and_LO );
				return FALSE;
			}
		else
			if( _lpRtu->Status & RTUBIT_ENABLELOLO )
				if( !( _lpRtu->Hi > _lpRtu->LoLo ) )
				{
					SET_LAST_ERROR( ERR_HI_and_LOLO );
					return FALSE;
				}

	//check: Lo > (LoLo)
	if( _lpRtu->Status & RTUBIT_ENABLELO )
		if( _lpRtu->Status & RTUBIT_ENABLELOLO )
			if( !( _lpRtu->Lo > _lpRtu->LoLo ) )
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
//	if the RTU that matchs the tag szTag_ is deleted, returns 0;
//...................................................................
_DLLEXP WRD _DLLFUNC RealGetWRD(LPCSTR szTag_)
{
	WRD _i;

	if(*szTag_ == '#')
	{
		WRD _wrd = TagToWRD(szTag_);
		if( _wrd == NULL)
			return 0;
		else
		{
			if( (gRTInfo.lpBase[_wrd]).Status & RTUBIT_VALID )
				return _wrd;
			else
				return 0;
		}
	}

	for(_i=1; _i<gRTInfo.wAdding; _i++)
	{
		if(  (gRTInfo.lpBase[_i].Status & RTUBIT_VALID )
		   &&((gRTInfo.lpBase[_i]).lpTag)  )
		{
			if( !CmpTag( REAL_GET_TAG(_i),
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
//	1:			successful with real name
//	2:			successful with pseudo name
//...................................................................
_DLLEXP int _DLLFUNC RealGetTag(WRD wrd_, LPSTR szTag_, WORD wSize_)
{
	int 	_ret = 0;

	CLEAR_LAST_ERROR;

	if( wrd_ >= gRTInfo.wCount )
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

	if( !wrd_ ) //0 is a valid parameter
	{
		WRDToTag( wrd_, szTag_, wSize_ );
		_ret = 2;	//pseudo name
	}
	else
	{
		LPRTU	_lpRtu;
		_lpRtu = gRTInfo.lpBase + wrd_;

		if( _lpRtu->Status & RTUBIT_VALID )
		{
			if( (_lpRtu->lpTag) && *(_lpRtu->lpTag->data) )
			{
				StringCopyn( szTag_, pREAL_GET_TAG(_lpRtu), wSize_ );
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
//Note:
//	In the release version, the Status can be read even the RTU
//is invalid, this is more resionable than returns a zero when
//the RTU is invalid;
//...................................................................
_DLLEXP STATUS _DLLFUNC RealGetStatus(WRD wrd_)
{
	if( wrd_ >= gRTInfo.wCount )
	{
		CORE_ASSERT(FALSE);
		SET_LAST_ERROR( ERR_Invalid_Entries );
		return NULL;
	}

	CORE_ASSERT( gRTInfo.lpBase[wrd_].Status & RTUBIT_VALID );
	return (gRTInfo.lpBase)[wrd_].Status;
}

//...................................................................
//Note:
//	In the release version, the Status can be read even the RTU
//is invalid, this is more resionable than returns a zero when
//the RTU is invalid;
//...................................................................
_DLLEXP STATUS _DLLFUNC RealSetStatus( WRD wrd_, STATUS status_ )
{
	STATUS _ret;
	
	if( wrd_ >= gRTInfo.wCount )
	{
		CORE_ASSERT(FALSE);
		SET_LAST_ERROR( ERR_Invalid_Entries );
		return 0;
	}

	CORE_ASSERT( gRTInfo.lpBase[wrd_].Status & RTUBIT_VALID );
	_ret = (gRTInfo.lpBase)[wrd_].Status;

	(gRTInfo.lpBase)[wrd_].Status = status_;

	return _ret;
}

//...................................................................
//Note:
//	In the release version, the value can be read even the RTU
//is invalid, this is more resionable than returns a zero when
//the RTU is invalid;
//...................................................................
_DLLEXP DIRTY _DLLFUNC RealGetDirty(WRD wrd_)
{
	if( wrd_ >= gRTInfo.wCount )
	{
		CORE_ASSERT(FALSE);
		SET_LAST_ERROR( ERR_Invalid_Entries );
		return NULL;
	}

	CORE_ASSERT( gRTInfo.lpBase[wrd_].Status & RTUBIT_VALID );
	return (gRTInfo.lpBase)[wrd_].Dirty;
}

//...................................................................
//Decription:
//	Set the Real tag if the new tag is legal after checked;
//Note:
//	if the new tag begins with '#', returns FALSE;
//...................................................................
_DLLEXP BOOL _DLLFUNC RealSetTag(WRD wrd_, LPCSTR szTag_)
{
	BOOL		_bOk;
	LPRTU		_lpRtu = (gRTInfo.lpBase) + wrd_;

	if( !szTag_ )
	{
		if( _lpRtu->lpTag )
			_lpRtu->Dirty |= RTUBIT_DIRTY_TAG;

		CobjFree( _lpRtu->lpTag );
		_lpRtu->lpTag = NULL;
		return TRUE;
	}

	if( *szTag_ == '#' )
		return FALSE;

	if( !CmpTag( pREAL_GET_TAG(_lpRtu), szTag_, CORE_MAX_TAG_LENGTH ) )
		return TRUE;	//the old and new tag are identical

	if( _lpRtu->Status & RTUBIT_VALID )
	{
		LPCOBJ	_lpTagOld, _lpTagNew;

		if( strlen(szTag_) >= CORE_MAX_TAG_LENGTH - 1)
			return FALSE;

		//save the old tag
		_lpTagOld = _lpRtu->lpTag;
		_lpTagNew = AllocTag( szTag_, &_bOk );

		if( !_bOk )
			return FALSE;

		_lpRtu->lpTag = _lpTagNew;

		if( !RealCheckLegality( wrd_ ) )
		{
			//the new tag is illegal
			CobjFree( _lpTagNew );
			_lpRtu->lpTag = _lpTagOld;
			return FALSE;
		}
		else
		{
			//set tag successful, and delete the old tag
			CobjFree( _lpTagOld );
			_lpRtu->Dirty |= RTUBIT_DIRTY_TAG;
			return TRUE;
		}
	}

	return FALSE;
}

//...................................................................
//...................................................................
_DLLEXP RTInfo FAR* _DLLFUNC GetRealTableInformation()
{
	return &gRTInfo;
}

//...................................................................
//...................................................................
_DLLEXP WORD _DLLFUNC RealGetElementType( WORD element_ )
{
	int 	_i;

	for(_i=0; _i<_NUM_NAMESET; _i++)
		if(element_ == ssNameSet[_i].element)
			return ssNameSet[_i].type;

	return 0;
}

//...................................................................
//...................................................................
_DLLEXP BOOL _DLLFUNC RealGetElementName(WORD element_, LPSTR szName_, WORD wSize_)
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
_DLLEXP WORD _DLLFUNC RealGetElement(LPSTR szName_)
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
_DLLEXP LPSTR _DLLFUNC RealGetData(LPSTR lpsz_, int size_, WRD wrd_, WORD order_)
{
	PU					_pu;
	GETVALUESTRING		_gvs;
	DWORD				_ret;
	int 				_i;

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

	_pu.prefix = PF_TBX|TBX_RT|order_;
	_pu.du.zzk__WRD = wrd_;
	_ret = PUGetValueString( &_pu, &_gvs, ssNameSet[_i].vsof );

	return _ret? lpsz_: NULL;
}

//...................................................................
//...................................................................
_DLLEXP BOOL _DLLFUNC RealSetData(LPCSTR lpsz_, WRD wrd_, WORD order_)
{
	PU			_pu;
	DWORD		_ret;
	
	int 		_i;

	if(  ( wrd_ >= gRTInfo.wCount )
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

	_pu.prefix = PF_TBX|TBX_RT|order_;
	_pu.du.zzk__WRD = wrd_;
	_ret = PUSetValueString( &_pu, lpsz_, ssNameSet[_i].vsof );

	return _ret? TRUE: FALSE;
}

//...................................................................
//Entries:
//	bPublicDirty_
//		If this is TRUE, the private dirty bits are excluded.
//...................................................................
_DLLEXP BOOL _DLLFUNC IsRealTableDirty( BOOL bPublicDirty_ )
{
	WRD 	_i;
	DIRTY	_mask = RTUBIT_DIRTY_ALL;

	if( bPublicDirty_ )
		_mask &= ~( RTUBIT_DIRTY_ALARM|
					RTUBIT_DIRTY_LASTLOG|
					RTUBIT_DIRTY_TRACEDATA|
					RTUBIT_DIRTY_DIRTY);

	for(_i=1; _i<gRTInfo.wAdding; _i++)
	{
		if( (gRTInfo.lpBase + _i)->Dirty & _mask )
			return TRUE;
	}

	return FALSE;
}

//...................................................................
//...................................................................
_DLLEXP void _DLLFUNC RealClearTableDirty()
{
	WRD 	_i;

	for(_i=1; _i<gRTInfo.wAdding; _i++)
		(gRTInfo.lpBase + _i)->Dirty &= (~RTUBIT_DIRTY_ALL);

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
_DLLEXP ALARM _DLLFUNC RealAlarm( WRD wrd_ )
{
	LPRTU	_lpRtu; 
	ALARM	_alarm; 	//temp value

	CLEAR_LAST_ERROR;
	if(  ( wrd_ >= gRTInfo.wCount )
	   ||(!RealIsValid(wrd_)) )
	{
		SET_LAST_ERROR( ERR_Invalid_Entries );
		CORE_ASSERT( FALSE );
		return FALSE;
	}

	_lpRtu = gRTInfo.lpBase + wrd_;

	//we check HiHi first
	if( _lpRtu->Status & RTUBIT_ENABLEHIHI )
	{
		//value touches HiHi
		if(  ( !(_lpRtu->Alarm & RTUBIT_ALMSTATE_HIHI) )
		   &&( _lpRtu->Value >= _lpRtu->HiHi + _lpRtu->AlarmBand ) )	//Note: here is "+"
		{
			_alarm = _lpRtu->Alarm; //old alarm state
			_lpRtu->Alarm &= (~(RTUBIT_ACK|RTUBIT_ALMSTATE_MASK));		//clear ACK and old alarm state
			_lpRtu->Alarm |= RTUBIT_ALMSTATE_HIHI;
			
			PostAlarm( TB_RT, wrd_, _lpRtu->Alarm, _alarm );
			return _lpRtu->Alarm;
		}
		
		//value keepes HiHi
		if(  ( (_lpRtu->Alarm & RTUBIT_ALMSTATE_HIHI) )
		   &&( _lpRtu->Value >= _lpRtu->HiHi - _lpRtu->AlarmBand ) )	//Note: here is "-"
		{
			return _lpRtu->Alarm;	//do nothing
		}

		//otherwise, goto the following cases
	}

	//then we check LoLo before Lo
	if( _lpRtu->Status & RTUBIT_ENABLELOLO )
	{
		//value touches LOLO
		if(  ( !(_lpRtu->Alarm & RTUBIT_ALMSTATE_LOLO) )
		   &&( _lpRtu->Value <= _lpRtu->LoLo - _lpRtu->AlarmBand ) )	//Note: here is "-"
		{
			_alarm = _lpRtu->Alarm;  //old alarm state
			_lpRtu->Alarm &= (~(RTUBIT_ACK|RTUBIT_ALMSTATE_MASK));		//clear ACK and old alarm state
			_lpRtu->Alarm |= RTUBIT_ALMSTATE_LOLO;
			
			PostAlarm( TB_RT, wrd_, _lpRtu->Alarm, _alarm );
			return _lpRtu->Alarm;
		}
		
		//value keepes LOLO
		if(  ( (_lpRtu->Alarm & RTUBIT_ALMSTATE_LOLO) )
		   &&( _lpRtu->Value <= _lpRtu->LoLo + _lpRtu->AlarmBand ) )	//Note: here is "+"
		{
			return _lpRtu->Alarm;	//do nothing
		}

		//otherwise, goto the following cases
	}

	//then we check Hi or Lo
	if( _lpRtu->Status & RTUBIT_ENABLELO )
	{
		//value touches LO
		if(  ( !(_lpRtu->Alarm & RTUBIT_ALMSTATE_LO) )
		   &&( _lpRtu->Value <= _lpRtu->Lo - _lpRtu->AlarmBand ) )	//Note: here is "-"
		{
			_alarm = _lpRtu->Alarm;  //old alarm state
			_lpRtu->Alarm &= (~(RTUBIT_ACK|RTUBIT_ALMSTATE_MASK));		//clear ACK and old alarm state
			_lpRtu->Alarm |= RTUBIT_ALMSTATE_LO;
			
			PostAlarm( TB_RT, wrd_, _lpRtu->Alarm, _alarm );
			return _lpRtu->Alarm;
		}
		
		//value keepes LO
		if(  ( (_lpRtu->Alarm & RTUBIT_ALMSTATE_LO) )
		   &&( _lpRtu->Value <= _lpRtu->Lo + _lpRtu->AlarmBand ) )	//Note: here is "+"
		{
			return _lpRtu->Alarm;	//do nothing
		}

		//otherwise, goto the following cases
	}

	//check Hi here
	if( _lpRtu->Status & RTUBIT_ENABLEHI )
	{
		//value touches Hi
		if(  ( !(_lpRtu->Alarm & RTUBIT_ALMSTATE_HI) )
		   &&( _lpRtu->Value >= _lpRtu->Hi + _lpRtu->AlarmBand ) )	//Note: here is "+"
		{
			_alarm = _lpRtu->Alarm; //old alarm state
			_lpRtu->Alarm &= (~(RTUBIT_ACK|RTUBIT_ALMSTATE_MASK));		//clear ACK and old alarm state
			_lpRtu->Alarm |= RTUBIT_ALMSTATE_HI;
			
			PostAlarm( TB_RT, wrd_, _lpRtu->Alarm, _alarm );
			return _lpRtu->Alarm;
		}
		
		//value keepes Hi
		if(  ( (_lpRtu->Alarm & RTUBIT_ALMSTATE_HI) )
		   &&( _lpRtu->Value >= _lpRtu->Hi - _lpRtu->AlarmBand ) )	//Note: here is "-"
		{
			return _lpRtu->Alarm;	//do nothing
		}

		//otherwise, goto the following cases
	}

	////////////////
	//here, value should be in normal range
	
	//value comes into normal range or alarm thresholds are disabled
	if( _lpRtu->Alarm & RTUBIT_ALMSTATE_MASK )
	{
		_alarm = _lpRtu->Alarm; //old alarm state
		//clear alarm	
		_lpRtu->Alarm &= (~(RTUBIT_ACK|RTUBIT_ALMSTATE_MASK));		//clear ACK and old alarm state
		PostAlarm( TB_RT, wrd_, _lpRtu->Alarm, _alarm );
		return _lpRtu->Alarm;
	}

	//value keep in normal range
	//do nothing here
	return _lpRtu->Alarm;
}

static BOOL RealInitializeTrace( WRD wrd_ )
{
	LPRTU	_lpRtu; 

	CLEAR_LAST_ERROR;
	if(  ( wrd_ >= gRTInfo.wCount )
	   ||(!RealIsValid(wrd_)) )
	{
		SET_LAST_ERROR( ERR_Invalid_Entries );
		CORE_ASSERT( FALSE );
		return FALSE;
	}

	_lpRtu = gRTInfo.lpBase + wrd_;

	//first, free the old trace data
	_FreeTraceData( _lpRtu->lpTraceData );
	_lpRtu->lpTraceData = NULL;

	if( _lpRtu->Status & RTUBIT_TRACE )
	{
		DWORD		_items;
		LPTRACE 	_lpTrace;

		_items = _lpRtu->TraceTime / gCoreInfo.uInterval;
		if( !_items )
			return TRUE;

		_lpTrace = (LPTRACE)FragAlloc( sizeof(TRACEHEAD) + _items * sizeof(REAL)  );
		if( !_lpTrace )
			return FALSE;

		//initialize the structure
		_lpTrace->Items = _items;
		_lpTrace->Adding = 0;
		_lpTrace->ValidItems = 0;
		_lpTrace->ItemBits = sizeof(REAL);

		//set the pointer to RTU
		_lpRtu->lpTraceData = _lpTrace;
	
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
//	This function sets the ACK bit and posts alarm message.
//Returns:
//	If no alarms are set, returns FALSE; if alarms have been 
//	acknowledged, return TRUE;
//Note:
//	When operator acknowledges the alarms, this function will
//	be called
//...................................................................
_DLLEXP BOOL _DLLFUNC RealAckAlarm( WRD wrd_ )
{
	LPRTU	_lpRtu; 

	CLEAR_LAST_ERROR;
	if(  ( wrd_ >= gRTInfo.wCount )
	   ||(!RealIsValid(wrd_)) )
	{
		SET_LAST_ERROR( ERR_Invalid_Entries );
		CORE_ASSERT( FALSE );
		return FALSE;
	}

	_lpRtu = gRTInfo.lpBase + wrd_;
	
	if( _lpRtu->Alarm & RTUBIT_ALMSTATE_MASK )
	{
		if( !(_lpRtu->Alarm & RTUBIT_ACK) ) //not ACK yet
		{
			ALARM	_alarm = _lpRtu->Alarm;

			_lpRtu->Alarm |= RTUBIT_ACK;		//set ACK bit
			PostAlarm( TB_RT, wrd_, _lpRtu->Alarm, _alarm ); 
		}
		return TRUE;
	}
	return FALSE;	//no alarm
}

//...................................................................
//...................................................................
_DLLEXP BOOL _DLLFUNC PUReadRealTable(SimplePU FAR* to_, LPPU from_)
{
	LPRTU		_lpRtu;
	SimplePU	_tmp;

	if( (from_->du.zzk__WRD == 0) || (from_->du.zzk__WRD >= gRTInfo.wAdding) )
		return FALSE;

	_lpRtu = gRTInfo.lpBase + from_->du.zzk__WRD;

	switch(from_->prefix & XT_MASK)
	{
		case RTU_VALUE:
			_tmp.prefix = PF_IMM|IMM_REAL;
			_tmp.du.zzk__REAL = _lpRtu->Value;
			break;

		case RTU_MIN:
			_tmp.prefix = PF_IMM|IMM_REAL;
			_tmp.du.zzk__REAL = _lpRtu->Min;
			break;

		case RTU_MAX:
			_tmp.prefix = PF_IMM|IMM_REAL;
			_tmp.du.zzk__REAL = _lpRtu->Max;
			break;

		case RTU_USERDATA:
			_tmp.prefix = PF_IMM|IMM_DWORD;
			_tmp.du.zzk__DWORD = _lpRtu->UserData;
			break;

		case RTU_SPAN:
			_tmp.prefix = PF_IMM|IMM_REAL;
			_tmp.du.zzk__REAL = _lpRtu->Max - _lpRtu->Min;
			break;

		case RTU_STATUS:
			_tmp.prefix = PF_IMM|IMM_STATUS;
			_tmp.du.zzk__STATUS = _lpRtu->Status;
			break;

		case RTU_TAG:
			_tmp.prefix = PF_IMM|IMM_LPCOBJ;
			_tmp.du.zzk__LPCOBJ = _lpRtu->lpTag;
			/*
				after calling PUCast, _tmp.du.zzk__LPCOBJ will be cleared.
			*/
			break;

		case RTU_UNIT:
			_tmp.prefix = PF_IMM|IMM_LPCOBJ;
			_tmp.du.zzk__LPCOBJ = _lpRtu->lpUnit;
			/*
				after calling PUCast, _tmp.du.zzk__LPCOBJ will be cleared.
			*/
			break;

		case RTU_COMMENT:
			_tmp.prefix = PF_IMM|IMM_LPCOBJ;
			_tmp.du.zzk__LPCOBJ = _lpRtu->lpComment;
			/*
				after calling PUCast, _tmp.du.zzk__LPCOBJ will be cleared.
			*/
			break;

		case RTU_DIRTY:
			_tmp.prefix = PF_IMM|IMM_DIRTY;
			_tmp.du.zzk__DIRTY = _lpRtu->Dirty;
			break;

		case RTU_LOGBAND:
			_tmp.prefix = PF_IMM|IMM_REAL;
			_tmp.du.zzk__REAL = _lpRtu->LogBand;
			break;

		case RTU_LASTLOG:
			_tmp.prefix = PF_IMM|IMM_REAL;
			_tmp.du.zzk__REAL = _lpRtu->LastLog;
			break;

		case RTU_ALARMBAND:
			_tmp.prefix = PF_IMM|IMM_REAL;
			_tmp.du.zzk__REAL = _lpRtu->AlarmBand;
			break;

		case RTU_ALARM:
			_tmp.prefix = PF_IMM|IMM_ALARM;
			_tmp.du.zzk__ALARM = _lpRtu->Alarm;
			break;

		case RTU_HIHI:
			_tmp.prefix = PF_IMM|IMM_REAL;
			_tmp.du.zzk__REAL = _lpRtu->HiHi;
			break;

		case RTU_HI:
			_tmp.prefix = PF_IMM|IMM_REAL;
			_tmp.du.zzk__REAL = _lpRtu->Hi;
			break;

		case RTU_LO:
			_tmp.prefix = PF_IMM|IMM_REAL;
			_tmp.du.zzk__REAL = _lpRtu->Lo;
			break;

		case RTU_LOLO:
			_tmp.prefix = PF_IMM|IMM_REAL;
			_tmp.du.zzk__REAL = _lpRtu->LoLo;
			break;

		case RTU_ALM:	//alarm state
			_tmp.prefix = PF_IMM|IMM_ALARM;
			_tmp.du.zzk__ALARM = _lpRtu->Alarm & RTUBIT_ALMSTATE_MASK;
			break;

		case RTU_ALMHIHI:
			_tmp.prefix = PF_IMM|IMM_BOOL;
			_tmp.du.zzk__BOOL = _lpRtu->Alarm & RTUBIT_ALMSTATE_HIHI;
			break;

		case RTU_ALMHI:
			_tmp.prefix = PF_IMM|IMM_BOOL;
			_tmp.du.zzk__BOOL = _lpRtu->Alarm & RTUBIT_ALMSTATE_HI;
			break;

		case RTU_ALMLO:
			_tmp.prefix = PF_IMM|IMM_BOOL;
			_tmp.du.zzk__BOOL = _lpRtu->Alarm & RTUBIT_ALMSTATE_LO;
			break;

		case RTU_ALMLOLO:
			_tmp.prefix = PF_IMM|IMM_BOOL;
			_tmp.du.zzk__BOOL = _lpRtu->Alarm & RTUBIT_ALMSTATE_LOLO;
			break;

		case RTU_ANA:
			_tmp.prefix = PF_IMM|IMM_BOOL;
			_tmp.du.zzk__BOOL = 
				(_lpRtu->Alarm & RTUBIT_ALMSTATE_MASK) && (!(_lpRtu->Alarm & RTUBIT_ACK));
			break;

		case RTU_ANAHIHI:
			_tmp.prefix = PF_IMM|IMM_BOOL;
			_tmp.du.zzk__BOOL = 
				(_lpRtu->Alarm & RTUBIT_ALMSTATE_HIHI) && (!(_lpRtu->Alarm & RTUBIT_ACK));
				break;

		case RTU_ANAHI:
			_tmp.prefix = PF_IMM|IMM_BOOL;
			_tmp.du.zzk__BOOL = 
				(_lpRtu->Alarm & RTUBIT_ALMSTATE_HI) && (!(_lpRtu->Alarm & RTUBIT_ACK));
			break;

		case RTU_ANALO:
			_tmp.prefix = PF_IMM|IMM_BOOL;
			_tmp.du.zzk__BOOL = 
				(_lpRtu->Alarm & RTUBIT_ALMSTATE_LO) && (!(_lpRtu->Alarm & RTUBIT_ACK));
			break;

		case RTU_ANALOLO:
			_tmp.prefix = PF_IMM|IMM_BOOL;
			_tmp.du.zzk__BOOL = 
				(_lpRtu->Alarm & RTUBIT_ALMSTATE_LOLO) && (!(_lpRtu->Alarm & RTUBIT_ACK));
			break;

		case RTU_ACK:
			_tmp.prefix = PF_IMM|IMM_BOOL;
			_tmp.du.zzk__BOOL = _lpRtu->Alarm & RTUBIT_ACK;
			break;

		case RTU_ACKHIHI:
			_tmp.prefix = PF_IMM|IMM_BOOL;
			_tmp.du.zzk__BOOL = 
				(_lpRtu->Alarm & RTUBIT_ALMSTATE_HIHI) && (_lpRtu->Alarm & RTUBIT_ACK);
			break;

		case RTU_ACKHI:
			_tmp.prefix = PF_IMM|IMM_BOOL;
			_tmp.du.zzk__BOOL = 
				(_lpRtu->Alarm & RTUBIT_ALMSTATE_HI) && (_lpRtu->Alarm & RTUBIT_ACK);
				break;

		case RTU_ACKLO:
			_tmp.prefix = PF_IMM|IMM_BOOL;
			_tmp.du.zzk__BOOL = 
				(_lpRtu->Alarm & RTUBIT_ALMSTATE_LO) && (_lpRtu->Alarm & RTUBIT_ACK);
				break;

		case RTU_ACKLOLO:
			_tmp.prefix = PF_IMM|IMM_BOOL;
			_tmp.du.zzk__BOOL = 
				(_lpRtu->Alarm & RTUBIT_ALMSTATE_LOLO) && (_lpRtu->Alarm & RTUBIT_ACK);
			break;

		case RTU_ENABLEALM:
			_tmp.prefix = PF_IMM|IMM_STATUS;
			_tmp.du.zzk__STATUS = _lpRtu->Status & RTUBIT_ENABLEMASK;
			break;

		case RTU_ENABLEHIHI:
			_tmp.prefix = PF_IMM|IMM_BOOL;
			_tmp.du.zzk__BOOL = _lpRtu->Status & RTUBIT_ENABLEHIHI;
			break;

		case RTU_ENABLEHI:
			_tmp.prefix = PF_IMM|IMM_BOOL;
			_tmp.du.zzk__BOOL = _lpRtu->Status & RTUBIT_ENABLEHI;
			break;

		case RTU_ENABLELO:
			_tmp.prefix = PF_IMM|IMM_BOOL;
			_tmp.du.zzk__BOOL = _lpRtu->Status & RTUBIT_ENABLELO;
			break;

		case RTU_ENABLELOLO:
			_tmp.prefix = PF_IMM|IMM_BOOL;
			_tmp.du.zzk__BOOL = _lpRtu->Status & RTUBIT_ENABLELOLO;
			break;

		case RTU_TRACETIME:
			_tmp.prefix = PF_IMM|IMM_DWORD;
			_tmp.du.zzk__DWORD = _lpRtu->TraceTime;
			break;

		case RTU_TRACEDATA:
			_tmp.prefix = PF_IMM|IMM_FPTR;
			_tmp.du.zzk__FPTR = _lpRtu->lpTraceData;
			break;

		case RTU_VALID:
			_tmp.prefix = PF_IMM|IMM_BOOL;
			_tmp.du.zzk__BOOL = _lpRtu->Status & RTUBIT_VALID;
			break;

		case RTU_LOGHIS:
			_tmp.prefix = PF_IMM|IMM_BOOL;
			_tmp.du.zzk__BOOL = _lpRtu->Status & RTUBIT_LOGHIS;
			break;

		case RTU_TRACE:
			_tmp.prefix = PF_IMM|IMM_BOOL;
			_tmp.du.zzk__BOOL = _lpRtu->Status & RTUBIT_TRACE;
			break;

		case RTU_VALUE_PERCENT:
			_tmp.prefix = PF_IMM|IMM_REAL;
			_tmp.du.zzk__REAL = _lpRtu->Value * ( _lpRtu->Max - _lpRtu->Min );
			break;

		default:
			CORE_ASSERT(0);
			return FALSE;
	}

	return PUCast(to_, &_tmp);
}

//...................................................................
//...................................................................
#define _RTU_SET_DIRTY( _ELE_ ) 						\
		{_lpRtu->Dirty |= RTUBIT_DIRTY_##_ELE_;}

_DLLEXP BOOL _DLLFUNC PUWriteRealTable(LPPU to_, SimplePU FAR* from_)
{
	SimplePU	_tmp;
	LPRTU		_lpRtu;
	REAL		_inner; //temp value

	_lpRtu = (gRTInfo.lpBase) + to_->du.zzk__WRD;
	if( !(_lpRtu->Status & RTUBIT_VALID) )
	{
		LOGERR1( "Write to invalid RTU: %d", to_->du.zzk__WRD );
		return FALSE;
	}

	switch(to_->prefix & 0x00FF)
	{
		case RTU_VALUE:
			_tmp.prefix = PF_IMM|IMM_REAL;
			if( !PUCast(&_tmp, from_) ) return FALSE;
			if( _lpRtu->Value == _tmp.du.zzk__REAL )
				return TRUE;	//same value
			_lpRtu->Value = _tmp.du.zzk__REAL;
			RealAlarm( to_->du.zzk__WRD );

			_RTU_SET_DIRTY( VALUE )
			break;

		case RTU_MIN:
			_tmp.prefix = PF_IMM|IMM_REAL;
			if( !PUCast(&_tmp, from_) ) return FALSE;
			if( _lpRtu->Min == _tmp.du.zzk__REAL )
				return TRUE;	//same value
			if( _lpRtu->Status & RTUBIT_DISABLE_CHECK )
			{
				_lpRtu->Min = _tmp.du.zzk__REAL;
			}
			else
			{
				if( _tmp.du.zzk__REAL < _lpRtu->Max )
					_lpRtu->Min = _tmp.du.zzk__REAL;
				else
					return FALSE;	//invalid min value
			}

			_RTU_SET_DIRTY( MIN )
			break;

		case RTU_MAX:
			_tmp.prefix = PF_IMM|IMM_REAL;
			if( !PUCast(&_tmp, from_) ) return FALSE;
			if( _lpRtu->Max == _tmp.du.zzk__REAL )
				return TRUE;	//same value
			if( _lpRtu->Status & RTUBIT_DISABLE_CHECK )
			{
				_lpRtu->Max = _tmp.du.zzk__REAL;
			}
			else
			{
				if( _tmp.du.zzk__REAL > _lpRtu->Min )
					_lpRtu->Max = _tmp.du.zzk__REAL;
				else
					return FALSE;	//invalid min value
			}

			_RTU_SET_DIRTY( MAX )
			break;

		case RTU_TAG:
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
				//need not check here, RealSetTag will do everything 
				if( RealSetTag(to_->du.zzk__WRD, PU_GET_COBJ_STRING(from_) ) )
				{
					//if success, delete the imported cobject
					CobjFree( from_->du.zzk__LPCOBJ );
					from_->du.zzk__LPCOBJ = NULL;

					//dirty has been set in RealSetTag
					//	_RTU_SET_DIRTY( TAG )
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

		case RTU_UNIT:
			if( IS_STRING_COBJ( from_->du.zzk__LPCOBJ ) )
			{
				if( !CobjCompareString( _lpRtu->lpUnit, from_->du.zzk__LPCOBJ ) )
					return TRUE;

				CobjFree( _lpRtu->lpUnit );
				_lpRtu->lpUnit = from_->du.zzk__LPCOBJ;
				from_->du.zzk__LPCOBJ = 0;

				_RTU_SET_DIRTY( UNIT )
				return TRUE;
			}
			else
			{
				LOGERR1("PU data is not a String:PU=%lp", from_);
				return FALSE;
			}
			break;

		case RTU_COMMENT:
			if( IS_STRING_COBJ( from_->du.zzk__LPCOBJ ) )
			{
				if( !CobjCompareString( _lpRtu->lpComment, from_->du.zzk__LPCOBJ ) )
					return TRUE;

				CobjFree ( _lpRtu->lpComment );
				_lpRtu->lpComment = from_->du.zzk__LPCOBJ;
				from_->du.zzk__LPCOBJ = 0;

				_RTU_SET_DIRTY( COMMENT )
				return TRUE;
			}
			else
			{
				LOGERR1("PU data is not a String:PU=%lp", from_);
				return FALSE;
			}

		case RTU_HIHI:
			_tmp.prefix = PF_IMM|IMM_REAL;
			if( !PUCast(&_tmp, from_) ) return FALSE;
			if( _lpRtu->HiHi == _tmp.du.zzk__REAL )
				return TRUE;

			if( _lpRtu->Status & RTUBIT_DISABLE_CHECK )
			{
				_lpRtu->HiHi = _tmp.du.zzk__REAL;
				_RTU_SET_DIRTY( HIHI )
				return TRUE;
			}

			_inner = _lpRtu->HiHi;
			_lpRtu->HiHi = _tmp.du.zzk__REAL;
			if( !RealCheckAlarmThreshold( to_->du.zzk__WRD ) )
			{
				_lpRtu->HiHi = _inner;
				return FALSE;
			}
			_RTU_SET_DIRTY( HIHI )
			RealAlarm( to_->du.zzk__WRD );
			break;

		case RTU_HI:
			_tmp.prefix = PF_IMM|IMM_REAL;
			if( !PUCast(&_tmp, from_) ) return FALSE;
			
			if( _lpRtu->Hi == _tmp.du.zzk__REAL )
				return TRUE;

			if( _lpRtu->Status & RTUBIT_DISABLE_CHECK )
			{
				_lpRtu->Hi = _tmp.du.zzk__REAL;
				_RTU_SET_DIRTY( HI )
				return TRUE;
			}

			_inner = _lpRtu->Hi;
			_lpRtu->Hi = _tmp.du.zzk__REAL;
			if( !RealCheckAlarmThreshold( to_->du.zzk__WRD ) )
			{
				_lpRtu->Hi = _inner;
				return FALSE;
			}
			_RTU_SET_DIRTY( HI )
			RealAlarm( to_->du.zzk__WRD );
			break;

		case RTU_LO:
			_tmp.prefix = PF_IMM|IMM_REAL;
			if( !PUCast(&_tmp, from_) ) return FALSE;

			if( _lpRtu->Lo == _tmp.du.zzk__REAL )
				return TRUE;
			
			if( _lpRtu->Status & RTUBIT_DISABLE_CHECK )
			{
				_lpRtu->Lo = _tmp.du.zzk__REAL;
				_RTU_SET_DIRTY( LO )
				return TRUE;
			}

			_inner = _lpRtu->Lo;
			_lpRtu->Lo = _tmp.du.zzk__REAL;
			if( !RealCheckAlarmThreshold( to_->du.zzk__WRD ) )
			{
				_lpRtu->Lo = _inner;
				return FALSE;
			}
			_RTU_SET_DIRTY( LO )
			RealAlarm( to_->du.zzk__WRD );
			break;

		case RTU_LOLO:
			_tmp.prefix = PF_IMM|IMM_REAL;
			if( !PUCast(&_tmp, from_) ) return FALSE;

			if( _lpRtu->LoLo == _tmp.du.zzk__REAL )
				return TRUE;
			
			if( _lpRtu->Status & RTUBIT_DISABLE_CHECK )
			{
				_lpRtu->LoLo = _tmp.du.zzk__REAL;
				_RTU_SET_DIRTY( LOLO )
				return TRUE;
			}

			_inner = _lpRtu->LoLo;
			_lpRtu->LoLo = _tmp.du.zzk__REAL;
			if( !RealCheckAlarmThreshold( to_->du.zzk__WRD ) )
			{
				_lpRtu->LoLo = _inner;
				return FALSE;
			}
			_RTU_SET_DIRTY( LOLO )
			RealAlarm( to_->du.zzk__WRD );
			break;

		case RTU_LOGBAND:
			_tmp.prefix = PF_IMM|IMM_REAL;
			if( !PUCast(&_tmp, from_) ) return FALSE;
			if( _lpRtu->LogBand == _tmp.du.zzk__REAL )
				return TRUE;	//same value
			_lpRtu->LogBand = _tmp.du.zzk__REAL;
			
			_RTU_SET_DIRTY( LOGBAND )
			break;

		case RTU_ALARMBAND:
			_tmp.prefix = PF_IMM|IMM_REAL;
			if( !PUCast(&_tmp, from_) ) return FALSE;
			if( _lpRtu->AlarmBand == _tmp.du.zzk__REAL )
				return TRUE;	//same value
			_lpRtu->AlarmBand = _tmp.du.zzk__REAL;
			
			_RTU_SET_DIRTY( ALARMBAND )
			break;

		case RTU_USERDATA:
			_tmp.prefix = PF_IMM|IMM_DWORD;
			if( !PUCast(&_tmp, from_) ) return FALSE;
			if( _lpRtu->UserData == _tmp.du.zzk__DWORD )
				return TRUE;	//same value
			_lpRtu->UserData = _tmp.du.zzk__DWORD;
			
			_RTU_SET_DIRTY( USERDATA )
			break;

		case RTU_TRACETIME:
		{
			_tmp.prefix = PF_IMM|IMM_DWORD;
			if( !PUCast(&_tmp, from_) ) return FALSE;
			if( _lpRtu->TraceTime == _tmp.du.zzk__DWORD )
				return TRUE;	//same value
			_lpRtu->TraceTime = _tmp.du.zzk__DWORD;
			
			_RTU_SET_DIRTY( TRACETIME )

			if( IS_RUN_MODE() )
				return RealInitializeTrace( to_->du.zzk__WRD );
			
			break;
		}

		case RTU_LOGHIS:
			_tmp.prefix = PF_IMM|IMM_BOOL;
			if( !PUCast(&_tmp, from_) ) return FALSE;
			if(    ( (_lpRtu->Status & RTUBIT_LOGHIS)? TRUE: FALSE )
				== ( _tmp.du.zzk__BOOL?TRUE:FALSE ) )
				return TRUE;	//same value

			_lpRtu->Status ^= (RTUBIT_LOGHIS);		//tag this bit
			_RTU_SET_DIRTY( STATUS )
			break;

		case RTU_TRACE:
		{
			_tmp.prefix = PF_IMM|IMM_BOOL;
			if( !PUCast(&_tmp, from_) ) return FALSE;
			if(    ( (_lpRtu->Status & RTUBIT_TRACE)? TRUE: FALSE )
				== ( _tmp.du.zzk__BOOL?TRUE:FALSE ) )
				return TRUE;	//same value

			_lpRtu->Status ^= (RTUBIT_TRACE);		//tag this bit
			_RTU_SET_DIRTY( STATUS )
			
			if( IS_RUN_MODE() )
				return RealInitializeTrace( to_->du.zzk__WRD );
			
			break;
		}

		case RTU_ENABLEHIHI:
			_tmp.prefix = PF_IMM|IMM_BOOL;
			if( !PUCast(&_tmp, from_) ) return FALSE;
			if(    ( (_lpRtu->Status & RTUBIT_ENABLEHIHI)? TRUE: FALSE )
				== ( _tmp.du.zzk__BOOL?TRUE:FALSE ) )
				return TRUE;	//same value

			_lpRtu->Status ^= (RTUBIT_ENABLEHIHI);		//tag this bit
			_RTU_SET_DIRTY( STATUS )
			
			if( !RealCheckAlarmThreshold(to_->du.zzk__WRD) )
				LOGERR1( "Alarm threshold error, RTU:0x%X", to_->du.zzk__WRD ); 
			RealAlarm( to_->du.zzk__WRD );
			break;

		case RTU_ENABLEHI:
			_tmp.prefix = PF_IMM|IMM_BOOL;
			if( !PUCast(&_tmp, from_) ) return FALSE;
			if(    ( (_lpRtu->Status & RTUBIT_ENABLEHI)? TRUE: FALSE )
				== ( _tmp.du.zzk__BOOL?TRUE:FALSE ) )
				return TRUE;	//same value

			_lpRtu->Status ^= (RTUBIT_ENABLEHI);		//tag this bit
			_RTU_SET_DIRTY( STATUS )
			
			if( !RealCheckAlarmThreshold(to_->du.zzk__WRD) )
				LOGERR1( "Alarm threshold error, RTU:0x%X", to_->du.zzk__WRD ); 
			RealAlarm( to_->du.zzk__WRD );
			break;

		case RTU_ENABLELO:
			_tmp.prefix = PF_IMM|IMM_BOOL;
			if( !PUCast(&_tmp, from_) ) return FALSE;
			if(    ( (_lpRtu->Status & RTUBIT_ENABLELO)? TRUE: FALSE )
				== ( _tmp.du.zzk__BOOL?TRUE:FALSE ) )
				return TRUE;	//same value

			_lpRtu->Status ^= (RTUBIT_ENABLELO);		//tag this bit
			_RTU_SET_DIRTY( STATUS )
			
			if( !RealCheckAlarmThreshold(to_->du.zzk__WRD) )
				LOGERR1( "Alarm threshold error, RTU:0x%X", to_->du.zzk__WRD ); 
			RealAlarm( to_->du.zzk__WRD );
			break;

		case RTU_ENABLELOLO:
			_tmp.prefix = PF_IMM|IMM_BOOL;
			if( !PUCast(&_tmp, from_) ) return FALSE;
			if(    ( (_lpRtu->Status & RTUBIT_ENABLELOLO)? TRUE: FALSE )
				== ( _tmp.du.zzk__BOOL?TRUE:FALSE ) )
				return TRUE;	//same value

			_lpRtu->Status ^= (RTUBIT_ENABLELOLO);		//tag this bit
			_RTU_SET_DIRTY( STATUS )
			
			if( !RealCheckAlarmThreshold(to_->du.zzk__WRD) )
				LOGERR1( "Alarm threshold error, RTU:0x%X", to_->du.zzk__WRD ); 
			RealAlarm( to_->du.zzk__WRD );
			break;

		case RTU_ACK:
		{
			//this function will do everthing
			return RealAckAlarm( to_->du.zzk__WRD );
		}

		case RTU_VALUE_PERCENT:
		{
			REAL	_val;

			_tmp.prefix = PF_IMM|IMM_REAL;
			if( !PUCast(&_tmp, from_) ) return FALSE;
			_val = _tmp.du.zzk__REAL * ( _lpRtu->Max - _lpRtu->Min ) + _lpRtu->Min;
			if( _lpRtu->Value == _val )
				return TRUE;	//same value
			_lpRtu->Value = _val;
			RealAlarm( to_->du.zzk__WRD );

			_RTU_SET_DIRTY( VALUE )
			break;
		}

		default:
			//read-only elements
			return FALSE;
	}

	return TRUE;
}

//...................................................................
//...................................................................
_DLLEXP BOOL _DLLFUNC PURealDirty( LPPU lppu_ )
{
	int 	_i;
	WORD	_ele;

	CORE_ASSERT( ((lppu_->prefix) & TBX_MASK) == TBX_RT );
	if( lppu_->du.zzk__WRD	>= gRTInfo.wAdding )
	{
		CORE_ASSERT( FALSE );
		return FALSE;
	}

	_ele = lppu_->prefix & XT_MASK;

	for(_i=0; _i<_NUM_NAMESET; _i++)
		if( _ele == ssNameSet[_i].element )
		{
			return (gRTInfo.lpBase + lppu_->du.zzk__WRD)->Dirty & ssNameSet[_i].dirty;
		}

	CORE_ASSERT( FALSE );	//should NOT go here
	return FALSE;	//no found
}
