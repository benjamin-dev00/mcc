//===================================================================
//
//Zhang 4.22, 1998
//===================================================================

#include "tfalgo.h"
#include "tfcmdmc.h"
#ifdef __ONSPEC
#include "Mtmfun.h"
#include "Mtdllfun.h"
#endif

void Display( LPCOBJPRIV lpCobj_All_, LPCOBJPRIVMV lpCobj_PrivMV_, 
					LPCOBJPRIVCV lpCobj_PrivCV_, LPCOBJPRIVDV lpCobj_PrivDV_ )
{
	WORD	_i, _j, _k;

	for( _j = 0; _j < lpCobj_All_->m; _j++ )
	{
		PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MDisplay = 100 * 
			( PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MData - PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MDisplayL ) /
				( PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MDisplayH - PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MDisplayL );
		WRITEMVPARAMETER( lpCobj_All_->LPMV, _j, MV_DISPLAY, REAL, (REAL)PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MDisplay );
	}
	for( _i = 0; _i < lpCobj_All_->p; _i++ )
	{	PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CDisplay = 100 * 
			( PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CData - PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CDisplayL ) /
				( PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CDisplayH - PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CDisplayL );
		WRITECVPARAMETER( lpCobj_All_->LPCV, _i, CV_DISPLAY, REAL, (REAL)PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CDisplay );
	}
	for( _k = 0; _k < lpCobj_All_->nd; _k++ )
	{
		PRIVDVGET( lpCobj_PrivDV_, _k, 0 )->DDisplay = 100 *
			( PRIVDVGET( lpCobj_PrivDV_, _k, 0 )->DData - PRIVDVGET( lpCobj_PrivDV_, _k, 0 )->DDisplayL ) /
				( PRIVDVGET( lpCobj_PrivDV_, _k, 0 )->DDisplayH - PRIVDVGET( lpCobj_PrivDV_, _k, 0 )->DDisplayL );
		WRITEDVPARAMETER( lpCobj_All_->LPDV, _k, DV_DISPLAY, REAL, (REAL)PRIVDVGET( lpCobj_PrivDV_, _k, 0 )->DDisplay );
	}
	return;
}