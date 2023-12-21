#include "tfalgo.h"
#include "tfcmdmc.h"

void GetLPG( LPCOBJPRIV lpCobj_All_, LPCOBJPRIVMV lpCobj_PrivMV_,
				LPCOBJPRIVCV lpCobj_PrivCV_, LPMATRIX lpmGtemp_ )
//	get the real G Matrix
{
	int			_i, _j, _addrow, _addcol;
	LPMATRIX	_lpmG;

	_addrow	= 0;
	_lpmG = MAKE_MATRIX( lpCobj_All_->LPG );
	for( _i = 0; _i < _lpmG->row; _i++ )
	{
		if( PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CControl_type == 4 )		 
			_addrow++;
		else
		{
			_addcol = 0;
			for( _j = 0; _j < _lpmG->col; _j++ )
			{
				if( PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MControl_type == 4 )
					_addcol++;
				else
					MGET( lpmGtemp_ , _i-_addrow, _j-_addcol ) =
							MGET( _lpmG, _i, _j );
			}
		}
	}
	return ;
}