//===================================================================
//
//Zheng zhikai	95-10-4
//===================================================================

#include "prehead.h"

#include "core.h"
#include "cmodu.h"
#include "cpu.h"
#include "calgo.h"
#include "cerror.h"

static WRD _near AlgoGetFreeRoom(void);

extern ATInfo		gATInfo;
extern DWORD		gdwLastError;
extern CoreInfo		gCoreInfo;

struct {
	WORD element;
	char szName[16];
} static ssNameSet[] =
{
	{AT_NAME,		"Name"},
	{AT_ID,			"ID"},
	{AT_CALCUPROC,	"Calcu Proc"},
	{AT_PARAMPROC,	"Param Proc"}
};

//...................................................................
//Description:
//	Registers an Algorithm in the Algo Table
//Entry:
//	ATU FAR* lp_
//		source algo-unit. The function copies the whole of lp_ to the
//		new unit.
//Return:
//		returns TRUE if the function succeeds. Otherwise, it is FALSE.
//...................................................................
_DLLEXP BOOL _DLLFUNC RegisterAlgorithm(LPATU lp_)
{
	WRD			_i;

	//if no free room
	if(gATInfo.wAdding >= gATInfo.wCount)
		return FALSE;

	//check whether a legal register
	for( _i=1; _i<gATInfo.wAdding; _i++ )
	{
		if( !strnicmp(gATInfo.lpBase[_i].szName, lp_->szName, LENGTH_NAME_ALGO) )
			return FALSE;	//the name is existed
		if( gATInfo.lpBase[_i].wID == lp_->wID )
			return FALSE;	//the id is existed
	}

	_i = AlgoGetFreeRoom();
	if( !_i )
	{
		//if no free room, function RealGetFreeRoom(...) set gdwLastError
		return FALSE;
	}
	memset( gATInfo.lpBase + _i, 0, sizeof(ATU) );
	memcpy( &((gATInfo.lpBase)[_i]), lp_, sizeof(ATU) );

	DispatchCoreMessage( TBX_AT );

	return TRUE;
}

//...................................................................
//...................................................................
_DLLEXP BOOL _DLLFUNC UnregisterAlgorithm( IDALGO wID_ )
{
	WRD		_i;

	//check whether a legal register
	for( _i=1; _i<gATInfo.wAdding; _i++ )
	{
		if( gATInfo.lpBase[_i].wID == wID_ )
		{
            //clear this in order to unregister
			memset( &((gATInfo.lpBase)[_i]), 0, sizeof(ATU) );
			DispatchCoreMessage( TBX_AT );
			return TRUE;	//find the algo
		}
	}

	return FALSE;
}

//...................................................................
//Description:
//...................................................................
static WRD _near AlgoGetFreeRoom()
{
	WRD		_i;

	CLEAR_LAST_ERROR;

	for(_i=1; _i<gATInfo.wAdding; _i++)
	{
		if( (gATInfo.lpBase + _i)->wID == 0 )
			break;
	}

	if( _i < gATInfo.wAdding )
		return _i;
	else	// _i == gATInfo.wAdding
	{
		if( gATInfo.wAdding < gATInfo.wCount )
		{
			gATInfo.wAdding ++;
			return _i;
		}
		else	//no free room
		{
			//if no free room, set last error
			SET_LAST_ERROR( ERR_ATU_No_Free_Room );
			return 0;
		}
	}
}


//...................................................................
//...................................................................
_DLLEXP BOOL _DLLFUNC IsAlgo(IDALGO id_)
{
	WRD	_i;

	for(_i=1; _i<gATInfo.wAdding; _i++)
	{
		if(gATInfo.lpBase[_i].wID == id_)
			return TRUE;
	}

	return FALSE;
}

//...................................................................
//...................................................................
_DLLEXP WORD _DLLFUNC AlgoGetNumParam(IDALGO id_)
{
	WRD	_i;

	for(_i=1; _i<gATInfo.wAdding; _i++)
		if(gATInfo.lpBase[_i].wID == id_)
			return (WORD)(*gATInfo.lpBase[_i].lpfnParam)(PIC_GETNUMPARAM, 0, 0, 0L);

	return 0;
}

//...................................................................
//...................................................................
_DLLEXP BOOL _DLLFUNC AlgoGetName(IDALGO id_, LPSTR szBuffer_, WORD wBufferSize_)
{
	WRD		_i;

	if(!szBuffer_)
		return FALSE;	//invalid string buffer

	for(_i=1; _i<gATInfo.wAdding; _i++)
	{
		if(gATInfo.lpBase[_i].wID == id_)
		{
			StringCopyn(szBuffer_, gATInfo.lpBase[_i].szName, wBufferSize_);
			return TRUE;
		}
	}

	*szBuffer_ = 0;
	return FALSE;	//the id of algorithm is not found
}

//...................................................................
//Description:
//...................................................................
_DLLEXP DWORD _DLLFUNC AlgoGetFlags( IDALGO id_ )
{
	WRD		_i;

	for(_i=1; _i<gATInfo.wAdding; _i++)
		if(gATInfo.lpBase[_i].wID == id_)
			return (gATInfo.lpBase)[_i].Flags;

	//failed
	CORE_ASSERT( FALSE );
	return NULL;
}

//...................................................................
//Description:
//  this function should use with care;
//...................................................................
_DLLEXP void _DLLFUNC AlgoSetFlags( IDALGO id_, DWORD dwFlags_ )
{
	WRD		_i;

	for(_i=1; _i<gATInfo.wAdding; _i++)
		if( gATInfo.lpBase[_i].wID == id_ )
		{
			(gATInfo.lpBase)[_i].Flags = dwFlags_;
			return;
		}

	//failed
	CORE_ASSERT( FALSE );
	return;
}

//...................................................................
//...................................................................
_DLLEXP BOOL _DLLFUNC AlgoGetParamName(IDALGO id_, WORD wNo_, LPGETPARAMNAME lpGPN_)
{
	WRD		_i;
	for(_i=1; _i<gATInfo.wAdding; _i++)
	{
		if(gATInfo.lpBase[_i].wID == id_)
		{
			return (BOOL)(*(gATInfo.lpBase[_i].lpfnParam))(PIC_GETPARAMNAME,
													 0,
													 wNo_,
													 (LONG)lpGPN_);
		}
	}

	return FALSE;	//the id of algorithm is not found
}

//...................................................................
//...................................................................
_DLLEXP BOOL _DLLFUNC AlgoGetParamOrder(IDALGO id_, LPGETPARAMORDER lpGPO_)
{
	WRD		_i;
	for(_i=1; _i<gATInfo.wAdding; _i++)
	{
		if(gATInfo.lpBase[_i].wID == id_)
		{
			return (BOOL)(*(gATInfo.lpBase[_i].lpfnParam))( PIC_GETPARAMORDER,
													  0,
													  0,
													  (LONG)lpGPO_);
		}
	}

	return FALSE;	//the id of algorithm is not found
}

//...................................................................
//...................................................................
_DLLEXP BOOL _DLLFUNC AlgoGetParamTypeName(IDALGO id_, WORD wNo_, LPGETPARAMTYPENAME lpGPTN_)
{
	WRD		_i;
	for(_i=1; _i<gATInfo.wAdding; _i++)
	{
		if(gATInfo.lpBase[_i].wID == id_)
		{
			return (BOOL)(*(gATInfo.lpBase[_i].lpfnParam))(PIC_GETPARAMTYPENAME,
													 0,
													 wNo_,
													 (LONG)lpGPTN_);
		}
	}

	return FALSE;	//the id of algorithm is not found
}

//...................................................................
//...................................................................
_DLLEXP WORD _DLLFUNC AlgoGetParamPrivilege(IDALGO id_, WORD wNo_)
{
	WRD		_i;

	for(_i=1; _i<gATInfo.wAdding; _i++)
	{
		if(gATInfo.lpBase[_i].wID == id_)
		{
			return (WORD) (*(gATInfo.lpBase[_i].lpfnParam))(PIC_GETPARAMPRIVILEGE, 0, wNo_, 0L);
		}
	}

	return 0;
}

//...................................................................
//...................................................................
_DLLEXP PDT _DLLFUNC AlgoGetParamType(IDALGO id_, WORD wNo_)
{
	WRD		_i;

	for(_i=1; _i<gATInfo.wAdding; _i++)
	{
		if(gATInfo.lpBase[_i].wID == id_)
		{
			return (BOOL)(*(gATInfo.lpBase[_i].lpfnParam))(PIC_GETPARAMTYPE, 0, wNo_, 0L);
		}
	}

	return FALSE;	//the id of algorithm is not found
}

//...................................................................
//...................................................................
_DLLEXP BOOL _DLLFUNC AlgoDecode(IDALGO id_, WRD wModu_, WORD wNo_, LPDECODE lpDecode_)
{
	WRD		_i;
	for(_i=1; _i<gATInfo.wAdding; _i++)
	{
		if(gATInfo.lpBase[_i].wID == id_)
		{
			return (BOOL)(*(gATInfo.lpBase[_i].lpfnParam))(PIC_DECODE,
													 wModu_,
													 wNo_,
													 (LONG)lpDecode_);
		}
	}

	return FALSE;	//the id of algorithm is not found
}

//...................................................................
//...................................................................
_DLLEXP BOOL _DLLFUNC AlgoEncode(IDALGO id_, WRD wModu_, WORD wNo_, LPENCODE lpDecode_)
{
	WRD		_i;
	for(_i=1; _i<gATInfo.wAdding; _i++)
	{
		if(gATInfo.lpBase[_i].wID == id_)
		{
			return (BOOL)(*(gATInfo.lpBase[_i].lpfnParam))(PIC_ENCODE,
													 wModu_,
													 wNo_,
													 (LONG)lpDecode_);
		}
	}

	return FALSE;	//the id of algorithm is not found
}

//...................................................................
//...................................................................
_DLLEXP DWORD _DLLFUNC AlgoGetValueString(IDALGO id_, WRD wModu_, WORD wNo_, LPGETVALUESTRING lpvs_)
{
	WRD		_i;
	for(_i=1; _i<gATInfo.wAdding; _i++)
	{
		if(gATInfo.lpBase[_i].wID == id_)
		{
			return (*(gATInfo.lpBase[_i].lpfnParam))(PIC_GETVALUESTRING,
													 wModu_,
													 wNo_,
													 (LONG)lpvs_);
		}
	}

	return FALSE;	//the id of algorithm is not found
}

//...................................................................
//...................................................................
_DLLEXP BOOL _DLLFUNC AlgoSetValueString(IDALGO id_, WRD wModu_, WORD wNo_, LPCSTR lpsz_)
{
	WRD		_i;
	for(_i=1; _i<gATInfo.wAdding; _i++)
	{
		if(gATInfo.lpBase[_i].wID == id_)
		{
			return (BOOL)(*(gATInfo.lpBase[_i].lpfnParam))(PIC_SETVALUESTRING,
													 wModu_,
													 wNo_,
													 (LONG)lpsz_);
		}
	}

	return FALSE;	//the id of algorithm is not found
}

//...................................................................
//...................................................................
_DLLEXP BOOL _DLLFUNC AlgoInitiate(IDALGO id_, WRD wModu_, LONG lStepInterval_)
{
	WRD		_i;
	for(_i=1; _i<gATInfo.wAdding; _i++)
	{
		if(gATInfo.lpBase[_i].wID == id_)
		{
			return (BOOL)(*(gATInfo.lpBase[_i].lpfnParam))(PIC_INITIATE,
													 wModu_,
													 0,
													 lStepInterval_);
		}
	}

	return FALSE;	//the id of algorithm is not found
}

//...................................................................
//...................................................................
_DLLEXP BOOL _DLLFUNC AlgoRun(IDALGO id_, WRD wModu_)
{
	WRD		_i;
	for(_i=1; _i<gATInfo.wAdding; _i++)
	{
		if(gATInfo.lpBase[_i].wID == id_)
			return (*(gATInfo.lpBase[_i].lpfnCalcu))(wModu_);
	}

	return FALSE;	//the id of algorithm is not found
}

//...................................................................
//...................................................................
_DLLEXP BOOL _DLLFUNC AlgoStop(IDALGO id_, WRD wModu_)
{
	WRD		_i;
	for(_i=1; _i<gATInfo.wAdding; _i++)
	{
		if(gATInfo.lpBase[_i].wID == id_)
		{
			return (BOOL)(*(gATInfo.lpBase[_i].lpfnParam))(PIC_STOP,
													 wModu_,
													 0,
													 0L);
		}
	}

	return FALSE;	//the id of algorithm is not found
}

//...................................................................
//...................................................................
_DLLEXP BOOL _DLLFUNC AlgoPICCreate(IDALGO id_, WRD wModu_, LONG lData_)
{
	WRD		_i;

	for(_i=1; _i<gATInfo.wAdding; _i++)
	{
		if(gATInfo.lpBase[_i].wID == id_)
		{
			return (BOOL)(*(gATInfo.lpBase[_i].lpfnParam))(PIC_CREATE,
													 wModu_,
													 0,
													 lData_);
		}
	}

	return FALSE;	//the id of algorithm is not found
}

//...................................................................
//...................................................................
_DLLEXP BOOL _DLLFUNC AlgoPICPaste(IDALGO id_, WRD wModu_)
{
	WRD		_i;

	for(_i=1; _i<gATInfo.wAdding; _i++)
	{
		if(gATInfo.lpBase[_i].wID == id_)
		{
			return (BOOL)(*(gATInfo.lpBase[_i].lpfnParam))(PIC_PASTE,
													 wModu_,
													 0, 0L);
		}
	}

	return FALSE;	//the id of algorithm is not found
}

//...................................................................
//...................................................................
_DLLEXP BOOL _DLLFUNC AlgoPICDelete(IDALGO id_, WRD wModu_)
{
	WRD		_i;

	for(_i=1; _i<gATInfo.wAdding; _i++)
	{
		if(gATInfo.lpBase[_i].wID == id_)
		{
			return (BOOL)(*(gATInfo.lpBase[_i].lpfnParam))(PIC_DELETE,
													 wModu_,
													 0, 0L);
		}
	}

	return FALSE;	//the id of algorithm is not found
}

//...................................................................
//...................................................................
_DLLEXP DWORD _DLLFUNC AlgoCallParamProc(IDALGO id_, PIC wCmd_, WRD wrd_, WORD wNo_, LONG lParam_)
{
	WRD		_i;

	for(_i=1; _i<gATInfo.wAdding; _i++)
	{
		if(gATInfo.lpBase[_i].wID == id_)
		{
			return (*(gATInfo.lpBase[_i].lpfnParam))( wCmd_, wrd_, wNo_, lParam_ );

		}
	}

	return FALSE;	//the id of algorithm is not found
}

//...................................................................
//...................................................................
_DLLEXP LPSTR _DLLFUNC AlgoGetData(LPSTR lpsz_, int size_, WRD wrd_, WORD order_)
{
	LPATU 	_lpATU;

	if(wrd_ <= (gATInfo.wCount - 1))
		_lpATU = gATInfo.lpBase + wrd_;
	else
		return FALSE;

	switch(order_)
	{
		case AT_NAME:
			StringCopyn(lpsz_, _lpATU->szName, size_);
			break;

		case AT_ID:
			ltoa(_lpATU->wID, lpsz_, 16);
			break;

		case AT_CALCUPROC:
			sprintf(lpsz_, "%Fp", _lpATU->lpfnCalcu);
			break;

		case AT_PARAMPROC:
			sprintf(lpsz_, "%Fp", _lpATU->lpfnParam);
			break;

		default:
			CORE_ASSERT(0);
			return NULL;
	}

	return lpsz_;
}

//...................................................................
//...................................................................
_DLLEXP BOOL _DLLFUNC AlgoSetData(LPCSTR lpsz_, WRD wrd_, WORD order_)
{
	ATU			_temp;
	char		*_end;
	LPATU 		_lpATU;

	CORE_ASSERT(wrd_ <= (gATInfo.wCount - 1));
	_lpATU = gATInfo.lpBase + wrd_;

	memcpy(&_temp, _lpATU, sizeof(ATU) );

	switch(order_)
	{
		case AT_NAME:
			if( IS_CO_ACCESSPRIVILEGE() )
			{
				StringCopyn(_lpATU->szName, lpsz_, LENGTH_NAME_ALGO);
			}
			else
			{
				SET_LAST_ERROR( ERR_ReadOnly );
				return FALSE;
			}
			break;

		case AT_ID:
			if( IS_CO_ACCESSPRIVILEGE() )
			{
				_lpATU->wID = strtoul(lpsz_, &_end, 0);
				if(*_end)
				{
					_lpATU->wID = _temp.wID;
					return FALSE;
				}
			}
			else
			{
				SET_LAST_ERROR( ERR_ReadOnly );
				return FALSE;
			}
			break;

		case AT_CALCUPROC:
			if( IS_CO_ACCESSPRIVILEGE() )
				sscanf(lpsz_, "%Fp", &(_lpATU->lpfnCalcu));
			else
			{
				SET_LAST_ERROR( ERR_ReadOnly );
				return FALSE;
			}
			break;

		case AT_PARAMPROC:
			if( IS_CO_ACCESSPRIVILEGE() )
				sscanf(lpsz_, "%Fp", &(_lpATU->lpfnParam));
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
_DLLEXP BOOL _DLLFUNC AlgoGetElementName(WORD element_, LPSTR szName_, WORD wSize_)
{
	unsigned		_i;

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
_DLLEXP WORD _DLLFUNC AlgoGetElement(LPSTR szName_)
{
	unsigned		_i;

	if( !szName_ )
	{
		SET_LAST_ERROR( ERR_Invalid_Entries );
		return FALSE;
	}

	for(_i=0; _i<sizeof(ssNameSet)/sizeof(ssNameSet[0]); _i++)
	{
		if( !strnicmp(ssNameSet[_i].szName, szName_, 3) )
			return ssNameSet[_i].element;
	}

	return 0;
}

