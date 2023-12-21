#include "tfalgo.h"
#include "tfcmdmc.h"
#include "creal.h"
#include "cbool.h"
#include "ctint.h"
#ifdef debug
#include "tfdebug.h"
#endif

extern const char _MVDiscription[NUMBER_MVPARAM][20];
extern const char _CVDiscription[NUMBER_MVPARAM][20];
extern const char _DVDiscription[NUMBER_MVPARAM][20];

extern	HINSTANCE ghInst;
extern 	DWORD _type_MV[NUMBER_MVPARAM];
extern	DWORD _type_CV[NUMBER_CVPARAM];
extern 	DWORD _type_DV[NUMBER_DVPARAM];
extern char	LocalDirectory[128];
ACRET ZPrivParamInitialize( WRD wMTU_, WRD wNo_ )
{
#ifdef	__ZZK
	WRD				_wrd;
	char			_Name[128];
	char _szPathBuffer[_MAX_PATH];
	char _szDrive[_MAX_DRIVE];
	char _szDir[_MAX_DIR];
	char _szFName[_MAX_FNAME];
	char _szExt[_MAX_EXT];
	char	_szBuffer[128];
	LPSTR			_lpszSection;
	char			_szSection[128], _sz_i[128], _szFileNo[128];
	LPPU 			_lppu, _lptempPU, _lpRealPU;
	LPLINKMATRIX	_lpLinkMatrix;
	LPCOBJ			_lpCobj_CV, _lpCobj_MV, _lpCobj_DV;
	int				_i, _j, _kr, _kb, _kw, _kc, _FileNo;
	WORD		 	_mm, _pp, _ndnd;
	BOOL			_Okay = TRUE;
	REAL			_tempTEST;
	SimplePU		_sPU;
//	LPMATRIX		_lpStepDV, _lpStepMV;
//	LPCOBJ			_lpCobj_StepDV, _lpCobj_StepMV;
//	LPCOBJ			_lpCobj_PrivDV, _lpCobj_PrivMV, _lpCobj_PrivCV;

	_FileNo = wMTU_ - 5;
	if( _FileNo < 0 )
		_FileNo = 0;
	itoa( _FileNo, _szFileNo, 10 );
	_lppu = ModuGetParameter(wMTU_);
    GetModuleFileName( ghInst, _szPathBuffer, sizeof(_szPathBuffer) );
	_splitpath( _szPathBuffer, _szDrive, _szDir, _szFName, _szExt );
	if( wNo_ == MDMCIRVD_CV )
	{
		MFPUREAD( _lppu, MDMCIRVD_p, WORD, _pp );
		_lpLinkMatrix = CVStructAlloc( NUMBER_CVPARAM , _pp );
		strcpy( _szFName, "TFMCCCORECV" );
		strcat( _szFName, _szFileNo  );
		strcpy( _szExt, ".INI" );
		_makepath( _szPathBuffer, _szDrive, "\\sjzmcc\\", _szFName, _szExt );
		for( _i = 0; _i < _pp; _i++ )
		{
			strcpy( _szSection, "MCCCV" );
			itoa(_i, _sz_i , 10 );
			strcat( _szSection, _sz_i );
			strcpy( _szPathBuffer + strlen(_szPathBuffer) + 1, _szSection );
			_lpszSection = (LPSTR)_szPathBuffer + strlen(_szPathBuffer) + 1;
			_kr = _kb = _kw = _kc = 0;
			for( _j = 0; _j < NUMBER_CVPARAM; _j++ )
			{
				sprintf( _Name, "%s%s%d%s%s", "CV", _CVDiscription[_j], _FileNo, "-", _sz_i );
				GetPrivateProfileString( 	 _lpszSection,
											 _CVDiscription[_j],
											 "0",
											 _szBuffer,
											 sizeof(_szBuffer),
											 _szPathBuffer );
/*				WritePrivateProfileString( _lpszSection,
											 _CVDiscription[_j],
											 "",
											 _szPathBuffer );
*/				_lptempPU = DLM_GET_LPPU( _lpLinkMatrix, _j, _i );
				if( _type_CV[_j] == VSOF_REAL )
				{
					_wrd = RealCreate( _Name, 0, 0, 1000 );
					if( !_wrd )
					{
						MessageBox( NULL, "ERROR", "CVRealCreat", MB_OK );
						return ACRET_FALSE;
					}
					_lptempPU->prefix = PF_TBX|TBX_RT|RTU_VALUE;
					_lptempPU->du.zzk__WRD = _wrd;
					_sPU.prefix = PF_IMM|IMM_REAL;
					sscanf( _szBuffer, "%f", &_sPU.du.zzk__REAL);
					PUWrite( _lptempPU, &_sPU );
					_kr++;
				}

				if( _type_CV[_j] == VSOF_BOOL )
				{
					int	_temp;
					_wrd = BoolCreate( _Name, 0 );
					if( !_wrd )
					{
						MessageBox( NULL, "ERROR", "CVBoolCreat", MB_OK );
						return ACRET_FALSE;
					}
					_lptempPU->prefix = PF_TBX|TBX_BT|BTU_VALUE;
					_lptempPU->du.zzk__WRD = _wrd;
					_sPU.prefix = PF_IMM|IMM_BOOL;
					sscanf( _szBuffer, "%d", &_temp );
					if( _temp == 0 )
						_sPU.du.zzk__BOOL = 0;
					else
						_sPU.du.zzk__BOOL = 1;
					PUWrite( _lptempPU, &_sPU );
					_kb++;
				}
				if( _type_CV[_j] == VSOF_WORD )
				{
					_wrd = TintCreate( _Name, 0, 0, 10000 );
					if( !_wrd )
					{
						MessageBox( NULL, "ERROR", "CVWordCreat", MB_OK );
						return ACRET_FALSE;
					}
					_lptempPU->prefix = PF_TBX|TBX_TT|TTU_VALUE;
					_lptempPU->du.zzk__WRD = _wrd;
					_sPU.prefix = PF_IMM|IMM_WORD;
					sscanf( _szBuffer, "%d", &_sPU.du.zzk__WORD );
					PUWrite( _lptempPU, &_sPU );
					_kw++;
				}
				if( _type_CV[_j] == VSOF_LPCOBJ )
				{
					_lptempPU->prefix = PF_IMM|IMM_LPCOBJ;
					if( _j < CV_LSCALE )
					{
						if( _j == CV_TAG )
							_lptempPU->du.zzk__LPCOBJ = AllocCString( _Name, &_Okay );
						else
							_lptempPU->du.zzk__LPCOBJ = AllocCString( _szBuffer, &_Okay );
					}
					else
						_lptempPU->du.zzk__LPCOBJ = NULL;
					_kc++;
				}
			}
			_lpRealPU = DLM_GET_LPPU( _lpLinkMatrix, CV_VALUE, _i );
			_lptempPU = DLM_GET_LPPU( _lpLinkMatrix, CV_TAG, _i );
			_lpRealPU->prefix += ( RTU_TAG - RTU_VALUE );
			PUWrite( _lpRealPU, _lptempPU );
			_lpRealPU->prefix -= ( RTU_TAG - RTU_VALUE );
			_lptempPU = DLM_GET_LPPU( _lpLinkMatrix, CV_UNIT, _i );
			_lpRealPU->prefix += ( RTU_UNIT - RTU_VALUE );
			PUWrite( _lpRealPU, _lptempPU );
			_lpRealPU->prefix -= ( RTU_UNIT - RTU_VALUE );
			_lptempPU = DLM_GET_LPPU( _lpLinkMatrix, CV_NAME, _i );
			_lpRealPU->prefix += ( RTU_COMMENT - RTU_VALUE );
			PUWrite( _lpRealPU, _lptempPU );
			_lpRealPU->prefix -= ( RTU_COMMENT - RTU_VALUE );
			if( ( _kr + _kb + _kc + _kw ) != NUMBER_CVPARAM )
				MessageBox( NULL, "number of CV", "error", MB_OK );
		}
		_lpCobj_CV = LINKMATRIX_2_COBJ( _lpLinkMatrix );
		MFPUWRITE( _lppu, MDMCIRVD_CV, LPCOBJ, _lpCobj_CV );
	}
	if( wNo_ == MDMCIRVD_MV )
	{
		MFPUREAD( _lppu, MDMCIRVD_m, WORD, _mm );
		_lpLinkMatrix = MVStructAlloc( NUMBER_MVPARAM, _mm );
		strcpy( _szFName, "TFMCCCOREMV" );
		strcat( _szFName, _szFileNo  );
		strcpy( _szExt, ".INI" );
		_makepath( _szPathBuffer, _szDrive, "\\sjzmcc\\", _szFName, _szExt );
		for( _i = 0; _i < _mm; _i++ )
		{
			strcpy( _szSection, "MCCMV" );
			itoa(_i, _sz_i , 10 );
			strcat( _szSection, _sz_i );
			strcpy( _szPathBuffer + strlen(_szPathBuffer) + 1, _szSection );
			_lpszSection = (LPSTR)_szPathBuffer + strlen(_szPathBuffer) + 1;
			_kr = _kb = _kw = _kc = 0;
			for( _j = 0; _j < NUMBER_MVPARAM; _j++ )
			{
				
				sprintf( _Name, "%s%s%d%s%s", "MV", _MVDiscription[_j], _FileNo, "-", _sz_i );
				GetPrivateProfileString( 	 _lpszSection,
											 _MVDiscription[_j],
											 "0",
											 _szBuffer,
											 sizeof(_szBuffer),
											 _szPathBuffer );
/*				WritePrivateProfileString( _lpszSection,
											 _MVDiscription[_j],
											 "",
											 _szPathBuffer );
*/				_lptempPU = DLM_GET_LPPU( _lpLinkMatrix, _j, _i );
				if( _type_MV[_j] == VSOF_REAL )
				{
					_wrd = RealCreate( _Name, 0, 0, 1000 );
					if( !_wrd )
					{
						MessageBox( NULL, "ERROR", "MVRealCreat", MB_OK );
						return ACRET_FALSE;
					}
					_lptempPU->prefix = PF_TBX|TBX_RT|RTU_VALUE;
					_lptempPU->du.zzk__WRD = _wrd;
					_sPU.prefix = PF_IMM|IMM_REAL;
					sscanf( _szBuffer, "%f", &(_sPU.du.zzk__REAL) );
					PUWrite( _lptempPU, &_sPU );
					_kr++;
				}

				if( _type_MV[_j] == VSOF_BOOL )
				{
					int	_temp;
					_wrd = BoolCreate( _Name, 0 );
					if( !_wrd )
					{
						MessageBox( NULL, "ERROR", "MVBoolCreat", MB_OK );
						return ACRET_FALSE;
					}
					_lptempPU->prefix = PF_TBX|TBX_BT|BTU_VALUE;
					_lptempPU->du.zzk__WRD = _wrd;
					_sPU.prefix = PF_IMM|IMM_BOOL;
					sscanf( _szBuffer, "%d", &_temp );
					if( _temp == 0 )
						_sPU.du.zzk__BOOL = 0;
					else
						_sPU.du.zzk__BOOL = 1;
					PUWrite( _lptempPU, &_sPU );
					_kb++;
				}
				if( _type_MV[_j] == VSOF_WORD )
				{
					_wrd = TintCreate( _Name, 0, 0, 10000 );
					if( !_wrd )
					{
						MessageBox( NULL, "ERROR", "MVWordCreat", MB_OK );
						return ACRET_FALSE;
					}
					_lptempPU->prefix = PF_TBX|TBX_TT|TTU_VALUE;
					_lptempPU->du.zzk__WRD = _wrd;
					_sPU.prefix = PF_IMM|IMM_WORD;
					sscanf( _szBuffer, "%d", &(_sPU.du.zzk__WORD) );
					PUWrite( _lptempPU, &_sPU );
					_kw++;
				}
				if( _type_MV[_j] == VSOF_LPCOBJ )
				{
					_lptempPU->prefix = PF_IMM|IMM_LPCOBJ;
					if( _j < MV_LSCALE )
					{
						if( _j == MV_TAG )
							_lptempPU->du.zzk__LPCOBJ = AllocCString( _Name, &_Okay );
						else
							_lptempPU->du.zzk__LPCOBJ = AllocCString( _szBuffer, &_Okay );
					}
					else
						_lptempPU->du.zzk__LPCOBJ = NULL;
					_kc++;
				}
			}
			_lpRealPU = DLM_GET_LPPU( _lpLinkMatrix, MV_VALUE, _i );
			_lptempPU = DLM_GET_LPPU( _lpLinkMatrix, MV_TAG, _i );
			_lpRealPU->prefix += ( RTU_TAG - RTU_VALUE );
			PUWrite( _lpRealPU, _lptempPU );
			_lpRealPU->prefix -= ( RTU_TAG - RTU_VALUE );
			_lptempPU = DLM_GET_LPPU( _lpLinkMatrix, MV_UNIT, _i );
			_lpRealPU->prefix += ( RTU_UNIT - RTU_VALUE );
			PUWrite( _lpRealPU, _lptempPU );
			_lpRealPU->prefix -= ( RTU_UNIT - RTU_VALUE );
			_lptempPU = DLM_GET_LPPU( _lpLinkMatrix, MV_NAME, _i );
			_lpRealPU->prefix += ( RTU_COMMENT - RTU_VALUE );
			PUWrite( _lpRealPU, _lptempPU );
			_lpRealPU->prefix -= ( RTU_COMMENT - RTU_VALUE );
			if( ( _kr + _kb + _kc + _kw ) != NUMBER_MVPARAM )
			MessageBox( NULL, "number of MV", "error", MB_OK );
		}
		_lpCobj_MV = LINKMATRIX_2_COBJ( _lpLinkMatrix );
		MFPUWRITE( _lppu, MDMCIRVD_MV, LPCOBJ, _lpCobj_MV );
	}
	if( wNo_ == MDMCIRVD_DV )
	{
		MFPUREAD( _lppu, MDMCIRVD_nd, WORD, _ndnd );
		if( _ndnd == 0 )	return ACRET_TRUE;
		_lpLinkMatrix = DVStructAlloc( NUMBER_DVPARAM, _ndnd );
		strcpy( _szFName, "TFMCCCOREDV" );
		strcat( _szFName, _szFileNo  );
		strcpy( _szExt, ".INI" );
		_makepath( _szPathBuffer, _szDrive, "\\sjzmcc\\", _szFName, _szExt );
		for( _i = 0; _i < _ndnd; _i++ )
		{
			strcpy( _szSection, "MCCDV" );
			itoa(_i, _sz_i , 10 );
			strcat( _szSection, _sz_i );
			strcpy( _szPathBuffer + strlen(_szPathBuffer) + 1, _szSection );
			_lpszSection = (LPSTR)_szPathBuffer + strlen(_szPathBuffer) + 1;
			_kr = _kb = _kw = _kc = 0;
			for( _j = 0; _j < NUMBER_DVPARAM; _j++ )
			{
				sprintf( _Name, "%s%s%d%s%s", "DV", _DVDiscription[_j], _FileNo, "-", _sz_i );
/*				WritePrivateProfileString( _lpszSection,
											 _DVDiscription[_j],
											 "",
											 _szPathBuffer );
*/				GetPrivateProfileString( 	 _lpszSection,
											 _DVDiscription[_j],
											 "0",
											 _szBuffer,
											 sizeof(_szBuffer),
											 _szPathBuffer );
				_lptempPU = DLM_GET_LPPU( _lpLinkMatrix, _j, _i );
				if( _type_DV[_j] == VSOF_REAL )
				{
					_lptempPU->prefix = PF_TBX|TBX_RT|RTU_VALUE;
					_wrd = RealCreate( _Name, 0, 0, 1000 );
					if( !_wrd )
					{
						MessageBox( NULL, "ERROR", "DVRealCreat", MB_OK );
						return ACRET_FALSE;
					}
					_lptempPU->du.zzk__WRD = _wrd;
					sscanf( _szBuffer, "%f", &_tempTEST );
					_sPU.prefix = PF_IMM|IMM_REAL;
					_sPU.du.zzk__REAL = _tempTEST;
					PUWrite( _lptempPU, &_sPU );
					_kr++;
				}

				if( _type_DV[_j] == VSOF_WORD )
				{
					_wrd = TintCreate( _Name, 0, 0, 1000 );
					if( !_wrd )
					{
						MessageBox( NULL, "ERROR", "DVWordCreat", MB_OK );
						return ACRET_FALSE;
					}
					_lptempPU->prefix = PF_TBX|TBX_TT|TTU_VALUE;
					_lptempPU->du.zzk__WRD = (WRD)_wrd;
					_sPU.prefix = PF_IMM|IMM_WORD;
					sscanf( _szBuffer, "%d", &(_sPU.du.zzk__WORD) );
					PUWrite( _lptempPU, &_sPU );
					_kw++;
				}
				if( _type_DV[_j] == VSOF_BOOL )
				{
					int	_temp;
					_wrd = BoolCreate( _Name, 0 );
					if( !_wrd )
					{
						MessageBox( NULL, "ERROR", "DVBoolCreat", MB_OK );
						return ACRET_FALSE;
					}

					_lptempPU->prefix = PF_TBX|TBX_BT|BTU_VALUE;
					_lptempPU->du.zzk__WRD = _wrd;
					_sPU.prefix = PF_IMM|IMM_BOOL;
					sscanf( _szBuffer, "%d", &_temp );
					if( _temp == 0 )
						_sPU.du.zzk__BOOL = 0;
					else
						_sPU.du.zzk__BOOL = 1;
					PUWrite( _lptempPU, &_sPU );
					_kb++;
				}
				if( _type_DV[_j] == VSOF_LPCOBJ )
				{
					_lptempPU->prefix = PF_IMM|IMM_LPCOBJ;
					if( _j <= DV_UNIT )
					{
						if( _j == DV_TAG )
							_lptempPU->du.zzk__LPCOBJ = AllocCString( _Name, &_Okay );
						else
							_lptempPU->du.zzk__LPCOBJ = AllocCString( _szBuffer, &_Okay );
					}
					else
						_lptempPU->du.zzk__LPCOBJ = NULL;
					_kc++;
				}
			}
			_lpRealPU = DLM_GET_LPPU( _lpLinkMatrix, DV_VALUE, _i );
			_lptempPU = DLM_GET_LPPU( _lpLinkMatrix, DV_TAG, _i );
			_lpRealPU->prefix += ( RTU_TAG - RTU_VALUE );
			PUWrite( _lpRealPU, _lptempPU );
			_lpRealPU->prefix -= ( RTU_TAG - RTU_VALUE );
			_lptempPU = DLM_GET_LPPU( _lpLinkMatrix, DV_UNIT, _i );
			_lpRealPU->prefix += ( RTU_UNIT - RTU_VALUE );
			PUWrite( _lpRealPU, _lptempPU );
			_lpRealPU->prefix -= ( RTU_UNIT - RTU_VALUE );
			_lptempPU = DLM_GET_LPPU( _lpLinkMatrix, DV_NAME, _i );
			_lpRealPU->prefix += ( RTU_COMMENT - RTU_VALUE );
			PUWrite( _lpRealPU, _lptempPU );
			_lpRealPU->prefix -= ( RTU_COMMENT - RTU_VALUE );
			if( ( _kr + _kb + _kc + _kw ) != NUMBER_DVPARAM )
				MessageBox( NULL, "number of DV", "error", MB_OK );
		}
 		_lpCobj_DV = LINKMATRIX_2_COBJ( _lpLinkMatrix );
		MFPUWRITE( _lppu, MDMCIRVD_DV, LPCOBJ, _lpCobj_DV );
	}

#endif
	return ACRET_TRUE;
}
