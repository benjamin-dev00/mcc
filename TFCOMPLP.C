#include "tfalgo.h"
#include "tfwsqp.h"
#ifdef __ONSPEC
#include "Mtmfun.h"
#include "Mtdllfun.h"
#endif
#define		LittleEdge	1.0e-4
extern char LocalDirectory[128];

WORD CompoundLP( LPCOBJPRIV lpCobj_All_, LPCOBJPRIVMV lpCobj_PrivMV_, LPCOBJPRIVCV lpCobj_PrivCV_,
				LPCOBJPRIVDV lpCobj_PrivDV_ )
{
	int			_i, _j, _row, _col, _l, _kk, _Apart_i;
	WORD		_freetemp = 0, _FreeCV = 0;
	LPMATRIX	_lpmGtemp;
//	double		_mG[pp][mm];
	WORD		_flag_IRVError = 0;
	WORD		_flagError;
	BOOL		_WrongInput, _NoIrv;
	double		_ResetValue;
	float		_Fault_limit, _Fault_set;
	int			_Freecount;
	LPCOBJAPART	_lpCobjApart;
	LPCOBJPRIV	_lpCobjApartAll;
	LPCOBJPRIVMV	_lpCobj_ApartMV;
	LPCOBJPRIVCV	_lpCobj_ApartCV;
	char	_GlobalFile[128], _szBuffer[128];

	_row = lpCobj_All_->p;
	_col = lpCobj_All_->m;

	//get an effective transfer function
	for( _i = 0; _i < lpCobj_All_->p; _i++ )
		if( PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CNoLP == 2 )
			PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CControl_type = 1;
	for( _i = 0; _i < lpCobj_All_->p; _i++ )
		if( PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CControl_type == 4 )
			_row--;

	for( _j = 0; _j < lpCobj_All_->m; _j++ )
		if( PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MControl_type == 4 )
			_col--;

	if( _row <= 0 || _col <= 0 )
	{
		MessageBeep(-1);
		LogError( "Varaible Lack", "tfcomplp.c, 46" );
		_flagError = 1;
		goto __BAD;
	}
	_lpmGtemp = MatrixAlloc( _row, _col );
	GetLPG( lpCobj_All_, lpCobj_PrivMV_, lpCobj_PrivCV_, _lpmGtemp );
//	get the real G Matrix
/*	for( _i = 0; _i < _row; _i++ )
		for( _j = 0; _j < _col; _j++ )
			_mG[_i][_j] = MGET( _lpmGtemp, _i, _j );
*/
	_flagError = 0;
	_flagError = sqp( lpCobj_All_, _lpmGtemp, lpCobj_PrivMV_, lpCobj_PrivCV_, lpCobj_PrivDV_ );
//  Here we would determine whether LP error is due to wrong input or not
	_WrongInput = 0;
	if( _flagError == 1 )
	{
		if( lpCobj_All_->Flag_AM != 2 )
//  not M->A state
		{
			for( _i = 0; _i < lpCobj_All_->p; _i++ )
			{
				if( PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CSetpoint != PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CLastSet )
				{
					PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CSetpoint = PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CLastSet;
					_ResetValue = PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CSetpoint;
					_ResetValue += PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CIni;
					UNUNIT( _ResetValue, PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CHScale, PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CLScale );
					WRITECVPARAMETER( lpCobj_All_->LPCV, (WORD)_i, CV_SET, REAL, (REAL)_ResetValue );
					PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CSetpoint = PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CLastSet;
					_WrongInput = 1;
				}
				if( PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CHigh != PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CLastHigh ||
						PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CLow != PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CLastLow )
				{
					PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CHigh = PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CLastHigh;
					PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CLow = PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CLastLow;
					_WrongInput = 1;
					_ResetValue = PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CHigh;
					_ResetValue += PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CIni;
					UNUNIT( _ResetValue, PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CHScale, PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CLScale );
					WRITECVPARAMETER( lpCobj_All_->LPCV, (WORD)_i, CV_HIGH, REAL, (REAL)_ResetValue );
#ifdef __NOHIGH
					strcpy( _GlobalFile, LocalDirectory );
#ifdef __ONSPEC
					strcat( _GlobalFile, "tfmcc.ini" );
#else
					strcat( _GlobalFile, "tfmcccore.ini" );
#endif
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
					WRITECVPARAMETER( lpCobj_All_->LPCV, (WORD)_i, CV_MAX, REAL, (REAL)_ResetValue );
#endif
					_ResetValue = PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CLow;
					_ResetValue += PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CIni;
					UNUNIT( _ResetValue, PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CHScale, PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CLScale );
					WRITECVPARAMETER( lpCobj_All_->LPCV, (WORD)_i, CV_LOW, REAL, (REAL)_ResetValue );
#ifdef __NOHIGH
					WRITECVPARAMETER( lpCobj_All_->LPCV, (WORD)_i, CV_MIN, REAL, (REAL)_ResetValue );
#endif
					PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CHigh = PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CLastHigh;
					PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CLow = PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CLastLow;
#ifdef __NOHIGH
					PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CMax = PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CLastHigh;
					PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CMin = PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CLastLow;
					if( lpCobj_All_->Flag_Fault == 0 )
					{
						PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CFault_max = PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CMax -
							( PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CMax - PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CMin ) * _Fault_limit;
						PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CFault_min = PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CMin +
							( PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CMax - PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CMin ) * _Fault_limit;
						PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CFault_xsetpoint = PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CMax -
							( PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CMax - PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CMin ) * _Fault_set;
						PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CFault_nsetpoint = PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CMin +
							( PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CMax - PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CMin ) * _Fault_set;
						PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CFault_high = PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CHigh -
							( PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CHigh - PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CLow ) * _Fault_limit;
						PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CFault_low = PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CLow +
							( PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CHigh - PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CLow ) * _Fault_limit;
						PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CFault_hsetpoint = PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CHigh -
							( PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CHigh - PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CLow ) * _Fault_set;
						PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CFault_lsetpoint = PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CLow +
							( PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CHigh - PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CLow ) * _Fault_set;
					}
#endif
				}
			}
			for( _j = 0; _j < lpCobj_All_->m; _j++ )
			{
				if( PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MIrv != 
						PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MLastIRV )
				{
					PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MIrv = 
						PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MLastIRV;
					_ResetValue =  PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MIrv;
					_ResetValue += PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MIni;
					UNUNIT( _ResetValue, PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MHScale, PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MLScale );
					WRITEMVPARAMETER( lpCobj_All_->LPMV, (WORD)_j, MV_SET, REAL, (REAL)_ResetValue );
					PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MIrv = 
						PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MLastIRV;
					_WrongInput = 1;
				}
				if( PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MLastHigh != PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MIrvHigh ||
						PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MLastLow != PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MIrvLow )
				{
					PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MIrvLow = 
						PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MLastLow;
					PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MIrvHigh = 
						PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MLastHigh;
					_ResetValue =  PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MIrvHigh;
					_ResetValue += PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MIni;
					UNUNIT( _ResetValue, PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MHScale, PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MLScale );
					WRITEMVPARAMETER( lpCobj_All_->LPMV, (WORD)_j, MV_HIGH, REAL, (REAL)_ResetValue );
#ifdef __NOHIGH
					WRITEMVPARAMETER( lpCobj_All_->LPMV, (WORD)_j, MV_MAX, REAL, (REAL)_ResetValue );
#endif
					_ResetValue =  PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MIrvLow;
					_ResetValue += PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MIni;
					UNUNIT( _ResetValue, PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MHScale, PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MLScale );
					WRITEMVPARAMETER( lpCobj_All_->LPMV, (WORD)_j, MV_LOW, REAL, (REAL)_ResetValue );
#ifdef __NOHIGH
					WRITEMVPARAMETER( lpCobj_All_->LPMV, (WORD)_j, MV_MIN, REAL, (REAL)_ResetValue );
#endif
					_WrongInput = 1;
					PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MIrvLow = 
						PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MLastLow;
					PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MIrvHigh = 
						PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MLastHigh;
#ifdef __NOHIGH
					PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MMin = 
						PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MLastLow;
					PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MMax = 
						PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MLastHigh;
					if( lpCobj_All_->Flag_Fault == 0 )
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
#endif
				}
			}
		}
	}
	if( _WrongInput == 1 )
	{
		_flagError = sqp( lpCobj_All_, _lpmGtemp, lpCobj_PrivMV_, lpCobj_PrivCV_, lpCobj_PrivDV_ );
		if( _flagError == 1 )
		{
//			MessageBox( NULL, "ERROR", "SORRY, PROBLEM HERE", MB_OK );
			MessageBeep( -1 );
			lpCobj_All_->ErrorCode = 10;
			LogError( "Parameter input", "tfcomplp.c, wrong input" );

/*			lpCobj_All_->Flag_Alarm = 1 ;
			lpCobj_All_->Flag_HopeAM = 0;
			WRITEALLPARAMETER( lpCobj_All_, MDMCIRVD_FLAGALARM, BOOL, 1 );
			WRITEALLPARAMETER( lpCobj_All_, MDMCIRVD_FLAGHOPEAM, BOOL, 1 );
	*/	}
	}
	if( _flagError == 1 )
	{
		for( _j = 0; _j < lpCobj_All_->m; _j++ )
			if( PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MControl_type == 1 )
			{
//				PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MControl_type = 9;
				PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MIRVWrong = 1;
//				PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MLastIRV =
//					PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MIrv;
				PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MControl_type = 2;
				_freetemp++;
				_flag_IRVError = 1;
			}
		if( _flag_IRVError == 1 )
		{
			_flagError = sqp( lpCobj_All_, _lpmGtemp, lpCobj_PrivMV_, lpCobj_PrivCV_, lpCobj_PrivDV_ );
		}
	}
//	Free CV setpoint
	if( _flagError == 1 )
	{
		CVPrioritySort( lpCobj_All_, lpCobj_PrivCV_ );
		for( _l = 0; (_l < (lpCobj_All_->p))&&(_flagError == 1); _l++ )
		{
			_kk = (int)MGET( COBJ_2_MATRIX( lpCobj_All_->LPCVSort ), _l, 0 );
			if( PRIVCVGET( lpCobj_PrivCV_, _kk, 0 )->CControl_type == 1 )
//note
//				PRIVCVGET( lpCobj_PrivCV_, _kk, 0 )->CControl_type == 2 )
			{
				PRIVCVGET( lpCobj_PrivCV_, _kk, 0 )->CLPWrong = 1;
				PRIVCVGET( lpCobj_PrivCV_, _kk, 0 )->CControl_type = 3;
				PRIVCVGET( lpCobj_PrivCV_, _kk, 0 )->CDirty = 1;
				lpCobj_All_->Flag_MCVRecount = 1;
/*				_row--;
				if( _row == 0 || _col == 0 )
				{
//					MessageBox( NULL, "No MV or CV to be controled", "ERROR", MB_OK );
					goto _end;
				}
				_lpmGtemp = MatrixAlloc( _row, _col );
				GetLPG( lpCobj_All_, lpCobj_PrivMV_, lpCobj_PrivCV_, _lpmGtemp );
*/				_flagError = sqp( lpCobj_All_, _lpmGtemp, lpCobj_PrivMV_,
						lpCobj_PrivCV_, lpCobj_PrivDV_ );
				_FreeCV++;
//				MATRIXFREE( _lpmGtemp );
			}
		}
	}

	if( _flagError == 1 && lpCobj_All_->ErrorCode != 1 )
	{
		lpCobj_All_->ErrorCode = 1;
		MessageBeep( -1 );
//		MessageBox( NULL, "ERROR", "LP", MB_OK );
		LOGERR0( "LPWrong" );
		lpCobj_All_->Flag_Alarm = 1;
		LogError( "LPWrong", "tfcomplp.c, LPWRONG" );

#ifdef __debug
		LPDebugFile( lpCobj_All_, lpCobj_PrivMV_, lpCobj_PrivCV_, lpCobj_PrivDV_ );
#endif
		LookOut( lpCobj_All_, lpCobj_PrivMV_, lpCobj_PrivCV_, lpCobj_PrivDV_ );
	}
	for( _i =0; _i < lpCobj_All_->p; _i++ )
	{
		if( PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CControl_type == 2 ||
				PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CControl_type == 3 )
		{
			PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CHEdge = 0;
			PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CLEdge = 0;
			if( fabs( PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CSteadOptim - 
				PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CFault_hsetpoint ) < LittleEdge )
				PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CHEdge = 1;
			if( fabs( PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CSteadOptim - 
				PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CFault_lsetpoint ) < LittleEdge )
				PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CLEdge = 1;
		}
        else
		{
		   PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CHEdge = 0;
		   PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CLEdge = 0;
		}
	}
	for( _j = 0; _j < lpCobj_All_->m; _j++ )
	{
		PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MHEdge = 0;
		PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MLEdge = 0;
        if( PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MControl_type == 2 ||
                PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MControl_type == 3 )
        {
            if( fabs( PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MSteadOptim -
                            PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MIrvHigh ) < LittleEdge ) 
                    PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MHEdge = 1;
            if( fabs( PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MSteadOptim - 
                            PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MIrvLow ) < LittleEdge )
                    PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MLEdge = 1;
        }
	}	
/*	if( lpCobj_All_->Freecount > 0 )
	{
		for( _i = 0; _i < lpCobj_All_->p; _i++ )
			if( PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CControl_type == 2 )
			{
				_Freecount--;
			}
	}
*/	/*if( _NoIrv == 0 && lpCobj_All_->Freecount > 0 )
	{
		for( _j = 0; _j < lpCobj_All_->m; _j++ )
		{
			if( PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MSatuation == 1 )
			{
				PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MIrv =
					PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MSteadOptim;
				PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MControl_type = 1;
//				WRITEMVPARAMETER( lpCobj_All_->LPMV, (WORD)_j, MV_CONTROLMODE, BOOL, 1 );
				lpCobj_All_->Freecount--;
			}
		}
	}
/*		for( _kk = 0; ( _kk < lpCobj_All_->p )&& ( lpCobj_All_->Freecount > 0 ); _kk++ )
		{
			_i = (int)MGET( COBJ_2_MATRIX( lpCobj_All_->LPCVSort ), (lpCobj_All_->p-_kk-1), 0 );
			if( PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CSDirty == 1 &&
					PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CControl_type == 3 )
			{
				PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CControl_type = 1;
//				WRITECVPARAMETER( lpCobj_All_->LPCV, (WORD)_i, CV_CONTROLMODE, BOOL, 1 );
				lpCobj_All_->Freecount--;
			}
		}*/
	for( _Apart_i = 0; _Apart_i < lpCobj_All_->No_Apart; _Apart_i++ )
	{
		_lpCobjApart = COBJ_2_APART( lpCobj_All_->LPApart );
		_lpCobjApartAll = APARTGET( _lpCobjApart, _Apart_i, 0 );
		_lpCobj_ApartMV = COBJ_2_PRIVMV( _lpCobjApartAll->LPINMV );
		_lpCobj_ApartCV = COBJ_2_PRIVCV( _lpCobjApartAll->LPINCV );
		
		for( _j = 0; _j < _lpCobjApartAll->m; _j++ )
			if( PRIVMVGET( _lpCobj_ApartMV, _j, 0 )->MDist == 1 )
			{
				PRIVMVGET( _lpCobj_ApartMV, _j, 0 )->MControl_type = 5;
//By ttf, 10. 19, 1998, dunno
//				PRIVMVGET( _lpCobj_ApartMV, _j, 0 )->MDist = 0;
			}
		GetFreedom( _lpCobjApartAll, _lpCobj_ApartMV, _lpCobj_ApartCV );
		MVPrioritySort( _lpCobjApartAll, _lpCobj_ApartMV );
		_NoIrv = 0;
		_Freecount = _lpCobjApartAll->Freecount;
		if( _NoIrv == 0 && _Freecount > 0 )
		{
			for( _kk = 0; ( _kk< _lpCobjApartAll->m)&&( _Freecount > 0 ); _kk++ )
			{
				_j = (int)MGET( COBJ_2_MATRIX( _lpCobjApartAll->LPMVSort ), (_lpCobjApartAll->m-_kk-1), 0 );
				if( PRIVMVGET( _lpCobj_ApartMV, _j, 0 )->MSDirty == 1 && _lpCobjApartAll->Flag_Alarm == 0 &&
						( PRIVMVGET( _lpCobj_ApartMV, _j, 0 )->MControl_type == 3  ||
							PRIVMVGET( _lpCobj_ApartMV, _j, 0 )->MControl_type == 2 ) )
				{
					PRIVMVGET( _lpCobj_ApartMV, _j, 0 )->MIrv =
						PRIVMVGET( _lpCobj_ApartMV, _j, 0 )->MSteadOptim;
					PRIVMVGET( _lpCobj_ApartMV, _j, 0 )->MControl_type = 1;
	//				WRITEMVPARAMETER( _lpCobjApartAll->LPMV, (WORD)_j, MV_CONTROLMODE, BOOL, 1 );
					_lpCobjApartAll->Freecount--;
					_Freecount--;
				}
			}
		}
	}
	for( _i = 0; _i < lpCobj_All_->p; _i++ )
	{
		if( lpCobj_All_->Flag_RDebug )
		{
			PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CLastSet = 
				PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CSetpoint;
			PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CLastHigh =
				PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CHigh;
			PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CLastLow =
				PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CLow;
		}
	}
	for( _j = 0; _j < lpCobj_All_->m; _j++ )
	{
		if( lpCobj_All_->Flag_RDebug )
		{
			PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MLastIRV = 
				PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MIrv;
			PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MLastHigh =
				PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MIrvHigh;
			PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MLastLow =
				PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MIrvLow;
		}
	}
	MATRIXFREE( _lpmGtemp );
	GetFreedom( lpCobj_All_, lpCobj_PrivMV_, lpCobj_PrivCV_ );
__BAD:
	for( _i = 0; _i < lpCobj_All_->p; _i++ )
	{
		if( PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CNoLP == 2 )
		{
			if( PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CControl_type == 1 )
				PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CControl_type = 3;
			else
			{
				MessageBeep( -1 );
				lpCobj_All_->ErrorCode = 11;
				LogError( "NoLP", "tfcomplp.c, 431" );

			}
		}
	}
	return _flagError;
}
