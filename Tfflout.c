//addtemp
#include "tfalgo.h"
#include "tfcmdmc.h"
#include "tfm_sr.h"
#include "tflm.h"
#ifdef __ONSPEC
#include "Mtmfun.h"
#include "Mtdllfun.h"
#endif
#ifdef __debug
#include "tfdebug.h"
#endif
//#define		NoDisturb
extern char LocalDirectory[128];

ACRET File_OUT( LPCOBJPRIV lpCobj_All_, LPCOBJPRIVMV lpCobj_PrivMV_,
				LPCOBJPRIVCV lpCobj_PrivCV_, LPCOBJPRIVDV lpCobj_PrivDV_ )
{
	LPMATRIX    	_lpmY, _lpmu0, _lpmPredU, _lpmY0, _lpmd0;
	LPDIMMATRIX		_lpDimSR, _lpDimDSR, _lpHistory, _lpDHistory;
	LPCOBJ			_lpCobj_History, _lpCobj_DHistory, _lpCobj_SR;
	LPCOBJ			_lpCobj_Y, _lpCobj_PreD, _lpCobj_D0;
	int				_row, _col, _axe;
	WORD			_i, _j, _k;
	LPMATRIX    	_lpmu, _lpmdelu;
	LPMATRIX		_lpmDist, _lpmPreDist, _lpmdelDist;
	LPCOBJ			_lpCobj_DSR, _lpCobj_DSRTemp, _lpCobj_SRTemp;
	LPCOBJ			_lpCobj_Dist;
	LPCOBJ			_lpCobj_Y0;
	WORD 			_N;
	BOOL			_bOk = TRUE;
	LPDIMMATRIX		_lpDimSRTemp, _lpDimDSRTemp;
	double			_IndexSRError = 0.0, _IndexDSRError = 0.0;
	LPCOBJPRIV		_lpPrivHistory;
	BOOL			_FS = 0;
#ifdef __debug
	double			_mypredN[pp][NN], _mtemp[pp], _tempSR[6][60];
	FILE			*_DOut;
	char			_szBuffer[128];
	double			_TempScale, _TempIni;
	LPCOBJAPART		_lpCobjApart;
	LPCOBJPRIV		_lpApart;
	int				_Real_i, _Real_j, _l;
	LPMATRIX		_lpmMV;
	LPMATRIX		_lpmCV;
#endif
#ifdef __debug
#ifndef NoDisturb
	double			_mDypredN[pp][NN];
#endif
#endif


#ifdef __Simul
#ifdef __FIX
	_FS = 1;
#endif
#endif
	_lpmPredU = COBJ_2_MATRIX( lpCobj_All_->LPPredU );
	_lpPrivHistory = COBJ_2_PRIV( lpCobj_All_->LPHistory );

	if( !_FS )
	{
		for( _j = 0; _j < lpCobj_All_->m; _j++ )
		{
			READMVPARAMETER( lpCobj_All_->LPMV, _j, MV_SAMPLE, REAL, PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MSample );
		}
		for( _k = 0; _k < lpCobj_All_->nd; _k++ )
		{
			READDVPARAMETER( lpCobj_All_->LPDV, _k, DV_VALUE, REAL, PRIVDVGET( lpCobj_PrivDV_, _k, 0 )->DData );
		}
	}
	_lpCobj_SRTemp = lpCobj_All_->LPOutSR;
	_lpCobj_DSRTemp = lpCobj_All_->LPOutDSR;
	_IndexSRError = lpCobj_All_->SRError;
	_IndexDSRError = lpCobj_All_->DSRError;
	_IndexSRError = 1 - _IndexSRError;
	_IndexDSRError = 1 - _IndexDSRError;
	if( _lpCobj_SRTemp == NULL )
	{
		MessageBox( NULL, "DATA LINK HAS NOT BEEN ESTABLISHED", "ERROR", MB_OK );
		return ACRET_FALSE;
	}
	_lpDimSRTemp= MAKE_DIMMATRIX( _lpCobj_SRTemp );
	_lpCobj_SR = TFCobjCopy( _lpCobj_SRTemp, &_bOk );
	_lpDimSR= MAKE_DIMMATRIX( _lpCobj_SR );
	
	_row = _lpDimSR->row;
	_col = _lpDimSR->col;
	_axe = _lpDimSR->axe;
	if( _axe <= 0 )
	{
		MessageBox( NULL, "error", "wrong N", MB_OK);
		CobjFree( _lpCobj_SR );
		return ACRET_FALSE;
	}
	for( _i = 0; _i < _row; _i++ )
		for( _j = 0; _j < _col; _j++ )
			for( _k = 0; _k < _axe; _k++ )
				DIMMGET( _lpDimSR, _i, _j, _k ) = DIMMGET( _lpDimSRTemp, _i, _j, _k ) * _IndexSRError;
	_lpmu0 = MatrixAlloc( lpCobj_All_->m, 1 );
	_lpmu = MatrixAlloc( lpCobj_All_->m, 1 );
	if( lpCobj_All_->nd != 0 )
	{
		_lpmDist = MatrixAlloc( lpCobj_All_->nd, 1 );
		_lpmd0 = MatrixAlloc( lpCobj_All_->nd, 1 );
		for( _k = 0; _k < lpCobj_All_->nd; _k++ )
			MGET( _lpmd0, _k, 0 ) = PRIVDVGET( lpCobj_PrivDV_, _k, 0 )->DIni;
	}
	_lpmY0 = MatrixAlloc( lpCobj_All_->p, 1 );
	_lpCobj_Y0 = MAKE_COBJ( _lpmY0 );
	for( _j = 0; _j < lpCobj_All_->m; _j++ )
		MGET( _lpmu0, _j, 0 ) = PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MIni;
	_lpCobj_D0 = MAKE_COBJ( _lpmd0 );
	_lpCobj_PreD = lpCobj_All_->LPPredD;
	_lpmPreDist = COBJ_2_MATRIX( lpCobj_All_->LPPredD );
	_lpmY = MatrixAlloc( _row, 1 );
	_lpCobj_Y = MAKE_COBJ( _lpmY );
	if( lpCobj_All_->Count == 2 )
	{
		_lpHistory = DimAlloc( _row, _col, _axe );
		_lpCobj_History = MAKE_COBJ( _lpHistory );
		lpCobj_All_->LPYUHis = _lpCobj_History;
		WRITEALLPARAMETER( lpCobj_All_, MDMCIRVD_YUHis, LPCOBJ, _lpCobj_History );
	}
	else
	{
		READALLPARAMETER( lpCobj_All_, MDMCIRVD_YUHis, LPCOBJ, _lpCobj_History );
		_lpCobj_History = lpCobj_All_->LPYUHis;
		_lpHistory = MAKE_DIMMATRIX( _lpCobj_History );
	}
	if( _lpCobj_DSRTemp != NULL )
	{
		_lpDimDSRTemp = MAKE_DIMMATRIX( _lpCobj_DSRTemp );
		_lpCobj_DSR = TFCobjCopy( _lpCobj_DSRTemp, &_bOk );
		_lpDimDSR = MAKE_DIMMATRIX( _lpCobj_DSR );
		_row = _lpDimDSR->row;
		_col = _lpDimDSR->col;
		_axe = _lpDimDSR->axe;
		for( _i = 0; _i < _row; _i++ )
			for( _j = 0; _j < _col; _j++ )
				for( _k = 0; _k < _axe; _k++ )
					DIMMGET( _lpDimDSR, _i, _j, _k ) = DIMMGET( _lpDimDSRTemp, _i, _j, _k ) * _IndexDSRError;
		if( lpCobj_All_->Count == 2 )
		{
			_lpDHistory = DimAlloc( _row, _col, _axe );
			_lpCobj_DHistory = MAKE_COBJ( _lpDHistory );
			lpCobj_All_->LPYDHis = _lpCobj_DHistory;
			WRITEALLPARAMETER( lpCobj_All_, MDMCIRVD_YDHis, LPCOBJ, _lpCobj_DHistory );
		}
		else
		{
			READALLPARAMETER( lpCobj_All_, MDMCIRVD_YDHis, LPCOBJ, _lpCobj_DHistory );
			_lpCobj_DHistory = lpCobj_All_->LPYDHis;
			_lpDHistory = MAKE_DIMMATRIX( _lpCobj_DHistory );
		}
	}
	else
		_lpCobj_DHistory = NULL;

	for( _i = 0; _i < lpCobj_All_->p; _i++ )
		MGET( _lpmY0, _i, 0 ) = PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CIni;

	if( (int)MGET( _lpmPredU, _lpmPredU->row-1, 0 ) == 0 )
	{
		for( _i = 0; _i < _lpmu0->row; _i++ )
			MGET( _lpmPredU, _i, 0 ) = MGET( _lpmu0, _i, 0 );
	}
	CobjKeepCopy( _lpCobj_Y, _lpCobj_Y0 );
	_row = _lpDimSR->row;
	_col = _lpDimSR->col;
	_N = _lpDimSR->axe;
	_lpmdelu = MatrixAlloc( _col, 1 );
	for( _j = 0; _j < lpCobj_All_->m; _j++ )
    	MGET( _lpmu, _j, 0 ) = PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MSample;
	for( _i = 0; _i < _lpmdelu->row; _i++ )
	{
//		MGET( _lpmdelu, _i, 0 ) = PRIVMVGET( lpCobj_PrivMV_, _i, 0 )->MDelt;
//		MGET( _lpmdelu, _i, 0 ) *= 
//			( PRIVMVGET( lpCobj_PrivMV_, _i, 0 )->MHScale -
//				PRIVMVGET( lpCobj_PrivMV_, _i, 0 )->MLScale );
		MGET( _lpmdelu, _i, 0 ) = MGET( _lpmu, _i, 0 ) -
									MGET( _lpmPredU, _i, 0 );
	}
	//if( _lpPrivHistory->Flag_AM == 2 )
		for( _i = 0; _i < _row; _i++ )
			for( _j = 0; _j < _col; _j++ )
				for( _k = 0; _k < _N; _k++ )
					DIMMGET( _lpHistory, _i, _j, _k ) -=
						( PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CIniError * 
					( PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CHScale - 
						PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CLScale ) / _col );
	for( _i = 0; _i < _row; _i++ )
		for( _j = 0; _j < _col; _j++ )
			for( _k = 0; _k < _N; _k++ )
				DIMMGET( _lpHistory, _i, _j, _k ) += MGET( _lpmdelu, _j, 0 ) *
							DIMMGET( _lpDimSR, _i, _j, _k );
	for( _i = 0; _i < _row; _i++ )
		for( _j = 0; _j < _col; _j++ )
			MGET( _lpmY, _i, 0 ) += DIMMGET( _lpHistory, _i, _j, 0 );
#ifdef __debug
	for( _i = 0; _i < _row; _i++ )
	{
		for( _k = 0; _k < _N; _k++ )
		{
			_mypredN[_i][_k] = 0.0;
			for( _j = 0; _j < _col; _j++ )
				_mypredN[_i][_k] += DIMMGET( _lpHistory, _i, _j, _k );
		}
		_mtemp[_i] = _mypredN[_i][_N-1];
		for( _j = 0; _j < 6; _j++ )
			for( _k = 0; _k < _N; _k++ )
			{
				_tempSR[_j][_k] = DIMMGET( _lpDimSR, _i, _j, _k );
			}
	}
#endif
	for( _i = 0; _i < _row; _i++ )
		for( _j = 0; _j < _col; _j++ )
			for( _k = 0; _k < _N-1; _k++ )
			{
				DIMMGET( _lpHistory, _i, _j, _k ) =
					DIMMGET( _lpHistory, _i, _j, _k+1 );
			}
 	if( _lpCobj_DSRTemp != NULL )
	{
		if( (int)MGET( _lpmPredU, _lpmPredU->row-1, 0 ) == 0 )
		{
			for( _i = 0; _i < _lpmd0->row; _i++ )
				MGET( _lpmPreDist, _i, 0 ) = MGET( _lpmd0, _i, 0 );
		}
		_lpmdelDist = MatrixAlloc( _lpDimDSR->col, 1 );
		for( _k = 0; _k < lpCobj_All_->nd; _k++ )
		{
			READDVPARAMETER( lpCobj_All_->LPDV, _k, DV_VALUE,  REAL, PRIVDVGET( lpCobj_PrivDV_, _k, 0 )->DData );
			MGET( _lpmDist, _k, 0 ) = PRIVDVGET( lpCobj_PrivDV_, _k, 0 )->DData;
		}
		_lpCobj_Dist = MAKE_COBJ( _lpmDist );
		mminus( _lpmdelDist, _lpmDist, _lpmPreDist );
		_row = _lpDimDSR->row;
		_col = _lpDimDSR->col;
		_N = _lpDimDSR->axe;

/*		if( _lpPrivHistory->Flag_AM == 2 )
		for( _i = 0; _i < _row; _i++ )
			for( _j = 0; _j < _col; _j++ )
				for( _k = 0; _k < _N; _k++ )
					DIMMGET( _lpDHistory, _i, _j, _k ) -= 
						( PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CIniError * 
					( PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CHScale - 
						PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CLScale ) / _col );
						
*/		for( _i = 0; _i < _row; _i++ )
			for( _j = 0; _j < _col; _j++ )
				for( _k = 0; _k < _N; _k++ )
					DIMMGET( _lpDHistory, _i, _j, _k ) += MGET( _lpmdelDist, _j, 0 ) *
								DIMMGET( _lpDimDSR, _i, _j, _k );
		for( _i = 0; _i < _row; _i++ )
			for( _j = 0; _j < _col; _j++ )
				MGET( _lpmY, _i, 0 ) += DIMMGET( _lpDHistory, _i, _j, 0 );
		for( _i = 0; _i < _row; _i++ )
			for( _j = 0; _j < _col; _j++ )
				for( _k = 0; _k < _N-1; _k++ )
				{
					DIMMGET( _lpDHistory, _i, _j, _k ) =
						DIMMGET( _lpDHistory, _i, _j, _k+1 );
				}
#ifdef __debug
#ifndef NoDisturb
		for( _i = 10; _i < _row; _i++ )
		{
			for( _k = 0; _k < _N; _k++ )
			{
				_mDypredN[_i][_k] = 0.0;
				for( _j = 0; _j < _col; _j++ )
					_mDypredN[_i][_k] += DIMMGET( _lpDHistory, _i, _j, _k );
			}
		}
#endif
#endif
//		MATRIXFREE( _lpmdelDist );
//		Modified by ttf on Jan. 18, 2000	Not sure.
		CobjKeepCopy( _lpCobj_PreD, _lpCobj_Dist );
	}
	for( _i = 0; _i < _lpmu->row; _i++ )
		MGET( _lpmPredU, _i, 0 ) = MGET( _lpmu, _i, 0 );
	if( lpCobj_All_->nd )
	{
		for( _i = 0; _i < _lpmDist->row; _i++ )
			MGET( _lpmPreDist, _i, 0 ) = MGET( _lpmDist, _i, 0 );
	}
	MGET( _lpmPredU, _lpmPredU->row-1, 0 )++;
	for( _i = 0; _i < _row; _i++ )
	{
        if( ( MGET( _lpmY, _i, 0 ) < 0.0 ) && _i != 11 )
            MGET( _lpmY, _i, 0 ) = 0.0;
		WRITECVPARAMETER( lpCobj_All_->LPCV, _i, CV_VALUE, REAL, (REAL)MGET( _lpmY, _i, 0 ) );
	}
//	for( _j = 0; _j < lpCobj_All_->m; _j++ )
//	{
//		PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MData = PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MSample;
//	}
#ifdef __debug
	if( lpCobj_All_->nd )
	{
		for( _i = 0; _i < _row; _i++ )
		{
			for( _k = 0; _k < _N; _k++ )
			{
				_mypredN[_i][_k] += _mDypredN[_i][_k];
			}
		}
	}
	for( _l = 0; _l < lpCobj_All_->No_Apart; _l++ )
	{
		sprintf( _szBuffer, "%s%s%d%s", LocalDirectory, "tfDout", _l, ".1" );
		if ((_DOut = fopen( _szBuffer, "w"))==NULL)
		{
			if( ( lpCobj_All_->ErrorCode & 0x0100 ) != 1 )
				MessageBox( NULL, "No Debug file", "tfflout", MB_OK );
			MessageBeep( -1 );
			LogError( "Debug File in tfflout", "Lack" );
			lpCobj_All_->ErrorCode = 8;
		}
		_lpCobjApart = COBJ_2_APART( lpCobj_All_->LPApart );
		_lpApart = APARTGET( _lpCobjApart, _l, 0 );
		_lpmMV = COBJ_2_MATRIX( _lpApart->LPApartMV );
		_lpmCV = COBJ_2_MATRIX( _lpApart->LPApartCV );
		for( _j = 0; _j < _lpApart->m; _j++ )
		{
			_Real_j = (int)MGET( _lpmMV, _l, _j ) - 1;
			_TempScale = MGET( _lpmdelu, _Real_j, 0 ) /( PRIVMVGET( lpCobj_PrivMV_, _Real_j, 0 )->MHScale - 
							PRIVMVGET( lpCobj_PrivMV_, _Real_j, 0 )->MLScale );
			fprintf( _DOut, "%5f\n", _TempScale );
		}
		for( _i = 0; _i < _lpApart->p; _i++ )
		{
			_Real_i = (int)MGET( _lpmCV, _l, _i ) - 1;
			_TempIni = (  PRIVCVGET( lpCobj_PrivCV_, _Real_i, 0 )->CIni -
							PRIVCVGET( lpCobj_PrivCV_, _Real_i, 0 )->CLScale ) /
								( PRIVCVGET( lpCobj_PrivCV_, _Real_i, 0 )->CHScale - 
								PRIVCVGET( lpCobj_PrivCV_, _Real_i, 0 )->CLScale );
			_TempScale = UNIT( MGET( _lpmY, _Real_i, 0 ), PRIVCVGET( lpCobj_PrivCV_, _Real_i, 0 )->CHScale,
									PRIVCVGET( lpCobj_PrivCV_, _Real_i, 0 )->CLScale );
			_TempScale -= _TempIni;
//			fprintf( _DOut, "%5f\n", _TempScale );
			for( _k = 0; _k < _N; _k++ )
			{
				_mypredN[_Real_i][_k] += PRIVCVGET( lpCobj_PrivCV_, _Real_i, 0 )->CIni;
				_TempScale = ( _mypredN[_Real_i][_k] - PRIVCVGET( lpCobj_PrivCV_, _Real_i, 0 )->CLScale ) /
								( PRIVCVGET( lpCobj_PrivCV_, _Real_i, 0 )->CHScale - PRIVCVGET( lpCobj_PrivCV_, _Real_i, 0 )->CLScale );
			
				_TempScale -= _TempIni;
//				fprintf( _DOut, "%5f\n", _TempScale );
			}
		}
		fclose( _DOut );
	}
#endif
	MATRIXFREE( _lpmdelu );
	MATRIXFREE( _lpmu0 );
 	MATRIXFREE( _lpmu );
	MATRIXFREE( _lpmY );
	MATRIXFREE( _lpmY0 );
	if( lpCobj_All_->nd != 0 )
	{
		MATRIXFREE( _lpmdelDist );
		MATRIXFREE( _lpmDist );
		MATRIXFREE( _lpmd0 );
		CobjFree( _lpCobj_DSR );
		if( !lpCobj_All_->Flag_Run )
		{
			DIMMATRIXFREE( _lpDHistory );
			lpCobj_All_->LPYDHis = NULL;
		}
	}
	CobjFree( _lpCobj_SR );
	if( !lpCobj_All_->Flag_Run )
	{
		DIMMATRIXFREE( _lpHistory );
		lpCobj_All_->LPYUHis = NULL;
	}

	return ACRET_TRUE;
}
