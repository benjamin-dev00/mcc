#include "tfalgo.h"
#include "tfwsqp.h"
extern char LocalDirectory[128];
void output( LPCOBJPRIV lpCobj_All_, LPMATRIX lpmG_, LPCOBJPRIVMV lpCobj_PrivMV_,
				LPCOBJPRIVCV lpCobj_PrivCV_, int n, int me, int mi, int sm,
				
				LPMATRIX lpmx_, LPMATRIX lpmu_,
				LPMATRIX lpmofun_, LPMATRIX lpmcfun_, double ppra, double pfun,
				LPMATRIX lpmindex_, LPMATRIX lpmqpindx_, int it, int nqp, LPMATRIX lpmnf_,
				LPMATRIX lpmng_, int itqp)

//	int n,me,mi,sm;
//	double x[nmax],u[mamax],ofun[1],cfun[mmax],ogra[namax],cgra[namax][mamax],
//			lgra[namax],ppra,pfun;
//	int index[1],qpindx[1],it,nqp,nf[1],ng[1],itqp;

{
	int _i,_j, _addrow, _addcol;
	FILE *fout;
	LPMATRIX		_lpmxtemp;
	char			_FileName[128];

	_lpmxtemp = MatrixAlloc( n, 1 );
	for( _i = 0; _i < n; _i++ )
		MGET( _lpmxtemp, _i, 0 ) = MGET( lpmx_, _i, 0 );

//    double	_mtemp[3];
	strcpy( _FileName, LocalDirectory );
	strcat( _FileName, "tflpout.ini" );
	if ((fout=fopen( _FileName,"w"))==NULL)
	{
		MessageBox( NULL, "cannot open this file\n", "", MB_OK );
		exit(0);
	}
	fprintf(fout,"******************************************\n");
	fprintf(fout,"          computational results\n");
	fprintf(fout,"******************************************\n");
	if((int)MGET( lpmindex_, 0, 0 )==0)
		fprintf(fout,"optimal solution:\n\n");
	else if( (int)MGET( lpmindex_, 0, 0 ) == 1 )
	{	
		fprintf(fout,"index=1: the kuhn-tucker conditions are satisfied with desirable accuracy,but the amount of decrease in the penalty function value is still large after iteratio limit\n");
//		MessageBox( NULL, "ERROR", "TFWOUT", MB_ABORTRETRYIGNORE );
	}
	else if((int)MGET( lpmindex_, 0, 0 )==2)
	{	
		fprintf(fout,"index=2: the amount of decrease in the penalty function value is sufficiently small,but kuhn-tucker conditions have not been satisfied with desirable accuracy after iteration limit\n");
//		MessageBox( NULL, "ERROR", "TFWOUT", MB_ABORTRETRYIGNORE );
	}
	else if((int)MGET( lpmindex_, 0, 0 )==3)
	{	
		fprintf(fout,"index=3: not only the amount of decrease in the penlty finction value is still large but also kuhn-tucker conditions are not satisfied with desirable accuracy after iteration limit\n");
//		MessageBox( NULL, "ERROR", "TFWOUT", MB_ABORTRETRYIGNORE );
	}
	else if((int)MGET( lpmindex_, 0, 0 )==4)
	{	
		fprintf(fout,"index=4: the generated sequence is regarded as divergent\n");
//		MessageBox( NULL, "ERROR", "TFWOUT", MB_ABORTRETRYIGNORE );
	}
	else if((int)MGET( lpmindex_, 0, 0 )==5)
	{	
		fprintf(fout,"index=5: the algorithm has been terminated since the change in successive values of the variables became too small at iteration\n");
//		MessageBox( NULL, "ERROR", "TFWOUT", MB_ABORTRETRYIGNORE );
	}
	else
	{
		fprintf(fout,"index=6: failure occurs in solving qp subproblem at iteration %d,error code * qpindx=%d\n",it,(int)MGET( lpmqpindx_, 0, 0 ));
//		MessageBox( NULL, "ERROR", "TFWOUT", MB_ABORTRETRYIGNORE );
	}
	fprintf(fout,"objective finction =  %f\n", MGET( lpmofun_, 0, 0 ));
	fprintf(fout,"penalty function   =  %f\n",pfun);
	fprintf(fout,"(penalty parameter =  %f)\n\n",ppra);
	fprintf(fout,"variables:\n");
	_addrow = 0;
	for( _i = 0; _i < lpCobj_All_->p; _i++ )
		if( PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CControl_type == 4 )
			_addrow++;
		else
		{
			MGET( _lpmxtemp, _i-_addrow, 0  ) += PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CIni;
			UNUNIT( MGET( _lpmxtemp, _i-_addrow, 0  ), PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CHScale,
				PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CLScale );
		}

	_addcol = 0;
	for( _j = 0; _j < lpCobj_All_->m; _j++ )
		if( PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MControl_type == 4 )
			_addcol++;
		else
		{
//note: are all the mvs here?
				MGET( _lpmxtemp, lpmG_->row+_j-_addcol, 0 ) += PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MIni;
				UNUNIT( MGET( _lpmxtemp, lpmG_->row+_j-_addcol, 0  ), PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MHScale,
				PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MLScale );
		}
	for(_j=0;_j<n;_j++)
	{
		fprintf(fout,"x( %d ) =  %f\n\n",_j, MGET( _lpmxtemp, _j, 0 ) );
//		_mtemp[_j] = MGET( _lpmxtemp, _j, 0 );
	}
	if(me==0)
	{
		fprintf(fout,"equality constrains no equality constraint\n\n");
		goto f4;
	}
	fprintf(fout,"equality constraints:      lagrange multipliers:\n");
	for(_i=0;_i<me;_i++)
	{
	fprintf(fout,"c( %d ) = %f               u( %d ) = %f\n\n",_i,MGET( lpmcfun_, _i, 0 ),_i, MGET( lpmu_, _i, 0 ));
	}

f4:
	if(mi==0)
	{
		fprintf(fout,"problem contrains no inequality constraint\n\n");
		goto s6;
	}
	fprintf(fout,"inequality constraints:     lagrange multipliers:\n");
	for(_i=me;_i<sm;_i++)
	{
	fprintf(fout,"c( %d ) = %f                u( %d ) = %f\n\n",_i,MGET( lpmcfun_, _i, 0 ),_i, MGET( lpmu_, _i, 0 ));
	}

s6:
	fprintf(fout,"number of major iterations   = %d\n",it);
	fprintf(fout,"number of qp problems solved = %d\n",nqp);
	fprintf(fout,"number of qp iterations =      %d\n",itqp);
	fprintf(fout,"number ot function valuations= %d\n",(int)MGET( lpmnf_, 0, 0 ) );
	fprintf(fout,"number of gradient valuations= %d\n",(int)MGET( lpmng_, 0, 0 ) );
	fprintf(fout,"number of iteration %d\n", it );
	fclose(fout);
	MATRIXFREE( _lpmxtemp );
    return;
}

