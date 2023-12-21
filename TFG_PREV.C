//===================================================================
//
//Zhang 4.22, 1998
//===================================================================

#include "tfalgo.h"
#include "tfcmdmc.h"
#ifdef __ONSPEC
#include "Mtmfun.h"
#include "Mtdllfun.h"
#endif

void GetPrevUD( LPCOBJPRIV lpCobj_All_ )
{
	LPCOBJ			_lpCobj_PredU, _lpCobj_PredD;
	LPMATRIX		_lpmPredU, _lpmPredD;
	
	if( lpCobj_All_->Count == 1  )
	{
		_lpmPredU = MatrixAlloc( lpCobj_All_->m+1, 1 );
		_lpCobj_PredU = MATRIX_2_COBJ( _lpmPredU );
		MGET( _lpmPredU, _lpmPredU->row-1, 0 ) = 0;
		lpCobj_All_->LPPredU = _lpCobj_PredU;
		WRITEALLPARAMETER( lpCobj_All_, MDMCIRVD_LPPredU, LPCOBJ, _lpCobj_PredU );
		if( lpCobj_All_->nd != 0 )
		{
			_lpmPredD = MatrixAlloc( lpCobj_All_->nd, 1 );
			_lpCobj_PredD = MATRIX_2_COBJ( _lpmPredD );
			lpCobj_All_->LPPredD = _lpCobj_PredD;
			WRITEALLPARAMETER( lpCobj_All_, MDMCIRVD_LPPredD, LPCOBJ, _lpCobj_PredD );
		}
	}
	else
	{
		READALLPARAMETER( lpCobj_All_, MDMCIRVD_LPPredU, LPCOBJ, _lpCobj_PredU );
		if( lpCobj_All_->nd != 0 )			
		{
			READALLPARAMETER( lpCobj_All_, MDMCIRVD_LPPredD, LPCOBJ, _lpCobj_PredD );
		}
	}
	return;
}
