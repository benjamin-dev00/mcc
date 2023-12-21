#include "tfalgo.h"
#include "tfcmdmc.h"

void ForceMV( LPCOBJPRIV lpCobj_All_, LPCOBJPRIVMV lpCobj_PrivMV_ )
{
	int			_j;
	double		_u, _mtemp;
	LPMATRIX	_lpmDeluM;
	BOOL		_Force = 0;
	//BOOL		_Find;
//	LPMATRIX 	_lpmypredtemp, _lpmAb, _lpmdelu, _lpmypredN;


//	_lpmypredN = MAKE_MATRIX( lpCobj_All_->LPPredN );
//	_lpmAb = MAKE_MATRIX(lpCobj_All_->LPAb);
//	_lpmdelu = MatrixAlloc( lpCobj_All_->m, 1 );
	_lpmDeluM = COBJ_2_MATRIX( lpCobj_All_->LPDeluM ) ;
//	_Find = 0;
	for( _j = 0; _j < lpCobj_All_->m; _j++ )
	{
		_Force = 0;
		if( PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MControl_type != 4 )
		{
			_mtemp = MGET( ( COBJ_2_MATRIX( lpCobj_All_->LPDeluM ) ), 0, 0 );
			if( MGET( _lpmDeluM, _j*lpCobj_All_->M, 0 ) > PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MDelmax  ||
					MGET( _lpmDeluM, _j*lpCobj_All_->M, 0 ) < PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MDelmin )
			{
				if( MGET( _lpmDeluM, _j*lpCobj_All_->M, 0 ) > PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MDelmax )
					MGET( _lpmDeluM, _j*lpCobj_All_->M, 0 ) = PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MDelmax;
				else
					MGET( _lpmDeluM, _j*lpCobj_All_->M, 0 ) = PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MDelmin;
				_Force = 1;
			}

			_u = PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MSample + MGET( _lpmDeluM, _j*lpCobj_All_->M, 0 );
			if( _u > PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MMax )
			{
				MGET( _lpmDeluM, _j*lpCobj_All_->M, 0 ) -= ( _u-PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MMax );
				_Force = 1;
			}
			else
				if( _u < PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MMin )
				{
					MGET( _lpmDeluM, _j*lpCobj_All_->M, 0 ) -= ( _u -PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MMin );
					_Force = 1;
				}
		}
/*		if( _Force == 1 && PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MSatuation == 1 )
		{
			MGET( _lpmdelu, _j, 0 ) = MGET( _lpmDeluM, _j * lpCobj_All_->M, 0 );
			_Find = 1;
		}*/
	}
/*	if( _Find == 1 )
	{
		_lpmypredtemp = MatrixAlloc(lpCobj_All_->p*lpCobj_All_->N, 1);
		mmultiple(_lpmypredtemp, _lpmAb, _lpmdelu);
		madd( _lpmypredN, _lpmypredtemp, _lpmypredN );
		MatrixFree( _lpmypredtemp );
	}
	MatrixFree( _lpmdelu );
*/
   return;
}
