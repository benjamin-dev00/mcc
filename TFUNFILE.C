#include "tfalgo.h"
#include "tfcmdmc.h"

ACRET 	UnUnitFile( LPCOBJPRIV lpCobj_All_, LPCOBJPRIVMV lpCobj_PrivMV_,
						LPCOBJPRIVCV lpCobj_PrivCV_, LPCOBJPRIVDV lpCobj_PrivDV_ )
{
	int		_i, _j, _k;

	for( _i = 0; _i < lpCobj_All_->p; _i++ )
	{
		UNUNIT( PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CMax, PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CHScale, PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CLScale );
		UNUNIT( PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CMin, PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CHScale, PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CLScale );
		UNUNIT( PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CHigh, PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CHScale, PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CLScale );
		UNUNIT( PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CLow, PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CHScale, PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CLScale );
		UNUNIT( PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CFault_max, PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CHScale, PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CLScale );
		UNUNIT( PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CFault_min, PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CHScale, PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CLScale );
		UNUNIT( PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CFault_high, PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CHScale, PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CLScale );
		UNUNIT( PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CFault_low, PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CHScale, PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CLScale );
		UNUNIT( PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CFault_xsetpoint, PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CHScale, PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CLScale );
		UNUNIT( PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CFault_nsetpoint, PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CHScale, PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CLScale );
		UNUNIT( PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CFault_hsetpoint, PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CHScale, PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CLScale );
		UNUNIT( PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CFault_lsetpoint, PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CHScale, PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CLScale );
		UNUNIT( PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CIni, PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CHScale, PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CLScale );
		UNUNIT( PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CLPIni, PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CHScale, PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CLScale );
		UNUNIT( PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CSetpoint, PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CHScale, PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CLScale );
		UNUNIT( PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CSteadOptim, PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CHScale, PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CLScale );
	}
	for( _j = 0; _j < lpCobj_All_->m; _j++ )
	{
		UNUNIT( PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MMax, 			PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MHScale, PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MLScale );
		UNUNIT( PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MMin, 			PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MHScale, PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MLScale );
		UNUNIT( PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MIrvHigh, 		PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MHScale, PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MLScale );
		UNUNIT( PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MIrvLow, 		PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MHScale, PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MLScale );
		UNUNIT( PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MFault_max, 	PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MHScale, PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MLScale );
		UNUNIT( PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MFault_min, 	PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MHScale, PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MLScale );
		UNUNIT( PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MIrvFault_high,
			PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MHScale, PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MLScale );
		UNUNIT( PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MIrvFault_low, PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MHScale, PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MLScale );
		UNUNIT( PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MFault_xirv, 	PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MHScale, PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MLScale );
		UNUNIT( PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MFault_nirv, 	PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MHScale, PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MLScale );
		UNUNIT( PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MFault_hirv, 	PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MHScale, PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MLScale );
		UNUNIT( PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MFault_lirv, 	PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MHScale, PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MLScale );
		PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MDelmax /= ( PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MHScale - PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MLScale );
		PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MDelmin /= ( PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MHScale - PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MLScale );
		PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MDelt /= ( PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MHScale - PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MLScale );
		UNUNIT( PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MIni, 			PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MHScale, PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MLScale );
		UNUNIT( PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MLPIni, 			PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MHScale, PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MLScale );
	}
	for( _k = 0; _k < lpCobj_All_->nd; _k++ )
	{
		UNUNIT( PRIVDVGET( lpCobj_PrivDV_, _k, 0 )->DData, PRIVDVGET( lpCobj_PrivDV_, _k, 0 )->DHScale, PRIVDVGET( lpCobj_PrivDV_, _k, 0 )->DLScale );
		UNUNIT( PRIVDVGET( lpCobj_PrivDV_, _k, 0 )->DIni, PRIVDVGET( lpCobj_PrivDV_, _k, 0 )->DHScale, PRIVDVGET( lpCobj_PrivDV_, _k, 0 )->DLScale );
		UNUNIT( PRIVDVGET( lpCobj_PrivDV_, _k, 0 )->DHistory, PRIVDVGET( lpCobj_PrivDV_, _k, 0 )->DHScale, PRIVDVGET( lpCobj_PrivDV_, _k, 0 )->DLScale );
		PRIVDVGET( lpCobj_PrivDV_, _k, 0 )->DDelt /= ( PRIVDVGET( lpCobj_PrivDV_, _k, 0 )->DHScale - PRIVDVGET( lpCobj_PrivDV_, _k, 0 )->DLScale );
	}


	return ACRET_TRUE;
}
