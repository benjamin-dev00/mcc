#include "tfalgo.h"
#include "tfcmdmc.h"
#ifdef debug
#include "tfdebug.h"
#endif

ACRET MergeLPG( LPCOBJPRIV lpCobj_All_, LPCOBJPRIVMV lpCobj_PrivMV_, LPCOBJPRIVCV lpCobj_PrivCV_,
				LPCOBJPRIVDV lpCobj_PrivDV_ )
{
	int				_i, _j, _Real_i, _Real_j, _l;
	LPMATRIX 		_lpmG, _lpmGApart;
	LPCOBJAPART		_lpPrivApart;

// here unify StepResponse

	_lpmG = MAKE_MATRIX( lpCobj_All_->LPG );

	for( _l = 0; _l < lpCobj_All_->No_Apart; _l++ )
	{
		_lpPrivApart = COBJ_2_APART( lpCobj_All_->LPApart );
		_lpmGApart = COBJ_2_MATRIX( APARTGET( _lpPrivApart, _l, 0 )->LPG );
		for ( _i = 0; _i < APARTGET( _lpPrivApart, _l, 0 )->p; _i++)
		{
			 _Real_i = (int)MGET( ( COBJ_2_MATRIX( lpCobj_All_->LPApartCV ) ), _l, _i ) - 1;
			for ( _j = 0; _j < APARTGET( _lpPrivApart, _l, 0 )->m; _j++)
			{
				_Real_j = (int)MGET( ( COBJ_2_MATRIX( lpCobj_All_->LPApartMV ) ), _l, _j ) - 1;
				MGET( _lpmG, _Real_i, _Real_j ) = MGET( _lpmGApart, _i, _j );
			}
		}
	}

	return	TRUE;
}
