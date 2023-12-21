#include "tfalgo.h"
#include "tfwsqp.h"
void fwdsub(int n, LPMATRIX lpml_, int trans, LPMATRIX lpmw_ )
//	int n;
//	float l[namax][namax],w[namax];
//	int trans;

{
	int i,j;
//	float 	_l[namax][namax], _w[namax];

/*	for (i=0;i<n;i++)
		_w[i] = MGET( lpmw_, i, 0 );
	for (i = 0;i < n;i++)
		for (j = 0;j < n;j++)
			_l[i][j] = MGET( lpml_, i, j );
*/
	if (trans) goto three;
	for (i=0;i<n;i++)
	{
		for(j=0;j<=(i-1);j++)
		{
			MGET( lpmw_, i, 0 )=MGET( lpmw_, i, 0 )-MGET( lpml_, i, j )*MGET( lpmw_, j, 0 );
		}
		MGET( lpmw_, i, 0 )=MGET( lpmw_, i, 0 )/MGET( lpml_, i, i );
	}
	return;

three:
	for(i=0;i<n;i++)
	{
		for(j=0;j<=(i-1);j++)
		{
			MGET( lpmw_, i, 0 )=MGET( lpmw_, i, 0 )-MGET( lpml_, j, i )*MGET( lpmw_, j, 0 );
		}
//		_temp = MGET( lpml_, i, i );
		MGET( lpmw_, i, 0 )=MGET( lpmw_, i, 0 )/MGET( lpml_, i, i );
	}
}
