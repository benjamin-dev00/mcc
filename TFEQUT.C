#include "tfalgo.h"
#include "tfcmdmc.h"
#include "stdlib.h"
#include "math.h"
#include "stdio.h"

int cagaus( LPMATRIX lpmA_, LPMATRIX lpmB_, LPMATRIX lpmX_ )
{
	int * js, l, k, i, j, is, n_;
	double d, t;

	n_ = lpmA_->row;
	js = malloc( n_ * sizeof(int) );
	l = 1;
	for(k=0; k <= n_-2; k++ )
	{
		d = 0.0;
		for( i = k; i <= n_-1; i++ )
			for( j = k; j <= n_-1; j++ )
			{
				t = fabs( MGET( lpmA_, i, j ) );
				if( t > d )
				{
					d = t;
					js[k] = j;
					is = i;
				}

			}
		if( ( d + 1.0 ) == 1.0 )
			l = 0;
		else
		{
			if( js[k] != k )
				for( i = 0; i <= n_-1; i++ )
				{
					t = MGET( lpmA_, i, k );
					MGET( lpmA_, i, k ) = MGET( lpmA_, i, (js[k]) );
					MGET( lpmA_, i, js[k] ) = t;
				}
			if( is != k )
			{
				for( j = k; j <= n_-1; j++ )
				{
					t = MGET( lpmA_, k, j );
					MGET( lpmA_, k, j ) = MGET( lpmA_, is, j );
					MGET( lpmA_, is, j ) = t;
				}
				t = MGET( lpmB_, k, 0 );
				MGET( lpmB_, k, 0 ) = MGET( lpmB_, is, 0 );
				MGET( lpmB_, is, 0 ) = t;
			}
		}
		if( l == 0 )
		{
			free( js );
//			MessageBox( NULL, "ERROR", "TFEQUT.C", MB_OK );
			MessageBeep( -1 );
			LogError( "tfequt.c", "59" );
			return( 0 );
		}
		d = MGET( lpmA_, k, k );
		for( j = k + 1; j <= n_ - 1; j++ )
		{
			MGET( lpmA_, k, j )= MGET( lpmA_, k, j )/d;
		}
		MGET( lpmB_, k, 0 ) = MGET( lpmB_, k, 0 )/d;
		for( i = k + 1; i <= n_ - 1; i++ )
		{
			for( j = k + 1; j <= n_ - 1; j++ )
			{
				MGET( lpmA_, i, j ) = MGET( lpmA_, i, j ) - 
					MGET( lpmA_, i, k ) * 
						MGET( lpmA_, k, j );
			}
			MGET( lpmB_, i, 0 ) = MGET( lpmB_, i, 0 ) - MGET( lpmA_, i , k ) * 
				MGET( lpmB_, k, 0 );
		}
	}
	d = MGET( lpmA_, ( n_ - 1), ( n_ - 1 ) );
	if( ( fabs( d ) + 1.0 ) == 1.0  )
	{
		free( js );
		//MessageBox( NULL, "ERROR", "TFEQUT.C", MB_OK );
		MessageBeep( -1 );
        LogError( "tfequt.c", "86" );
		return( 0 );
	}
	MGET( lpmX_, (n_-1), 0 ) = MGET( lpmB_, ( n_ - 1 ), 0 ) / d;
	for( i = n_ - 2; i >= 0; i-- )
	{
		t = 0.0;
		for( j = i + 1; j <= n_ - 1; j++ )
			t = t + MGET( lpmA_, i, j ) * MGET( lpmX_, j, 0 );
		MGET( lpmX_, i, 0 ) = MGET( lpmB_, i, 0 ) - t;
	}
	js[ n_ - 1 ] = n_ - 1;
	for( k = n_ - 1; k >= 0; k-- )
		if( js[k] != k )
		{
			t = MGET( lpmX_, k, 0 );
			MGET( lpmX_, k, 0 ) = MGET( lpmX_, ( js[k] ), 0 );
			MGET( lpmX_, ( js[k] ), 0 ) = t;
		}
	free( js );
	return(1);
}
