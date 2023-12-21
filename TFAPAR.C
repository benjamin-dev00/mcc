//===================================================================
//
//ttf  97-10-07
//===================================================================
//APART a one-dimension matrix to all digital members
#include "tfalgo.h"
#include "tfcmdmc.h"
#define APART_ALL	   			0
#define APART_APART   			0

#define APART_ALL_PDT 			PDT_COBJ
#define APART_APART_PDT 		PDT_REAL
#define APARTCONST				1


DWORD _DLLFUNC APARTParam(PIC wCmd_, WRD wMTU_, WORD wNo_, LONG lParam_)
{
	DWORD			_ret;
	LPPU			_lppu, _lppuvar,_lppunow;
	int				_Num_fix, _Num;

	static PDU	_sfixPDU[] = {
		_ELE_PDU( APART_ALL,			"pAll",		VSOF_LPCOBJ, PU_000, PU_PRI_RW),
	};

	static PDU	_svarPDU[] = {
		_ELE_PDU( APART_APART,			"pApart",	VSOF_REAL, PU_000, PU_PRI_RW),
	};

	static VPSTRUCT	_sVPD = {
		1,			//the default number of groups of var parameters
		0,			//the low bound of var counter
		sizeof( _sfixPDU ) / sizeof( _sfixPDU[0] ),
		&( _sfixPDU[0] ),
		sizeof( _svarPDU ) / sizeof( _svarPDU[0] ),
		&( _svarPDU[0] )
	};

	_ret = DefProcessVarPIC(wCmd_, wMTU_, wNo_, lParam_, TBX_MT, &_sVPD);

	switch( wCmd_ )
	{
		case PIC_INITIALIZEPARAM:
		{
			_ret = DefProcessVarPIC(wCmd_, wMTU_, wNo_, lParam_, TBX_MT, &_sVPD);
			if( _ret == FALSE ) return _ret;
			_lppu = MeterGetParameter( wMTU_ );
			_lppuvar = _lppu + sizeof( _sfixPDU ) / sizeof( _sfixPDU[0] );

			_Num_fix = sizeof( _sfixPDU ) / sizeof( _sfixPDU[0] );
			if( wNo_ >= _Num_fix )
			{
				_Num = ( wNo_ - _Num_fix ) / ( sizeof( _svarPDU ) / sizeof( _svarPDU[0] ) );
				_lppunow = _lppuvar + _Num;
				MFPUWRITE( _lppunow, APART_APART, REAL, 0.0 );
			}
		}
		default:
			return	_ret;
	}
}


ACRET _DLLFUNC APARTCalcu(WRD wMTU_)
{
	int				_i, _Num_var, _Num_Para;
	LPPU			_lppu, _lppuvar;
	LPMATRIX		_lpAll;
	LPCOBJ			_lpCobj_All;

	_lppu = MeterGetParameter( wMTU_ );
	ASSERT( MeterGetAlgo(wMTU_) == IDA_APART );

	MFPUREAD( _lppu, APART_ALL, COBJ, _lpCobj_All );
	if( _lpCobj_All == NULL )
	{
		MessageBox( NULL, "ERROR", "TFAPART.C", MB_OK );
		return FALSE;
	}
	_lpAll = MAKE_MATRIX( _lpCobj_All );
	_Num_Para = MeterGetNumParam( wMTU_ );
	_Num_var = _Num_Para - APARTCONST;
	if( _Num_var != _lpAll->row )
	{
		MessageBox( NULL, "Wrong Number of Apart Member", "ERROR", MB_OK );
    	return	TRUE;
	}
	_lppuvar = _lppu + APARTCONST;
	for( _i = 0; _i < _Num_var; _i++ )
	{
		MFPUWRITE( _lppuvar, APART_APART, REAL, MGET( _lpAll, _i, 0 ) );
		_lppuvar++;
    }

	return	TRUE;
}