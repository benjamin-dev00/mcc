#include "tfalgo.h"
#include "tfwsqp.h"
void qpsolv(int n, int me, int sm, LPMATRIX lpma_, LPMATRIX lpmb_,
			LPMATRIX lpmc_, LPMATRIX lpmg_, LPMATRIX lpmx_,
			LPMATRIX lpmv_, LPMATRIX lpml_, LPMATRIX lpmq_, LPMATRIX lpmw_,
			LPMATRIX lpmas_, LPMATRIX lpmdv_, LPMATRIX lpmzv_,
			LPMATRIX lpmrv_, LPMATRIX lpmr_, LPMATRIX lpmnm_, LPMATRIX lpmjset_,
			LPMATRIX lpmmj_, LPMATRIX lpmqpindx_, LPMATRIX lpmit_,
			int scale, LPMATRIX lpmscl_ )
/*	int n,me,sm;
	float a[namax][mamax],b[mamax],c[namax],g[namax][namax],x[namax],v[mamax];
	float r[namax][namax],nm[namax][namax],l[namax][namax],w[namax],q[namax][namax];
	float as[namax],dv[namax],zv[namax],rv[mamax];
	float scl[mamax];
	int qpindx[1],jset[mamax],it[1];
	int mj[mamax],scale;
*/

{
//	float oscl;/*res[namax]*/
	int i,j;
	int trans;
	int nj;/*!! it is used in the whole function, pay attention to its value*/
	int js;/*it is used in chkopt function*/
//	int jk[1],jp[1];  /*used in rtest and drop function*/
	LPMATRIX _lpmjk, _lpmjp, _lpmoptm;
//	int optm[1]; /*used in chkopt function*/
//	float us[1];/*used in chkopt function and caculate t2*/
	LPMATRIX 	_lpmus;
	double 	zmax,	t1, t2,t,ts, _zv[namax], _itmax;
//note: should be freed!!!
	_lpmus = MatrixAlloc( 2, 1 );
	_lpmjk = MatrixAlloc( 2, 1 );
	_lpmjp = MatrixAlloc( 2, 1 );
	_lpmoptm = MatrixAlloc( 2, 1 );
//note:
//	if (scale)
//		oscl = scal( n, sm,lpmg_, lpmc_, lpma_, lpmb_, lpmscl_, lpmqpindx_);

/*-------------find the initial solution----------*/

//.notes: keeping from warning
	nj = 0;
	nj=initia( n, me, sm, lpma_,lpmb_, lpmc_, lpmg_, lpmx_, lpmv_,
			   lpmmj_, lpmjset_, nj, lpmr_, lpmnm_, lpml_, lpmw_, lpmq_, lpmqpindx_);
	for(j=0;j<n;j++)
		_zv[j] = MGET( lpmnm_, j, 0 );

	if ((int)MGET( lpmqpindx_, 0, 0 )!=0) goto end;
	MGET( lpmit_, 0, 0 )=0;
	_itmax=10*(sm+n);




/*-------check optimality----------*/
one:
//note:
	js = 0;
	js=chkopt( n, me, sm, lpma_, lpmb_, lpmx_, lpmmj_, js, _lpmus, _lpmoptm);
	if(js==-1) /*--- ! it is different from fortran's js*/
	{
		MGET( lpmqpindx_, 0, 0 )=0;
//		if(scale)
//			qpout(n,sm, lpmg_, lpmc_, lpma_, lpmb_, lpmv_,oscl, lpmscl_); /*get qp subproblem solution*/
		goto end;                                                     	}
	for(j=0;j<n;j++)
	{
		MGET( lpmas_, j, 0 ) = MGET( lpma_, j, js );
	}

/*--------update the current solution-------*/
three:
  /*	prnt(n,sm,a,b,c,g,x,v,res,jset,nj,it); */

	MGET( lpmit_, 0, 0 )=MGET( lpmit_, 0, 0 )+1;
	if((int)MGET( lpmit_, 0, 0 )>_itmax)
	{
		MGET( lpmqpindx_, 0, 0 )=4;
		MessageBox( NULL, "number of iteratijons exceeds iteration limit\n", "", MB_OK );
		goto end;
	}



/*-------comptue the vector dv---------*/
	for(i=0;i<n;i++)
	{
		MGET( lpmdv_, i, 0 )=0.0;
		for(j=0;j<n;j++)
		{
			MGET( lpmdv_, i, 0 )=MGET( lpmdv_, i, 0 )+MGET( lpmnm_, j, i )*MGET( lpmas_, j, 0 );
		}
	}
	for(j=0;j<n;j++)
		_zv[j] = MGET( lpmnm_, j, 0 );

/*--------compute vectors zv and rv--------*/
	for(j=0;j<n;j++)
	{
		MGET( lpmzv_, j, 0 )=0.0;
		for(i=nj;i<n;i++)
		{
			MGET( lpmzv_, j, 0 )=MGET( lpmzv_, j, 0 )+MGET( lpmnm_, j, i )*MGET( lpmdv_, i, 0 );
		}
	}
	for(i=0;i<nj;i++)
	{
		MGET( lpmrv_, i, 0 )=MGET( lpmdv_, i, 0 );
	}
	trans=0;
	bwdsub(nj, lpmr_, trans, lpmrv_ );

/*-------check if (a) zv<>0 or (b) zv=0--------*/
	zmax=0.0;
	for(j=0;j<n;j++)
	{
        _zv[j] = MGET( lpmas_, j, 0 );
		zmax=(fabs(MGET( lpmzv_, j, 0 ))>zmax)?fabs(MGET( lpmzv_, j, 0 )):zmax;
	}
	if(zmax<zero) goto f15;

/*--------case (a): zv<>0----*/
/*--------compute t1-------*/
//note:
	t1 = 0;
	t1=rtest( me,nj, lpmv_, lpmrv_, lpmjset_, t1, _lpmjk, _lpmjp);

/*--------compute t2--------*/
	ts=0.0;
	for(j=0;j<n;j++)
	{
		ts=ts+MGET( lpmas_, j, 0 )*MGET( lpmzv_, j, 0 );
	}
	t2=-MGET( _lpmus, 0, 0 )/ts;
	if(t1<t2) goto e11;

/*---------case (a-1): t1>=t2---------*/
/*---------constrian js is added to jset (full step)--------*/
	for (j=0;j<n;j++)
	{
		MGET( lpmx_, j, 0 )=MGET( lpmx_, j, 0 )+t2*MGET( lpmzv_, j, 0 );
	}
	for(i=0;i<nj;i++)
	{
		MGET( lpmv_, (int)MGET( lpmjset_, i, 0 ), 0 )=
			MGET( lpmv_, (int)MGET( lpmjset_, i, 0 ), 0 )-t2 * MGET( lpmrv_, i, 0 );
	}
	MGET( lpmv_, js, 0 )=MGET( lpmv_, js, 0 )+t2;
//	MessageBox( NULL, "add constraint , full step\n", "", MB_OK );/*write to data file*/
	nj=add(n,sm,nj, lpmmj_, lpmjset_, lpmr_, lpmnm_, lpmdv_,js);
	goto one;

/*----------case (a-2): t1<t2----------*/
/*---------constraint jk is dropped from jset (partial step)-------*/
e11:
	for(j=0;j<n;j++)
	{
		MGET( lpmx_, j, 0 )=MGET( lpmx_, j, 0 )+t1*MGET( lpmzv_, j, 0 );
	}
	for(i=0;i<nj;i++)
	{
		MGET( lpmv_, (int)MGET( lpmjset_, i, 0 ), 0 ) -=
		 					t1*MGET( lpmrv_, i, 0 );
	}
	MGET( lpmv_, js, 0 )=MGET( lpmv_, js, 0 )+t1;
	MGET( _lpmus, 0, 0 )=-1 * MGET( lpmb_, js, 0 );
	for(j=0;j<n;j++)
	{
		MGET( _lpmus, 0, 0 )=MGET( _lpmus, 0, 0 )+MGET( lpmas_, j, 0 )*MGET( lpmx_, j, 0 );
	}
//	MessageBox( NULL, "drop constraint : partial step\n", "", MB_OK );/*write to data file*/
	nj=drop(n, nj, lpmmj_, lpmjset_, lpmr_, lpmnm_,_lpmjk,_lpmjp);
	goto three;

/*--------case (b): zv=0--------*/
/*--------check if MGET( lpmrv_, i, 0 )<=0 for all active inequality constraints------*/

f15:
//note:
	t = 0;
	t=rtest( me,nj, lpmv_, lpmrv_, lpmjset_, t, _lpmjk, _lpmjp );
	if((int)MGET( _lpmjk, 0, 0 ) != -1) goto s16;

/*--------case (b-1)-------*/
/*--------problem has turned out to be infeasible-------*/
	MGET( lpmqpindx_, 0, 0 )=3;
//	MessageBox( NULL, "problem is infeasible\n", "", MB_OK );
	goto end;

/*--------case (b-2)---------*/
/*---------constraint jk is dropped from jset (partial step)------*/
s16:
	for (i=0;i<nj;i++)
	{
		MGET( lpmv_, (int)MGET( lpmjset_, i, 0 ), 0 ) -= t*MGET( lpmrv_, i, 0 );
	}
	MGET( lpmv_, js, 0 )=t;
//	printf("*---drop----* jk=%d\n", (int)MGET( _lpmjk, 0, 0 ));/*write to data file*/
	nj=drop( n, nj, lpmmj_, lpmjset_, lpmr_, lpmnm_,_lpmjk,_lpmjp);
	goto three;

end:
	MATRIXFREE( _lpmjp );
	MATRIXFREE( _lpmus );
	MATRIXFREE( _lpmjk );
	MATRIXFREE( _lpmoptm );

	return;
}







