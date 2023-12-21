#include "tfalgo.h"
#include "tfmcv.h"
#include "Mtmfun.h"
#include "Mtdllfun.h"

void ReadPriv( LPCOBJPRIV lpCobj_All_,
					LPCOBJPRIVMV lpCobj_PrivMV_,
						LPCOBJPRIVCV lpCobj_PrivCV_,
							LPCOBJPRIVDV lpCobj_PrivDV_ )
{
/*	int 		_i, _j, _k;
	LPCOBJ      _lpCobj_Priv;

	for( _i = 0; _i < lpCobj_All_->p; _i++ )
	{
		ONSPECREADCV( lpCobj_All_->LPCV, _i, CV_PRIV, COBJ, _lpCobj_Priv );
		PRIVCVGET( lpCobj_PrivCV_, _i, 0 ) = ( LPVARCV )((void far*)&( _lpCobj_Priv ->data[0]));
	}
	for( _j = 0; _j < lpCobj_All_->m; _j++ )
	{
		ONSPECREADMV( lpCobj_All_->LPMV, _j, MV_PRIV, COBJ, _lpCobj_Priv );
		PRIVMVGET( lpCobj_PrivMV_, _j, 0 ) = ( LPVARMV )((void far*)&( _lpCobj_Priv ->data[0]));
	}
	for( _k = 0; _k < lpCobj_All_->nd; _k++ )
	{
		ONSPECREADDV( lpCobj_All_->LPDV, _k, DV_PRIV, COBJ, _lpCobj_Priv );
		PRIVDVGET( lpCobj_PrivDV_, _k, 0 ) = ( LPVARDV )((void far*)&( _lpCobj_Priv ->data[0]));
	}
*/	return;
}