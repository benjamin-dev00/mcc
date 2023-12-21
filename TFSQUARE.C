#include "tfalgo.h"
#include "stdlib.h"
//===================================================================
//
//Zhang	97-4-1
//===================================================================

#include "math.h"
#include "stdio.h"

int _DLLFUNC cagaus( LPMATRIX lpma_, LPMATRIX lpmb_, int n, LPMATRIX lparrayx_ )
{
	int 	l, k, i, j, is, q;
	int    *js;
	double 	d, t;

	js = malloc( n*sizeof(int));
	l = 1;
	for( k = 0; k <= n-1; k++ )
	{
		d = 0.0;
		for( i = k; i < n-1; i++ )
			for( j = k; j <= n-1; j++ )
			{
				t = fabs( MGET( lpma_, i, j ) );
				if ( t > d )
				{
					d = t;
					js[k] = j;
					is = i;
				}
			}
		if( d + 1.0 == 1.0 )
			l = 0;
		else
			{
				if( js[k] != k )
					for( i = 0;i <= n-1; i++ )
					{
						q = js[k];
						t = MGET( lpma_, i, k );
						MGET( lpma_, i, k ) = MGET( lpma_, i, q );
						MGET( lpma_, i, q ) = t;
					}
				if( is != k )
				{
					for( j = k; j <= n-1; j++ )
					{
						t = MGET( lpma_, k, j );
						MGET( lpma_, k, j ) = MGET( lpma_, is, j );
						MGET( lpma_, is, j ) = t;
					}
					t = MGET( lpmb_, k, 0 );
					MGET( lpmb_, k, 0 ) = MGET( lpmb_, is, 0 );
					MGET( lpmb_, is, 0 ) = t;
				}
			}
		if( l == 0 )
		{
			free( js );
			MessageBox( NULL, "fail", "", MB_OK  );
			return 0;
		}
		d = MGET( lpma_, k, k );
		for( j = k+1; j <= n-1; j++ )
			MGET( lpma_, k, j ) /= d;
		MGET( lpmb_, k, 0 ) /= d;
		for( i = k+1; i <= n-1; i++ )
		{
			for( j = k+1; j <= n-1; j++ )
				MGET( lpma_, i, j ) = MGET( lpma_, i, j ) -
					MGET( lpma_, i, k ) * MGET( lpma_, k, j );
			MGET( lpmb_, i, 0 ) = MGET( lpmb_, i, 0 ) - MGET( lpma_, i, k ) *
				MGET( lpmb_, k, 0 );
		}
	}
	d = MGET( lpma_, (n-1), (n-1) );
	if( fabs( d ) + 1.0 == 1.0 )
	{
		free( js );
		MessageBox( NULL, "error", "", MB_OK );
		return 0;
	}
	MGET( lparrayx_, n-1, 0 ) = MGET( lpmb_, n-1, 0 ) / d;
	for( i = n-2; i >= 0; i-- )
	{
		t = 0.0;
		for( j = i+1; j <= n-1; j++ )
			t += ( MGET( lpma_, i, j ) * MGET( lparrayx_, j, 0 ) );
		MGET( lparrayx_, i, 0 ) = MGET( lpmb_, i, 0 ) - t;
	}
	js[n-1] = n-1;
	for( k = n-1; k >= 0; k-- )
	if( js[k] != k )
		{
			t = MGET( lparrayx_, k, 0 );
			MGET( lparrayx_, k, 0 ) = MGET( lparrayx_, js[k], 0 );
			MGET( lparrayx_, js[k], 0 ) = t;
		}
	free( js );
	return 1;
}

