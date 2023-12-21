#include "tfalgo.h"
#include "tfcmdmc.h"
#ifdef __ONSPEC
#include "mtmfun.h"
#include "mtdllfun.h"
#endif
#ifndef __SJZ__
#define __SJZ__
#endif

extern DWORD		_type_CV[NUMBER_CVPARAM];
extern const char		_CVDiscription[NUMBER_CVPARAM][20];
extern char LocalDirectory[128];
ACRET Read_FileCV( LPCOBJPRIV lpCobj_All_, LPCOBJPRIVCV lpCobj_PrivCV_ )
{
#ifdef __ONSPEC
	char			_szBuffer[128];
	char			_SectionName[128], _FileName[128], _szCVNo[128];
	BYTE FAR*		_lpStart;
	WORD			_j;
	float			_TempFloat;
#endif
	WORD				_i, _p;
//	double			_Fault;

	_p = lpCobj_All_->p;
#ifdef __ONSPEC
	strcpy( _FileName, LocalDirectory );
	strcat( _FileName, "TFMCCCV0.INI" );
	for( _i = 0; _i < _p; _i++ )
	{
		_lpStart = (BYTE FAR* )( PRIVCVGET( lpCobj_PrivCV_, _i, 0 ) );
		strcpy( _SectionName, "MCCCV" );
		itoa( _i, _szCVNo, 10 );
		strcat( _SectionName, _szCVNo );
		for( _j = 0; _j < NUMBER_CVPARAM; _j++ )
		{
			if( _type_CV[_j] != VSOF_LPCOBJ )
				GetPrivateProfileString( _SectionName,
										 _CVDiscription[_j],
										 "0",
										 _szBuffer,
										 sizeof(_szBuffer),
										_FileName );
			if( _j <= CV_UNIT )
			{
				if( _j == CV_VALUE )
				{
					sscanf( _szBuffer, "%f", &_TempFloat );
					PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CData = 
						(double)_TempFloat;
				}
			}
			else
				if( _j <= CV_HSET )
				{
					_lpStart = (BYTE FAR*)( &( PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CHScale ) );
					sscanf( _szBuffer, "%f", &_TempFloat );
					( * ( double* )( _lpStart + ( _j - CV_HSCALE ) * sizeof(double) ) ) = 
						(double)_TempFloat;
				}
				else
					if( _j <= CV_N )
					{
						_lpStart = (BYTE FAR*)( &(PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CRControl_type) );
						sscanf( _szBuffer, "%d", ( WORD* ) ( _lpStart + 
								( _j - CV_RCONTROLMODE ) * sizeof(WORD) ) );
					}
					else
						if( _j < CV_PRIV )
						{
							_lpStart = (BYTE FAR*)( &(PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CInvalid ) );
							sscanf( _szBuffer, "%d", ( BOOL* )( _lpStart + ( _j - CV_STATE ) * sizeof(BOOL) ) );
						}
		}
		PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CBad = 0;
		PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CHOverCount = 0;
		PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CLOverCount = 0;
		PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CDelayCount = 0;
		PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CLOver = 0;
		PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CHOver = 0;
		PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CNoLP = 0;
		PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CNoIll = 0;
		PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CAlarm = 0;
		PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CLPWrong = 0;
		PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CLEdge = 0;
		PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CHEdge = 0;
		PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CError = 0.0;
#ifdef __SJZ__
		if( _i == 0 )
		{
			PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CControl_type = 1;
			PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CCM = 1;
		}	
		else
		{
			PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CControl_type = 0;
			PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CCM = 0;
		}	
#endif
//		PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CControl_type = 
//			PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CCM;
	}
#endif
#ifdef _SetIni
	for( _i = 0; _i < _p; _i++ )
		PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CIni = 
			PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CData;
#endif
#ifdef __ZZK
	for( _i = 0; _i < _p; _i++ )
	{
		READCVPARAMETER( lpCobj_All_->LPCV, _i, CV_VALUE, REAL, PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CData );
		READCVPARAMETER( lpCobj_All_->LPCV, _i, CV_MAX, REAL, PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CMax );
		READCVPARAMETER( lpCobj_All_->LPCV, _i, CV_MIN, REAL, PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CMin );
		READCVPARAMETER( lpCobj_All_->LPCV, _i, CV_HIGH, REAL, PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CHigh );
		READCVPARAMETER( lpCobj_All_->LPCV, _i, CV_LOW, REAL, PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CLow );
	}
#endif
/*	for( _i = 0; _i < _p; _i++ )
	{
		if( PRIVCVGET( lpCobj_PrivCV_, _i, 0)->CMax <= 1 )
			_Fault = 0.005;
		else
			if( PRIVCVGET( lpCobj_PrivCV_, _i, 0)->CMax <= 10 )
				_Fault = 0.05;
			else
				if( PRIVCVGET( lpCobj_PrivCV_, _i, 0)->CMax <= 100 )
					_Fault = 0.5;
				else
					_Fault = 5;

		PRIVCVGET( lpCobj_PrivCV_, _i, 0)->CFault_max = 
			PRIVCVGET( lpCobj_PrivCV_, _i, 0)->CMax - _Fault;
		PRIVCVGET( lpCobj_PrivCV_, _i, 0)->CFault_min = 
			PRIVCVGET( lpCobj_PrivCV_, _i, 0)->CMin + _Fault;
		PRIVCVGET( lpCobj_PrivCV_, _i, 0)->CFault_high = 
			PRIVCVGET( lpCobj_PrivCV_, _i, 0)->CHigh - _Fault;
		PRIVCVGET( lpCobj_PrivCV_, _i, 0)->CFault_low = 
			PRIVCVGET( lpCobj_PrivCV_, _i, 0)->CLow + _Fault;
		PRIVCVGET( lpCobj_PrivCV_, _i, 0)->CFault_xsetpoint = 
			PRIVCVGET( lpCobj_PrivCV_, _i, 0)->CMax - 2 * _Fault;
		PRIVCVGET( lpCobj_PrivCV_, _i, 0)->CFault_nsetpoint = 
			PRIVCVGET( lpCobj_PrivCV_, _i, 0)->CMin + 2 * _Fault;
		PRIVCVGET( lpCobj_PrivCV_, _i, 0)->CFault_hsetpoint = 
			PRIVCVGET( lpCobj_PrivCV_, _i, 0)->CHigh - 2 * _Fault;
		PRIVCVGET( lpCobj_PrivCV_, _i, 0)->CFault_lsetpoint = 
			PRIVCVGET( lpCobj_PrivCV_, _i, 0)->CLow + 2 * _Fault;
		PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CIni = PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CData;
		WRITECVPARAMETER( lpCobj_All_->LPCV, _i, CV_FMAX, REAL, (REAL) PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CFault_max );
		WRITECVPARAMETER( lpCobj_All_->LPCV, _i, CV_FMIN, REAL, (REAL) PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CFault_min );
		WRITECVPARAMETER( lpCobj_All_->LPCV, _i, CV_FHIGH, REAL, (REAL) PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CFault_high );
		WRITECVPARAMETER( lpCobj_All_->LPCV, _i, CV_FLOW, REAL, (REAL) PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CFault_low );
		WRITECVPARAMETER( lpCobj_All_->LPCV, _i, CV_FXSET, REAL, (REAL) PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CFault_xsetpoint );
		WRITECVPARAMETER( lpCobj_All_->LPCV, _i, CV_FNSET, REAL, (REAL) PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CFault_nsetpoint );
		WRITECVPARAMETER( lpCobj_All_->LPCV, _i, CV_FHSET, REAL, (REAL) PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CFault_hsetpoint );
		WRITECVPARAMETER( lpCobj_All_->LPCV, _i, CV_FLSET, REAL, (REAL) PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CFault_lsetpoint );
		WRITECVPARAMETER( lpCobj_All_->LPCV, _i, CV_INI, REAL, (REAL) PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CIni );
	}
*/
	return ACRET_TRUE;
}
