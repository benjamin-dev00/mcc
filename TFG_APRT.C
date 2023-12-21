#include "tfalgo.h"
#include "tfcmdmc.h"

ACRET GetApartMCV( LPCOBJPRIV lpCobjGlobalAll_ )
{
	int				_i, _l, _Real_i, _Real_j;
	LPCOBJAPART		_lpPrivApart;
	LPCOBJPRIV		_lpCobjPrivApart;
	LPMATRIX		_lpmCV, _lpmMV;
	LPCOBJPRIVMV	_lpCobjPrivMV, _lpPrivApartMV ;
	LPCOBJPRIVCV	_lpCobjPrivCV, _lpPrivApartCV;
	LPCOBJ			_lpCobjOriginal, _lpCobjDestin;


	_lpmMV = ( COBJ_2_MATRIX ( lpCobjGlobalAll_->LPApartMV ) );
	_lpmCV = ( COBJ_2_MATRIX ( lpCobjGlobalAll_->LPApartCV ) );
	_lpCobjPrivMV = COBJ_2_PRIVMV( lpCobjGlobalAll_->LPINMV );
	_lpCobjPrivCV = COBJ_2_PRIVCV( lpCobjGlobalAll_->LPINCV );
	for( _i = 0; _i < lpCobjGlobalAll_->No_Apart; _i++  )
	{
		_lpPrivApart = COBJ_2_APART( lpCobjGlobalAll_->LPApart );
		_lpCobjPrivApart = APARTGET( _lpPrivApart, _i, 0 );
		for( _l = 0; _l < _lpCobjPrivApart->p; _l++ )
		{
			_Real_i = (int)MGET( _lpmCV, _l, 0 ) - 1;
			_lpCobjOriginal = MAKE_COBJ( PRIVCVGET( _lpCobjPrivCV, _Real_i, 0 ) );
			_lpPrivApartCV = COBJ_2_PRIVCV( _lpCobjPrivApart->LPINCV );
			_lpCobjDestin = MAKE_COBJ( PRIVCVGET( _lpPrivApartCV, _l, 0 ));
			CobjKeepCopy( _lpCobjDestin, _lpCobjOriginal );
		}
		for( _l = 0; _l < _lpCobjPrivApart->m; _l++ )
		{
			_Real_j = (int)MGET( _lpmMV, _l, 0 ) - 1 ;
			_lpCobjOriginal = MAKE_COBJ( PRIVMVGET( _lpCobjPrivMV, _Real_j, 0 ) );
			_lpPrivApartMV = COBJ_2_PRIVMV( _lpCobjPrivApart->LPINMV );
			_lpCobjDestin = MAKE_COBJ( PRIVMVGET( _lpPrivApartMV, _l, 0 ));
			CobjKeepCopy( _lpCobjDestin, _lpCobjOriginal );
		}
	}
	return ACRET_TRUE;
}
