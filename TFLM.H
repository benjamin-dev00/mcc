#define mxMAXNAM        20
#define REAL_MAT	1
#define INT_MAT		2
#include "stdlib.h"
#include "stdio.h"
#include "string.h"
#include "math.h"
#include "windows.h"

typedef struct SRmatrix {
		char    name[mxMAXNAM];         /* name is now an array */
		int     row;                    /* row dimension */
		int     col;                    /* column dimension */
		double  *pr;                    /* pointer to real part */
} SRMatrix, * LPMATRIXL;

#define	SRMGET(lpmat, _row, _col)		\
		(*( (( double*)((lpmat)->pr)) + (lpmat)->col*(_row) + (_col) ))

void mxFreeMatrix( SRMatrix* pmat );
void mxZeros( int row, int col, SRMatrix *lpmat );// all zeros matrix of row * col;
void mxEye( int row, int col, SRMatrix *lpmat );	// identity matrix of row * col;
void mxOnes( int row, int col, SRMatrix *lpmat ); // all ones matrix of row * col;
int mxInvert( SRMatrix*, SRMatrix* );
SRMatrix mxExpm( SRMatrix* );
double mxNorm( SRMatrix* );
void mxFirstNonZero( SRMatrix *lpmat, int* _row, int* _col );
int mxIsZero( SRMatrix *lpmat );
void mxCopy( SRMatrix* lpmxDes, SRMatrix* lpmxSrc );
void mxMul1( SRMatrix* lpmultiplicand, double multiplicator, SRMatrix* lpmxProduct );
void mxMul2( SRMatrix* lpmxA, SRMatrix* lpmxB, SRMatrix* lpmxRes );
void mxDiv1( SRMatrix* lpmxDividend, double divisor, SRMatrix* lpmxQuotient );
void mxSub( SRMatrix* lpmxMinuend, SRMatrix* lpmxSubtrahend, SRMatrix* lpmxDifference );
void mxAdd( SRMatrix* lpmxSummand, SRMatrix* lpmxAddend, SRMatrix* lpmxSum );
void mxTrans( SRMatrix* lpmxSrc, SRMatrix* lpmxDest );
BOOL DataRead( SRMatrix* lpsNum, SRMatrix* lpsDen, SRMatrix* lpNum, SRMatrix* lpDen );
void TransToStat( SRMatrix*, SRMatrix*, SRMatrix*, SRMatrix*, SRMatrix*, SRMatrix* );
void SimToDis( SRMatrix*  , SRMatrix*  , double, SRMatrix* , SRMatrix* );
void CalStep( SRMatrix*, SRMatrix*, SRMatrix*, SRMatrix*, SRMatrix*, double, double, int );
void FreeMat( SRMatrix*, SRMatrix*, SRMatrix*, SRMatrix*, SRMatrix*, SRMatrix*, SRMatrix*, SRMatrix* );
void tf2step( SRMatrix* lpNum, SRMatrix* lpDen, double delay, double Ts, int nstep, SRMatrix* lpmxStepRes );

