#include "tfalgo.h"
#include "tfwsqp.h"
#ifdef __debug
#include "tfdebug.h"
#endif

extern char LocalDirectory[128];
//mv has no field control
void ParameterInitial1( LPCOBJPRIV lpCobj_All_, LPMATRIX lpmNumberOfNM, LPMATRIX lpmG_,
									LPCOBJPRIVMV lpCobj_PrivMV_, LPCOBJPRIVCV lpCobj_PrivCV_ )
{
	int     _n, _me, _mi, _i, _j;
	_n =  lpmG_->row + lpmG_->col;

	_me = lpmG_->row;
//note: Field Control should not be considered as equality equation,
//                              the setpoint is given by  result of LP
	for( _i = 0; _i < lpCobj_All_->p; _i++ )
		if( PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CControl_type == 1 )
			_me++;

	_mi = lpmG_->col;
	for( _j = 0; _j < lpCobj_All_->m; _j++ )
		if( PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MControl_type == 5 ||
				PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MControl_type == 1
				|| PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MControl_type == 7)
		{
			_me++;
			_mi--;
		}

//note: here the limitation of deltu is not considered;
	for( _i = 0; _i < lpCobj_All_->p; _i++ )
		if( PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CControl_type == 2 ||
			PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CControl_type == 3 )
				_mi++;

	_mi = 2 * _mi;

	MGET( lpmNumberOfNM, 0, 0 ) = _n;
	MGET( lpmNumberOfNM, 1, 0 ) = _me;
	MGET( lpmNumberOfNM, 2, 0 ) = _mi;
	return;
}

void ParameterInitial2( LPCOBJPRIV lpCobj_All_, LPMATRIX lpmx_,
						LPMATRIX lpmOC_, LPMATRIX lpmK_, LPMATRIX lpmG_,
						LPCOBJPRIVMV lpCobj_PrivMV_, LPCOBJPRIVCV lpCobj_PrivCV_,
						LPCOBJPRIVDV lpCobj_PrivDV_ )
{
	int             _i, _j, _me, _addrow, _addcol, _mi, _Row_Priv, _Col_Priv;
	int				_setrow, _nsetrow, _k, _imax, _jmax, _l, _tempi, _Apart;
	LPMATRIX    	_lpmxinput, _lpmOCinput, _lpmLPPriv;
	double          _LPDist, _tempHis, _tempNow, _us2, _ys1, _ys2;
	LPDIMMATRIX     _lpDimDSR;
	WRD             _rowenow, _rowinow, _rowetemp;
	BOOL			_Found, _CompIndex;
	LPCOBJPRIV		_lpCobj_Apart;
	LPMATRIX		_lpmCV;
	LPCOBJAPART		_lpApart;
	LPCOBJPRIVDV	_lpApartDV;
	char			_szFile[128], _szLPBuffer[128], _szLPEquation[128], _szLPIndex[128], _szCompIndex[128];
	float			_LPIndex;

#ifdef __debug
	double          _mtemp[80][40], _mtempk[80], _Temp_LP;
	FILE			*_DFile;
	char			_szBuffer[128], _FileName[128], _sz_MVIndex[128];
	LPMATRIX		_lpmG;



	sprintf( _szBuffer, "%s%s", LocalDirectory, "tfDFile.1" );
	if ((_DFile = fopen( _szBuffer, "w"))==NULL)
		MessageBox( NULL, "No Debug file", "tflppara", MB_OK );

#endif
	_lpmLPPriv = COBJ_2_MATRIX(lpCobj_All_->LPLPPriv);
	_lpmxinput = MatrixAlloc( lpCobj_All_->p+lpCobj_All_->m, 1 );
	_lpmOCinput = MatrixAlloc( 1, lpCobj_All_->p+lpCobj_All_->m );

	for( _i = 0; _i < lpCobj_All_->p; _i++ )
	{
		MGET( _lpmxinput, _i, 0 ) = PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CLPIni;
		MGET( _lpmOCinput, 0, _i ) = -PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CLPIndex * 
			( PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CHScale - 
				PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CLScale );
	}
	for( _j = 0; _j < lpCobj_All_->m; _j++ )
	{
		MGET( _lpmxinput, _j+lpCobj_All_->p, 0 ) = PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MLPIni;
		MGET( _lpmOCinput, 0, _j+lpCobj_All_->p ) = -PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MLPIndex * 
			( PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MHScale - 
			  PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MLScale );
	}

	_me = lpmG_->row;
	for( _i = 0; _i < lpCobj_All_->p; _i++ )        
		if( PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CControl_type == 1 )
			_me++;

	_mi = lpmG_->col;
	for( _j = 0; _j < lpCobj_All_->m; _j++ )
		if( PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MControl_type == 5 ||
					PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MControl_type == 1 ||
					PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MControl_type == 7 )
		{
			_me++;
			_mi--;
		}
	for( _i = 0; _i < lpCobj_All_->p; _i++ )
		if( PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CControl_type == 2 ||
			PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CControl_type == 3 )
				_mi++;

	_mi = 2 * _mi;

	MGET( lpmK_, 0, 0 ) = lpCobj_All_->LpK;
	_addrow = 0;
	_addcol = 0;
	_rowinow = _me;
	_setrow = 0;
	_nsetrow = 0;
	_rowenow = 0;
	for( _i = 0; _i < lpCobj_All_->p; _i++ )
		if( PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CControl_type == 4 )
			_addrow++;
		else
		{
			MGET( lpmx_, _i-_addrow, 0 ) = MGET( _lpmxinput, _i, 0 );               //CV: intiatial x;
//	LP object here
			MGET( lpmOC_, 0, _i-_addrow ) = MGET( _lpmOCinput, 0, _i );             //CV: index of SEQ;
//  Model for CV: 1
			MGET( lpmOC_, (_i+1-_addrow), (_i-_addrow) ) = 1;                       //CV: equation of transfer function;
			_addcol = 0;
			_rowenow++;
			for( _j = 0; _j < lpCobj_All_->m; _j++ )
				if( PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MControl_type == 4 )
					_addcol++;
				else
					MGET( lpmOC_, ( _i+1-_addrow ), ( lpmG_->row+_j-_addcol)) =
						-MGET( lpmG_, _i-_addrow, _j-_addcol );               //MV: equation of transfer function;
			_LPDist = 0;
			_Found = 0;
			for( _l = 0; _l < lpCobj_All_->No_Apart && _Found == 0; _l++ )
			{
				_lpCobj_Apart = APARTGET( COBJ_2_APART( lpCobj_All_->LPApart ), _l, 0 );
				_lpmCV = COBJ_2_MATRIX( lpCobj_All_->LPApartCV );
				for( _tempi = 0; _tempi < _lpCobj_Apart->p&& _Found == 0; _tempi++ )
				{
					if( ( MGET( _lpmCV, _l, _tempi ) - 1 ) == _i )
					{
						_Apart = _l;
						_Found = 1;
					}
				}
			}
			_lpApart = COBJ_2_APART( lpCobj_All_->LPApart );
			_lpDimDSR = MAKE_DIMMATRIX( APARTGET( _lpApart, _Apart, 0 )->LPTempDSR );
			for( _k = 0; _k < APARTGET( _lpApart, _Apart, 0 )->nd; _k++ )
			{
				_lpApartDV = COBJ_2_PRIVDV( APARTGET( _lpApart, _Apart, 0 )->LPINDV );
				_LPDist += PRIVDVGET( lpCobj_PrivDV_, _k, 0 )->DData * DIMMGET( _lpDimDSR, _i - 10, _k,  lpCobj_All_->N-1 );
			}
			MGET( lpmK_, ( _i+1-_addrow ), 0 ) = -1 * _LPDist;
								 //DV: equation of transfer function;
// Steady Model would be corrected here
//			MGET( lpmK_, ( _i+1-_addrow ), 0 ) -= MGET( _lpmIsSteady, (_i+2*lpCobj_All_->p), 0 );
			if( PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CControl_type == 1 ) 
			{
				MGET( lpmOC_, (_i+1+lpmG_->row-_nsetrow-_addrow), _i-_addrow ) = 1;
				MGET( lpmK_, ( lpmG_->row + _i+1-_nsetrow-_addrow ), 0 ) =
					-PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CSetpoint;                 //CV: equation of setpoint;
				_setrow++;
				_rowenow++;
			}
			else
			{
				MGET( lpmOC_, (_me+1+2*( _i-_addrow-_setrow )), (_i-_addrow) ) = 1;
				MGET( lpmK_, (_me+1+2*(_i-_addrow-_setrow)), 0 ) = -PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CFault_lsetpoint; //CV: inequation
				MGET( lpmOC_, (_me+1+2*(_i-_addrow-_setrow)+1), (_i-_addrow) ) = -1;
				MGET( lpmK_, (_me+1+2*(_i-_addrow-_setrow)+1), 0 ) = PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CFault_hsetpoint;
				_rowinow = _me+1+2*(_i-_addrow-_setrow)+1;
				_nsetrow++;
			}
		}
	_addcol = 0;
	for( _j = 0; _j < lpCobj_All_->m; _j++ )
		if( PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MControl_type == 4 )
			_addcol++;
		else
			MGET( lpmx_, _j+lpCobj_All_->p-_addrow-_addcol, 0 ) = MGET( _lpmxinput, _j+lpCobj_All_->p, 0 );

	_addrow = 0;
	_addcol = 0;
	_rowetemp = _rowenow;
	for( _j = 0; _j < lpCobj_All_->m; _j++ )
		if( PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MControl_type == 4 )
			_addcol++;
		else
		{
			if( PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MControl_type == 5 ||
					PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MControl_type == 1 ||
					PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MControl_type == 7 )
			{
				_addrow++;
				MGET( lpmOC_, _rowenow+_addrow, lpmG_->row+_j-_addcol ) = 1;
				if( PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MControl_type == 5 ||
						PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MControl_type == 7 )
					MGET( lpmK_, _rowenow+_addrow, 0 ) = -PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MData;        //MV: equation of satuation;
				else
					MGET( lpmK_, _rowenow+_addrow, 0 ) = -PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MIrv; //MV: equation of IRV;
				_rowetemp = _rowenow + _addrow;
			}
		}
	_rowenow = _rowetemp;
//	Now the equation for LP-Plus
	sprintf( _szFile, "%s%s", LocalDirectory, "tflpequt.ini" );
	for( _l = 0; _l < lpCobj_All_->p; _l++ )
	{
		sprintf( _szLPEquation, "%s%d", "EQUATION", _l );
		sprintf( _szCompIndex, "%s", "COMPLP" );
		GetPrivateProfileString( _szLPEquation,
								 _szCompIndex,
								 "0",
								 _szLPBuffer,
								 sizeof(_szLPBuffer),
								_szFile );
		sscanf( _szLPBuffer, "%d", &_CompIndex );
		_ys1 = PRIVCVGET( lpCobj_PrivCV_, _l, 0 )->CHScale - PRIVCVGET( lpCobj_PrivCV_, _l, 0 )->CLScale;
		if( _ys1 <= epsnorm )
		{
			MessageBox( NULL, "ys1 Scale is 0 ", "ERROR IN LPPARA", MB_OK );
			lpCobj_All_->Flag_Run = 0;
			lpCobj_All_->Flag_Alarm = 1;
			LogError( "LP CV Scale", "tflppara.c, line 233" );
			_ys1 = 1.0;
			goto _return;
		}
		if( _CompIndex )
		{
			for( _i = 0; _i < lpCobj_All_->p; _i++ )
			{
				_addrow = 0;
				sprintf( _szLPIndex, "%s%d", "CV", _i );
				GetPrivateProfileString( _szLPEquation,
										 _szLPIndex,
										 "0",
										 _szLPBuffer,
										 sizeof(_szLPBuffer),
										_szFile );
				sscanf( _szLPBuffer, "%f", &_LPIndex );
				if( PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CControl_type == 4 )
				{
					_addrow++;
//				if( _LPIndex != 0 )
						//MessageBox( NULL, "ERROR", "LPINDEX", MB_OK );
				}
				else
				{
					_ys2 = PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CHScale - PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CLScale;
					if( _ys2 <= epsnorm )
					{
						MessageBox( NULL, "ys2 Scale is 0 ", "ERROR IN LPPARA", MB_OK );
						lpCobj_All_->Flag_Run = 0;
						lpCobj_All_->Flag_Alarm = 1;
						LogError( "LP CV Scale i", "tflppara.c, line 264" );	
						_ys2 = 1.0;
						goto _return;
					}
					MGET( lpmOC_, (_l+1), (_i-_addrow) ) -= _LPIndex * _ys2 / _ys1;
				}
			}
			for( _j = 0; _j < lpCobj_All_->m; _j++ )
			{
				_addcol = 0;
				sprintf( _szLPIndex, "%s%d", "MV", _j );
				GetPrivateProfileString( _szLPEquation,
										 _szLPIndex,
										 "0",
										 _szLPBuffer,
										 sizeof(_szLPBuffer),
										_szFile );
				sscanf( _szLPBuffer, "%f", &_LPIndex );
				if( PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MControl_type == 4 )
				{
					_addcol++;
//					if( _LPIndex != 0 )
//						MessageBox( NULL, "ERROR", "LPINDEX", MB_OK );
				}
				else
				{
					_us2 = PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MHScale - PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MLScale;
					MGET( lpmOC_, (_l+1), , lpmG_->row + _j-_addcol ) -= _LPIndex * _us2 / _ys1;
				}
			}
		}
	}
	if( _rowenow != _me )
	{
	//	MessageBox( NULL, "ERROR", "Equality equation", MB_OK );
		lpCobj_All_->Flag_Run = 0;
		lpCobj_All_->Flag_Alarm = 1;
		LogError( "Equality Equation", "tflppara.c, line 301" );

	}
	_rowetemp = _rowinow;
	_addrow = 0;
	 _addcol = 0;
	for( _j = 0; _j < lpCobj_All_->m; _j++ )
		if( PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MControl_type == 4 )
			_addcol++;
		else
		{
//                      lpCobj_All_->m = ( _j + _lpmGTemp->row - _addcol );
//                      _temp = MGET( _lpmOCinput, 0, _j+lpCobj_All_->p );
			MGET( lpmOC_, 0, ( _j + lpmG_->row - _addcol ) ) =
				MGET( _lpmOCinput, 0, _j+lpCobj_All_->p );              //MV: index of SEQ;
//                      _temp = MGET( lpmOC_, 0, ( _j + _lpmGTemp->row - _addcol ) );
			if( PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MControl_type != 5 &&
					PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MControl_type != 1 &&
						PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MControl_type != 7 )
			{
				MGET( lpmOC_, ( _rowinow+ 2 * _addrow + 1 ),
					( _j + lpmG_->row - _addcol ) ) = 1;
				MGET( lpmK_, (_rowinow+2*_addrow+1), 0 )  =
					-PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MIrvLow;
				MGET( lpmOC_, (_rowinow+2*_addrow+2),
					( _j + lpmG_->row - _addcol ) ) = -1;
				MGET( lpmK_, ( _rowinow + 2 * _addrow + 2 ), 0 ) = 
					PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MIrvHigh;
				_rowetemp = _rowinow + 2 * _addrow + 2;
				_addrow++;
			}
		}

	if( _rowetemp != _mi+_me )
	{
		MessageBox( NULL, "ERROR", "inequality equation", MB_OK );
		lpCobj_All_->Flag_Run = 0;
		lpCobj_All_->Flag_Alarm = 1;
	}

#ifdef __debug
	for( _i = 0; _i < lpmOC_->row; _i++ )
	{
		for( _j = 0; _j < lpmOC_->col; _j++ )
		{
			_mtemp[_i][_j] = MGET( lpmOC_, _i, _j );
			fprintf( _DFile, "%3f", _mtemp[_i][_j] );
		}
		fprintf( _DFile, "\n" );
	}
	fclose( _DFile );
	sprintf( _szBuffer, "%s%s", LocalDirectory, "tfDFile.2" );
	if ((_DFile = fopen( _szBuffer, "w"))==NULL)
		MessageBox( NULL, "No Debug file", "tflppara", MB_OK );
	for( _i = 0; _i < lpmK_->row; _i++ )
	{
		_mtempk[_i] = MGET( lpmK_, _i, 0 );
		fprintf( _DFile, "%f\n", _mtempk[_i] );
	}
	fclose( _DFile );
#endif
	_imax = 4 * lpCobj_All_->p + 3 * lpCobj_All_->m + 1;
	_jmax = lpCobj_All_->p + lpCobj_All_->m + 1;
	if( _imax < lpmOC_->row +1 )
	{
		MessageBox( NULL, "ERROR", "LPPRIV", MB_OK );
		lpCobj_All_->Flag_Run = 0;
		lpCobj_All_->Flag_Alarm = 1;
		LogError( "LPPriv","tflppara.c, line 369" );

	}
	if( _jmax < lpmOC_->col +1 )
	{
		MessageBox( NULL, "ERROR", "LPPRIV", MB_OK );
		lpCobj_All_->Flag_Run = 0;
		lpCobj_All_->Flag_Alarm = 1;
		LogError( "LPPriv 2 ", "tflppara.c, line 337" );

	}
	lpCobj_All_->Flag_LPRecount = 0;
	_Row_Priv = (int)MGET( _lpmLPPriv, 0, 0 );
	_Col_Priv = (int)MGET( _lpmLPPriv, 0, 1 );
//	if( ( _Row_Priv != (lpmOC_->row +1) ) ||
//		   ( _Col_Priv != (lpmOC_->col +1) ) );
//			lpCobj_All_->Flag_LPRecount = 1;
//	else
//	{
	_Row_Priv = max( _Row_Priv, lpmOC_->row+1 );
	_Col_Priv = max( _Col_Priv, lpmOC_->col+1 );
	for( _i = 1; _i < _Row_Priv && ( lpCobj_All_->Flag_LPRecount == 0 ); _i++ )
	{
		for( _j = 0; _j < _Col_Priv-1 && ( lpCobj_All_->Flag_LPRecount == 0 ); _j++ )
		{
			_tempHis = MGET( _lpmLPPriv, _i, _j );
			_tempNow = MGET( lpmOC_, _i-1, _j );
			if ( fabs( _tempHis - _tempNow ) > 1.0e-10 )
				lpCobj_All_->Flag_LPRecount = 1;
		}
		_tempHis = MGET( _lpmLPPriv, _i, _Col_Priv-1 );
		_tempNow = MGET( lpmK_, _i-1, 0 );
		if( fabs( _tempHis - _tempNow ) > 1.0e-10 )
			lpCobj_All_->Flag_LPRecount = 1;
	}
//	}
#ifdef __debug
	if( lpCobj_All_->Flag_LPRecount == 1 )
	{
		sprintf( _FileName, "%s%s", LocalDirectory, "tflp.ini" );
		_lpmG = MAKE_MATRIX( lpCobj_All_->LPG );
		for( _j = 0; _j < lpCobj_All_->m; _j++ )
		{
			sprintf( _sz_MVIndex, "%s%d", "MV", _j );
			_Temp_LP = PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MLPIndex;
			for( _i = 0; _i < lpCobj_All_->p; _i++ )
			{
				_Temp_LP += MGET( _lpmG, _i, _j ) * PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CLPIndex;
			}
			sprintf( _szBuffer, "%f", _Temp_LP );
			WritePrivateProfileString( "LPIndex",
						   _sz_MVIndex,
						   _szBuffer,
						   _FileName );
		}
	}
#endif
	MGET( _lpmLPPriv, 0, 0 ) = lpmOC_->row +1;
	MGET( _lpmLPPriv, 0, 1 ) = lpmOC_->col +1;
	for( _i = 0; _i < lpmOC_->row; _i++ )
		for( _j = 0; _j < lpmOC_->col; _j++ )
			MGET( _lpmLPPriv, _i+1, _j ) = MGET( lpmOC_, _i, _j );
	for( _i = 0; _i < lpmK_->row; _i++ )
		MGET( _lpmLPPriv, _i+1, lpmOC_->col ) = MGET( lpmK_, _i, 0 );

_return:

	MATRIXFREE( _lpmxinput );
	MATRIXFREE( _lpmOCinput );
	return ;
}
void gval( int  sm_, LPMATRIX lpmOC_, LPMATRIX lpmx_,
		LPMATRIX lpmogra_, LPMATRIX lpmcgra_ )
{
	int             _n, _i, _j;

	_n = lpmx_->row;
	for( _i = 0; _i < _n; _i++ )
	{
		MGET( lpmogra_, _i, 0 ) = MGET( lpmOC_, 0, _i );
		for( _j = 0; _j < sm_; _j++ )
			MGET( lpmcgra_, _i, _j ) = MGET( lpmOC_, _j+1, _i );
	}
}

