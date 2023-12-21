#include "tfalgo.h"
#include "tfcmdmc.h"
#ifdef debug
#include "tfdebug.h"
#endif

ACRET GET_SR( LPCOBJPRIV lpCobj_All_, LPCOBJPRIVMV lpCobj_PrivMV_, LPCOBJPRIVCV lpCobj_PrivCV_,
				LPCOBJPRIVDV lpCobj_PrivDV_ )
{
	int				_i, _j, _kk;
	LPDIMMATRIX 	_lpmSR, _lpmDSR;
	LPMATRIX 		_lpmG;
	double			_ul, _yl, _dl, _uyl, _dyl;
#ifdef __debug
#include "tfdebug.h"
	double			_mG[pp][mm];
#endif


// outlook, for temp use///////////
//	double			_mG[pp][mm];
// ////////////////////////////////

// they are the scale field of MV, CV, DV;

	if( lpCobj_All_->LPTempSR == NULL  )
	{
		lpCobj_All_->Flag_Run = 0;
		lpCobj_All_->Flag_Alarm = 1;
		LogError( "No SR", "tfg_sr, line 24" );
		return FALSE;
	}
	_lpmSR = MAKE_DIMMATRIX( lpCobj_All_->LPTempSR );
	_lpmDSR = MAKE_DIMMATRIX( lpCobj_All_->LPTempDSR );

#ifdef __debug
	for ( _i = 0; _i < lpCobj_All_->p; _i++)
		for ( _j = 0; _j < lpCobj_All_->m; _j++)
			_mG[_i][_j] = DIMMGET( _lpmSR, _i, _j, (lpCobj_All_->N-1) );
#endif
	for ( _i = 0; _i < lpCobj_All_->p; _i++)
	{
		_yl = PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CHScale - PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CLScale;
		if( _yl <= epsnorm )
		{
//			MessageBox( NULL, "CV Scale is 0 ", "ERROR", MB_OK );
			lpCobj_All_->Flag_Run = 0;
			lpCobj_All_->Flag_Alarm = 1;
			LogError( "CV Scale", "tfg_sr, line 43" );
			_yl = 1.0;
            return FALSE;
		}

		for ( _j = 0; _j < lpCobj_All_->m; _j++)
		{
			_ul = PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MHScale - PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MLScale;
			if( _ul <= epsnorm )
			{
				MessageBox( NULL, "MV Scale is 0 ", "ERROR", MB_OK );
            	_ul = 1.0;
				lpCobj_All_->Flag_Run = 0;
				lpCobj_All_->Flag_Alarm = 1;
				LogError( "MV Scale", "tfg_sr, line 57" );
				return FALSE;
			}
			_uyl = _ul / _yl ;
			for ( _kk = 0; _kk < lpCobj_All_->N; _kk++)
				DIMMGET( _lpmSR, _i, _j, _kk ) *= _uyl;
		}
	}

// outlook, for temp use////////////////////////////////////////////////
//	for ( _i = 0; _i < lpCobj_All_->p; _i++)
//		for ( _j = 0; _j < lpCobj_All_->m; _j++)
//			_mG[_i][_j] = DIMMGET( _lpmSR, _i, _j, (lpCobj_All_->N-1) );
// /////////////////////////////////////////////////////////////////////

// here can watch the data value of the model's step_response 
#ifdef debug
	for ( _i = 0; _i < lpCobj_All_->p; _i++)
		for ( _j = 0; _j < lpCobj_All_->m; _j++)
			_mG[_i][_j] = DIMMGET( _lpmSR, _i, _j, (lpCobj_All_->N-1) );
#endif
// here unify StepResponse

	_lpmG = MAKE_MATRIX( lpCobj_All_->LPG );

	for ( _i = 0; _i < lpCobj_All_->p; _i++)
		for ( _j = 0; _j < lpCobj_All_->m; _j++)
			MGET( _lpmG, _i, _j ) = DIMMGET( _lpmSR, _i, _j, (lpCobj_All_->N-1) );
// get Disturbance Step-response
	for ( _i = 0; _i < lpCobj_All_->p; _i++)
	{
		_yl = PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CHScale - PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CLScale;
		if( _yl <= epsnorm )
		{
			MessageBox( NULL, "CV Scale is 0 ", "ERROR", MB_OK );
			_yl = 1.0;
			lpCobj_All_->Flag_Run = 0;
			lpCobj_All_->Flag_Alarm = 1;
			LogError( "CV Scale", "tfg_sr, line 87" );
            return FALSE;
		}
		for ( _j = 0; _j < lpCobj_All_->nd; _j++)
		{
			_dl = PRIVDVGET( lpCobj_PrivDV_, _j, 0 )->DHScale - PRIVDVGET( lpCobj_PrivDV_, _j, 0 )->DLScale;
			if( _dl <= epsnorm )
			{
				MessageBox( NULL, "DV Scale is 0 ", "ERROR", MB_OK );
				_dl = 1.0;
				lpCobj_All_->Flag_Run = 0;
				lpCobj_All_->Flag_Alarm = 1;
				LogError( "DV Scale", "tfg_sr, line 99" );
	            return FALSE;
			}
			_dyl = _dl / _yl;
			for ( _kk = 0; _kk < lpCobj_All_->N; _kk++)
				DIMMGET( _lpmDSR, _i, _j, _kk ) *= _dyl;
		}
	}
// unify Disturbance Step-response

	return	TRUE;
}


