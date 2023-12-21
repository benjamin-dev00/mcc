#include "tfalgo.h"
#include "tfcmdmc.h"

extern const char _MVDiscription[NUMBER_MVPARAM][20];
extern const char _CVDiscription[NUMBER_MVPARAM][20];
extern const char _DVDiscription[NUMBER_MVPARAM][20];

extern	HINSTANCE ghInst;
extern 	DWORD _type_MV[NUMBER_MVPARAM];
extern	DWORD _type_CV[NUMBER_CVPARAM];
extern 	DWORD _type_DV[NUMBER_DVPARAM];
ACRET ZPrivParamSave( WRD wMTU_, WRD wNo_ )
{
	char _szPathBuffer[_MAX_PATH];
	char _szDrive[_MAX_DRIVE];
	char _szDir[_MAX_DIR];
	char _szFName[_MAX_FNAME];
	char _szExt[_MAX_EXT];
	char	_szBuffer[128];
	LPSTR			_lpszSection;
	char			_szSection[128], _sz_i[128], _szFileNo[128];
	LPPU 			_lppu, _lptempPU;
	LPLINKMATRIX	_lpLinkMatrix;
	LPCOBJ			_lpCobj_CV, _lpCobj_MV, _lpCobj_DV;
	int				_i, _j, _kr, _kb, _kw, _kc, _FileNo;
	WORD		 	_mm, _pp, _ndnd;
	BOOL			_Okay = TRUE;
//	LPMATRIX		_lpStepDV, _lpStepMV;
//	LPCOBJ			_lpCobj_StepDV, _lpCobj_StepMV;
//	LPCOBJ			_lpCobj_PrivDV, _lpCobj_PrivMV, _lpCobj_PrivCV;

	_FileNo = wMTU_ - 4;
	if( _FileNo < 0 )
		_FileNo = 0;
	itoa( _FileNo, _szFileNo, 10 );
	_lppu = ModuGetParameter(wMTU_);
    GetModuleFileName( ghInst, _szPathBuffer, sizeof(_szPathBuffer) );
	_splitpath( _szPathBuffer, _szDrive, _szDir, _szFName, _szExt );
	if( wNo_ == MDMCIRVD_CV )
	{
		MFPUREAD( _lppu, MDMCIRVD_p, WORD, _pp );
		_lpLinkMatrix = CVStructAlloc( NUMBER_CVPARAM, _pp );
		for( _i = 0; _i < _pp; _i++ )
		{
			strcpy( _szFName, "MCCCV" );
			strcat( _szFName, _szFileNo );
			strcpy( _szExt, ".INI" );
			_makepath( _szPathBuffer, _szDrive, _szDir, _szFName, _szExt );
			strcpy( _szSection, "MCCCV" );
			itoa(_i, _sz_i , 10 );
			strcat( _szSection, _sz_i );
			strcpy( _szPathBuffer + strlen(_szPathBuffer) + 1, _szSection );
			_lpszSection = (LPSTR)_szPathBuffer + strlen(_szPathBuffer) + 1;
			_kr = _kb = _kw = _kc = 0;
			for( _j = 0; _j < NUMBER_CVPARAM; _j++ )
			{
				_lptempPU = DLM_GET_LPPU( _lpLinkMatrix, _j, _i );
				if( _type_CV[_j] == VSOF_REAL )
				{
					_kr++;
				}

				if( _type_CV[_j] == VSOF_BOOL );
				if( _type_CV[_j] == VSOF_WORD )
					_kw++;
				if( _type_CV[_j] == VSOF_LPCOBJ )
					_kc++;
			}
		}
		_lpCobj_CV = LINKMATRIX_2_COBJ( _lpLinkMatrix );
		MFPUWRITE( _lppu, MDMCIRVD_CV, LPCOBJ, _lpCobj_CV );
	}
	if( wNo_ == MDMCIRVD_MV )
	{
		MFPUREAD( _lppu, MDMCIRVD_m, WORD, _mm );
		_lpLinkMatrix = MVStructAlloc( NUMBER_MVPARAM, _mm );
		for( _i = 0; _i < _mm; _i++ )
		{
			strcpy( _szFName, "MCCMV" );
			strcat( _szFName, _szFileNo );
			strcpy( _szExt, ".INI" );
			_makepath( _szPathBuffer, _szDrive, _szDir, _szFName, _szExt );
			strcpy( _szSection, "MCCMV" );
			itoa(_i, _sz_i , 10 );
			strcat( _szSection, _sz_i );
			strcpy( _szPathBuffer + strlen(_szPathBuffer) + 1, _szSection );
			_lpszSection = (LPSTR)_szPathBuffer + strlen(_szPathBuffer) + 1;
			_kr = _kb = _kw = _kc = 0;
			for( _j = 0; _j < NUMBER_MVPARAM; _j++ )
			{
				
				_lptempPU = DLM_GET_LPPU( _lpLinkMatrix, _j, _i );
				if( _type_MV[_j] == VSOF_REAL )
				{
					_kr++;
				}

				if( _type_MV[_j] == VSOF_BOOL )
				{
					_kb++;
				}
				if( _type_MV[_j] == VSOF_WORD )
				{
					_kw++;
				}
				if( _type_MV[_j] == VSOF_LPCOBJ )
				{
					_kc++;
				}
			}
		}
		_lpCobj_MV = LINKMATRIX_2_COBJ( _lpLinkMatrix );
		MFPUWRITE( _lppu, MDMCIRVD_MV, LPCOBJ, _lpCobj_MV );
	}
	if( wNo_ == MDMCIRVD_DV )
	{
		MFPUREAD( _lppu, MDMCIRVD_nd, WORD, _ndnd );
		if( _ndnd == 0 )	return ACRET_TRUE;
		_lpLinkMatrix = DVStructAlloc( NUMBER_DVPARAM, _ndnd );
		for( _i = 0; _i < _ndnd; _i++ )
		{
			strcpy( _szFName, "MCCDV" );
			strcat( _szFName, _szFileNo );
			strcpy( _szExt, ".INI" );
			_makepath( _szPathBuffer, _szDrive, _szDir, _szFName, _szExt );
			strcpy( _szSection, "MCCDV" );
			itoa(_i, _sz_i , 10 );
			strcat( _szSection, _sz_i );
			strcpy( _szPathBuffer + strlen(_szPathBuffer) + 1, _szSection );
			_lpszSection = (LPSTR)_szPathBuffer + strlen(_szPathBuffer) + 1;
			_kr = _kb = _kw = _kc = 0;
			for( _j = 0; _j < NUMBER_DVPARAM; _j++ )
			{
				_lptempPU = DLM_GET_LPPU( _lpLinkMatrix, _j, _i );
				if( _type_DV[_j] == VSOF_REAL )
					sprintf( _szBuffer, "%f", _lptempPU->du.zzk__REAL );

				if( _type_DV[_j] == VSOF_WORD )
					sprintf( _szBuffer, "%d", _lptempPU->du.zzk__WORD );

				if( _type_DV[_j] == VSOF_BOOL )
					sprintf( _szBuffer, "%d", _lptempPU->du.zzk__BOOL );

				if( _type_DV[_j] == VSOF_LPCOBJ )
					sprintf( _szBuffer, "%s", _lptempPU->du.zzk__LPCOBJ );

				WritePrivateProfileString(	_lpszSection,
											_DVDiscription[_j],
											 _szBuffer,
											 _szPathBuffer );
			}
		}
 		_lpCobj_DV = LINKMATRIX_2_COBJ( _lpLinkMatrix );
		MFPUWRITE( _lppu, MDMCIRVD_DV, LPCOBJ, _lpCobj_DV );
	}

	return ACRET_TRUE;
}
