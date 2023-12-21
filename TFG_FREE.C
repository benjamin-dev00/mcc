#include "tfalgo.h"
#include "tfcmdmc.h"
void GetFreedom( LPCOBJPRIV lpCobj_All_, LPCOBJPRIVMV lpCobj_PrivMV_, LPCOBJPRIVCV lpCobj_PrivCV_ )
{
	int 	 	_i, _j;
	int 		_freecount;

	_freecount = 0;


	for( _j = 0; _j < lpCobj_All_->m; _j++ )
		if( PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MControl_type != 4 &&
			PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MControl_type != 5 &&
				PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MControl_type != 7 &&
					PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MControl_type != 1 )
			_freecount++;

	for( _i = 0; _i < lpCobj_All_->p; _i++ )
	{
		if( PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CControl_type == 1 )
			_freecount--;
		if( PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CControl_type == 2 )
//	if control_type is 2, then the over flag should have been set.
//				if( PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CHOver== 1
//					|| PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CLOver== 1 )
				_freecount--;
	}
	lpCobj_All_->Freecount = _freecount;
	return	;
}

