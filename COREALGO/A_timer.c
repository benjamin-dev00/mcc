//===================================================================
//
//Zheng zhikai	95-4-22
//===================================================================
#include "prehead.h"

#include "core.h"
#include "cmodu.h"
#include "calgo.h"
#include "corerw.h"
#include "cpu.h"
#include "crun.h"

#include "algo.h"
#include "algodef.h"
#include <dos.h>
#include <time.h>

#include "AlgoRes.h"

HINSTANCE	ghInst;

enum _timer_param{
	TIMER_OUT = 0,
	TIMER_PERIOD,
	TIMER_TRIGGER,
	TIMER_NUM_PARAM
};

#define TIMER_OUT_PDT		PDT_BOOL
#define TIMER_PERIOD_PDT	PDT_DWORD
#define TIMER_TRIGGER_PDT	PDT_WRD

//...................................................................
//...................................................................
_DLLEXP DWORD _DLLFUNC TimerParam(PIC wCmd_, WRD wMTU_, WORD wNo_, LONG lParam_)
{
	DWORD	_ret;
	
	static PDU _sPDU[] =
	{
		_ELE_PDU(TIMER_OUT,		"Out",		VSOF_BOOL, 		PU_NORMAL,	PRI_ALL ),
		_ELE_PDU(TIMER_PERIOD,	"Period",	VSOF_DWORD_MS, 	PU_NORMAL,	PRI_ALL ),
		_ELE_PDU(TIMER_TRIGGER,	"Trigger",	VSOF_WRDMTU, 	PU_NORMAL,	PRI_ALL|PDUF_NODEFAULT )
	};

	if(	wCmd_ == PIC_GETICON )
		return (DWORD)LoadIcon( ghInst, MAKEINTRESOURCE(IDI_TIMER1) );

	_ret = _DefAlgoParam( wCmd_,
						  wMTU_,
						  wNo_,
						  lParam_,
						  sizeof(_sPDU)/sizeof(PDU),
						  IDA_TIMER,
						  &(_sPDU[0]) );

	/*
	if( (wCmd_ == PIC_INITIALIZEPARAM) && (wNo_ == TIMER_PERIOD) )
	{
		LPPU	_lpPrm;

		_lpPrm = ModuGetParameter( wMTU_ );
		MFPUWRITE( _lpPrm, TIMER_PERIOD, DWORD, 60L*1000L );	//one minute
	}
	*/

	return _ret;
}

//...................................................................
//Description:
//	Algorithm:
//...................................................................
_DLLEXP ACRET _DLLFUNC TimerCalcu(WRD wMTU_)
{
	LPPU	_lpPrm;
	DWORD	_ticks, _st, _period;

	_lpPrm = ModuGetParameter( wMTU_ );

	MFPUREAD( _lpPrm, TIMER_PERIOD, DWORD, _period );
	_ticks = CoreGetTicks();
	_st = CoreGetStepTime();

	if( ldiv( _ticks*_st, _period ).rem == 0 )
	{
		WRD	_trigger;

		MFPUREAD( _lpPrm, TIMER_TRIGGER, WRD, _trigger );
		MFPUWRITE( _lpPrm, TIMER_OUT, BOOL, TRUE );

        //initiate a new modu thread
		if( _trigger )
			InitiateModuThread( _trigger );
	}
	else
	{
		MFPUWRITE( _lpPrm, TIMER_OUT, BOOL, FALSE );
	}

	return  ACRET_TRUE;
}


