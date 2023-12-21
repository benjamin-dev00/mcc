//addtemp
#include "tfalgo.h"
#include "tflm.h"

void mxFirstNonZero( SRMatrix *lpmat, int *row, int *col )
{
	double _zero = 0;
	int _row, _col;
	for ( _row=0; _row<lpmat->row; _row++ )
	{
		for ( _col=0; _col<lpmat->col; _col++ )
		{
			if ( SRMGET( lpmat, _row, _col ) != _zero )
				break;
		}
	}
	*row = _row-1;
	*col = _col;
	return;
}

int mxIsZero( SRMatrix *lpmat )
{
	int _row, _col;
	for ( _row=0; _row<lpmat->row; _row++ )
	{
		for ( _col=0; _col<lpmat->col; _col++ )
		{
			if ( SRMGET( lpmat, _row, _col ) != 0 )
				return 0;
		}
	}
	return 1;
}


void mxFreeMatrix( SRMatrix* pmat )
{
	if ( pmat != NULL)
		free( (*pmat).pr );
	return;
}

void mxZeros( int row, int col, SRMatrix *lpmat )
{
	int i = 0, j = 0;
	if ( lpmat == NULL )
	{
//		MessageBox(NULL, "Null matrix pointer!", "err", MB_OK);
		return;
	}

	if( (row<=0) | (col<=0) )
	{
		lpmat->pr = NULL;
		lpmat->row = 0;
		lpmat->col = 0;
//		MessageBox(NULL, "col and row must great than 0!", "err", MB_OK);
		return;
	}

	lpmat->row = row;
	lpmat->col = col;

	lpmat->pr = (double *) malloc( row * col * sizeof(double) );
	for ( i = 0; i < row; i++ )
	{
		for ( j= 0; j < col; j++ )
		{
			*( lpmat->pr+i*col+j ) = 0 ;
		}
	}
	return ;
}

void mxOnes( int row, int col, SRMatrix *lpmat )
{
	int i = 0, j = 0;
	if ( lpmat == NULL )
	{
//		MessageBox(NULL, "Null matrix pointer!", "err", MB_OK);
		return;
	}

	if( (row<=0) | (col<=0) )
	{
		lpmat->pr = NULL;
		lpmat->row = 0;
		lpmat->col = 0;
//		MessageBox(NULL, "col and row must great than 0!", "err", MB_OK);
		return;
	}

	lpmat->row = row;
	lpmat->col = col;
	lpmat->pr = (double *) malloc( row * col * sizeof(double) );
	for ( i = 0; i < row; i++ )
	{
		for ( j = 0; j < col; j++ )
		{
			*( lpmat->pr+i*col+j ) = 1.0 ;
		}
	}
	return;
}

void mxEye( int row, int col, SRMatrix *lpmat )
{
	int i = 0, j = 0;
	if ( lpmat == NULL )
	{
//		MessageBox(NULL, "Null matrix pointer!", "err", MB_OK);
		return;
	}

	if( (row<=0) | (col<=0) )
	{
//		MessageBox(NULL, "col and row must great than 0!", "err", MB_OK);
		lpmat->pr = NULL;
		lpmat->row = 0;
		lpmat->col = 0;
		return;
	}

	lpmat->row = row;
	lpmat->col = col;

	lpmat->pr = (double *) malloc( row * col * sizeof(double) );
	for ( i = 0; i < row; i++ )
	{
		for ( j = 0; j< col; j++ )
		{
			if ( j == i )
				*( lpmat->pr+i*col+j ) = 1 ;
			else
				*( lpmat->pr+i*col+j ) = 0 ;
		}
	}
	return;
}


int mxInvert(SRMatrix* lpmxSrc, SRMatrix* lpmxResult )
{
	int	  	kl, _row, _col, m1, m2;

	SRMatrix 	mxTemp, mxTemp2;
	double	z;

	//check the dim, if illegal

	if ( (lpmxSrc==NULL) | (lpmxResult==NULL) )
	{
//		MessageBox(NULL, "Null matrix pointer!", "err", MB_OK);
		return 0;
	}

	if(lpmxSrc->row <= 0)
	{
//		MessageBox(NULL, "Row and col must greater than 0!", "err", MB_OK);
		return 0;
	}

	if((lpmxSrc->row != lpmxSrc->col)|(lpmxResult->row != lpmxResult->col))
	{
//		MessageBox(NULL, "not square matrix!!!", "err", MB_OK);
		return 0;
	}
	//then calcul.

	mxZeros(lpmxSrc->row, lpmxSrc->col, &mxTemp );
	mxZeros(lpmxSrc->row, lpmxSrc->col, &mxTemp2 );
	mxCopy( &mxTemp2, lpmxResult );

	for(_row = 0; _row < lpmxSrc->row; _row++ )
		for(_col = 0; _col < lpmxSrc->col; _col++ )
		{
			SRMGET(&mxTemp, _row, _col) = SRMGET(lpmxSrc, _row, _col);
			SRMGET(&mxTemp2, _row, _col) = 0.0;
			SRMGET(&mxTemp2, _row, _row) = 1.0;
		}

	for(_col = 0; _col < lpmxSrc->col; _col++)
	{
		z = SRMGET(&mxTemp, _col, _col);
		kl=_col;
		for(m2 = _col; m2 < lpmxSrc->row; m2++)
			if(fabsl(SRMGET(&mxTemp, m2, _col)) >fabs(z))
			{
				z = SRMGET(&mxTemp, m2, _col);
				kl = m2;
			}

		for(m1 = 0; m1 < lpmxSrc->row; m1++)
		{
			z = SRMGET(&mxTemp2, kl, m1);
			SRMGET(&mxTemp2, kl, m1) = SRMGET(&mxTemp2, _col, m1);
			SRMGET(&mxTemp2, _col, m1) = z;
			z = SRMGET(&mxTemp, kl, m1);
			SRMGET(&mxTemp, kl, m1) = SRMGET(&mxTemp, _col, m1);
			SRMGET(&mxTemp, _col, m1) = z;
		}
		z = SRMGET(&mxTemp, _col, _col);

		if( (fabs)(z) <= 0.0000000001)
		{
			mxFreeMatrix(&mxTemp);
//			MessageBox(NULL, "error", "iminvertable", MB_OK);
			return 0;
		}

		for(m1 = 0; m1 < lpmxSrc->row; m1++)
		{
			SRMGET(&mxTemp, _col, m1) /= z;
			SRMGET(&mxTemp2, _col, m1)/=z;
		}

		for(m2 = 0; m2 <lpmxSrc->row; m2++)
			if(m2 != _col)
			{
				z = SRMGET(&mxTemp, m2, _col);
				for(m1 = 0; m1 <lpmxSrc->row; m1++)
				{
					SRMGET(&mxTemp, m2, m1) -= z * SRMGET(&mxTemp, _col, m1);
					SRMGET(&mxTemp2, m2, m1) -= z * SRMGET(&mxTemp2, _col, m1);
				}
			}
	}
	mxCopy( lpmxResult, &mxTemp2 );
	mxFreeMatrix(&mxTemp);
	mxFreeMatrix(&mxTemp2);
	return 1;
}

double mxNorm( SRMatrix* lpmxSrc )
/* Calculate the MAXIMUM of the accumulate of ABS of row.
max(sum(abs((*lpmxSrc'))))
*/
{
	SRMatrix mxTemp;
	int _row, _col;
	double fRes;

	if ( lpmxSrc == NULL )
	{
//		MessageBox(NULL, "Null matrix pointer!", "err", MB_OK);
		return 0;
	}
	if( (lpmxSrc->row<=0) | (lpmxSrc->col<=0) )
	{
//		MessageBox(NULL, "col and row must great than 0!", "err", MB_OK);
		lpmxSrc->pr = NULL;
		lpmxSrc->row = 0;
		lpmxSrc->col = 0;
		return 0;
	}


	mxZeros( lpmxSrc->row, 1, &mxTemp );
	fRes = 0;
	for ( _row = 0; _row < lpmxSrc->row; _row++ )
	{
		SRMGET( &mxTemp, _row, 0 ) = 0;
		for ( _col = 0; _col < lpmxSrc->col; _col++ )
			SRMGET( &mxTemp, _row, 0 ) += fabs( SRMGET( lpmxSrc, _row, _col ) );
	}
	for ( _row = 0; _row < mxTemp.row; _row++ )
	{
		fRes = max( fRes, SRMGET( &mxTemp, _row, 0 ) );
	}

	mxFreeMatrix( &mxTemp );

	return fRes;
}

void mxCopy( SRMatrix* lpmxDes, SRMatrix* lpmxSrc )
{
	int _row, _col;

	if ( (lpmxSrc==NULL) | (lpmxDes==NULL) )
	{
//		MessageBox(NULL, "Null matrix pointer!", "err", MB_OK);
		return;
	}
	if( (lpmxSrc->row) * (lpmxSrc->col) != (lpmxDes->col) * (lpmxDes->row) )
	{
//		MessageBox(NULL, "SRMatrix size must be coinsident!", "err", MB_OK);
		return;
	}


	lpmxDes->row = lpmxSrc->row;
	lpmxDes->col = lpmxSrc->col;

	for ( _row = 0; _row < lpmxSrc->row; _row++ )
	{
		for( _col = 0; _col < lpmxSrc->col; _col++ )
		{
			SRMGET( lpmxDes, _row, _col ) = SRMGET( lpmxSrc, _row, _col );
		}
	}

	return ;
}


void mxDiv1( SRMatrix* lpmxDividend, double fDivisor, SRMatrix* lpmxQuotient )
// mxQuotient = mxDividend / fDivisor;
{
	int _row, _col;
	SRMatrix mxTemp;

	mxZeros( lpmxQuotient->row, lpmxQuotient->col, &mxTemp );

	for ( _row = 0; _row < lpmxDividend->row; _row++ )
	for ( _col = 0; _col < lpmxDividend->col; _col++ )
		SRMGET( &mxTemp, _row, _col ) = SRMGET( lpmxDividend, _row, _col )/fDivisor;

	mxCopy( lpmxQuotient, &mxTemp );
	mxFreeMatrix( &mxTemp );
	return;
}

void mxMul1( SRMatrix* lpmxMultiplicand, double fMultiplicator, SRMatrix* lpmxProduct )
// mxProduct = mxMultiplicand * fMultiplication;
{
	int _row, _col;
	SRMatrix mxTemp;

	mxZeros( lpmxProduct->row, lpmxProduct->col, &mxTemp );

	for ( _row = 0; _row < lpmxMultiplicand->row; _row++ )
	for ( _col = 0; _col < lpmxMultiplicand->col; _col++ )
		SRMGET( &mxTemp, _row, _col ) = SRMGET( lpmxMultiplicand, _row, _col )*fMultiplicator;

	mxCopy( lpmxProduct, &mxTemp );
	mxFreeMatrix( &mxTemp );
	return;
}

void mxMul2( SRMatrix* lpmxA, SRMatrix* lpmxB, SRMatrix* lpmxRes )
// *lpmxRes = (*lpmxA) * (*lpmxB)
{
	int		_row, _col, _temp;
	SRMatrix	mxTemp;


	//check the dim, if illegal
	if((lpmxA->row < 0) | (lpmxA->col < 0))
	{
//		MessageBox(NULL, "err", "", MB_OK);
		return;
	}

	if(lpmxA->col != lpmxB->row)
	{
//		MessageBox(NULL, "err", "", MB_OK);
		return;
	}

	//then calcul.
	mxZeros( lpmxA->row, lpmxB->col, &mxTemp );

	for( _row = 0; _row < lpmxA->row; _row ++ )
		for(_col = 0; _col<lpmxB->col; _col++)
		{
			for(_temp=0; _temp < lpmxA->col; _temp++)
			{
				SRMGET( &mxTemp, _row, _col) += SRMGET(lpmxA, _row, _temp) * SRMGET(lpmxB, _temp, _col);
			}
		}
	mxCopy( lpmxRes, &mxTemp );
	mxFreeMatrix( &mxTemp );

	return;
}

void mxAdd( SRMatrix* lpmxSummand, SRMatrix* lpmxAddend, SRMatrix* lpmxSum )
{
	int _row, _col;
	for ( _row = 0; _row < lpmxSummand->row; _row++ )
	for ( _col = 0; _col < lpmxSummand->col; _col++ )
		SRMGET( lpmxSum, _row, _col ) = SRMGET( lpmxSummand, _row, _col )+SRMGET( lpmxAddend, _row, _col );
	return;
}

void mxSub( SRMatrix* lpmxMinuend, SRMatrix* lpmxSubtrahend, SRMatrix* lpmxRes )
{
	int _row, _col;
	for ( _row = 0; _row < lpmxMinuend->row; _row++ )
	for ( _col = 0; _col < lpmxMinuend->col; _col++ )
		SRMGET( lpmxRes, _row, _col ) = SRMGET( lpmxMinuend, _row, _col )-SRMGET( lpmxSubtrahend, _row, _col );
	return;
}

SRMatrix mxExpm( SRMatrix* lpmxSrc )
/*  SRMatrix exponential via Pade approximation.
[f,e] = log2(norm(A,'inf'));
s = max(0,e+1);
A = A/2^s;

% Pade approximation for exp(A)
X = A;
c = 1/2;
E = eye(size(A)) + c*A;
D = eye(size(A)) - c*A;
q = 6;
p = 1;
for k = 2:q
   c = c * (q-k+1) / (k*(2*q-k+1));
   X = A*X;
   cX = c*X;
   E = E + cX;
   if p
	 D = D + cX;
   else
	 D = D - cX;
   end
   p = ~p;
end
E = D\E;

% Undo scaling by repeated squaring
for k=1:s, E = E*E; end
*/
{
	int _row = lpmxSrc->row;
	int	iss;
	SRMatrix AX, X, E, D, CX, mxTemp, _eye, mxTemp2;
	double s;
	double c = 0.5;
	int q = 6,p = 1,k;

	mxZeros( _row, _row, &AX );
	mxZeros( _row, _row, &X );
	mxZeros( _row, _row, &E );
	mxZeros( _row, _row, &D );
	mxZeros( _row, _row, &CX );
	mxZeros( _row, _row, &mxTemp );
	mxZeros( _row, _row, &mxTemp2 );

	if ( lpmxSrc->row != lpmxSrc->col )
//		MessageBox(NULL, "Not square matrix", "err", MB_OK);

	s = mxNorm( lpmxSrc );
	if( s>0 )
		iss = (int)max( 0, floor( log(s)/log(2) )+2 );
	else
		iss = (int)s;
	mxDiv1( lpmxSrc, pow(2,iss), &AX );
	mxCopy( &X, &AX );
	mxEye( _row, _row, &_eye );
	mxMul1( &AX, c, &mxTemp );
	mxAdd( &_eye, &mxTemp, &E );
	mxSub( &_eye, &mxTemp, &D );
	for ( k = 2; k < q; k++ )
	{
		c = c*( q+1-k )/(k*(2*q+1-k));
		mxMul2( &AX, &X, &X );
		mxMul1( &X, c, &CX);
		mxAdd( &E, &CX, &E);
		if(p)
			mxAdd( &D, &CX, &D );
		else
			mxSub( &D, &CX, &D );
		p = 1 - p;
	}
	mxInvert( &D, &mxTemp2 );
	mxMul2( &mxTemp2, &E, &E );

	for( k = 1; k <= iss; k++ )
	{
		mxMul2( &E, &E, &E );
	}

	mxFreeMatrix( &AX );
	mxFreeMatrix( &X );
	mxFreeMatrix( &D );
	mxFreeMatrix( &CX );
	mxFreeMatrix( &_eye );
	mxFreeMatrix( &mxTemp );
	mxFreeMatrix( &mxTemp2 );

	return E;
}

void mxTrans( SRMatrix* lpmxSrc, SRMatrix* lpmxDest )
{
	SRMatrix mxTemp;
	int	_row,_col;

	mxZeros( lpmxSrc->col, lpmxSrc->row, &mxTemp );
	for( _row=0; _row<lpmxSrc->row; _row++ )
	{
		for(_col=0; _col<lpmxSrc->col; _col++ )
		{
			SRMGET( &mxTemp, _col, _row ) = SRMGET( lpmxSrc, _row, _col );
		}
	}
	mxCopy( lpmxDest, &mxTemp );
	mxFreeMatrix( &mxTemp );
	return;
}

//SRMatrix A,B,C,D,AA,BB,mxStep,X,X0,U,XT,CT,DT;
void FreeMat( SRMatrix* lpmxNum, SRMatrix* lpmxDen, SRMatrix* lpmxA, SRMatrix* lpmxB, SRMatrix* lpmxC, SRMatrix* lpmxD, SRMatrix* lpmxAA, SRMatrix* lpmxBB )
{
	mxFreeMatrix( lpmxNum );
	mxFreeMatrix( lpmxDen );
	mxFreeMatrix( lpmxA );
	mxFreeMatrix( lpmxB );
	mxFreeMatrix( lpmxC );
	mxFreeMatrix( lpmxD );
	mxFreeMatrix( lpmxAA );
	mxFreeMatrix( lpmxBB );
	return;
}

BOOL DataRead( SRMatrix* lpsNum, SRMatrix* lpsDen, SRMatrix* lpNum, SRMatrix* lpDen )	// read in transfer and make them the same rank.
/*
	sNum 	: numerator ,Row vector
	sDen	: Denominator ,Row vector
*/
{
	int	i=0, n=0, m=0;
	int nrow, ncol;
	int drow, dcol;
	SRMatrix tempNum, tempDen;

	mxFirstNonZero( lpsNum, &nrow, &ncol );
	mxFirstNonZero( lpsDen, &drow, &dcol );

	if ( (lpsNum->col-ncol) > (lpsDen->col-dcol) )
	{
		MessageBox(NULL, "the Rank of Den must higher than Num!", "err", MB_OK);
		return FALSE;
	}
	n = lpsDen->col-dcol;
	m = lpsNum->col-ncol;
	mxZeros( 1, m, &tempNum );
	mxZeros( 1, n, &tempDen );


	for ( i=0; i<m; i++ )
	{
		SRMGET( &tempNum, tempNum.row-1, i ) = SRMGET(lpsNum, 0, ncol+i);
	}

	for ( i=0; i<n; i++ )
	{
		SRMGET( &tempDen, tempDen.row-1, i ) = SRMGET(lpsDen, 0, dcol+i);
	}

	mxZeros( 1, n, lpNum );
	mxZeros( 1, n, lpDen );
	for ( i = 0; i < n; i++ )
	{
		if ( i >= n-m )
		{
			SRMGET( lpNum, lpNum->row-1, i ) = SRMGET(&tempNum, 0, i-(n-m));
//			*(lpNum->pr+i) = *(tempNum.pr+i-n+m);
		}
	}
	for ( i = 0; i < n; i++ )
	{
//		*(lpDen->pr+i) = *(tempDen.pr+i);
		SRMGET( lpDen, lpDen->row-1, i ) = SRMGET( &tempDen, 0, i );
	}
	mxFreeMatrix( &tempNum );
	mxFreeMatrix( &tempDen );
	return TRUE;
}


void TransToStat(SRMatrix* lpmxNum, SRMatrix* lpmxDen, SRMatrix* lpmxA, SRMatrix* lpmxB, SRMatrix* lpmxC, SRMatrix* lpmxD )
/*
Den = Den(1,2:n)/Den(1,1);
Num = Num/Den(1,1);
A = [-Den; eye(n-2,n-1)];
B = eye(n-1,1);
C = Num(:,2:n) - Num(:,1)*Den;
D = Num(:,1);
*/
{
	int i=0, j=0, n=0;
	SRMatrix	tempEye,DDen;
	n = lpmxDen->col ;

	for ( i = 1; i < n; i++ )
	{
		*(lpmxDen->pr+i) = (*(lpmxDen->pr+i)) / (*(lpmxDen->pr+0));
	}
	for ( i = 0; i < n; i++ )
	{
		*(lpmxNum->pr+i) = (*(lpmxNum->pr+i)) / (*(lpmxDen->pr+0));
	}

	mxZeros( 1, n-1, &DDen );
	for ( i = 0; i < n-1; i++ )
	{
		*(DDen.pr+i) = *(lpmxDen->pr+i+1);
	}

	mxEye( n-2, n-1, &tempEye );
	mxZeros( n-1, n-1, lpmxA );
	for ( i = 0; i < n-1; i++ )
	{
		*(lpmxA->pr+i) = -*( DDen.pr+i );
	}
	for ( i = 0; i < n-2; i++ )
	{
		for (j = 0; j < n-1; j++ )
		{
			*( lpmxA->pr+(i+1)*(n-1)+j ) = *( tempEye.pr+i*(n-1)+j );
		}
	}

	mxEye( n-1, 1, lpmxB );

	mxZeros( 1, n-1, lpmxC );
	for ( i = 0; i < n-1; i++ )
	{
		*(lpmxC->pr+i) = *( lpmxNum->pr+i+1 ) - ( *( lpmxNum->pr ) ) * ( *(DDen.pr+i) );
	}

	mxZeros( 1, 1, lpmxD );
	*( lpmxD->pr ) = *( lpmxNum->pr );

	mxFreeMatrix( &tempEye );
	mxFreeMatrix( &DDen );

	return;
}

void SimToDis( SRMatrix* lpmxA, SRMatrix* lpmxB, double T, SRMatrix* lpmxAA, SRMatrix* lpmxBB )
/*
	X'= Ax+Bu
	-> X[n+1] = AA*X[n] + BB*u[n];
	AA = S(1:na,1:na);	// na : col of A;
	BB = S(1:na,na+1:na+nb); // nb : col of B;
	S = expm([[A B]*T; zeros(nb,n+nb)]);
*/
{
	SRMatrix S;
	SRMatrix mxTemp;
	int _row=0, _col=0;

	mxZeros( lpmxA->col+lpmxB->col, lpmxA->col+lpmxB->col, &mxTemp );

	for ( _row = 0; _row < lpmxA->row; _row++ )
	{
		for( _col=0; _col<mxTemp.col; _col++ )
		{
			if( _col<lpmxA->col )
				SRMGET( &mxTemp, _row, _col ) = SRMGET( lpmxA, _row, _col )*T;
			else
				SRMGET( &mxTemp, _row, _col ) = SRMGET( lpmxB, _row, _col-lpmxA->col )*T;
		}
	}

	S = mxExpm( &mxTemp );

	//AA = S(1:na,1:na);	// na : col of A;
	mxZeros( lpmxA->col, lpmxA->col, lpmxAA );
	for ( _row=0; _row<lpmxA->col; _row++ )
	{
		for ( _col=0; _col<lpmxA->col; _col++ )
		{
			SRMGET( lpmxAA, _row, _col ) = SRMGET( &S, _row, _col );
		}
	}

	//BB = S(1:na,na+1:na+nb); // nb : col of B;
	mxZeros( lpmxA->col, lpmxB->col, lpmxBB );
	for ( _row=0; _row<lpmxA->col; _row++ )
	{
		for ( _col=0; _col<lpmxB->col; _col++ )
		{
			SRMGET( lpmxBB, _row, _col ) = SRMGET( &S, _row, _col+lpmxA->col );
		}
	}

	mxFreeMatrix( &mxTemp );
	mxFreeMatrix( &S );
	return;
}

void CalStep(  SRMatrix* lpmxAA, SRMatrix* lpmxBB, SRMatrix* lpmxC, SRMatrix* lpmxD, SRMatrix* lpmxStep, double sDelay, double Ts, int nstep )
{
	int _row,_col;
	SRMatrix mxTemp,mxTemp2,mxTemp3,X0,X,XT,U,CT,DT;
	int dDelay;

	mxZeros( lpmxAA->col, 1, &X0 );
	mxZeros( lpmxAA->col, nstep, &X );
	mxZeros( nstep, lpmxAA->row, &XT );
	mxOnes( nstep, 1, &U );
//	mxZeros( nstep, 1, lpmxStep );
	mxZeros( lpmxC->col, lpmxC->row, &CT );
	mxZeros( lpmxD->col, lpmxD->row, &DT );
	mxZeros( lpmxAA->row, 1, &mxTemp );
	mxZeros( lpmxAA->row, 1, &mxTemp2 );
	mxZeros( nstep, 1, &mxTemp3 );

	for ( _col=0; _col<nstep; _col++ )
	{
		for ( _row=0; _row<X.row; _row++ )
		{
			SRMGET( &X, _row, _col ) = SRMGET( &X0, _row, 0 );
		}
		mxMul2( lpmxAA, &X0, &mxTemp );
		mxMul1( lpmxBB, SRMGET( &U, _col, 0 ), &mxTemp2 );
		mxAdd( &mxTemp, &mxTemp2, &X0 );
	}
	mxFreeMatrix( &mxTemp );
	mxFreeMatrix( &mxTemp2 );

	mxTrans( &X, &XT );
	mxTrans( lpmxC, &CT );
	mxTrans( lpmxD, &DT );


	// y=x*c.'+ ones(n,1)*d.';
	mxZeros( nstep, CT.col, &mxTemp );
	mxZeros( nstep, DT.col, &mxTemp2 );
	mxMul2( &XT, &CT, &mxTemp );// ?????
	mxMul2( &U, &DT, &mxTemp2 );//??????
	mxAdd( &mxTemp, &mxTemp2, lpmxStep );

	dDelay = (int)ceil( sDelay/Ts );
	for ( _row = 0; _row<lpmxStep->row-dDelay; _row++ )
	{
		SRMGET( &mxTemp3, _row+dDelay, 0 ) = SRMGET( lpmxStep, _row, 0 );
	}
	mxCopy( lpmxStep, &mxTemp3 );

	mxFreeMatrix( &mxTemp );
	mxFreeMatrix( &mxTemp2 );
	mxFreeMatrix( &X0 );
	mxFreeMatrix( &XT );
	mxFreeMatrix( &X );
	mxFreeMatrix( &U );
	mxFreeMatrix( &CT );
	mxFreeMatrix( &DT );
	return ;
}

/*
NOTE:
	sNum		: matrix of 1 x m
	sDen		: matrix of 1 x n
	mxStepRes	: matrix of nstep x 1
	sDelay		: continuous time, not discrete time
*/
void tf2step( SRMatrix* lpsNum, SRMatrix* lpsDen, double sDelay, double Ts, int nstep, SRMatrix* lpmxStepRes )
{
	SRMatrix Num,Den,A,B,C,D,AA,BB;
	int i,j;

	for ( i = 0; i < lpmxStepRes->row; i++ )
	{
		for( j=0; j<lpmxStepRes->col; j++ )
		{
            SRMGET( lpmxStepRes, i, j ) = 0;
		}
	}

	if ( mxIsZero( lpsNum ) )
	{
//		mxZeros( lpmxStepRes->row, lpmxStepRes->col, lpmxStepRes );
		return;
	}
	if ( DataRead( lpsNum, lpsDen, &Num, &Den ) )
	{
		TransToStat( &Num, &Den, &A, &B, &C, &D );
		SimToDis( &A, &B, Ts, &AA, &BB );
		CalStep( &AA, &BB, &C, &D, lpmxStepRes, sDelay, Ts, nstep );
		FreeMat( &Num, &Den, &A, &B, &C, &D, &AA, &BB );
		return;
	}
	return;
}

