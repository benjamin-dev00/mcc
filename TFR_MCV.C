#include "tfalgo.h"
#include "tfcmdmc.h"
#ifdef __ONSPEC
#include "Mtmfun.h"
#include "Mtdllfun.h"
#endif

void ReadMCV( LPCOBJPRIV lpCobj_All_, LPCOBJPRIVMV lpCobj_PrivMV_, LPCOBJPRIVCV lpCobj_PrivCV_,
				LPCOBJPRIVDV lpCobj_PrivDV_ )
{
	WORD			_i, _j, _k;
	LPCOBJAPART		_lpCobjApart;
    double          _TempData, _TempHScale, _TempLScale, _LastIni;
	BOOL			_LastSatuation;
	BOOL			_Flag_CVNotSteady, _Flag_MVNotSteady, _Hist_State;
	char			_szBuffer[128];
	double			_MDelt, _LastData;
	LPCOBJPRIVMV	_lpApart_MV;
	LPCOBJPRIVCV	_lpApart_CV;
	int				_Apart_i;
	WORD			_Real_i, _Real_j, _Real_k;
	LPMATRIX		_lpmMV, _lpmCV, _lpmDV;
	BOOL			_Auto, _FS;
	

//	READSCALE( lpCobj_All_, lpCobj_PrivMV_,	lpCobj_PrivCV_, lpCobj_PrivDV_ );
	_lpCobjApart = COBJ_2_APART( lpCobj_All_->LPApart );
	for( _j = 0; _j < lpCobj_All_->m; _j++ )
	{
		READMVPARAMETER( lpCobj_All_->LPMV, _j, MV_AM,
			BOOL, PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MAM );
//  Ini is not allow to be changed ordinarily
//		if( lpCobj_All_->Flag_RDebug || lpCobj_All_->Count == 1 )
			READMVPARAMETER( lpCobj_All_->LPMV, _j, MV_INI,
				REAL, PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MIni );
// start from initial value
		if( lpCobj_All_->Count == 1 && lpCobj_All_->IniStart == 1 )
		{
			PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MData = 
				PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MIni;
			PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MSample = 
				PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MIni;
			WRITEMVPARAMETER( lpCobj_All_->LPMV, _j, MV_VALUE, REAL, 
				(REAL)PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MData );
			WRITEMVPARAMETER( lpCobj_All_->LPMV, _j, MV_SAMPLE, REAL, 
				(REAL)PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MData );
		}
		if( lpCobj_All_->Count == 1 )
		{
			READMVPARAMETER( lpCobj_All_->LPMV, _j, MV_INI,
				REAL, PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MHistory );
		}
		PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MCT_history = PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MRControl_type;
		READMVPARAMETER( lpCobj_All_->LPMV, _j, MV_CONTROLMODE,
			BOOL, PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MCM );
		PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MControl_type = 
			PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MCM;
		if( PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MControl_type == 0 )
			PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MControl_type = 2;
		_Hist_State = PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MInvalidate;
			READMVPARAMETER( lpCobj_All_->LPMV, _j, MV_STATE,
			BOOL, PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MInvalidate );
		if( !PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MInvalidate && _Hist_State )
			PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MHope_AM = 1;
		if( PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MInvalidate == 1 )
		{
			PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MControl_type = 4;
			//continue;
		}
//		_LastSatuation = PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MSetSatuation;
//		READMVPARAMETER( lpCobj_All_->LPMV, _j, MV_MSETSATUATION,
//			BOOL, PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MSetSatuation );

		if( PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MControl_type == 1 ||
			PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MSetSatuation == 1 )
			READMVPARAMETER( lpCobj_All_->LPMV, _j, MV_SET,
				REAL, PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MIrv );
		_LastSatuation = PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MSetSatuation;
		READMVPARAMETER( lpCobj_All_->LPMV, _j, MV_MSETSATUATION,
			BOOL, PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MSetSatuation );
		if( PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MSetSatuation == 1 )
		{
			if( _LastSatuation == 0 )
			{
				PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MIrv = 
					PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MData;
				WRITEMVPARAMETER( lpCobj_All_->LPMV, _j, MV_SET, REAL, (REAL)PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MData );
			}
			else
			{
				if( !lpCobj_All_->Flag_Alarm )
				{
					PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MLastIRV = 
						PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MData;
					PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MData = 
						PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MIrv;
					WRITEMVPARAMETER( lpCobj_All_->LPMV, _j, MV_VALUE, REAL, (REAL)PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MData );
				}
			}
		}
		if( lpCobj_All_->Flag_RDebug )
		{
	//here write the last CM
			READMVPARAMETER( lpCobj_All_->LPMV, _j, MV_KEYMV,
				BOOL, PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MKey );
			_TempHScale = PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MHScale;
			_TempLScale = PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MLScale;
			READMVPARAMETER( lpCobj_All_->LPMV, _j, MV_HSCALE,
				REAL, PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MHScale );
			READMVPARAMETER( lpCobj_All_->LPMV, _j, MV_LSCALE,
				REAL, PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MLScale );
			if( PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MLScale >= 
					PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MHScale )
			{
				PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MHScale = _TempHScale;
				PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MLScale = _TempLScale;
				WRITEMVPARAMETER( lpCobj_All_->LPMV, _j, MV_HSCALE, REAL, (REAL)PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MHScale );
				WRITEMVPARAMETER( lpCobj_All_->LPMV, _j, MV_LSCALE, REAL, (REAL)PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MLScale );
			}
			READMVPARAMETER( lpCobj_All_->LPMV, _j, MV_ALARMMAX,
				REAL, PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MHAlarm );
			READMVPARAMETER( lpCobj_All_->LPMV, _j, MV_ALARMMIN,
				REAL, PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MLAlarm );
			READMVPARAMETER( lpCobj_All_->LPMV, _j, MV_DELMAX,
				REAL, PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MDelmax );
			READMVPARAMETER( lpCobj_All_->LPMV, _j, MV_DELMIN,
				REAL, PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MDelmin );
/*			if( PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MSetSatuation == 1 )
			{
				if( _LastSatuation == 0 )
				{
					PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MIrv = 
						PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MData;
					WRITEMVPARAMETER( lpCobj_All_->LPMV, _j, MV_SET, REAL, PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MData );
				}
				else
				{
					if( !lpCobj_All_->Flag_Alarm )
					{
						PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MLastIRV = 
							PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MData;
						PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MData = 
							PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MIrv;
						WRITEMVPARAMETER( lpCobj_All_->LPMV, _j, MV_VALUE, REAL, PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MData );
					}
				}
			}*/
		}
	}
	for( _j = 0; _j < lpCobj_All_->m; _j++ )
	{
		READMVPARAMETER( lpCobj_All_->LPMV, _j, MV_MAX,
			REAL, PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MMax );
		READMVPARAMETER( lpCobj_All_->LPMV, _j, MV_MIN,
			REAL, PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MMin );
		READMVPARAMETER( lpCobj_All_->LPMV, _j, MV_HIGH,
			REAL, PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MIrvHigh );
		READMVPARAMETER( lpCobj_All_->LPMV, _j, MV_LOW,
			REAL, PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MIrvLow );
		READMVPARAMETER( lpCobj_All_->LPMV, _j, MV_LPINDEX,
			REAL, PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MLPIndex );
	}
	for( _j = 0; _j < lpCobj_All_->m; _j++ )
	{
		if( lpCobj_All_->Flag_RDebug )
		{
			READMVPARAMETER( lpCobj_All_->LPMV, _j, MV_LPINI,
				REAL, PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MLPIni );
//			READMVPARAMETER( lpCobj_All_->LPMV, _j, MV_LPOUT,
//				REAL, PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MSteadOptim );
			READMVPARAMETER( lpCobj_All_->LPMV, _j, MV_R,
				REAL, PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MR );
			READMVPARAMETER( lpCobj_All_->LPMV, _j, MV_S,
				REAL, PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MP );
			READMVPARAMETER( lpCobj_All_->LPMV, _j, MV_SL,
				REAL, PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MPL );
			READMVPARAMETER( lpCobj_All_->LPMV, _j, MV_DELR,
				REAL, PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MDelR );
			READMVPARAMETER( lpCobj_All_->LPMV, _j, MV_DELS,
				REAL, PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MDelP );
			READMVPARAMETER( lpCobj_All_->LPMV, _j, MV_RBROKEN,
				REAL, PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MRMax );
//note by ttf on Dec. 5, 1998, test Max R, P, Q
/*			if(PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MRMax  >= lpCobj_All_->RBroken )
				PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MRMax  = lpCobj_All_->RBroken;
			if(PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MPMax  >= lpCobj_All_->PBroken )
				PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MPMax  = lpCobj_All_->PBroken;
*/			READMVPARAMETER( lpCobj_All_->LPMV, _j, MV_SBROKEN,
				REAL, PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MPMax );
			READMVPARAMETER( lpCobj_All_->LPMV, _j, MV_PRIORITY,
				WORD, PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MPriority );
			READMVPARAMETER( lpCobj_All_->LPMV, _j, MV_DIST,
				BOOL, PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MDist );
//			if( lpCobj_All_->Flag_Fault == 1 )
//			{
				READMVPARAMETER( lpCobj_All_->LPMV, _j, MV_FMAX, 		REAL, PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MFault_max );
				READMVPARAMETER( lpCobj_All_->LPMV, _j, MV_FMIN, 		REAL, PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MFault_min );
				READMVPARAMETER( lpCobj_All_->LPMV, _j, MV_FHIGH, 		REAL, PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MIrvFault_high );
				READMVPARAMETER( lpCobj_All_->LPMV, _j, MV_FLOW, 		REAL, PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MIrvFault_low );
				READMVPARAMETER( lpCobj_All_->LPMV, _j, MV_FXSET, 		REAL, PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MFault_xirv );
				READMVPARAMETER( lpCobj_All_->LPMV, _j, MV_FNSET, 		REAL, PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MFault_nirv );
				READMVPARAMETER( lpCobj_All_->LPMV, _j, MV_FHSET, 		REAL, PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MFault_hirv );
				READMVPARAMETER( lpCobj_All_->LPMV, _j, MV_FLSET, 		REAL, PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MFault_lirv );
//			}
			READMVPARAMETER( lpCobj_All_->LPMV, _j, MV_STEADYFIELD,
				REAL, PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MSteadyField );
		}
//?????
/*		if( APARTGET( _lpCobjApart, 0, 0 )->Flag_AM <= 1 || PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MAM == 0 ||
				lpCobj_All_->Count == 1 )
		{*/
//#ifndef __Simul
//		_MDelt = PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MDelt;
		_FS = 0;
#ifdef	__FIX
#ifdef __Simul
		_FS = 1;
#endif
#endif
		if( lpCobj_All_->Count >= 2 )
		{
			if( _FS != 1)
			{
				PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MSample = 
					PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MData;
				_LastData = PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MData;
				READMVPARAMETER( lpCobj_All_->LPMV, _j, MV_SAMPLE,
					REAL, PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MData );
				PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MSample = _LastData;
			}
			else
				PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MSample = 
					PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MData;
		}
//#endif
//			PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MIni = 
//				PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MData;
		//}
		if( PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MControl_type != 4 )
		{
			if( PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MData >
					PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MHAlarm ||
				PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MData <
					PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MLAlarm )
			{
				if( PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MBad == lpCobj_All_->BadMax )
				{
					char	_szError[128];
					PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MAlarm = 1;
					sprintf( _szError, "%s%d", "MV", _j+1 );
					LogError( _szError, "超报警限" );
				}
				else
				{	
					PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MBad++;
					PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MData = 
						PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MHistory;
					WRITEMVPARAMETER( lpCobj_All_->LPMV, _j, MV_VALUE, REAL, 
						(REAL)PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MData );
				}
			}
			else
			{
				PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MAlarm = 0;
				PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MBad = 0;
			}
		}
//		READMVPARAMETER( lpCobj_All_->LPMV, _j, MV_HOPEAM,
//			BOOL, PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MHope_AM );

		if(  !(PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MApartAM) )
			PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MHope_AM = 1;
		if( !PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MAM )
			PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MHope_AM = 0;
		if(  PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MApartAM )
			if( !PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MInvalidate  )
				if( !PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MHope_AM )
					if( PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MAM )
					{
						MessageBeep( -1 );
						LogError( "MV HopeAM", "HopeManu" );
					}

		//if( PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MSatuation == 0 )
			PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MDelt =
				PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MData -
					PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MHistory;
		_MDelt = PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MDelt;
		PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MHistory =
						PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MData;
/*		if( PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MData > 
				PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MFault_max )
			PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MData = 
				PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MFault_max;
		if( PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MData < 
				PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MFault_min )
			PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MData = 
				PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MFault_min;*/
		if( lpCobj_All_->Count >= 2 )
		{
			if( PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MControl_type == 1 )
			{
				if( PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MIrv > 
					PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MIrvHigh )
				{
					PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MIrv = 
						PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MIrvHigh;
					WRITEMVPARAMETER( lpCobj_All_->LPMV, _j, MV_SET, REAL, 
						(REAL)PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MIrvHigh );
				}
				if( PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MIrv < 
						PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MIrvLow )
				{
					PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MIrv = 
						PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MIrvLow;
					WRITEMVPARAMETER( lpCobj_All_->LPMV, _j, MV_SET, REAL, 
						(REAL)PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MIrvLow );
				}
			}
			if( PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MControl_type == 2 )
			{
				if( PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MIrv > 
					PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MIrvHigh )
				{
					PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MIrv = 
						PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MIrvHigh;
					WRITEMVPARAMETER( lpCobj_All_->LPMV, _j, MV_SET, REAL, 
						(REAL)PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MIrvHigh );
				}
				if( PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MIrv < 
					PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MIrvLow )
				{
					PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MIrv = 
						PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MIrvLow;
					WRITEMVPARAMETER( lpCobj_All_->LPMV, _j, MV_SET, REAL, 
						(REAL)PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MIrvLow );
				}
			}
		}
//		WRITEMVPARAMETER( lpCobj_All_->LPMV, _j, MV_SET,
//			REAL, (float)PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MIrv );
		//READMVPARAMETER( lpCobj_All_->LPMV, _j, MV_DELT,
		//	REAL, _LastDelt );
		WRITEMVPARAMETER( lpCobj_All_->LPMV, _j, MV_DELT,
			REAL, (REAL)PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MDelt );
		//PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MDelt = _LastDelt;
		READMVPARAMETER( lpCobj_All_->LPMV, _j, MV_DISPLAYH,
			REAL, PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MDisplayH );
		READMVPARAMETER( lpCobj_All_->LPMV, _j, MV_DISPLAYL,
			REAL, PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MDisplayL );
		if( PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MDisplayL >= 
				PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MMax )
			PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MDisplayL = 
				PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MMin;
		if( PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MDisplayH <= 
				PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MMin )
			PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MDisplayH = 
				PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MMax;
		if( PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MDisplayL >= 
				PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MDisplayH )
		{
			PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MDisplayL = 
				PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MMin;
			PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MDisplayH = 
				PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MMax;
		}
		WRITEMVPARAMETER( lpCobj_All_->LPMV, _j, MV_DISPLAYH,
			REAL, (float)PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MDisplayH );
		WRITEMVPARAMETER( lpCobj_All_->LPMV, _j, MV_DISPLAYL,
			REAL, (float)PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MDisplayL );
		if( PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MControl_type == 0 )
			PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MControl_type = 2;
	}
	for( _i = 0; _i < lpCobj_All_->p; _i++ )
	{
		PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CCT_history =
			PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CRControl_type;
	}
	for( _i = 0; _i < lpCobj_All_->p; _i++ )
	{
		READCVPARAMETER( lpCobj_All_->LPCV, _i, CV_CONTROLMODE,
			WORD, PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CCM );
		PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CControl_type =
			PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CCM;
		if( PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CControl_type == 0 )
			PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CControl_type = 2;
		READCVPARAMETER( lpCobj_All_->LPCV, _i, CV_STATE,
			BOOL, PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CInvalid );
		if( PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CInvalid == 1 )
		{
			PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CControl_type = 4;
			//continue;
		}
		if( PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CControl_type == 1 )
			READCVPARAMETER( lpCobj_All_->LPCV, _i, CV_SET, 			REAL, PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CSetpoint );
		READCVPARAMETER( lpCobj_All_->LPCV, _i, CV_MAX, 			REAL, PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CMax );
		READCVPARAMETER( lpCobj_All_->LPCV, _i, CV_MIN, 			REAL, PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CMin );
		READCVPARAMETER( lpCobj_All_->LPCV, _i, CV_HIGH, 			REAL, PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CHigh );
		READCVPARAMETER( lpCobj_All_->LPCV, _i, CV_LOW, 			REAL, PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CLow );
		READCVPARAMETER( lpCobj_All_->LPCV, _i, CV_LPINDEX, 		REAL, PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CLPIndex );
		READCVPARAMETER( lpCobj_All_->LPCV, _i, CV_PRIORITY, 			WORD, PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CPriority );
	}
	if( lpCobj_All_->Flag_RDebug )
	{
		for( _i = 0; _i < lpCobj_All_->p; _i++ )
		{
			_TempHScale = PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CHScale;
			_TempLScale = PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CLScale;
			READCVPARAMETER( lpCobj_All_->LPCV, _i, CV_HSCALE, 			REAL, PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CHScale );
			READCVPARAMETER( lpCobj_All_->LPCV, _i, CV_LSCALE, 			REAL, PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CLScale );
			if( PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CLScale >= 
					PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CHScale )
			{
				PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CHScale = _TempHScale;
				PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CLScale = _TempLScale;
				WRITECVPARAMETER( lpCobj_All_->LPCV, _i, CV_HSCALE, REAL, (REAL)PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CHScale );
				WRITECVPARAMETER( lpCobj_All_->LPCV, _i, CV_LSCALE, REAL, (REAL)PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CLScale );
			}
			READCVPARAMETER( lpCobj_All_->LPCV, _i, CV_ALARMMAX, 			REAL, PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CHAlarm );
			READCVPARAMETER( lpCobj_All_->LPCV, _i, CV_ALARMMIN, 			REAL, PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CLAlarm );
			READCVPARAMETER( lpCobj_All_->LPCV, _i, CV_LPINI, 		REAL, PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CLPIni );
//			READCVPARAMETER( lpCobj_All_->LPCV, _i, CV_LPOUT, 		REAL, PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CSteadOptim );
			READCVPARAMETER( lpCobj_All_->LPCV, _i, CV_Q,	 			REAL, PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CQ );
			READCVPARAMETER( lpCobj_All_->LPCV, _i, CV_QL,	 		REAL, PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CQL );
			READCVPARAMETER( lpCobj_All_->LPCV, _i, CV_DELQ,	 		REAL, PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CDelQ );
			READCVPARAMETER( lpCobj_All_->LPCV, _i, CV_QBROKEN,	 		REAL, PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CQMax );
//			if( PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CQMax >= lpCobj_All_->QBroken )
//				PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CQMax = lpCobj_All_->QBroken;
			READCVPARAMETER( lpCobj_All_->LPCV, _i, CV_Q,	 			REAL, PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CQ );
//			READCVPARAMETER( lpCobj_All_->LPCV, _i, CV_ALARM, 		BOOL, PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CAlarm );
			READCVPARAMETER( lpCobj_All_->LPCV, _i, CV_KEYCV, 		BOOL, PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CKey );
			PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CQDirtyCount = 0;
//			WRITECVPARAMETER( lpCobj_All_->LPCV, _i, CV_SET, 		REAL, 
//				PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CSetpoint );
//			if( lpCobj_All_->Flag_Fault == 1 )
//			{
				READCVPARAMETER( lpCobj_All_->LPCV, _i, CV_FMAX, REAL, PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CFault_max );
				READCVPARAMETER( lpCobj_All_->LPCV, _i, CV_FMIN, REAL, PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CFault_min );
				READCVPARAMETER( lpCobj_All_->LPCV, _i, CV_FHIGH, REAL, PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CFault_high );
				READCVPARAMETER( lpCobj_All_->LPCV, _i, CV_FLOW, REAL, PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CFault_low );
				READCVPARAMETER( lpCobj_All_->LPCV, _i, CV_FXSET, REAL, PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CFault_xsetpoint );
				READCVPARAMETER( lpCobj_All_->LPCV, _i, CV_FNSET, REAL, PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CFault_nsetpoint );
				READCVPARAMETER( lpCobj_All_->LPCV, _i, CV_FHSET, REAL, PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CFault_hsetpoint );
				READCVPARAMETER( lpCobj_All_->LPCV, _i, CV_FLSET, REAL, PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CFault_lsetpoint );
//			}
			READCVPARAMETER( lpCobj_All_->LPCV, _i, CV_STEADYFIELD, 		REAL, PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CSteadyField );
		}

	}
	//read DV now
	for( _i = 0; _i < lpCobj_All_->p; _i++ )
	{
		if( PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CSetpoint >=
				PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CMax )
			PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CSetpoint =
				PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CMax;
		if( PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CSetpoint <=
				PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CMin )
			PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CSetpoint =
				PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CMin;
		_TempData = PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CData;
		if( PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CControl_type != 4 )
		{
			READCVPARAMETER( lpCobj_All_->LPCV, _i, CV_VALUE,
				REAL, PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CData );
			if( PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CData >
					PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CHAlarm ||
				PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CData <
					PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CLAlarm )
			{
				if( PRIVCVGET(  lpCobj_PrivCV_, _i, 0 )->CBad == lpCobj_All_->BadMax )
				{
					PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CAlarm = 1;
				}
				else
				{
					PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CBad++;
					PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CData = _TempData;
				}
			}
			else
			{
				PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CAlarm = 0;
				PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CBad = 0;
			}
		}
	}
	if( lpCobj_All_->Flag_RDebug )
	{
		for( _k = 0; _k < lpCobj_All_->nd; _k++ )
		{
			READDVPARAMETER( lpCobj_All_->LPDV, _k, DV_INI,	 		REAL, PRIVDVGET( lpCobj_PrivDV_, _k, 0 )->DIni );
			READDVPARAMETER( lpCobj_All_->LPDV, _k, DV_HISTORY, 		REAL, PRIVDVGET( lpCobj_PrivDV_, _k, 0 )->DHistory );
			READDVPARAMETER( lpCobj_All_->LPDV, _k, DV_STEADYFIELD, 		REAL, PRIVDVGET( lpCobj_PrivDV_, _k, 0 )->DSteadyField );
		}
	}	
	for( _j = 0; _j < lpCobj_All_->m; _j++ )
	{
		if( ( PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MAlarm == 1 ) && 
				lpCobj_All_->Flag_AM > 1 && PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MControl_type!= 4  )
		{
			if( PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MKey == 1 )
			{
				char	_szError[128];
				lpCobj_All_->Flag_Alarm = 1;
				sprintf( _szError, "%s%d", "MV", _j+1 );
				LogError( _szError, "关键MV超报警限" );
			}
			else
			{
				char	_szError[128];
				sprintf( _szError, "%s%d", "MV", _j+1 );
				LogError( _szError, "非关键MV超报警限" );
				PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MInvalidate = 1;
				PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MControl_type = 4;
			}
		}
	}
	for( _i = 0; _i < lpCobj_All_->p; _i++ )
	{
		if( PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CAlarm == 1 && PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CControl_type != 4)
		{
			if( PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CKey == 1 )
			{
//				lpCobj_All_->Flag_Run = 0;
				if( lpCobj_All_->Flag_AM > 1 )
				{
					char	_szError[128];
					lpCobj_All_->Flag_Alarm = 1;
					PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CAlarm = 1;
					sprintf( _szError, "%s%d", "CV", _i+1 );
					LogError( _szError, "关键CV超报警限" );
				}
			}
			else
			{
				if( lpCobj_All_->Flag_AM > 1 )
				{
					char	_szError[128];
					sprintf( _szError, "%s%d", "CV", _i+1 );
					LogError( _szError, "非关键CV超报警限" );
					PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CInvalid = 1;
					PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CAlarm = 1;
					PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CControl_type = 4;
				}
			}
		}
		_LastIni = PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CIni;
		READCVPARAMETER( lpCobj_All_->LPCV, _i, CV_INI, 			REAL, PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CIni );
		if( lpCobj_All_->Count > 1 )
			PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CIniError =   
				( PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CIni - _LastIni )/
				( PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CHScale - 
					PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CLScale );
		else
			PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CIniError = 0.0;
		if( lpCobj_All_->Count == 1 && lpCobj_All_->IniStart == 1 )
		{
			PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CData = 
				PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CIni;
			WRITECVPARAMETER( lpCobj_All_->LPCV, _i, CV_VALUE, REAL, 
				(REAL)PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CIni );
		}
		READCVPARAMETER( lpCobj_All_->LPCV, _i, CV_DISPLAYH,
			REAL, PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CDisplayH );
		READCVPARAMETER( lpCobj_All_->LPCV, _i, CV_DISPLAYL,
			REAL, PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CDisplayL );
		if( PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CDisplayH <= 
				PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CMin )
			PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CDisplayH = 
				PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CMax;
		if( PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CDisplayL >= 
				PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CMax )
			PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CDisplayL = 
				PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CMin;
		if( PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CDisplayL >= 
				PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CDisplayH )
		{
			PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CDisplayL = 
				PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CMin;
			PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CDisplayH = 
				PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CMax;
		}
		WRITECVPARAMETER( lpCobj_All_->LPCV, _i, CV_DISPLAYH,
			REAL, (REAL)PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CDisplayH );
		WRITECVPARAMETER( lpCobj_All_->LPCV, _i, CV_DISPLAYL,
			REAL, (REAL)PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CDisplayL );
		if( PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CControl_type == 0 )
			PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CControl_type = 2;
	}
	for( _k = 0; _k < lpCobj_All_->nd; _k++ )
	{
		if( lpCobj_All_->Flag_RDebug )
		{
			_TempHScale = PRIVDVGET( lpCobj_PrivDV_, _k, 0 )->DHScale;
			_TempLScale = PRIVDVGET( lpCobj_PrivDV_, _k, 0 )->DLScale;
			READDVPARAMETER( lpCobj_All_->LPDV, _k, DV_HSCALE,
				REAL, PRIVDVGET( lpCobj_PrivDV_, _k, 0 )->DHScale );
			READDVPARAMETER( lpCobj_All_->LPDV, _k, DV_LSCALE,
				REAL, PRIVDVGET( lpCobj_PrivDV_, _k, 0 )->DLScale );
			if( PRIVDVGET( lpCobj_PrivDV_, _k, 0 )->DHScale <= 
					PRIVDVGET( lpCobj_PrivDV_, _k, 0 )->DLScale )
			{
				PRIVDVGET( lpCobj_PrivDV_, _k, 0 )	->DHScale = _TempHScale;
				PRIVDVGET( lpCobj_PrivDV_, _k, 0 )->DLScale = _TempLScale;
			}
		}
		_TempData = PRIVDVGET( lpCobj_PrivDV_, _k, 0 )->DData;
		READDVPARAMETER( lpCobj_All_->LPDV, _k, DV_VALUE, 		REAL, PRIVDVGET( lpCobj_PrivDV_, _k, 0 )->DData );
		if( PRIVDVGET( lpCobj_PrivDV_, _k, 0 )->DData > PRIVDVGET( lpCobj_PrivDV_, _k, 0 )->DHScale || 
			 PRIVDVGET( lpCobj_PrivDV_, _k, 0 )->DData < PRIVDVGET( lpCobj_PrivDV_, _k, 0 )->DLScale )
		{
			if( PRIVDVGET( lpCobj_PrivDV_, _k, 0 )->DBad == lpCobj_All_->BadMax )
			{
				PRIVDVGET( lpCobj_PrivDV_, _k, 0 )->DAlarm = 1;
			}
			else
			{
				PRIVDVGET( lpCobj_PrivDV_, _k, 0 )->DBad++;
				PRIVDVGET( lpCobj_PrivDV_, _k, 0 )->DData = _TempData;
				WRITEDVPARAMETER( lpCobj_All_->LPDV, _k, DV_VALUE, 		REAL, (REAL)PRIVDVGET( lpCobj_PrivDV_, _k, 0 )->DData );
			}
		}
//		PRIVDVGET( lpCobj_PrivDV_, _k, 0 )->DData = lpCobj_All_->DFilter * 
//			PRIVDVGET( lpCobj_PrivDV_, _k, 0 )->DHistory + ( 1 - lpCobj_All_->DFilter ) * 
//				PRIVDVGET( lpCobj_PrivDV_, _k, 0 )->DData;
		PRIVDVGET( lpCobj_PrivDV_, _k, 0 )->DDelt =
				PRIVDVGET( lpCobj_PrivDV_, _k, 0 )->DData - PRIVDVGET( lpCobj_PrivDV_, _k, 0 )->DHistory ;
	// here count is sure to be greater than or equal to 1
		PRIVDVGET( lpCobj_PrivDV_, _k, 0 )->DHistory = PRIVDVGET( lpCobj_PrivDV_, _k, 0 )->DData;
		WRITEDVPARAMETER( lpCobj_All_->LPDV, _k, DV_HISTORY, 		REAL, (float)PRIVDVGET( lpCobj_PrivDV_, _k, 0 )->DHistory );
		READDVPARAMETER( lpCobj_All_->LPDV, _k, DV_DISPLAYH, 		REAL, PRIVDVGET( lpCobj_PrivDV_, _k, 0 )->DDisplayH );
		READDVPARAMETER( lpCobj_All_->LPDV, _k, DV_DISPLAYL, 		REAL, PRIVDVGET( lpCobj_PrivDV_, _k, 0 )->DDisplayL );
		if( PRIVDVGET( lpCobj_PrivDV_, _k, 0 )->DDisplayH <= 
				PRIVDVGET( lpCobj_PrivDV_, _k, 0 )->DLScale )
			PRIVDVGET( lpCobj_PrivDV_, _k, 0 )->DDisplayH = 
				PRIVDVGET( lpCobj_PrivDV_, _k, 0 )->DHScale;
		if( PRIVDVGET( lpCobj_PrivDV_, _k, 0 )->DDisplayL >= 
				PRIVDVGET( lpCobj_PrivDV_, _k, 0 )->DHScale )
			PRIVDVGET( lpCobj_PrivDV_, _k, 0 )->DDisplayL = 
				PRIVDVGET( lpCobj_PrivDV_, _k, 0 )->DLScale;
		if( PRIVDVGET( lpCobj_PrivDV_, _k, 0 )->DDisplayL >= 
				PRIVDVGET( lpCobj_PrivDV_, _k, 0 )->DDisplayH )
		{
			PRIVDVGET( lpCobj_PrivDV_, _k, 0 )->DDisplayL = 
				PRIVDVGET( lpCobj_PrivDV_, _k, 0 )->DLScale;
			PRIVDVGET( lpCobj_PrivDV_, _k, 0 )->DDisplayH = 
				PRIVDVGET( lpCobj_PrivDV_, _k, 0 )->DHScale;
		}
		WRITEDVPARAMETER( lpCobj_All_->LPDV, _k, DV_DISPLAYH, 		REAL, (float)PRIVDVGET( lpCobj_PrivDV_, _k, 0 )->DDisplayH );
		WRITEDVPARAMETER( lpCobj_All_->LPDV, _k, DV_DISPLAYL, 		REAL, (float)PRIVDVGET( lpCobj_PrivDV_, _k, 0 )->DDisplayL );
	}
	for( _i = 0; _i < lpCobj_All_->p; _i++ )
	{
		if( PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CControl_type == 2 )
			if( PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CLow ==
					PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CHigh )
			{
				PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CControl_type = 1;
				PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CCM = 1;
				PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CSetpoint = 
					PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CHigh;
				WRITECVPARAMETER( lpCobj_All_->LPCV, _i, CV_CONTROLMODE,
					BOOL, PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CCM ); ////		191-3598055
				WRITECVPARAMETER( lpCobj_All_->LPCV, _i, CV_SET,
					REAL, (REAL)PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CSetpoint );
			}
			else
				if( PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CLow > PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CHigh )
				{
					PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CLow = 
						PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CLastLow;
					PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CHigh = 
						PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CLastHigh;
					WRITECVPARAMETER( lpCobj_All_->LPCV, _i, CV_LOW, REAL, 
						(REAL)PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CLastLow );
					WRITECVPARAMETER( lpCobj_All_->LPCV, _i, CV_HIGH, REAL, 
						(REAL)PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CLastHigh );
					MessageBeep(-1);
                    LogError( "CV", "CV Low > High" );
				}
		//illegal parameter of field-control, it would be set to setpoint control, and the setpoint is the original 
		//sample data
		if( PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CMin >= PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CMax )
		{
			PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CMin = 
				PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CLastLow;
			PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CMax = 
				PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CLastHigh;
			WRITECVPARAMETER( lpCobj_All_->LPCV, _i, CV_MIN, REAL, 
				(REAL)PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CLastLow );
			WRITECVPARAMETER( lpCobj_All_->LPCV, _i, CV_MAX, REAL, 
				(REAL)PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CLastHigh );
			MessageBeep( -1 );
            LogError( "CV ", "CV Min > Max ");
		}

		COMPARE( PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CMin, PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CMax );	
		COMPARE( PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CLow, PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CHigh );	
		COMPARE( PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CFault_min, PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CFault_max );	
		COMPARE( PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CFault_low, PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CFault_high );	
		COMPARE( PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CFault_nsetpoint, PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CFault_xsetpoint );	
		COMPARE( PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CFault_lsetpoint, PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CFault_hsetpoint );
		if( PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CControl_type == 2 )
			if( PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CFault_hsetpoint == 
				PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CFault_lsetpoint )
			{
				PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CControl_type = 1;
				PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CCM = 1;
				PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CSetpoint = PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CFault_lsetpoint;
				WRITECVPARAMETER( lpCobj_All_->LPCV, _i, CV_CONTROLMODE, BOOL, 1 );
				WRITECVPARAMETER( lpCobj_All_->LPCV, _i, CV_SET, REAL,  (REAL)PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CFault_lsetpoint );
			}

	}
	for( _j = 0; _j < lpCobj_All_->m; _j++ )
	{
		if( PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MMin >= 
				PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MMax )
		{
			PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MMin = 
				PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MLastLow;
			PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MMax = 
				PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MLastHigh;
			WRITEMVPARAMETER( lpCobj_All_->LPMV, _j, MV_MIN, REAL, 
				(REAL)PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MLastLow );
			WRITEMVPARAMETER( lpCobj_All_->LPMV, _j, MV_MAX, REAL, 
				(REAL)PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MLastHigh );
			MessageBeep( -1 );
            LogError( "MV", "MV Min > Max " );
                }
		if( PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MControl_type == 2 )
			if( PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MIrvLow > 
					PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MIrvHigh )
			{
				PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MIrvLow = 
					PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MLastLow;
				PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MIrvHigh = 
					PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MLastHigh;
				WRITEMVPARAMETER( lpCobj_All_->LPMV, _j, MV_LOW, REAL, 
					(REAL)PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MLastLow );
				WRITEMVPARAMETER( lpCobj_All_->LPMV, _j, MV_HIGH, REAL, 
					(REAL)PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MLastHigh );
				MessageBeep( -1 );
                LogError( "MV", "MV Low > High " );
			}
			else
				if( PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MIrvLow == 
					PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MIrvHigh )
				{
					PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MControl_type = 1;
					PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MCM = 1;
					PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MIrv = 
						PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MIrvHigh;
					WRITEMVPARAMETER( lpCobj_All_->LPMV, _j, MV_CONTROLMODE, 	BOOL,  PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MCM );
					WRITEMVPARAMETER( lpCobj_All_->LPMV, _j, MV_SET, 	REAL,  (REAL)PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MIrv );
				}
		COMPARE( PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MMin, PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MMax );	
		COMPARE( PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MIrvLow, PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MIrvHigh );	
		COMPARE( PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MFault_min, PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MFault_max );	
		COMPARE( PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MIrvFault_low, PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MIrvFault_high );	
		COMPARE( PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MFault_nirv, PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MFault_xirv );
		COMPARE( PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MFault_lirv, PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MFault_hirv );
		if( PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MControl_type == 2 )
			if( PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MIrvLow == 
				PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MIrvHigh )
			{
				PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MControl_type = 1;
				PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MCM = 1;
				PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MIrv = 
					PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MIrvLow;
				WRITEMVPARAMETER( lpCobj_All_->LPMV, _j, MV_CONTROLMODE, BOOL, 1 );
				WRITEMVPARAMETER( lpCobj_All_->LPMV, _j, MV_SET, 	REAL,  (REAL)PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MIrv );
			}
	}

// max and low are both useless now
#ifdef __NOHIGH
	for( _i = 0; _i < lpCobj_All_->p; _i++ )
	{
		PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CMax = PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CHigh;
		PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CMin = PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CLow;
		PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CFault_max = PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CFault_high;
		PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CFault_min = PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CFault_low;
		PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CFault_xsetpoint = PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CFault_hsetpoint;
		PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CFault_nsetpoint = PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CFault_lsetpoint;
		WRITECVPARAMETER( lpCobj_All_->LPCV, _i, CV_MAX, REAL, (REAL)PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CMax );
		WRITECVPARAMETER( lpCobj_All_->LPCV, _i, CV_MIN, REAL, (REAL)PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CMin );
		WRITECVPARAMETER( lpCobj_All_->LPCV, _i, CV_FMAX, REAL, (REAL)PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CFault_max );
		WRITECVPARAMETER( lpCobj_All_->LPCV, _i, CV_FMIN, REAL, (REAL)PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CFault_min );
		WRITECVPARAMETER( lpCobj_All_->LPCV, _i, CV_FXSET, REAL, (REAL)PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CFault_xsetpoint );
		WRITECVPARAMETER( lpCobj_All_->LPCV, _i, CV_FNSET, REAL, (REAL)PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CFault_nsetpoint );
	}
	for( _j = 0; _j < lpCobj_All_->m; _j++ )
	{
		PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MMax = PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MIrvHigh;
		PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MMin = PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MIrvLow;
		PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MFault_max = PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MIrvFault_high;
		PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MFault_min = PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MIrvFault_low;
		PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MFault_xirv = PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MFault_hirv;
		PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MFault_nirv = PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MFault_lirv;
		WRITEMVPARAMETER( lpCobj_All_->LPMV, _j, MV_MAX, REAL, (REAL)PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MMax );
		WRITEMVPARAMETER( lpCobj_All_->LPMV, _j, MV_MIN, REAL, (REAL)PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MMin );
		WRITEMVPARAMETER( lpCobj_All_->LPMV, _j, MV_FMAX, REAL, (REAL)PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MFault_max );
		WRITEMVPARAMETER( lpCobj_All_->LPMV, _j, MV_FMIN, REAL, (REAL)PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MFault_min );
		WRITEMVPARAMETER( lpCobj_All_->LPMV, _j, MV_FXSET, REAL, (REAL)PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MFault_xirv );
		WRITEMVPARAMETER( lpCobj_All_->LPMV, _j, MV_FNSET, REAL, (REAL)PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MFault_nirv );
	}
#endif
//  if it is not auto, all MV's and CV's are considered to be bad
	for( _Apart_i = 0; _Apart_i < lpCobj_All_->No_Apart; _Apart_i++ )
	{
		if( APARTGET( _lpCobjApart, _Apart_i, 0 )->Flag_AM <= 1 )
		{
			_lpApart_MV = COBJ_2_PRIVMV( APARTGET( _lpCobjApart, _Apart_i, 0 )->LPINMV );
			_lpApart_CV = COBJ_2_PRIVCV( APARTGET( _lpCobjApart, _Apart_i, 0 )->LPINCV );
			for( _j = 0; _j < APARTGET( _lpCobjApart, _Apart_i, 0 )->m; _j++ )
				PRIVMVGET( _lpApart_MV, _j, 0 )->MControl_type = 4;
			for( _j = 0; _j < APARTGET( _lpCobjApart, _Apart_i, 0 )->p; _j++ )
				PRIVCVGET( _lpApart_CV, _j, 0 )->CControl_type = 4;
		}
	}
	if( lpCobj_All_->Count >= 2 )
	{
		for( _i = 0; _i < lpCobj_All_->p; _i++ )
		{ 
			if( PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CSetpoint > PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CHAlarm ||
					PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CSetpoint < PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CLAlarm  )
			{
				PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CSetpoint = PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CLastSet;
				WRITECVPARAMETER( lpCobj_All_->LPCV, _i, CV_SET, REAL, (REAL)PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CSetpoint );
			}
			if( PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CHigh > PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CHAlarm ||
					PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CHigh < PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CLAlarm  )
			{
				PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CHigh = PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CLastHigh;
				WRITECVPARAMETER( lpCobj_All_->LPCV, _i, CV_HIGH, REAL, (REAL)PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CLastHigh );
			}
			if( PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CLow > PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CHAlarm ||
					PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CLow < PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CLAlarm  )
			{
				PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CLow = PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CLastLow;
				WRITECVPARAMETER( lpCobj_All_->LPCV, _i, CV_LOW, REAL, (REAL)PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CLastLow );
			}
		}
		for( _j = 0; _j < lpCobj_All_->m; _j++ )
		{
			if( PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MIrv > PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MHAlarm ||
					PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MIrv < PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MLAlarm )
			{
				PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MIrv = PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MLastIRV;
				WRITEMVPARAMETER( lpCobj_All_->LPMV, _j, MV_SET, REAL, (REAL)PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MIrv );
			}
			if( PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MIrvHigh > PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MHAlarm ||
					PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MIrvHigh < PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MLAlarm )
			{
				PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MIrvHigh = PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MLastHigh;
				WRITEMVPARAMETER( lpCobj_All_->LPMV, _j, MV_HIGH, REAL, (REAL)PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MLastHigh );
			}
			if( PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MIrvLow > PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MHAlarm ||
					PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MIrvHigh < PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MLAlarm )
			{
				PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MIrvLow = PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MLastLow;
				WRITEMVPARAMETER( lpCobj_All_->LPMV, _j, MV_LOW, REAL, (REAL)PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MLastLow );
			}
		}
	}
	_Auto = 0;
	for( _Apart_i = 0; _Apart_i < lpCobj_All_->No_Apart; _Apart_i++ )
	{
		_Flag_CVNotSteady = 0;
		_Flag_MVNotSteady = 0;
		if( APARTGET( _lpCobjApart, _Apart_i, 0 )->Flag_AM == 2 )
		{
			_lpApart_MV = COBJ_2_PRIVMV( APARTGET( _lpCobjApart, _Apart_i, 0 )->LPINMV );
			_lpApart_CV = COBJ_2_PRIVCV( APARTGET( _lpCobjApart, _Apart_i, 0 )->LPINCV );
			_lpmMV = COBJ_2_MATRIX( lpCobj_All_->LPApartMV );
			_lpmCV = COBJ_2_MATRIX( lpCobj_All_->LPApartCV );
			for( _i = 0; ( _i < APARTGET( _lpCobjApart, _Apart_i, 0 )->p ) && ( _Flag_CVNotSteady == 0 ); _i++ )
			{
				if( !( PRIVCVGET( _lpApart_CV, _i, 0 )->CInvalid ) )
				{
					if( ( PRIVCVGET( _lpApart_CV, _i, 0 )->CData < 
						PRIVCVGET( _lpApart_CV, _i, 0 )->CFault_lsetpoint ) || 
						( PRIVCVGET( _lpApart_CV, _i, 0 )->CData > 
						PRIVCVGET( _lpApart_CV, _i, 0 )->CFault_hsetpoint ) )
					{
						_Real_i = (int)MGET( _lpmCV, _Apart_i, _i ) - 1;
						if( PRIVCVGET( _lpApart_CV, _i, 0 )->CData < 
							PRIVCVGET( _lpApart_CV, _i, 0 )->CFault_lsetpoint )
						{
							PRIVCVGET( _lpApart_CV, _i, 0 )->CLow = ( PRIVCVGET( _lpApart_CV, _i, 0 )->CData - PRIVCVGET( _lpApart_CV, _i, 0 )->CHigh * 
										PRIVCVGET( _lpApart_CV, _i, 0 )->CLowSet ) / ( 1 - PRIVCVGET( _lpApart_CV, _i, 0 )->CLowSet );
							PRIVCVGET( _lpApart_CV, _i, 0 )->CMin = PRIVCVGET( _lpApart_CV, _i, 0 )->CLow;
							WRITECVPARAMETER( lpCobj_All_->LPCV, _Real_i, CV_LOW, REAL, (REAL)PRIVCVGET( _lpApart_CV, _i, 0 )->CLow );
							WRITECVPARAMETER( lpCobj_All_->LPCV, _Real_i, CV_MIN, REAL, (REAL)PRIVCVGET( _lpApart_CV, _i, 0 )->CLow );
						}
						else
						{
							PRIVCVGET( _lpApart_CV, _i, 0 )->CHigh = ( PRIVCVGET( _lpApart_CV, _i, 0 )->CData - PRIVCVGET( _lpApart_CV, _i, 0 )->CLow * 
										PRIVCVGET( _lpApart_CV, _i, 0 )->CHighSet ) / ( 1 - PRIVCVGET( _lpApart_CV, _i, 0 )->CHighSet );
							PRIVCVGET( _lpApart_CV, _i, 0 )->CMax = PRIVCVGET( _lpApart_CV, _i, 0 )->CHigh;
							WRITECVPARAMETER( lpCobj_All_->LPCV, _Real_i, CV_HIGH, REAL, (REAL)PRIVCVGET( _lpApart_CV, _i, 0 )->CHigh );
							WRITECVPARAMETER( lpCobj_All_->LPCV, _Real_i, CV_MAX, REAL, (REAL)PRIVCVGET( _lpApart_CV, _i, 0 )->CHigh );
						}
			//			_Flag_CVNotSteady = 1;
					}
				}
			}
			for( _j = 0; ( _j < APARTGET( _lpCobjApart, _Apart_i, 0 )->m ) &&
							_Flag_MVNotSteady == 0; _j++ )
			{
				if( !( PRIVMVGET( _lpApart_MV, _j, 0 )->MInvalidate ) )
				{
					_Real_j = (int)MGET( _lpmMV, _Apart_i, _j ) - 1;
					if( ( PRIVMVGET( _lpApart_MV, _j, 0 )->MData < 
							PRIVMVGET( _lpApart_MV, _j, 0 )->MLAlarm )|| 
							PRIVMVGET( _lpApart_MV, _j, 0 )->MData >
								PRIVMVGET( _lpApart_MV, _j, 0 )->MHAlarm )
					{
						_Flag_MVNotSteady = 1;
						continue;
					}
					if( ( PRIVMVGET( _lpApart_MV, _j, 0 )->MData < 
							PRIVMVGET( _lpApart_MV, _j, 0 )->MIrvLow )|| 
							PRIVMVGET( _lpApart_MV, _j, 0 )->MData >
								PRIVMVGET( _lpApart_MV, _j, 0 )->MIrvHigh )
					{
						if( PRIVMVGET( _lpApart_MV, _j, 0 )->MData < 
							PRIVMVGET( _lpApart_MV, _j, 0 )->MIrvLow )
						{
							PRIVMVGET( _lpApart_MV, _j, 0 )->MIrvLow = 
								PRIVMVGET( _lpApart_MV, _j, 0 )->MData + 
									PRIVMVGET( _lpApart_MV, _j, 0 )->MDelmin;
							PRIVMVGET( _lpApart_MV, _j, 0 )->MMin = 
								PRIVMVGET( _lpApart_MV, _j, 0 )->MIrvLow;
							WRITEMVPARAMETER( lpCobj_All_->LPMV, _Real_j, MV_MIN, REAL, 
								(REAL)PRIVMVGET( _lpApart_MV, _j, 0 )->MMin );
							WRITEMVPARAMETER( lpCobj_All_->LPMV, _Real_j, MV_LOW, REAL, 
								(REAL)PRIVMVGET( _lpApart_MV, _j, 0 )->MMin );
						}
						else
						{
							PRIVMVGET( _lpApart_MV, _j, 0 )->MIrvHigh = 
							PRIVMVGET( _lpApart_MV, _j, 0 )->MData + 
								PRIVMVGET( _lpApart_MV, _j, 0 )->MDelmax;
							PRIVMVGET( _lpApart_MV, _j, 0 )->MMax = 
								PRIVMVGET( _lpApart_MV, _j, 0 )->MIrvHigh;
							WRITEMVPARAMETER( lpCobj_All_->LPMV, _Real_j, MV_MAX, REAL, 
								(REAL)PRIVMVGET( _lpApart_MV, _j, 0 )->MMax );
							WRITEMVPARAMETER( lpCobj_All_->LPMV, _Real_j, MV_HIGH, REAL, 
								(REAL)PRIVMVGET( _lpApart_MV, _j, 0 )->MMax );
						}
					}
				}
			}
			if( _Flag_CVNotSteady == 1 || _Flag_MVNotSteady == 1 )
			{
				if( _Flag_CVNotSteady )
				{
					if( lpCobj_All_->No_Apart <= 3 )
					{
						if( _Apart_i == 0 )
						{
							sprintf( _szBuffer, "%s%d", "反再CV", _i );
						}
						if( _Apart_i == 1 )
						{
							sprintf( _szBuffer, "%s%d", "分馏CV", _i );
						}
						if( _Apart_i == 2 )
						{
							sprintf( _szBuffer, "%s%d", "吸收稳定CV", _i );
						}
					}
					else
						sprintf( _szBuffer, "%s%d%s%d", "Model", _Apart_i, "CV", _i ); 
				}
				else
				{
					if( lpCobj_All_->No_Apart <= 3 )
					{
						if( _Apart_i == 0 )
						{
							sprintf( _szBuffer, "%s%d", "反再MV范围超过报警限", _j );
						}
						if( _Apart_i == 1 )
						{
							sprintf( _szBuffer, "%s%d", "分馏MV范围超过报警限", _j );
						}
						if( _Apart_i == 2 )
						{
							sprintf( _szBuffer, "%s%d", "吸收稳定MV范围超过报警限", _j );
						}
					}
					else
						sprintf( _szBuffer, "%s%d%s%d", "Model", _Apart_i, "MV", _j ); 
				}
				MessageBox( NULL, "抱歉, 区域太窄, 请暂时放宽区域范围", _szBuffer, MB_SYSTEMMODAL );
				APARTGET( _lpCobjApart, _Apart_i, 0 )->Flag_AM = 0;
				if( _Apart_i == 0 )
				{
					lpCobj_All_->Flag_AM1 = 0;
					WRITEALLPARAMETER( lpCobj_All_, MDMCIRVD_FLAGAM1, BOOL, 0 );
				}
				if( _Apart_i == 1 )
				{
					lpCobj_All_->Flag_AM2 = 0;
					WRITEALLPARAMETER( lpCobj_All_, MDMCIRVD_FLAGAM2, BOOL, 0 );
				}
				if( _Apart_i == 2 )
				{
					lpCobj_All_->Flag_AM3 = 0;
					WRITEALLPARAMETER( lpCobj_All_, MDMCIRVD_FLAGAM3, BOOL, 0 );
				}
			}
		}

// manu---->auto
		if( APARTGET( _lpCobjApart, _Apart_i, 0 )->Flag_AM == 2 )
		{
			_lpmMV = COBJ_2_MATRIX( lpCobj_All_->LPApartMV );
			_lpmCV = COBJ_2_MATRIX( lpCobj_All_->LPApartCV );
			_lpmDV = COBJ_2_MATRIX( lpCobj_All_->LPApartDV );
			for( _j = 0; _j < APARTGET( _lpCobjApart, _Apart_i, 0 )->m; _j++ )
			{
				_Real_j = (int)MGET( _lpmMV, _Apart_i, _j ) - 1;
	//			_LastIni = PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MIni;
	//			READEMVPARAMETER( lpCobj_All_->LPMV, _j, MV_INI, REAL, 
	//				PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MIni );
				if( PRIVMVGET( lpCobj_PrivMV_, _Real_j, 0 )->MControl_type == 1 )
					PRIVMVGET( lpCobj_PrivMV_, _Real_j, 0 )->MIrv = 
						PRIVMVGET( lpCobj_PrivMV_, _Real_j, 0 )->MData;
				PRIVMVGET( lpCobj_PrivMV_, _Real_j, 0 )->MIni = 
					PRIVMVGET( lpCobj_PrivMV_, _Real_j, 0 )->MData;
                                PRIVMVGET( lpCobj_PrivMV_, _Real_j, 0 )->MIrv = 
					PRIVMVGET( lpCobj_PrivMV_, _Real_j, 0 )->MData;
				PRIVMVGET( lpCobj_PrivMV_, _Real_j, 0 )->MApartAM = 1;
				WRITEMVPARAMETER( lpCobj_All_->LPMV, _Real_j, MV_MAPARTAM, BOOL, 1 );
				WRITEMVPARAMETER( lpCobj_All_->LPMV, _Real_j, MV_INI, REAL, 
					(REAL)PRIVMVGET( lpCobj_PrivMV_, _Real_j, 0 )->MIni );
				WRITEMVPARAMETER( lpCobj_All_->LPMV, _Real_j, MV_SET, REAL, 
					(REAL)PRIVMVGET( lpCobj_PrivMV_, _Real_j, 0 )->MIrv );
				WRITEMVPARAMETER( lpCobj_All_->LPMV, _Real_j, MV_LOW, REAL, 
					(REAL)PRIVMVGET( _lpApart_MV, _j, 0 )->MIrvLow );
				WRITEMVPARAMETER( lpCobj_All_->LPMV, _Real_j, MV_HIGH, REAL, 
					(REAL)PRIVMVGET( _lpApart_MV, _j, 0 )->MIrvHigh );
				WRITEMVPARAMETER( lpCobj_All_->LPMV, _Real_j, MV_MAX, REAL, 
					(REAL)PRIVMVGET( _lpApart_MV, _j, 0 )->MMax );
				WRITEMVPARAMETER( lpCobj_All_->LPMV, _Real_j, MV_MIN, REAL, 
					(REAL)PRIVMVGET( _lpApart_MV, _j, 0 )->MMin );
				if( PRIVMVGET( _lpApart_MV, _j, 0 )->MAM )
				{
					PRIVMVGET( _lpApart_MV, _j, 0 )->MHope_AM = 1;
					WRITEMVPARAMETER( lpCobj_All_->LPMV, _Real_j, MV_HOPEAM, BOOL, 1);
				}
			}
			for( _i = 0; _i < APARTGET( _lpCobjApart, _Apart_i, 0 )->p; _i++ )
			{
				_Real_i = (int)MGET( _lpmCV, _Apart_i, _i ) - 1;
				PRIVCVGET( lpCobj_PrivCV_, _Real_i, 0 )->CIniError +=   
					( PRIVCVGET( lpCobj_PrivCV_, _Real_i, 0 )->CData - 
						PRIVCVGET( lpCobj_PrivCV_, _Real_i, 0 )->CIni )/
					( PRIVCVGET( lpCobj_PrivCV_, _Real_i, 0 )->CHScale - 
						PRIVCVGET( lpCobj_PrivCV_, _Real_i, 0 )->CLScale );
				if(	PRIVCVGET( lpCobj_PrivCV_, _Real_i, 0 )->CControl_type == 1 )
					PRIVCVGET( lpCobj_PrivCV_, _Real_i, 0 )->CSetpoint = 
						PRIVCVGET( lpCobj_PrivCV_, _Real_i, 0 )->CData;
				PRIVCVGET( lpCobj_PrivCV_, _Real_i, 0 )->CIni = 
					PRIVCVGET( lpCobj_PrivCV_, _Real_i, 0 )->CData;
				WRITECVPARAMETER( lpCobj_All_->LPCV, _Real_i, CV_INI, REAL, 
					(REAL)PRIVCVGET( lpCobj_PrivCV_, _Real_i, 0 )->CIni );
				WRITECVPARAMETER( lpCobj_All_->LPCV, _Real_i, CV_SET, REAL, 
					(REAL)PRIVCVGET( lpCobj_PrivCV_, _Real_i, 0 )->CIni );
			}
			for( _k = 0; _k < APARTGET( _lpCobjApart, _Apart_i, 0 )->nd; _k++ )
			{
				_Real_k = (int)MGET( _lpmCV, _Apart_i, _k ) - 1;
				PRIVDVGET( lpCobj_PrivDV_, _Real_k, 0 )->DIni = 
					PRIVDVGET( lpCobj_PrivDV_, _Real_k, 0 )->DData;
			}
		}
		if( APARTGET( _lpCobjApart, _Apart_i, 0 )->Flag_AM >= 2 )
		{
			_Auto = 1;
		}
		if( APARTGET( _lpCobjApart, _Apart_i, 0 )->Flag_AM < 2 )
		{
			_lpmMV = COBJ_2_MATRIX( lpCobj_All_->LPApartMV );
			_lpmCV = COBJ_2_MATRIX( lpCobj_All_->LPApartCV );
			for( _j = 0; _j < APARTGET( _lpCobjApart, _Apart_i, 0 )->m; _j++ )
			{
				_Real_j = (int)MGET( _lpmMV, _Apart_i, _j ) - 1;
				PRIVMVGET( lpCobj_PrivMV_, _Real_j, 0 )->MControl_type = 4;
			}
			for( _i = 0; _i < APARTGET( _lpCobjApart, _Apart_i, 0 )->p; _i++ )
			{
				_Real_i = (int)MGET( _lpmCV, _Apart_i, _i ) - 1;
				PRIVCVGET( lpCobj_PrivCV_, _Real_i, 0 )->CControl_type = 4;  
			}
		}
	}
	if( _Auto == 1 )
	{
		WRITEALLPARAMETER( lpCobj_All_, MDMCIRVD_FLAGAM, BOOL, 1 );
		lpCobj_All_->Flag_AM = 3;
	}
	else
	{
		WRITEALLPARAMETER( lpCobj_All_, MDMCIRVD_FLAGAM, BOOL, 0 );
		lpCobj_All_->Flag_AM = 0;
	}

	return ;
}
