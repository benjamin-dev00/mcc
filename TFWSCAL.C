#include "sqp.h"
float scal( int n, int sm, LPMATRIX lpmg_, LPMATRIX lpmc_, LPMATRIX lpma_,
			LPMATRIX lpmb_, LPMATRIX lpmscl_, LPMATRIX lpmqpindx_)
//	int n,sm,qpindx[1];
//	float g[namax][namax],a[namax][mamax],c[namax],b[mamax],scl[mamax],_oscl;

{
	float _oscl;
	int i,j;
	_oscl = 0.0;
	for(j=0;j<n;j++)
	{
		if(fabs(MGET( lpmc_, j, 0 ))>_oscl)
			_oscl=fabs(MGET( lpmc_, j, 0 ));
	}
	_oscl = _oscl/(float)sm;
	if(_oscl<10.0)
		_oscl=1.0;
	for(j=0;j<n;j++)
	{
		MGET( lpmc_, j, 0 )=MGET( lpmc_, j, 0 )/_oscl;
		for(i=0;i<n;i++)
		{
			MGET( lpmg_, i, j )=MGET( lpmg_, i, j )/_oscl;
		}
	}
	for(i=0;i<sm;i++)
	{
		MGET( lpmscl_, i, 0 )=0.0;
		for(j=0;j<n;j++)
		{
			if(fabs(MGET( lpma_, j, i ))>MGET( lpmscl_, i, 0 ))
				MGET( lpmscl_, i, 0 )=fabs(MGET( lpma_, j, i ));
		}
		if(MGET( lpmscl_, i, 0 )<zero)
		{
			MGET( lpmqpindx_, 0, 0 ) = 2;
			return(_oscl);
		}
		if(MGET( lpmscl_, i, 0 )<10.0)
			MGET( lpmscl_, i, 0 )=1.0;
		for(j=0;j<n;j++)
		{
			MGET( lpma_, j, i )=MGET( lpma_, j, i )/MGET( lpmscl_, i, 0 );
		}
		MGET( lpmb_, i, 0 )=MGET( lpmb_, i, 0 )/MGET( lpmscl_, i, 0 );
	}
	return(_oscl);
}

