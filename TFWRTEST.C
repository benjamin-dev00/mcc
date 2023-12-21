#include "tfalgo.h"
#include "tfwsqp.h"
double rtest( int me,int nj, LPMATRIX lpmv_, LPMATRIX lpmrv_,
			LPMATRIX lpmjset_, double t, LPMATRIX lpmjk_, LPMATRIX lpmjp_ )
/*	int m,me,nj;
	double v[mamax],rv[mamax];
	double t;
	int jk[1],jp[1],jset[mamax];
  */
{
	double large,ratio;
	int i,l;

	large=1.0e+20;
	MGET( lpmjk_, 0, 0 )=-1;
	t=large;
	for(i=me;i<nj;i++)
	{
		l= (int)MGET( lpmjset_, i, 0 );
		if( MGET( lpmrv_, i, 0 ) < zero)  goto one;
		ratio=MGET( lpmv_, l, 0 )/ MGET( lpmrv_, i, 0 );
		if(ratio<t)
		{
			t=ratio;
			MGET( lpmjk_, 0, 0 )=l;
			MGET( lpmjp_, 0, 0 )=i;
		}

	one:
		continue;
	}
	return(t);
}
