#ifndef __CMATRIX_H
#define __CMATRIX_H

#include <math.h>

typedef struct _tagMatrix{
	int			row;
	int 		col;
#ifndef	__WIN32__
	BYTE		Nomeaning[4];
#endif
	double  	data[1];
}Matrix, * LPMATRIX;

#define	MATRIX_2_COBJ(_pm_)			\
    ( _pm_ == NULL ) ? NULL : ( (LPCOBJ)( (BYTE FAR*)(_pm_) ) )
//get matrix pointer from a Cobj pointer

#define COBJ_2_MATRIX(_lpCobj_)	\
	( _lpCobj_ == NULL ) ? NULL : ( (LPMATRIX)((void far*)&(_lpCobj_->data[0])) )

//lp->data 's type should be (double *) , orelse some big matrix can't work well

#define 	MGET(lp, _row, _col)		\
		 (*( (( double *)((lp)->data)) + (lp)->col*(_row) + (_col) ))

#define MATRIXFREE( lp )		\
	if( !(lp) )                 \
		MessageBox(NULL, "Error", "MATRIXFREE", MB_OK);    \
	else                        \
	{                           \
		MatrixFree( lp );       \
		(lp) = 0;               \
	}

LPMATRIX __declspec( dllexport ) MatrixAlloc( int row, int col );

DWORD _DLLFUNC ObjectMatrix(LPPU lpPU_, WORD wMode_, HWND hwnd_);

/////////////////// Link Matrix ///////////////////////////
typedef struct _tagLinkMatrix{
	int			row;
	int 		col;
#ifndef	__WIN32__
	BYTE		Nomeaning[4];
#endif
	PU  		data[1];
}LINKMATRIX, * LPLINKMATRIX;

#define	LINKMATRIX_2_COBJ(_pm_)			\
        ( (LPCOBJ)( (BYTE FAR*)(_pm_)  ) )
//get matrix pointer from a Cobj pointer

#define COBJ_2_LINKMATRIX(_lpCobj_)	( (LPLINKMATRIX)((void far*)&(_lpCobj_->data[0])) )

#define 	DLM_GET_LPPU(lp, _row, _col)		\
			(( ((PU _FAR*)((lp)->data)) + (lp)->col*(_row) + (_col) ))

LPLINKMATRIX LinkMatrixAlloc( int row, int col );
void LinkMatrixFree( LPLINKMATRIX lpm_ );

DWORD ObjectLinkMatrix(LPPU lpPU_, WORD wMode_, HWND hwnd_);

#endif
