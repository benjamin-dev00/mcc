//	consider freedom, if it is less than 0, than free some CV
#include "tfalgo.h"
#include "tfcmdmc.h"
#ifdef __ONSPEC
#include "Mtmfun.h"
#include "Mtdllfun.h"
#endif

// the control_type should be freed to be bad CV...????
void FreeCV( LPCOBJPRIV lpCobj_All_, LPCOBJPRIVCV lpCobj_PrivCV_ )
{
	WRD     _flag_found;
	int		_i, _k, _l, _addfree = 0, _Real_i, _freecount;
	
	CVPrioritySort( lpCobj_All_, lpCobj_PrivCV_ );
	_freecount = lpCobj_All_->Freecount;
	for( _l = 0; _l < (-_freecount); _l++ )
	{
		_flag_found = 0;
		for( _i = 0; (_i < lpCobj_All_->p) && ( _flag_found == 0 ); _i++ )
		{
			_k = (int)MGET( ( COBJ_2_MATRIX( lpCobj_All_->LPCVSort ) ), _i, 0 );
			if( PRIVCVGET( lpCobj_PrivCV_, _k, 0 )->CControl_type == 1 )
			{
				_Real_i = (int)MGET( ( COBJ_2_MATRIX( lpCobj_All_->LPApartCV ) ), lpCobj_All_->Apart_i, _k ) - 1;
				if( lpCobj_All_->Flag_CControl )
					WRITECVPARAMETER( lpCobj_All_->LPCV, (WORD)_Real_i, CV_CONTROLMODE, BOOL, 0 );
				PRIVCVGET( lpCobj_PrivCV_, _k, 0 )->CControl_type = 3;
				_flag_found = 1;
				PRIVCVGET( lpCobj_PrivCV_, _k, 0 )->CNoLP = 1;
				_addfree++;
			}
/*			if( PRIVCVGET( lpCobj_PrivCV_, _k, 0 )->CControl_type == 2 )
			{
				WRITECVPARAMETER( lpCobj_All_->LPCV, (WORD)_k, CV_CONTROLMODE, BOOL, 0 );
				PRIVCVGET( lpCobj_PrivCV_, _k, 0 )->CControl_type = 3;
				PRIVCVGET( lpCobj_PrivCV_, _k, 0 )->CNoLP = 2;
				_flag_found = 1;
	            _addfree++;
			}*/
		}
	}

	lpCobj_All_->Freecount += _addfree;
	if( lpCobj_All_->Freecount < 0 )
//		MessageBox( NULL, "ERROR", "NO FREEDOM NOW", MB_OK );
	{
		MessageBeep(-1);
		LogError( "Freecv", "Freedom lack" );
	//	lpCobj_All_->Flag_Alarm = 1 ;
		lpCobj_All_->Flag_HopeAM = 0;
	//	WRITEALLPARAMETER( lpCobj_All_, MDMCIRVD_FLAGALARM, BOOL, 1 );
		WRITEALLPARAMETER( lpCobj_All_, MDMCIRVD_FLAGHOPEAM, BOOL, 1 );
	}
	return ;
}
