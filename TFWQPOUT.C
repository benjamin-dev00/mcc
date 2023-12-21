#include "sqp.h"
void qpout(int n, int sm, LPMATRIX lpmg_, LPMATRIX lpmc_, LPMATRIX lpma_,
		LPMATRIX lpmb_, LPMATRIX lpmv_, float oscl, LPMATRIX lpmscl_)
/*	int n,sm;
	float g[namax][namax],a[namax][mamax],c[namax],b[mamax],scl[mamax],
			v[mamax],oscl;
*/
{
	int i,j;
	for(j=0;j<n;j++)
	{
		MGET( lpmc_, j, 0 )=MGET( lpmc_, j, 0 )*oscl;
		for(i=0;i<n;i++)
			MGET( lpmg_, i, j )=MGET( lpmg_, i, j )*oscl;
	}
	for(i=0;i<sm;i++)
	{
		MGET( lpmb_, i, 0 )=MGET( lpmb_, i, 0 )*MGET( lpmscl_, i, 0 );
		for(j=0;j<n;j++)
		{
			MGET( lpma_, j, i )=MGET( lpma_, j, i )*MGET( lpmscl_, i, 0 );
		}
	}
	for(i=0;i<sm;i++)
	{
		MGET( lpmv_, i, 0 ) = MGET( lpmv_, i, 0 ) *
								oscl/MGET( lpmscl_, i, 0 );
	}
}
