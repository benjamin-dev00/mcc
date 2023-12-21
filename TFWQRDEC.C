#include "tfalgo.h"
#include "tfwsqp.h"
int qrdec( int n, int sm, LPMATRIX lpmr_, LPMATRIX lpmq_ )
//	int n,sm;
//	float r[namax][namax],q[namax][namax];
{
	int jrank;
	double gamma,c,s,v,w;
	int jzero,jn;
	int i,j,k,l;

	jn=(sm<(n-1))?sm:(n-1);
	jrank=-1;
	for (i=0;i<n;i++)
	{
		for (j=0;j<n;j++)
		{
			MGET( lpmq_, i, j )=0.0;
		}
		MGET( lpmq_, i, i )=1.0e0;
	}
	for (i=0;i<jn;i++)
	{
		jzero=1;
		for (j=(i+1);j<n;j++)
		{
			gamma=MGET( lpmr_, i, i )*MGET( lpmr_, i, i ) +
					MGET( lpmr_, j, i )*MGET( lpmr_, j, i );
			if(gamma<zero) goto five;
			jzero=0;
			gamma=sqrt(gamma);
			c=MGET( lpmr_, i, i )/gamma;
			s=MGET( lpmr_, j, i )/gamma;
			for (l=i;l<sm;l++)
			{
				v=MGET( lpmr_, i, l );
				w=MGET( lpmr_, j, l );
				MGET( lpmr_, i, l )=c*v+s*w;
				MGET( lpmr_, j, l )=-s*v+c*w;
			}
			for(k=0;k<n;k++)
			{
				v=MGET( lpmq_, k, i );
				w=MGET( lpmq_, k, j );
				MGET( lpmq_, k, i )=c*v+s*w;
				MGET( lpmq_, k, j )=-s*v+c*w;
			}

		five:	continue;
		}
		if(jzero)
		{
			jrank=i;
			return(jrank);
		}
	}
	return(jrank);
}

