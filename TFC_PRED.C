#include "tfalgo.h"
#include "tfcmdmc.h"
#ifdef __debug
#include "tfdebug.h"
#endif

void CorrectPred( LPCOBJPRIV lpCobj_All_, LPCOBJPRIVMV lpCobj_PrivMV_ )
{
	//LPMATRIX 	_lpmypredtemp, _lpmAb;
	LPMATRIX	 _lpmdelu;
	//LPMATRIX	 _lpmypredN;
	int			_j;
#ifdef __debug
	double		_mU, _mSet, _mIni;
#endif

	//_lpmypredN = MAKE_MATRIX( lpCobj_All_->LPPredN );
	//_lpmypredtemp = MatrixAlloc(lpCobj_All_->p*lpCobj_All_->N, 1);
	//_lpmAb = MAKE_MATRIX(lpCobj_All_->LPAb);
	_lpmdelu = MatrixAlloc( lpCobj_All_->m, 1 );

	for( _j = 0; _j < lpCobj_All_->m; _j++ )
		if( PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MControl_type == 8 )
		{
#ifdef __debug
			_mIni = PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MSample;
			_mSet = PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MIrv;
#endif
			MGET( _lpmdelu, _j, 0 ) = MGET( COBJ_2_MATRIX( lpCobj_All_->LPDeluM ), _j*lpCobj_All_->M, 0 );
			PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MSample += MGET( _lpmdelu, _j, 0 );
			if( PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MSample < PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MMin )
			{
				MGET( _lpmdelu, _j, 0 ) = PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MSample - 
											PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MMin;
				MGET( COBJ_2_MATRIX( lpCobj_All_->LPDeluM ), _j*lpCobj_All_->M, 0 ) -=
					MGET( _lpmdelu, _j, 0 );
				MGET( _lpmdelu, _j, 0 ) = MGET( COBJ_2_MATRIX( lpCobj_All_->LPDeluM ), _j*lpCobj_All_->M, 0 );
				PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MSample = PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MMin;
			}
			if( PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MSample > PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MMax )
			{
				MGET( _lpmdelu, _j, 0 ) = PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MSample - 
											PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MMax;
				MGET( COBJ_2_MATRIX( lpCobj_All_->LPDeluM ), _j*lpCobj_All_->M, 0 ) -=
					MGET( _lpmdelu, _j, 0 );
				MGET( _lpmdelu, _j, 0 ) = MGET( COBJ_2_MATRIX( lpCobj_All_->LPDeluM ), _j*lpCobj_All_->M, 0 );
				PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MSample = PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MMax;
			}
#ifdef __debug
			_mU = PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MSample;
			_mU = _mU;
#endif
//			PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MHistory = PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MSample;
//			PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MHistory +=
//				PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MIni;
//			UNUNIT( PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MHistory,
//				PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MHScale,
//					PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MLScale );
			PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MDelt = MGET( _lpmdelu, _j, 0 );
		}
		else
			MGET( _lpmdelu, _j, 0 ) = 0.0;
//	mmultiple(_lpmypredtemp, _lpmAb, _lpmdelu);
//	madd( _lpmypredN, _lpmypredtemp, _lpmypredN );

	MATRIXFREE( _lpmdelu );
//	MATRIXFREE( _lpmypredtemp );
   return;
}
