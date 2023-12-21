#include "tfalgo.h"
#include "tfcmdmc.h"
#include "creal.h"

void READSCALE( LPCOBJPRIV lpCobj_All_, LPCOBJPRIVMV lpCobj_PrivMV_,
					LPCOBJPRIVCV lpCobj_PrivCV_, LPCOBJPRIVDV lpCobj_PrivDV_ )
{
#ifdef __ZZK
	int				_i, _j, _k;
	SimplePU		__pu_;
	LPLINKMATRIX	_lpLinkMatrix;
	LPPU			_lptempPU;

	_lpLinkMatrix = COBJ_2_LINKMATRIX( lpCobj_All_->LPMV );
	for( _j = 0; _j < lpCobj_All_->m; _j++ )
	{
		_lptempPU = DLM_GET_LPPU( _lpLinkMatrix, MV_VALUE, _j );
		_lptempPU->prefix += ( RTU_MAX - RTU_VALUE );
		__pu_.prefix = PF_IMM|IMM_REAL;
		__pu_.du.zzk__FULL = (WORD)NULL;
		PURead( &__pu_, (_lptempPU) );
		PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MHScale = __pu_.du.zzk__REAL;
		_lptempPU->prefix -= ( RTU_MAX - RTU_VALUE );
		_lptempPU->prefix += ( RTU_MIN - RTU_VALUE );
		PURead( &__pu_, (_lptempPU) );
		PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MLScale = __pu_.du.zzk__REAL;
		_lptempPU->prefix -= ( RTU_MIN - RTU_VALUE );
		_lptempPU->prefix += ( RTU_HIHI - RTU_VALUE );
		PURead( &__pu_, (_lptempPU) );
		PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MHAlarm = __pu_.du.zzk__REAL;
		_lptempPU->prefix -= ( RTU_HIHI - RTU_VALUE );
		_lptempPU->prefix += ( RTU_LOLO - RTU_VALUE );
		PURead( &__pu_, (_lptempPU) );
		PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MLAlarm = __pu_.du.zzk__REAL;
		_lptempPU->prefix -= ( RTU_LOLO - RTU_VALUE );
	}
	_lpLinkMatrix = COBJ_2_LINKMATRIX( lpCobj_All_->LPCV );
	for( _i = 0; _i < lpCobj_All_->p; _i++ )
	{
		_lptempPU = DLM_GET_LPPU( _lpLinkMatrix, CV_VALUE, _i );
		_lptempPU->prefix += ( RTU_MAX - RTU_VALUE );
		__pu_.prefix = PF_IMM|IMM_REAL;
		__pu_.du.zzk__FULL = (WORD)NULL;
		PURead( &__pu_, (_lptempPU) );
		PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CHScale = __pu_.du.zzk__REAL;
		_lptempPU->prefix -= ( RTU_MAX - RTU_VALUE );
		_lptempPU->prefix += ( RTU_MIN - RTU_VALUE );
		PURead( &__pu_, (_lptempPU) );
		PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CLScale = __pu_.du.zzk__REAL;
		_lptempPU->prefix -= ( RTU_MIN - RTU_VALUE );
		_lptempPU->prefix += ( RTU_HIHI - RTU_VALUE );
		PURead( &__pu_, (_lptempPU) );
		PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CHAlarm = __pu_.du.zzk__REAL;
		_lptempPU->prefix -= ( RTU_HIHI - RTU_VALUE );
		_lptempPU->prefix += ( RTU_LOLO - RTU_VALUE );
		PURead( &__pu_, (_lptempPU) );
		PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CLAlarm = __pu_.du.zzk__REAL;
		_lptempPU->prefix -= ( RTU_LOLO - RTU_VALUE );
	}
	_lpLinkMatrix = COBJ_2_LINKMATRIX( lpCobj_All_->LPDV );
	for( _k = 0; _k < lpCobj_All_->nd; _k++ )
	{
		_lptempPU = DLM_GET_LPPU( _lpLinkMatrix, DV_VALUE, _k );
		_lptempPU->prefix += ( RTU_MAX - RTU_VALUE );
		__pu_.prefix = PF_IMM|IMM_REAL;
		__pu_.du.zzk__FULL = (WORD)NULL;
		PURead( &__pu_, (_lptempPU) );
		PRIVDVGET( lpCobj_PrivDV_, _k, 0 )->DHScale = __pu_.du.zzk__REAL;
		_lptempPU->prefix -= ( RTU_MAX - RTU_VALUE );
		_lptempPU->prefix += ( RTU_MIN - RTU_VALUE );
		PURead( &__pu_, (_lptempPU) );
		PRIVDVGET( lpCobj_PrivDV_, _k, 0 )->DLScale = __pu_.du.zzk__REAL;
		_lptempPU->prefix -= ( RTU_MIN - RTU_VALUE );
	}
#endif
	return;
}
