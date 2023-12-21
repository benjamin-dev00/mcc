#include "tfalgo.h"
#include "tfwsqp.h"
int initia(int n, int me, int sm, LPMATRIX lpma_, LPMATRIX lpmb_,
			LPMATRIX lpmc_, LPMATRIX lpmg_, LPMATRIX lpmx_, LPMATRIX lpmv_,
			LPMATRIX lpmmj_, LPMATRIX lpmjset_, int nj, LPMATRIX lpmr_,
			LPMATRIX lpmnm_, LPMATRIX lpml_, LPMATRIX lpmw_, LPMATRIX lpmq_,
			LPMATRIX lpmqpindx_)
/*	int n,me,sm;
	float a[namax][mamax],b[mamax],c[namax],g[namax][namax],x[namax],v[mamax];
	float r[namax][namax],nm[namax][namax],l[namax][namax],w[namax],q[namax][namax];
	int qpindx[1],nj,jset[mamax];
	int mj[mamax];
*/
{
	int trans;/*care if return a value*/
	int jrank;
	int i,j;

	MGET( lpmqpindx_, 0, 0 )=0;

/*--------comptue the cholesky decomposition of g---------*/
	chodec(n, lpmg_, lpml_, lpmqpindx_ );
	if ((int)MGET( lpmqpindx_, 0, 0 )==1)
	{
//		MessageBox( NULL, "*matrix g is not positive definite*", "", MB_OK );
	return(nj);}

/*--------intialize the index set J-------*/
	nj=me;
	for (i=0;i<sm;i++)
	{
		MGET( lpmmj_, i, 0 )=0;
	}
	for (i=0;i<me;i++)
	{
		MGET( lpmjset_, i, 0 )=i;
		MGET( lpmmj_, i, 0 )=1;
	}

/*--------compute BsubJ=(inverse of l)*AsubJ---------
			(matrix BsubJ is stored in array r)   */
	for (i=0;i<nj;i++)
	{
		for (j=0;j<n;j++)
		{
			MGET( lpmw_, j, 0 )=MGET( lpma_, j, i );
		}
		trans=0;
		fwdsub(n, lpml_, trans, lpmw_ );
		for (j=0;j<n;j++)
		{
			MGET( lpmr_, j, i )=MGET( lpmw_, j, 0 );
		}
	}

/*---------compute the QR decomposition of matrix BsubJ----------*/
	jrank=qrdec(n,nj, lpmr_, lpmq_ );
	if(jrank==-1) goto six;

	MessageBox( NULL, "*the equality constrians are not independent*", "", MB_OK );/* write to data file*/

	MGET( lpmqpindx_, 0, 0 )=2;
	return(nj);

/*-----------compute matrix nm---------*/

six:
	for (i=0;i<nj;i++)
	{
		for(j=0;j<n;j++)
		{
			MGET( lpmw_, j, 0 )=MGET( lpmq_, j, i );
		}
		trans=1;
		bwdsub(n, lpml_,trans, lpmw_);
		for (j=0;j<n;j++)
		{
			MGET( lpmnm_, j, i )=MGET( lpmw_, j, 0 );
		}
	}
	for (i=0;i<(n-nj);i++)
	{
		for(j=0;j<n;j++)
		{
			MGET( lpmw_, j, 0 )=MGET( lpmq_, j, i+nj );
		}
		trans=1;
		bwdsub(n, lpml_,trans, lpmw_ );
		for(j=0;j<n;j++)
		{
			MGET( lpmnm_, j, i+nj )=MGET( lpmw_, j, 0 );
		}
	}

/*---------compute the initial solution----------*/
	if(nj==0)
	{
		for (j=0;j<n;j++)
		{
			MGET( lpmx_, j, 0 )=0.0;
		}
		goto s16;
	}
	for(i=0;i<nj;i++)
	{
		MGET( lpmw_, i, 0 )=MGET( lpmb_, i, 0 );
	}
	trans=1;
	fwdsub(nj, lpmr_, trans, lpmw_);
	for(j=0;j<n;j++)
	{
		MGET( lpmx_, j, 0 )=0.0;
		for (i=0;i<nj;i++)
		{
			MGET( lpmx_, j, 0 )=MGET( lpmx_, j, 0 )+MGET( lpmnm_, j, i )*MGET( lpmw_, i, 0 );
		}
	}

s16:
	for(i=nj;i<n;i++)
	{
		MGET( lpmw_, i, 0 )=0.0;
		for(j=0;j<n;j++)
		{
			MGET( lpmw_, i, 0 )=MGET( lpmw_, i, 0 )+MGET( lpmnm_, j, i )*MGET( lpmc_, j, 0 );
		}
	}
	for (j=0;j<n;j++)
	{
		for(i=nj;i<n;i++)
		{
			MGET( lpmx_, j, 0 )=MGET( lpmx_, j, 0 )-MGET( lpmnm_, j, i )*MGET( lpmw_, i, 0 );
		}
	}

/*----------compute the multipliers v--------*/
	for (i=0;i<sm;i++)
	{
		MGET( lpmv_, i, 0 )=0.0;
	}
	if (nj==0) return(nj);
	for(i=0;i<nj;i++)
	{
		MGET( lpmv_, i, 0 )=MGET( lpmb_, i, 0 );
	}
	trans=1;
	fwdsub(nj, lpmr_,trans, lpmv_ );
	trans=0;
	bwdsub(nj, lpmr_,trans, lpmv_);
	for(i=0;i<nj;i++)
	{
		MGET( lpmw_, i, 0 )=0.0;
		for(j=0;j<n;j++)
		{
			MGET( lpmw_, i, 0 )=MGET( lpmw_, i, 0 )+MGET( lpmnm_, j, i )*MGET( lpmc_, j, 0 );
		}
	}
	trans=0;
	bwdsub(nj,lpmr_,trans, lpmw_ );
	for (i=0;i<nj;i++)
	{
		MGET( lpmv_, i, 0 )=MGET( lpmv_, i, 0 )+MGET( lpmw_, i, 0 );
	}
return(nj);
}













