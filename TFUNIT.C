//	Unify the CV, MV, and DV

#include "tfalgo.h"
#include "tfcmdmc.h"
#ifdef __ONSPEC
#include "Mtmfun.h"
#include "Mtdllfun.h"
#endif

void UnitMCV( LPCOBJPRIV lpCobj_All_, LPCOBJPRIVMV lpCobj_PrivMV_,
							LPCOBJPRIVCV lpCobj_PrivCV_,
							LPCOBJPRIVDV lpCobj_PrivDV_ )
{
	WORD		_i, _j, _k, _Real_i, _Real_j;

	for( _i = 0; _i < lpCobj_All_->p; _i++ )
	{
		if( PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CHScale <=
			 PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CLScale )
		{
			MessageBox( NULL, "ERROR", "CVSCALE", MB_OK );
			PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CLScale = 0;
			PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CHScale = 1.0;
			lpCobj_All_->Flag_Run = 0;
            lpCobj_All_->Flag_Alarm = 1;
			LogError( "CV Scale HL", "tfunit.c, CV" );
			_Real_i = (int)MGET( ( COBJ_2_MATRIX( lpCobj_All_->LPApartCV ) ), _i, 0 ) - 1;
			WRITECVPARAMETER( lpCobj_All_->LPCV, _Real_i, CV_HSCALE, REAL, 1 );
			WRITECVPARAMETER( lpCobj_All_->LPCV, _Real_i, CV_LSCALE, REAL, 0 );
		}
	}
	for( _i = 0; _i < lpCobj_All_->p; _i++ )
	{
		UNIT( PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CData, PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CHScale, PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CLScale );
		UNIT( PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CMax, PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CHScale, PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CLScale );
		UNIT( PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CMin, PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CHScale, PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CLScale );
		UNIT( PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CLastSet, PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CHScale, PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CLScale );
		UNIT( PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CHigh, PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CHScale, PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CLScale );
		UNIT( PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CLastHigh, PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CHScale, PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CLScale );
		UNIT( PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CLow, PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CHScale, PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CLScale );
		UNIT( PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CLastLow, PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CHScale, PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CLScale );
		UNIT( PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CFault_max, PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CHScale, PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CLScale );
		UNIT( PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CFault_min, PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CHScale, PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CLScale );
		UNIT( PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CFault_high, PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CHScale, PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CLScale );
		UNIT( PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CFault_low, PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CHScale, PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CLScale );
		UNIT( PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CFault_xsetpoint, PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CHScale, PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CLScale );
		UNIT( PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CFault_nsetpoint, PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CHScale, PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CLScale );
		UNIT( PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CFault_hsetpoint, PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CHScale, PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CLScale );
		UNIT( PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CFault_lsetpoint, PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CHScale, PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CLScale );
		UNIT( PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CIni, PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CHScale, PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CLScale );
		UNIT( PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CLPIni, PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CHScale, PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CLScale );
		UNIT( PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CSetpoint, PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CHScale, PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CLScale );
		UNIT( PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CSteadOptim, PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CHScale, PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CLScale );
	}
	for( _j = 0; _j < lpCobj_All_->m; _j++ )
	{
		if( PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MHScale <=
			PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MLScale )
		{
			MessageBox( NULL, "ERROR", "MVSCALE", MB_OK );
			PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MHScale = 1.0;
			PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MLScale = 0.0;
			lpCobj_All_->Flag_Run = 0;
            lpCobj_All_->Flag_Alarm = 1;
			LogError( "MV Scale HL", "tfunit.c,MV" );
			_Real_j = (int)MGET( ( COBJ_2_MATRIX( lpCobj_All_->LPApartMV ) ), lpCobj_All_->Apart_i, _j ) - 1;
			WRITEMVPARAMETER( lpCobj_All_->LPMV, _Real_j, MV_HSCALE, REAL, 1.0 );
			WRITEMVPARAMETER( lpCobj_All_->LPMV, _Real_j, MV_HSCALE, REAL, 0.0 );
		}
	}
	for( _j = 0; _j < lpCobj_All_->m; _j++ )
	{
		UNIT( PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MData, 			PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MHScale, PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MLScale );
		UNIT( PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MSample, 			PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MHScale, PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MLScale );
		UNIT( PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MMax, 			PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MHScale, PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MLScale );
		UNIT( PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MMin, 			PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MHScale, PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MLScale );
		UNIT( PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MIrvHigh, 		PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MHScale, PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MLScale );
		UNIT( PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MIrvLow, 		PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MHScale, PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MLScale );
		UNIT( PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MLastIRV, 		PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MHScale, PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MLScale );
		UNIT( PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MLastHigh, 		PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MHScale, PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MLScale );
		UNIT( PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MLastLow, 		PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MHScale, PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MLScale );
		UNIT( PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MFault_max, 	PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MHScale, PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MLScale );
		UNIT( PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MFault_min, 	PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MHScale, PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MLScale );
		UNIT( PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MIrvFault_high,
			PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MHScale, PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MLScale );
		UNIT( PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MIrvFault_low, PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MHScale, PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MLScale );
		UNIT( PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MFault_xirv, 	PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MHScale, PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MLScale );
		UNIT( PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MFault_nirv, 	PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MHScale, PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MLScale );
		UNIT( PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MFault_hirv, 	PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MHScale, PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MLScale );
		UNIT( PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MFault_lirv, 	PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MHScale, PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MLScale );
		PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MDelmax /= ( PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MHScale - PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MLScale );
		PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MDelmin /= ( PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MHScale - PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MLScale );
		PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MDelt /= ( PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MHScale - PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MLScale );
		UNIT( PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MIrv, 			PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MHScale, PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MLScale );
		UNIT( PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MIni, 			PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MHScale, PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MLScale );
		UNIT( PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MLPIni, 			PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MHScale, PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MLScale );
		UNIT( PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MSteadOptim, 	PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MHScale, PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MLScale );
	}
	for( _k = 0; _k < lpCobj_All_->nd; _k++ )
	{
		if( PRIVDVGET( lpCobj_PrivDV_, _k, 0 )->DHScale <=
			PRIVDVGET( lpCobj_PrivDV_, _k, 0 )->DLScale )
		{
			MessageBox( NULL, "ERROR", "DVSCALE", MB_OK );
			PRIVDVGET( lpCobj_PrivDV_, _k, 0 )->DHScale = 1.0;
			PRIVDVGET( lpCobj_PrivDV_, _k, 0 )->DLScale = 0.0;
			lpCobj_All_->Flag_Run = 0;
			lpCobj_All_->Flag_Alarm = 1;
			LogError( "DV Scale HL", "tfunit.c, DV" );
			WRITEDVPARAMETER( lpCobj_All_->LPDV, _k, DV_HSCALE, REAL, 1.0 );
			WRITEDVPARAMETER( lpCobj_All_->LPDV, _k, DV_HSCALE, REAL, 0.0 );
		}
	}
	for( _k = 0; _k < lpCobj_All_->nd; _k++ )
	{
		UNIT( PRIVDVGET( lpCobj_PrivDV_, _k, 0 )->DData, PRIVDVGET( lpCobj_PrivDV_, _k, 0 )->DHScale, PRIVDVGET( lpCobj_PrivDV_, _k, 0 )->DLScale );
		UNIT( PRIVDVGET( lpCobj_PrivDV_, _k, 0 )->DIni, PRIVDVGET( lpCobj_PrivDV_, _k, 0 )->DHScale, PRIVDVGET( lpCobj_PrivDV_, _k, 0 )->DLScale );
		UNIT( PRIVDVGET( lpCobj_PrivDV_, _k, 0 )->DHistory, PRIVDVGET( lpCobj_PrivDV_, _k, 0 )->DHScale, PRIVDVGET( lpCobj_PrivDV_, _k, 0 )->DLScale );
		PRIVDVGET( lpCobj_PrivDV_, _k, 0 )->DDelt /= ( PRIVDVGET( lpCobj_PrivDV_, _k, 0 )->DHScale - PRIVDVGET( lpCobj_PrivDV_, _k, 0 )->DLScale );
	}
	return;
}

