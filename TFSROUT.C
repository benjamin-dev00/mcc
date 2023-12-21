//===================================================================
//
//ttf  97-10-06
//===================================================================
//from the SR-matrix, calculate the output of the model
#include "tfalgo.h"
#include "tfcmdmc.h"

#define	MAKEMATRIXOUT_Y				0
#define	MAKEMATRIXOUT_Y0			1
#define	MAKEMATRIXOUT_U			    2
#define	MAKEMATRIXOUT_U0			3
#define	MAKEMATRIXOUT_D			    4
#define	MAKEMATRIXOUT_D0			5
#define	MAKEMATRIXOUT_PREU			6
#define	MAKEMATRIXOUT_PRED			7
#define	MAKEMATRIXOUT_HISTORY		8
#define	MAKEMATRIXOUT_DHISTORY		9
#define	MAKEMATRIXOUT_SR			10
#define	MAKEMATRIXOUT_DSR			11
#define MAKEMATRIXOUT_Y_PDT  		PDT_COBJ
#define MAKEMATRIXOUT_Y0_PDT  		PDT_COBJ
#define MAKEMATRIXOUT_HISTORY_PDT  	PDT_COBJ
#define MAKEMATRIXOUT_DHISTORY_PDT  PDT_COBJ
#define MAKEMATRIXOUT_U_PDT  		PDT_COBJ
#define MAKEMATRIXOUT_U0_PDT  		PDT_COBJ
#define MAKEMATRIXOUT_PREU_PDT  	PDT_COBJ
#define MAKEMATRIXOUT_D_PDT  		PDT_COBJ
#define MAKEMATRIXOUT_D0_PDT  		PDT_COBJ
#define MAKEMATRIXOUT_PRED_PDT  	PDT_COBJ
#define MAKEMATRIXOUT_SR_PDT  		PDT_COBJ
#define MAKEMATRIXOUT_DSR_PDT  		PDT_COBJ

DWORD _DLLFUNC MakeMatrixOutputParam(PIC wCmd_, WRD wMTU_, WORD wNo_, LONG lParam_)
{
	DWORD		_ret;
	LPPU		_lppu;
	LPMATRIX    _lpmY, _lpmu0, _lpmPredU;
	LPDIMMATRIX	_lpDimSR, _lpDimDSR, _lpHistory, _lpDHistory;
	LPCOBJ		_lpCobj_History, _lpCobj_DHistory, _lpCobj_SR;
	LPCOBJ		_lpCobj_DSR, _lpCobj_Y, _lpCobj_U0, _lpCobj_D0;
	int			_row, _col, _axe;

	static PDU _sPDU[] =
	{
		_ELE_PDU( MAKEMATRIXOUT_Y,			"Y",			VSOF_COBJ,		PU_000, PU_PRI_RW),
		_ELE_PDU( MAKEMATRIXOUT_Y0,			"Y0",			VSOF_COBJ,		PU_000, PU_PRI_RW),
		_ELE_PDU( MAKEMATRIXOUT_U,			"U",			VSOF_COBJ,		PU_000, PU_PRI_RW),
		_ELE_PDU( MAKEMATRIXOUT_U0,			"U0",			VSOF_COBJ,		PU_000, PU_PRI_RW),
		_ELE_PDU( MAKEMATRIXOUT_D,			"Dist",			VSOF_COBJ,		PU_000, PU_PRI_RW),
		_ELE_PDU( MAKEMATRIXOUT_D0,			"Dist0",		VSOF_COBJ,		PU_000, PU_PRI_RW),
		_ELE_PDU( MAKEMATRIXOUT_PREU,		"PreU",			VSOF_COBJ,		PU_000, PU_PRI_RW),
		_ELE_PDU( MAKEMATRIXOUT_PRED,		"PreD",			VSOF_COBJ,		PU_000, PU_PRI_RW),
		_ELE_PDU( MAKEMATRIXOUT_HISTORY, 	"History", 		VSOF_COBJ,		PU_000, PU_PRI_RW),
		_ELE_PDU( MAKEMATRIXOUT_DHISTORY, 	"DHistory", 	VSOF_COBJ,		PU_000, PU_PRI_RW),
		_ELE_PDU( MAKEMATRIXOUT_SR, 		"StepRes", 		VSOF_COBJ,		PU_000, PU_PRI_RW),
		_ELE_PDU( MAKEMATRIXOUT_DSR, 		"DistSR", 		VSOF_COBJ,		PU_000, PU_PRI_RW),
	};

	_ret = _DefAlgoParam( wCmd_,
						  wMTU_,
						  wNo_,
						  lParam_,
						  sizeof(_sPDU)/sizeof(PDU),
						  &(_sPDU[0]) );

	switch( wCmd_ )
	{
		case PIC_INITIATE:
		{
			LPCOBJ	_lpCobj_Preu, _lpCobj_PreD;
			BOOL	_bOk = TRUE;

			_lppu = MeterGetParameter( wMTU_ );
			MFPUREAD( _lppu, MAKEMATRIXOUT_SR, 		COBJ, _lpCobj_SR );
			MFPUREAD( _lppu, MAKEMATRIXOUT_DSR, 	COBJ, _lpCobj_DSR );
			if( _lpCobj_SR == NULL )
			{
				MessageBox( NULL, "DATA LINK HAS NOT BEEN ESTABLISHED", "ERROR", MB_OK );
				return TRUE;
			}
			MFPUREAD( _lppu, MAKEMATRIXOUT_U0, 		COBJ, _lpCobj_U0 );
			_lpmu0 = COBJ_2_MATRIX( _lpCobj_U0 );
			_lpmPredU = MatrixAlloc( _lpmu0->row+1, 1 );
			MFPUREAD( _lppu, MAKEMATRIXOUT_D0, 		COBJ, _lpCobj_D0 );
			MGET( _lpmPredU, _lpmu0->row, 0 ) = 0;
			_lpCobj_Preu = MATRIX_2_COBJ( _lpmPredU );
			if( _lpCobj_D0 != NULL )
				_lpCobj_PreD = TFCobjCopy ( _lpCobj_D0, &_bOk );
			else
				_lpCobj_PreD = NULL;
			MFPUWRITE( _lppu, MAKEMATRIXOUT_PREU, 	COBJ, _lpCobj_Preu );
			MFPUWRITE( _lppu, MAKEMATRIXOUT_PRED, 	COBJ, _lpCobj_PreD );
			_lpDimSR = MAKE_DIMMATRIX( _lpCobj_SR );
			_row = _lpDimSR->row;
			_col = _lpDimSR->col;
			_axe = _lpDimSR->axe;
			_lpmY = MatrixAlloc( _row, 1 );
			_lpCobj_Y = MAKE_COBJ( _lpmY );
			MFPUWRITE( _lppu, MAKEMATRIXOUT_Y, COBJ, _lpCobj_Y );
			_lpHistory = DimAlloc( _row, _col, _axe );
			_lpCobj_History = MAKE_COBJ( _lpHistory );
			MFPUWRITE( _lppu, MAKEMATRIXOUT_HISTORY, COBJ, _lpCobj_History );
			if( _lpCobj_DSR != NULL )
			{
				_lpDimDSR = MAKE_DIMMATRIX( _lpCobj_DSR );
				_row = _lpDimDSR->row;
				_col = _lpDimDSR->col;
				_axe = _lpDimDSR->axe;
				_lpDHistory = DimAlloc( _row, _col, _axe );
				_lpCobj_DHistory = MAKE_COBJ( _lpDHistory );
			}
			else
				_lpCobj_DHistory = NULL;
			MFPUWRITE( _lppu, MAKEMATRIXOUT_DHISTORY, COBJ, _lpCobj_DHistory );

			return	TRUE;
		}

		case PIC_STOP:
		{
			_lppu = MeterGetParameter( wMTU_ );
//			MFPUREAD( _lppu, MAKEMATRIXOUT_DHISTORY, COBJ, _lpCobj_DHis );
			MFPUWRITE( _lppu, MAKEMATRIXOUT_HISTORY, COBJ, NULL );
			MFPUWRITE( _lppu, MAKEMATRIXOUT_DHISTORY, COBJ, NULL );
			MFPUWRITE( _lppu, MAKEMATRIXOUT_Y, 		 COBJ, NULL );
			MFPUWRITE( _lppu, MAKEMATRIXOUT_PREU, 	 COBJ, NULL );
			MFPUWRITE( _lppu, MAKEMATRIXOUT_PRED, 	 COBJ, NULL );
			return TRUE;
		}
	}
	return _ret;
}

//...................................................................
//Description:
//	Algorithm:	use the linearity of the plant and the step-resonse
//...................................................................
ACRET _DLLFUNC MakeMatrixOutputCalcu(WRD wMTU_)
{
	int				_i, _j, _k;
	LPPU			_lppu;
	LPMATRIX    	_lpmu, _lpmPreu, _lpmdelu,_lpmy, _lpmD0;
	LPMATRIX		_lpmDist, _lpmPreDist, _lpmu0, _lpmdelDist;
	LPDIMMATRIX		_lpDimSR, _lpDimHis, _lpDimDSR, _lpDimDHis;
	LPCOBJ			_lpCobj_His, _lpCobj_DHis, _lpCobj_SR, _lpCobj_DSR;
	LPCOBJ			_lpCobj_U, _lpCobj_Dist, _lpCobj_PreU, _lpCobj_PreDist;
	LPCOBJ			_lpCobj_Y, _lpCobj_Y0, _lpCobj_U0, _lpCobj_D0;
	WORD 			_row, _col, _N;

	_lppu = MeterGetParameter( wMTU_ );
	ASSERT( MeterGetAlgo(wMTU_) == IDA_MATRIXOUT );
	ASSERT( MeterGetNumParam(wMTU_) == 12 );
	MFPUREAD( _lppu, MAKEMATRIXOUT_SR,   		COBJ, _lpCobj_SR );
	MFPUREAD( _lppu, MAKEMATRIXOUT_DSR,   		COBJ, _lpCobj_DSR );
	MFPUREAD( _lppu, MAKEMATRIXOUT_HISTORY,   	COBJ, _lpCobj_His );
	MFPUREAD( _lppu, MAKEMATRIXOUT_DHISTORY,   	COBJ, _lpCobj_DHis );
	MFPUREAD( _lppu, MAKEMATRIXOUT_Y,		   	COBJ, _lpCobj_Y );
	MFPUREAD( _lppu, MAKEMATRIXOUT_Y0,		   	COBJ, _lpCobj_Y0 );
	MFPUREAD( _lppu, MAKEMATRIXOUT_U,   		COBJ, _lpCobj_U );
	MFPUREAD( _lppu, MAKEMATRIXOUT_U0,   		COBJ, _lpCobj_U0 );
	MFPUREAD( _lppu, MAKEMATRIXOUT_D,   		COBJ, _lpCobj_Dist );
	MFPUREAD( _lppu, MAKEMATRIXOUT_D0,   		COBJ, _lpCobj_D0 );
	MFPUREAD( _lppu, MAKEMATRIXOUT_PREU,   		COBJ, _lpCobj_PreU );
	MFPUREAD( _lppu, MAKEMATRIXOUT_PRED,   		COBJ, _lpCobj_PreDist );
	_lpDimHis = MAKE_DIMMATRIX( _lpCobj_His);
	_lpDimSR = MAKE_DIMMATRIX( _lpCobj_SR );
	_lpDimDHis = MAKE_DIMMATRIX( _lpCobj_DHis);
	_lpDimDSR = MAKE_DIMMATRIX( _lpCobj_DSR );
	if( _lpCobj_SR == NULL )
	{
		MessageBox( NULL, "DATA LINK HAS NOT BEEN ESTABLISHED", "ERROR", MB_OK );
		return TRUE;
	}
	_lpmu = COBJ_2_MATRIX( _lpCobj_U );
	_lpmu0 = COBJ_2_MATRIX( _lpCobj_U0 );
	_lpmPreu = COBJ_2_MATRIX( _lpCobj_PreU );
	if( (int)MGET( _lpmPreu, _lpmPreu->row-1, 0 ) == 0 )
	{
		for( _i = 0; _i < _lpmu0->row; _i++ )
			MGET( _lpmPreu, _i, 0 ) = MGET( _lpmu0, _i, 0 );
	}
	CobjKeepCopy( _lpCobj_Y, _lpCobj_Y0 );
	_lpmy = MAKE_MATRIX( _lpCobj_Y );
	_row = _lpDimSR->row;
	_col = _lpDimSR->col;
	_N = _lpDimSR->axe;
	_lpmdelu = MatrixAlloc( _col, 1 );
	for( _i = 0; _i < _lpmdelu->row; _i++ )
		MGET( _lpmdelu, _i, 0 ) = MGET( _lpmu, _i, 0 ) -
        							MGET( _lpmPreu, _i, 0 );
	if( _N <= 0 )
	{
		MessageBox( NULL, "error", "wrong N", MB_OK);
		return FALSE;
	}
	for( _i = 0; _i < _row; _i++ )
		for( _j = 0; _j < _col; _j++ )
			for( _k = 0; _k < _N-1; _k++ )
			{
				DIMMGET( _lpDimHis, _i, _j, _k ) =
					DIMMGET( _lpDimHis, _i, _j, _k+1 );
			}
	for( _i = 0; _i < _row; _i++ )
		for( _j = 0; _j < _col; _j++ )
			for( _k = 0; _k < _N; _k++ )
				DIMMGET( _lpDimHis, _i, _j, _k ) += MGET( _lpmdelu, _j, 0 ) *
							DIMMGET( _lpDimSR, _i, _j, _k );
	for( _i = 0; _i < _row; _i++ )
		for( _j = 0; _j < _col; _j++ )
			MGET( _lpmy, _i, 0 ) += DIMMGET( _lpDimHis, _i, _j, 0 );
	if( _lpDimDSR != NULL )
	{
		_lpmD0 = COBJ_2_MATRIX( _lpCobj_D0 );
		_lpmDist = COBJ_2_MATRIX( _lpCobj_Dist );
		_lpmPreDist = COBJ_2_MATRIX ( _lpCobj_PreDist );
		if( (int)MGET( _lpmPreu, _lpmPreu->row-1, 0 ) == 0 )
		{
			for( _i = 0; _i < _lpmD0->row; _i++ )
				MGET( _lpmPreDist, _i, 0 ) = MGET( _lpmD0, _i, 0 );
		}
		_lpmdelDist = MatrixAlloc( _lpDimDSR->col, 1 );
		mminus( _lpmdelDist, _lpmDist, _lpmPreDist );
		_row = _lpDimDSR->row;
		_col = _lpDimDSR->col;
		_N = _lpDimDSR->axe;

		for( _i = 0; _i < _row; _i++ )
			for( _j = 0; _j < _col; _j++ )
				for( _k = 0; _k < _N-1; _k++ )
				{
					DIMMGET( _lpDimDHis, _i, _j, _k ) =
						DIMMGET( _lpDimDHis, _i, _j, _k+1 );
				}
		for( _i = 0; _i < _row; _i++ )
			for( _j = 0; _j < _col; _j++ )
				for( _k = 0; _k < _N; _k++ )
					DIMMGET( _lpDimDHis, _i, _j, _k ) += MGET( _lpmdelDist, _j, 0 ) *
								DIMMGET( _lpDimDSR, _i, _j, _k );
		for( _i = 0; _i < _row; _i++ )
			for( _j = 0; _j < _col; _j++ )
				MGET( _lpmy, _i, 0 ) += DIMMGET( _lpDimDHis, _i, _j, 0 );
		MATRIXFREE( _lpmdelDist );
		CobjKeepCopy( _lpCobj_PreDist, _lpCobj_Dist );
		MFPUWRITE( _lppu, MAKEMATRIXOUT_PRED, COBJ, _lpCobj_PreDist );
	}
	for( _i = 0; _i < _lpmu->row; _i++ )
		MGET( _lpmPreu, _i, 0 ) = MGET( _lpmu, _i, 0 );
	MGET( _lpmPreu, _lpmPreu->row-1, 0 )++;
	MFPUWRITE( _lppu, MAKEMATRIXOUT_PREU, COBJ, _lpCobj_PreU );
	MFPUWRITE( _lppu, MAKEMATRIXOUT_Y, 	  COBJ, _lpCobj_Y );
	MATRIXFREE( _lpmdelu );
	return	TRUE;
}


