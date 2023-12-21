#include "tfalgo.h"
#include "tfwsqp.h"
void chodec(int n, LPMATRIX lpmg_, LPMATRIX lpml_, LPMATRIX lpmqpindx_ )
//int n,qpindx[1];
//float g[namax][namax],l[namax][namax];
{
int j,k,i,k2;
for (j=0;j<n;j++)
	{
	MGET( lpml_, j, j )=MGET( lpmg_, j, j );
	for (k=0;k<=(j-1);k++)
		{
		MGET( lpml_, j, j )=MGET( lpml_, j, j )-MGET( lpml_, j, k )*MGET( lpml_, j, k );
		}
	if (MGET( lpml_, j, j )<zero)
		{MGET( lpmqpindx_, 0, 0 )=1;
		 return;}
	MGET( lpml_, j, j )=sqrt(MGET( lpml_, j, j ));
	for (i=j+1;i<n;i++)
		{
		MGET( lpml_, i, j )=MGET( lpmg_, i, j );
		for (k2=0;k2<=(j-1);k2++)
			{
			MGET( lpml_, i, j )=MGET( lpml_, i, j )-MGET( lpml_, i, k2 )*MGET( lpml_, j, k2 );
			}
		MGET( lpml_, i, j )=MGET( lpml_, i, j )/MGET( lpml_, j, j );
		}
	}
}



