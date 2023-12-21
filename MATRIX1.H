#include <math.h>

typedef struct _tagMatrix{
	int			row;
	int 		col;
#ifndef	__WIN32__
	BYTE		Nomeaning[4];
#endif
	double  	data[1];
}Matrix, huge* LPMATRIX;

#define	MATRIX_2_COBJ(_pm_)			\
	( (LPCOBJ)( (BYTE FAR*)(_pm_) - sizeof(IDCOBJ) - sizeof(WORD) ) )
//get matrix pointer from a Cobj pointer

#define COBJ_2_MATRIX(_lpCobj_)	( (LPMATRIX)((void far*)&(_lpCobj_->data[0])) )

//lp->data 's type should be (double huge*) , orelse some big matrix can't work well

#define 	MGET(lp, _row, _col)		\
			(*( (( double huge*)((lp)->data)) + (lp)->col*(_row) + (_col) ))

#define MATRIXFREE( lp )		\
	if( !(lp) )                 \
		MessageBox(NULL, "Error", "", MB_OK);    \
	else                        \
	{                           \
		MatrixFree( lp );       \
		(lp) = 0;               \
	}

LPMATRIX MatrixAlloc( int row, int col );
int _DLLFUNC  madd(LPMATRIX lpmResult, LPMATRIX lpma, LPMATRIX lpmb );
void _DLLFUNC MatrixFree( LPMATRIX lpm );

DWORD _DLLFUNC ObjectMatrix(LPPU lpPU_, WORD wMode_, HWND hwnd_);

