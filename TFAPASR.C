#include "tfalgo.h"
#include "tfcmdmc.h"
#ifdef __ONSPEC
#include "mtmfun.h"
#include "mtdllfun.h"
#endif
extern char LocalDirectory[128];
// Here we could get the apart system, and note that COBJ could be saved automatically
// only when the memory allocated is not freed
ACRET ApartSR( LPCOBJPRIV lpCobjPrivAll_ )
{
	
	char			_GlobalFile[128];
	WORD			_NoApart, _i, _temp, _j, _k, _NoMV, _NoCV, _p, _m, _N, _P, _M, _NoDV;
	int				_ii, _jj, _kk, _Real_i, _Real_j, _nd, _No_nd, _Real_k;
	FILE			* fin;
	LPCOBJAPART		_lpPrivApart;
	LPMATRIX		_lpmCV, _lpmMV, _lpmDV;
	LPCOBJ			_lpCobjAll, _lpCobjApartTemp, _lpCobj_ApartSR, _lpCobj_ApartDSR;
	LPCOBJ			_lpCobj_MV, _lpCobj_CV, _lpCobj_DV, _lpCobjSource;
	LPDIMMATRIX		_lpDimSR, _lpDimApartSR, _lpDimApartDSR, _lpDimDSR;
	LPCOBJPRIVMV	_lpCobj_PrivMV, _lpGlobal_PrivMV;
	LPCOBJPRIVCV	_lpCobj_PrivCV, _lpGlobal_PrivCV;
	LPCOBJPRIVDV	_lpCobj_PrivDV, _lpGlobal_PrivDV;
	BOOL			_Okay = TRUE;
	LPCOBJPRIV		_lpCobj_Apart;
	LPMATRIX		_lpmP, _lpmQ, _lpmR, _lpmHerr, _lpmAb, _lpmmul2;
	LPMATRIX		_lpmA, _lpmmul3, _lpmypredN, _lpmG;

	strcpy( _GlobalFile, LocalDirectory );
	strcat( _GlobalFile, "tfparapt.ini" );
	if ((fin=fopen( _GlobalFile, "r"))==NULL)
	{
		MessageBox( NULL, "cannot open this file\n", "tfapasr.c", MB_OK );
		return ACRET_FALSE;
	}
	fscanf( fin, "%5d\n", &_NoApart );
	lpCobjPrivAll_->No_Apart = _NoApart;
	WRITEALLPARAMETER( lpCobjPrivAll_, MDMCIRVD_NOAPART, WORD, _NoApart );
	_lpPrivApart = PrivApartAlloc( _NoApart, 1 );
	_lpCobjAll = PRIV_2_COBJ( lpCobjPrivAll_ );
	for( _i = 0; _i < _NoApart; _i++ )
	{
		_lpCobjApartTemp = TFCobjCopy( _lpCobjAll, &_Okay );
		APARTGET( _lpPrivApart, _i, 0 ) = COBJ_2_PRIV( _lpCobjApartTemp );
	}
	lpCobjPrivAll_->LPApart = APART_2_COBJ( _lpPrivApart );
	WRITEALLPARAMETER( lpCobjPrivAll_, MDMCIRVD_Apart, LPCOBJ, lpCobjPrivAll_->LPApart );
	_lpGlobal_PrivMV = COBJ_2_PRIVMV( lpCobjPrivAll_->LPINMV );
	_lpGlobal_PrivCV = COBJ_2_PRIVCV( lpCobjPrivAll_->LPINCV );
	_lpGlobal_PrivDV = COBJ_2_PRIVDV( lpCobjPrivAll_->LPINDV );
	_lpmMV = MatrixAlloc( lpCobjPrivAll_->No_Apart, lpCobjPrivAll_->m );
	_lpmCV = MatrixAlloc( lpCobjPrivAll_->No_Apart, lpCobjPrivAll_->p );
	if( lpCobjPrivAll_->nd )
		_lpmDV = MatrixAlloc( lpCobjPrivAll_->No_Apart, lpCobjPrivAll_->nd );
	else
		_lpmDV = NULL;
	lpCobjPrivAll_->LPApartMV = MATRIX_2_COBJ(_lpmMV);
	lpCobjPrivAll_->LPApartCV = MATRIX_2_COBJ(_lpmCV);
	lpCobjPrivAll_->LPApartDV = MATRIX_2_COBJ( _lpmDV );
	for( _i = 0; _i < _NoApart; _i++ )
	{
		_j = 0;
		_k = 0;
		_No_nd = 0;
		_NoMV = 0;
		_NoCV = 0;
		_NoDV = 0;
		fscanf( fin, "%5d", &_temp );
		while( _temp != 0 )
		{
			MGET( _lpmMV, _i, _j ) = (double)_temp;
			_j++;
			_NoMV++;
			fscanf( fin, "%5d", &_temp );
		}
		APARTGET( _lpPrivApart, _i, 0 )->m = _NoMV; 
		fscanf( fin, "%5d", &_temp );
		while( _temp != 0 )
		{
			MGET( _lpmCV, _i, _k ) = ( double )_temp;
			fscanf( fin, "%5d", &_temp );
			_k++;
			_NoCV++;
		}
		APARTGET( _lpPrivApart, _i, 0 )->p = _NoCV; 
		fscanf( fin, "%5d", &_temp );
		while( _temp != 0 )
		{
			MGET( _lpmDV, _i, _No_nd ) = ( double )_temp;
			fscanf( fin, "%5d", &_temp );
			_No_nd++;
			_NoDV++;
		}
		APARTGET( _lpPrivApart, _i, 0 )->nd = _NoDV; 
		fscanf( fin, "\n" );
	}
	fclose( fin );
	_lpDimSR = MAKE_DIMMATRIX( lpCobjPrivAll_->LPSR );
	_lpDimDSR = MAKE_DIMMATRIX( lpCobjPrivAll_->LPDSR );
	_N = lpCobjPrivAll_->N;
	for( _i = 0; _i < _NoApart; _i++ )
	{
		_lpCobj_Apart = APARTGET( _lpPrivApart, _i, 0 );
		_p = APARTGET( _lpPrivApart, _i, 0 )->p;
		_m = APARTGET( _lpPrivApart, _i, 0 )->m;
		_nd = APARTGET( _lpPrivApart, _i, 0 )->nd;
		_lpDimApartSR = DimAlloc( _p, _m, _N);
		_lpCobj_ApartSR = MAKE_COBJ( _lpDimApartSR );
		APARTGET( _lpPrivApart, _i, 0 )->LPSR = _lpCobj_ApartSR;
		if( _nd )
		{
			_lpDimApartDSR = DimAlloc( _p, _nd, _N );
			_lpCobj_ApartDSR = MAKE_COBJ( _lpDimApartDSR ); 
			APARTGET( _lpPrivApart, _i, 0 )->LPDSR = _lpCobj_ApartDSR;
		}
		else
			APARTGET( _lpPrivApart, _i, 0 )->LPDSR = NULL;
		for( _ii = 0; _ii < _p; _ii++ )
		{
			_Real_i = (int)MGET( _lpmCV, _i, _ii ) - 1;
			for( _jj = 0; _jj < _m; _jj++ )
			{
				_Real_j = (int)MGET( _lpmMV, _i, _jj ) - 1;
				for( _kk = 0; _kk < _N; _kk++ )
				{
					DIMMGET( _lpDimApartSR, _ii, _jj, _kk ) = 
						DIMMGET( _lpDimSR, _Real_i, _Real_j, _kk );
				}
			}
			if( _nd )
				for( _jj = 0; _jj < _nd; _jj++ )
				{
					_Real_j = (int)MGET( _lpmDV, _i, _jj ) - 1;
					for( _kk = 0; _kk < _N; _kk++ )
					{
						DIMMGET( _lpDimApartDSR, _ii, _jj, _kk ) = 
							DIMMGET( _lpDimDSR, _Real_i, _Real_j, _kk );
					}
				}
		}
		_lpCobj_PrivCV = PrivCVAlloc( _p, 1 );
		_lpCobj_PrivMV = PrivMVAlloc( _m, 1 );
		if( _nd )
			_lpCobj_PrivDV = PrivDVAlloc( _nd, 1 );
		else
			_lpCobj_PrivDV = NULL;
		for( _ii = 0; _ii < _p; _ii++ )
		{
			_Real_i = (int)MGET( _lpmCV, _i, _ii ) - 1;
			_lpCobjSource = MAKE_COBJ( PRIVCVGET( _lpGlobal_PrivCV, _Real_i, 0 ) );
			PRIVCVGET( _lpCobj_PrivCV, _ii, 0 ) = ( LPVARCV )((void far*)&( _lpCobjSource->data[0]));
		}
		for( _j = 0; _j < _m; _j++ )
		{
			_Real_j = (int)MGET( _lpmMV, _i, _j ) - 1;
			_lpCobjSource = MAKE_COBJ( PRIVMVGET( _lpGlobal_PrivMV, _Real_j, 0 ) );
			PRIVMVGET( _lpCobj_PrivMV, _j, 0 ) = ( LPVARMV)((void far*)&( _lpCobjSource->data[0]));
		}
		for( _k = 0; _k < _nd; _k++ )
		{
			_Real_k = (int)MGET( _lpmDV, _i, _k ) - 1;
			_lpCobjSource = MAKE_COBJ( PRIVDVGET( _lpGlobal_PrivDV, _Real_k, 0 ) );
			PRIVDVGET( _lpCobj_PrivDV, _k, 0 ) = ( LPVARDV)((void far*)&( _lpCobjSource->data[0]));
		}
		_lpCobj_MV = PRIVMV_2_COBJ( _lpCobj_PrivMV );
		_lpCobj_CV = PRIVCV_2_COBJ( _lpCobj_PrivCV );
		if( _nd )
			_lpCobj_DV = PRIVCV_2_COBJ( _lpCobj_PrivDV );
		else
			_lpCobj_DV = NULL;
		APARTGET( _lpPrivApart, _i, 0 )->LPINMV = _lpCobj_MV;
		APARTGET( _lpPrivApart, _i, 0 )->LPINCV = _lpCobj_CV;
		APARTGET( _lpPrivApart, _i, 0 )->LPINDV = _lpCobj_DV;
	}
	_lpPrivApart = COBJ_2_APART( lpCobjPrivAll_->LPApart );
	for( _i = 0; _i < lpCobjPrivAll_->No_Apart; _i++ )
	{
		_lpCobj_Apart = APARTGET( _lpPrivApart, _i, 0 );
		_m = _lpCobj_Apart->m;
		_p = _lpCobj_Apart->p;
		_nd = _lpCobj_Apart->nd;
		_M = _lpCobj_Apart->M;
		_P = _lpCobj_Apart->P;
		_N = _lpCobj_Apart->N;
		_lpCobj_Apart->LPMVSort = MATRIX_2_COBJ( MatrixAlloc( _m, 1 ) );
		_lpCobj_Apart->LPCVSort = MATRIX_2_COBJ( MatrixAlloc( _p, 1 ) );
		_lpCobj_Apart->LPDeluM = MATRIX_2_COBJ( MatrixAlloc(_m*_M, 1) );
		_lpCobj_Apart->LPTempSR = TFCobjCopy( _lpCobj_Apart->LPSR, &_Okay );
		if( _lpCobj_Apart->nd )
			_lpCobj_Apart->LPTempDSR = TFCobjCopy( _lpCobj_Apart->LPDSR, &_Okay );
		_lpmP 		  =	MatrixAlloc( _m*_M, _m*_M );
		_lpmQ 		  = MatrixAlloc( _p*_P, _p*_P );
		_lpmR 		  =	MatrixAlloc( _m*_M, _m*_M );
		_lpmHerr 	  = MatrixAlloc( _p*_N, _p );
		_lpmA         = MatrixAlloc(_p*_P, _m*_M);
		_lpmAb   	  = MatrixAlloc(_p*_N, _m);
		_lpmmul3   	  = MatrixAlloc(_M*_m, _M*_m);
		_lpmmul2 	  = MatrixAlloc(_m*_M, _p*_P);
		_lpmypredN 	  = MatrixAlloc(_p*_N, 1);
		_lpmG		  = MatrixAlloc(_p, _m);

		_lpCobj_Apart->LPIsSteady = ( MATRIX_2_COBJ( MatrixAlloc( ( 1+_p*3 + 3 * _nd ), 1 ) ) );
		_lpCobj_Apart->LPSteady = ( MATRIX_2_COBJ( MatrixAlloc( _m+_nd, (_p + _m + _nd ) ) ) );
		_lpCobj_Apart->LPS = MAKE_COBJ( _lpmP );
		_lpCobj_Apart->LPQ = MAKE_COBJ( _lpmQ );
		_lpCobj_Apart->LPR  = MAKE_COBJ( _lpmR );
		_lpCobj_Apart->LPH  = MAKE_COBJ( _lpmHerr );
		_lpCobj_Apart->LPAb = MAKE_COBJ( _lpmAb );
		_lpCobj_Apart->LPA  = MAKE_COBJ( _lpmA );
		_lpCobj_Apart->LPMul2 = MAKE_COBJ( _lpmmul2 );
		_lpCobj_Apart->LPMul3 = MAKE_COBJ( _lpmmul3 );
		_lpCobj_Apart->LPPredN = MAKE_COBJ( _lpmypredN );
		_lpCobj_Apart->LPG = MAKE_COBJ( _lpmG );
		_lpCobj_Apart->LPHistory = TFCobjCopy( PRIV_2_COBJ( lpCobjPrivAll_ ), &_Okay );
		_lpCobj_Apart->Flag_AM = 1;

	//Modified by Chen
	}
	return ACRET_TRUE;
};
