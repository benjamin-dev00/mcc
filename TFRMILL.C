#include "tfalgo.h"
#include "tfcmdmc.h"
#include "tfcond.h"

//note: if there are 2 ill-condioned cv, then....?????!!!
void RemoveIll( LPCOBJPRIV lpCobj_All_, LPCOBJPRIVMV lpCobj_PrivMV_, LPCOBJPRIVCV lpCobj_PrivCV_ )
{
	int 		_i, _j, _row = 0, _col = 0, _kk, _num, _addrow, _addcol;
	double		_Cond_Num, _eigmax, _eigmin, _temp;
	WORD 		_flag_ill = 1;
	LPMATRIX	_lpmu, _lpmv, _lpmG, _lpmGtemp;
	LPCOBJ		_lpCobjG;
	LPDIMMATRIX	_lpmSR;
	BOOL		_bOK = TRUE;
/*#ifdef __debug
#include "tfdebug.h"
	double			_mG[pp][mm];
	FILE * _fin;
#endif*/
//	double		_mG[pp][mm];

// if CV's control_type is 3, it may be changed to 2,
// for avoiding the vibration, it should be considered
// into Matrix G;
	for( _i = 0; _i < lpCobj_All_->p; _i++ )
	{
		if( PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CControl_type > 4 )
			MessageBox( NULL, "ERROR", "CONTROL TYPE", MB_OK );
		if( PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CControl_type != 4 &&
				( PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CHOver == 1 ||
					PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CLOver == 1 ||
						PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CDelayCount != 0 ) )
			_row++;
	}
	for( _j = 0; _j < lpCobj_All_->m; _j++ )
		if( PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MControl_type != 4 &&
				PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MControl_type != 5 &&
					PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MControl_type != 7 )
			_col++;

	if( _row <= 1 || _col <= 1 )
		return;

	_lpCobjG = CobjCopy( lpCobj_All_->LPG, &_bOK );
	_lpmG = COBJ_2_MATRIX( _lpCobjG );
	_lpmSR = MAKE_DIMMATRIX( lpCobj_All_->LPSR );
	for ( _i = 0; _i < lpCobj_All_->p; _i++)
		for ( _j = 0; _j < lpCobj_All_->m; _j++)
			MGET( _lpmG, _i, _j ) = DIMMGET( _lpmSR, _i, _j, (lpCobj_All_->N-1) );
	
/*#ifdef __debug
	if ((_fin=fopen( "C:\\MATLAB\\MCC\\tfing.txt", "w"))==NULL)
	{
		MessageBox( NULL, "cannot open stp file\n", "", MB_OK );
		return ;
	}
	for ( _i = 0; _i < lpCobj_All_->p; _i++)
	{
		for ( _j = 0; _j < lpCobj_All_->m; _j++)
		{
			_mG[_i][_j] = MGET( _lpmG, _i, _j );
			fprintf( _fin, "%f%s", (float)(_mG[_i][_j]), " " );
		}
		fprintf( _fin, "%s\n", ";" );
	}
	fclose( _fin );
#endif
*/	while( _flag_ill == 1 )
	{
		if( _row <= 1 || _col <= 1 )
			goto __return ;

		_num = min( _row, _col );
		_lpmGtemp = MatrixAlloc( _row, _col );
		_addrow	= 0;
		for( _i = 0; _i < _lpmG->row; _i++ )
		{
			if( PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CControl_type == 4 ||
					( PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CHOver == 0 &&
						PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CLOver == 0 &&
							PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CDelayCount == 0 ) ||
								PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CNoIll == 1 )
				_addrow++;
			else
			{
				_addcol = 0;
				for( _j = 0; _j < _lpmG->col; _j++ )
				{
					if(  PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MControl_type == 4 ||
						PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MControl_type == 5 ||
						PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MControl_type == 7 )
						_addcol++;
					else
						MGET( _lpmGtemp , _i-_addrow, _j-_addcol ) =
								MGET( _lpmG, _i, _j );
				}
			}
		}
		_lpmu = MatrixAlloc( _row, _row );
		_lpmv = MatrixAlloc( _col, _col );
/*		for( _i = 0; _i < _row; _i++ )
			for( _j = 0; _j < _col; _j++ )
				_mG[_i][_j] = MGET( _lpmGtemp, _i, _j );
 */
		svd( _lpmGtemp, _lpmu, _lpmv,  eps_svd );
		_eigmax = -10.0;
		_eigmin = lpCobj_All_->Big_Cond*100;
		for( _i = 0; _i < _num; _i++ )
			_eigmax = max( _eigmax, ( fabs( MGET( _lpmGtemp, _i, _i ) ) ) );

		for( _i = 0; _i < _num; _i++ )
//		_temp = MGET( _lpmbc, _i, 0 );
			_eigmin = min( _eigmin, ( fabs( MGET( _lpmGtemp, _i, _i ) ) ) );

		if( fabs(_eigmin) <= eps_cond )
			_Cond_Num = 2 * lpCobj_All_->Big_Cond;
		else
			_Cond_Num = _eigmax / _eigmin;

		MATRIXFREE(	_lpmGtemp );

		if( _Cond_Num >= lpCobj_All_->Big_Cond )
			_flag_ill = 1;
		else
		{
			MATRIXFREE( _lpmu );
			MATRIXFREE( _lpmv );
			goto __return ;
		}

//here ill-conditioned CV is sorted by its effecion
		if( _row <= _col )
		{
//			double	_mtemp[2][2];
			LPMATRIX _lpmutemp;

			_lpmutemp = MatrixAlloc( _lpmu->row, 1 );
			for( _i = 0; _i < _lpmu->row; _i++ )
				MGET( _lpmutemp, _i, 0 ) = MGET( _lpmu, _i, _num-1 );
			_addrow = 0;
			for( _i = 0; _i < _lpmG->row; _i++ )
			{
				if( PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CControl_type ==4 ||
					( PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CHOver == 0 &&
						PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CLOver == 0 &&
							PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CDelayCount == 0 ) ||
								PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CNoIll == 1 )
					_addrow++;
/*				else
					 if ( PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CNoIll == 1 )
						MGET( _lpmutemp, _i-_addrow, 0 ) = 0.0;*/
			}
			_temp = fabs( MGET( _lpmutemp, 0, 0 ) );
			for( _i = 0; _i < _row; _i++ )
			  if( fabs( MGET( _lpmutemp, _i, 0 ) ) >= _temp )
				 _kk = _i;
/*			for( _i = 0; _i < 2; _i++ )
				for( _j = 0; _j < 2; _j++ )
					_mtemp[_i][_j] = MGET( _lpmu, _i, _j );
*/			_addrow	= 0;
			_j = 0;
			for( _i = 0; _i < _lpmG->row; _i++ )
			{
				if( PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CControl_type ==4 ||
					( PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CHOver == 0 &&
						PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CLOver == 0 &&
							PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CDelayCount == 0 ) ||
								PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CNoIll == 1 )
					_addrow++;
				else
				{
					if( _kk == _j )
					{
						PRIVCVGET( lpCobj_PrivCV_, (_addrow+_kk), 0 )->CControl_type = 3;
						PRIVCVGET( lpCobj_PrivCV_, (_addrow+_kk), 0 )->CHOver = 0;
						PRIVCVGET( lpCobj_PrivCV_, (_addrow+_kk), 0 )->CLOver = 0;
						PRIVCVGET( lpCobj_PrivCV_, (_addrow+_kk), 0 )->CNoIll = 1;
						PRIVCVGET( lpCobj_PrivCV_, (_addrow+_kk), 0 )->CDelayCount = lpCobj_All_->MaxDelay;
//						lpPrivCV_[_addrow+_kk]->CDirty = 1;
						lpCobj_All_->Freecount++;
						_row--;
					 }
					 _j++;
				}
			}
            MATRIXFREE( _lpmutemp );
		}
		else
		{
			_temp = fabs( MGET( _lpmv, _num-1, 0 ) );
			for( _j = 0; _j < _col; _j++ )
				if( fabs( MGET( _lpmv, _num-1, _j ) ) >= _temp )
					_kk = _j;

			_addcol = 0;
			_i = 0;
			for( _j = 0; _j < _lpmG->col; _j++ )
					if(  PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MControl_type == 4 ||
						PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MControl_type == 5 ||
						PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MControl_type == 7 )
						_addcol++;
					else
					{
						if( _kk == _i )
						{
							PRIVMVGET( lpCobj_PrivMV_, (_addcol+_kk), 0 )->MControl_type = 5;
//							lpPrivMV_[_addcol+_kk]->MPDirty = 1;
							lpCobj_All_->Freecount--;
		                     _col--;
						}
                  _i++;
				}
		}
		MATRIXFREE( _lpmu );
		MATRIXFREE( _lpmv );
	}
__return:
	CobjFree( _lpCobjG );
	return;
}
//http://202.120.145.100/leowang/myself.htm
