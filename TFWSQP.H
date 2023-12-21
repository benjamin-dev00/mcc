#include "tfcmdmc.h"
#define nmax 	30
#define memax 	30
#define mimax 	50
#define mmax 	memax+mimax
#define namax	nmax+2*memax+mimax
#define mamax 	3*memax+2*mimax
#define zero 	1.0e-8
#define itmax	10

void bupd(int n, int mi, LPMATRIX lpmOC_, LPMATRIX lpmK_,
			LPMATRIX lpmx_, LPMATRIX lpmnewx_,
			LPMATRIX lpmogra_, LPMATRIX lpmcgra_, LPMATRIX lpmu_, LPMATRIX lpmb_,
			LPMATRIX lpms_, LPMATRIX lpmy_, LPMATRIX lpmbs_, LPMATRIX lpmlg_,
			LPMATRIX lpmlgnew_, LPMATRIX lpmw3_, LPMATRIX lpmw4_,
			LPMATRIX lpmw5_, int grad, int it, LPMATRIX lpmng_,LPMATRIX lpmnf_ );

double lsearc(int n, int me, int mi, int sm_, LPMATRIX lpmOC_,
				LPMATRIX lpmK_, LPMATRIX lpmx_, LPMATRIX lpmnewx_,
				LPMATRIX lpmd1_, LPMATRIX lpmd2_, int basic_, LPMATRIX lpmofun_,
				LPMATRIX lpmcfun_, double qpobj_, double ppra_, double pfun_,
				LPMATRIX lpmoldpf_, double eps_, LPMATRIX lpmindex_, LPMATRIX lpmnf_);

double qpval(int n, int na, LPMATRIX lpmogra_, LPMATRIX lpmb_,
		LPMATRIX lpmd1_, double ppra_, double qpobj_, int feasbl );

int check(int n, int mi, int sm, LPMATRIX lpmOC_, LPMATRIX lpmK_, LPMATRIX lpmx_,
			LPMATRIX lpmcfun_, LPMATRIX lpmcgra_, LPMATRIX lpmcgra1_,
			LPMATRIX lpmd1_, LPMATRIX lpmak_, LPMATRIX lpmw0_, LPMATRIX lpmw1_,
			LPMATRIX lpmw2_, LPMATRIX lpmw3_, LPMATRIX lpmw4_,
			LPMATRIX lpmcon1_, int grad, int corr, LPMATRIX lpmnf_,
			LPMATRIX lpmng_ );

void auxqp( int n, int me, int mi, int sm, int na, 
			LPMATRIX lpmcgra_, LPMATRIX lpmrhs_, LPMATRIX lpmogra_,
			LPMATRIX lpmb_, double ppra);

int contes(int n, int me, int sm, LPMATRIX lpmx_, LPMATRIX lpmu_,
	double pfun_, LPMATRIX lpmoldpf_, LPMATRIX lpmcfun_, LPMATRIX lpmogra_,
	LPMATRIX lpmcgra_, LPMATRIX lpmlgra_, double bound, double eps1,
	double eps2, LPMATRIX lpmindex_ );

double pval(int me, int mi, LPMATRIX lpmofun_,
			LPMATRIX lpmcfun_, double ppra_, double pfun_);

void dif( int mi, int sm, LPMATRIX lpmOC_, LPMATRIX lpmK_, LPMATRIX lpmx_,
		LPMATRIX lpmogra_, LPMATRIX lpmcgra_, LPMATRIX lpmy_,
		LPMATRIX lpmcy1_, LPMATRIX lpmcy2_, LPMATRIX lpmnf_ );

void gval( int sm_, LPMATRIX lpmOC_, LPMATRIX lpmx_,
		LPMATRIX lpmogra_, LPMATRIX lpmcgra_ );

void fval( int n_, int sm_, LPMATRIX lpmOC_, LPMATRIX lpmK_, LPMATRIX lpmx_,LPMATRIX lpmofun_, LPMATRIX lpmcfun_ );

void output( LPCOBJPRIV lpCobj_All, LPMATRIX lpmG_, LPCOBJPRIVMV lpCobj_PrivMV_,
			 LPCOBJPRIVCV lpCobj_PrivCV_, int n, int me, int mi, int sm,
			 LPMATRIX lpmx_, LPMATRIX lpmu_,
		LPMATRIX lpmofun_, LPMATRIX lpmcfun_, double ppra, double pfun,
		LPMATRIX lpmindex_, LPMATRIX lpmqpindx_, int it, int nqp, LPMATRIX lpmnf_,
		LPMATRIX lpmng_, int itqp);

void input(int n, int me, int mi );

void qpsolv(int n, int me, int sm, LPMATRIX lpma_, LPMATRIX lpmb_,
			LPMATRIX lpmc_, LPMATRIX lpmg_, LPMATRIX lpmx_,
			LPMATRIX lpmv_, LPMATRIX lpml_, LPMATRIX lpmq_, LPMATRIX lpmw_,
			LPMATRIX lpmas_, LPMATRIX lpmdv_, LPMATRIX lpmzv_,
			LPMATRIX lpmrv_, LPMATRIX lpmr_, LPMATRIX lpmnm_, LPMATRIX lpmjset_,
			LPMATRIX lpmmj_, LPMATRIX lpmqpindx_, LPMATRIX lpmit_,
			int scale, LPMATRIX lpmscl_ );

int drop(int n, int nj, LPMATRIX lpmmj_, LPMATRIX lpmjset_,
			LPMATRIX lpmr_, LPMATRIX lpmnm_, LPMATRIX lpmjk_, LPMATRIX lpmjp_);

double rtest( int me,int nj, LPMATRIX lpmv_, LPMATRIX lpmrv_,
			LPMATRIX lpmjset_, double t, LPMATRIX lpmjk_, LPMATRIX lpmjp_ );

int chkopt(int n, int me, int sm, LPMATRIX lpma_, LPMATRIX lpmb_,
			LPMATRIX lpmx_, LPMATRIX lpmmj_, int js, LPMATRIX lpmus_,
			LPMATRIX lpmoptm_ );

int add( int n, int sm, int nj, LPMATRIX lpmmj_, LPMATRIX lpmjset_,
			LPMATRIX lpmr_, LPMATRIX lpmnm_, LPMATRIX lpmdv_, int js);

int initia(int n, int me, int sm, LPMATRIX lpma_, LPMATRIX lpmb_,
			LPMATRIX lpmc_, LPMATRIX lpmg_, LPMATRIX lpmx_, LPMATRIX lpmv_,
			LPMATRIX lpmmj_, LPMATRIX lpmjset_, int nj, LPMATRIX lpmr_,
			LPMATRIX lpmnm_, LPMATRIX lpml_, LPMATRIX lpmw_, LPMATRIX lpmq_,
			LPMATRIX lpmqpindx_);

void bwdsub( int n, LPMATRIX lpml_, int trans, LPMATRIX lpmw_ );

void fwdsub(int n, LPMATRIX lpml_, int trans, LPMATRIX lpmw_ );

int qrdec( int n, int sm, LPMATRIX lpmr_, LPMATRIX lpmq_ );

void chodec(int n, LPMATRIX lpmg_, LPMATRIX lpml_, LPMATRIX lpmqpindx_ );

void qpout(int n, int sm, LPMATRIX lpmg_, LPMATRIX lpmc_, LPMATRIX lpma_,
		LPMATRIX lpmb_, LPMATRIX lpmv_, double oscl, LPMATRIX lpmscl_);

double scal( int n, int sm, LPMATRIX lpmg_, LPMATRIX lpmc_, LPMATRIX lpma_,
			LPMATRIX lpmb_, LPMATRIX lpmscl_, LPMATRIX lpmqpindx_);

void 	ParameterInitial1( LPCOBJPRIV lpCobj_All_, LPMATRIX lpmNumberOfNM, LPMATRIX lpmG,
									LPCOBJPRIVMV lpCobj_PrivMV_, LPCOBJPRIVCV lpCobj_PrivCV_ );

void 	ParameterInitial2( LPCOBJPRIV lpCobj_All_, LPMATRIX lpmx_,
						LPMATRIX lpmOC_, LPMATRIX lpmK_,
						LPMATRIX lpmG, LPCOBJPRIVMV lpCobj_PrivMV_, LPCOBJPRIVCV lpCobj_PrivCV_,
						LPCOBJPRIVDV  _lpPrivDV );

WORD	sqp( LPCOBJPRIV lpCobj_All_, LPMATRIX lpmG_, LPCOBJPRIVMV lpCobj_PrivMV_,
				LPCOBJPRIVCV lpCobj_PrivCV_, LPCOBJPRIVDV lpCobj_PrivDV_ );




