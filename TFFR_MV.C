#include "tfalgo.h"
#include "tfcmdmc.h"
#ifdef __ONSPEC
#include "mtmfun.h"
#include "mtdllfun.h"
#endif
#ifndef __SJZ__
#define __SJZ__
#endif

extern const char		_MVDiscription[NUMBER_MVPARAM][20];
extern DWORD		_type_MV[NUMBER_MVPARAM];

extern char LocalDirectory[128];

ACRET Read_FileMV( LPCOBJPRIV lpCobj_All_, LPCOBJPRIVMV lpCobj_PrivMV_ )
{
	int				_i, _m;
	WORD			_j;
//	double			_Fault;
#ifdef __ONSPEC
	char			_szBuffer[128];
	char			_szSection[128], _sz_i[128], _FileName[128];
	BYTE FAR*		_lpStart;
	float	_TempFloat;
	
	strcpy( _FileName, LocalDirectory );
	strcat( _FileName, "TfMCCMV0.INI" );

#endif
	_m = lpCobj_All_->m;
#ifdef __ONSPEC
	for( _i = 0; _i < _m; _i++ )
	{
		strcpy( _szSection, "MCCMV" );
		itoa(_i, _sz_i , 10 );
		strcat( _szSection, _sz_i );
		_lpStart = (BYTE FAR*)(&(PRIVMVGET( lpCobj_PrivMV_, _i, 0 )->MHScale));
		for( _j = 0; _j < NUMBER_MVPARAM; _j++ )
		{
			if( _type_MV[_j] != VSOF_LPCOBJ )
				GetPrivateProfileString( _szSection,
										 _MVDiscription[_j],
										 "0",
										 _szBuffer,
										 sizeof(_szBuffer),
										_FileName );
			if( _j <= MV_UNIT )
			{
				if( _j == MV_VALUE )
				{
					sscanf( _szBuffer, "%f", &_TempFloat );
					PRIVMVGET( lpCobj_PrivMV_, _i, 0 )->MData = (double)_TempFloat;
				}
			}
			else
			{
				if( _j <= MV_MLASTLOW )
				{
					float	_TempFloat;
					sscanf( _szBuffer, "%f", &_TempFloat );
					(* ( double * )( (BYTE FAR*)_lpStart + 
								( _j - MV_HSCALE ) * sizeof(double) ) ) = (double)_TempFloat;
				}
				else
				{
					if( _j <= MV_BAD )
					{
						_lpStart = (BYTE FAR*)( &( PRIVMVGET( lpCobj_PrivMV_, _i, 0 )->MRControl_type ) );
						sscanf( _szBuffer, "%d", ( WORD * )( (BYTE FAR*)_lpStart + 
									( _j - MV_RCONTROLMODE )  * sizeof(WORD) ) );
					}
					else
					{
						if( _j < MV_PRIV )
						{
							_lpStart = (BYTE FAR*)( &( PRIVMVGET( lpCobj_PrivMV_, _i, 0 )->MInvalidate ) );
							sscanf( _szBuffer, "%d", ( BOOL * )( (BYTE FAR*)_lpStart + 
									( _j - MV_STATE )* sizeof(BOOL) ) );
						}
					}
				}
			}
		}
		PRIVMVGET( lpCobj_PrivMV_, _i, 0 )->MBad = 0;
		PRIVMVGET( lpCobj_PrivMV_, _i, 0 )->MHOverCount = 0;
		PRIVMVGET( lpCobj_PrivMV_, _i, 0 )->MLOverCount = 0;
		PRIVMVGET( lpCobj_PrivMV_, _i, 0 )->MLEdge = 0;
		PRIVMVGET( lpCobj_PrivMV_, _i, 0 )->MHEdge = 0;
		PRIVMVGET( lpCobj_PrivMV_, _i, 0 )->MHope_AM = 1;
		PRIVMVGET( lpCobj_PrivMV_, _i, 0 )->MAlarm = 0;
		PRIVMVGET( lpCobj_PrivMV_, _i, 0 )->MSetSatuation = 0;
		PRIVMVGET( lpCobj_PrivMV_, _i, 0 )->MIRVWrong = 0;
		PRIVMVGET( lpCobj_PrivMV_, _i, 0 )->MSatuation = 0;
		PRIVMVGET( lpCobj_PrivMV_, _i, 0 )->MDelt = 0;
#ifdef __SJZ__
		PRIVMVGET( lpCobj_PrivMV_, _i, 0 )->MControl_type = 0;
		PRIVMVGET( lpCobj_PrivMV_, _i, 0 )->MCM = 0;
#endif

//		PRIVMVGET( lpCobj_PrivMV_, _i, 0)->MControl_type = 
//			PRIVMVGET( lpCobj_PrivMV_, _i, 0)->MCM;
	}
#endif
#ifdef _SetIni
	for( _j = 0; _j < _m; _j++ )
		PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MIni  = 
			PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MData;
#endif
#ifdef __ZZK
	for( _i = 0; _i < _m; _i++ )
	{
		READMVPARAMETER( lpCobj_All_->LPMV, (WORD)_i, MV_MAX, REAL, PRIVMVGET( lpCobj_PrivMV_, _i, 0)->MMax );
		READMVPARAMETER( lpCobj_All_->LPMV, (WORD)_i, MV_MIN, REAL, PRIVMVGET( lpCobj_PrivMV_, _i, 0)->MMin );
		READMVPARAMETER( lpCobj_All_->LPMV, (WORD)_i, MV_HIGH, REAL, PRIVMVGET( lpCobj_PrivMV_, _i, 0)->MIrvHigh );
		READMVPARAMETER( lpCobj_All_->LPMV, (WORD)_i, MV_LOW, REAL, PRIVMVGET( lpCobj_PrivMV_, _i, 0)->MIrvLow );
		READMVPARAMETER( lpCobj_All_->LPMV, (WORD)_i, MV_VALUE, REAL, PRIVMVGET( lpCobj_PrivMV_, _i, 0)->MData );
	}
#endif
/*	for( _i = 0; _i < _m; _i++ )
	{
		if( PRIVMVGET( lpCobj_PrivMV_, _i, 0)->MMax <= 1 )
			_Fault = 0.005;
		else
			if( PRIVMVGET( lpCobj_PrivMV_, _i, 0)->MMax <= 10 )
				_Fault = 0.05;
			else
				if( PRIVMVGET( lpCobj_PrivMV_, _i, 0)->MMax <= 100 )
					_Fault = 0.5;
				else
					_Fault = 5;
		PRIVMVGET( lpCobj_PrivMV_, _i, 0)->MFault_max = 
			PRIVMVGET( lpCobj_PrivMV_, _i, 0)->MMax - _Fault;
		PRIVMVGET( lpCobj_PrivMV_, _i, 0)->MFault_min = 
			PRIVMVGET( lpCobj_PrivMV_, _i, 0)->MMin + _Fault;
		PRIVMVGET( lpCobj_PrivMV_, _i, 0)->MIrvFault_high = 
			PRIVMVGET( lpCobj_PrivMV_, _i, 0)->MIrvHigh - _Fault;
		PRIVMVGET( lpCobj_PrivMV_, _i, 0)->MIrvFault_low = 
			PRIVMVGET( lpCobj_PrivMV_, _i, 0)->MIrvLow + _Fault;
		PRIVMVGET( lpCobj_PrivMV_, _i, 0)->MFault_xirv = 
			PRIVMVGET( lpCobj_PrivMV_, _i, 0)->MMax - 2 * _Fault;
		PRIVMVGET( lpCobj_PrivMV_, _i, 0)->MFault_nirv = 
			PRIVMVGET( lpCobj_PrivMV_, _i, 0)->MMin + 2 * _Fault;
		PRIVMVGET( lpCobj_PrivMV_, _i, 0)->MFault_hirv = 
			PRIVMVGET( lpCobj_PrivMV_, _i, 0)->MIrvHigh - 2 * _Fault;
		PRIVMVGET( lpCobj_PrivMV_, _i, 0)->MFault_lirv = 
			PRIVMVGET( lpCobj_PrivMV_, _i, 0)->MIrvLow + 2 * _Fault;
		PRIVMVGET( lpCobj_PrivMV_, _i, 0)->MIni = PRIVMVGET( lpCobj_PrivMV_, _i, 0)->MData; 
		WRITEMVPARAMETER( lpCobj_All_->LPMV, (WORD)_i, MV_INI, REAL, (REAL)PRIVMVGET( lpCobj_PrivMV_, _i, 0)->MIni );
		WRITEMVPARAMETER( lpCobj_All_->LPMV, (WORD)_i, MV_FMAX, REAL, (REAL)PRIVMVGET( lpCobj_PrivMV_, _i, 0)->MFault_max );
		WRITEMVPARAMETER( lpCobj_All_->LPMV, (WORD)_i, MV_FMIN, REAL, (REAL)PRIVMVGET( lpCobj_PrivMV_, _i, 0)->MFault_min );
		WRITEMVPARAMETER( lpCobj_All_->LPMV, (WORD)_i, MV_FHIGH, REAL, (REAL)PRIVMVGET( lpCobj_PrivMV_, _i, 0)->MIrvFault_high );
		WRITEMVPARAMETER( lpCobj_All_->LPMV, (WORD)_i, MV_FLOW, REAL, (REAL)PRIVMVGET( lpCobj_PrivMV_, _i, 0)->MIrvFault_low );
		WRITEMVPARAMETER( lpCobj_All_->LPMV, (WORD)_i, MV_FXSET, REAL, (REAL)PRIVMVGET( lpCobj_PrivMV_, _i, 0)->MFault_xirv );
		WRITEMVPARAMETER( lpCobj_All_->LPMV, (WORD)_i, MV_FNSET, REAL, (REAL)PRIVMVGET( lpCobj_PrivMV_, _i, 0)->MFault_nirv );
		WRITEMVPARAMETER( lpCobj_All_->LPMV, (WORD)_i, MV_FHSET, REAL, (REAL)PRIVMVGET( lpCobj_PrivMV_, _i, 0)->MFault_hirv );
		WRITEMVPARAMETER( lpCobj_All_->LPMV, (WORD)_i, MV_FLSET, REAL, (REAL)PRIVMVGET( lpCobj_PrivMV_, _i, 0)->MFault_lirv );
	}*/
	return ACRET_TRUE;
}
