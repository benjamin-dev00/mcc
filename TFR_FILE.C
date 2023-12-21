#include "tfalgo.h"
#include "tfcmdmc.h"
#ifdef __ONSPEC
#include "Mtmfun.h"
#include "Mtdllfun.h"
#endif
const char	GlobalDiscription[NUMBER_GLOBAL][40] =
{
"File_No",
"_N",
"_P",
"_M",
"p",
"m",
"nd",
"count",
"M_ParaCount",
"RecountCount",
"LPCount",
"No_Apart",
"FreeParaCount",
"FreeParaMax",
"SteadyStep",
"ErrorCode",
"A/M",
"FreeCount",
"MaxDelay",
"Modu",
"N1",
"N2",
"SteadyCount",
"CalSteadyMax",
"Apart_i",
"BadMax",
"CVClassif",
"MVClassif",
"Flag_Fault",
"Flag_LP",
"ALARM",
"ReSetPara",
"SRMode",
"ModelChange",
"LPChange",
"Hope_AM",
"Recount",
"LPRecount",
"StartMode",
"WeightMode",
"DebugMode",
"HMode",
"AM1",
"AM2",
"AM3",
"Run",
"Steady",
"MCVRecout",
"RDebug",
"Save",
"IniStart",
"Flag_CorSteady",
"CalSteady",
"Error",
"LPNUM_K",
"alpha",
"Big_Cond",
"PBroken",
"QBroken",
"RBroken",
"SRError",
"DSRError",
"Cor_Steady",
"SampleT",
"DFilter",
};

extern char LocalDirectory[128];
ACRET Read_FileAll( LPCOBJPRIV lpCobj_PrivAll_ )
{
	char			_FileName[128], _szBuffer[128];
	int				_i;
	BYTE FAR*		_lp;
	float			_TempFloat;

	strcpy( _FileName, LocalDirectory );
#ifdef __ONSPEC
	strcat( _FileName, "TFMCC.INI" );
#else
	strcat( _FileName, "TFMCCCORE.INI" );
#endif
	for( _i = 0; _i <= MDMCIRVD_SAMPLETIME; _i++ )
	{
		GetPrivateProfileString( "MCC",
								 GlobalDiscription[_i],
								 "0",
								 _szBuffer,
								 sizeof(_szBuffer),
								_FileName );
		if( _i < NUMBER_GLOBALPARAMWORD )
		{
			if( _i < MDMCIRVD_FLAGAM )
			{
				sscanf( _szBuffer, "%d", ( ( WORD * )( (BYTE FAR*)lpCobj_PrivAll_ + 
							_i * sizeof(WORD) ) ) );
			}
			else
			{
				if( _i == MDMCIRVD_FLAGAM )
				{
					sscanf( _szBuffer, "%d", &(lpCobj_PrivAll_->Flag_AM ) );
				}
				if( _i == MDMCIRVD_FREECOUNT )
				{
					sscanf( _szBuffer, "%d", &(lpCobj_PrivAll_->Freecount) );
				}
				else
				{
					_lp = (BYTE FAR* )( &(lpCobj_PrivAll_->MaxDelay) );
					sscanf( _szBuffer, "%d", ( ( WORD * )( (BYTE FAR*)_lp + 
							( _i - MDMCIRVD_MAXDELAY ) * sizeof(WORD) ) ) );
				}
			}	
		}
		else
			if( _i < NUMBER_GLOBALPARAMWORD + NUMBER_GLOBALPARAMBOOL )
			{
				_lp = (BYTE FAR* )( &(lpCobj_PrivAll_->Flag_CControl) );
				sscanf( _szBuffer, "%d", ( ( BOOL * )( _lp + ( _i - NUMBER_GLOBALPARAMWORD ) * sizeof( BOOL ) ) )  );
			}
			else
				if( _i < NUMBER_GLOBALPARAMWORD + NUMBER_GLOBALPARAMBOOL + NUMBER_GLOBALPARAMDOUBLE )
				{
					_lp = (BYTE FAR* )( &( lpCobj_PrivAll_->LpK ) );
					sscanf( _szBuffer, "%f",  &_TempFloat );
					(* ( ( double *)( _lp + ( _i - NUMBER_GLOBALPARAMWORD - NUMBER_GLOBALPARAMBOOL )* sizeof(double) ) ) )
						= (double)_TempFloat;
				}
	}
	GetPrivateProfileString( "MCC",
							 "Modu",
							 "0",
							 _szBuffer,
							 sizeof(_szBuffer),
							_FileName );
	sscanf( _szBuffer, "%d", &lpCobj_PrivAll_->wModu );
	GetPrivateProfileString( "MCC",
							 "Save",
							 "0",
							 _szBuffer,
							 sizeof(_szBuffer),
							_FileName );
	sscanf( _szBuffer, "%d", &lpCobj_PrivAll_->Flag_Save );
	GetPrivateProfileString( "MCC",
							 "DFilter",
							 "0",
							 _szBuffer,
							 sizeof(_szBuffer),
							_FileName );
	sscanf( _szBuffer, "%f", &_TempFloat );
	lpCobj_PrivAll_->DFilter = (double)_TempFloat;
	GetPrivateProfileString( "MCC",
							 "ErrorCode",
							 "0",
							 _szBuffer,
							 sizeof(_szBuffer),
							_FileName );
	sscanf( _szBuffer, "%d", &lpCobj_PrivAll_->ErrorCode );
	GetPrivateProfileString( "MCC",
							 "IniStart",
							 "0",
							 _szBuffer,
							 sizeof(_szBuffer),
							_FileName );
	sscanf( _szBuffer, "%d", &lpCobj_PrivAll_->IniStart );
	GetPrivateProfileString( "MCC",
							 "Steady",
							 "0",
							 _szBuffer,
							 sizeof(_szBuffer),
							_FileName );
	sscanf( _szBuffer, "%d", &lpCobj_PrivAll_->Flag_Steady );
	GetPrivateProfileString( "MCC",
							 "Cor_Steady",
							 "0",
							 _szBuffer,
							 sizeof(_szBuffer),
							_FileName );
	sscanf( _szBuffer, "%f", &_TempFloat );
	lpCobj_PrivAll_->Cor_Steady = (double)_TempFloat;
	GetPrivateProfileString( "MCC",
							 "SteadyStep",
							 "0",
							 _szBuffer,
							 sizeof(_szBuffer),
							_FileName );
	sscanf( _szBuffer, "%d", &lpCobj_PrivAll_->SteadyStep );
/*	GetPrivateProfileString( "MCC",
							 "Delay",
							 "0",
							 _szBuffer,
							 sizeof(_szBuffer),
							_FileName );
/	sscanf( _szBuffer, "%d", &lpCobj_PrivAll_->Delay );*/
	GetPrivateProfileString( "MCC",
							 "N1",
							 "0",
							 _szBuffer,
							 sizeof(_szBuffer),
							_FileName );
	sscanf( _szBuffer, "%d", &lpCobj_PrivAll_->N1 );
	GetPrivateProfileString( "MCC",
							 "N2",
							 "0",
							 _szBuffer,
							 sizeof(_szBuffer),
							_FileName );
	sscanf( _szBuffer, "%d", &lpCobj_PrivAll_->N2 );
	GetPrivateProfileString( "MCC",
							 "CalSteady",
							 "0",
							 _szBuffer,
							 sizeof(_szBuffer),
							_FileName );
	sscanf( _szBuffer, "%d", &lpCobj_PrivAll_->CalSteady );
	GetPrivateProfileString( "MCC",
							 "CalSteadyMax",
							 "0",
							 _szBuffer,
							 sizeof(_szBuffer),
							_FileName );
	sscanf( _szBuffer, "%d", &lpCobj_PrivAll_->CalSteadyMax );
	lpCobj_PrivAll_->LPCount = 0;
	lpCobj_PrivAll_->MakeParaCount = 0;
	lpCobj_PrivAll_->RecountCount = 0;
	lpCobj_PrivAll_->MParaCount = 0;
	lpCobj_PrivAll_->Flag_LPRecount = 0;
	lpCobj_PrivAll_->Flag_Recount = 0;
	lpCobj_PrivAll_->ErrorCode = 0;
	lpCobj_PrivAll_->Count = 0;
	lpCobj_PrivAll_->Flag_Alarm = 0;

	return ACRET_TRUE;
}
