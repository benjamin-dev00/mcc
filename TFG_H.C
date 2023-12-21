#include "tfalgo.h"
#include "tfcmdmc.h"

ACRET GET_H( LPCOBJPRIV lpCobj_All_ )
{
	LPMATRIX	_lpmHerr;
	int			_i, _j;
	BOOL		_Flag_HMode;

	_Flag_HMode = lpCobj_All_->Flag_HMode;
	_lpmHerr = MAKE_MATRIX( lpCobj_All_->LPH );
	if( _Flag_HMode == 0 )
	{
		for ( _i = 0; _i < lpCobj_All_->p*lpCobj_All_->N; _i++)
			for ( _j = 0; _j < lpCobj_All_->p; _j++)
			{
				if( (_i/lpCobj_All_->N) == _j )
					MGET(_lpmHerr, _i, _j) =  lpCobj_All_->Alpha;
				else
					MGET(_lpmHerr, _i, _j) =  0.0;
			}
	}
	else
	{
		for ( _j = 0; _j < lpCobj_All_->p; _j++)
			for ( _i = 0; _i < lpCobj_All_->p*lpCobj_All_->N; _i++)
			{
				MGET(_lpmHerr, _i, _j) =  0.0;
				if( (_i/lpCobj_All_->N) == _j )
					MGET(_lpmHerr, _i, _j) +=  power( lpCobj_All_->Alpha, (_i%lpCobj_All_->N) );
			}
	}
	for ( _j = 0; _j < lpCobj_All_->p; _j++)
		MGET(_lpmHerr, _j*lpCobj_All_->N, _j) =  1.0;

	lpCobj_All_->LPH = MAKE_COBJ( _lpmHerr );
	return TRUE;
}


