#include <windows.H>
#include <math.h>
typedef struct _tagMatrix{
	int			row;
	int 		col;
	double  	data[1];
}Matrix, * LPMATRIX;
#define 	MGET(lp, _row, _col)		\
		 (*( (( double *)((lp)->data)) + (lp)->col*(_row) + (_col) ))
LPMATRIX	MatrixAlloc( int row, int col )
{
	LPMATRIX	lpMatrix;
	int			_i, _j;

	if( row <= 0 || col <= 0 )
	{
		MessageBox( NULL, "Dimension of Matrix should be greater than 0", "ERRO", MB_OK );
		return NULL;
	}
	lpMatrix = (LPMATRIX)malloc( (long)row * (long)col * (long)sizeof(double) * 1L + sizeof(Matrix) );
	lpMatrix->row = row;
	lpMatrix->col = col;
	for(_i = 0; _i < lpMatrix->row; _i++ )
		for(_j = 0; _j < lpMatrix->col; _j++ )
			MGET( lpMatrix, _i, _j) = 0.0;

	return lpMatrix;
}

void MatrixFree( LPMATRIX lpm )
{
	free( (void*) lpm  );
	return ;
}
int minvert(LPMATRIX lpmResult, LPMATRIX lpmsrc )
{
	int			kl, _row, _col, m1, m2;


	LPMATRIX 	lpmtemp;
	double 		z;

	//check the dim, if illegal

	if(lpmsrc->row <= 0)
	{
		MessageBox(NULL, "err", "", MB_OK);
		return 0;
	}

	if(lpmsrc->row != lpmsrc->col)
	{
		MessageBox(NULL, "err", "", MB_OK);
		return 0;
	}
		//then calcul.

	lpmResult->row = lpmsrc->row;
	lpmResult->col = lpmsrc->col;

	lpmtemp = MatrixAlloc(lpmsrc->row, lpmsrc-> col);


	for(_row = 0; _row < lpmsrc->row; _row++ )
		for(_col = 0; _col < lpmsrc->col; _col++ )
		{
			MGET(lpmtemp, _row, _col) = MGET(lpmsrc, _row, _col);
			MGET(lpmResult, _row, _col) = 0.0;
			MGET(lpmResult, _row, _row) = 1.0;
		}

	for(_col = 0; _col < lpmsrc->col; _col++)
	{
		z = MGET(lpmtemp, _col, _col);
		kl=_col;
		for(m2 = _col; m2 < lpmsrc->row; m2++)
			if(fabsl(MGET(lpmtemp, m2, _col)) >fabs(z))
			{
				z = MGET(lpmtemp, m2, _col);
				kl = m2;
			}

		for(m1 = 0; m1 < lpmsrc->row; m1++)
		{
			z = MGET(lpmResult, kl, m1);
			MGET(lpmResult, kl, m1) = MGET(lpmResult, _col, m1);
			MGET(lpmResult, _col, m1) = z;
			z = MGET(lpmtemp, kl, m1);
			MGET(lpmtemp, kl, m1) = MGET(lpmtemp, _col, m1);
			MGET(lpmtemp, _col, m1) = z;
		}
		z = MGET(lpmtemp, _col, _col);

		if( (fabs)(z) <= 0.0000000001)
		{
			MatrixFree(lpmtemp);
			MessageBox(NULL, "error", "iminvertable", MB_OK);
   			return 0;
		}

		for(m1 = 0; m1 < lpmsrc->row; m1++)
		{
			MGET(lpmtemp, _col, m1) /= z;
			MGET(lpmResult, _col, m1)/=z;
		}

		for(m2 = 0; m2 <lpmsrc->row; m2++)
			if(m2 != _col)
			{
				z = MGET(lpmtemp, m2, _col);
				for(m1 = 0; m1 <lpmsrc->row; m1++)
				{
					MGET(lpmtemp, m2, m1) -= z * MGET(lpmtemp, _col, m1);
					MGET(lpmResult, m2, m1) -= z * MGET(lpmResult, _col, m1);
				}
			}
	}
	MatrixFree(lpmtemp);
	return 1;
}
