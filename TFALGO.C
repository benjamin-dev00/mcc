//===================================================================
//
//Zhang
//===================================================================

#include "tfalgo.h"
#include "tfmcv.h"

///////////////  PrivCVAlloc  //////////////////////////////////////////
//...................................................................
//...................................................................

//LPMATRIX	lpypred, lpd;
//double 	y0, SR[100], arrayh[20], delu;
//WORD	    N;
//int		count = 0;
//LPPU		_lppu;
//REAL		a, b;
//static	WORD	_N;
void DimMatrixFree(LPDIMMATRIX _lpm)
{
	CobjFree( MAKE_COBJ(_lpm));
	return ;
}
LPCOBJPRIVCV _DLLFUNC PrivCVAlloc( int row, int col )
{
	LPCOBJPRIVCV	_lpPriv;
	LPCOBJ			_lpCobj;
	int				_i, _j;

	if( row <= 0 || col <= 0 )
	{
		MessageBox( NULL, "Dimension of PRIVCV should be greater than 0", "ERRO", MB_OK );
		return NULL;
	}
	_lpCobj = CobjAlloc( IDCOBJ_MATRIX, (long)row * (long)col * (long)sizeof(double) * 1L + sizeof(CobjPrivCV) + sizeof(COBJ) );
	_lpPriv = COBJ_2_PRIVCV( _lpCobj );
	_lpPriv->row = row;
	_lpPriv->col = col;
	for(_i = 0; _i < _lpPriv->row; _i++ )
		for(_j = 0; _j < _lpPriv->col; _j++ )
			PRIVCVGET( _lpPriv, _i, _j) = NULL;

	return _lpPriv;
}
LPCOBJAPART _DLLFUNC PrivApartAlloc( int row, int col )
{
	LPCOBJAPART		_lpPriv;
	LPCOBJ			_lpCobj;
	int				_i, _j;

	if( row <= 0 || col <= 0 )
	{
		MessageBox( NULL, "Dimension of PRIVMV should be greater than 0", "ERRO", MB_OK );
		return NULL;
	}
	_lpCobj = CobjAlloc( IDCOBJ_MATRIX, (long)row * (long)col * (long)sizeof(double) * 1L + sizeof(CobjApart) + sizeof(COBJ) );
	_lpPriv = COBJ_2_APART( _lpCobj );
	_lpPriv->row = row;
	_lpPriv->col = col;
	for(_i = 0; _i < _lpPriv->row; _i++ )
		for(_j = 0; _j < _lpPriv->col; _j++ )
			APARTGET( _lpPriv, _i, _j) = NULL;

	return _lpPriv;
}
LPCOBJPRIVMV _DLLFUNC PrivMVAlloc( int row, int col )
{
	LPCOBJPRIVMV	_lpPriv;
	LPCOBJ			_lpCobj;
	int				_i, _j;

	if( row <= 0 || col <= 0 )
	{
		MessageBox( NULL, "Dimension of PRIVMV should be greater than 0", "ERRO", MB_OK );
		return NULL;
	}
	_lpCobj = CobjAlloc( IDCOBJ_MATRIX, (long)row * (long)col * (long)sizeof(double) * 1L + sizeof(CobjPrivMV) + sizeof(COBJ) );
	_lpPriv = COBJ_2_PRIVMV( _lpCobj );
	_lpPriv->row = row;
	_lpPriv->col = col;
	for(_i = 0; _i < _lpPriv->row; _i++ )
		for(_j = 0; _j < _lpPriv->col; _j++ )
			PRIVMVGET( _lpPriv, _i, _j) = NULL;

	return _lpPriv;
}
LPCOBJPRIVDV _DLLFUNC PrivDVAlloc( int row, int col )
{
	LPCOBJPRIVDV	_lpPriv;
	LPCOBJ			_lpCobj;
	int				_i, _j;

	if( row <= 0 || col <= 0 )
	{
		MessageBox( NULL, "Dimension of PRIVDV should be greater than 0", "ERRO", MB_OK );
		return NULL;
	}
	_lpCobj = CobjAlloc( IDCOBJ_MATRIX, (long)row * (long)col * (long)sizeof(double) * 1L + sizeof(CobjPrivDV) + sizeof(COBJ) );
	_lpPriv = COBJ_2_PRIVDV( _lpCobj );
	_lpPriv->row = row;
	_lpPriv->col = col;
	for(_i = 0; _i < _lpPriv->row; _i++ )
		for(_j = 0; _j < _lpPriv->col; _j++ )
			PRIVDVGET( _lpPriv, _i, _j) = NULL;

	return _lpPriv;
}

LPMATRIX __declspec( dllexport ) LMatrixAlloc( int row, int col )
{
	LPMATRIX		_lpmS;
	LPCOBJ			_lpCobjS;
	long			_i, _j, _longip;

	if( row <= 0 || col <= 0 )
	{
		MessageBox( NULL, "Dimension of LMatrix should be greater than 0", "ERRO", MB_OK );
		return NULL;
	}
	_lpCobjS = CobjAlloc( IDCOBJ_MATRIX, (long)(row) * (long)(col) * (long)sizeof(double) *
						1L + sizeof(Matrix) + sizeof(COBJ) );
	_lpmS = MAKE_MATRIX( _lpCobjS );
	_lpmS->row = row;
	_lpmS->col = col;
	for(_i = 0; _i < _lpmS->row; _i++ )
		for(_j = 0; _j < _lpmS->col; _j++ )
		{
			_longip = (long)(_lpmS)->col*(_i)*1L +	(long)(_j)*1L;

			(*( (( double *)((_lpmS)->data)) + _longip ))= 0.0;
		}
	return _lpmS;
}
//void MatrixFree(LPMATRIX lpm)
void _DLLFUNC PrivApartFree( LPCOBJAPART lpm )
{
	int		_row, _col;
	LPCOBJ	_LPCobj;

	for( _row = 0; _row < lpm->row; _row++ )
		for( _col = 0; _col < lpm->col; _col++ )
		{
			APARTGET( lpm, _row, _col );
			if( APARTGET( lpm, _row, _col ) )
			{
				_LPCobj = MAKE_COBJ( APARTGET( lpm, _row, _col ) )	;
				CobjFree( _LPCobj );
			}
		}
	
	return ;
}
void _DLLFUNC PrivCVCobjFree( LPCOBJPRIVCV lpm )
{
	int		_row, _col;
	LPCOBJ	_LPCobj;
	for( _row = 0; _row < lpm->row; _row++ )
		for( _col = 0; _col < lpm->col; _col++ )
		{
			if( PRIVCVGET( lpm, _row, _col ) )
			{	
				_LPCobj = MAKE_COBJ( PRIVCVGET( lpm, _row, _col ) )	;
				CobjFree( _LPCobj );
			}
		}
	
	return ;
}
void _DLLFUNC PrivMVCobjFree( LPCOBJPRIVMV lpm )
{
	int		_row, _col;
	LPCOBJ	_LPCobj;
	for( _row = 0; _row < lpm->row; _row++ )
		for( _col = 0; _col < lpm->col; _col++ )
		{
			if( PRIVMVGET( lpm, _row, _col ) )
			{
				_LPCobj = MAKE_COBJ( PRIVMVGET( lpm, _row, _col ) )	;
				CobjFree( _LPCobj );
			}
		}
	

	return ;
}
void _DLLFUNC PrivDVCobjFree( LPCOBJPRIVDV lpm )
{
	int		_row, _col;
	LPCOBJ	_LPCobj;
	for( _row = 0; _row < lpm->row; _row++ )
		for( _col = 0; _col < lpm->col; _col++ )
		{
			if( PRIVDVGET( lpm, _row, _col ) )
			{
				_LPCobj = MAKE_COBJ( PRIVDVGET( lpm, _row, _col ) )	;
				CobjFree( _LPCobj );
			}
		}
	return ;
}
LPDIMMATRIX DimAlloc(int _row, int _col, int _axe)
{
	LPDIMMATRIX	_lpDimMatrix;
	LPCOBJ		_lpCobj;
	int 		_i, _j, _k;
	if( _row <= 0 || _col <= 0 || _axe <= 0 )
	{
		MessageBox( NULL, "Dimension of DimMatrix should be greater than 0", "ERRO", MB_OK );
		return NULL;
	}
	_lpCobj = CobjAlloc( IDCOBJ_MATRIX, _row * _col * _axe * sizeof(double) + sizeof(Matrix) + sizeof(COBJ) );
	_lpDimMatrix = MAKE_DIMMATRIX( _lpCobj );
	_lpDimMatrix->row = _row;
	_lpDimMatrix->col = _col;
	_lpDimMatrix->axe = _axe;
	for(_i = 0; _i < _lpDimMatrix->row; _i++ )
		for(_j = 0; _j < _lpDimMatrix->col; _j++ )
			for(_k = 0; _k < _lpDimMatrix->col; _k++ )
				DIMMGET(_lpDimMatrix, _i, _j, _k) = 0.0;

	return _lpDimMatrix;
}

int _DLLFUNC mminus(LPMATRIX lpmResult, LPMATRIX lpma, LPMATRIX lpmb )
{
	int	_row, _col;

	//check the dim, if illegal
	if((lpma->row<=0)|(lpma->col<=0))
	{
		MessageBox(NULL, "err", "", MB_OK);
		return 0;
	}

	if((lpma->row)!=(lpmb->row))
	{
		MessageBox(NULL, "err", "", MB_OK);
		return 0;
	}

	if((lpma->col)!=(lpmb->col))
	{
		MessageBox(NULL, "err", "", MB_OK);
		return 0;
	}

	//then calcul.
	lpmResult->row = lpma->row;
	lpmResult->col = lpma->col;

	for(_row = 0; _row < lpma->row; _row++ )
		for(_col = 0; _col < lpma->col; _col++ )
			MGET(lpmResult, _row, _col) = MGET(lpma, _row, _col) - MGET(lpmb, _row, _col);
	return 1;
}

int _DLLFUNC  mmultiple(LPMATRIX lpmResult, LPMATRIX lpma, LPMATRIX lpmb )
{
	int		_row, _col, _temp;
	//check the dim, if illegal
	if((lpma->row <= 0) | (lpma->col <= 0))
	{
		MessageBox(NULL, "err", "", MB_OK);
		return 0;
	}

	if(lpma->col!=lpmb->row)
	{
		MessageBox(NULL, "err", "", MB_OK);
		return 0;
	}

	//then calcul.

	for(_row = 0; _row < lpma->row; _row++ )
		for(_col = 0; _col < lpmb->col; _col++ )
			MGET(lpmResult, _row , _col) = 0.0;

	lpmResult->row= lpma->row;
	lpmResult->col = lpmb->col;
	for( _row = 0; _row < lpma->row; _row ++)
		for(_col = 0; _col<lpmb->col; _col++)
		{
			for(_temp=0; _temp < lpma->col; _temp++)
			{
				MGET( lpmResult, _row, _col) += MGET(lpma, _row, _temp) * MGET(lpmb, _temp, _col);
			}
		}
/*	for( _row = 0; _row < lpma->row; _row ++)
		for(_col = 0; _col<lpmb->col; _col++)
		{
			data[_row][_col] = MGET(lpmResult, _row, _col);
		}

*/
	return 1;
}
int _DLLFUNC  Lmmultiple(LPMATRIX lpmResult, LPMATRIX lpma, LPMATRIX lpmb )
{
	long		_row, _col, _temp;
	//check the dim, if illegal
	if((lpma->row <= 0) | (lpma->col <= 0))
	{
		MessageBox(NULL, "err", "", MB_OK);
		return 0;
	}

	if(lpma->col!=lpmb->row)
	{
		MessageBox(NULL, "err", "", MB_OK);
		return 0;
	}

	//then calcul.

	for(_row = 0; _row < lpma->row; _row++ )
		for(_col = 0; _col < lpmb->col; _col++ )
			LMGET(lpmResult, _row , _col) = 0.0;

	lpmResult->row= lpma->row;
	lpmResult->col = lpmb->col;
	for( _row = 0; _row < lpma->row; _row ++)
		for(_col = 0; _col<lpmb->col; _col++)
		{
			for(_temp=0; _temp < lpma->col; _temp++)
			{
				LMGET( lpmResult, _row, _col) += LMGET(lpma, _row, _temp) * LMGET(lpmb, _temp, _col);
			}
		}
/*	for( _row = 0; _row < lpma->row; _row ++)
		for(_col = 0; _col<lpmb->col; _col++)
		{
			data[_row][_col] = MGET(lpmResult, _row, _col);
		}

*/
	return 1;
}



int _DLLFUNC  mtrans(LPMATRIX lpmResult, LPMATRIX lpmsrc)
{
	int	_row, _col;

	//check the dim, if illegal

	if(lpmsrc->row<=0)
	{
		MessageBox(NULL, "err", "", MB_OK);
		return 0;
	}

	//then calcul.

	lpmResult->row = lpmsrc->col;
	lpmResult->col = lpmsrc->row;
	for(_row = 0; _row < lpmsrc->col; _row++ )
		for(_col = 0; _col < lpmsrc->row; _col++ )
			MGET(lpmResult, _row, _col) = MGET(lpmsrc,_col, _row);

	return 1;
}


int _DLLFUNC  minvert(LPMATRIX lpmResult, LPMATRIX lpmsrc )
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
			MATRIXFREE(lpmtemp);
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
	MATRIXFREE(lpmtemp);
	return 1;


}

