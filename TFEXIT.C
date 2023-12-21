#include "tfalgo.h"
#include "tfcmdmc.h"
#ifdef __ONSPEC
#include "Mtmfun.h"
#include "Mtdllfun.h"
#endif

ACRET LookOut( LPCOBJPRIV lpCobj_All_, LPCOBJPRIVMV lpCobj_PrivMV_, 
					 LPCOBJPRIVCV lpCobj_PrivCV, LPCOBJPRIVDV _pCobj_PrivDV )
{
	WORD	_j;
	//, _Real_j;
	//LPMATRIX	_lpmMV;
//	double	_TempData, _DeltU;
//	BOOL	_Flag_Change;

	MessageBeep( -1 );
    LogError( "Exit", "tfexit.c" );
        //	if( lpCobj_All_->Apart_i != lpCobj_All_->No_Apart )
	//	_lpmMV = COBJ_2_MATRIX( lpCobj_All_->LPApartMV );
	for( _j = 0; _j < lpCobj_All_->m; _j++ )
	{
		PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MIrv = PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MData;
		PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MSetSatuation = 1;
		PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MControl_type = 5;
/*		_TempData = PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MData;
		PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MControl_type = 5;
		if( PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MAM )
		{
			_DeltU = -PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MData;
			_Flag_Change = 0;
			if( _DeltU > PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MDelmax )
			{
				_DeltU = PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MDelmax;
				_Flag_Change = 1;
			}
			if( _DeltU < PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MDelmin )
			{
				_DeltU = PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MDelmin;
				_Flag_Change = 1;
			}
			if( _Flag_Change )
			{
				PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MData += _DeltU;
				PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MData += PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MIni;
			}
			else
				PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MData = PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MIni;
		}
		else
			PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MData +=
				PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MIni;
		UNUNIT( PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MData, PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MHScale, PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MLScale );
		if( lpCobj_All_->Apart_i != lpCobj_All_->No_Apart )
			_Real_j = (int)MGET( _lpmMV, lpCobj_All_->Apart_i, _j ) - 1;
		else
			_Real_j = _j;
		WRITEMVPARAMETER( lpCobj_All_->LPMV, _Real_j, MV_VALUE, REAL, 
		  (REAL)PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MData );

		if( PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MAM )
			PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MData = 0;
		else
			PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MData = _TempData;
		PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MSetSatuation = 0;
		WRITEMVPARAMETER( lpCobj_All_->LPMV, _Real_j, MV_MSETSATUATION, BOOL, 0 );
*/
	}
	return ACRET_TRUE;
}
