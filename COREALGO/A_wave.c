//===================================================================
//
//Zheng zhikai	95-4-22
//===================================================================
#include "prehead.h"

#include "coreall.h"
#include <math.h>
#include "algo.h"
#include "algodef.h"
#include "AlgoRes.h"

HINSTANCE	ghInst;

enum _param_sine{
	SIN_SIN = 0,
	SIN_AMPLITUDE,
	SIN_PERIOD,
	SIN_ANGLE,
	SIN_BIAS,
	SIN_NUM_PARAM
};
#define SIN_SIN_PDT			PDT_REAL
#define SIN_AMPLITUDE_PDT	PDT_REAL
#define SIN_PERIOD_PDT		PDT_DWORD
#define SIN_ANGLE_PDT		PDT_REAL
#define SIN_BIAS_PDT		PDT_REAL

//...................................................................
//...................................................................
_DLLEXP DWORD _DLLFUNC SinParam(PIC wCmd_, WRD wMTU_, WORD wNo_, LONG lParam_)
{
	DWORD	_ret;
	
	static PDU _sPDU[] =
	{
		_ELE_PDU(SIN_SIN,			"Sin",		VSOF_REAL, 		PU_NORMAL,	PRI_ALL ),
		_ELE_PDU(SIN_AMPLITUDE,		"Amp",		VSOF_REAL, 		PU_NORMAL,	PRI_ALL ),
		_ELE_PDU(SIN_PERIOD,		"Period",	VSOF_DWORD_MS,	PU_NORMAL,	PRI_ALL ),
		_ELE_PDU(SIN_ANGLE,			"Angle",	VSOF_REAL, 		PU_NORMAL,	PRI_ALL ),
		_ELE_PDU(SIN_BIAS,			"Bias",		VSOF_REAL, 		PU_NORMAL,	PRI_ALL )
	};

	if(	wCmd_ == PIC_GETICON )
		return (DWORD)LoadIcon( ghInst, MAKEINTRESOURCE(IDI_SIN) );

	_ret = _DefAlgoParam( wCmd_,
						  wMTU_,
						  wNo_,
						  lParam_,
						  sizeof(_sPDU)/sizeof(PDU),
						  IDA_SIN,
						  &(_sPDU[0]) );

	return _ret;
}

//...................................................................
//Description:
//	Algorithm:
//...................................................................
_DLLEXP ACRET _DLLFUNC SinCalcu(WRD wMTU_)
{
	REAL	_A, _angle, _bias, _tmp;
	DWORD	_period, _ticks, _st;

	LPPU	_lpPrm = ModuGetParameter( wMTU_ );

	MFPUREAD( _lpPrm, SIN_AMPLITUDE, REAL, _A );
	MFPUREAD( _lpPrm, SIN_PERIOD, DWORD, _period );
	MFPUREAD( _lpPrm, SIN_ANGLE, REAL, _angle );
	MFPUREAD( _lpPrm, SIN_BIAS, REAL, _bias );

	_ticks = CoreGetTicks();
	_st = CoreGetStepTime();

	_tmp = (REAL)ldiv( _ticks*_st, _period ).rem;

	_tmp = ( _tmp/_period + _angle/360 ) *(2 * (REAL)M_PI);
	_tmp = _A * (REAL)sin(_tmp) + _bias;

	MFPUWRITE( _lpPrm, SIN_SIN, REAL, _tmp );
	return  ACRET_TRUE;
}


