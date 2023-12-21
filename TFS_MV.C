#include "tfalgo.h"
#include "tfcmdmc.h"
// The priority would be sorted by increase order in this function 
ACRET MVPrioritySort( LPCOBJPRIV lpCobj_All_, LPCOBJPRIVMV lpCobj_PrivMV_ )
{
	int			_j, _kk, _tempPrio, _l;
	LPMATRIX	_lpmPriority;

	_lpmPriority = MatrixAlloc( lpCobj_All_->m, 1 );
	for( _j = 0; _j < lpCobj_All_->m; _j++ )
		MGET( _lpmPriority, _j, 0 ) = PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MPriority;

	for( _l = 0; _l < lpCobj_All_->m; _l++ )
	{
		_tempPrio = (int)Big_Priority;
		for( _j = 0; _j < lpCobj_All_->m; _j++ )
		{
			if( ( int )MGET( _lpmPriority, _j, 0 ) <= _tempPrio && ( int )MGET( _lpmPriority, _j, 0 ) >= 0 )
			{
				_kk = _j;
				_tempPrio = ( int )MGET( _lpmPriority, _j, 0 );
			}
		}
		MGET( _lpmPriority, _kk, 0 ) = -lpCobj_All_->m + _l;
		MGET( ( COBJ_2_MATRIX( lpCobj_All_->LPMVSort ) ), _l, 0 ) = _kk;
	}
	for( _l = 0; _l < lpCobj_All_->m; _l++ )
	{
		MGET( _lpmPriority, _l, 0 ) +=  lpCobj_All_->m;
		PRIVMVGET( lpCobj_PrivMV_, _l, 0 )->MFault_Priority = ( int )MGET( _lpmPriority, _l, 0 );
	}
	MATRIXFREE( _lpmPriority );
	return	TRUE;
}
