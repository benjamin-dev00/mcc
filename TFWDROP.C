#include "tfalgo.h"
#include "tfwsqp.h"
int drop(int n, int nj, LPMATRIX lpmmj_, LPMATRIX lpmjset_,
			LPMATRIX lpmr_, LPMATRIX lpmnm_, LPMATRIX lpmjk_, LPMATRIX lpmjp_)
/*	int n,m,nj;
	float r[namax][namax],nm[namax][namax];
	int mj[mamax],jset[mamax];
	int jk[1],jp[1];
*/
{
	double gamma,c,s,v,w;
	int i,j,l;

	if((int)MGET( lpmjp_, 0, 0 )==(nj-1)) goto six;
	for(i=(int)MGET( lpmjp_, 0, 0 );i<(nj-1);i++)
	{
		for(j=0;j<n;j++)
		{
			MGET( lpmr_, j, i )=MGET( lpmr_, j, i+1 );
		}
	}
   for(i=(int)MGET( lpmjp_, 0, 0 );i<(nj-1);i++)
	{
		gamma=MGET( lpmr_, i, i )*MGET( lpmr_, i, i )+MGET( lpmr_, i+1, i )*MGET( lpmr_, i+1, i );
		if(gamma<zero) goto four;
		gamma=sqrt(gamma);
		c=MGET( lpmr_, i, i )/gamma;
		s=MGET( lpmr_, i+1, i )/gamma;
		for(l=i;l<(nj-1);l++)
		{
			v=MGET( lpmr_, i, l );
			w=MGET( lpmr_, i+1, l );
			MGET( lpmr_, i, l )=c*v+w*s;
			MGET( lpmr_, i+1, l )=-s*v+c*w;
		}
		for(j=0;j<n;j++)
		{
			v=MGET( lpmnm_, j, i );
			w=MGET( lpmnm_, j, i+1 );
			MGET( lpmnm_, j, i )=c*v+w*s;
			MGET( lpmnm_, j, i+1 )=-s*v+c*w;
		}

	four:
		continue;
	}
	for(i=(int)MGET( lpmjp_, 0, 0 );i<(nj-1);i++)
	{
		MGET( lpmjset_, i, 0 )=MGET( lpmjset_, i+1, 0 );
	}

six:
	MGET( lpmmj_, (int)(MGET( lpmjk_, 0, 0 )), 0 )=0;
	nj=nj-1;
	return(nj);
}

