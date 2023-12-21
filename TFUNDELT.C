#include 	"tfalgo.h"
#include 	"tfcmdmc.h"

ACRET UnDeltFile( LPCOBJPRIV lpCobj_All_, LPCOBJPRIVMV lpCobj_PrivMV_,
				LPCOBJPRIVCV lpCobj_PrivCV_,
				LPCOBJPRIVDV lpCobj_PrivDV_ )
{
	int			_i, _j, _k;
	double		_ini;

	for( _j = 0; _j < lpCobj_All_->m; _j++ )
	{
		_ini = PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MIni;
		PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MMax += _ini;
		PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MMin += _ini;
		PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MIrvHigh += _ini;
		PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MIrvLow += _ini;
		PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MSteadOptim += _ini;
		PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MLPIni += _ini;
		PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MFault_max  +=_ini;
		PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MFault_min  +=_ini;
		PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MIrvFault_high  +=_ini;
		PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MIrvFault_low  +=_ini;
		PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MFault_xirv  +=_ini;
		PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MFault_nirv  +=_ini;
		PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MFault_hirv  +=_ini;
		PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MFault_lirv  +=_ini;
	}
	for( _i = 0; _i < lpCobj_All_->p; _i++ )
	{
		_ini = PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CIni;
		PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CData += _ini;
		PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CSetpoint += _ini;
		PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CLastSet += _ini;
		PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CMax += _ini;
		PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CMin += _ini;
		PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CHigh += _ini;
		PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CLow += _ini;
		PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CSteadOptim += _ini;
		PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CLPIni += _ini;
		PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CFault_max  +=_ini;
		PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CFault_min  +=_ini;
		PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CFault_high  +=_ini;
		PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CFault_low  +=_ini;
		PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CFault_xsetpoint  +=_ini;
		PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CFault_nsetpoint  +=_ini;
		PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CFault_hsetpoint  +=_ini;
		PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CFault_lsetpoint  +=_ini;
	}
//read DV now
	for( _k = 0; _k < lpCobj_All_->nd; _k++ )
	{
		_ini = PRIVDVGET( lpCobj_PrivDV_, _k, 0 )->DIni;
		PRIVDVGET( lpCobj_PrivDV_, _k, 0 )->DData  +=_ini;
	}
	return ACRET_TRUE;
}