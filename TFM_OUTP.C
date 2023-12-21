//===================================================================
//
//ttf  97-10-06
//===================================================================
//from the step-response, calculate the output of the model
#include "tfalgo.h"
#include "tfcmdmc.h"

#define	MAKEOUTP_Y				0
#define	MAKEOUTP_HISTORY		1
#define	MAKEOUTP_SR			    2
#define	MAKEOUTP_U			    3
#define	MAKEOUTP_U0				4
#define	MAKEOUTP_N				5
#define	MAKEOUTP_PREU		    6
#define MAKEOUTP_Y_PDT  		PDT_REAL
#define MAKEOUTP_SR_PDT  		PDT_COBJ
#define MAKEOUTP_HISTORY_PDT  	PDT_COBJ
#define MAKEOUTP_U_PDT  		PDT_REAL
#define MAKEOUTP_U0_PDT  		PDT_REAL
#define MAKEOUTP_N_PDT  		PDT_WORD
#define MAKEOUTP_PREU_PDT  		PDT_REAL

DWORD _DLLFUNC MakeOutputParam(PIC wCmd_, WRD wMTU_, WORD wNo_, LONG lParam_)
{
	DWORD		_ret;
	WORD		_N;
	LPPU		_lppu;
	LPMATRIX    _lpHistory;
	LPCOBJ		_lpCobj_History, _lpCobj_SR;
    double		_u0;

	static PDU _sPDU[] =
	{
		_ELE_PDU( MAKEOUTP_Y,			"Y",			VSOF_REAL,		PU_000, PU_PRI_RW),
		_ELE_PDU( MAKEOUTP_HISTORY, 	"History", 		VSOF_COBJ,		PU_000, PU_PRI_RW),
		_ELE_PDU( MAKEOUTP_SR, 			"StepRes", 		VSOF_COBJ,		PU_000, PU_PRI_RW),
		_ELE_PDU( MAKEOUTP_U,			"U",			VSOF_REAL,		PU_000, PU_PRI_RW),
		_ELE_PDU( MAKEOUTP_U0,			"U0",			VSOF_REAL,		PU_000, PU_PRI_RW),
		_ELE_PDU( MAKEOUTP_N,			"N",			VSOF_WORD,		PU_000, PU_PRI_RW),
		_ELE_PDU( MAKEOUTP_PREU,		"PreU",			VSOF_REAL,		PU_000, PU_PRI_RW),
	};

	_ret = _DefAlgoParam( wCmd_,
						  wMTU_,
						  wNo_,
						  lParam_,
						  sizeof(_sPDU)/sizeof(PDU),
						  &(_sPDU[0]) );

	if( wCmd_ == PIC_INITIALIZEPARAM )
	{
		LPPU	_lppu;

		if( _ret == FALSE ) return _ret;
		_lppu = MeterGetParameter( wMTU_ );

		if( wNo_ == MAKEOUTP_U0 )	MFPUWRITE( _lppu, MAKEOUTP_U0, REAL, 0.0 );
		if( wNo_ == MAKEOUTP_U )	MFPUWRITE( _lppu, MAKEOUTP_U, REAL, 0.0 );
		if( wNo_ == MAKEOUTP_Y )	MFPUWRITE( _lppu, MAKEOUTP_Y, REAL, 0.0 );
		if( wNo_ == MAKEOUTP_N )	MFPUWRITE( _lppu, MAKEOUTP_N, WORD, 60 );
		if( wNo_ == MAKEOUTP_PREU )	MFPUWRITE( _lppu, MAKEOUTP_PREU, REAL, 0 );

	}
	switch( wCmd_ )
	{
		case PIC_INITIATE:
		{
			_lppu = MeterGetParameter( wMTU_ );
			MFPUREAD( _lppu, MAKEOUTP_SR, 		COBJ, _lpCobj_SR );
			if( _lpCobj_SR == NULL )
			{
				MessageBox( NULL, "DATA LINK HAS NOT BEEN ESTABLISHED", "ERROR", MB_OK );
				return TRUE;
			}
			MFPUREAD( _lppu, MAKEOUTP_N, WORD, _N );
			MFPUREAD( _lppu, MAKEOUTP_U0, REAL, _u0 );
			MFPUWRITE( _lppu, MAKEOUTP_PREU, REAL, _u0 );
			_lpHistory = MatrixAlloc( _N, 1 );
			_lpCobj_History = MAKE_COBJ( _lpHistory );
			MFPUWRITE( _lppu, MAKEOUTP_HISTORY, COBJ, _lpCobj_History );

			return	TRUE;
		}

		case PIC_STOP:
		{
			_lppu = MeterGetParameter( wMTU_ );
			MFPUWRITE( _lppu, MAKEOUTP_HISTORY, COBJ, NULL );
			return TRUE;
		}
	}
	return _ret;
}

//...................................................................
//Description:
//	Algorithm:	use the linearity of the plant and the step-resonse
//...................................................................
ACRET _DLLFUNC MakeOutputCalcu(WRD wMTU_)
{
	int				_i;
	LPPU			_lppu;
	LPMATRIX    	_lpHistory, _lpSR;
	LPCOBJ			_lpCobj_History, _lpCobj_SR;
	double			_PreU, _u, _delu;
    WORD 			_N;

	_lppu = MeterGetParameter( wMTU_ );
	ASSERT( MeterGetAlgo(wMTU_) == IDA_MAKEOUT );
	ASSERT( MeterGetNumParam(wMTU_) == 7 );
	MFPUREAD( _lppu, MAKEOUTP_SR,   COBJ, _lpCobj_SR );
	if( _lpCobj_SR == NULL )
	{
		MessageBox( NULL, "DATA LINK HAS NOT BEEN ESTABLISHED", "ERROR", MB_OK );
		return TRUE;
	}
	MFPUREAD( _lppu, MAKEOUTP_N, 	WORD, _N );
	if( _N <= 0 )
	{
		MessageBox( NULL, "error", "wrong N", MB_OK);
        return FALSE;
	}

	MFPUREAD( _lppu, MAKEOUTP_HISTORY, COBJ, _lpCobj_History );
	_lpHistory = MAKE_MATRIX( _lpCobj_History );
	_lpSR = MAKE_MATRIX( _lpCobj_SR );
	for( _i = 0; _i < _N-1; _i++ )
		MGET( _lpHistory, _i, 0 ) = MGET( _lpHistory, _i+1, 0 );
	MFPUREAD( _lppu, MAKEOUTP_U, 	REAL, _u );
	MFPUREAD( _lppu, MAKEOUTP_PREU,	REAL, _PreU );
	_delu = _u - _PreU;
	for( _i = 0; _i < _N; _i++ )
		MGET( _lpHistory, _i, 0 ) += _delu * MGET( _lpSR, _i, 0 );
	MFPUWRITE( _lppu, MAKEOUTP_HISTORY, COBJ, _lpCobj_History );
	MFPUWRITE( _lppu, MAKEOUTP_Y, REAL, MGET( _lpHistory, 0, 0 ) );

	return	TRUE;
}


