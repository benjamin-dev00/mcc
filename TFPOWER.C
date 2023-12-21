#include "tfalgo.h"
#include "tfcmdmc.h"

double	power( double x_, int n_ )
{
	int _i;
	double	_y;

	_y = 1.0;
	for( _i = 0; _i < n_; _i++ )
		_y *= x_;
	
	return	_y;
}