#include "tfalgo.h"
//#include "algo2.h"

WORD	_DLLFUNC ebstq( LPMATRIX lpmbc_, LPMATRIX lpmq_, double eps, int l )
{
	int		_i, j, k, it, n, tm;
	double 	d, f, h, g, tp, r, e, s;

	n = lpmbc_->row;
	MGET( lpmbc_, (n-1), 1 ) = 0.0;
	d = 0.0;
	f = 0.0;

	for( j = 0; j <= n-1; j++ )
	{
		it = 0;
		h = eps * ( fabs( MGET( lpmbc_, j, 0 ) ) + fabs( MGET( lpmbc_, j, 1 ) ));
		if( h > d )
			d = h;
		tm = j;
		while( (tm <= n-1 ) && ( fabs( MGET( lpmbc_, tm, 1 ) )  > d ) )
			tm = tm + 1;
		if( tm != j )
		{
			do
			{
				if( it == l )
				{
					MessageBox(NULL, "err", "", MB_OK);
					return -1;
				}

				it = it + 1;
				g = MGET( lpmbc_, j, 0 );
				tp = ( MGET( lpmbc_, (j+1), 0 ) - g ) / ( 2.0 * MGET( lpmbc_, j, 1 ) );
				r = sqrt( tp * tp + 1.0 );

				if ( tp >= 0.0 )
					MGET( lpmbc_, j, 0 ) = MGET( lpmbc_, j, 1 ) / ( tp + r );
				else
					MGET( lpmbc_, j, 0 ) = MGET( lpmbc_, j, 1 ) / ( tp - r );

				h = g - MGET( lpmbc_, j, 0 );
				for( _i = (j+1); _i <= n-1; _i++ )
					MGET( lpmbc_, _i, 0 ) = MGET( lpmbc_, _i, 0 ) - h;
				f = f + h;
				tp = MGET( lpmbc_, tm, 0 );
				e = 1.0;
				s = 0.0;
				for( _i = tm-1; _i >= j; _i-- )
				{
					g = e * MGET( lpmbc_, _i, 1 );
					h = e * tp;
					if( fabs(tp) >= fabs( MGET( lpmbc_, _i, 1 )))
					{
						e = MGET( lpmbc_, _i, 1 ) / tp;
						r = sqrt( e * e + 1.0 );
						MGET( lpmbc_, (_i+1), 1 ) = s * tp * r;
						s = e / r;
						e = 1.0 / r;
					}
					else
					{
						e = tp / MGET( lpmbc_, _i, 1 );
						r = sqrt( e * e + 1.0 );
						MGET( lpmbc_, (_i+1), 1 ) = s * MGET( lpmbc_, _i, 1 ) * r;
						s = 1.0 / r;
						e = e / r;
					}

					tp= e * MGET( lpmbc_, _i, 0 ) - s * g;
					MGET( lpmbc_, (_i+1), 0 ) = h + s * ( e * g + s * MGET( lpmbc_, _i, 0 ) );
					for( k = 0; k <= n-1; k++ )
					{
						h = MGET( lpmq_, k, (_i+1) );
						MGET( lpmq_, k, (_i+1) ) = s * MGET( lpmq_, k, _i ) + e * h;
						MGET( lpmq_, k, _i ) = e * MGET( lpmq_, k, _i ) - s * h;
					}
				}
				MGET( lpmbc_, j, 1 ) = s * tp;
				MGET( lpmbc_, j, 0 ) = e * tp;
			}
			while( fabs( MGET( lpmbc_, j, 1 ) > d ));
		}
		MGET( lpmbc_, j, 0 ) = MGET( lpmbc_, j, 0 ) + f;
	}
	for( _i = 0; _i <= n-1; _i++ )
	{
		k = _i;
		tp = MGET( lpmbc_, _i, 0 );

		if( (_i+1) <= (n-1) )
		{
			j = _i + 1;
			while( ( j <= n-1 ) && (MGET( lpmbc_, j, 0 ) <= tp ) )
			{
				k = j;
				tp = MGET( lpmbc_, j, 0 );
				j = j + 1;
			}
		}

		if( k != _i )
		{
			MGET( lpmbc_, k, 0 ) = MGET( lpmbc_, _i, 0 );
			MGET( lpmbc_, _i, 0 ) = tp;
			for( j = 0; j <= n-1; j++ )
			{
				tp = MGET( lpmq_, j, _i );
				MGET( lpmq_, j, _i ) = MGET( lpmq_, j, k );
				MGET( lpmq_, j, k ) = tp;
			}
		}
	}

	return TRUE;
}

void _DLLFUNC eastrq( LPMATRIX lpmGtG_, LPMATRIX lpmq_, LPMATRIX lpmbc_ )
{
	int _i, j, k, n, i;
	double h, f, g, h2, _b[5], _a[5][5];
	n = lpmGtG_->row;

	for( _i = 0; _i <= n-1; _i++ )
		for( j = 0; j <= n-1; j++ )
		{
			MGET( lpmq_, _i, j) = MGET( lpmGtG_, _i, j );
			_a[_i][j] = MGET( lpmq_, _i, j );
        }

	for( _i = n-1; _i >= 1; _i-- )
	{
		h = 0.0;

		for( i = 0; i < n; i++ )
			_b[i] = MGET( lpmbc_, i, 1 );

		if( _i > 1 )
			for( k = 0; k <= _i-1; k++ )
				h = h + MGET( lpmq_, _i, k ) * MGET( lpmq_, _i, k );

		if( ( h + 1.0 ) == 1.0 )
		{
			MGET( lpmbc_, (_i-1), 1 ) = 0.0;

			if( _i == 1)
				MGET( lpmbc_, _i, 1 ) = MGET( lpmq_, _i, (_i-1) );

			MGET( lpmbc_, _i, 0 ) = 0.0;
		}
		else
		{
			MGET( lpmbc_, _i, 1 ) = sqrt( h );

			if( MGET( lpmq_, _i, (_i-1) ) > 0.0 )
				MGET( lpmbc_, _i, 1 ) = -MGET( lpmbc_, _i, 1 );

			for( i = 0; i < n; i++ )
				_b[i] = MGET( lpmbc_, i, 1 );

			h = h - MGET( lpmq_, _i, (_i-1) ) * MGET( lpmbc_, _i, 1);
			MGET( lpmq_, _i, (_i-1) ) -= MGET( lpmbc_, _i, 1 );
			f = 0.0;
			for( j = 0; j <= _i-1; j++ )
			{
				MGET( lpmq_, j, _i ) = MGET( lpmq_, _i, j ) / h;
				g = 0.0;
				for( k = 0; k <= j; k++)
					g = g + MGET( lpmq_, j, k ) * MGET( lpmq_, _i, k );

				if( j+1 <= _i-1 )
					for(k = j+1; k <= _i-1; k++ )
						g = g + MGET( lpmq_, k, j ) * MGET( lpmq_, _i, k );

				MGET( lpmbc_, j, 1 ) = g / h;

				for( i = 0; i < n; i++ )
					_b[i] = MGET( lpmbc_, i, 1 );

				f = f + g * MGET( lpmq_, j, _i );
			}
			for( i = 0; i <= n-1; i++ )
				for( j = 0; j <= n-1; j++ )
				_a[i][j] = MGET( lpmq_, i, j );

			h2 = f / (h+h);
			for( j = 0; j <= _i-1; j++ )
			{
				f = MGET( lpmq_, _i, j );
				g = MGET( lpmbc_, j, 1 ) - h2 * f;
				MGET( lpmbc_, j, 1 ) = g;
				for( i = 0; i < n; i++ )
					_b[i] = MGET( lpmbc_, i, 1 );

				for( k = 0; k <= j; k++ )
					MGET( lpmq_, j, k ) -= f * MGET( lpmbc_, k, 1 ) + g * MGET( lpmq_, _i, k );
			}
			for( i = 0; i <= n-1; i++ )
				for( j = 0; j <= n-1; j++ )
					_a[i][j] = MGET( lpmq_, i, j );

			MGET( lpmbc_, _i, 0 ) = h;
		}
	}
	for( i = 0; i < n; i++ )
		_b[i] = MGET( lpmbc_, i, 1 );

	MGET( lpmbc_, 0, 0 ) = 0.0;
	for( _i = 0; _i <= n-2; _i++ )
		MGET( lpmbc_, _i, 1 ) = MGET( lpmbc_, (_i+1), 1 );
	MGET( lpmbc_, (n-1), 1 ) = 0.0;
	for( _i = 0; _i <= n-1; _i++ )
	{
		if((MGET( lpmbc_, _i, 0 ) != 0.0) && ( _i-1 >= 0 ) )
			for( j = 0; j<= _i-1; j++)
			{
				g = 0.0;
				for( k = 0; k <= _i-1; k++ )
					g = g + MGET( lpmq_, _i, k ) * MGET( lpmq_, k, j );

				for( k = 0; k <= _i-1; k++ )
					MGET( lpmq_, k, j ) -= g * MGET( lpmq_, k, _i );
			}
			MGET( lpmbc_, _i, 0 ) = MGET( lpmq_, _i, _i );
			MGET( lpmq_, _i, _i ) = 1.0;
			if( _i-1 >= 0 )
				for( j = 0; j <= _i-1; j++ )
				{
					MGET( lpmq_, _i, j ) = 0.0;
					MGET( lpmq_, j, _i ) = 0.0;
				}
	}

	return;
}
