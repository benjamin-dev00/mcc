#include "tfalgo.h"
#include "tfwsqp.h"
void bwdsub( int n, LPMATRIX lpml_, int trans, LPMATRIX lpmw_ )
//	int n,trans;
//	float l[namax][namax],w[namax];

{
	int i,j;

	if (trans) goto three;
	for(i=(n-1);i>=0;i--)
	{
		for(j=(i+1);j<n;j++)
		{
			MGET( lpmw_, i, 0 )=MGET( lpmw_, i, 0 )-MGET( lpml_, i, j )*MGET( lpmw_, j, 0 );
		}
		MGET( lpmw_, i, 0 )=MGET( lpmw_, i, 0 )/MGET( lpml_, i, i );
	}
	return;

three:
	for(i=(n-1);i>=0;i--)
	{
		for(j=(i+1);j<n;j++)
		{
			MGET( lpmw_, i, 0 )=MGET( lpmw_, i, 0 )-MGET( lpml_, j, i )*MGET( lpmw_, j, 0 );
		}
		MGET( lpmw_, i, 0 )=MGET( lpmw_, i, 0 )/MGET( lpml_, i, i );
	}
}


