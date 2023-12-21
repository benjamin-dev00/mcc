#include "prehead.h"

_DLLEXP POINT *  _DLLFUNC ChangePoint( DWORD var_ )
{
	static POINT _sp;
	_sp.x=LOWORD(var_);
	_sp.y=HIWORD(var_);

	return &_sp;
}
