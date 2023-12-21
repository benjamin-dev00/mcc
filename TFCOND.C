#include "tfalgo.h"
#include "tfcmdmc.h"
#include "tfcond.h"

//note: if there are 2 ill-condioned cv, then....?????!!!
double GetConditionNumber( LPCOBJPRIV lpCobj_All_, LPCOBJPRIVMV lpCobj_PrivMV_, LPCOBJPRIVCV lpCobj_PrivCV_ )
{
	int 		_i, _j, _row = 0, _col = 0, _num, _addrow, _addcol;
	double		_Cond_Num, _eigmax, _eigmin;
	LPMATRIX	_lpmu, _lpmv, _lpmG, _lpmGtemp;
//	double		_mG[pp][mm];


	for( _i = 0; _i < lpCobj_All_->p; _i++ )
		if( PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CControl_type == 1 ||
			PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CControl_type == 2 )
			_row++;

	for( _j = 0; _j < lpCobj_All_->m; _j++ )
		if( PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MControl_type != 4 )
			_col++;

	if( _row <= 1 || _col <= 1 )
		return 1;

	_lpmG = MAKE_MATRIX( lpCobj_All_->LPG );

	_num = min( _row, _col );
	_lpmGtemp = MatrixAlloc( _row, _col );
	_addrow	= 0;
	for( _i = 0; _i < _lpmG->row; _i++ )
	{
		if( PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CControl_type != 1 &&
			PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CControl_type != 2 )
			_addrow++;
		else
		{
			_addcol = 0;
			for( _j = 0; _j < _lpmG->col; _j++ )
			{
				if(  PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MControl_type == 4 )
					_addcol++;
				else
					MGET( _lpmGtemp , _i-_addrow, _j-_addcol ) =
							MGET( _lpmG, _i, _j );
			}
		}
	}
	_lpmu = MatrixAlloc( _row, _row );
	_lpmv = MatrixAlloc( _col, _col );
/*	for( _i = 0; _i < _row; _i++ )
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
	MATRIXFREE( _lpmu );
	MATRIXFREE( _lpmv );
//note:	2 ill_CV is not allowed!!!now.. begin....
	return _Cond_Num;
}
