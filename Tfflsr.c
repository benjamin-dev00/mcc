//addtemp
#include "tfalgo.h"
#include "tfcmdmc.h"
#include "tfm_sr.h"
#include "tflm.h"
#ifdef __ONSPEC
#include "Mtmfun.h"
#include "Mtdllfun.h"
#endif
extern char LocalDirectory[128];
ACRET File_SR( LPCOBJPRIV lpCobj_All_ )
{
	int		    _i, _in, _out,_j, _Order, _k, _Dist, _imax, _Ordernow;
	WORD		_N, _Rank, _end, _No;
	FILE 		*fin;
	LPMATRIX    _lpSR, _lpmNum, _lpmDom,_lpAllNumDom;
	LPDIMMATRIX _lpDimSR, _lpDimDSR;
	LPCOBJ		_lpCobj_SR, _lpCobj_DSR;
	LPMATRIXL 	_lpsNum, _lpsDen, _lpmstepRes;
	double		_up, _Td;
	float		_Ts, _temp;
	char		_Buffer[64], _FileName[128], _Buffer1[128];

	_N = lpCobj_All_->N;
	_No = lpCobj_All_->File_No;
	itoa( _No, _Buffer, 10 );
	strcpy( _FileName, LocalDirectory );
	strcat( _FileName, "tfinput" );
	strcat( _FileName, _Buffer );
	strcat( _FileName, ".ini" );
	if ((fin=fopen( _FileName, "r"))==NULL)
	{
		MessageBox( NULL, "cannot open this file\n", "", MB_OK );
		return ACRET_FALSE;
	}
	fscanf( fin, "%5d", &_in );
	fscanf( fin, "%5d", &_out );
	fscanf( fin, "%5d", &_Dist );
	fscanf( fin, "%5f", &_Ts );
	fscanf( fin, "%5d", &_Rank );
	if( _out == 0 || _in == 0 )
	{
		MessageBox( NULL, "ERROR", "NO MV OR CV IN TFFALSR.C", MB_OK );
		return ACRET_FALSE;
	}
	READALLPARAMETER( lpCobj_All_, MDMCIRVD_mSR,LPCOBJ,_lpCobj_SR );	
	READALLPARAMETER( lpCobj_All_, MDMCIRVD_mDSR,LPCOBJ,_lpCobj_DSR );	

	_lpDimSR = MAKE_DIMMATRIX( _lpCobj_SR );
	_lpDimDSR = MAKE_DIMMATRIX( _lpCobj_DSR );

	_imax = 2 * (_in + _Dist ) * _out;
	_lpAllNumDom = MatrixAlloc( _imax, _Rank+2 );
	for ( _i = 0; _i < _imax; _i++ )
	{
		if( (_i % 2) == 0 )
			_end = _Rank+1;
		else
			_end = _Rank+2;
		for( _j = 0; _j < _end; _j++ )
		{
			fscanf( fin, "%16g", &_temp );
			MGET( _lpAllNumDom, _i, _j ) = _temp;
		}
		fscanf( fin, "\n" );
	}
	fclose(fin);
	_lpsNum = ( LPMATRIXL )malloc( sizeof(char)*mxMAXNAM+sizeof(int)*2+sizeof(double));
	_lpsDen = ( LPMATRIXL )malloc( sizeof(char)*mxMAXNAM+sizeof(int)*2+sizeof(double));
	_lpmstepRes = ( LPMATRIXL )malloc( sizeof(char)*mxMAXNAM+sizeof(int)*2+sizeof(double));
	_lpmDom = MatrixAlloc( _Rank+1, 1 );
	_lpmNum = MatrixAlloc( _Rank+1, 1 );
	_lpsNum->row = 1;
	_lpsNum->col = _Rank + 1;
	_lpmstepRes->row = _N;
	_lpmstepRes->col = 1;
	_lpsDen->row = 1;
	_lpsDen->col = _Rank + 1;
	_lpsNum->pr = _lpmNum->data;
	_lpsDen->pr = _lpmDom->data;
	_lpSR = MatrixAlloc( _N, 1 );
	_lpmstepRes->pr = _lpSR->data;
	for( _Order = 1; _Order <= _imax /2; _Order++ )
	{
		for( _i = 0; _i <= _Rank; _i++ )
		{
			MGET( _lpmNum, _i, 0 ) = MGET( _lpAllNumDom, 2*_Order-1, _i );
			MGET( _lpmDom, _i, 0 ) = MGET( _lpAllNumDom, 2*(_Order-1), _i );
		}
		_Td = MGET( _lpAllNumDom, 2*_Order-1, _Rank+1 );

		_up = 0.0;
		for( _i = 0; _i <= _Rank; _i++ )
			_up += fabs( MGET( _lpmNum, _i, 0 ) );

		if( fabs( _up ) < epssr )
			for( _i = 0; _i < _N; _i++ )
				MGET( _lpSR, _i, 0 ) = 0.0;
		tf2step( _lpsNum, _lpsDen, _Td, _Ts, _N, _lpmstepRes );
		if( _Order <= _in * _out )
			for( _k = 0; _k < _N; _k++ )
				DIMMGET( _lpDimSR, ((_Order-1)/_in), ((_Order-1)%_in), _k ) =
					MGET( _lpSR, _k, 0 );
		else
		{
         	_Ordernow = _Order - _in * _out;
			for( _k = 0; _k < _N; _k++ )
				DIMMGET( _lpDimDSR, ((_Ordernow-1)/_Dist), ((_Ordernow-1)%_Dist), _k ) =
					MGET( _lpSR, _k, 0 );
		}
	}
	for( _i = 0; _i < lpCobj_All_->p; _i++ )
	{
		itoa( ( 11+_i ), _Buffer, 10);
		for( _j = 0; _j < lpCobj_All_->m; _j++ )
		{
			itoa( ( 6+_j ), _Buffer1, 10);
			sprintf( _FileName, "%s%s%s%s%s%s%s", LocalDirectory, "mccstp\\cv", _Buffer, "mv", "0", _Buffer1, ".stp" );
			if ((fin=fopen( _FileName, "w"))==NULL)
			{
				MessageBox( NULL, "cannot open this file\n", "", MB_OK );
				return ACRET_FALSE;
			}
			if( _i == 0 && _j == 0 )
				fprintf( fin, "%f\n", (float)lpCobj_All_->N );
			for( _k = 0; _k < lpCobj_All_->N; _k++ )
				fprintf( fin, "%f\n", DIMMGET( _lpDimSR, _i, _j, _k ) );
			fclose( fin );
		}
		for( _j = 0; _j < lpCobj_All_->nd; _j++ )
		{
			itoa( _j+1, _Buffer1, 10);
			sprintf( _FileName, "%s%s%s%s%s%s%s", LocalDirectory, "mccstp\\cv", _Buffer, "dv", "0", _Buffer1, ".stp" );
			if ((fin=fopen( _FileName, "w"))==NULL)
			{
				MessageBox( NULL, "cannot open this file\n", "", MB_OK );
				return ACRET_FALSE;
			}
			for( _k = 0; _k < lpCobj_All_->N; _k++ )
				fprintf( fin, "%f\n", DIMMGET( _lpDimDSR, _i, _j, _k ) );
			fclose( fin );
		}
	}
	MATRIXFREE( _lpAllNumDom );
	MATRIXFREE( _lpmDom );
	MATRIXFREE( _lpmNum );
	free( _lpsDen );
	free( _lpsNum );
	MATRIXFREE( _lpSR );
	free( _lpmstepRes );
//  SR should be free!
	return ACRET_TRUE;
}
