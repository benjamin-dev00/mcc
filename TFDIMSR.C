//===================================================================
//
//ttf  97-10-06
//===================================================================
//from step-response, make up the SR-matrix needed by the core algorithm
#include "tfalgo.h"
#include "tfcmdmc.h"
#define DIMSR_SR	   			0
#define DIMSR_SRS   			0
#define DIMSR_p		   			1
#define DIMSR_m		   			2
#define DIMSR_N		   			3

#define DIMSR_SR_PDT 			PDT_COBJ
#define DIMSR_SRS_PDT 			PDT_COBJ
#define DIMSR_m_PDT 			PDT_WORD
#define DIMSR_p_PDT 			PDT_WORD
#define DIMSR_N_PDT 			PDT_WORD
#define DIMSRCONST				4

#define	DIMMFPUREAD(__lpPUBase_, __param_, __type_, __value_)	\
	{                                            				\
		SimplePU	__pu_;                       				\
		__pu_.wPrefix = PF_IMM|IMM_##__type_;       			\
		__pu_.du.zzk__FULL = NULL;  /* while read cobj, this must be clear */ \
		PURead( &__pu_, (__lpPUBase_ + __param_) );       		\
		(__value_) = __pu_.du.zzk__##__type_;					\
	}
#define	DIMMFPUWRITE(__lpPUBase_, __param_, __type_, __value_)	\
	{                                            				\
		SimplePU	__pu_;                       				\
		__pu_.wPrefix = PF_IMM|IMM_##__type_;       			\
		__pu_.du.zzk__##__type_ = __value_;          			\
		PUWrite( (__lpPUBase_ + __param_), &__pu_);       		\
	}

DWORD _DLLFUNC DimSRParam(PIC wCmd_, WRD wMTU_, WORD wNo_, LONG lParam_)
{
	DWORD			_ret;
	WORD			_N, _m, _p;
	LPPU			_lppu, _lppuvar,_lppunow;
//	LPMATRIX    	_lpSR;
//	LPCOBJ			_lpCobj_SR;
	LPCOBJ			_lpCobj_DimSR;
	LPDIMMATRIX		_lpDimSR;
	int				_Num_fix, _Num_var, _Num_Para, _Num;

	static PDU	_sfixPDU[] = {
		_ELE_PDU(DIMSR_SR,			"CobjSR",		VSOF_COBJ, PU_000, PU_PRI_RW),
		_ELE_PDU(DIMSR_p,			"p",			VSOF_WORD, PU_000, PU_PRI_RW),
		_ELE_PDU(DIMSR_m,			"m",			VSOF_WORD, PU_000, PU_PRI_RW),
		_ELE_PDU(DIMSR_N,			"N",			VSOF_WORD, PU_000, PU_PRI_RW),
	};

	static PDU	_svarPDU[] = {
		_ELE_PDU( DIMSR_SRS,			"stepres",		VSOF_COBJ, PU_000, PU_PRI_RW),
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
		if( wNo_ < _Num_fix )
		{
			if( wNo_ == DIMSR_N )MFPUWRITE( _lppu, DIMSR_N, WORD, 60 );
			if( wNo_ == DIMSR_p )MFPUWRITE( _lppu, DIMSR_p, WORD, 3 );
			if( wNo_ == DIMSR_m )MFPUWRITE( _lppu, DIMSR_m, WORD, 2 );
		}
		else
		{
			_Num = ( wNo_ - _Num_fix ) / ( sizeof( _svarPDU ) / sizeof( _svarPDU[0] ) );
			_lppunow = _lppuvar + _Num;
			MFPUWRITE( _lppunow, DIMSR_SRS, COBJ, NULL );
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
				MFPUREAD( _lppu, DIMSR_N, WORD, _N );
				MFPUREAD( _lppu, DIMSR_m, WORD, _m );
				MFPUREAD( _lppu, DIMSR_p, WORD, _p );
				if( _Num_var != _p*_m )
                	MessageBox( NULL, "ERROR", "WRONG NUMBER OF SRS", MB_OK );
				_lpDimSR = DimAlloc( _p, _m, _N );
				_lpCobj_DimSR = MAKE_COBJ( _lpDimSR );
/*				for( _i = 0; _i < _p; _i++ )
					for( _j = 0; _j < _m; _j++ )
					{
						_lpSR = MatrixAlloc( _N, 1 );
						_lpCobj_SR = MAKE_COBJ( _lpSR );
						DIMMFPUWRITE( _lppu, DIMSR_SRS+_i*_m+_j, COBJ, _lpCobj_SR );
					}
*/
				MFPUWRITE( _lppu, DIMSR_SR, COBJ, _lpCobj_DimSR );
				return	TRUE;
			}

		case PIC_STOP:
			{
				_lppu = MeterGetParameter( wMTU_ );
				MFPUWRITE( _lppu, DIMSR_SR,   COBJ, NULL );
				return	TRUE;
			}
	}
	return _ret;
}


ACRET _DLLFUNC DimSRCalcu(WRD wMTU_)
{
	int				_i, _j, _k;
	WORD			_N, _m, _p;
	LPPU			_lppu, _lppuvar;
	LPMATRIX    	_lpSR;
	LPDIMMATRIX		_lpDimSR;
	LPCOBJ			_lpCobj_SR, _lpCobj_DimSR;
	_lppu = MeterGetParameter( wMTU_ );
	ASSERT( MeterGetAlgo(wMTU_) == IDA_DIMSR );
	MFPUREAD( _lppu, DIMSR_N, WORD, _N );
	MFPUREAD( _lppu, DIMSR_m, WORD, _m );
	MFPUREAD( _lppu, DIMSR_p, WORD, _p );
	MFPUREAD( _lppu, DIMSR_SR, COBJ, _lpCobj_DimSR );
	_lpDimSR = MAKE_DIMMATRIX( _lpCobj_DimSR );
    _lppuvar = _lppu + DIMSRCONST;

	for( _i = 0; _i < _p; _i++ )
		for( _j = 0; _j < _m; _j++ )
		{
			DIMMFPUREAD( _lppuvar, _i*_m+_j, COBJ, _lpCobj_SR );
			_lpSR = COBJ_2_MATRIX( _lpCobj_SR );
			for( _k = 0; _k < _N; _k++ )
				DIMMGET( _lpDimSR, _i, _j, _k ) = MGET( _lpSR, _k, 0 );
		}

	MFPUWRITE( _lppu, DIMSR_SR, COBJ, _lpCobj_DimSR );
	return	TRUE;
}