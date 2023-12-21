#include "tfalgo.h"
#include "tfcmdmc.h"
#include "tfcond.h"
#ifdef __ONSPEC
#include "Mtmfun.h"
#include "Mtdllfun.h"
#endif
#ifdef __debug
#include "tfdebug.h"
#endif
extern char LocalDirectory[128];
// Here Disturbance is considered to be constant
void CorSteady( LPCOBJPRIV lpCobj_All_, LPCOBJPRIVMV lpCobj_PrivMV_, LPCOBJPRIVCV lpCobj_PrivCV_, LPCOBJPRIVDV lpCobj_PrivDV_ )
{
	double			_miny = 0.0, _maxy = 0.0;
#ifdef __debug
double _Amply[mm+ndnd];
#endif
	WORD			_i, _j, _k, _Real_i, _Real_j; 
	LPMATRIX		_lpmSteady;
	LPMATRIX		_lpmCV, _lpmMV;
	LPDIMMATRIX		_lpmSR, _lpmDSR, _lpGDSR, _lpGSR;
	LPCOBJ			_lpCobj_GHistory;
	LPCOBJPRIV		_lpCobjPrivHistory, _lpGlobal;
	FILE			*_AmplyFile;
	char			_szFile[128], _sz_i[128];
	float			_temp3;

	_lpmSR = MAKE_DIMMATRIX( lpCobj_All_->LPSR );
	_lpmDSR = MAKE_DIMMATRIX( lpCobj_All_->LPDSR );
	_lpCobj_GHistory = lpCobj_All_->LPHistory;
	_lpCobjPrivHistory = COBJ_2_PRIV( _lpCobj_GHistory );
	_lpmCV = COBJ_2_MATRIX( lpCobj_All_->LPApartCV );
	_lpmMV = COBJ_2_MATRIX( lpCobj_All_->LPApartMV );
	_lpmSteady = COBJ_2_MATRIX( lpCobj_All_->LPSteady );
	if( lpCobj_All_->Flag_Steady == 0 &&
			_lpCobjPrivHistory->Flag_Steady == 1 )
	{
		itoa( ( lpCobj_All_->Apart_i + 1 ), _sz_i, 10 );
		sprintf( _szFile, "%s%s%s", LocalDirectory, "tfsteady.in", _sz_i );
		if ((_AmplyFile = fopen( _szFile, "r"))==NULL)
		{
			MessageBox( NULL, "No Debug file", "tfsteady.ini", MB_OK );
			return;
		}
		for( _i = 0; _i < lpCobj_All_->p; _i++ )
		{
			for( _j = 0; _j < lpCobj_All_->m; _j++ )
			{
				fscanf( _AmplyFile, "%10f", &_temp3 );
#ifdef __debug
				_Amply[_j] = _temp3;
#endif
				for( _k = 0; _k < lpCobj_All_->N; _k++ )
					DIMMGET( _lpmSR, _i, _j, _k ) *= _temp3;
			}
			for( _j = 0; _j < lpCobj_All_->nd; _j++ )
			{
				fscanf( _AmplyFile, "%10f", &_temp3 );
#ifdef __debug				
				_Amply[lpCobj_All_->m+_j] = _temp3;
#endif
				for( _k = 0; _k < lpCobj_All_->N; _k++ )
					DIMMGET( _lpmDSR, _i, _j, _k ) *= _temp3;
			}
			fscanf( _AmplyFile, "\n" );
		}
		fclose( _AmplyFile );
		_lpGlobal = COBJ_2_PRIV( lpCobj_All_->LPGlobal );
		_lpGSR = MAKE_DIMMATRIX( _lpGlobal->LPSR );
		_lpGDSR = MAKE_DIMMATRIX( _lpGlobal->LPDSR );
		for( _i = 0; _i < lpCobj_All_->p; _i++ )
		{
			_Real_i = (int)MGET( _lpmCV, lpCobj_All_->Apart_i, _i ) - 1;
			for( _j = 0; _j < lpCobj_All_->m; _j++ )
			{
				_Real_j = (int)MGET( _lpmMV, lpCobj_All_->Apart_i, _j ) - 1;
				for( _k = 0; _k < lpCobj_All_->N; _k++ )
				{
					DIMMGET( _lpGSR, _Real_i, _Real_j, _k ) = 
						DIMMGET( _lpmSR, _i, _j, _k );
				}
			}
		}
		lpCobj_All_->SteadyCount = 0;
		lpCobj_All_->CalSteady = 0;
		lpCobj_All_->Flag_CorSteady = 0;
	}
	return;
}