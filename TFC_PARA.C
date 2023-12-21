#include "tfalgo.h"
#include "tfcmdmc.h"
#ifdef __ONSPEC
#include "Mtmfun.h"
#include "Mtdllfun.h"
#endif
//#define	   	_Fault_limit	0.05
//#define	   	_Fault_set	0.1

extern char LocalDirectory[128];
void CorrectParam( LPCOBJPRIV lpCobj_All_, LPCOBJPRIVMV lpCobj_PrivMV_,
						LPCOBJPRIVCV lpCobj_PrivCV_ )
{
	int 	_i, _j, _flag_LP, _Real_i, _Real_j;
	char	_GlobalFile[128], _szBuffer[128], _FileName[128];
	float	_Fault_limit, _Fault_set;
// note: If MV is bad..!!!!!!!

	strcpy( _GlobalFile, LocalDirectory );
#ifdef __ONSPEC
	strcat( _GlobalFile, "tfmcc.ini" );
#else
	strcat( _GlobalFile, "tfmcccore.ini" );
#endif
	strcpy( _FileName, LocalDirectory );
	strcat( _FileName, "tfoutall.ini" );
	GetPrivateProfileString( "MCC",
							 "Fault_limit",
							 "0",
							 _szBuffer,
							 sizeof(_szBuffer),
							 _GlobalFile );
	sscanf( _szBuffer, "%f", &_Fault_limit );
	GetPrivateProfileString( "MCC",
							 "Fault_set",
							 "0",
							 _szBuffer,
							 sizeof(_szBuffer),
							 _GlobalFile );
	sscanf( _szBuffer, "%f", &_Fault_set );
	/*if( lpCobj_All_->Count == 4 )
	{
		sprintf( _szBuffer, "%f", _Fault_set );
		WritePrivateProfileString( "MCC",
								   "Fault_set",
								   _szBuffer,
								   _FileName );
		sprintf( _szBuffer, "%f", _Fault_limit );
		WritePrivateProfileString( "MCC",
								   "Fault_limit",
								   _szBuffer,
								   _FileName );
	}*/
	if( lpCobj_All_->Flag_Fault == 0 )
	{
		for( _i = 0; _i < lpCobj_All_->p; _i++ )
		{
			PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CFault_max = PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CMax -
				( PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CMax - PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CMin ) * PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CHighFault;
			PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CFault_min = PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CMin +
				( PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CMax - PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CMin ) * PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CLowFault;
			PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CFault_xsetpoint = PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CMax -
				( PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CMax - PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CMin ) * PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CHighSet;
			PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CFault_nsetpoint = PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CMin +
				( PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CMax - PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CMin ) * PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CLowSet;
			PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CFault_high = PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CHigh -
				( PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CHigh - PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CLow ) * PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CHighFault;
			PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CFault_low = PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CLow +
				( PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CHigh - PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CLow ) * PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CLowFault;
			PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CFault_hsetpoint = PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CHigh -
				( PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CHigh - PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CLow ) * PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CHighSet;
			PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CFault_lsetpoint = PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CLow +
				( PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CHigh - PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CLow ) * PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CLowSet;
		}
		for( _j = 0; _j < lpCobj_All_->m; _j++ )
		{
			PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MFault_max = PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MMax -
				( PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MMax - PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MMin ) *_Fault_limit;
			PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MFault_min = PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MMin +
				( PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MMax - PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MMin ) * _Fault_limit;
			PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MIrvFault_high = PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MIrvHigh -
				( PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MIrvHigh - PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MIrvLow ) *_Fault_limit;
			PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MIrvFault_low = PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MIrvLow +
				( PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MIrvHigh - PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MIrvLow ) * _Fault_limit;
			PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MFault_hirv = PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MIrvHigh -
				( PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MIrvHigh - PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MIrvLow ) *_Fault_set;
			PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MFault_lirv = PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MIrvLow +
				( PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MIrvHigh - PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MIrvLow ) *_Fault_set;
			PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MFault_xirv = PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MMax -
				( PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MMax - PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MMin ) *_Fault_set;
			PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MFault_nirv = PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MMin +
				( PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MMax - PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MMin ) *_Fault_set;
		}
	}
	for( _j = 0; _j < lpCobj_All_->m; _j++ )
	{
		if( PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MInvalidate == 1 )
		{
			PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MControl_type = 4;
			PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MHope_AM = 0;
		}
		else
		{
			if( PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MAlarm == 1 )
			{
				if( PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MKey == 1 )
				{
					PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MHope_AM = 0;
					PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MControl_type = 5;
				}
			}
/*			else
				if( PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MHope_AM == 1 )
					PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MControl_type = 7;
  */	}
		if( PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MControl_type == 3 )
		{
//			MessageBox( NULL, "ERROR", "MVControl_type", MB_OK );
			lpCobj_All_->Flag_Run = 0;
			lpCobj_All_->Flag_Alarm = 1;
			LogError( "Correct Param", "tfc_para.c, line 117" );
		}
		if( PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MControl_type < 4 )
		{
			if( PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MDist == 1 )
				PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MControl_type = 7;
			if( PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MSetSatuation == 1 )
				PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MControl_type = 5;
			if( PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MAM == 0 )
				PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MControl_type = 5;
		}
	}

	for( _i = 0; _i < lpCobj_All_->p; _i++ )
		PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CDirty = 0;

	for( _j = 0; _j < lpCobj_All_->m; _j++ )
	{
		PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MRDirty = 0;
		PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MPDirty = 0;
	}

	for( _i = 0; _i < lpCobj_All_->p; _i++ )
	{
		_Real_i = (int)MGET( ( COBJ_2_MATRIX( lpCobj_All_->LPApartCV ) ), lpCobj_All_->Apart_i, _i ) - 1;
		if( PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CInvalid == 1 )
			PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CControl_type = 4;
		if( PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CHigh > PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CMax || 
				PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CLow < PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CMin )
		{
			PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CHigh = PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CMax;
			PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CFault_high = PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CFault_max;
			PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CFault_hsetpoint = PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CFault_xsetpoint;
			WRITECVPARAMETER( lpCobj_All_->LPCV, (WORD)_Real_i, CV_HIGH, REAL, (REAL)PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CHigh );
			WRITECVPARAMETER( lpCobj_All_->LPCV, (WORD)_Real_i, CV_FHIGH, REAL, (REAL)PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CFault_high );
			WRITECVPARAMETER( lpCobj_All_->LPCV, (WORD)_Real_i, CV_FHSET, REAL, (REAL)PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CFault_hsetpoint );
			PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CLow = PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CMin;
			PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CFault_low = PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CFault_min;
			PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CFault_lsetpoint = PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CFault_nsetpoint;
			WRITECVPARAMETER( lpCobj_All_->LPCV, (WORD)_Real_i, CV_LOW, REAL, (REAL)PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CLow );
			WRITECVPARAMETER( lpCobj_All_->LPCV, (WORD)_Real_i, CV_FLOW, REAL, (REAL)PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CFault_low );
			WRITECVPARAMETER( lpCobj_All_->LPCV, (WORD)_Real_i, CV_FLSET, REAL, (REAL)PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CFault_lsetpoint );
		}
		if( PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CControl_type == 0 || PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CControl_type == 1 )
		{
			PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CHigh = PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CMax;
			PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CLow = PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CMin;
			PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CFault_high = PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CFault_max;
			PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CFault_low = PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CFault_min;
			PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CFault_hsetpoint = PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CFault_xsetpoint;
			PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CFault_lsetpoint = PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CFault_nsetpoint;
		}
		if( PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CControl_type == 1 )
		{
			if( PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CSetpoint >= PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CFault_max )
			{
				PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CSetpoint = PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CFault_xsetpoint;
				WRITECVPARAMETER( lpCobj_All_->LPCV, (WORD)_Real_i, CV_SET, REAL, (REAL)PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CSetpoint );
			}
			if( PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CSetpoint <= PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CFault_min)
			{
				PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CSetpoint = PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CFault_nsetpoint;
				WRITECVPARAMETER( lpCobj_All_->LPCV, (WORD)_Real_i, CV_SET, REAL, (REAL)PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CSetpoint );
			}
		}
	}

	for( _j = 0; _j < lpCobj_All_->m; _j++ )
	{
		_Real_j = (int)MGET( ( COBJ_2_MATRIX( lpCobj_All_->LPApartMV ) ), lpCobj_All_->Apart_i, _j ) - 1;
		if( PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MIrvHigh > PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MMax || 
				PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MIrvLow < PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MMin )
		{
			PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MIrvHigh = PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MMax ;
			PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MIrvFault_high = PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MFault_max;
			PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MFault_hirv = PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MFault_xirv;
			WRITEMVPARAMETER( lpCobj_All_->LPMV, (WORD)_Real_j, MV_HIGH, REAL, (REAL)PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MIrvHigh );
			WRITEMVPARAMETER( lpCobj_All_->LPMV, (WORD)_Real_j, MV_FHIGH, REAL, (REAL)PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MIrvFault_high );
			WRITEMVPARAMETER( lpCobj_All_->LPMV, (WORD)_Real_j, MV_FHSET, REAL, (REAL)PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MFault_hirv );
			PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MIrvLow = PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MMin ;
			PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MIrvFault_low = PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MFault_min;
			PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MFault_lirv = PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MFault_nirv;
			WRITEMVPARAMETER( lpCobj_All_->LPMV, (WORD)_Real_j, MV_LOW, REAL, (REAL)PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MIrvLow );
			WRITEMVPARAMETER( lpCobj_All_->LPMV, (WORD)_Real_j, MV_FLOW, REAL, (REAL)PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MIrvFault_low );
			WRITEMVPARAMETER( lpCobj_All_->LPMV, (WORD)_Real_j, MV_FLSET, REAL, (REAL)PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MFault_lirv );
		}

		if( PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MControl_type == 1 )
		{
			PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MIrvHigh = PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MMax;
			PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MIrvLow = PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MMin;
			PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MIrvFault_high = PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MFault_max;
			PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MIrvFault_low = PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MFault_min;
			PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MFault_hirv = PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MFault_xirv;
			PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MFault_lirv = PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MFault_nirv;
			if( PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MIrv > PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MMax )
			{
				PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MIrv = PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MMax;
				WRITEMVPARAMETER( lpCobj_All_->LPMV, (WORD)_Real_j, MV_SET, REAL, (REAL)PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MIrv );
			}
			if( PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MIrv < PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MMin )
			{
				PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MIrv = PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MMin;
				WRITEMVPARAMETER( lpCobj_All_->LPMV, (WORD)_Real_j, MV_SET, REAL, (REAL)PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MIrv );
			}
		}
	}

// note: here the model is supposed to be a delt model
	if( lpCobj_All_->Count == 1 )
	{
		for( _j = 0; _j < lpCobj_All_->m; _j++ )
		{
//			PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MData = 0.0;
			PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MCT_history = 4;
		}
		for( _i = 0; _i < lpCobj_All_->p; _i++ )
		{
//			PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CData = 0.0;
			PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CCT_history = 4;
		}
	}
	if( lpCobj_All_->Flag_AM == 2 )
	{
		for( _j = 0; _j < lpCobj_All_->m; _j++ )
			PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MCT_history = 4;
		for( _i = 0; _i < lpCobj_All_->p; _i++ )
		  PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CCT_history = 4;
	}
// here if IRV is changed, then recover it from no IRV; 
	if( lpCobj_All_->Flag_MChange == 1 || lpCobj_All_->Flag_LPChange == 1 )
	{
		for( _j = 0; _j < lpCobj_All_->m; _j++ )
			PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MIRVWrong = 0;
		for( _i = 0; _i < lpCobj_All_->p; _i++ )
			PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CLPWrong = 0;
	}

	if( lpCobj_All_->Count >= 2 )
	{
		_flag_LP = 0;
		for( _j = 0;  _j < lpCobj_All_->m ; _j++ )
		{
			if( PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MControl_type !=
					PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MLastCM )
				_flag_LP = 1;
			else
			{
				if( (PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MLastIRV !=
						PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MIrv) &&
							PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MControl_type == 1 )
					_flag_LP = 1;
				else
					if( PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MControl_type != 1 )
					{
						if( (PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MLastHigh !=
								PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MIrvHigh) ||
									(PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MLastLow !=
										PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MIrvLow) )
							_flag_LP = 1;
					}
			}
		}

	//	here if _flag_CControl == 1, Max - Min should be considered at
	//	the same time
	//  Any changes
		for( _i = 0; _i < lpCobj_All_->p; _i++ )
		{
			if( PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CControl_type !=
				PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CLastCM )
				_flag_LP = 1;
			else
				if( PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CControl_type == 1 )
				{
					if( PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CSetpoint !=
						PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CLastSet )
						_flag_LP = 1;
				}
				else
					if( PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CControl_type == 2 )
					{
						if( ( PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CLastHigh !=
								PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CHigh) ||
							( PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CLow !=
								PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CLastLow) )
							_flag_LP = 1;
					}
		}
		if( _flag_LP == 1 )
		{
			for( _i = 0; _i < lpCobj_All_->p; _i++ )
				if( PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CLPWrong == 1 )
					PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CLPWrong = 0;
			for( _j = 0; _j < lpCobj_All_->m; _j++ )
				if( PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MIRVWrong == 1 )
				{
					PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MIRVWrong = 0;
					PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MCT_history = 0;
				}
		}
		else
		{
			for( _i = 0; _i < lpCobj_All_->p; _i++ )
				if( PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CLPWrong == 1 )
					PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CControl_type = 2;
			for( _j = 0; _j < lpCobj_All_->m; _j++ )
				if( PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MIRVWrong == 1 )
					PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MControl_type = 2;
		}
	}
// it has been done in the file "tfr_mcv.c"
	for( _i = 0; _i < lpCobj_All_->p; _i++ )
	{
		COMPARE( PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CMin, PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CMax );	
		COMPARE( PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CLow, PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CHigh );	
		COMPARE( PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CFault_min, PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CFault_max );	
		COMPARE( PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CFault_low, PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CFault_high );	
		COMPARE( PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CFault_nsetpoint, PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CFault_xsetpoint );	
		COMPARE( PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CFault_lsetpoint, PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CFault_hsetpoint );
	}
	for( _j = 0; _j < lpCobj_All_->m; _j++ )
	{
		COMPARE( PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MMin, PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MMax );	
		COMPARE( PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MIrvLow, PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MIrvHigh );	
		COMPARE( PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MFault_min, PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MFault_max );	
		COMPARE( PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MIrvFault_low, PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MIrvFault_high );
		COMPARE( PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MFault_nirv, PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MFault_xirv );
		COMPARE( PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MFault_lirv, PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MFault_hirv );
	}

	return;
}
