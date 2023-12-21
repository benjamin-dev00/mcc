//===================================================================
//
//Zheng zhikai	98-4-16
//===================================================================
#include "prehead.h"

#include "coreall.h"
#include "algo.h"
#include "algodef.h"

#include "acr.h"
#include "AlgoRes.h"

HINSTANCE	ghInst;

#define	EXCHANGEREAL_RESULT			0
#define EXCHANGEREAL_RESULT_PDT		PDT_DWORD

#define	EXCHANGEREAL_RTU			0
#define	EXCHANGEREAL_ACCESS			1
#define EXCHANGEREAL_RTU_PDT		PDT_WRDRTU
#define	EXCHANGEREAL_ACCESS_PDT		PDT_int

//for ExchangeREAL_ACCESS
#define ACCESS_INPUT	1
#define ACCESS_OUTPUT	2

static VSOFNAME _mode_vsof[] =	//Access
{
	{ACCESS_INPUT,	"Input"},
	{ACCESS_OUTPUT,	"Output"},
};


///////////////  ExchangeR  //////////////////////////////////////////
//...................................................................
//...................................................................
_DLLEXP DWORD _DLLFUNC ExchangeRParam(PIC wCmd_, WRD wMTU_, WORD wNo_, LONG lParam_)
{
	static PDU	_sfixPDU[] = {
		_ELE_PDU(EXCHANGEREAL_RESULT,	"Idle",	VSOF_DWORD,	PU_NORMAL,	PRI_ALL )
	};

	static PDU	_svarPDU[] = {
		_ELE_PDU(EXCHANGEREAL_RTU,		"Rtu",		VSOF_WRDRTU,	PU_NORMAL,	PRI_ALL  ),
		_ELE_PDU(EXCHANGEREAL_ACCESS,	"Access",	VSOF_int_USER,	PU_NORMAL,	PRI_ALL  )
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
		return (DWORD)LoadIcon( ghInst, MAKEINTRESOURCE(IDI_EXCHANGER) );

	if( ( wCmd_ == PIC_GETVSOFENUMSTRUCT ) && 
		( wNo_ > 1 ) && 
		( (wNo_ - 1 ) % 2 == EXCHANGEREAL_ACCESS ) )
	{
		LPVSOFENUMSTRUCT _lpes = (LPVSOFENUMSTRUCT)lParam_;
		
		_lpes->lpVsofName = _mode_vsof;
		_lpes->nItem = sizeof(_mode_vsof)/sizeof(_mode_vsof[0]);
		return TRUE;
	}

	return DefProcessVarPIC(wCmd_, wMTU_, wNo_, lParam_, TBX_MT, IDA_EXCHANGER, &_sVPD);
}

//...................................................................
//Description:
//...................................................................
_DLLEXP ACRET _DLLFUNC ExchangeRCalcu(WRD wMTU_)
{
	SimplePU 	_spu, _spu1;
	PU			_pu;

	unsigned 	_i, _items;
	WORD		_wNumParam;
	LPPU		_lpPrm, _lppu;

	WRD			_wrd;
	int			_access;

	char		_szBuffer[256];

	CORE_ASSERT( ModuGetAlgo(wMTU_) == IDA_EXCHANGER );

	_spu.prefix = PF_IMM|IMM_WRDRTU;
	_spu1.prefix = PF_IMM|IMM_int;

	_wNumParam = ModuGetNumParam(wMTU_);
	_lpPrm = ModuGetParameter(wMTU_);

	//_items = ( _wNumParam - fix_param ) / var_param ;
	//
	_items = ( _wNumParam - 1 ) / 2 ;

	for(_i=0, _lppu = _lpPrm + 1;
		_i<_items;
		_i++, _lppu = _lpPrm + 1 + _i*2 )
	{
		if( !PURead( &_spu, _lppu ) )
			continue;
		_wrd = _spu.du.zzk__WRD;

		_lppu ++;
		PURead( &_spu1, _lppu );
		_access = _spu.du.zzk__int;

		if( !_wrd )
			continue;

		if( !RealGetTag( _wrd, _szBuffer, sizeof(_szBuffer) ) )
			continue;

		_pu.prefix = PF_TBX|TBX_RT|RTU_VALUE;
		_pu.du.zzk__WRD = _wrd;

		if( _access & ACCESS_INPUT )
		{
			float	_data;

			if( 1 != ReadCorebaseFloat( _szBuffer, &_data ) )
				continue;	//if the old data or failed
			
			WRITEPU(&_pu, REAL, _data);
			continue;
		}

		if( _access & ACCESS_OUTPUT )
		{
			float	_data;
		
			READPU(&_pu, REAL, _data);

			WriteCorebaseFloat( _szBuffer, _data );
			continue;
		}
	}

	return  ACRET_TRUE;
}

