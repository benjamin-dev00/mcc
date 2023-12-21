//===================================================================
//
//ttf  97-10-07
//===================================================================
//gather the members to be a one-dimension matrix
#include "tfalgo.h"
#include "tfcmdmc.h"
#define GATHER_ALL	   			0
#define GATHER_APART   			0

#define GATHER_ALL_PDT 			PDT_COBJ
#define GATHER_APART_PDT 		PDT_REAL
#define GATHERCONST				1


DWORD _DLLFUNC GATHERParam(PIC wCmd_, WRD wMTU_, WORD wNo_, LONG lParam_)
{
	DWORD			_ret;
	LPPU			_lppu, _lppuvar,_lppunow;
	LPCOBJ			_lpCobj_GATHER;
	LPMATRIX		_lpGATHER;
	int				_Num_fix, _Num_var, _Num_Para, _Num;

	static PDU	_sfixPDU[] = {
		_ELE_PDU(GATHER_ALL,			"gAll",		VSOF_COBJ, PU_000, PU_PRI_RW),
	};

	static PDU	_svarPDU[] = {
		_ELE_PDU(GATHER_APART,			"gApart",	VSOF_REAL, PU_000, PU_PRI_RW),
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

	if( wCmd_ == PIC_INITIALIZEPARAM )
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
			MFPUWRITE( _lppunow, GATHER_APART, REAL, wNo_ );
		}
		return	_ret;
	}
	switch( wCmd_ )
	{
		case PIC_INITIATE:
			{

				_lppu = MeterGetParameter( wMTU_ );
				_Num_Para = MeterGetNumParam( wMTU_ );
				_Num_fix = sizeof( _sfixPDU ) / sizeof( _sfixPDU[0] );
				_Num_var = _Num_Para - _Num_fix;
				_lpGATHER = MatrixAlloc( _Num_var, 1 );
				_lpCobj_GATHER = MAKE_COBJ( _lpGATHER );
				MFPUWRITE( _lppu, GATHER_ALL, COBJ, _lpCobj_GATHER );
				return	TRUE;
			}

		case PIC_STOP:
			{
				_lppu = MeterGetParameter( wMTU_ );
				MFPUWRITE( _lppu, GATHER_ALL,   COBJ, NULL );
				return	TRUE;
			}
	}
	return _ret;
}


ACRET _DLLFUNC GATHERCalcu(WRD wMTU_)
{
	int				_i, _Num_var, _Num_Para;
	LPPU			_lppu, _lppuvar;
	LPMATRIX		_lpGather;
	LPCOBJ			_lpCobj_GATHER;
    double			_Apart;

	_lppu = MeterGetParameter( wMTU_ );
	ASSERT( MeterGetAlgo(wMTU_) == IDA_GATHER );

	MFPUREAD( _lppu, GATHER_ALL, COBJ, _lpCobj_GATHER );
	if( _lpCobj_GATHER == NULL )
	{
		MessageBox( NULL, "ERROR", "GATHER.C", MB_OK );
		return FALSE;
	}
	_lpGather = MAKE_MATRIX( _lpCobj_GATHER );
	_Num_Para = MeterGetNumParam( wMTU_ );
	_Num_var = _Num_Para - GATHERCONST;
	_lppuvar = _lppu + GATHERCONST;
	for( _i = 0; _i < _Num_var; _i++ )
	{
		MFPUREAD( _lppuvar, GATHER_APART, REAL, _Apart );
		_lppuvar++;
		MGET( _lpGather, _i, 0 ) = _Apart;
    }

	MFPUWRITE( _lppu, GATHER_ALL, COBJ, _lpCobj_GATHER );
	return	TRUE;
}