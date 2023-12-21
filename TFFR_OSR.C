#include "tfalgo.h"
#include "tfcmdmc.h"
#ifdef __ONSPEC
#include "Mtmfun.h"
#include "Mtdllfun.h"
#endif

extern char LocalDirectory[128];
ACRET ReadFileOutSR( LPCOBJPRIV lpCobjPrivAll_ )
{
	int				_i, _j, _k, _l;
	FILE*			_fin;
	char			_sz_i[128], _FileName[128], _sz_j[128], _sz_k[128], _szTemp_i[128], _szTemp_j[128], _szTemp_k[128];
	float			_temp;
	LPCOBJ			_lpCobj_SR, _lpCobj_DSR;
	LPDIMMATRIX		_lpDimSR, _lpDimDSR;
	

	READALLPARAMETER( lpCobjPrivAll_, MDMCIRVD_OUTSR,LPCOBJ,_lpCobj_SR );	
	READALLPARAMETER( lpCobjPrivAll_, MDMCIRVD_OUTDSR,LPCOBJ,_lpCobj_DSR );	

	_lpDimSR = MAKE_DIMMATRIX( _lpCobj_SR );
	_lpDimDSR = MAKE_DIMMATRIX( _lpCobj_DSR );
	for( _i = 0; _i < lpCobjPrivAll_->p; _i++ )
	{
		itoa( (_i+1), _szTemp_i, 10 );
		if( (_i+1) < 10 )
			sprintf( _sz_i, "%s%s", "0", _szTemp_i );
		else
			sprintf( _sz_i, "%s", _szTemp_i );
		for( _j = 0; _j < lpCobjPrivAll_->m; _j++ )
		{
			itoa( (_j+1), _szTemp_j, 10 );
			if( (_j+1) < 10 )
				sprintf( _sz_j, "%s%s", "0", _szTemp_j );
			else
				sprintf( _sz_j, "%s", _szTemp_j );
			sprintf( _FileName, "%s%s%s%s%s%s", LocalDirectory, "mccostp\\CV", _sz_i, "MV", _sz_j, ".sto" );
			if ((_fin=fopen( _FileName, "r"))==NULL)
			{
//				MessageBox( NULL, "cannot open stp file\n", "", MB_OK );
//				return ACRET_FALSE;
				continue;
			}
			fscanf( _fin, "%f\n", &_temp );
			for( _l = 0; _l < lpCobjPrivAll_->N; _l++ )
			{
				fscanf( _fin, "%f\n", &_temp );
				DIMMGET( _lpDimSR, _i, _j, _l ) = 
					(double)_temp;
			}
			fclose( _fin );
		}
		if( lpCobjPrivAll_->nd )
		{
			for( _k = 0; _k < lpCobjPrivAll_->nd; _k++ )		
			{
				itoa( (_k+1), _szTemp_k, 10 );
				if( (_k+1) < 10 )
					sprintf( _sz_k, "%s%s", "0", _szTemp_k );
				else
					sprintf( _sz_k, "%s", _szTemp_k );
				sprintf( _FileName, "%s%s%s%s%s%s", LocalDirectory, "mccostp\\CV", _sz_i, "DV", _sz_k, ".sto" );
				if ((_fin=fopen( _FileName, "r"))==NULL)
				{
					//MessageBox( NULL, "cannot open stp file\n", "", MB_OK );
					//return ACRET_FALSE;
					continue;
				}
				fscanf( _fin, "%f\n", &_temp );
				for( _l = 0; _l < lpCobjPrivAll_->N; _l++ )
				{
					fscanf( _fin, "%f\n", &_temp );
					DIMMGET( _lpDimDSR, _i, _k, _l ) = 
						(double)_temp;
				}
				fclose( _fin );
			}
		}
	}
	return ACRET_TRUE;
};
