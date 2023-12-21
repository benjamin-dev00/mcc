#include "tfalgo.h"
#include "tfcmdmc.h"
#include "tfmat.h"
#ifdef	__debug
#include "tfdebug.h"
extern char LocalDirectory[128];
#endif

void Dyna_Opti( LPCOBJPRIV lpCobj_All_, LPCOBJPRIVMV lpCobj_PrivMV_, LPCOBJPRIVCV lpCobj_PrivCV_ )
{
	int         _i, _j, _k, _Real_i;
	LPMATRIX 	_lpmypredN, _lpmypredP, _lpmW, _lpmWP, _lpmIRV, _lpmIRVM;
	LPMATRIX	_lpmuM, _lpmL, _lpmu, _lpmdelu;
	LPMATRIX 	_lpmmul2, _lpmmul3, _lpmerrcP, _lpmerruM, _lpmdeluM1,_lpmdeluM2;
#ifdef	__debug
	double		_mdeluM1[mm][MM], _mdeluM2[mm][MM], _merruM[mm][MM], _merrcP[pp][PP], _mtemp;
	LPMATRIX	_lpmQ, _lpmP, _lpmR;
	double		_arrayq[pp][PP], _arrayP[mm][MM], _arrayr[mm][MM];
	char		_szBuffer[128];
	FILE		*_DFile;
#endif

	_lpmypredN = MAKE_MATRIX( lpCobj_All_->LPPredN );
	_lpmypredP = MatrixAlloc(lpCobj_All_->p*lpCobj_All_->P, 1);
	_lpmW = MatrixAlloc(lpCobj_All_->p, 1);
	_lpmWP = MatrixAlloc(lpCobj_All_->P*lpCobj_All_->p, 1);
	_lpmerrcP = MatrixAlloc(lpCobj_All_->P*lpCobj_All_->p, 1);
	_lpmIRV = MatrixAlloc(lpCobj_All_->m, 1);
	_lpmuM = MatrixAlloc(lpCobj_All_->m*lpCobj_All_->M, 1);
	_lpmerruM = MatrixAlloc(lpCobj_All_->m*lpCobj_All_->M, 1);
	_lpmIRVM = MatrixAlloc(lpCobj_All_->m*lpCobj_All_->M, 1);
	_lpmdeluM1 = MatrixAlloc(lpCobj_All_->m*lpCobj_All_->M, 1);
	_lpmdeluM2 = MatrixAlloc(lpCobj_All_->m*lpCobj_All_->M, 1);
	_lpmL 	   = MatrixAlloc(lpCobj_All_->m, lpCobj_All_->m*lpCobj_All_->M);
	_lpmu = MatrixAlloc(lpCobj_All_->m, 1);
	_lpmdelu = MatrixAlloc(lpCobj_All_->m,1);

/*	for( _i = 0; _i < lpCobj_All_->p; _i++ )
		for( _j = 0; _j < lpCobj_All_->N; _j++ )
			_mypredN[_i][_j] = MGET( _lpmypredN, (_i*lpCobj_All_->N+_j), 0 );
*/
	for( _i = 0; _i < lpCobj_All_->m; _i++ )
		MGET( _lpmu, _i, 0 ) = PRIVMVGET( lpCobj_PrivMV_, _i, 0 )->MSample;

	for(_i = 0; _i < lpCobj_All_->m; _i++)
		for(_j = 0; _j < lpCobj_All_->M; _j++)
			MGET(_lpmuM, (_i*lpCobj_All_->M+_j), 0) = MGET(_lpmu, _i, 0);


	for(_i = 0; _i < lpCobj_All_->p*lpCobj_All_->P; _i++)
	{
		_Real_i = _i / lpCobj_All_->P;
		MGET(_lpmypredP, _i, 0) = MGET(_lpmypredN, (_i/lpCobj_All_->P*lpCobj_All_->N+_i-_i/lpCobj_All_->P*lpCobj_All_->P + 
			PRIVCVGET( lpCobj_PrivCV_, _Real_i, 0 )->CN ), 0);
	}

	for(_i = 0; _i < lpCobj_All_->p; _i++) 
		MGET(_lpmW, _i, 0 ) = PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CSetpoint;

	for(_i = 0; _i < lpCobj_All_->p; _i++)
		for(_k = 0; _k < lpCobj_All_->P; _k++)
			MGET(_lpmWP, (_i*lpCobj_All_->P+_k), 0) = MGET(_lpmW, _i, 0);

	mminus(_lpmerrcP, _lpmWP, _lpmypredP);
	for( _i = 0; _i < lpCobj_All_->m; _i++ )
		MGET( _lpmIRV, _i, 0 ) = PRIVMVGET( lpCobj_PrivMV_, _i, 0 )->MIrv;

	for(_i = 0; _i < lpCobj_All_->m; _i++)
		for(_j = 0; _j < lpCobj_All_->M; _j++)
			MGET(_lpmIRVM, (_i*lpCobj_All_->M+_j), 0) = MGET(_lpmIRV, _i, 0);

	mminus(_lpmerruM, _lpmIRVM, _lpmuM);
	_lpmmul2 = MAKE_MATRIX( lpCobj_All_->LPMul2 );
	_lpmmul3 = MAKE_MATRIX( lpCobj_All_->LPMul3 );
	mmultiple(_lpmdeluM1, _lpmmul2, _lpmerrcP);
	mmultiple(_lpmdeluM2, _lpmmul3, _lpmerruM);
	for(_i = 0; _i < lpCobj_All_->m; _i++)
	{
		if( PRIVMVGET( lpCobj_PrivMV_, _i, 0 )->MSatuation == 1  )
		{
			for(_j = 0; _j < lpCobj_All_->M; _j++)
			{
				MGET(_lpmdeluM1, (_i * lpCobj_All_->M + _j), 0) = 0.0;
				MGET(_lpmdeluM2, (_i * lpCobj_All_->M + _j), 0) = 0.0;
			}
		}
	}
	madd(( COBJ_2_MATRIX( lpCobj_All_->LPDeluM ) ), _lpmdeluM1, _lpmdeluM2);

	for(_i = 0; _i < lpCobj_All_->m; _i++)
		MGET( _lpmL, _i, _i*lpCobj_All_->M ) = 1.0;

	mmultiple( _lpmdelu, _lpmL, ( COBJ_2_MATRIX( lpCobj_All_->LPDeluM ) ));

#ifdef __debug
	for(_i = 0; _i < lpCobj_All_->m; _i++)
		for(_j = 0; _j < lpCobj_All_->M; _j++)
		{
			_merruM[_i][_j] = MGET(_lpmerruM, (_i * lpCobj_All_->M + _j), 0);
			_mdeluM1[_i][_j] = MGET(_lpmdeluM1, (_i * lpCobj_All_->M + _j), 0);
			_mdeluM2[_i][_j] = MGET(_lpmdeluM2, (_i * lpCobj_All_->M + _j), 0);
		}
	for(_i = 0; _i < lpCobj_All_->p; _i++)
		for(_j = 0; _j < lpCobj_All_->P; _j++)
			_merrcP[_i][_j] = MGET( _lpmerrcP, (_i * lpCobj_All_->P + _j), 0 );
	_mtemp = MGET( ( COBJ_2_MATRIX( lpCobj_All_->LPDeluM ) ), 0, 0 );
#endif

#ifdef __debug
	_lpmP 		= MAKE_MATRIX( lpCobj_All_->LPS );
	_lpmQ 		= MAKE_MATRIX( lpCobj_All_->LPQ );
	_lpmR 		= MAKE_MATRIX( lpCobj_All_->LPR );
	for( _i = 0; _i < lpCobj_All_->p; _i++ )
		for (_j = 0; _j < lpCobj_All_->P; _j++)
			_arrayq[_i][_j] = MGET( _lpmQ, (_i*lpCobj_All_->P+_j), (_i*lpCobj_All_->P+_j) );
	for( _i = 0; _i < lpCobj_All_->m; _i++ )
		for (_j = 0; _j < lpCobj_All_->M; _j++)
			_arrayr[_i][_j] = MGET( _lpmR, (_i*lpCobj_All_->M+_j), (_i*(lpCobj_All_->M)+_j) );

	for( _i = 0; _i < lpCobj_All_->m; _i++ )
		for (_j = 0; _j < lpCobj_All_->M; _j++)
		_arrayP[_i][_j] = MGET( _lpmP, (_i*lpCobj_All_->M+_j), (_i*lpCobj_All_->M+_j) );
	sprintf( _szBuffer, "%s%s", LocalDirectory, "tfDyn.1" );
	if ((_DFile = fopen( _szBuffer, "w"))==NULL)
		MessageBox( NULL, "No Dynamic file", "tfdyopti.c", MB_OK );

	for( _i = 0; _i < lpCobj_All_->m * lpCobj_All_->M; _i++ )
		for( _j = 0; _j < lpCobj_All_->p * lpCobj_All_->P; _j++ )
		{
			fprintf( _DFile, "%f\n", MGET( _lpmmul2, _i, _j ) );
		}
	fclose( _DFile );
	sprintf( _szBuffer, "%s%s", LocalDirectory, "tfDyn.2" );
	if ((_DFile = fopen( _szBuffer, "w"))==NULL)
		MessageBox( NULL, "No Dynamic file", "tfdyopti.c", MB_OK );

	for( _i = 0; _i < lpCobj_All_->m * lpCobj_All_->M; _i++ )
		for( _j = 0; _j < lpCobj_All_->m * lpCobj_All_->M; _j++ )
		{
			fprintf( _DFile, "%f\n", MGET( _lpmmul3, _i, _j ) );
		}
	fclose( _DFile );
#endif
	MATRIXFREE(_lpmypredP);
	MATRIXFREE( _lpmW );
	MATRIXFREE(_lpmu);
	MATRIXFREE(_lpmdelu);
	MATRIXFREE(_lpmdeluM1);
	MATRIXFREE(_lpmdeluM2);
	MATRIXFREE(_lpmuM);
	MATRIXFREE(_lpmerrcP);
	MATRIXFREE(_lpmWP);
	MATRIXFREE(_lpmerruM);
	MATRIXFREE(_lpmL);
	MATRIXFREE(_lpmIRVM);
	MATRIXFREE(_lpmIRV);


	return;
}
