#include "tfalgo.h"
#include "tfwsqp.h"
int add( int n, int sm, int nj, LPMATRIX lpmmj_, LPMATRIX lpmjset_,
			LPMATRIX lpmr_, LPMATRIX lpmnm_, LPMATRIX lpmdv_, int js)
/*	int n,m,nj,js,jset[mamax],mj[mamax];
	float r[namax][namax],nm[namax][namax],dv[namax];
  */
{
	double gamma,c,s,v,w;
	int j,k;

	for (j=0;j<n;j++)
	{
		MGET( lpmr_, j, nj )=MGET( lpmdv_, j, 0 );
	}
	for (j=(nj+1);j<n;j++)
	{
		gamma=MGET( lpmr_, nj, nj )*MGET( lpmr_, nj, nj )+MGET( lpmr_, j, nj )*MGET( lpmr_, j, nj );
		if(gamma<zero) goto three;
		gamma=sqrt(gamma);
		c=MGET( lpmr_, nj, nj )/gamma;
		s=MGET( lpmr_, j, nj )/gamma;
		MGET( lpmr_, nj, nj )=gamma;
		MGET( lpmr_, j, nj )=0.0;
		for(k=0;k<n;k++)
		{
			v=MGET( lpmnm_, k, nj );
			w=MGET( lpmnm_, k, j );
			MGET( lpmnm_, k, nj )=c*v+s*w;
			MGET( lpmnm_, k, j )=-s*v+c*w;
		}

	three:
		continue;
	}
	MGET( lpmjset_, nj, 0 )=js;
	MGET( lpmmj_, js, 0 ) = 1;
	nj=nj+1;
	return(nj);
}


