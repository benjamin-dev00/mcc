//===================================================================
//
//Zheng zhikai	95-4-22
//===================================================================
#include "prehead.h"

#include "coreall.h"

#include "csysstmp.h"
#include "algo.h"
#include "algodef.h"
#include "AlgoRes.h"

HINSTANCE	ghInst;

///////////////  ShowFwnd  //////////////////////////////////////////
#define	SHOWFWND_IDLE			0
#define	SHOWFWND_FWND			1
#define SHOWFWND_IDLE_PDT		PDT_TINT
#define SHOWFWND_FWND_PDT		PDT_LPCOBJ
//...................................................................
//...................................................................
_DLLEXP DWORD _DLLFUNC ShowFwndParam(PIC wCmd_, WRD wMTU_, WORD wNo_, LONG lParam_)
{
	static PDU	_sfixPDU[] = {
		_ELE_PDU(SHOWFWND_IDLE,	"Idle",	VSOF_TINT,		PU_NORMAL,	PRI_ALL )
	};

	static PDU	_svarPDU[] = {
		_ELE_PDU(SHOWFWND_FWND,	"Wnd",	VSOF_LPCOBJ_STR,	PU_NORMAL,	PRI_ALL|PDUF_NODEFAULT )
	};

	static VPSTRUCT	_sVPD = {
		1,			//the default number of groups of var parameters
		0,			//the low bound of var counter
		sizeof( _sfixPDU ) / sizeof( _sfixPDU[0] ),
		&( _sfixPDU[0] ),
		sizeof( _svarPDU ) / sizeof( _svarPDU[0] ),
		&( _svarPDU[0] )
	};

	if(	wCmd_ == PIC_GETICON )
		return (DWORD)LoadIcon( ghInst, MAKEINTRESOURCE(IDI_SHOWWND) );

	return DefProcessVarPIC(wCmd_, wMTU_, wNo_, lParam_, TBX_MT, IDA_SHOWWND, &_sVPD);
}

//...................................................................
//Description:
//...................................................................
_DLLEXP ACRET _DLLFUNC ShowFwndCalcu(WRD wMTU_)
{
	unsigned 	_i;
	WORD		_wNumParam;
	LPPU		_lppu0, _lppu;
	WRD			_wrd;
	LPCOBJ		_lpCobj;

	CORE_ASSERT( ModuGetAlgo(wMTU_) == IDA_SHOWWND );

	_wNumParam = ModuGetNumParam(wMTU_);
	_lppu0 = ModuGetParameter(wMTU_);

	for(_i=1, _lppu = _lppu0 + 1; 	//Note: _i is from 1
		_i<_wNumParam;
		_i++, _lppu++ )
	{
		READPU( _lppu, LPCOBJ, _lpCobj );

		_wrd = 0;
		if( GET_COBJ_DATA(_lpCobj) )
			_wrd = FindFwndByName( (LPSTR)GET_COBJ_DATA(_lpCobj) );

		if( _wrd )
			OpenFwnd( _wrd );
	}

	return  ACRET_TRUE;
}

///////////////  CloseFwnd  //////////////////////////////////////////
#define	CLOSEFWND_IDLE			0
#define	CLOSEFWND_FWND			1
#define CLOSEFWND_IDLE_PDT		PDT_TINT
#define CLOSEFWND_FWND_PDT		PDT_LPCOBJ
//...................................................................
//...................................................................
_DLLEXP DWORD _DLLFUNC CloseFwndParam(PIC wCmd_, WRD wMTU_, WORD wNo_, LONG lParam_)
{
	static PDU	_sfixPDU[] = {
		_ELE_PDU(CLOSEFWND_IDLE,	"Idle",	VSOF_TINT,		PU_NORMAL,	PRI_ALL|PDUF_NODEFAULT )
	};

	static PDU	_svarPDU[] = {
		_ELE_PDU(CLOSEFWND_FWND,	"Wnd",	VSOF_LPCOBJ_STR,	PU_NORMAL,	PRI_ALL|PDUF_NODEFAULT )
	};

	static VPSTRUCT	_sVPD = {
		1,			//the default number of groups of var parameters
		0,			//the low bound of var counter
		sizeof( _sfixPDU ) / sizeof( _sfixPDU[0] ),
		&( _sfixPDU[0] ),
		sizeof( _svarPDU ) / sizeof( _svarPDU[0] ),
		&( _svarPDU[0] )
	};

	if(	wCmd_ == PIC_GETICON )
		return (DWORD)LoadIcon( ghInst, MAKEINTRESOURCE(IDI_CLOSEWND) );

	return DefProcessVarPIC(wCmd_, wMTU_, wNo_, lParam_, TBX_MT, IDA_CLOSEWND, &_sVPD);
}

//...................................................................
//Description:
//...................................................................
_DLLEXP ACRET _DLLFUNC CloseFwndCalcu(WRD wMTU_)
{
	unsigned 	_i;
	WORD		_wNumParam;
	LPPU		_lppu0, _lppu;
	WRD			_wrd;
	LPCOBJ		_lpCobj;

	CORE_ASSERT( ModuGetAlgo(wMTU_) == IDA_CLOSEWND );

	_wNumParam = ModuGetNumParam(wMTU_);
	_lppu0 = ModuGetParameter(wMTU_);

	for(_i=1, _lppu = _lppu0 + 1; 	//Note: _i is from 1
		_i<_wNumParam;
		_i++, _lppu++ )
	{
		READPU( _lppu, LPCOBJ, _lpCobj );

		_wrd = 0;
		if( GET_COBJ_DATA(_lpCobj) )
			_wrd = FindFwndByName( (LPSTR)GET_COBJ_DATA(_lpCobj) );

		if( !_wrd )
			continue;

		CloseFwnd( _wrd );
	}

	return  ACRET_TRUE;
}

///////////////  ReplaceFwnd  //////////////////////////////////////////
#define	REPLACEFWND_FWND_CLOSE			0
#define	REPLACEFWND_FWND_OPEN			1
#define REPLACEFWND_FWND_CLOSE_PDT		PDT_LPCOBJ
#define REPLACEFWND_FWND_OPEN_PDT		PDT_LPCOBJ
//...................................................................
//...................................................................
_DLLEXP DWORD _DLLFUNC ReplaceFwndParam(PIC wCmd_, WRD wMTU_, WORD wNo_, LONG lParam_)
{
	DWORD	_ret;

	static PDU _sPDU[] =
	{
		_ELE_PDU(REPLACEFWND_FWND_CLOSE,	"Close",	VSOF_LPCOBJ_STR, 	PU_NORMAL,	PRI_ALL  ),
		_ELE_PDU(REPLACEFWND_FWND_OPEN,		"Open",		VSOF_LPCOBJ_STR, 	PU_NORMAL,	PRI_ALL  ),
	};

	if(	wCmd_ == PIC_GETICON )
		return (DWORD)LoadIcon( ghInst, MAKEINTRESOURCE(IDI_REPLACEWND) );

	_ret = _DefAlgoParam( wCmd_,
						  wMTU_,
						  wNo_,
						  lParam_,
						  sizeof(_sPDU)/sizeof(PDU),
						  IDA_REPLACEWND, 
						  &(_sPDU[0]) );

	return _ret;
}

//...................................................................
//Description:
//...................................................................
_DLLEXP ACRET _DLLFUNC ReplaceFwndCalcu(WRD wMTU_)
{
	LPPU		_lpPrm;
	LPCOBJ		_lpCobj;
	WRD			_wrd;
	
	CORE_ASSERT( ModuGetAlgo(wMTU_) == IDA_REPLACEWND );

	_lpPrm = ModuGetParameter(wMTU_);

	MFPUREAD( _lpPrm, REPLACEFWND_FWND_CLOSE, LPCOBJ, _lpCobj );
	_wrd = 0;
	if( GET_COBJ_DATA(_lpCobj) )
		_wrd = FindFwndByName( (LPSTR)GET_COBJ_DATA(_lpCobj) );
	if( _wrd )
		CloseFwnd( _wrd );

	MFPUREAD( _lpPrm, REPLACEFWND_FWND_OPEN,  LPCOBJ, _lpCobj );
	_wrd = 0;
	if( GET_COBJ_DATA(_lpCobj) )
		_wrd = FindFwndByName( (LPSTR)GET_COBJ_DATA(_lpCobj) );
	if( _wrd )
		OpenFwnd( _wrd );

	return  ACRET_TRUE;
}


