//===================================================================
//
//Zheng zhikai	95-4-21
//===================================================================

#include "prehead.h"

#include "core.h"
#include "cface.h"
#include "cstamp.h"
#include "cerror.h"

static WRD _near StampGetFreeRoom(void);

extern STInfo		gSTInfo;
extern FTInfo		gFTInfo;
extern DWORD		gdwLastError;
extern CoreInfo		gCoreInfo;

struct {
	WORD element;
	char szName[16];
} static ssNameSet[] =
{
	{ST_NAME,		"Name"},
	{ST_ID,			"ID"},
	{ST_PAINTPROC,	"Paint Proc"},
	{ST_BUILDPROC,	"Build Proc"},
	{ST_PARAMPROC,	"Param Proc"}
};


//...................................................................
//Description:
//	Registers an stamp in Stamp Table
//Entry:
//	STU FAR* lp_
//		source stamp-unit. The function copies the whole of the struct
//		pointed by lp_ to the new unit.
//Return:
//		returns TRUE if the function succeeds. Otherwise, it is NULL.
//...................................................................
_DLLEXP BOOL _DLLFUNC RegisterStamp(STU FAR* lp_)
{
	WRD			_i;

	//if no free room
	if(gSTInfo.wAdding >= gSTInfo.wCount)
		return FALSE;

	//check whether a legal register
	if( (!lp_->lpfnParam) || (!lp_->lpfnPaint) || (!lp_->lpfnBuild) )
		return FALSE;

	for(_i=1; _i<gSTInfo.wAdding; _i++)
	{
		if(!strnicmp(gSTInfo.lpBase[_i].szName, lp_->szName, LENGTH_NAME_STAMP))
			return FALSE;	//the name is existed
		if(gSTInfo.lpBase[_i].wID == lp_->wID)
			return FALSE;	//the id is existed
	}

	_i = StampGetFreeRoom();
	if( !_i )
	{
		//if no free room, function StampGetFreeRoom(...) has set gdwLastError
		return FALSE;
	}
	memset( gSTInfo.lpBase + _i, 0, sizeof(STU) );
	memcpy( &((gSTInfo.lpBase)[_i]), lp_, sizeof(STU) );

	DispatchCoreMessage( TBX_ST );
	return TRUE;
}

//...................................................................
//...................................................................
_DLLEXP BOOL _DLLFUNC UnregisterStamp( IDSTAMP wID_ )
{
	WRD		_i;

	//check whether a legal register
	for( _i=1; _i<gSTInfo.wAdding; _i++ )
	{
		if( gSTInfo.lpBase[_i].wID == wID_ )
		{
			//clear this in order to unregister
			memset( &((gSTInfo.lpBase)[_i]), 0, sizeof(STU) );
			DispatchCoreMessage( TBX_ST );
			return TRUE;	//find the stamp
		}
	}

	return FALSE;
}

//...................................................................
//Description:
//...................................................................
_DLLEXP DWORD _DLLFUNC StampGetFlags( IDSTAMP id_ )
{
	WRD		_i;

	for(_i=1; _i<gSTInfo.wAdding; _i++)
		if(gSTInfo.lpBase[_i].wID == id_)
			return (gSTInfo.lpBase)[_i].Flags;

	//failed
	CORE_ASSERT( FALSE );
	return NULL;
}

//...................................................................
//Description:
//  this function should use with care;
//...................................................................
_DLLEXP void _DLLFUNC StampSetFlags( IDSTAMP id_, DWORD dwFlags_ )
{
	WRD		_i;

	for(_i=1; _i<gSTInfo.wAdding; _i++)
		if( gSTInfo.lpBase[_i].wID == id_ )
		{
			(gSTInfo.lpBase)[_i].Flags = dwFlags_;
			return;
		}

	//failed
	CORE_ASSERT( FALSE );
	return;
}


//...................................................................
//Description:
//...................................................................
static WRD _near StampGetFreeRoom()
{
	WRD		_i;

	CLEAR_LAST_ERROR;

	for(_i=1; _i<gSTInfo.wAdding; _i++)
	{
		if( (gSTInfo.lpBase + _i)->wID == 0 )
			break;
	}

	if( _i < gSTInfo.wAdding )
		return _i;
	else	// _i == gSTInfo.wAdding
	{
		if( gSTInfo.wAdding < gSTInfo.wCount )
		{
			gSTInfo.wAdding ++;
			return _i;
		}
		else	//no free room
		{
			//if no free room, set last error
			SET_LAST_ERROR( ERR_STU_No_Free_Room );
			return 0;
		}
	}
}

//...................................................................
//...................................................................
_DLLEXP BOOL _DLLFUNC IsStamp( IDSTAMP id_ )
{
	WRD	_i;

	for(_i=1; _i<gSTInfo.wAdding; _i++)
	{
		if(gSTInfo.lpBase[_i].wID == id_)
		{
			return TRUE;
		}
	}

	return FALSE;
}

//...................................................................
//...................................................................
_DLLEXP WORD _DLLFUNC StampGetNumParam(IDSTAMP id_)
{
	WRD	_i;

	for(_i=1; _i<gSTInfo.wAdding; _i++)
	{
		if(gSTInfo.lpBase[_i].wID == id_)
			return (WORD)(*(gSTInfo.lpBase[_i].lpfnParam))(PIC_GETNUMPARAM, 0, 0, 0L);
	}

	return FALSE;
}

//...................................................................
//Description:
//...................................................................
_DLLEXP SBTPROC _DLLFUNC StampGetBuildProc(IDSTAMP id_)
{
	WRD	_i;

	for(_i=1; _i<gSTInfo.wAdding; _i++)
	{
		if(gSTInfo.lpBase[_i].wID == id_)
			return gSTInfo.lpBase[_i].lpfnBuild;
	}

	return NULL;
}

//...................................................................
//Description:
//...................................................................
_DLLEXP PARAMPROC _DLLFUNC StampGetParamProc(IDSTAMP id_)
{
	WRD	_i;

	for(_i=1; _i<gSTInfo.wAdding; _i++)
	{
		if(gSTInfo.lpBase[_i].wID == id_)
			return gSTInfo.lpBase[_i].lpfnParam;
	}

	return NULL;
}

//...................................................................
//...................................................................
_DLLEXP LPSTR _DLLFUNC StampGetData(LPSTR lpsz_, int size_, WRD wrd_, WORD order_)
{
	LPSTU 	_lpSTU;

	CORE_ASSERT(wrd_ <= (gSTInfo.wCount - 1));
	_lpSTU = gSTInfo.lpBase + wrd_;

	switch(order_)
	{
		case ST_NAME:
			StringCopyn(lpsz_, _lpSTU->szName, size_);
			break;

		case ST_ID:
			ltoa(_lpSTU->wID, lpsz_, 16);
			break;

		case ST_PAINTPROC:
			sprintf(lpsz_, "%Fp", _lpSTU->lpfnPaint);
			break;

		case ST_PARAMPROC:
			sprintf(lpsz_, "%Fp", _lpSTU->lpfnParam);
			break;

		case ST_BUILDPROC:
			sprintf(lpsz_, "%Fp", _lpSTU->lpfnBuild);
			break;

		default:
			CORE_ASSERT(0);
			return NULL;
	}

	return lpsz_;
}

//...................................................................
//...................................................................
_DLLEXP BOOL _DLLFUNC StampSetData(LPCSTR lpsz_, WRD wrd_, WORD order_)
{
	STU			_temp;
	char		*_end;
	LPSTU 		_lpSTU;

	CORE_ASSERT(wrd_ <= (gSTInfo.wCount - 1));
	_lpSTU = gSTInfo.lpBase + wrd_;

	memcpy(&_temp, _lpSTU, sizeof(STU) );

	switch(order_)
	{
		case ST_NAME:
			if( IS_CO_ACCESSPRIVILEGE() )
			{
				StringCopyn(_lpSTU->szName, lpsz_, LENGTH_NAME_STAMP);
			}
			else
			{
				SET_LAST_ERROR( ERR_ReadOnly );
				return FALSE;
			}
			break;

		case ST_ID:
			if( IS_CO_ACCESSPRIVILEGE() )
			{
				_lpSTU->wID = strtoul(lpsz_, &_end, 0);
				if(*_end)
				{
					_lpSTU->wID = _temp.wID;
					return FALSE;
				}
			}
			else
			{
				SET_LAST_ERROR( ERR_ReadOnly );
				return FALSE;
			}
			break;

		case ST_PAINTPROC:
			if( IS_CO_ACCESSPRIVILEGE() )
			{
				sscanf(lpsz_, "%Fp", &(_lpSTU->lpfnPaint));
			}
			else
			{
				SET_LAST_ERROR( ERR_ReadOnly );
				return FALSE;
			}
			break;

		case ST_PARAMPROC:
			if( IS_CO_ACCESSPRIVILEGE() )
			{
				sscanf(lpsz_, "%Fp", &(_lpSTU->lpfnParam));
			}
			else
			{
				SET_LAST_ERROR( ERR_ReadOnly );
				return FALSE;
			}
			break;

		case ST_BUILDPROC:
			if( IS_CO_ACCESSPRIVILEGE() )
			{
				sscanf(lpsz_, "%Fp", &(_lpSTU->lpfnBuild));
			}
			else
			{
				SET_LAST_ERROR( ERR_ReadOnly );
				return FALSE;
			}
			break;

		default:
			CORE_ASSERT(0);
			return FALSE;
	}

	return TRUE;
}

//...................................................................
//...................................................................
_DLLEXP BOOL _DLLFUNC StampGetElementName(WORD element_, LPSTR szName_, WORD wSize_)
{
	unsigned	_i;

	CLEAR_LAST_ERROR;

	if( !szName_ )
	{
		SET_LAST_ERROR( ERR_Invalid_Entries );
		return FALSE;
	}

	for(_i=0; _i<sizeof(ssNameSet)/sizeof(ssNameSet[0]); _i++)
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
//	Retrieves ID of the element by its ASCII name
//...................................................................
_DLLEXP WORD _DLLFUNC StampGetElement(LPSTR szName_)
{
	unsigned	_i;

	if( !szName_ )
	{
		SET_LAST_ERROR( ERR_Invalid_Entries );
		return FALSE;
	}

	for(_i=0; _i<sizeof(ssNameSet)/sizeof(ssNameSet[0]); _i++)
	{
		if( !strnicmp(ssNameSet[_i].szName, szName_, 4) )
			return ssNameSet[_i].element;
	}

	return 0;
}

//...................................................................
//Description:
//...................................................................
_DLLEXP SRTPROC _DLLFUNC StampGetPaintProc(IDSTAMP id_)
{
	WRD	_i;

	for(_i=1; _i<gSTInfo.wAdding; _i++)
	{
		if(gSTInfo.lpBase[_i].wID == id_)
			return gSTInfo.lpBase[_i].lpfnPaint;
	}

	return NULL;
}


//...................................................................
//...................................................................
_DLLEXP BOOL _DLLFUNC StampGetName(IDSTAMP id_, LPSTR szBuffer_, WORD wBufferSize_)
{
	WRD		_i;

	CORE_ASSERT(szBuffer_);
	for(_i=1; _i<gSTInfo.wAdding; _i++)
	{
		if(gSTInfo.lpBase[_i].wID == id_)
		{
			if(szBuffer_)
				StringCopyn(szBuffer_, gSTInfo.lpBase[_i].szName, wBufferSize_);
			return TRUE;
		}
	}

	//no found Stamp with the ID:id_
	if(szBuffer_) szBuffer_[0] = 0;		//set to null string.
	return FALSE;
}

//...................................................................
//...................................................................
_DLLEXP BOOL _DLLFUNC StampGetParamName(IDSTAMP id_, WORD wNo_, LPGETPARAMNAME lpGPN_)
{
	WRD		_i;
	for(_i=1; _i<gSTInfo.wAdding; _i++)
	{
		if(gSTInfo.lpBase[_i].wID == id_)
		{
			return (BOOL)( *(gSTInfo.lpBase[_i].lpfnParam) )(PIC_GETPARAMNAME,
													   0,
													   wNo_,
													   (LONG)lpGPN_);
		}
	}


	return FALSE;	//the id of algorithm is not found
}

//...................................................................
//...................................................................
_DLLEXP BOOL _DLLFUNC StampGetParamOrder(IDSTAMP id_, LPGETPARAMORDER lpGPO_)
{
	WRD		_i;
	for(_i=1; _i<gSTInfo.wAdding; _i++)
	{
		if(gSTInfo.lpBase[_i].wID == id_)
		{
			return (BOOL)(*(gSTInfo.lpBase[_i].lpfnParam))( PIC_GETPARAMORDER,
													  0,
													  0,
													  (LONG)lpGPO_);
		}
	}

	return FALSE;	//the id of algorithm is not found
}

//...................................................................
//...................................................................
_DLLEXP WORD _DLLFUNC StampGetParamPrivilege(IDSTAMP id_, WORD wNo_)
{
	WRD		_i;

	for(_i=1; _i<gSTInfo.wAdding; _i++)
	{
		if(gSTInfo.lpBase[_i].wID == id_)
		{
			return (WORD)(*(gSTInfo.lpBase[_i].lpfnParam))(PIC_GETPARAMPRIVILEGE, 0, wNo_, 0L);
		}
	}

	return 0;	//the id of stamp is not found
}

//...................................................................
//...................................................................
_DLLEXP PDT _DLLFUNC StampGetParamType(IDSTAMP id_, WORD wNo_)
{
	WRD		_i;

	for(_i=1; _i<gSTInfo.wAdding; _i++)
	{
		if(gSTInfo.lpBase[_i].wID == id_)
		{
			return (PDT)(*(gSTInfo.lpBase[_i].lpfnParam))(PIC_GETPARAMTYPE, 0, wNo_, 0L);
		}
	}

	return FALSE;	//the id of stamp is not found
}

//...................................................................
//...................................................................
_DLLEXP BOOL _DLLFUNC StampGetParamTypeName(IDSTAMP id_, WORD wNo_, LPGETPARAMTYPENAME lpGPTN_)
{
	WRD		_i;
	for(_i=1; _i<gSTInfo.wAdding; _i++)
	{
		if(gSTInfo.lpBase[_i].wID == id_)
		{
			return (BOOL)(*(gSTInfo.lpBase[_i].lpfnParam))(PIC_GETPARAMTYPENAME,
													 0,
													 wNo_,
													 (LONG)lpGPTN_);
		}
	}

	return FALSE;	//the id of algorithm is not found
}

//...................................................................
//...................................................................
_DLLEXP BOOL _DLLFUNC StampEncode(IDSTAMP id_, WRD wFace_, WORD wNo_, LPENCODE lpDecode_)
{
	WRD		_i;
	for(_i=1; _i<gSTInfo.wAdding; _i++)
	{
		if(gSTInfo.lpBase[_i].wID == id_)
		{
			return (BOOL)(*(gSTInfo.lpBase[_i].lpfnParam))(PIC_ENCODE,
													 wFace_,
													 wNo_,
													 (LONG)lpDecode_);
		}
	}

	return FALSE;	//the id of algorithm is not found
}

//...................................................................
//...................................................................
_DLLEXP BOOL _DLLFUNC StampDecode(IDSTAMP id_, WRD wFace_, WORD wNo_, LPDECODE lpDecode_)
{
	WRD		_i;
	for(_i=1; _i<gSTInfo.wAdding; _i++)
	{
		if(gSTInfo.lpBase[_i].wID == id_)
		{
			return (BOOL)(*(gSTInfo.lpBase[_i].lpfnParam))(PIC_DECODE,
													 wFace_,
													 wNo_,
													 (LONG)lpDecode_);
		}
	}

	return FALSE;	//the id of algorithm is not found
}

//...................................................................
//...................................................................
_DLLEXP BOOL _DLLFUNC StampGetValueString(IDSTAMP id_, WRD wFace_, WORD wNo_, LPGETVALUESTRING lpvs_)
{
	WRD		_i;
	for(_i=1; _i<gSTInfo.wAdding; _i++)
	{
		if(gSTInfo.lpBase[_i].wID == id_)
		{
			return (BOOL)(*(gSTInfo.lpBase[_i].lpfnParam))(PIC_GETVALUESTRING,
													 wFace_,
													 wNo_,
													 (LONG)lpvs_);
		}
	}

	return FALSE;	//the id of algorithm is not found
}

//...................................................................
//...................................................................
_DLLEXP BOOL _DLLFUNC StampSetValueString(IDSTAMP id_, WRD wFace_, WORD wNo_, LPCSTR lpsz_)
{
	WRD		_i;
	for(_i=1; _i<gSTInfo.wAdding; _i++)
	{
		if(gSTInfo.lpBase[_i].wID == id_)
		{
			return (BOOL)(*(gSTInfo.lpBase[_i].lpfnParam))(PIC_SETVALUESTRING,
													 wFace_,
													 wNo_,
													 (LONG)lpsz_);
		}
	}

	return FALSE;	//the id of algorithm is not found
}

//...................................................................
//...................................................................
_DLLEXP BOOL _DLLFUNC StampInitiate(IDSTAMP id_, WRD wFace_, LONG lSampleInterval_)
{
	WRD		_i;
	for(_i=1; _i<gSTInfo.wAdding; _i++)
	{
		if(gSTInfo.lpBase[_i].wID == id_)
		{
			return (BOOL)(*(gSTInfo.lpBase[_i].lpfnParam))(PIC_INITIATE,
													 wFace_,
													 0,
													 lSampleInterval_);
		}
	}

	return FALSE;	//the id of stamp is not found
}

//...................................................................
//...................................................................
_DLLEXP BOOL _DLLFUNC StampStop(IDSTAMP id_, WRD wFace_)
{
	WRD		_i;
	for(_i=1; _i<gSTInfo.wAdding; _i++)
	{
		if(gSTInfo.lpBase[_i].wID == id_)
		{
			return (BOOL)(*(gSTInfo.lpBase[_i].lpfnParam))(PIC_STOP,
													 wFace_,
													 0,
													 0L);
		}
	}

	return FALSE;	//the id of stamp is not found
}

//...................................................................
//...................................................................
_DLLEXP BOOL _DLLFUNC StampPICCreate(IDSTAMP id_, WRD wFace_, LONG lData_)
{
	WRD		_i;
	for(_i=1; _i<gSTInfo.wAdding; _i++)
	{
		if(gSTInfo.lpBase[_i].wID == id_)
		{
			return (BOOL)(*(gSTInfo.lpBase[_i].lpfnParam))(PIC_CREATE,
													 wFace_,
													 0,
													 lData_);
		}
	}

	return FALSE;	//the id of stamp is not found
}

//...................................................................
//...................................................................
_DLLEXP BOOL _DLLFUNC StampPICPaste(IDSTAMP id_, WRD wFace_)
{
	WRD		_i;
	for(_i=1; _i<gSTInfo.wAdding; _i++)
	{
		if(gSTInfo.lpBase[_i].wID == id_)
		{
			return (BOOL)(*(gSTInfo.lpBase[_i].lpfnParam))(PIC_PASTE,
													 wFace_,
													 0,
													 0L);
		}
	}

	return FALSE;	//the id of stamp is not found
}

//...................................................................
//...................................................................
_DLLEXP BOOL _DLLFUNC StampPICDelete(IDSTAMP id_, WRD wFace_)
{
	WRD		_i;
	for(_i=1; _i<gSTInfo.wAdding; _i++)
	{
		if(gSTInfo.lpBase[_i].wID == id_)
		{
			return (BOOL)(*(gSTInfo.lpBase[_i].lpfnParam))(PIC_DELETE,
													 wFace_,
													 0,
													 0L);
		}
	}

	return FALSE;	//the id of stamp is not found
}

//...................................................................
//Description:
//...................................................................
_DLLEXP DWORD _DLLFUNC StampCallBuildProc(SCMD wCmd_, WRD wFTU_, HWND hwnd_, WORD wParam_, LONG lParam_)
{
	SBTPROC		_lpProc;

	if((gFTInfo.lpBase)[wFTU_].Status & FTUBIT_VALID)
	{
		_lpProc = StampGetBuildProc((gFTInfo.lpBase)[wFTU_].Stamp);
		CORE_ASSERT(_lpProc);

		return (*_lpProc)(wCmd_, wFTU_, hwnd_, wParam_, lParam_);
	}

	//the FTU is invalid
	CORE_ASSERT(0);
	return 0;
}

//...................................................................
//Description:
//...................................................................
_DLLEXP DWORD _DLLFUNC StampCallParamProc(IDSTAMP id_, PIC wCmd_, WRD wrd_, WORD wNo_, LONG lParam_)
{
	WRD		_i;

	for(_i=1; _i<gSTInfo.wAdding; _i++)
	{
		if(gSTInfo.lpBase[_i].wID == id_)
		{
			return (*(gSTInfo.lpBase[_i].lpfnParam))( wCmd_, wrd_, wNo_, lParam_ );

		}
	}

	return FALSE;	//the id of stamp is not found
}

//...................................................................
//Description:
//...................................................................
_DLLEXP void _DLLFUNC SetMsgCode( LPFIA lpFia_ )
{
	UINT	_msg = lpFia_->msg;

	lpFia_->MsgCode = 0;
	if( _msg >= WM_MOUSEFIRST && _msg <= WM_MOUSELAST )
	{
		lpFia_->MsgCode |= MSG_MOUSE;

		if( _msg == WM_LBUTTONDOWN || _msg == WM_LBUTTONDBLCLK )
			lpFia_->MsgCode |= MSG_TRIGGER;
	}
	else if( _msg == WM_KEYDOWN )
	{
		switch( (UINT)(lpFia_->wParam) )
		{
			case VK_SPACE:
			case VK_RETURN:
				lpFia_->MsgCode |= MSG_TRIGGER;
				break;

			case VK_PRIOR:
			case VK_NEXT:
			case VK_END:
			case VK_HOME:
			case VK_LEFT:
			case VK_UP:
			case VK_RIGHT:
			case VK_DOWN:
				lpFia_->MsgCode |= MSG_ARROW;
				break;

			case VK_TAB:
				lpFia_->MsgCode |= MSG_TAB;
				break;

			default:
				lpFia_->MsgCode |= MSG_OTHERSKEYS;
				break;
		}
	}

	return;
}
