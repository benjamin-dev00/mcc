#include "tfalgo.h"
#include "tfcmdmc.h"
#include "creal.h"
#include "cbool.h"
#include "ctint.h"
extern const char		_MVDiscription[NUMBER_MVPARAM][20];
extern DWORD			_type_MV[NUMBER_MVPARAM];
extern const char		_CVDiscription[NUMBER_CVPARAM][20];
extern DWORD			_type_CV[NUMBER_CVPARAM];
extern const char		_DVDiscription[NUMBER_DVPARAM][20];
extern DWORD			_type_DV[NUMBER_DVPARAM];

ACRET StructTableCreat( WRD wMTU_ )
{
	LPPU			_lppu;
	char			_Name[128];
	LPCOBJ			_lpCobj_MV, _lpCobj_CV, _lpCobj_DV;
	WRD				_wrd;
	LPPU			_lptempPU;
	LPLINKMATRIX	_lpLinkMatrixMV, _lpLinkMatrixCV, _lpLinkMatrixDV;
	WORD			_mm, _pp, _k, _j, _i, _ndnd;

	_lppu = ModuGetParameter( wMTU_ );
	MFPUREAD( _lppu, MDMCIRVD_MV, LPCOBJ, _lpCobj_MV );
	MFPUREAD( _lppu, MDMCIRVD_CV, LPCOBJ, _lpCobj_CV );
	MFPUREAD( _lppu, MDMCIRVD_DV, LPCOBJ, _lpCobj_DV );
	MFPUREAD( _lppu, MDMCIRVD_m, WORD, _mm );
	MFPUREAD( _lppu, MDMCIRVD_p, WORD, _pp );
	MFPUREAD( _lppu, MDMCIRVD_nd, WORD, _ndnd );
	_lpLinkMatrixMV = COBJ_2_LINKMATRIX( _lpCobj_MV );
	_lpLinkMatrixCV = COBJ_2_LINKMATRIX( _lpCobj_CV );
	_lpLinkMatrixDV = COBJ_2_LINKMATRIX( _lpCobj_DV );

	for( _k = 0; _k < _mm; _k++ )
	{
		for( _j = 0; _j < NUMBER_MVPARAM; _j++ )
		{
			_lptempPU = DLM_GET_LPPU( _lpLinkMatrixMV, _j, _k );
			sprintf( _Name, "%s%d", _MVDiscription[_j], wMTU_ );
			if( _type_MV[_j] == VSOF_REAL )
			{
				_wrd = RealCreate( _Name, 0, 0, 100 );
				if( !_wrd )
					return ACRET_FALSE;
				_lptempPU->prefix = PF_TBX|TBX_RT;
				_lptempPU->du.zzk__WRD = _wrd;
			}
			if( _type_MV[_j] == VSOF_BOOL )
			{
				_wrd = BoolCreate( _Name, 0 );
				if( !_wrd )
					return ACRET_FALSE;
				_lptempPU->prefix = PF_TBX|TBX_BT;
				_lptempPU->du.zzk__WRD = _wrd;
			}
			if( _type_MV[_j] == VSOF_WORD )
			{
				_wrd = TintCreate( _Name, 0, 0, 1000 );
				if( !_wrd )
					return ACRET_FALSE;
				_lptempPU->prefix = PF_TBX|TBX_TT;
				_lptempPU->du.zzk__WRD = _wrd;
			}
		}
	}
	for( _i = 0; _i < _pp; _i++ )
	{
		for( _j = 0; _j < NUMBER_CVPARAM; _j++ )
		{
			_lptempPU = DLM_GET_LPPU( _lpLinkMatrixCV, _j, _i );
			sprintf( _Name, "%s%d", _CVDiscription[_j], wMTU_ );
			if( _type_CV[_j] == VSOF_REAL )
			{
				_wrd = RealCreate( _Name, 0, 0, 100 );
				if( !_wrd )
					return ACRET_FALSE;
				_lptempPU->prefix = PF_TBX|TBX_RT;
				_lptempPU->du.zzk__WRD = _wrd;
			}
			if( _type_CV[_j] == VSOF_BOOL )
			{
				_wrd = BoolCreate( _Name, 0 );
				if( !_wrd )
					return ACRET_FALSE;
				_lptempPU->prefix = PF_TBX|TBX_BT;
				_lptempPU->du.zzk__WRD = _wrd;
			}
			if( _type_CV[_j] == VSOF_WORD )
			{
				_wrd = TintCreate( _Name, 0, 0, 1000 );
				if( !_wrd )
					return ACRET_FALSE;
				_lptempPU->prefix = PF_TBX|TBX_TT;
				_lptempPU->du.zzk__WRD = _wrd;
			}
		}
	}
	for( _k = 0; _k < _ndnd; _k++ )
	{
		for( _j = 0; _j < NUMBER_DVPARAM; _j++ )
		{
			_lptempPU = DLM_GET_LPPU( _lpLinkMatrixDV, _j, _k );
			sprintf( _Name, "%s%d", _DVDiscription[_j], wMTU_ );
			if( _type_DV[_j] == VSOF_REAL )
			{
				_wrd = RealCreate( _Name, 0, 0, 100 );
				if( !_wrd )
					return ACRET_FALSE;
				_lptempPU->prefix = PF_TBX|TBX_RT;
				_lptempPU->du.zzk__WRD = _wrd;
			}
			if( _type_DV[_j] == VSOF_BOOL )
			{
				_wrd = BoolCreate( _Name, 0 );
				if( !_wrd )
					return ACRET_FALSE;
				_lptempPU->prefix = PF_TBX|TBX_BT;
				_lptempPU->du.zzk__WRD = _wrd;
			}
			if( _type_DV[_j] == VSOF_WORD )
			{
				_wrd = TintCreate( _Name, 0, 0, 1000 );
				if( !_wrd )
					return ACRET_FALSE;
				_lptempPU->prefix = PF_TBX|TBX_TT;
				_lptempPU->du.zzk__WRD = _wrd;
			}
		}
	}
	return	ACRET_TRUE;
}