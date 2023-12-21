#include "tfalgo.h"
#include "tfcmdmc.h"
#include "tfcond.h"
#ifdef __ONSPEC
#include "Mtmfun.h"
#include "Mtdllfun.h"
#endif
#ifdef __debug		//Outlook: __debug 使用14*17模型
#include "tfdebug.h"
#endif
extern char LocalDirectory[128];
// Here Disturbance is considered to be constant
void IsSteady( LPCOBJPRIV lpCobj_All_, LPCOBJPRIVCV lpCobj_PrivCV_, LPCOBJPRIVMV lpCobj_PrivMV_, LPCOBJPRIVDV lpCobj_PrivDV_ )
{
	LPMATRIX		_lpmIsSteady;
	WORD			_CountSteady;
	double			_miny = 0.0, _maxy = 0.0, _swap, _eigmax, _eigmin, _Cond_Num;
	double			_Amply;
	WORD			_i, _p, _j, _k, _num; 
	BOOL			_Flag_Found;
	LPMATRIX		_lpmSteady, _lpmA, _lpmB, _lpmX, _lpmGTemp;
	LPMATRIX		_lpmu, _lpmv, _lpmSteadyTemp;
	LPDIMMATRIX		_lpmSR, _lpmDSR;
#ifdef __debug
	double			_mA[mm+ndnd][mm+ndnd], _mB[mm+ndnd];
#endif
	FILE			*_DFile, *_AmplyFile;
	char			_szFile[128], _sz_i[128];
	double			_temp1, _temp2, _DVTemp;
	WORD			_No_X;

	_lpmIsSteady = COBJ_2_MATRIX( lpCobj_All_->LPIsSteady );
	_lpmSR = MAKE_DIMMATRIX( lpCobj_All_->LPSR );
	_lpmDSR = MAKE_DIMMATRIX( lpCobj_All_->LPDSR );
	_lpmSteady = COBJ_2_MATRIX( lpCobj_All_->LPSteady );
	_Flag_Found = 0;
	for( _j = 0; _j < lpCobj_All_->m; _j++ )
	{
		_temp1 = fabs( PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MData - 
			PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MSteadOptim );
		_temp2 = PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MHScale / 100000;
		if( _temp1 >= PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MSteadyField )
			_Flag_Found = 1;
	}
	if( lpCobj_All_->Flag_AM >= 2 && _Flag_Found == 0 )
	{
		MGET( _lpmIsSteady, ( _lpmIsSteady->row - 1 ), 0 )++;
	}
	else
	{
		MGET( _lpmIsSteady, ( _lpmIsSteady->row - 1 ), 0 ) = 0;
		goto END;
	}
	_CountSteady = (WORD)MGET( _lpmIsSteady, ( _lpmIsSteady->row - 1 ), 0 );
	_p = lpCobj_All_->p;
	if( _CountSteady <=  lpCobj_All_->SteadyStep )
	{
		for( _i = 0; _i < lpCobj_All_->p; _i++ )
		{
			_swap = PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CData;
			if( _CountSteady == 1 )
			{
				_miny = _swap;
				_maxy = _swap;
				MGET( _lpmIsSteady, _i, 0 ) = _miny;
				MGET( _lpmIsSteady, _i+_p, 0 ) = _maxy;
			}
			if( _CountSteady == 2 )
			{
				_miny = _swap;
				MGET( _lpmIsSteady, _i, 0 ) = _swap;
				_maxy = MGET( _lpmIsSteady, _i+_p, 0 );
				if( _miny > _maxy )
				{
					MGET( _lpmIsSteady, _i, 0 ) = _maxy;
					MGET( _lpmIsSteady, _i+_p, 0 ) = _miny;
				}
			}
			if( _CountSteady > 2 )
			{
				_miny = MGET( _lpmIsSteady, _i, 0 );
				_maxy = MGET( _lpmIsSteady, _i+_p, 0 );
				if( _swap < _miny )
					MGET( _lpmIsSteady, _i, 0 ) = _swap;
				else
					if( _swap > _maxy )
						MGET( _lpmIsSteady, _i+_p, 0 ) = _swap;
			}
			_swap = _maxy - _miny;
			PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CSteadyDelt = _swap * 
				( PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CHScale - 
					PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CLScale );
		}
	}
	if( lpCobj_All_->nd )
	{	for( _k = 0; _k < lpCobj_All_->nd; _k++ )
		{
			_swap = PRIVDVGET( lpCobj_PrivDV_, _k, 0 )->DData;
			if( lpCobj_All_->Apart_i )
			{
				_swap += PRIVDVGET( lpCobj_PrivDV_, _k, 0 )->DIni;
				UNUNIT( _swap, PRIVDVGET( lpCobj_PrivDV_, _k, 0 )->DHScale,
					PRIVDVGET( lpCobj_PrivDV_, _k, 0 )->DLScale );
			}
			if( _CountSteady == 1 )
			{
				_miny = _swap;
				_maxy = _swap;
				MGET( _lpmIsSteady, _k+3*_p, 0 ) = _miny;
				MGET( _lpmIsSteady, _k+3*_p+lpCobj_All_->nd, 0 ) = _maxy;
			}
			if( _CountSteady == 2 )
			{
				_miny = _swap;
				MGET( _lpmIsSteady, _k+3*_p, 0 ) = _swap;
				_maxy = MGET( _lpmIsSteady, _k+3*_p+lpCobj_All_->nd, 0 );
				if( _miny > _maxy )
				{
					MGET( _lpmIsSteady, _k+3*_p, 0 ) = _maxy;
					MGET( _lpmIsSteady, _k+3*_p+lpCobj_All_->nd, 0 ) = _miny;
				}
			}
			if( _CountSteady > 2 )
			{
				_miny = MGET( _lpmIsSteady, _k+3*_p, 0 );
				_maxy = MGET( _lpmIsSteady, _k+3*_p+lpCobj_All_->nd, 0 );
				if( _swap < _miny )
					MGET( _lpmIsSteady, _k+3*_p, 0 ) = _swap;
				else
					if( _swap > _maxy )
						MGET( _lpmIsSteady, _k+3*_p+lpCobj_All_->nd, 0 ) = _swap;
			}
			_swap = _maxy - _miny;
		}
	}
	if( _CountSteady ==  lpCobj_All_->SteadyStep )
	{
		_Flag_Found = 0;
		for( _i = 0; ( _i < lpCobj_All_->p ) &&
							( _Flag_Found == 0 ); _i++ )
		{
			_miny = MGET( _lpmIsSteady, _i, 0 );
			_maxy = MGET( _lpmIsSteady, _i+_p, 0 );
			_swap = _maxy - _miny;
			if( _swap > PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CSteadyField )
				_Flag_Found = 1;
		}
		for( _k = 0; ( _k < lpCobj_All_->nd ) && ( _Flag_Found == 0 ); _k++ )
		{
			_miny = MGET( _lpmIsSteady, _k+3*_p, 0 );
			_maxy = MGET( _lpmIsSteady, _k+3*_p+lpCobj_All_->nd, 0 );
			_swap = _maxy - _miny;
			if( _swap > PRIVDVGET( lpCobj_PrivDV_, _k, 0 )->DSteadyField )
				_Flag_Found = 1;
		}
		if( _Flag_Found == 0 )
		{
			lpCobj_All_->Flag_CorSteady = 1;
			_CountSteady = 0;
			lpCobj_All_->SteadyCount++;
		}
		else
		{
			lpCobj_All_->Flag_CorSteady = 0;
			_CountSteady = 0;
		}
		MGET( _lpmIsSteady, ( _lpmIsSteady->row - 1 ), 0 ) = _CountSteady;
	}
	_No_X = lpCobj_All_->m + lpCobj_All_->nd;
	if( lpCobj_All_->Flag_CorSteady == 1 && lpCobj_All_->Flag_AM > 1 )
	{
		_lpmSteadyTemp = MatrixAlloc( 1, _No_X );
		for( _j = 0; _j < _No_X; _j++ )
		{
			MGET( _lpmSteadyTemp , 0, _j ) = MGET( _lpmSteady, ( lpCobj_All_->SteadyCount - 1 ) % _No_X, _j );
			if( _j < lpCobj_All_->m )
				MGET( _lpmSteady, ( lpCobj_All_->SteadyCount - 1 ) % 5, _j ) = 
					( PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MSteadOptim - 
						PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MIni );
			else
			{
				_DVTemp = ( PRIVDVGET( lpCobj_PrivDV_, _j-lpCobj_All_->m,  0 )->DData - 
					PRIVDVGET( lpCobj_PrivDV_, _j-lpCobj_All_->m,  0 )->DIni );
				MGET( _lpmSteady, ( lpCobj_All_->SteadyCount - 1 ) % 5, _j ) = _DVTemp;
			}
		}
		if( lpCobj_All_->SteadyCount >= 2 || lpCobj_All_->CalSteady == 1 )
		{
			if( lpCobj_All_->CalSteady == 1 )
			{
				_lpmGTemp = MatrixAlloc( _No_X, _No_X );
				_lpmu = MatrixAlloc( _No_X, _No_X );
				_lpmv = MatrixAlloc( _No_X, _No_X );
				for( _i = 0; _i < _No_X; _i++ )
					for( _j = 0; _j < _No_X; _j++ )
					{
						MGET( _lpmGTemp, _i, _j ) = MGET( _lpmSteady, _i, _j );
					}
			}
			else
			{
				_lpmGTemp = MatrixAlloc( lpCobj_All_->SteadyCount, _No_X );
				_lpmu = MatrixAlloc( lpCobj_All_->SteadyCount, lpCobj_All_->SteadyCount );
				_lpmv = MatrixAlloc( _No_X, _No_X );
				for( _i = 0; _i < lpCobj_All_->SteadyCount; _i++ )
					for( _j = 0; _j < _No_X; _j++ )
					{
						MGET( _lpmGTemp, _i, _j ) = MGET( _lpmSteady, _i, _j );
					}
				}
			svd( _lpmGTemp, _lpmu, _lpmv,  eps_svd );
			_eigmax = -10.0;
			_eigmin = lpCobj_All_->Big_Cond*100;
			_num = min( _lpmu->row, _lpmv->row );
			for( _i = 0; _i < _num; _i++ )
				_eigmax = max( _eigmax, ( fabs( MGET( _lpmGTemp, _i, _i ) ) ) );

			for( _i = 0; _i < _num; _i++ )
	//		_temp = MGET( _lpmbc, _i, 0 );
				_eigmin = min( _eigmin, ( fabs( MGET( _lpmGTemp, _i, _i ) ) ) );

			if( fabs(_eigmin) <= eps_cond )
				_Cond_Num = 2 * lpCobj_All_->Big_Cond;
			else
				_Cond_Num = _eigmax / _eigmin;

			MATRIXFREE( _lpmGTemp);
			MATRIXFREE( _lpmu );
			MATRIXFREE( _lpmv );
			if( _Cond_Num >= lpCobj_All_->CalSteadyMax )
			{
				for( _j = 0; _j < lpCobj_All_->m; _j++ )
					MGET( _lpmSteady, ( lpCobj_All_->SteadyCount - 1 ) % 5, _j ) = 
						MGET( _lpmSteadyTemp , 0, _j );
				for( _k = 0; _k < lpCobj_All_->nd; _k++ )
					MGET( _lpmSteady, ( lpCobj_All_->SteadyCount - 1 ) % 5, _k + lpCobj_All_->m ) = 
						MGET( _lpmSteadyTemp , 0, lpCobj_All_->m + _k );
				lpCobj_All_->SteadyCount--;
				lpCobj_All_->Flag_CorSteady = 0;
			}
		}
		MATRIXFREE( _lpmSteadyTemp );
	}
	if( lpCobj_All_->Flag_CorSteady == 1 && lpCobj_All_->Flag_AM > 1 )
	{
		for( _i = 0; _i < lpCobj_All_->p; _i++ )
		{
			MGET( _lpmIsSteady, ( _i+2*_p ), 0 ) = lpCobj_All_->Cor_Steady *
				( PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CData - PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CSteadOptim );
			MGET( _lpmSteady, ( ( lpCobj_All_->SteadyCount - 1 ) % 5 ), _i + _No_X ) = 
				MGET( _lpmIsSteady, ( _i+2*_p ), 0 );
		}
		if( !lpCobj_All_->Flag_Alarm )
		{

#ifdef __Simul
#ifdef __debug
/*			if( lpCobj_All_->Apart_i == 0 )
			{
				if( lpCobj_All_->SteadyCount == 1 )
				{
					WRITEMVPARAMETER( lpCobj_All_->LPMV, 1, MV_LOW, REAL, 102.5 );
				}
				if( lpCobj_All_->SteadyCount == 2 )
				{
					WRITEMVPARAMETER( lpCobj_All_->LPMV, 4, MV_LOW, REAL, 221 );
				}
				if( lpCobj_All_->SteadyCount == 3 )
				{
					WRITEMVPARAMETER( lpCobj_All_->LPMV, 3, MV_HIGH, REAL, 34 );
				}
				if( lpCobj_All_->SteadyCount == 4 )
				{
					WRITEMVPARAMETER( lpCobj_All_->LPMV, 2, MV_HIGH, REAL, 12 );
				}
			}
			if( lpCobj_All_->Apart_i == 1 )
			{
				if( lpCobj_All_->SteadyCount == 2 )
				{
					WRITECVPARAMETER( lpCobj_All_->LPCV, 10, CV_LOW, REAL, 91 );
				}
				if( lpCobj_All_->SteadyCount == 1 )
				{
					WRITEDVPARAMETER( lpCobj_All_->LPDV, 0, DV_VALUE, REAL, (REAL)2.55 );
				}
			}*/
	#endif
	#endif
		}
	}
//  Correct the steady gains here
	if( ( lpCobj_All_->SteadyCount == _No_X ||
				lpCobj_All_->CalSteady == 1 ) && 
			lpCobj_All_->Flag_CorSteady == 1 && 
				lpCobj_All_->Flag_AM > 1 )
	{
		_lpmA = MatrixAlloc( _No_X, _No_X );
		_lpmB = MatrixAlloc( _No_X, 1 );
		_lpmX = MatrixAlloc( _No_X, 1 );
		itoa( ( lpCobj_All_->Apart_i + 1 ), _sz_i, 10 );
		sprintf( _szFile, "%s%s%s", LocalDirectory, "tfsteady.da", _sz_i );
		if ((_DFile = fopen( _szFile, "w"))==NULL)
			MessageBox( NULL, "No Debug file", "tfsteady", MB_OK );
		sprintf( _szFile, "%s%s%s", LocalDirectory, "tfsteady.in", _sz_i );
		if ((_AmplyFile = fopen( _szFile, "w"))==NULL)
			MessageBox( NULL, "No Debug file", "tfsteady.ini", MB_OK );
		for( _i = 0; _i < lpCobj_All_->p; _i++ )
		{
			for( _j = 0; _j < _No_X; _j++ )
			{
				for( _k = 0; _k < _No_X; _k++ )
				{
					MGET( _lpmA, _j, _k ) = 
						MGET( _lpmSteady, _j, _k );
				}
				MGET( _lpmB, _j, 0 ) = MGET( _lpmSteady, _j, _No_X + _i );
			}
			
#ifdef __debug
			for( _j = 0; _j < _No_X; _j++ )
			{
				for( _k = 0; _k < _No_X; _k++ )
				{
					_mA[_j][_k] = MGET( _lpmA, _j, _k );
					fprintf( _DFile, "%10f", _mA[_j][_k]);
				}
				fprintf( _DFile, "\n" );
			}
			for( _j = 0; _j < _No_X; _j++ )
			{
				_mB[_j] = MGET( _lpmB, _j, 0 );
				fprintf( _DFile, "%10f", _mB[_j] );
			}
			fprintf( _DFile, "\n");
#endif
			_j = cagaus( _lpmA, _lpmB, _lpmX );					//			A * X = B, get X from this equation
			if( _j )
			{
				for( _j = 0; _j < lpCobj_All_->m; _j++ )
				{
					if( fabs( DIMMGET( _lpmSR, _i, _j, ( lpCobj_All_->N - 1 ) ) ) > 1.0e-7 )
						_Amply = 1 + ( MGET( _lpmX, _j, 0 ) / DIMMGET( _lpmSR, _i, _j, ( lpCobj_All_->N - 1 ) ) );
					else
						_Amply = 1.0;
					fprintf( _AmplyFile, "%10f", _Amply );
				}
				for( _k = 0; _k < lpCobj_All_->nd; _k++ )
				{
					if( fabs( DIMMGET( _lpmDSR, _i, _k, ( lpCobj_All_->N - 1 ) ) ) > 1.0e-7 )
						_Amply = 1 + ( MGET( _lpmX, lpCobj_All_->m+_k, 0 ) / DIMMGET( _lpmDSR, _i, _k, ( lpCobj_All_->N - 1 ) ) );
					else
						_Amply = 1.0;
					fprintf( _AmplyFile, "%10f", _Amply );
				}
				lpCobj_All_->Flag_Steady = 1;
			}
			else
				for( _j = 0; _j < _No_X; _j++ )
				{
					_Amply = 1.0;
					fprintf( _AmplyFile, "%10f", _Amply );
				}
			fprintf( _AmplyFile, "\n" );
		}
		fclose( _DFile );
		fclose( _AmplyFile );
		WRITEALLPARAMETER( lpCobj_All_, MDMCIRVD_STEADY,   BOOL, lpCobj_All_->Flag_Steady );
		MessageBeep( MB_ICONEXCLAMATION  );
        LogError( "Steady", "tfsteady.c" );
		MATRIXFREE( _lpmA );
		MATRIXFREE( _lpmB );
		MATRIXFREE( _lpmX );
		lpCobj_All_->CalSteady = 1;
		CobjKeepCopy( lpCobj_All_->LPTempSR, lpCobj_All_->LPSR );
		if( lpCobj_All_->nd )
			CobjKeepCopy( lpCobj_All_->LPTempDSR, lpCobj_All_->LPDSR );
	}
	
	lpCobj_All_->Flag_CorSteady = 0;
	if( lpCobj_All_->SteadyCount == _No_X )
		lpCobj_All_->SteadyCount = 0;
END:
	return;
}
