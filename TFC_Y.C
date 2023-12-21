#include "tfalgo.h"
#include "tfcmdmc.h"
#ifdef __debug
#include "tfdebug.h"
#endif
#define	ErrorLowLimit	1.0e-3
extern char	LocalDirectory[128];

ACRET Correct_y( LPCOBJPRIV lpCobj_All_, LPCOBJPRIVMV lpCobj_PrivMV_,
					LPCOBJPRIVCV lpCobj_PrivCV_, LPCOBJPRIVDV lpCobj_PrivDV_ )
{
	int			_i, _j, _k;
	LPMATRIX    _lpmH, _lpmycor, _lpmerr, _lpmypredN, _lpmS, _lpmypredtemp;
	LPMATRIX	_lpmy, _lpmypred1, _lpmdelu, _lpmAb;
	LPMATRIX	_lpmTempPred, _lpmAD, _lpmDistPred, _lpmdeltD, _lpmDistPredt;
	LPDIMMATRIX	_lpDimDSR, _lpmSR;
	double		_LLimit;
	LPCOBJPRIV	_lpPrivHistory;
#ifdef __debug
	double			_mypredN[pp][NN];
	FILE			*_DOut;
#ifdef __Simul
	FILE			*_DIn;
	float			_temp;
	double			_temp1;
	BOOL			_Flag_Error = FALSE;
	char			_szError[128];
#endif
	char			_szBuffer[128];
#endif

	_lpPrivHistory = COBJ_2_PRIV( lpCobj_All_->LPHistory );
	if( lpCobj_All_->Count <= 1 || lpCobj_All_->Flag_Run == 0 )
	{
		return TRUE;
	}
#ifdef __debug
	sprintf( _szBuffer, "%s%s%d%s", LocalDirectory, "tfDout", lpCobj_All_->Apart_i, ".2" );
	if ((_DOut = fopen( _szBuffer, "w"))==NULL)
	{
		if( ( lpCobj_All_->ErrorCode & 0x0100 ) != 1 )
			MessageBox( NULL, "No Debug file", "tfflout", MB_OK );
		MessageBeep( -1 );
	    LogError( "No Debug File:tfflout.2", "tfc_y.c, 38 " );
		lpCobj_All_->ErrorCode = 8;
	}
#ifdef __Simul
	sprintf( _szBuffer, "%s%s%d%s", LocalDirectory, "tfDout", lpCobj_All_->Apart_i, ".1" );
	if ((_DIn = fopen( _szBuffer, "r"))==NULL)
	{
//		if( ( lpCobj_All_->ErrorCode & 0x0100 ) != 1 )
//			MessageBox( NULL, "No Debug file", "tfflout", MB_OK );
		MessageBeep( -1 );
		lpCobj_All_->ErrorCode = 8;
        LogError( "No Debug File:tffloug.1", "tfc_y.c 49 ");
	}
#endif
#endif

	_lpmSR = MAKE_DIMMATRIX(lpCobj_All_->LPTempSR);
	_lpmAb = MAKE_MATRIX(lpCobj_All_->LPAb);
	for (_i = 0; _i < lpCobj_All_->p * lpCobj_All_->N; _i++)
		for (_j = 0; _j < lpCobj_All_->m; _j++)
			MGET(_lpmAb, _i, _j) = DIMMGET(_lpmSR, _i/lpCobj_All_->N, _j, (_i-_i/lpCobj_All_->N*lpCobj_All_->N));
	_lpDimDSR = MAKE_DIMMATRIX( lpCobj_All_->LPTempDSR );

	_lpmypredN = MAKE_MATRIX( lpCobj_All_->LPPredN );
//	if( lpCobj_All_->Flag_AM == 2 )
//	{
		for( _i = 0; _i < lpCobj_All_->p; _i++ )
			for( _j = 0; _j < lpCobj_All_->N; _j++ )
			{
				MGET( _lpmypredN, ( _i*lpCobj_All_->N+ _j ), 0 ) -= 
					PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CIniError;
			}
//	}
	_lpmypred1 = MatrixAlloc(lpCobj_All_->p, 1);
	_lpmS = LMatrixAlloc( lpCobj_All_->p*lpCobj_All_->N, lpCobj_All_->p*lpCobj_All_->N );
	_lpmypredtemp = MatrixAlloc(lpCobj_All_->p*lpCobj_All_->N, 1);
	_lpmerr = MatrixAlloc(lpCobj_All_->p, 1);
	_lpmycor = MatrixAlloc(lpCobj_All_->p*lpCobj_All_->N, 1);
	_lpmy = MatrixAlloc(lpCobj_All_->p, 1);
	_lpmdelu = MatrixAlloc(lpCobj_All_->m,1);
	if( lpCobj_All_->nd == 0 )
	{
		_lpmDistPred = NULL;
		_lpmAD = NULL;
	}
	else
	{
		_lpmDistPred = MatrixAlloc( lpCobj_All_->p*lpCobj_All_->N, lpCobj_All_->nd );
		_lpmAD = MatrixAlloc( lpCobj_All_->p * lpCobj_All_->N, lpCobj_All_->nd );
	}
	_lpmTempPred = MatrixAlloc( lpCobj_All_->p*lpCobj_All_->N, 1 );

/*	if( flagAM_ == 2 )
	{
		MFPUREAD(_lppu, MDMCIRVD_mSR, COBJ,_lpCobj_SR);
		_lpmSR = MAKE_DIMMATRIX(_lpCobj_SR);
		MFPUREAD(_lppu, MDMCIRVD_mAb, COBJ,lpCobj_All_->LPAb);
		_lpmAb = MAKE_MATRIX(lpCobj_All_->LPAb);

		for (_i = 0; _i < lpCobj_All_->p * lpCobj_All_->N; _i++)
			for (_l = 0; _l < lpCobj_All_->m; _l++)
				MGET(_lpmAb, _i, _l) = DIMMGET(_lpmSR, _i/lpCobj_All_->N, _l, (_i-_i/lpCobj_All_->N*lpCobj_All_->N));
	}
*/
	for(_i = 0; _i < lpCobj_All_->p*lpCobj_All_->N; _i++)
	{
		if(_i < lpCobj_All_->p*lpCobj_All_->N-1)
			LMGET(_lpmS, _i, _i+1) = 1.0;
		for(_j = 0; _j < lpCobj_All_->p; _j++)
		{
			if(_i == (_j+1) * lpCobj_All_->N-1 )
			{
				LMGET(_lpmS, _i, _i) = 1.0;
				if( _i < lpCobj_All_->p*lpCobj_All_->N-1)
					LMGET(_lpmS, _i, _i+1) = 0.0;
			}
		}
	}


	if( lpCobj_All_->nd > 0  )
		for( _i = 0; _i < lpCobj_All_->p; _i++ )
			for( _j = 0; _j < lpCobj_All_->nd; _j++ )
				for( _k = 0; _k < lpCobj_All_->N; _k++ )
					MGET( _lpmAD, _i*lpCobj_All_->N+_k, _j ) =
						DIMMGET( _lpDimDSR, _i, _j, _k );

	if( lpCobj_All_->nd == 0 )
		_lpmdeltD = NULL;
	else
	{
		_lpmdeltD = MatrixAlloc( lpCobj_All_->nd, 1 );
		for( _j = 0; _j < lpCobj_All_->nd; _j++ )
			MGET( _lpmdeltD, _j, 0 ) = PRIVDVGET( lpCobj_PrivDV_, _j, 0 )->DDelt;
		mmultiple( _lpmDistPred, _lpmAD, _lpmdeltD );
	}
	for( _i = 0; _i < lpCobj_All_->p; _i++)
		MGET( _lpmy, _i, 0) = PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CData;

	for( _j = 0; _j < lpCobj_All_->m; _j++)
	{
		MGET(_lpmdelu, _j, 0) = PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MDelt;
//#ifdef __Simul
//		if( PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MSatuation == 1 )
//			MGET(_lpmdelu, _j, 0) = 0.0;
//#endif
	}
	mmultiple(_lpmTempPred, _lpmAb, _lpmdelu);
	_lpmDistPredt = MatrixAlloc( lpCobj_All_->p*lpCobj_All_->N, 1 );
	if( lpCobj_All_->nd > 0 )
	{
		for( _j = 0; _j < lpCobj_All_->nd; _j++ )
		{
			for( _i = 0; _i < lpCobj_All_->p*lpCobj_All_->N; _i++ )
				MGET( _lpmDistPredt, _i, 0 ) = MGET( _lpmDistPred, _i, _j );
			madd( _lpmTempPred, _lpmTempPred, _lpmDistPredt );
		}
	}
#ifdef __debug
	for( _i = 0; _i < lpCobj_All_->p; _i++ )
		for( _j = 0; _j < lpCobj_All_->N; _j++ )
			_mypredN[_i][_j] = MGET( _lpmypredN, _i*lpCobj_All_->N+_j, 0 );
#endif
	madd(_lpmypredN, _lpmTempPred, _lpmypredN);
	for(_j = 0; _j < lpCobj_All_->p; _j++)
		MGET(_lpmypred1, _j, 0) = MGET(_lpmypredN, _j*lpCobj_All_->N, 0);
#ifdef __debug
	for( _j = 0; _j < lpCobj_All_->m; _j++ )
	{
		fprintf( _DOut, "%5f\n", MGET( _lpmdelu, _j, 0 ) );
	}
/*	for( _i = 0; _i < lpCobj_All_->p; _i++ )
	{
		fprintf( _DOut, "%5f\n", MGET( _lpmypred1, _i, 0 ) );
		for( _j = 0; _j < lpCobj_All_->N; _j++ )
		{
			_mypredN[_i][_j] = MGET( _lpmypredN, _i*lpCobj_All_->N+_j, 0 );
			fprintf( _DOut, "%5f\n", _mypredN[_i][_j]);
		}
	}*/
	fclose( _DOut );
#endif
	for( _i = 0; _i < lpCobj_All_->p; _i++ )
	{
		_LLimit = 0.0;
		UNIT( _LLimit, PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CHScale, PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CLScale );
		_LLimit -= PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CIni;
		if( ( MGET( _lpmypred1, _i, 0 ) < _LLimit ) && ( _i != 1 && lpCobj_All_->Apart_i != 1 ) )
				MGET( _lpmypred1, _i, 0 ) = _LLimit;
	}
	mminus(_lpmerr, _lpmy, _lpmypred1);

#ifdef __debug
#ifdef __Simul
/*	for( _j = 0; _j < lpCobj_All_->m; _j++ )
		fscanf( _DIn, "%f\n", &_temp );*/
#endif
#endif
	for( _i = 0; _i < lpCobj_All_->p; _i++ )
	{
		PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CError = MGET( _lpmerr, _i, 0 );
#ifdef __Simul
#ifdef __debug
		_temp1 = fabs( PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CError );
		if( !_Flag_Error )
			if( fabs( PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CError ) > ErrorLowLimit )
			{
				_Flag_Error = TRUE;
				lpCobj_All_->ErrorCode = 4;
				MessageBeep( -1 );
				printf( _szError, "%s%d%s%f", "CV", _i, "Error=", 
					PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CError );
				LogError( "CVModelError", _szError );
			}
#endif
#endif
/*
#ifdef __debug
#ifdef __Simul
		fscanf( _DIn, "%f\n", &_temp );
		for( _k = 0; _k < lpCobj_All_->N; _k++ )
		{
			fscanf( _DIn, "%f\n", &_temp );
//			_tempPredN[_i][_k] = _temp;
			if( !_Flag_Error )
			if( fabs( _temp - MGET( _lpmypredN, _i*lpCobj_All_->N+_k, 0 ) ) > ErrorLowLimit && 
					lpCobj_All_->Flag_AM != 2 )
			{
				_Flag_Error = TRUE;
				lpCobj_All_->ErrorCode = 4;
				MessageBeep( -1 );
	            LogError( "Predictive model", "tfc_y.c 51" );
			}
		}
#endif
#endif*/
	}
#ifdef __debug
#ifdef __Simul
	fclose( _DIn );
#endif
#endif
	_lpmH = MAKE_MATRIX( lpCobj_All_->LPH );
	mmultiple( _lpmycor, _lpmH, _lpmerr);
	madd(_lpmypredtemp, _lpmypredN, _lpmycor);
	Lmmultiple(_lpmypredN, _lpmS,_lpmypredtemp);
#ifdef __debug
	for( _i = 0; _i < lpCobj_All_->p; _i++ )
		for( _j = 0; _j < lpCobj_All_->N; _j++ )
			_mypredN[_i][_j] = MGET( _lpmypredN, _i*lpCobj_All_->N+_j, 0 );
#endif
	for( _j = 0; _j < lpCobj_All_->m; _j++)
	{
		PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MDelt = 0;
	}
	MATRIXFREE(_lpmycor);
	MATRIXFREE(_lpmerr);
	MATRIXFREE(_lpmy);
	MATRIXFREE(_lpmypred1);
	MATRIXFREE(_lpmypredtemp);
	MATRIXFREE( _lpmS );
	MATRIXFREE( _lpmdelu );
	MATRIXFREE( _lpmTempPred );
	if( lpCobj_All_->nd > 0 )
	{
		MATRIXFREE( _lpmDistPred );
		MATRIXFREE( _lpmAD );
		MATRIXFREE( _lpmdeltD );
	}
	MATRIXFREE( _lpmDistPredt );

	return	TRUE;
}
