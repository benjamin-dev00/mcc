#include "tfalgo.h"
#include "tfwsqp.h"
int chkopt(int n, int me, int sm, LPMATRIX lpma_, LPMATRIX lpmb_,
			LPMATRIX lpmx_, LPMATRIX lpmmj_, int js, LPMATRIX lpmus_,
			LPMATRIX lpmoptm_ )
/*	int n,me,sm;
	int js,optm[1];
	float us[1];
	float a[namax][mamax],b[mamax],x[namax];
	int mj[mamax];*/

{
	double res;
	int i,j;

	js=-1;
	MGET( lpmus_, 0, 0 )=-zero;
	for(i=me;i<sm;i++)
	{
		if((int)MGET( lpmmj_, i, 0 )) goto two;
		res=-MGET( lpmb_, i, 0 );
		for(j=0;j<n;j++)
		{
			res=res+MGET( lpma_, j, i )*MGET( lpmx_, j, 0 );
		}
		if(res<MGET( lpmus_, 0, 0 ))
		{
			MGET( lpmus_, 0, 0 )=res;
			js=i;
		}

		two:
		continue;
	}
	if(js==-1) MGET( lpmoptm_, 0, 0 ) = 1;
	return(js);
}
