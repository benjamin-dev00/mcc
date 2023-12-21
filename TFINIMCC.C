#include "tfalgo.h"
#include "tfcmdmc.h"
// It would deal with the disturbance.
WRD Initiate( LPCOBJPRIV lpCobj_All_, LPCOBJPRIVCV lpCobj_PrivCV_, LPCOBJPRIVDV lpCobj_PrivDV_ )
{
	int			_i, _j, _k;
	LPMATRIX	_lpmAD, _lpmDistPred, _lpmdeltD, _lpmypredN, _lpmDistPredt;
	LPDIMMATRIX	_lpmDSR;

	if( lpCobj_All_->nd == 0 )
		return  TRUE;
	_lpmypredN = MAKE_MATRIX( lpCobj_All_->LPPredN );
	_lpmDistPred = MatrixAlloc( lpCobj_All_->p*lpCobj_All_->N, lpCobj_All_->nd );
	_lpmAD = MatrixAlloc( lpCobj_All_->p * lpCobj_All_->N, lpCobj_All_->nd );

	for( _i = 0; _i < lpCobj_All_->p; _i++ )
		for( _j = 0; _j < lpCobj_All_->N; _j++ )
			MGET( _lpmypredN, (_i*lpCobj_All_->N+_j), 0 ) =
				PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CData;

	_lpmDSR = MAKE_DIMMATRIX( lpCobj_All_->LPDSR );

	for( _i = 0; _i < lpCobj_All_->p; _i++ )
		for( _j = 0; _j < lpCobj_All_->nd; _j++ )
			for( _k = 0; _k < lpCobj_All_->N; _k++ )
				MGET( _lpmAD, _i*lpCobj_All_->N+_k, _j ) = DIMMGET( _lpmDSR, _i, _j, _k );
	_lpmdeltD = MatrixAlloc( lpCobj_All_->nd, 1  );
	for( _j = 0; _j < lpCobj_All_->nd; _j++ )
		MGET( _lpmdeltD, _j, 0 ) = PRIVDVGET( lpCobj_PrivDV_, _j, 0 )->DDelt;
	mmultiple( _lpmDistPred, _lpmAD, _lpmdeltD );
	_lpmDistPredt = MatrixAlloc( lpCobj_All_->p*lpCobj_All_->N, 1 );
	for( _i = 0; _i < lpCobj_All_->nd; _i++ )
	{
		for( _k = 0; _k < lpCobj_All_->p*lpCobj_All_->N; _k++ )
			MGET( _lpmDistPredt, _k, 0 ) = MGET( _lpmDistPred, _k, _i );
		madd( _lpmypredN, _lpmypredN, _lpmDistPredt );
	}
	MATRIXFREE( _lpmDistPred );
	MATRIXFREE( _lpmDistPredt );
	MATRIXFREE( _lpmAD );
	MATRIXFREE( _lpmdeltD );

	return TRUE;
}


