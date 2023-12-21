#include "tfalgo.h"
#include "tfcmdmc.h"
ACRET CVPrioritySort( LPCOBJPRIV lpCobj_All_, LPCOBJPRIVCV lpCobj_PrivCV_ )
{
	int 		_i, _k, _l;
	LPMATRIX 	_lpPriority, _lpCVSort;
	double		_tempPrio;

	_lpPriority = MatrixAlloc( lpCobj_All_->p, 1 );
	_lpCVSort = COBJ_2_MATRIX( lpCobj_All_->LPCVSort );
	for( _i = 0; _i < lpCobj_All_->p; _i++ )
		MGET( _lpPriority, _i, 0 ) = PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CPriority;

	for( _l = 0; _l < lpCobj_All_->p; _l++ )
	{
		_tempPrio = Big_Priority;
		for( _i = 0; _i < lpCobj_All_->p; _i++ )
			if( MGET( _lpPriority, _i, 0 ) <= _tempPrio && MGET( _lpPriority, _i, 0 ) >= 0 )
			{
				_k = _i;
				_tempPrio = MGET( _lpPriority, _i, 0 );
			}
		MGET( _lpPriority, _k, 0 ) = - lpCobj_All_->p + _l;
        MGET( ( COBJ_2_MATRIX( lpCobj_All_->LPCVSort ) ), _l, 0 ) = _k;
	}

	for( _l = 0; _l < lpCobj_All_->p; _l++ )
	{
		MGET( _lpPriority, _l, 0 ) +=  lpCobj_All_->p;
		PRIVCVGET( lpCobj_PrivCV_, _l, 0 )->CFault_Priority = (WORD)MGET( _lpPriority, _l, 0 );
    }
	MATRIXFREE( _lpPriority );
	return	TRUE;
}

