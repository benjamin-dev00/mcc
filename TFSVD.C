#include "tfalgo.h"
#include "tfsvd.h"
void svd( LPMATRIX lpmA_, LPMATRIX lpmu_, LPMATRIX lpmv_, double eps_svd_ )
{
	int 		_row, _col, _size;
//	LPMATRIX	_lpmtemp;
//	double 	_a[3][4], _u[3][3], _v[4][4];

	_row = lpmA_->row;
	_col = lpmA_->col;
//	_lpmtemp = MatrixAlloc( _row, _col );
	_size = ( int )max( _row, _col ) + 1;
//	here _i can be detected if error occurse;
//	_i = dluav( lpmA_, _row, _col, lpmu_, lpmv_, eps_svd_, _size );
	dluav( lpmA_, _row, _col, lpmu_, lpmv_, eps_svd_, _size );
/*	for( _i = 0; _i < _row; _i++ )
		for( _j = 0; _j < _col; _j ++ )
			_a[_i][_j] = MGET( lpmA_, _i, _j );
	for( _i = 0; _i < _row; _i++ )
		for( _j = 0; _j < _row; _j ++ )
			_u[_i][_j] = MGET( lpmu_, _i, _j );
	for( _i = 0; _i < _col; _i++ )
		for( _j = 0; _j < _col; _j ++ )
			_v[_i][_j] = MGET( lpmv_, _i, _j );

	mmultiple( _lpmtemp, lpmu_, lpmA_ );
	mmultiple( lpmA_, _lpmtemp, lpmv_ );
	for( _i = 0; _i < _row; _i++ )
		for( _j = 0; _j < _col; _j ++ )
			_a[_i][_j] = MGET( lpmA_, _i, _j );

	MATRIXFREE( _lpmtemp );                  */
	return ;
}
int dluav( LPMATRIX lpmA_, int svdm, int n, LPMATRIX lpmu_, LPMATRIX lpmv_, double eps,
				int ka )
{
	int 		 i, j, k, l, it, ll, kk, mm, nn, ml, ks, foot1, foot2;
	double	 d, dd, sm, sml, eml, sk, ek, b, c, shh, f, g, cs, sn;
	LPMATRIX  _lpms, _lpme, _lpmw;
//	double	 _a[4][3], _u[4][4], _v[3][3], _ee[5], _sss[5], _ww[5];
	_lpms = MatrixAlloc( ka, 1 );
	_lpme = MatrixAlloc( ka, 1 );
	_lpmw = MatrixAlloc( ka, 1 );

	for( i = 0; i < ka; i++ )
	{
		MGET( _lpms, i, 0 ) = 0.0;
		MGET( _lpme, i, 0 ) = 0.0;
		MGET( _lpmw, i, 0 ) = 0.0;
	}

	for( i = 0; i < svdm; i++ )
		for( j = 0; j < svdm; j++ )
			MGET( lpmu_, i, j ) = 0.0;

	for( i = 0; i < n; i++ )
		for( j = 0; j < n; j++ )
			MGET( lpmv_, i, j ) = 0.0;

	it = 60;
	k = n-1;
	if( svdm- 2 < n-1 )
		k = svdm - 2;
	l = svdm-1;
	if( n-3 < svdm-1 )
		l = n-3;
	if( l < 0 )
		l = -1;
	ll = k;
	if( l > k )
		ll = l;
	if( ll >= 0 )
	{
		for( kk = 0; kk <= ll; kk++ )
		{
			if( kk <= k )
			{
				d = 0.0;
				for( i = kk; i < svdm; i++ )
					d += MGET( lpmA_, i, kk ) * MGET( lpmA_, i, kk );
				MGET( _lpms, kk, 0 ) = sqrt(d);
				if ( fabs(MGET( _lpms, kk, 0 )) >eps )
				{
					if( fabs(MGET( lpmA_, kk, kk )) > eps )
						if(MGET( lpmA_, kk, kk ) < 0.0 )
							MGET( _lpms, kk, 0 ) = -MGET( _lpms, kk, 0 );

					for( i = kk; i < svdm; i++ )
						MGET( lpmA_, i, kk ) = MGET( lpmA_, i, kk ) / MGET( _lpms, kk, 0 );

					MGET( lpmA_, kk, kk ) += 1.0;
				}
				MGET( _lpms, kk, 0 ) = -MGET( _lpms, kk, 0 );
			}
			if( n-1 >= kk + 1 )
			{
				for( j = kk + 1; j < n; j++ )
				{
					if( (kk <= k ) && ( fabs(MGET( _lpms, kk, 0 )) >eps ) )
					{
						d = 0.0;
						for( i = kk; i < svdm; i++ )
							d = d + MGET( lpmA_, i, kk ) * MGET( lpmA_, i, j );

						d = -d / MGET( lpmA_, kk, kk );

						for( i = kk; i < svdm; i++ )
							MGET( lpmA_, i, j ) += d * MGET( lpmA_, i, kk );
					}
					MGET( _lpme, j, 0 ) = MGET( lpmA_, kk, j );
				}
			}
			if( kk <= k)
			{
				for( i = kk; i < svdm; i++ )
					MGET( lpmu_, i, kk ) =  MGET( lpmA_, i, kk );
			}
			if( kk <= l )
			{
				d = 0.0;
				for( i = kk + 1; i < n; i++ )
					d = d + MGET( _lpme, i, 0 ) * MGET( _lpme, i, 0 );
				MGET( _lpme, kk, 0 ) = sqrt(d);
				if( fabs(MGET( _lpme, kk, 0 )) > eps )
				{
					if( fabs(MGET( _lpme, kk+1, 0 )) > eps )
					{
						if( (MGET( _lpme, kk+1, 0 )) < 0.0 )
							MGET( _lpme, kk, 0 ) = - MGET( _lpme, kk, 0 );
					}
					for( i = kk+1; i < n; i++ )
						MGET( _lpme, i, 0 ) = MGET( _lpme, i, 0 ) / MGET( _lpme, kk, 0 );
					MGET( _lpme, kk+1, 0 ) = 1.0 + MGET( _lpme, kk+1, 0 );
				}

				MGET( _lpme, kk, 0 ) = -MGET( _lpme, kk, 0 );
				if( (kk+1 <= svdm-1 ) && (fabs(MGET( _lpme, kk, 0 )) > eps ) )
				{
					for( i = kk + 1; i < svdm; i++ )
						MGET( _lpmw, i, 0 ) = 0.0;
					for( j = kk+1; j < n; j++ )
						for( i = kk+1; i < svdm; i++ )
							MGET( _lpmw, i, 0 ) = MGET( _lpmw, i, 0 ) + MGET( _lpme, j, 0 ) * MGET( lpmA_, i, j );
					for( j = kk+1; j < n; j++ )
						for( i = kk+1; i < svdm; i++ )
							MGET( lpmA_, i, j ) -= MGET( _lpmw, i, 0 ) * MGET( _lpme, j, 0 ) / MGET( _lpme, kk+1, 0 );
				}
				for( i = kk+1; i < n; i++ )
					MGET( lpmv_, i, kk ) = MGET( _lpme, i, 0 );
			}
		}
	}
	mm = n-1;
	if( svdm < n-1 )
		mm = svdm;
	if( k < n-1 )
		MGET( _lpms, k+1, 0 ) = MGET( lpmA_, k+1, k+1 );
	if( svdm-1 < mm )
		MGET( _lpms, mm, 0 ) = 0.0;
	if( l + 1 < mm )
		MGET( _lpme, l+1, 0 ) = MGET( lpmA_, l+1, mm );
	MGET( _lpme, mm, 0 ) = 0.0;
	nn = svdm-1;
	if( svdm-1 > n-1 )
		nn = n-1;
	if( nn >= k + 1 )
	{
		for( j = k+1; j <= nn; j++ )
		{
			for( i = 0; i < svdm; i++ )
				MGET( lpmu_, i, j ) = 0.0;
			MGET( lpmu_, j, j ) = 1.0;
		}
	}

	if(k >= 0 )
	{
		for( kk = k; kk >= 0; kk-- )
		{
			if( fabs(MGET( _lpms, kk, 0 )) > eps )
			{
				if( nn >= kk + 1 )
				{
					for( j = kk+1; j <= nn; j++ )
					{
						d = 0.0;
						for( i = kk; i < svdm; i++ )
							d = d + MGET( lpmu_, i, kk ) * MGET( lpmu_, i, j )
									/ MGET( lpmu_, kk, kk );
						d = -d;
						for( i = kk; i < svdm; i++ )
							MGET( lpmu_, i, j ) += d * MGET( lpmu_, i, kk );
					}
				}
				for( i = kk; i < svdm; i++ )
					MGET( lpmu_, i, kk ) = - MGET( lpmu_, i, kk );
//note : ?????????????????????????
				MGET( lpmu_, kk, kk ) += 1.0;
				if( kk-1 >= 0 )
					for( i = 0; i <= kk-1; i++ )
						MGET( lpmu_, i, kk ) = 0.0;
			}
			else
			{
				for( i = 0; i < svdm; i++ )
					MGET( lpmu_, i, kk ) = 0.0;
				MGET( lpmu_, kk, kk ) = 1.0;
			}
		}
	}
	for( kk = n-1; kk >= 0; kk-- )
	{
		if( ( kk <= l ) && ( fabs(MGET( _lpme, kk, 0 )) > eps ) )
		{
			 for( j = kk + 1; j < n; j++ )
			 {
				d= 0.0;
				for( i = kk+1; i < n; i++ )
					d = d + MGET( lpmv_, i, kk ) * MGET( lpmv_, i, j )
						 / MGET( lpmv_, kk+1, kk );
				d = -d;
				for( i = kk + 1; i < n; i++ )
					MGET( lpmv_, i, j ) += d * MGET( lpmv_, i, kk );
			 }
		}
		for ( i = 0; i < n; i++ )
			MGET( lpmv_, i, kk ) = 0.0;
		MGET( lpmv_, kk, kk ) = 1.0;
	}
	for( i = 0; i < svdm; i++ )
		for( j = 0; j < n; j++ )
			MGET( lpmA_, i, j ) = 0.0;
	ml = mm;
	it = 60;
	for( ; ;)
	{
		foot2 = 0;
		if( mm == -1 )
		{
			if( svdm-1 >= n-1 )
				i = n-1;
			else
				i = svdm-1;
			for( j = 0; j <= i-1; j++ )
			{
				MGET( lpmA_, j, j ) = MGET( _lpms, j, 0 );
//note: ??????
				MGET( lpmA_, j, j+1 ) = MGET( _lpme, j, 0 );
			}
			MGET( lpmA_, i, i ) = MGET( _lpms, i, 0 );
			if( svdm-1 < n-1 )
				MGET( lpmA_, i, i+1 ) = MGET( _lpme, i, 0 );
			for( i = 0; i < n-1; i++ )
				for( j = i+1; j < n; j++ )
				{
					d = MGET( lpmv_, i, j );
					MGET( lpmv_, i, j ) = MGET( lpmv_, j, i );
					MGET( lpmv_, j, i ) = d;
				}

//			ppp( a, e, s, v, svdm, n );
			MATRIXFREE(_lpms);
			MATRIXFREE(_lpme);
			MATRIXFREE(_lpmw);
			return(0);
		}
		if( it == 0 )
		{
			if( svdm-1 >= n-1 )
				i = n-1;
			else
				i = svdm-1;
			for( j = 0; j <= i-1; j++ )
			{
	//note: ?????????????
				MGET( lpmA_, j, j ) = MGET( _lpms, j, 0 );
				MGET( lpmA_, j, j+1 ) = MGET( _lpme, j, 0 );
			}
			MGET( lpmA_, i, i ) = MGET( _lpms, i, 0 );
			if( svdm-1 < n-1 )
				MGET( lpmA_, i, i+1 ) = MGET( _lpme, i, 0 );
			for( i = 0; i < n-1; i++ )
				for( j = i+1; j < n; j++ )
				{
					d = MGET( lpmv_, i, j );
					MGET( lpmv_, i, j ) = MGET( lpmv_, j, i );
               MGET( lpmv_, j, i ) = d;
				}
//			ppp( a, e, s, v, svdm, n );
			MATRIXFREE(_lpms);
			MATRIXFREE(_lpme);
			MATRIXFREE(_lpmw);
			return(-1);
		}
		foot1 = 0;
		kk = mm - 1;

		while( (kk != -1 ) && ( foot1 == 0 ) )
		{
			d = fabs(MGET( _lpms, kk, 0 )) + fabs(MGET( _lpms, kk+1, 0 ));
			dd = fabs(MGET( _lpme, kk, 0 ));
			if( dd > eps * d )
				kk = kk - 1;
			else
			{
				foot1 = 1;
				MGET( _lpme, kk, 0 ) = 0.0;
			}
		}
		if( kk == mm - 1 )
		{
			kk = kk + 1;
			if(MGET( _lpms, kk, 0 ) < 0.0 )
			{
				MGET( _lpms, kk, 0 ) = - MGET( _lpms, kk, 0 );
				for( i = 0; i < n; i++ )
					MGET( lpmv_, i, kk ) = -MGET( lpmv_, i, kk );
			}
			while( (kk != ml ) && ( MGET( _lpms, kk, 0 ) < MGET( _lpms, kk+1, 0 ) ) )
			{
				d = MGET( _lpms, kk, 0 );
				MGET( _lpms, kk, 0 ) = MGET( _lpms, kk+1, 0 );
				MGET( _lpms, kk+1, 0 ) = d;
				if( kk < n-1 )
					for( i = 0; i < n; i++ )
					{
						d = MGET( lpmv_, i, kk );
						MGET( lpmv_, i, kk ) = MGET( lpmv_, i, kk+1 );
						MGET( lpmv_, i, kk+1 ) = d;
					}
				if( kk < svdm-1 )
					for( i = 0; i < svdm; i++ )
					{
						d = MGET( lpmu_, i, kk );
						MGET( lpmu_, i, kk ) = MGET( lpmu_, i, kk+1 );
						MGET( lpmu_, i, kk+1 ) = d;
					}
				kk = kk + 1;
			}
			it = 60;
			mm = mm - 1;
			foot2 = 1;
		}
		if( foot2 == 0 )
		{
			foot1 = 0;
			ks = mm;
			while( (ks > kk ) && ( foot1 == 0  ) )
			{
				d = 0.0;
				if( ks != mm )
					d = d + fabs( MGET( _lpme, ks, 0 ));
				if( ks != kk + 1 )
					d = d + fabs( MGET( _lpme, ks-1, 0 ));
				dd = fabs( MGET( _lpms, ks, 0 ) );
				if( dd > eps * d )
					ks = ks - 1;
				else
				{
					MGET( _lpms, ks, 0 ) = 0.0;
					foot1 = 1;
				}
			}
			if( ks == kk )
			{
				kk = kk + 1;
				d = fabs( MGET( _lpms, mm, 0 ));
				if( fabs(MGET( _lpms, mm-1, 0 )) > d )
					d = fabs(MGET( _lpms, mm-1, 0 ));
				if( fabs(MGET( _lpme, mm-1, 0 )) > d )
					d = fabs(MGET( _lpme, mm-1, 0 ));
				if( fabs(MGET( _lpms, kk, 0 )) > d )
					d = fabs(MGET( _lpms, kk, 0 ));
				if( fabs( MGET( _lpme, kk, 0 ) ) > d )
					d = fabs(MGET( _lpme, kk, 0 ));
				sm = MGET( _lpms, mm, 0 ) / d;
				sml = MGET( _lpms, mm-1, 0 ) / d;
				eml = MGET( _lpme, mm-1, 0 ) / d;
				sk = MGET( _lpms, kk, 0 ) / d;
				ek = MGET( _lpme, kk, 0 ) / d;
				b = ( ( sml + sm ) * ( sml - sm ) + eml * eml ) / 2.0;
//note: ?????
				c = sml * eml;
				c = c * c;
				shh = 0.0;
				if( ( fabs(b) > eps ) || ( fabs(c) > eps ) )
				{
					shh = sqrt( b* b + c );
					if( b < 0.0 )
						shh = - shh;
					shh = c / ( b + shh );
				}
				f = ( sk + sm ) * ( sk - sm ) - shh;
				g = sk * ek;
				for( i = kk; i <= mm - 1; i++ )
				{
					sss( &f, &g, &cs, &sn );
					if( i != kk )
						MGET( _lpme, i-1, 0 ) = f;
					f = cs * MGET( _lpms, i, 0 )+ sn * MGET( _lpme, i, 0 );
					MGET( _lpme, i, 0 ) = cs * MGET( _lpme, i, 0 ) - sn * MGET( _lpms, i, 0 );
					g = sn * MGET( _lpms, i+1, 0 );
					MGET( _lpms, i+1, 0 ) = cs * MGET( _lpms, i+1, 0 );
					if((cs != 1.0 ) || (fabs(sn) > eps ) )
						for(j = 0; j < n; j++ )
						{
							d = cs * MGET( lpmv_, j, i ) + sn * MGET( lpmv_, j, i+1 );
							MGET( lpmv_, j, i+1 ) = - sn * MGET( lpmv_, j, i ) +
																cs * MGET( lpmv_, j, i+1 );
							MGET( lpmv_, j, i ) = d;
						}
					sss( &f, &g, &cs, &sn );
					MGET( _lpms, i, 0 ) = f;
					f = cs * MGET( _lpme, i, 0 ) + sn * MGET( _lpms, i+1, 0 );
					MGET( _lpms, i+1, 0 ) = -sn * MGET( _lpme, i, 0 ) + cs * MGET( _lpms, i+1, 0 );
					g = sn * MGET( _lpme, i+1, 0 );
					MGET( _lpme, i+1, 0 ) = cs * MGET( _lpme, i+1, 0 );
					if( i < svdm-1 )
						if( ( cs != 1.0 ) || ( fabs(sn) > eps ) )
							for( j = 0; j < svdm; j++ )
							{
								d = cs * MGET( lpmu_, j, i ) + sn * MGET( lpmu_, j, i+1 );
								MGET( lpmu_, j, i+1 ) = -sn * MGET( lpmu_, j, i ) + cs * MGET( lpmu_, j, i+1 );
								MGET( lpmu_, j, i ) = d;
							}
				}
				MGET( _lpme, mm-1, 0 ) = f;
				it = it - 1;
				foot2 = 1;
			}
		}
		if( foot2 == 0 )
		{
			if( ks == mm )
			{
				kk = kk + 1;
				f = MGET( _lpme, mm-1, 0 );
				MGET( _lpme, mm-1, 0 ) = 0.0;
				for( i = mm-1; i >= kk; i-- )
				{
//note: ????
					g = MGET( _lpms, i, 0 );
					sss( &g, &f, &cs, &sn );
					MGET( _lpms, i, 0 ) = g;
					if( i != kk )
					{
						f = -sn * MGET( _lpme, i-1, 0 );
	//note: ??????????????
						MGET( _lpme, i-1, 0 ) = cs * MGET( _lpme, i-1, 0 );
					}
					if( ( cs != 1.0 ) || ( fabs(sn) > eps ) )
						for( j = 0; j < n; j++ )
						{
							d = cs * MGET( lpmv_, j, i ) + sn * MGET( lpmv_, j, mm );
							MGET( lpmv_, j, mm ) = -sn * MGET( lpmv_, j, i ) + cs * MGET( lpmv_, j, mm );
							MGET( lpmv_, j, i ) = d;
						}
				}
				foot2 = 1;
			}
		}
		if( foot2 == 0  )
		{
			kk = ks + 1;
			f = MGET( _lpme, kk-1, 0 );
			MGET( _lpme, kk-1, 0 ) = 0.0;
			for( i = kk; i <= mm; i++ )
			{
				g = MGET( _lpms, i, 0 );
				sss( &g, &f, &cs, &sn );
				MGET( _lpms, i, 0 ) = f;
	//note :????
				f = -sn * MGET( _lpme, i, 0 );
				MGET( _lpme, i, 0 ) = cs * MGET( _lpme, i, 0 );
				if( ( cs != 1.0) || (fabs(sn) > eps ) )
					for( j = 0; j < svdm; j++ )
					{
						d = cs * MGET( lpmu_, j, i ) + sn * MGET( lpmu_, j, kk-1 );
						MGET( lpmu_, j, kk-1 ) = -sn * MGET( lpmu_, j, i ) + cs * MGET( lpmu_, j, kk-1 );
						MGET( lpmu_, j, i ) = d;
					}
			}
		}
	}
}

static void sss( double *f, double *g, double *cs, double *sn )
{
	double r, d;

	if( (fabs(*f) + fabs(*g)) == 0.0 )
	{
		(*cs) = 1.0;
		(*sn) = 0.0;
		d = 0.0;
	}
	else
	{
		d = sqrt( (*f) * (*f)+ (*g) * (*g) );
		if( fabs(*f) > fabs(*g))
		{
			if( (*f) < 0.0 )
				d = -d;
		}
		if(fabs(*g) > fabs(*f))
		{
			if( *g < 0.0 )
				d = -d;
		}
		(*cs) = *f / d;
		(*sn) = *g / d;
	}
	r = 1.0;
	if( fabs(*f) > fabs(*g))
		r = (*sn);
	else
		if( (*cs) != 0.0 )
			r = 1.0 /(*cs);
	(*f) = d;
	(*g) = r;
	return;
}
//http://210.34.0.13/~bbs/framegrp.html
