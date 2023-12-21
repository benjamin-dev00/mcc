#include "tfalgo.h"
#include "tfcmdmc.h"
#ifdef __ONSPEC
#include "Mtmfun.h"
#include "Mtdllfun.h"
#endif

extern char LocalDirectory[128];
void RealAll( LPCOBJPRIV _lpCobj_Priv )
{
	BOOL			_AM, _Flag_ChangeAM, _LastAM, _Flag_SJZ = 0;
	LPCOBJPRIV		_lpCobj_HistoryAll, _lpCobjApart, _lpCobjPrivHistory;
	LPCOBJ			_lpCobj_History, _lpCobj_GHistory;
	char			_GlobalFile[128];
	LPCOBJAPART		_lpPrivApart;
	int				_i, _j;
	LPCOBJPRIVMV	_lpCobjPrivMV, _lpCobjGMV;
	char			_szBuffer[128];

	//Read the state of debug, determine OP/ENG state
	_lpPrivApart = COBJ_2_APART( _lpCobj_Priv->LPApart );
	READALLPARAMETER( _lpCobj_Priv, MDMCIRVD_FLAGDEBUG,  		
							BOOL, _lpCobj_Priv->Flag_Debug );
	if( _lpCobj_Priv->Flag_Debug == 1 )
	{
		strcpy( _GlobalFile, LocalDirectory );
#ifdef __ONSPEC
		strcat( _GlobalFile, "tfmcc.ini" );
#else
		strcat( _GlobalFile, "tfmcccore.ini" );
#endif
		GetPrivateProfileString( "MCC",
								 "RDebug",
								 "0",
								 _szBuffer,
								 sizeof(_szBuffer),
								 _GlobalFile );
		sscanf( _szBuffer, "%d", &(_lpCobj_Priv->Flag_RDebug) );
//Read the max condtion number of steady equation
		GetPrivateProfileString( "MCC",
								 "CalSteadyMax",
								 "0",
								 _szBuffer,
								 sizeof(_szBuffer),
								 _GlobalFile );
		sscanf( _szBuffer, "%d", &(_lpCobj_Priv->CalSteadyMax ) );
//Read the max possible breakdown times
		GetPrivateProfileString( "MCC",
								 "BadMax",
								 "0",
								 _szBuffer,
								 sizeof(_szBuffer),
								 _GlobalFile );
		sscanf( _szBuffer, "%d", &(_lpCobj_Priv->BadMax ) );
	}
	else
		_lpCobj_Priv->Flag_RDebug = 0;
//  There are some flags should be read no matter OP/ENG
	if( _lpCobj_Priv->Count == 1 )
		_lpCobj_Priv->Flag_RDebug = 1;
//Read global parameters from display
	READALLPARAMETER( _lpCobj_Priv, MDMCIRVD_FLAGAM,  		BOOL, _AM );
	READALLPARAMETER( _lpCobj_Priv, MDMCIRVD_FLAGRUN,  	BOOL, _lpCobj_Priv->Flag_Run );
	if( _lpCobj_Priv->Flag_RDebug )
	{
		READALLPARAMETER( _lpCobj_Priv, MDMCIRVD_FLAGFAULT,  	BOOL, _lpCobj_Priv->Flag_Fault );
		READALLPARAMETER( _lpCobj_Priv, MDMCIRVD_FLAGLP,  		BOOL, _lpCobj_Priv->Flag_LP );
//		READALLPARAMETER( _lpCobj_Priv, MDMCIRVD_FLAGHOPEAM,  	BOOL, _lpCobj_Priv->Flag_HopeAM );
//		READALLPARAMETER( _lpCobj_Priv, MDMCIRVD_FLAGALARM,  	BOOL, _lpCobj_Priv->Flag_Alarm );
		READALLPARAMETER( _lpCobj_Priv, MDMCIRVD_FLAGRESP,  	BOOL, _lpCobj_Priv->Flag_ReSP );
//		READALLPARAMETER( _lpCobj_Priv, MDMCIRVD_SRMODE,	 	BOOL, _lpCobj_Priv->Flag_SRMode );
		READALLPARAMETER( _lpCobj_Priv, MDMCIRVD_FLAGMCHANGE, 	BOOL, _lpCobj_Priv->Flag_MChange );
		READALLPARAMETER( _lpCobj_Priv, MDMCIRVD_FLAGLPCHANGE, BOOL, _lpCobj_Priv->Flag_LPChange );
		READALLPARAMETER( _lpCobj_Priv, MDMCIRVD_CCLASSIF,  	BOOL, _lpCobj_Priv->Flag_CControl );
		READALLPARAMETER( _lpCobj_Priv, MDMCIRVD_MCLASSIF,  	BOOL, _lpCobj_Priv->Flag_MControl );
		READALLPARAMETER( _lpCobj_Priv, MDMCIRVD_count,  		WORD, _lpCobj_Priv->Count );
		READALLPARAMETER( _lpCobj_Priv, MDMCIRVD_LPK,  		REAL, _lpCobj_Priv->LpK );
		READALLPARAMETER( _lpCobj_Priv, MDMCIRVD_alpha,  		REAL, _lpCobj_Priv->Alpha );
		READALLPARAMETER( _lpCobj_Priv, MDMCIRVD_BIGCOND,  	REAL, _lpCobj_Priv->Big_Cond );
		READALLPARAMETER( _lpCobj_Priv, MDMCIRVD_PBROKEN,  	REAL, _lpCobj_Priv->PBroken );
		READALLPARAMETER( _lpCobj_Priv, MDMCIRVD_QBROKEN,  	REAL, _lpCobj_Priv->QBroken );
		READALLPARAMETER( _lpCobj_Priv, MDMCIRVD_RBROKEN,  	REAL, _lpCobj_Priv->RBroken );
		READALLPARAMETER( _lpCobj_Priv, MDMCIRVD_SRERROR,  	REAL, _lpCobj_Priv->SRError );
		READALLPARAMETER( _lpCobj_Priv, MDMCIRVD_DSRERROR,  	REAL, _lpCobj_Priv->DSRError );
		READALLPARAMETER( _lpCobj_Priv, MDMCIRVD_MPARAMAX,		WORD, _lpCobj_Priv->MParaMax );
		READALLPARAMETER( _lpCobj_Priv, MDMCIRVD_FLAGWEIGHT,  	BOOL, _lpCobj_Priv->Flag_Weight );
		READALLPARAMETER( _lpCobj_Priv, MDMCIRVD_MAXDELAY,		WORD, _lpCobj_Priv->MaxDelay );
		READALLPARAMETER( _lpCobj_Priv, MDMCIRVD_FLAGHMODE,  	BOOL, _lpCobj_Priv->Flag_HMode );
//		READALLPARAMETER( _lpCobj_Priv, MDMCIRVD_NOAPART, 		WORD, _lpCobj_Priv->No_Apart );
		READALLPARAMETER( _lpCobj_Priv, MDMCIRVD_SAMPLETIME, 	REAL, _lpCobj_Priv->SampleTime );
		READALLPARAMETER( _lpCobj_Priv, MDMCIRVD_DFILTER,	 	REAL, _lpCobj_Priv->DFilter );
		READALLPARAMETER( _lpCobj_Priv, MDMCIRVD_INISTART,	 	BOOL, _lpCobj_Priv->IniStart );
		READALLPARAMETER( _lpCobj_Priv, MDMCIRVD_STEADYSTEP,	WORD, _lpCobj_Priv->SteadyStep );
		READALLPARAMETER( _lpCobj_Priv, MDMCIRVD_STEADY,		BOOL, _lpCobj_Priv->Flag_Steady );
		READALLPARAMETER( _lpCobj_Priv, MDMCIRVD_CORSTEADY,		REAL, _lpCobj_Priv->Cor_Steady );
	}
//  if the number of sub-system is less than 4, then we could get its AM state
//	from GlobalParameters
//  Determine the A/M state
	for( _i = 0; _i < _lpCobj_Priv->No_Apart; _i++ )
	{
		_lpCobjApart = APARTGET( _lpPrivApart, _i, 0 );
		_lpCobjApart->Flag_AM = _AM;
	}
	if( _lpCobj_Priv->No_Apart <= 3 )
	{
		_Flag_SJZ = 1;
		_i = _lpCobj_Priv->No_Apart;
		READALLPARAMETER( _lpCobj_Priv, MDMCIRVD_FLAGAM1,  		BOOL, _lpCobj_Priv->Flag_AM1 );
		READALLPARAMETER( _lpCobj_Priv, MDMCIRVD_FLAGAM2,  		BOOL, _lpCobj_Priv->Flag_AM2 );
		READALLPARAMETER( _lpCobj_Priv, MDMCIRVD_FLAGAM3,  		BOOL, _lpCobj_Priv->Flag_AM3 );
		_lpCobjApart = APARTGET( _lpPrivApart, 0, 0 );
		_lpCobjApart->Flag_AM = _lpCobj_Priv->Flag_AM1;
		if( _lpCobj_Priv->No_Apart >= 2 )
		{
			_lpCobjApart = APARTGET( _lpPrivApart, 1, 0 );
			_lpCobjApart->Flag_AM = _lpCobj_Priv->Flag_AM2;
		}
		if( _lpCobj_Priv->No_Apart >= 2 )
		{
			_lpCobjApart = APARTGET( _lpPrivApart, 2, 0 );
			_lpCobjApart->Flag_AM = _lpCobj_Priv->Flag_AM3;
		}
		_lpCobjApart = APARTGET( _lpPrivApart, 0, 0 );
		_lpCobjPrivMV = COBJ_2_PRIVMV( _lpCobjApart->LPINMV );
		if( ( !_lpCobj_Priv->Flag_AM1 ) )
		{
			_lpCobjApart->Flag_AM = 0;
			for( _j = 0; _j < _lpCobjApart->m; _j++ )
				PRIVMVGET( _lpCobjPrivMV, _j, 0 )->MApartAM = 0;

		}
		else
			for( _j = 0; _j < _lpCobjApart->m; _j++ )
				PRIVMVGET( _lpCobjPrivMV, _j, 0 )->MApartAM = 1;
		_i--;
		if( _i > 0 )
		{
			_lpCobjApart = APARTGET( _lpPrivApart, 1, 0 );
			_lpCobjPrivMV = COBJ_2_PRIVMV( _lpCobjApart->LPINMV );
			if( ( !_lpCobj_Priv->Flag_AM2 ) )
			{
				_lpCobjApart->Flag_AM = 0;
				for( _j = 0; _j < _lpCobjApart->m; _j++ )
					PRIVMVGET( _lpCobjPrivMV, _j, 0 )->MApartAM = 0;
			}
			else
				for( _j = 0; _j < _lpCobjApart->m; _j++ )
					PRIVMVGET( _lpCobjPrivMV, _j, 0 )->MApartAM = 1;
			_i--;
			if( _i > 0 )
			{
				_lpCobjApart = APARTGET( _lpPrivApart, 2, 0 );
				_lpCobjPrivMV = COBJ_2_PRIVMV( _lpCobjApart->LPINMV );
				if( ( !_lpCobj_Priv->Flag_AM3 ) )
				{
					_lpCobjApart->Flag_AM = 0;
					for( _j = 0; _j < _lpCobjApart->m; _j++ )
						PRIVMVGET( _lpCobjPrivMV, _j, 0 )->MApartAM = 0;
				}
				else
					for( _j = 0; _j < _lpCobjApart->m; _j++ )
						PRIVMVGET( _lpCobjPrivMV, _j, 0 )->MApartAM = 1;
			}
		}
	}
	// from the global parameters, decide the sub-global parameters
	for( _i = 0; _i < _lpCobj_Priv->No_Apart; _i++ )
	{
		_lpCobjApart = APARTGET( _lpPrivApart, _i, 0 );
		_lpCobjApart->P = _lpCobj_Priv->P;
		_lpCobjApart->M = _lpCobj_Priv->M;
		_lpCobjApart->Flag_Fault = _lpCobj_Priv->Flag_Fault;
		_lpCobjApart->Flag_LP = _lpCobj_Priv->Flag_LP;
//		_lpCobjApart->Flag_HopeAM = _lpCobj_Priv->Flag_HopeAM;
		_lpCobjApart->Flag_ReSP = _lpCobj_Priv->Flag_ReSP;
		_lpCobjApart->Flag_Run = _lpCobj_Priv->Flag_Run;
		_lpCobjApart->Flag_Alarm = _lpCobj_Priv->Flag_Alarm;
//		_lpCobjApart->Flag_SRMode = _lpCobj_Priv->Flag_SRMode;
		_lpCobjApart->Flag_MChange = _lpCobj_Priv->Flag_MChange;
		_lpCobjApart->Flag_LPChange = _lpCobj_Priv->Flag_LPChange;
		_lpCobjApart->Flag_MControl = _lpCobj_Priv->Flag_MControl;
		_lpCobjApart->Flag_CControl = _lpCobj_Priv->Flag_CControl;
		_lpCobjApart->Alpha = _lpCobj_Priv->Alpha;
		_lpCobjApart->LpK = _lpCobj_Priv->LpK;
		_lpCobjApart->Count = _lpCobj_Priv->Count;
		_lpCobjApart->Big_Cond = _lpCobj_Priv->Big_Cond;
		_lpCobjApart->PBroken = _lpCobj_Priv->PBroken;
		_lpCobjApart->QBroken = _lpCobj_Priv->QBroken;
		_lpCobjApart->RBroken = _lpCobj_Priv->RBroken;
		_lpCobjApart->SRError = _lpCobj_Priv->SRError;
		_lpCobjApart->DSRError = _lpCobj_Priv->DSRError;
		_lpCobjApart->MParaMax = _lpCobj_Priv->MParaMax;
		_lpCobjApart->Flag_Weight = _lpCobj_Priv->Flag_Weight;
		_lpCobjApart->Flag_HMode = _lpCobj_Priv->Flag_HMode;
		_lpCobjApart->CalSteadyMax = _lpCobj_Priv->CalSteadyMax;
		_lpCobjApart->Cor_Steady = _lpCobj_Priv->Cor_Steady;
		_lpCobjApart->SteadyStep = _lpCobj_Priv->SteadyStep;
		_lpCobjApart->Flag_Steady = _lpCobj_Priv->Flag_Steady;
//		_lpCobjApart->CalSteady = _lpCobj_Priv->CalSteady;
		_lpCobjApart->Flag_Alarm = _lpCobj_Priv->Flag_Alarm;
		_lpCobjApart->LPApartMV =  _lpCobj_Priv->LPApartMV;
		_lpCobjApart->LPApartCV =  _lpCobj_Priv->LPApartCV;
		_lpCobjApart->LPApart =    _lpCobj_Priv->LPApart;
		_lpCobj_History = _lpCobjApart->LPHistory;
		if( _lpCobj_History == NULL )
		{
			MessageBox( NULL, "ERROR", "No Cobj_history in r_all", MB_OK );
			LogError( "LPHistory", "tfr_all.c");
			_lpCobjApart->Flag_Run = 0;
			_lpCobj_Priv->Flag_Run = 0;
		//Modified by chen
			WRITEALLPARAMETER( _lpCobj_Priv, MDMCIRVD_FLAGALARM,		BOOL, 1 );
			WRITEALLPARAMETER( _lpCobj_Priv, MDMCIRVD_FLAGHOPEAM, 		BOOL, 0 );
			return ;
		}
		_lpCobj_HistoryAll = COBJ_2_PRIV( _lpCobj_History );
		_lpCobjApart->MakeParaCount = _lpCobj_HistoryAll->MakeParaCount;
		_lpCobjApart->RecountCount = _lpCobj_HistoryAll->RecountCount;
		
// Reset the parameters or not, this flag would be changed to be 0 soon if it is 1
		if( _lpCobjApart->Flag_ReSP == 1 )
		{
			if( _lpCobj_HistoryAll->Flag_ReSP == 0 )
				_lpCobjApart->Flag_ReSP = 2;
			if( _lpCobj_HistoryAll->Flag_ReSP == 1 )
				_lpCobjApart->Flag_ReSP = 1;
		}
// Get the Step-response matrix used by this sampling time
		CobjKeepCopy( _lpCobjApart->LPTempSR, _lpCobjApart->LPSR );
		if( _lpCobjApart->nd )
			CobjKeepCopy( _lpCobjApart->LPTempDSR, _lpCobjApart->LPDSR );
// There is something happened, manu state is expected, but ...
		if( !_lpCobj_HistoryAll->Flag_HopeAM )
		{
			if( _AM == 1 )
			{
				MessageBeep(-1);
				LogError( "Manu", "tfr_all.c, 229 " );
			}
		}
// If the sub-modu number is greater than 3
		if( !_Flag_SJZ )
		{
			_lpCobjPrivMV = COBJ_2_PRIVMV( _lpCobjApart->LPINMV );
			_LastAM = PRIVMVGET( _lpCobjPrivMV, 0, 0 )->MApartAM;
			_Flag_ChangeAM = 0;
			for( _j = 0; _j < _lpCobjApart->m; _j++ )
			{
				READMVPARAMETER( _lpCobjApart->LPMV, (WORD)_j, MV_MAPARTAM,
					BOOL, PRIVMVGET( _lpCobjPrivMV, _j, 0 )->MApartAM );
				if( PRIVMVGET( _lpCobjPrivMV, _j, 0 )->MApartAM != _LastAM )
				{
					_Flag_ChangeAM = 1;
					break;
				}
			}
			if( _Flag_ChangeAM )
			{
				if( _LastAM )
					PRIVMVGET( _lpCobjPrivMV, _j, 0 )->MApartAM = 0;
				else
					PRIVMVGET( _lpCobjPrivMV, _j, 0 )->MApartAM = 1;
					
			}
			if( _lpCobjApart->Flag_AM == 1 )
			{
				for( _j = 0; _j < _lpCobj_Priv->m; _j++ )			
					if( !PRIVMVGET( _lpCobjPrivMV, _j, 0 )->MApartAM )
					{
						_lpCobjApart->Flag_AM = 0;
						break;
					}
			}
		}
		if( _lpCobj_HistoryAll->Flag_AM > 1 )
		{
			if( _lpCobjApart->Flag_AM == 1 )
				_lpCobjApart->Flag_AM = 3;
			else
				_lpCobjApart->Flag_AM = 1;
		}
		else
		{
			if( _lpCobjApart->Flag_AM == 1 )
				_lpCobjApart->Flag_AM = 2;
			else
				_lpCobjApart->Flag_AM = 0;
		}
	}
//	WRITEALLPARAMETER( _lpCobj_Priv, MDMCIRVD_MAKEPARACOUNT, WORD, _lpCobj_Priv->MakeParaCount );
//	WRITEALLPARAMETER( _lpCobj_Priv, MDMCIRVD_RECOUNTCOUNT, WORD, _lpCobj_Priv->RecountCount );
	_lpCobj_GHistory = ( _lpCobj_Priv )->LPHistory;
	_lpCobjPrivHistory = COBJ_2_PRIV( _lpCobj_GHistory );
	_lpCobj_Priv->LPCount = _lpCobjPrivHistory->LPCount;
	_lpCobj_Priv->ErrorCode = _lpCobjPrivHistory->ErrorCode;
	if( _lpCobj_Priv->Flag_MChange == 1 || _lpCobj_Priv->Flag_LPChange )
		_lpCobj_Priv->ErrorCode = 0;
	WRITEALLPARAMETER( _lpCobj_Priv, MDMCIRVD_LPCOUNT, WORD, _lpCobj_Priv->LPCount );
// A/M is clear now
	if( _lpCobjPrivHistory->Flag_AM > 1 )
	{
		if( _AM == 1 )
			_lpCobj_Priv->Flag_AM = 3;
		else
			_lpCobj_Priv->Flag_AM = 1;
	}
	else
	{
		if( _AM == 1 )
			_lpCobj_Priv->Flag_AM = 2;
		else
			_lpCobj_Priv->Flag_AM = 0;
	}
	_lpCobjGMV = COBJ_2_PRIVMV( _lpCobj_Priv->LPINMV );
	for( _j = 0; _j < _lpCobj_Priv->m; _j++ )
		WRITEMVPARAMETER( _lpCobj_Priv->LPMV, (WORD)_j, MV_MAPARTAM, BOOL, PRIVMVGET( _lpCobjGMV, _j, 0 )->MApartAM ); 

	if( _lpCobj_Priv->Flag_AM == 0 )
	{
		_lpCobj_Priv->Flag_Alarm = 0;
		WRITEALLPARAMETER( _lpCobj_Priv, MDMCIRVD_FLAGALARM, BOOL, 0 );
	}
	for( _i = 0; _i < _lpCobj_Priv->No_Apart; _i++ )
	{
		_lpCobjApart = APARTGET( _lpPrivApart, _i, 0 );
		_lpCobjApart->Flag_Alarm = _lpCobj_Priv->Flag_Alarm;
	}
// when manu, no error exist
	if( _lpCobj_Priv->Flag_AM <= 1 )
	{
		_lpCobj_Priv->ErrorCode = 0;
	}
return;
}