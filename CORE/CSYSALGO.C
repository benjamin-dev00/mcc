//===================================================================
//csysalgo.c
//	System Algorithms
//Zheng zhikai	95-2-6
//===================================================================

#include "prehead.h"

#include "core.h"
#include "cmodu.h"
#include "calgo.h"
#include "corerw.h"
#include "crun.h"

#include "csysalgo.h"
#include "CoreRes.h"

extern HINSTANCE	ghInst;

#define DEF_DEF( _name_ )																\
_DLLEXP	DWORD _ZZKDECL Algo##_name_##Param(PIC wCmd_, WRD wFTU_, WORD wNo_, LONG lParam_);	\
_DLLEXP	ACRET _ZZKDECL Algo##_name_(WRD wMTU_);

extern ATInfo		gATInfo;

DEF_DEF(Null)
DEF_DEF(Root)
DEF_DEF(Start)
DEF_DEF(End)
DEF_DEF(Loop)
DEF_DEF(Break)
DEF_DEF(Call)
DEF_DEF(Return)
DEF_DEF(Execute)

//DEF_DEF()

static ATU _sATUs[] = {
	{"Null", 		IDA_NULL, 		(void *)AlgoNull, 		(void *)AlgoNullParam},
	{"Root", 		IDA_ROOT, 		(void *)AlgoRoot, 		(void *)AlgoRootParam},

	{"Start",		IDA_START,		(void *)AlgoStart,		(void *)AlgoStartParam},
	{"End",			IDA_END,		(void *)AlgoEnd,		(void *)AlgoEndParam},
	{"Loop",		IDA_LOOP,		(void *)AlgoLoop,		(void *)AlgoLoopParam},
	{"Break",		IDA_BREAK,		(void *)AlgoBreak,		(void *)AlgoBreakParam},
	{"Call",		IDA_CALL,		(void *)AlgoCall,		(void *)AlgoCallParam},
	{"Return",		IDA_RETURN,		(void *)AlgoReturn,		(void *)AlgoReturnParam},
	{"Execute",		IDA_EXECUTE,	(void *)AlgoExecute,	(void *)AlgoExecuteParam},

	//unimplement yet
	//{"Goto",		IDA_GOTO,		AlgoGoto,		AlgoGotoParam},
};

//...................................................................
//...................................................................
BOOL RegisterSystemAlgos()
{
	int		_wrd;

	for(_wrd=0; _wrd < sizeof(_sATUs)/sizeof(ATU); _wrd++)
		if(!RegisterAlgorithm( &_sATUs[_wrd]) )
			return FALSE;

	//set the "Root" as a hiden algorihm
	AlgoSetFlags( IDA_ROOT, AlgoGetFlags( IDA_ROOT ) | ATF_HIDEN );
	return TRUE;
}

BOOL AccessSystemAlgoDefaultParameter( LPCSTR lpszDefName_, BOOL bSave_ )
{
	BOOL	_ret = TRUE;	
	int		_wrd;
	char	_szBuffer[_MAX_PATH + LENGTH_NAME_ALGO + 2];

	for(_wrd=0; _wrd < sizeof(_sATUs)/sizeof(ATU); _wrd++)
	{
		strcpy( _szBuffer, lpszDefName_ );
		strcpy( _szBuffer + strlen(_szBuffer) + 1, _sATUs[_wrd].szName );
		if( !(*_sATUs[_wrd].lpfnParam)( bSave_? PIC_SAVEDEFAULT: PIC_LOADDEFAULT, 
										0, 0, (LPARAM)_szBuffer ) )
		{
			LOGWARN2( "COREBASE.DLL %s defParam failed: %s", bSave_?"Save": "Load", _sATUs[_wrd].szName );
			_ret = FALSE;
		}
	}

	return _ret;
}

// Algorithm: NULL
//...................................................................
//...................................................................
#pragma argsused
_DLLEXP DWORD _DLLFUNC AlgoNullParam(PIC wCmd_, WRD wMTU_, WORD wNo_, LONG lParam_)
{
	CORE_ASSERT(wNo_ == 0);
	switch( wCmd_ )
	{
		case PIC_GETICON:
			return LoadIcon( ghInst, MAKEINTRESOURCE(IDI_NULL) );

		default:
			return _DefAlgoParam(wCmd_,
								  wMTU_,
								  wNo_,
								  lParam_,
								  0,		//no param
								  IDA_NULL,
								  NULL );
	}
}

//...................................................................
//Description:
//		does nothing
//...................................................................
_DLLEXP ACRET _DLLFUNC AlgoNull(WRD wMTU_)
{
	CORE_ASSERT( ModuGetAlgo(wMTU_) == IDA_NULL );

	return TRUE;
}

///Algorithm: ROOT
//...................................................................
//...................................................................
#pragma argsused
_DLLEXP DWORD _DLLFUNC AlgoRootParam(PIC wCmd_, WRD wMTU_, WORD wNo_, LONG lParam_)
{
	CORE_ASSERT(wNo_ == 0);

	switch( wCmd_ )
	{
		case PIC_PASTE:
			CORE_ASSERT(FALSE);	//never be here
			return FALSE;

		case PIC_CANCOPY:
			return FALSE;
		
		case PIC_GETICON:
			return LoadIcon( ghInst, MAKEINTRESOURCE(IDI_ROOT) );

		default:
			return _DefAlgoParam(wCmd_,
								  wMTU_,
								  wNo_,
								  lParam_,
								  0,		//no param
								  IDA_ROOT,
								  NULL );
	}
}

//...................................................................
//Description:
//		does nothing
//...................................................................
_DLLEXP ACRET _DLLFUNC AlgoRoot(WRD wMTU_)
{
	CORE_ASSERT( ModuGetAlgo(wMTU_) == IDA_ROOT );

	return TRUE;
}

// Algorithm: Start
//...................................................................
//...................................................................
#pragma argsused
_DLLEXP DWORD _DLLFUNC AlgoStartParam(PIC wCmd_, WRD wMTU_, WORD wNo_, LONG lParam_)
{
	CORE_ASSERT(wNo_ == 0);

	switch( wCmd_ )
	{
		case PIC_GETICON:
			return LoadIcon( ghInst, MAKEINTRESOURCE(IDI_START) );

		default:
			return _DefAlgoParam(wCmd_,
								  wMTU_,
								  wNo_,
								  lParam_,
								  0,		//no param
								  IDA_START,
								  NULL );
	}

}

//...................................................................
//Description:
//		does nothing
//...................................................................
_DLLEXP ACRET _DLLFUNC AlgoStart(WRD wMTU_)
{
	CORE_ASSERT( ModuGetAlgo(wMTU_) == IDA_START );

	return TRUE;
}

// Algorithm: End
//...................................................................
//...................................................................
#pragma argsused
_DLLEXP DWORD _DLLFUNC AlgoEndParam(PIC wCmd_, WRD wMTU_, WORD wNo_, LONG lParam_)
{
	CORE_ASSERT(wNo_ == 0);
	switch( wCmd_ )
	{
		case PIC_GETICON:
			return LoadIcon( ghInst, MAKEINTRESOURCE(IDI_END) );

		default:
			return _DefAlgoParam(wCmd_,
								  wMTU_,
								  wNo_,
								  lParam_,
								  0,		//no param
								  IDA_END,
								  NULL );
	}
}

//...................................................................
//Description:
//		does nothing
//...................................................................
_DLLEXP ACRET _DLLFUNC AlgoEnd(WRD wMTU_)
{
	CORE_ASSERT( ModuGetAlgo(wMTU_) == IDA_END );

    //terminate the current modu thread
	return ACRET_END;
}

// Algorithm: Loop
#define LOOP_CONDITION			0
#define LOOP_INTERVAL			1
#define LOOP_IV_COUNTER			2
#define LOOP_CONDITION_PDT		PDT_BOOL
#define LOOP_INTERVAL_PDT		PDT_DWORD
#define LOOP_IV_COUNTER_PDT		PDT_DWORD

//...................................................................
//...................................................................
#pragma argsused
_DLLEXP DWORD _DLLFUNC AlgoLoopParam(PIC wCmd_, WRD wMTU_, WORD wNo_, LONG lParam_)
{
	DWORD	_ret;
	static PDU _sPDU[] =
	{
		_ELE_PDU(LOOP_CONDITION,	"Condition",	VSOF_BOOL,	PU_NORMAL,	PRI_ALL ),
		_ELE_PDU(LOOP_INTERVAL,		"Interval",		VSOF_DWORD, PU_NORMAL,	PRI_ALL ),
		_ELE_PDU(LOOP_IV_COUNTER,	"Counter",		VSOF_DWORD, PU_NIV,		PRI_DEV|PDUF_NODEFAULT )
	};

	switch( wCmd_ )
	{
		case PIC_GETICON:
			return LoadIcon( ghInst, MAKEINTRESOURCE(IDI_LOOP) );

		default:
			_ret = _DefAlgoParam( wCmd_,
								  wMTU_,
								  wNo_,
								  lParam_,
								  sizeof(_sPDU)/sizeof(PDU),
								  IDA_LOOP,
								  &(_sPDU[0]) );
	}

	//let _DefAlgoParam(...) process first
	if( wCmd_ == PIC_CREATE )
	{
		STATUS 	_status;

		//all LOOP node should set MTU_SKIPCHILD
		_status = ModuGetStatus( wMTU_ );
		_status |= MTUBIT_SKIPCHILD;
		ModuSetStatus( wMTU_, _status );
	}
	else if( wCmd_ == PIC_INITIALIZEPARAM )
	{
		LPPU	_lpPrm;

		_lpPrm = ModuGetParameter( wMTU_ );
		MFPUWRITE( _lpPrm, LOOP_CONDITION, BOOL, TRUE );
	}

	return _ret;
}

//...................................................................
//Description:
//		does nothing
//...................................................................
_DLLEXP ACRET _DLLFUNC AlgoLoop(WRD wMTU_)
{
	LPPU	_lpPrm;
	BOOL	_bCondition;
	DWORD	_dwCounter, _dwInterval;
	int		_iStack;
	WRD		_wrd, _wMTU_child;
	WORD	_wFlags;
	ACRET	_ret = ACRET_TRUE;

	_iStack = GetCurrentModuThread();

	CORE_ASSERT( ModuGetAlgo(wMTU_) == IDA_LOOP );
	_lpPrm = ModuGetParameter( wMTU_ );
	MFPUREAD( _lpPrm, LOOP_CONDITION, BOOL, _bCondition );

	if( !_bCondition )
	{
		//LOOP condition if FALSE, does not loop
		//clear STACKFLAG_LOOP flag for this modu in stack
		_wrd = PopModuThread( _iStack, &_wFlags );
		CORE_ASSERT( _wrd == wMTU_ );
		_wFlags &= ( ~STACKFLAG_LOOP );
		PushModuThread( _iStack, _wrd, _wFlags );

		return ACRET_TRUE;
	}

	///////////////////////////////
	//or blocking or do loop here

	MFPUREAD( _lpPrm, LOOP_INTERVAL, DWORD, _dwInterval );
	if( _dwInterval )
	{
		MFPUREAD( _lpPrm, LOOP_IV_COUNTER, DWORD, _dwCounter );
		if( _dwCounter == 0 )
		{
			//set to initial value again
			MFPUWRITE( _lpPrm, LOOP_IV_COUNTER, DWORD, _dwInterval );
		}
		else
		{
			_dwCounter --;
			MFPUWRITE( _lpPrm, LOOP_IV_COUNTER, DWORD, _dwCounter );

			return ACRET_BLOCK;	//keep BLOCK state
		}
	}

	//do loop here
	_wMTU_child = ModuGetChild( wMTU_ );
	if( _wMTU_child )
	{
		//add STACKFLAG_LOOP flag for this modu in stack
		_wrd = PopModuThread( _iStack, &_wFlags );
		CORE_ASSERT( _wrd == wMTU_ );
		PushModuThread( _iStack, _wrd, (WORD)(_wFlags | STACKFLAG_LOOP) );

		MFPUREAD( _lpPrm, LOOP_INTERVAL, DWORD, _dwInterval );

		//Note: Don't call RunModuTree function for each child
		//here, we only need to call RunModuTree function for
		//the first child. When the function running, it will run
		//for each child. The detail references to the technique
		//notes;
		_ret = RunModuTree( _iStack, _wMTU_child, STACKFLAG_NORMAL );
	}
	return _ret;	//always return _ret
}

// Algorithm: Break
//...................................................................
//...................................................................
#pragma argsused
_DLLEXP DWORD _DLLFUNC AlgoBreakParam(PIC wCmd_, WRD wMTU_, WORD wNo_, LONG lParam_)
{
	CORE_ASSERT(wNo_ == 0);

	switch( wCmd_ )
	{
		case PIC_GETICON:
			return LoadIcon( ghInst, MAKEINTRESOURCE(IDI_BREAK) );

		default:
			return _DefAlgoParam(wCmd_,
								  wMTU_,
								  wNo_,
								  lParam_,
								  0,		//no param
								  IDA_BREAK,
								  NULL );
	}
}

//...................................................................
//Description:
//		does nothing
//...................................................................
_DLLEXP ACRET _DLLFUNC AlgoBreak(WRD wMTU_)
{
	CORE_ASSERT( ModuGetAlgo(wMTU_) == IDA_BREAK );

	//break current loop
	return ACRET_BREAK;
}

#define CALL_ENTRY			0
#define CALL_ENTRY_PDT		PDT_WRDMTU

// Algorithm: Call
//...................................................................
//...................................................................
#pragma argsused
_DLLEXP DWORD _DLLFUNC AlgoCallParam(PIC wCmd_, WRD wMTU_, WORD wNo_, LONG lParam_)
{
	static PDU _sPDU[] =
	{
		_ELE_PDU(CALL_ENTRY,	"Entry",	VSOF_WRDMTU, PU_NORMAL, PRI_ALL|PDUF_NODEFAULT ),
	};

	switch( wCmd_ )
	{
		case PIC_GETICON:
			return LoadIcon( ghInst, MAKEINTRESOURCE(IDI_CALL) );

		default:
			return _DefAlgoParam(wCmd_,
								  wMTU_,
								  wNo_,
								  lParam_,
								  sizeof(_sPDU)/sizeof(PDU),
								  IDA_CALL,
								  &(_sPDU[0]) );
	}
}

//...................................................................
//Description:
//		does nothing
//...................................................................
_DLLEXP ACRET _DLLFUNC AlgoCall(WRD wMTU_)
{
	LPPU	_lpPrm;
	WRD		_wrd;
	ACRET	_ret = ACRET_TRUE;

	CORE_ASSERT( ModuGetAlgo(wMTU_) == IDA_CALL );
	_lpPrm = ModuGetParameter( wMTU_ );

	MFPUREAD( _lpPrm, CALL_ENTRY, WRDMTU, _wrd );
	if( _wrd )
	{
		int		_iStack;
		WORD	_wFlags;

		//add STACKFLAG_CALL flag for this modu in stack
		_iStack = GetCurrentModuThread();
		_wrd = PopModuThread( _iStack, &_wFlags );
		CORE_ASSERT( _wrd == wMTU_ );
		PushModuThread( _iStack, _wrd, (WORD)(_wFlags | STACKFLAG_CALL) );

		_ret = RunModuTree( _iStack, _wrd, STACKFLAG_ROOT );
		if( _ret == ACRET_RETURN )
		{
			//filt this return value, otherwise, if we return
			//with this value, the whole thread will terminate
			return ACRET_TRUE;
		}
	}

	return _ret;
}

// Algorithm: Return
//...................................................................
//...................................................................
#pragma argsused
_DLLEXP DWORD _DLLFUNC AlgoReturnParam(PIC wCmd_, WRD wMTU_, WORD wNo_, LONG lParam_)
{
	CORE_ASSERT(wNo_ == 0);
	switch( wCmd_ )
	{
		case PIC_GETICON:
			return LoadIcon( ghInst, MAKEINTRESOURCE(IDI_RETURN) );

		default:
			return _DefAlgoParam(wCmd_,
								  wMTU_,
								  wNo_,
								  lParam_,
								  0,		//no param
								  IDA_RETURN,
								  NULL );
	}
}

//...................................................................
//Description:
//		does nothing
//...................................................................
_DLLEXP ACRET _DLLFUNC AlgoReturn(WRD wMTU_)
{
	CORE_ASSERT( ModuGetAlgo(wMTU_) == IDA_RETURN );

	return ACRET_RETURN;
}

#define EXECUTE_ENTRY			0
#define EXECUTE_ENTRY_PDT		PDT_WRDMTU

// Algorithm: Execute
//...................................................................
//...................................................................
#pragma argsused
_DLLEXP DWORD _DLLFUNC AlgoExecuteParam(PIC wCmd_, WRD wMTU_, WORD wNo_, LONG lParam_)
{
	static PDU _sPDU[] =
	{
		_ELE_PDU(EXECUTE_ENTRY,	"Entry",	VSOF_WRDMTU, PU_NORMAL, PRI_ALL|PDUF_NODEFAULT ),
	};

	switch( wCmd_ )
	{
		case PIC_GETICON:
			return LoadIcon( ghInst, MAKEINTRESOURCE(IDI_EXECUTE) );

		default:
			return _DefAlgoParam( wCmd_,
								  wMTU_,
								  wNo_,
								  lParam_,
								  sizeof(_sPDU)/sizeof(PDU),
								  IDA_EXECUTE,
								  &(_sPDU[0]) );
	}
}

//...................................................................
//Description:
//...................................................................
_DLLEXP ACRET _DLLFUNC AlgoExecute(WRD wMTU_)
{
	LPPU	_lpPrm;
	WRD		_wrd;

	CORE_ASSERT( ModuGetAlgo(wMTU_) == IDA_EXECUTE );
	_lpPrm = ModuGetParameter( wMTU_ );

	MFPUREAD( _lpPrm, EXECUTE_ENTRY, WRDMTU, _wrd );
	if( _wrd )
	{

		int		_iStack;

		//get the current thread
		_iStack = GetCurrentModuThread();

		InitiateModuThread( _wrd );

		//restore the current modu thread!
		SetCurrentModuThread( _iStack);
	}

	//Note:
	//Don't return the result of InitiateModuThread, which
	//is the result of another thread and is meaningless for
	//this thread!
	//always returns ACRET_TRUE here! 
	return ACRET_TRUE;
}
