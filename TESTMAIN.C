#include "d:\zmcc\tfalgo.h"
#include "d:\zmcc\tfcmdmc.h"
#include "stdio.h"
extern int cagaus( LPMATRIX lpmA_, LPMATRIX lpmB_, int n_, LPMATRIX lpmX_ );

int PASCAL WinMain( HANDLE hIns, HANDLE hHisIns, LPSTR lpstr, int CmdShow )
{
	LPMATRIX _lpmA, _lpmB, _lpmX;
	int i, j;
	static double a[4][4] = 
	{
		{ 0.2368, 0.2471, 0.2568, 1.2671 },
		{ 0.1968, 0.2071, 1.2168, 0.2271 },
		{ 0.1581, 1.1675, 0.1768, 0.1871 },
		{ 1.1161, 0.1254, 0.1397, 0.1490 }
	};
	static double x[4], b[4] = 
	{ 1.8471, 1.7471, 1.6471, 1.5471 }; 

	_lpmA = MatrixAlloc( 4, 4 );
	_lpmB = MatrixAlloc( 4, 1 );
	_lpmX = MatrixAlloc( 4, 1 );

	for( i = 0; i < 4; i++ )
		for( j = 0; j < 4; j++ )
		MGET( _lpmA, i, j ) = a[i][j];
	i = cagaus( _lpmA, _lpmB, 4, _lpmX );
	if( i != 0 )
		for( i = 0; 0 <= 3; i++ )
			x[i] = MGET( _lpmX, i, 0 );

	MATRIXFREE( _lpmA );
	MATRIXFREE( _lpmB );
	MATRIXFREE( _lpmX );
	return TRUE;
}
