/*----------------------------------------------------------------------
This program is for solving the following nonlinear programming problem:

						Min    f(x)

						s.t.     Ci(x)=0,       i=1,2,...,me;
								 Ci(x)>=0,      i=me+1,...,sm(=me+mi);

		where x is an n_-vector, f(x) and Ci(x) are twicecontinuously
		differential functions.
		This program is used SQP method.
-----------------------------------------------------------------------*/
//===================================================================
//
//Zhang
//===================================================================
#include "tfalgo.h"
#include "tfwsqp.h"
void fval( int n_, int sm_, LPMATRIX lpmOC_, LPMATRIX lpmK_, LPMATRIX lpmx_,LPMATRIX lpmofun_, LPMATRIX lpmcfun_ )
{
	int _i, _j;
	MGET( lpmofun_, 0, 0 ) = 0.0;
	for( _j = 0; _j < n_; _j++ )
		MGET( lpmofun_, 0, 0 ) += MGET( lpmOC_, 0, _j ) * MGET( lpmx_, _j, 0 );
	MGET( lpmofun_, 0, 0 ) += MGET( lpmK_, 0, 0 );
	for( _i = 0; _i < sm_; _i++ )
	{
		MGET( lpmcfun_, _i, 0 ) = 0.0;
		for( _j = 0; _j < n_; _j++ )
		{
			MGET( lpmcfun_, _i, 0 ) += (MGET( lpmOC_, (_i+1), _j )* MGET( lpmx_, _j, 0 ));
		}
		MGET( lpmcfun_, _i, 0 ) += (MGET( lpmK_, (_i+1), 0 ));
	}
}

/*#include "dif.c"     */
//#pragma argsused
void dif(int n_, int sm_, LPMATRIX lpmOC_, LPMATRIX lpmK_,
			LPMATRIX lpmx_, LPMATRIX lpmogra_, LPMATRIX lpmcgra_,
			LPMATRIX lpmy_, LPMATRIX lpmcy1_, LPMATRIX lpmcy2_,
			LPMATRIX lpmnf_ )
//      int n_,me,mi,sm,nf[1];
//      double x[nmax],ogra[namax],cgra[namax][mamax],y[namax],cy1[namax],cy2[namax];

{
	int i,j,k;
//      double fy1[1],fy2[1],delta,step,dstep;
	double delta,step,dstep;
	LPMATRIX _lpmfy1, _lpmfy2;
//      double _x[nmax], _y[namax], _ogra[namax], _cgra[namax][mamax], _cy1[namax], _cy2[namax];
	delta=1.0e-5;
	_lpmfy1 = MatrixAlloc( 2, 1 );
	_lpmfy2 = MatrixAlloc( 2, 1 );
//note: should be freed!!!!
/*      for(j=0;j<n_;j++)
	{
		_y[j] = MGET( lpmy_, j, 0 );
		_ogra[j] = MGET( lpmogra_, j, 0 );
		_x[j] = MGET( lpmx_, j, 0 );
		for(k=0;k<sm_;k++)
			_cgra[j][k] = MGET( lpmcgra_, j, k );
	}
*/
	for(j=0;j<n_;j++)
	{
		for(i=0;i<n_;i++)
		{
			MGET( lpmy_, i, 0 ) = MGET( lpmx_, i, 0 );
//                      _y[i] = MGET( lpmy_, i, 0 );
		}
		step=delta*(1.0e0>fabs(MGET( lpmx_, j, 0 ) ) ?
				1.0e0:fabs(MGET( lpmx_, j, 0 )));
		dstep=2.0*step;
		MGET( lpmy_, j, 0 ) = MGET( lpmx_, j, 0 ) + step;
		fval( n_, sm_, lpmOC_, lpmK_, lpmy_, _lpmfy1, lpmcy1_ );
		MGET( lpmnf_, 0, 0 )= MGET( lpmnf_, 0, 0 ) +1;
		MGET( lpmy_, j, 0 ) = MGET( lpmx_, j, 0 ) - step;
		fval( n_, sm_, lpmOC_, lpmK_, lpmy_, _lpmfy2, lpmcy2_ );
		MGET( lpmnf_, 0, 0 ) = MGET( lpmnf_, 0, 0 ) + 1;
		MGET( lpmogra_, j, 0 ) = ( MGET( _lpmfy1, 0, 0 ) -
								MGET( _lpmfy2, 0, 0 ) ) / dstep;
//              _ogra[j] = MGET( lpmogra_, j, 0 );
		for(k=0;k<sm_;k++)
		{
			MGET( lpmcgra_, j, k ) = ( MGET( lpmcy1_, k, 0 ) -
										MGET( lpmcy2_, k, 0 ) ) / dstep;
//                      _cgra[j][k] = MGET( lpmcgra_, j, k );
		}
	}
/*      for(j=0;j<n_;j++)
	{
		_y[j] = MGET( lpmy_, j, 0 );
		_ogra[j] = MGET( lpmogra_, j, 0 );
		_x[j] = MGET( lpmx_, j, 0 );
	}
*/
	MATRIXFREE( _lpmfy1 );
	MATRIXFREE( _lpmfy2 );
}

double pval(int me, int mi, LPMATRIX lpmofun_,
			LPMATRIX lpmcfun_, double ppra_, double pfun_)
//      int me,mi,sm;
//      double ofun[1],cfun[mmax],ppra,pfun;

{
	int i;
	pfun_ = 0.0;
	for(i=0;i<me;i++)
	{
		pfun_ = pfun_ + fabs(MGET( lpmcfun_, i, 0 ));
	}
	for(i=0;i<mi;i++)
	{
		pfun_ = pfun_ + fabs(0.0e0 < MGET( lpmcfun_, me+i, 0 )
			? 0.0e0: MGET( lpmcfun_, me+i, 0 ));
	}
	pfun_ = MGET( lpmofun_, 0, 0 ) + ppra_ * pfun_;
	return( pfun_ );
}


int contes(int n_, int me, int sm, LPMATRIX lpmx_, LPMATRIX lpmu_,
	double pfun_, LPMATRIX lpmoldpf_, LPMATRIX lpmcfun_, LPMATRIX lpmogra_,
	LPMATRIX lpmcgra_, LPMATRIX lpmlgra_, double bound, double eps1,
	double eps2, LPMATRIX lpmindex_ )
//      int n_,me,sm,index[1];
//      double x[nmax],u[mamax],pfun,oldpf[1],cfun[mmax],ogra[namax],
//                      cgra[namax][mamax],lgra[namax],eps1,eps2,bound;

{
	int j,i, _flagsatisfied = 0;
	double cmax;
//      double  _ogra[namax], _cgra[namax][namax], _lgra[namax], _u[mamax];

/*      for(j=0;j<n_;j++)
	{
		_ogra[j] = MGET( lpmogra_, j, 0 );
		_lgra[j] = MGET( lpmlgra_, j, 0 );
		for( i = 0; i < sm; i++ )
			_cgra[j][i] = MGET( lpmcgra_, j, i );
	}
	for( i = 0; i < sm; i++ )
		_u[i] = MGET( lpmu_, i, 0 );
*/
	for (j=0;j<n_;j++)
	{
		if(fabs(MGET( lpmx_, j, 0))>bound)
		{
			MGET( lpmindex_, 0, 0 )=4;
			return _flagsatisfied;
		}
	}

	for(j=0;j<n_;j++)
	{
		MGET( lpmlgra_, j, 0 ) = MGET( lpmogra_, j, 0 );
		// lgra[j]=ogra[j];
		for(i=0;i<sm;i++)
		{
			MGET( lpmlgra_, j, 0 ) = MGET( lpmlgra_, j, 0 ) - MGET( lpmu_, i, 0 )
			 * MGET( lpmcgra_, j, i );
		}
//              _lgra[j] = MGET( lpmlgra_, j, 0 );
	}

	for(j=0;j<n_;j++)
	{
		if(fabs( MGET( lpmlgra_, j, 0 ) > eps1 ))
		{
			MGET( lpmindex_, 0, 0 ) = 2;
			goto s7;
		}
	}
	cmax=0.0;
	for(i=0;i<me;i++)
	{
		cmax=cmax>fabs(MGET( lpmcfun_, i, 0 ))?cmax:fabs(MGET( lpmcfun_, i, 0 ));
	}
	for(i=me;i<sm;i++)
	{
		cmax=cmax>(-1*MGET( lpmcfun_, i, 0 ))?cmax:(-1*MGET( lpmcfun_, i, 0 ) );
	}
	if(cmax>eps1)
	{
		MGET( lpmindex_, 0, 0 )=2;
		goto s7;
	}
	for(i=me;i<sm;i++)
		if(MGET( lpmu_, i, 0 ) < -eps1 )
		{
			MGET( lpmindex_, 0, 0 ) = 2;
	 goto s7;
		}

	for(i=me;i<sm;i++)
	{
		if((MGET( lpmu_, i, 0 )>eps1)&&(MGET( lpmcfun_, i, 0 )>eps1))
		{
			MGET( lpmindex_, 0, 0 )=2;
			goto s7;
		}
	}
	MGET( lpmindex_, 0, 0 )=1;

s7:
	if((MGET( lpmoldpf_, 0, 0 ) - pfun_ )<(eps2*(1.0e0+fabs(pfun_))))
	{
		if(MGET( lpmindex_, 0, 0 )==2) return _flagsatisfied;
		MGET( lpmindex_, 0, 0 )=0;
	}
	else
	{
		if(MGET( lpmindex_, 0, 0 )==1) return _flagsatisfied;
		MGET( lpmindex_, 0, 0 )=3;
	}
	return _flagsatisfied;
}

void auxqp( int n_, int me, int mi, int sm, int na,
			LPMATRIX lpmcgra_, LPMATRIX lpmrhs_, LPMATRIX lpmogra_,
			LPMATRIX lpmb_, double ppra)

//      int n_,me,mi,sm,na,mea,ma;
//      double cgra[namax][mamax],rhs[mamax],ogra[namax],b[namax][namax],ppra;
{
	double wsmall=1.0e-3;
//      int i,j,naux,mia,h;
	int i,j,naux;
//      int     mia;

//      MessageBox( NULL, "solve auxiliary qp subproblem since basic qp is infeasible\n_", "", MB_OK );
	naux=2*me+mi;
	na=n_+naux;
//      mea=me;
//      mia=mi+naux;
//      ma=me+mia;
	for(j=0;j<naux;j++)
	{
		MGET( lpmogra_, n_+j, 0 ) = ppra;
		//ogra[n_+j]=ppra;
	}
	for(i=0;i < sm;i++)
	{
		for(j=0;j<naux;j++)
		{
			MGET( lpmcgra_, n_+j, i ) = 0.0;
//                      cgra[n_+j][i]=0.0;
		}
	}
	for(i=0;i<me;i++)
	{
		MGET( lpmcgra_, n_+i, i ) = 1.0e0;
		//cgra[n_+i][i]=1.0e0;
		MGET( lpmcgra_, n_+me+i, i ) = -1.0e0;
		//cgra[n_+me+i][i]=-1.0e0;
	}
	for(i=0;i<mi;i++)
	{
		MGET( lpmcgra_, (n_+2*me+i), me+i ) = 1.0e0;
//              cgra[n_+2*me+i][me+i]=1.0e0;
	}
	for(i=0;i<naux;i++)
	{
		MGET( lpmrhs_, sm+i, 0 ) = 0.0;
		//rhs[sm+i]=0.0;
		for(j=0;j<na;j++)
		{
			MGET( lpmcgra_, j, sm+i ) = 0.0;
			//cgra[j][sm+i]=0.0;
		}
	}
	for(i=0;i<naux;i++)
	{
		MGET( lpmcgra_, n_+i, sm+i ) = 1.0e0;
		//cgra[n_+i][sm+i]=1.0e0;
	}
	for(i=0;i<naux;i++)
	{
		for(j=0;j<n_;j++)
		{
			MGET( lpmb_, n_+i, j ) = 0.0;
			//b[n_+i][j]=0.0;
			MGET( lpmb_, j, n_+i ) = 0.0;
			//b[j][n_+i]=0.0;
		}
	}
	for(i=0;i<naux;i++)
	{
		for(j=0;j<naux;j++)
		{
			if(i==j)
				MGET( lpmb_, n_+i, n_+j ) = wsmall;
//                              b[n_+i][n_+j]=wsmall;
			else
				MGET( lpmb_, n_+i, n_+j ) = 0.0;
				//b[n_+i][n_+j]=0.0;
		}
	}
}

int check(int n_, int mi, int sm_, LPMATRIX lpmOC_, LPMATRIX lpmK_,
			 LPMATRIX lpmx_, LPMATRIX lpmcfun_, LPMATRIX lpmcgra_,
			 LPMATRIX lpmcgra1_, LPMATRIX lpmd1_, LPMATRIX lpmak_,
			 LPMATRIX lpmw0_, LPMATRIX lpmw1_, LPMATRIX lpmw2_,
			 LPMATRIX lpmw3_, LPMATRIX lpmw4_, LPMATRIX lpmcon1_,
			 int grad, int corr, LPMATRIX lpmnf_, LPMATRIX lpmng_ )
//      int n_,me,mi,sm,grad,corr,nf[1],ng[1];
//      double x[nmax],cfun[mmax],cgra[namax][mamax],cgra1[namax][mmax],d1[namax],
//      ak[namax][mmax],w0[namax],w1[namax],w2[namax],w3[namax],w4[namax],con1[mmax];


{
	double ci,ca,cd,camax,cdmax,eps,sigma;
	int empty,i,j;
	LPMATRIX _lpmobj1;

	eps=1.0e-1;
	sigma=5.0e-2;
	corr=0;
	_lpmobj1 = MatrixAlloc( 2, 1 );
//note: should be freed!!!
	for(j=0;j<n_;j++)
	{
		MGET( lpmw0_, j, 0 ) = MGET( lpmx_, j, 0 ) + MGET( lpmd1_, j, 0 );
	}
	fval( n_, sm_, lpmOC_, lpmK_, lpmw0_, _lpmobj1, lpmcon1_ );
	MGET(lpmnf_, 0, 0 ) = MGET( lpmnf_, 0, 0 ) + 1;
	if(grad)
	{
		gval( sm_, lpmOC_, lpmw0_, lpmw1_, lpmcgra1_ );
		MGET( lpmng_, 0, 0 ) = MGET( lpmng_, 0, 0 ) + 1;
	}
	else
		{dif( n_, sm_, lpmOC_, lpmK_, lpmw0_, lpmw1_, lpmcgra1_, lpmw2_,
			lpmw3_, lpmw4_, lpmnf_);}
	for(i=0;i<sm_;i++)
	{
		for(j=0;j<n_;j++)
		{
			MGET( lpmak_, j, i ) = 5.0e-1*( MGET( lpmcgra_, j, i ) +
				MGET( lpmcgra1_, j, i ));
		}
	}
	camax=0.0;
	cdmax=0.0;
	empty=1;
	for(i=0;i<sm_;i++)
	{
		ci= MGET( lpmcfun_, i, 0 );
		if(fabs(ci) >= eps) goto f4;
		empty=0;
		ca=ci- MGET( lpmcon1_, i, 0 );
		cd=ca;
		for(j=0;j<n_;j++)
		{
			ca=ca + MGET( lpmak_, j, i ) * MGET( lpmd1_, j, 0 );
			cd=cd + MGET( lpmcgra_, j, i ) * MGET( lpmd1_, j, 0 );
		}
		ca=fabs(ca);
		cd=fabs(cd);
		camax=ca>camax?ca:camax;
		cdmax=cd>cdmax?cd:cdmax;
	f4:
		continue;
	}
	if(empty)
		{
			MATRIXFREE( _lpmobj1 );
			return(corr);
		}
	if(camax<sigma*cdmax)
	{
		corr=1;
//              MessageBox( NULL, "solve modified qp subproblem to obtain second-order correction\n_", "", MB_OK );
	}
	MATRIXFREE( _lpmobj1 );
	return(corr);
}

double qpval(int n_, int na, LPMATRIX lpmogra_, LPMATRIX lpmb_,
		LPMATRIX lpmd1_, double ppra_, double qpobj_, int feasbl )
//      int n_,na,feasbl;
//      double ogra[namax],b[namax][namax],d1[namax],ppra,qpobj;

{
	int i,j;
	qpobj_=0.0;
	for(j=0;j<n_;j++)
	{
		qpobj_ = qpobj_ + MGET( lpmogra_, j, 0 ) * MGET( lpmd1_, j, 0);
		for(i=0;i<n_;i++)
		{
			qpobj_ = qpobj_ + (5.0e-1) * MGET( lpmd1_, i, 0 ) *
				MGET( lpmb_, i, j ) * MGET( lpmd1_, j, 0);
		}
	}
	if(feasbl) return( qpobj_ );
	for(j=n_;j<na;j++)
	{
		qpobj_ = qpobj_ + ppra_ * MGET( lpmd1_, j, 0);
	}
	return(qpobj_);
}

double lsearc(int n_, int me, int mi, int sm_, LPMATRIX lpmOC_, LPMATRIX lpmK_,
				LPMATRIX lpmx_, LPMATRIX lpmnewx_, LPMATRIX lpmd1_,
				LPMATRIX lpmd2_, int basic_, LPMATRIX lpmofun_,
				LPMATRIX lpmcfun_, double qpobj_, double ppra_, double pfun_,
				LPMATRIX lpmoldpf_, double eps_, LPMATRIX lpmindex_,
				LPMATRIX lpmnf_)
//      int n_,me,mi,sm,basic,index[1],nf[1];
//      double x[nmax],newx[nmax],d1[namax],d2[nmax], ppra,pfun,oldpf[1],
//                      cfun[mmax],qpobj,ofun[1],eps;
{
	double newpf_, d1norm,minst,st,beta,gamma,pred;
	int j;

	beta=1.0e-2;
	gamma=5.0e-1;
	d1norm=0.0;
	for(j=0;j<n_;j++)
		d1norm=d1norm+MGET( lpmd1_, j, 0)*MGET( lpmd1_, j, 0);

	if(d1norm>1.0e0)
	{
		d1norm=sqrt(d1norm);
		minst=eps_/d1norm;
	}
	else
	  {     minst=eps_;}
	pred = MGET( lpmofun_, 0, 0 ) + qpobj_ - pfun_;
	st=1.0e0;
t2:
	for(j=0;j<n_;j++)
	{
		if(basic_)
			MGET( lpmnewx_, j, 0 )=MGET( lpmx_, j, 0)+st*MGET( lpmd1_, j, 0);
		else
			MGET( lpmnewx_, j, 0 )=MGET( lpmx_, j, 0)+st*MGET( lpmd1_, j, 0 ) +
				st * st * ( MGET( lpmd2_, j, 0 ) - MGET( lpmd1_, j, 0));
	}
	fval( n_, sm_, lpmOC_, lpmK_, lpmnewx_, lpmofun_, lpmcfun_ );
	MGET( lpmnf_, 0, 0 )=MGET( lpmnf_, 0, 0 )+1;
//note: it is added for deleting warning;!!!
	newpf_ = 0.0;
	newpf_ = pval( me, mi, lpmofun_, lpmcfun_, ppra_, newpf_ );
	if(newpf_<=(pfun_ + beta*st*pred)) goto f4;
	st=gamma*st;
	if(st>minst) goto t2;
	MGET( lpmindex_, 0, 0 )=5;
f4:
	MGET( lpmoldpf_, 0, 0) = pfun_;
	pfun_ = newpf_;
/*      if(basic_)
		MessageBox( NULL, "one-dimensional search-basic search direction", "", MB_OK );
	else
		MessageBox( NULL, "one-dimensional search-second order correction", "", MB_OK );
	MessageBox( NULL, "norm of basic search direction=%f\n_",d1norm);
	printf("step-size=%f\n_",st);*/
	return(pfun_);
}


void bupd(int n_, int sm, LPMATRIX lpmOC_, LPMATRIX lpmK_,
			LPMATRIX lpmx_, LPMATRIX lpmnewx_, LPMATRIX lpmogra_,
			LPMATRIX lpmcgra_, LPMATRIX lpmu_, LPMATRIX lpmb_,
			LPMATRIX lpms_, LPMATRIX lpmy_, LPMATRIX lpmbs_, LPMATRIX lpmlg_,
			LPMATRIX lpmlgnew_, LPMATRIX lpmw3_, LPMATRIX lpmw4_,
			LPMATRIX lpmw5_, int grad, int it, LPMATRIX lpmng_,LPMATRIX lpmnf_ )
//      int n_,me,mi,sm,grad,it,ng[1],nf[1];
//      double x[nmax],newx[nmax],ogra[namax],cgra[namax][mamax],u[mamax],
//                      lg[namax],b[namax][mamax],lgnew[namax],s[nmax],y[nmax],bs[namax],
//                      w3[namax],w4[namax],w5[namax];

{
	int i,j,nn;
	double w,wmax,sy,sbs,theta,theta0,eps,c1,c2;
	eps=1.0e-5;

	for(j=0;j<n_;j++)
	{
		MGET( lpmlg_, j, 0 )=MGET( lpmogra_, j, 0 );
		for(i=0;i<sm;i++)
		{
			MGET( lpmlg_, j, 0 )=MGET( lpmlg_, j, 0 )-MGET( lpmu_, i, 0 )*MGET( lpmcgra_, j, i );
		}
	}
	if(grad)
	{
		gval( sm, lpmOC_, lpmnewx_, lpmogra_, lpmcgra_);
		MGET( lpmng_, 0, 0 )=MGET( lpmng_, 0, 0 )+1;
	}
	else
		dif( n_,sm, lpmOC_, lpmK_, lpmnewx_, lpmogra_, lpmcgra_, lpmw3_,
			lpmw4_, lpmw5_, lpmnf_ );
	nn=5*n_;
	if((it%nn)==0)
	{
		for(i=0;i<n_;i++)
		{

			for(j=0;j<n_;j++)
			{
				if(i==j)
					MGET( lpmb_, i, j )=1.0e0;
				else
					MGET( lpmb_, i, j )=0.0;
			}
		}
		goto t10;
	}
	for(j=0;j<n_;j++)
	{
		MGET( lpmlgnew_, j, 0 )=MGET( lpmogra_, j, 0 );
		for(i=0;i<sm;i++)
		{
			MGET( lpmlgnew_, j, 0 )=MGET( lpmlgnew_, j, 0 )-MGET( lpmu_, i, 0 )*MGET( lpmcgra_, j, i );
		}
	}
	for(j=0;j<n_;j++)
	{
		MGET( lpms_, j, 0 )=MGET( lpmnewx_, j, 0 )-MGET( lpmx_, j, 0);
		MGET( lpmy_, j, 0 )=MGET( lpmlgnew_, j, 0 )-MGET( lpmlg_, j, 0 );
	}
	for(i=0;i<n_;i++)
	{
		MGET( lpmbs_, i, 0 )=0.0;
		for(j=0;j<n_;j++)
		{
			MGET( lpmbs_, i, 0 )=MGET( lpmbs_, i, 0 )+MGET( lpmb_, i, j )*MGET( lpms_, j, 0 );
		}
	}
	wmax=0.0;
	for(j=0;j<n_;j++)
	{
		w=MGET( lpmy_, j, 0 )-MGET( lpmbs_, j, 0 );
		if(fabs(w)>wmax)
			wmax=fabs(w);
	}
	if(wmax<eps) goto t10;
	sy=0.0;
	sbs=0.0;
	for(j=0;j<n_;j++)
	{
		sy=sy+MGET( lpms_, j, 0 )*MGET( lpmy_, j, 0 );
		sbs=sbs+MGET( lpms_, j, 0 )*MGET( lpmbs_, j, 0 );
	}
	if(sqrt(sbs)<eps) goto t10;
	if(sy<0.2e0*sbs)
	{
		theta=0.8e0*sbs/(sbs-sy);
		theta0=1.0e0-theta;
		sy=0.0;
		for(j=0;j<n_;j++)
		{
			MGET( lpmy_, j, 0 )=theta*MGET( lpmy_, j, 0 )+theta0*MGET( lpmbs_, j, 0 );
			sy=sy+MGET( lpms_, j, 0 )*MGET( lpmy_, j, 0 );
		}
	}
	c1=1.0e0/sy;
	c2=1.0e0/sbs;
	for(i=0;i<n_;i++)
	{
		for(j=0;j<=i;j++)
		{
			MGET( lpmb_, i, j )=MGET( lpmb_, i, j )+c1*MGET( lpmy_, i, 0 )*MGET( lpmy_, j, 0 )
					- c2*MGET( lpmbs_, i, 0 )*MGET( lpmbs_, j, 0 );
			MGET( lpmb_, j, i )=MGET( lpmb_, i, j );
		}
	}

t10:
	for(j=0;j<n_;j++)
	{
		MGET( lpmx_, j, 0)=MGET( lpmnewx_, j, 0 );
	}
}




/*-----------main program----------*/
WORD sqp( LPCOBJPRIV lpCobj_All_, LPMATRIX lpmG_, LPCOBJPRIVMV lpCobj_PrivMV_, LPCOBJPRIVCV lpCobj_PrivCV_,
				LPCOBJPRIVDV lpCobj_PrivDV_ )
//void main()
{
/*static double x[nmax],u[mamax],ogra[namax],cfun[mmax],cgra[namax][mamax],
		b[namax][namax],d1[namax],d2[nmax],pk[nmax],ak[namax][mmax],rhs[mamax],
		cgra1[namax][mmax],newx[nmax],u2[mmax],s[nmax],y[nmax],w0[namax],
		w1[namax],w2[namax],w3[namax],w4[namax],w5[namax],wl[namax][namax],
		wq[namax][namax],wr[namax][namax],nm[namax][namax],scl[mamax];
*/
LPMATRIX        _lpmx, _lpmu, _lpmogra, _lpmcfun, _lpmcgra, _lpmb, _lpmd1, _lpmd2,
			_lpmpk, _lpmak, _lpmrhs, _lpmnewx, _lpmu2, _lpms, _lpmy, _lpmw0,
			_lpmcgra1, _lpmw1, _lpmw2, _lpmw3, _lpmw4, _lpmw5, _lpmwl, _lpmwq,
			_lpmwr, _lpmnm, _lpmscl;
LPMATRIX        _lpmofun, _lpmoldpf, _lpmqpindx, _lpmqpit, _lpmjset;
LPMATRIX        _lpmmj, _lpmindex, _lpmnf, _lpmng, _lpmOC, _lpmK, _lpmNumberOfNM;

//double ofun[1],pfun,oldpf[1],ppra,qpobj,umax,initpp,eps1,eps2,eps3,bound,rho;
//int qpindx[1],qpit[1],jset[mamax];
double pfun,ppra,qpobj,umax,initpp,eps1,eps2,eps3,bound,rho;
int   grad,feasbl,corr,basic, scale, _i;
//int valid,grad,feasbl,corr,basic,mj[mamax],scale;
int n_,me,mi,sm_,na,mea,ma,naux,mia;
int it,nqp,itqp, _addcol, _j, _addrow;
//int index[1],itmax,it,nqp,itqp,nf[1],ng[1];
int i,j;
WORD    _flagError = 0;
int       _flag_convergence;

//double _ogra[namax];
//FILE *fin;

eps1=1.0e-5;
eps2=1.0e-4;
eps3=1.0e-50;
bound=1.0e+20;
initpp=1.0e0;
rho=1.0e+1;
grad = 0;
scale = 1;
_lpmNumberOfNM = MatrixAlloc( 3, 1 );
ParameterInitial1( lpCobj_All_, _lpmNumberOfNM, lpmG_, lpCobj_PrivMV_, lpCobj_PrivCV_ );
n_ = (int)MGET( _lpmNumberOfNM, 0, 0 );
me = (int)MGET( _lpmNumberOfNM, 1, 0 );
mi = (int)MGET( _lpmNumberOfNM, 2, 0 );
sm_=me+mi;
_lpmOC = MatrixAlloc( sm_+1, n_ );
_lpmK = MatrixAlloc( sm_+1, 1 );
naux=2*me+mi;
na=n_+naux;
mea=me;
mia=mi+naux;
ma=me+mia;
_lpmx = MatrixAlloc( nmax, 1 );
ParameterInitial2( lpCobj_All_, _lpmx, _lpmOC, _lpmK, lpmG_, lpCobj_PrivMV_, lpCobj_PrivCV_, lpCobj_PrivDV_ );
if( lpCobj_All_->Flag_LPRecount == 0 )
{
	for( _j = 0; _j < lpCobj_All_->m; _j++ )
		PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MSDirty = 1;
	goto NonCalEnd;
}
_lpmu = MatrixAlloc( mamax, 1 );
_lpmogra = MatrixAlloc( namax, 1 );
_lpmcfun = MatrixAlloc( mmax, 1 );
_lpmcgra = MatrixAlloc( namax, mamax );
_lpmb = MatrixAlloc( namax, mamax );
_lpmd1 = MatrixAlloc( namax, 1 );
_lpmd2 = MatrixAlloc( nmax, 1 );
_lpmpk = MatrixAlloc( nmax, 1 );
_lpmak = MatrixAlloc( namax, mmax );
_lpmrhs = MatrixAlloc( mamax, 1 );
_lpmcgra1 = MatrixAlloc( namax, sm_ );
_lpmnewx = MatrixAlloc( nmax, 1 );
_lpmu2 = MatrixAlloc( mmax, 1 );
_lpms = MatrixAlloc( nmax, 1 );
_lpmy = MatrixAlloc( nmax, 1 );
_lpmw0 = MatrixAlloc( namax, 1 );
_lpmw1 = MatrixAlloc( namax, 1 );
_lpmw2 = MatrixAlloc( namax, 1 );
_lpmw3 = MatrixAlloc( namax, 1 );
_lpmw4 = MatrixAlloc( namax, 1 );
_lpmw5 = MatrixAlloc( namax, 1 );
_lpmwl = MatrixAlloc( namax, namax );
_lpmwq = MatrixAlloc( namax, namax );
_lpmwr = MatrixAlloc( namax, namax );
_lpmnm = MatrixAlloc( namax, namax );
_lpmscl = MatrixAlloc( mamax, 1 );

_lpmofun = MatrixAlloc ( 1, 1 );
_lpmoldpf = MatrixAlloc ( 2, 1 );
_lpmqpindx = MatrixAlloc ( 2, 1 );
_lpmqpit = MatrixAlloc ( 2, 1 );
_lpmjset = MatrixAlloc ( mamax, 1 );
_lpmmj = MatrixAlloc ( mamax, 1 );
_lpmnf = MatrixAlloc ( 2, 1 );
_lpmng = MatrixAlloc ( 2, 1 );
_lpmindex = MatrixAlloc ( 2, 1 );
/*------------read initial data from data-file--------------*/

//n_=3;
//me=2;
//mi=6;
//sm_=me+mi;
//MGET( _lpmx, 0, 0 ) = 0.8;
//MGET( _lpmx, 1, 0 ) = 0.6;
//MGET( _lpmx, 2, 0 ) = 0.0;


/*-------------initialize---------------------*/
ppra=initpp;
//max=100>20*(n_+sm_)?100:20*(n_+sm_);
/*itmax=200<max?200:max;*/
it=0;
nqp=0;
itqp=0;
MGET( _lpmnf, 0, 0 )=0;
MGET( _lpmng, 0, 0 )=0;
for (i=0;i<n_;i++)
{
	for (j=0;j<n_;j++)
	{
		if(i==j)
			MGET( _lpmb, i, j )=1.0e0;
		else
			MGET( _lpmb, i, j )=0.0;
	}
}
fval( n_, sm_, _lpmOC, _lpmK, _lpmx, _lpmofun, _lpmcfun);
MGET( _lpmnf, 0, 0 )=MGET( _lpmnf, 0, 0 )+1;
if(grad)
	{
	gval( sm_, _lpmOC, _lpmx, _lpmogra, _lpmcgra);
	MGET( _lpmng, 0, 0 )=MGET( _lpmng, 0, 0 )+1;
	}
else
{
	dif( n_, sm_, _lpmOC, _lpmK, _lpmx,_lpmogra, _lpmcgra, _lpmw2, _lpmw3, _lpmw4, _lpmnf);
}
//note: pfun is given 0 for keeping from warning;
pfun = 0;
//for( _i = 0; _i < n_; _i++ )
//      _ogra[_i] = MGET( _lpmogra, _i, 0 );
pfun=pval(me,mi, _lpmofun,_lpmcfun, ppra, pfun);
/*-------------iteration--------------------*/

/*-------print out intermediate results------------*/
t2:
	/*prnt(n_,me,mi,sm_,x,u,ofun,cfun,ppra,pfun,it);*/

/*--------check termination criteria--------------*/
_flag_convergence = contes(n_,me,sm_, _lpmx, _lpmu, pfun, _lpmoldpf, _lpmcfun,
		_lpmogra, _lpmcgra, _lpmw0, bound,
				 eps1,eps2, _lpmindex);

if(((int)MGET( _lpmindex, 0, 0 )<1)||(int)(MGET( _lpmindex, 0, 0 )==4)) goto e8;
it=it+1;

/*---------solve basic qp subproblem-------------*/
for(i=0;i<sm_;i++)
{
	MGET( _lpmrhs, i, 0 )=-MGET( _lpmcfun, i, 0 );
//      _temp = MGET( _lpmrhs, i, 0 );
}

qpsolv(n_,me,sm_, _lpmcgra, _lpmrhs, _lpmogra,_lpmb, _lpmd1,
		_lpmu, _lpmwl, _lpmwq, _lpmw0, _lpmw1, _lpmw2, _lpmw3,
		_lpmw4, _lpmwr, _lpmnm, _lpmjset, _lpmmj, _lpmqpindx,
		_lpmqpit, scale, _lpmscl);
nqp=nqp+1;
itqp=itqp+(int)MGET( _lpmqpit, 0, 0 );
if(((int)MGET( _lpmqpindx, 0, 0 )==1)||(int)(MGET( _lpmqpindx, 0, 0 )==2)||(int)(MGET( _lpmqpindx, 0, 0 )==4))
{
	MGET( _lpmindex, 0, 0 )=6;
	goto e8;
}
if((int)MGET( _lpmqpindx, 0, 0 ) == 3)
{

	/*------basic qp subproblem is infeasible-----*/
	feasbl=0;
	auxqp(n_,me,mi,sm_,na, _lpmcgra, _lpmrhs, _lpmogra, _lpmb, ppra);
	qpsolv(na,mea,ma, _lpmcgra, _lpmrhs, _lpmogra, _lpmb, _lpmd1, _lpmu,
			_lpmwl, _lpmwq, _lpmw0, _lpmw1, _lpmw2, _lpmw3, _lpmw4, _lpmwr,
			_lpmnm, _lpmjset, _lpmmj, _lpmqpindx, _lpmqpit, scale, _lpmscl);
	nqp=nqp+1;
	itqp=itqp+(int)MGET( _lpmqpit, 0, 0 );
	if((int)MGET( _lpmqpindx, 0, 0 )>0)
	{
		MGET( _lpmindex, 0, 0 )=6;
		goto e8;
	}
	basic=1;
	goto f5;
}
else
	feasbl=1;


/*---------check if second-order correction is necessary-------*/
//note: warning!!
corr = 0;
corr=check(n_, mi,sm_, _lpmOC, _lpmK, _lpmx, _lpmcfun, _lpmcgra, _lpmcgra1, _lpmd1,
		   _lpmak, _lpmw0, _lpmw1, _lpmw2, _lpmw3, _lpmw4, _lpmw5, grad,
		   corr,_lpmnf, _lpmng);
if(!corr)
{
	basic=1;
	goto f5;
}

/*----------solve modified qp subproblem------------*/
for(j=0;j<n_;j++)
{
	MGET( _lpmpk, j, 0 )=MGET( _lpmogra, j, 0 );
	for(i=0;i<sm_;i++)
	{
		MGET( _lpmpk, j, 0 )=MGET( _lpmpk, j, 0 )+(5.0e-1) *
			MGET( _lpmu, i, 0 )*(MGET( _lpmcgra1, j, i )-MGET( _lpmcgra, j, i ));
	}
}
qpsolv(n_,me,sm_, _lpmak, _lpmrhs, _lpmpk, _lpmb, _lpmd2,
		_lpmu2, _lpmwl, _lpmwq, _lpmw0, _lpmw1, _lpmw2,
		_lpmw3, _lpmw4, _lpmwr, _lpmnm, _lpmjset, _lpmmj,
		_lpmqpindx, _lpmqpit, scale, _lpmscl);
nqp=nqp+1;
itqp=itqp+(int)MGET( _lpmqpit, 0, 0 );
if((int)MGET( _lpmqpindx, 0, 0 )==0)
	basic=0;
else
	basic=1;

/*--------update penalty parameter----------*/
f5:
	if(!feasbl) goto s7;
umax=0.0;
for(i=0;i<sm_;i++)
{
	umax=fabs(MGET( _lpmu, i, 0 ))>umax?fabs(MGET( _lpmu, i, 0 )):umax;
}
if(umax>ppra)
{
	ppra=umax+rho;
	pfun=pval(me,mi, _lpmofun, _lpmcfun, ppra,pfun);
}

/*----------perform one-dimensional seach---------*/
s7:
//note: warnging!!
qpobj = 0;
qpobj=qpval(n_,na, _lpmogra, _lpmb, _lpmd1, ppra, qpobj, feasbl);
pfun=lsearc(n_,me,mi, sm_, _lpmOC, _lpmK, _lpmx, _lpmnewx, _lpmd1, _lpmd2,
			basic, _lpmofun, _lpmcfun,qpobj,ppra,pfun, _lpmoldpf,
			eps3, _lpmindex, _lpmnf);

/*----------update current solution and approximate hessian matric-------*/
bupd(n_, sm_, _lpmOC, _lpmK, _lpmx, _lpmnewx, _lpmogra, _lpmcgra, _lpmu,
		_lpmb, _lpms, _lpmy, _lpmw0, _lpmw1, _lpmw2, _lpmw3,
			_lpmw4, _lpmw5, grad,it, _lpmng, _lpmnf);
if((int)MGET( _lpmindex, 0, 0 )==5 && _flag_convergence == 1 )
{
	MGET( _lpmindex, 0, 0 ) = 7;
	goto e8;
}
if((int)MGET( _lpmindex, 0, 0 )==5||it>itmax) goto e8;
goto t2;

/*---------print out computational results-------*/
e8:
pfun=pval( me, mi, _lpmofun, _lpmcfun,ppra,pfun);
output( lpCobj_All_, lpmG_, lpCobj_PrivMV_, lpCobj_PrivCV_, n_,me,mi,sm_, _lpmx, _lpmu, _lpmofun, _lpmcfun, ppra, pfun,
		_lpmindex, _lpmqpindx,  it,nqp,_lpmnf, _lpmng,itqp);

if ( MGET( _lpmindex, 0, 0 ) != 0 && MGET( _lpmindex, 0, 0 ) != 7 )
	_flagError = 1;
lpCobj_All_->LPCount++;
if( !_flagError )
{
	_addrow = 0;
	for( _i = 0; _i < lpCobj_All_->p; _i++ )
		if( PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CControl_type == 4 )
		{
			_addrow++;
			PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CSDirty = 0;
		}
		else
		{
//note: here is changed but not having checked out
//				if( PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CControl_type == 2 )
//				{
			PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CSteadOptim = MGET( _lpmx, _i-_addrow, 0  );
			PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CSetpoint = PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CSteadOptim;
//				}
			PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CSDirty = 1;
		}

	_addcol = 0;
	for( _j = 0; _j < lpCobj_All_->m; _j++ )
		if( PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MControl_type == 4 )
		{
			_addcol++;
			PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MSDirty = 0;
		}
		else
		{
//note: are all the mvs here?
			PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MSteadOptim = MGET( _lpmx, lpmG_->row+_j-_addcol, 0 );
			PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MSDirty = 1;
			PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MIrv = MGET( _lpmx, lpmG_->row+_j-_addcol, 0 );
		}
}
	MATRIXFREE( _lpmu );
	MATRIXFREE( _lpmogra );
	MATRIXFREE( _lpmcfun );
	MATRIXFREE( _lpmcgra );
	MATRIXFREE( _lpmb );
	MATRIXFREE( _lpmpk );
	MATRIXFREE( _lpmak );
	MATRIXFREE( _lpmrhs );
	MATRIXFREE( _lpmnewx );
	MATRIXFREE( _lpmu2 );
	MATRIXFREE( _lpms );
	MATRIXFREE( _lpmwr );
	MATRIXFREE( _lpmnm );
	MATRIXFREE( _lpmscl );
	MATRIXFREE( _lpmofun );
	MATRIXFREE( _lpmoldpf );
	MATRIXFREE( _lpmqpindx );
	MATRIXFREE( _lpmqpit );
	MATRIXFREE( _lpmjset );
	MATRIXFREE( _lpmmj );
	MATRIXFREE( _lpmindex );
	MATRIXFREE( _lpmnf );
	MATRIXFREE( _lpmng );
	MATRIXFREE( _lpmd1 );
	MATRIXFREE( _lpmd2 );
	MATRIXFREE( _lpmy );
	MATRIXFREE( _lpmw0 );
	MATRIXFREE( _lpmwl );
	MATRIXFREE( _lpmwq );
	MATRIXFREE( _lpmcgra1 );
	MATRIXFREE( _lpmw1 );
	MATRIXFREE( _lpmw2 );
	MATRIXFREE( _lpmw3 );
	MATRIXFREE( _lpmw4 );
	MATRIXFREE( _lpmw5 );
NonCalEnd:
	MATRIXFREE( _lpmOC );
	MATRIXFREE( _lpmK );
	MATRIXFREE( _lpmNumberOfNM );
	MATRIXFREE( _lpmx );

	return _flagError;
}
