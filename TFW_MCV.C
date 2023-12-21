#include "tfalgo.h"
#include "tfcmdmc.h"
#ifdef __ONSPEC
#include "Mtmfun.h"
#include "Mtdllfun.h"
#endif
#define	MAXNUMBER		500
#define	RESETNUMBER		240
// it should not be odd number

void WriteMCV( LPCOBJPRIV lpCobj_All_, LPCOBJPRIVMV lpCobj_PrivMV_,
						LPCOBJPRIVCV lpCobj_PrivCV_, LPCOBJPRIVDV lpCobj_PrivDV_ )
{
	double		_u, _LastSet, _TempIrv;
	WORD		_j, _i, _Real_j, _Real_i, _Apart_i;
	WORD		_Control_type;
	LPCOBJ		_lpCobj_History, _lpCobj_All;
	LPCOBJAPART _lpPrivApart;
	LPCOBJPRIV	_lpApart;
	LPMATRIX	_lpmDeluM;
	LPMATRIX	_lpmP, _lpmQ, _lpmR;
	BOOL		_bReset = FALSE, _FS;
	WORD		_MakeParaCount, _RecountCount, _MParaCount;

	_lpmDeluM = MatrixAlloc( lpCobj_All_->m, 1 );
	_lpPrivApart = COBJ_2_APART( lpCobj_All_->LPApart );
	CobjKeepCopy( lpCobj_All_->LPHistory, PRIV_2_COBJ( lpCobj_All_ ) );
	for( _Apart_i = 0; _Apart_i < lpCobj_All_->No_Apart; _Apart_i++ )
	{
		_lpApart = APARTGET( _lpPrivApart, _Apart_i, 0 );
		_lpCobj_All = PRIV_2_COBJ( _lpApart );
		_lpCobj_History = _lpApart->LPHistory;
		CobjKeepCopy( _lpCobj_History, _lpCobj_All );
		_lpmP 		= MAKE_MATRIX( APARTGET( _lpPrivApart, _Apart_i, 0 )->LPS );
		_lpmQ 		= MAKE_MATRIX( APARTGET( _lpPrivApart, _Apart_i, 0 )->LPQ );
		_lpmR 		= MAKE_MATRIX( APARTGET( _lpPrivApart, _Apart_i, 0 )->LPR );
		for( _i = 0; _i < _lpApart->p; _i++ )
		{
			_Real_i = (int)MGET( ( COBJ_2_MATRIX( lpCobj_All_->LPApartCV ) ), _Apart_i, _i ) - 1;
			WRITECVPARAMETER( lpCobj_All_->LPCV,(WORD)_Real_i, CV_REALQ, REAL, (float)MGET( _lpmQ, _i*lpCobj_All_->P, _i*lpCobj_All_->P ) );
		}
		for( _j = 0; _j < _lpApart->m; _j++ )
		{
			_Real_j = (int)MGET( ( COBJ_2_MATRIX( lpCobj_All_->LPApartMV ) ), _Apart_i, _j ) - 1; 
			MGET( _lpmDeluM, _Real_j, 0 ) = MGET( ( COBJ_2_MATRIX( _lpApart->LPDeluM ) ), _j*_lpApart->M, 0 );
			WRITEMVPARAMETER( lpCobj_All_->LPMV,(WORD)_Real_j, MV_REALR, REAL, (float)MGET( _lpmR, _j*lpCobj_All_->M, _j*lpCobj_All_->M ) );
			WRITEMVPARAMETER( lpCobj_All_->LPMV,(WORD)_Real_j, MV_REALS, REAL, (float)MGET( _lpmP, _j*lpCobj_All_->M, _j*lpCobj_All_->M ) );
		}
	}
	for( _i = 0; _i < lpCobj_All_->p; _i++ )
	{
		PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CRControl_type = 
			PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CControl_type;
		_Control_type = PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CCM;
		if( PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CInvalid == 1 )
			_Control_type = 4;
		if( _Control_type == 0 )
        	_Control_type = 2;
		PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CLastCM = _Control_type;
		if( _Control_type != 4 )
		{
			if( _Control_type == 1 )
			{
/*				if( lpCobj_All_->Flag_RDebug )
				{
					_TempSet = PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CSetpoint;
					READCVPARAMETER( lpCobj_All_->LPCV, _i, CV_SET, REAL, _LastSet );
					PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CSetpoint = _TempSet;

				}
				else
				{
					_LastSet = PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CSetpoint;
					PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CLastSet = _LastSet;
				}
				if( PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CLastSet == 
						_LastSet )
					WRITECVPARAMETER( lpCobj_All_->LPCV, _i, CV_SET,
						REAL, (REAL)PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CSetpoint ); ////		191-3598055*/
			}
			else
			{
				PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CLastHigh =
					PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CHigh;
				PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CLastLow =
					PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CLow;
				WRITECVPARAMETER( lpCobj_All_->LPCV, _i, CV_SET,
					REAL, (REAL)PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CSetpoint ); ////		191-3598055
			}
		}
		WRITECVPARAMETER( lpCobj_All_->LPCV, _i, CV_LPOUT, 	    REAL,
			(REAL)PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CSteadOptim );
//		WRITECVPARAMETER( lpCobj_All_->LPCV, _i, CV_PRIV, 		LPCOBJ,
//			PRIVCV_2_COBJ( PRIVCVGET( lpCobj_PrivCV_, _i, 0 ) ) );
		WRITECVPARAMETER( lpCobj_All_->LPCV, _i, CV_RCONTROLMODE,
			WORD, PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CRControl_type ); ////		191-3598055
		WRITECVPARAMETER( lpCobj_All_->LPCV, _i, CV_ERROR,
			REAL, (REAL)( PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CError) ); ////		191-3598055
		WRITECVPARAMETER( lpCobj_All_->LPCV, _i, CV_HOVERCOUNT, WORD, PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CHOverCount );
		WRITECVPARAMETER( lpCobj_All_->LPCV, _i, CV_LOVERCOUNT, WORD, PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CLOverCount );
		WRITECVPARAMETER( lpCobj_All_->LPCV, _i, CV_LASTSET,
			REAL, (REAL)(PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CLastSet) ); ////		191-3598055
		WRITECVPARAMETER( lpCobj_All_->LPCV, _i, CV_LASTHIGH,
			REAL, (REAL)PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CLastHigh ); ////		191-3598055
		WRITECVPARAMETER( lpCobj_All_->LPCV, _i, CV_LASTLOW,
			REAL, (REAL)PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CLastLow ); ////		191-3598055
		WRITECVPARAMETER( lpCobj_All_->LPCV, _i, CV_HEDGE,
			BOOL, PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CHEdge ); ////		191-3598055
		WRITECVPARAMETER( lpCobj_All_->LPCV, _i, CV_LEDGE,
			BOOL, PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CLEdge ); 
		WRITECVPARAMETER( lpCobj_All_->LPCV, _i, CV_STEADYDELT,
			REAL, (REAL)PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CSteadyDelt ); 
		WRITECVPARAMETER( lpCobj_All_->LPCV, _i, CV_ALARM,
			BOOL, PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CAlarm ); 
		WRITECVPARAMETER( lpCobj_All_->LPCV, _i, CV_HOVER,
			BOOL, PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CHOver ); 
		WRITECVPARAMETER( lpCobj_All_->LPCV, _i, CV_LOVER,
			BOOL, PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CLOver ); 
		WRITECVPARAMETER( lpCobj_All_->LPCV, _i, CV_NOLP,
			WORD, PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CNoLP ); 
		WRITECVPARAMETER( lpCobj_All_->LPCV, _i, CV_NOILL, 
			BOOL, PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CNoIll ); 
		WRITECVPARAMETER( lpCobj_All_->LPCV, _i, CV_LPWRONG, 
			BOOL, PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CLPWrong ); 
	}
	for( _j = 0; _j < lpCobj_All_->m; _j++ )
	{
		PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MRControl_type = 
			PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MControl_type;
		_Control_type = PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MCM;
		if( PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MInvalidate == 1 )
			_Control_type = 4;
		if( _Control_type == 0 )
			_Control_type = 2;
		if( PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MSetSatuation == 1 )
			_Control_type = 5;
		PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MLastCM = _Control_type;
		if( _Control_type != 4 )
		{
			if( _Control_type == 1 )
			{
				if( lpCobj_All_->Flag_RDebug )
				{
					_TempIrv = PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MIrv;
					READMVPARAMETER( lpCobj_All_->LPMV, _j, MV_SET, 	REAL, _LastSet );
					PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MIrv = _TempIrv;
				}
				else
				{
					_LastSet = PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MIrv;
					PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MLastIRV = _LastSet;
				}
				if(	PRIVMVGET( lpCobj_PrivMV_, _j, 0)->MLastIRV == 
					 _LastSet )
					WRITEMVPARAMETER( lpCobj_All_->LPMV, _j, MV_SET, 		REAL,
						(REAL)PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MIrv );
			}
			else
			{
				if( _Control_type == 2 )
				{
					PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MLastHigh =
					PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MIrvHigh;
					PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MLastLow =
						PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MIrvLow;
					WRITEMVPARAMETER( lpCobj_All_->LPMV, _j, MV_SET, 		REAL,
						(REAL)PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MIrv );
				}
				else
					if( _Control_type == 5 )
					{
						//if( PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MIrv != 
						//	PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MLastIrv )
					}
			}
		}

		WRITEMVPARAMETER( lpCobj_All_->LPMV, _j, MV_LPOUT, 		REAL,
			(REAL)PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MSteadOptim );
		if( !lpCobj_All_->Flag_Alarm )
			_u = PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MSample +
				MGET( _lpmDeluM, _j, 0 ) *
					( PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MHScale -
						PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MLScale );
		else
			_u = PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MSample;
	//	WRITEMVPARAMETER( lpCobj_All_->LPMV, _j, MV_PRIV, 	LPCOBJ,
	//			PRIVMV_2_COBJ( PRIVMVGET( lpCobj_PrivMV_, _j, 0 ) ) );
		
		if( lpCobj_All_->Flag_AM >= 1 && PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MAM == 1 && 
			PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MApartAM == 1 )
		{
			READMVPARAMETER( lpCobj_All_->LPMV, _j, MV_DIST, BOOL, PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MDist );
			if( !( PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MDist ) )
			{
				WRITEMVPARAMETER( lpCobj_All_->LPMV, _j, MV_VALUE, 	REAL,  (float)_u );
//????????
				PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MData = _u;
			}
			else
			{
				_u = PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MData;
				WRITEMVPARAMETER( lpCobj_All_->LPMV, _j, MV_VALUE, 	REAL,  (float)_u );
				PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MDelt = 
					PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MData -
						PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MSample;
				PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MDelt /= 
					( PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MHScale - PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MLScale );

			}
		}
		else
		{
//#ifdef __Simul
//			READMVPARAMETER( lpCobj_All_->LPMV, _j, MV_VALUE, 	REAL, 
//				PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MData  );
//#else
			_FS = 0;
#ifdef __Simul
#ifdef __FIX
			_FS = 1;
#endif
#endif
			if( !_FS )
				READMVPARAMETER( lpCobj_All_->LPMV, _j, MV_SAMPLE, 	REAL, 
					PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MData  );
//#endif
/*			if( PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MData > 
					PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MMax )
				PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MData = 
					PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MMax;
			if( PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MData < 
					PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MMin )
				PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MData = 
					PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MMin;*/
			WRITEMVPARAMETER( lpCobj_All_->LPMV, _j, MV_VALUE, 	REAL,  
				(REAL)PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MData );
		}


		// PROBLEM HERE 
		//WRITEMVPARAMETER( lpCobj_All_->LPMV, _j, MV_INI, 		REAL,  (REAL)PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MIni );
		WRITEMVPARAMETER( lpCobj_All_->LPMV, _j, MV_RCONTROLMODE, 	WORD,  PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MRControl_type );
		WRITEMVPARAMETER( lpCobj_All_->LPMV, _j, MV_LOVERCOUNT, 		BOOL,  PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MLOverCount );
		WRITEMVPARAMETER( lpCobj_All_->LPMV, _j, MV_HOVERCOUNT, 		WORD,  PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MHOverCount );
		WRITEMVPARAMETER( lpCobj_All_->LPMV, _j, MV_HEDGE, 			BOOL,  PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MHEdge );
		WRITEMVPARAMETER( lpCobj_All_->LPMV, _j, MV_LEDGE, 			BOOL,  PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MLEdge );
		WRITEMVPARAMETER( lpCobj_All_->LPMV, _j, MV_ALARM, 			BOOL,  PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MAlarm );
		WRITEMVPARAMETER( lpCobj_All_->LPMV, _j, MV_SATUATION, 		BOOL,  PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MSatuation );
		WRITEMVPARAMETER( lpCobj_All_->LPMV, _j, MV_IRVWRONG, 		BOOL,  PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MIRVWrong );
		WRITEMVPARAMETER( lpCobj_All_->LPMV, _j, MV_HOPEAM, 		BOOL,  PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MHope_AM );
	}
	lpCobj_All_->Count++;
	WRITEALLPARAMETER( lpCobj_All_, MDMCIRVD_count, 		WORD, lpCobj_All_->Count );
	WRITEALLPARAMETER( lpCobj_All_, MDMCIRVD_FLAGMCHANGE,	BOOL, 0 );
	WRITEALLPARAMETER( lpCobj_All_, MDMCIRVD_FLAGLPCHANGE,BOOL, 0 );
	WRITEALLPARAMETER( lpCobj_All_, MDMCIRVD_FLAGALARM,BOOL, lpCobj_All_->Flag_Alarm );
//	WRITEALLPARAMETER( lpCobj_All_, MDMCIRVD_CORSTEADY,REAL, (REAL)lpCobj_All_->Cor_Steady );
	WRITEALLPARAMETER( lpCobj_All_, MDMCIRVD_P,		WORD, lpCobj_All_->P );
	WRITEALLPARAMETER( lpCobj_All_, MDMCIRVD_M,		WORD, lpCobj_All_->M );
	WRITEALLPARAMETER( lpCobj_All_, MDMCIRVD_p,		WORD, lpCobj_All_->p );
	WRITEALLPARAMETER( lpCobj_All_, MDMCIRVD_m,		WORD, lpCobj_All_->m );
	WRITEALLPARAMETER( lpCobj_All_, MDMCIRVD_nd,	WORD, lpCobj_All_->nd );
	WRITEALLPARAMETER( lpCobj_All_, MDMCIRVD_FILENO,WORD, lpCobj_All_->File_No );
	WRITEALLPARAMETER( lpCobj_All_, MDMCIRVD_N,		WORD, lpCobj_All_->N );
	WRITEALLPARAMETER( lpCobj_All_, MDMCIRVD_NOAPART,		WORD, lpCobj_All_->No_Apart );
	WRITEALLPARAMETER( lpCobj_All_, MDMCIRVD_FREECOUNT,		int, lpCobj_All_->Freecount );
	WRITEALLPARAMETER( lpCobj_All_, MDMCIRVD_FLAGHOPEAM,		BOOL, lpCobj_All_->Flag_HopeAM );
	WRITEALLPARAMETER( lpCobj_All_, MDMCIRVD_FLAGLPRECOUNT,		BOOL, lpCobj_All_->Flag_LPRecount );
	WRITEALLPARAMETER( lpCobj_All_, MDMCIRVD_FLAGRECOUNT,		BOOL, lpCobj_All_->Flag_Recount );
	WRITEALLPARAMETER( lpCobj_All_, MDMCIRVD_STARTMODE,		BOOL, lpCobj_All_->StartMode );
	WRITEALLPARAMETER( lpCobj_All_, MDMCIRVD_SRMODE,		BOOL, lpCobj_All_->Flag_SRMode );
//	WRITEALLPARAMETER( lpCobj_All_, MDMCIRVD_MCLASSIF,		BOOL, lpCobj_All_->Flag_MControl );
//	WRITEALLPARAMETER( lpCobj_All_, MDMCIRVD_CCLASSIF,		BOOL, lpCobj_All_->Flag_CControl );

	if( !lpCobj_All_->Flag_RDebug )
	{
		READALLPARAMETER( lpCobj_All_, MDMCIRVD_FLAGRUN,  	BOOL, lpCobj_All_->Flag_Run );
		READALLPARAMETER( lpCobj_All_, MDMCIRVD_FLAGAM1,  	BOOL, lpCobj_All_->Flag_AM1 );
		READALLPARAMETER( lpCobj_All_, MDMCIRVD_FLAGAM2,  	BOOL, lpCobj_All_->Flag_AM2 );
		READALLPARAMETER( lpCobj_All_, MDMCIRVD_FLAGAM3,  	BOOL, lpCobj_All_->Flag_AM3 );
		READALLPARAMETER( lpCobj_All_, MDMCIRVD_FLAGAM,  	BOOL, lpCobj_All_->Flag_AM );
		READALLPARAMETER( lpCobj_All_, MDMCIRVD_FLAGDEBUG,  	BOOL, lpCobj_All_->Flag_Debug );
		WRITEALLSTRUCT( lpCobj_All_ );
		for(_i=0;_i< lpCobj_All_->m;_i++)
		{	
			READMVPARAMETER( lpCobj_All_->LPMV, _i, MV_CONTROLMODE, 	BOOL,  
				PRIVMVGET( lpCobj_PrivMV_, _i, 0 )->MControl_type );
			if( PRIVMVGET( lpCobj_PrivMV_, _i, 0 )->MControl_type == 1 )
			{	
				READMVPARAMETER( lpCobj_All_->LPMV, _i, MV_SET,		REAL,  
					PRIVMVGET( lpCobj_PrivMV_, _i, 0 )->MIrv );
			}
			READMVPARAMETER( lpCobj_All_->LPMV, _i, MV_HIGH,		REAL,  
				PRIVMVGET( lpCobj_PrivMV_, _i, 0 )->MIrvHigh );
			READMVPARAMETER( lpCobj_All_->LPMV, _i, MV_LOW,		REAL,  
				PRIVMVGET( lpCobj_PrivMV_, _i, 0 )->MIrvLow );
//			PRIVMVGET( lpCobj_PrivMV_, _i, 0 )->MControl_type = 
//				PRIVMVGET( lpCobj_PrivMV_, _i, 0 )->MCM;
			WRITEMVSTRUCT( lpCobj_All_, lpCobj_PrivMV_,_i);
		}
		for(_i=0;_i<lpCobj_All_->p;_i++)
		{
			READCVPARAMETER( lpCobj_All_->LPCV, _i, CV_CONTROLMODE, 	BOOL,  
				PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CControl_type );
//			PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CControl_type = 
//				PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CCM;
			if( PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CControl_type == 1 )
			{
				READCVPARAMETER( lpCobj_All_->LPCV, _i, CV_SET,		REAL,  
					PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CSetpoint );
			}
			READCVPARAMETER( lpCobj_All_->LPCV, _i, CV_HIGH,		REAL,  
				PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CHigh );
			READCVPARAMETER( lpCobj_All_->LPCV, _i, CV_LOW,		REAL,  
				PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CLow );
			WRITECVSTRUCT( lpCobj_All_, lpCobj_PrivCV_,_i);
		}
		for(_i=0;_i<lpCobj_All_->nd;_i++)
			WRITEDVSTRUCT( lpCobj_All_, lpCobj_PrivDV_,_i);
	}
	if( lpCobj_All_->Flag_Fault == 0 )
	{
		for( _i = 0; _i < lpCobj_All_->p; _i++ )
		{
			WRITECVPARAMETER( lpCobj_All_->LPCV, _i, CV_FMAX, REAL, (REAL)PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CFault_max );
			WRITECVPARAMETER( lpCobj_All_->LPCV, _i, CV_FMIN, REAL, (REAL)PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CFault_min );
			WRITECVPARAMETER( lpCobj_All_->LPCV, _i, CV_FHIGH, REAL, (REAL)PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CFault_high );
			WRITECVPARAMETER( lpCobj_All_->LPCV, _i, CV_FLOW, REAL, (REAL)PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CFault_low );
			WRITECVPARAMETER( lpCobj_All_->LPCV, _i, CV_FXSET, REAL, (REAL)PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CFault_xsetpoint );
			WRITECVPARAMETER( lpCobj_All_->LPCV, _i, CV_FNSET, REAL, (REAL)PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CFault_nsetpoint );
			WRITECVPARAMETER( lpCobj_All_->LPCV, _i, CV_FHSET, REAL, (REAL)PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CFault_hsetpoint );
			WRITECVPARAMETER( lpCobj_All_->LPCV, _i, CV_FLSET, REAL, (REAL)PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CFault_lsetpoint );
		}
		for( _j = 0; _j < lpCobj_All_->m; _j++ )
		{
			WRITEMVPARAMETER( lpCobj_All_->LPMV, _j, MV_FMAX, REAL, (REAL)PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MFault_max );
			WRITEMVPARAMETER( lpCobj_All_->LPMV, _j, MV_FMIN, REAL, (REAL)PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MFault_min );
			WRITEMVPARAMETER( lpCobj_All_->LPMV, _j, MV_FHIGH, REAL, (REAL)PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MIrvFault_high );
			WRITEMVPARAMETER( lpCobj_All_->LPMV, _j, MV_FLOW, REAL, (REAL)PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MIrvFault_low );
			WRITEMVPARAMETER( lpCobj_All_->LPMV, _j, MV_FXSET, REAL, (REAL)PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MFault_xirv );
			WRITEMVPARAMETER( lpCobj_All_->LPMV, _j, MV_FNSET, REAL, (REAL)PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MFault_nirv );
			WRITEMVPARAMETER( lpCobj_All_->LPMV, _j, MV_FHSET, REAL, (REAL)PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MFault_hirv );
			WRITEMVPARAMETER( lpCobj_All_->LPMV, _j, MV_FLSET, REAL, (REAL)PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MFault_lirv );
		}
	}
#ifdef __Simul
	for( _j = 0; _j < lpCobj_All_->m; _j++ )
	{
		if( !( PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MDist ) )
		{
			PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MSample =
				PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MData;
			WRITEMVPARAMETER( lpCobj_All_->LPMV, _j, MV_SAMPLE, REAL, 
				(REAL)PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MSample );
		}
	}
#endif
	MATRIXFREE( _lpmDeluM);
	for( _i = 0; _i < lpCobj_All_->p; _i++ )
	{
		RESETMCVDATA( PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CHOverCount, MAXNUMBER, RESETNUMBER );
		RESETMCVDATA( PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CLOverCount, MAXNUMBER, RESETNUMBER );
		WRITECVPARAMETER( lpCobj_All_->LPCV, _i, CV_HOVERCOUNT, WORD, PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CHOverCount );
		WRITECVPARAMETER( lpCobj_All_->LPCV, _i, CV_LOVERCOUNT, WORD, PRIVCVGET( lpCobj_PrivCV_, _i, 0 )->CLOverCount );
	}
	for( _j = 0; _j < lpCobj_All_->m; _j++ )
	{
		RESETMCVDATA( PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MHOverCount, MAXNUMBER, RESETNUMBER );
		RESETMCVDATA( PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MLOverCount, MAXNUMBER, RESETNUMBER );
		WRITEMVPARAMETER( lpCobj_All_->LPMV, _j, MV_HOVERCOUNT, WORD, PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MHOverCount );
		WRITEMVPARAMETER( lpCobj_All_->LPMV, _j, MV_LOVERCOUNT, WORD, PRIVMVGET( lpCobj_PrivMV_, _j, 0 )->MLOverCount );
	}
	RESETGDATA( lpCobj_All_->Count, MAXNUMBER, RESETNUMBER, &_bReset )
	RESETGDATA( lpCobj_All_->LPCount, MAXNUMBER, RESETNUMBER, &_bReset )
	WRITEALLPARAMETER( lpCobj_All_, MDMCIRVD_count, WORD, lpCobj_All_->Count );
	WRITEALLPARAMETER( lpCobj_All_, MDMCIRVD_LPCOUNT, WORD, lpCobj_All_->LPCount );
	_bReset = 0;
	RESETGDATA( lpCobj_All_->RecountCount, MAXNUMBER, RESETNUMBER, &_bReset )
	if( _bReset )
	{
		for( _Apart_i = 0; _Apart_i < lpCobj_All_->No_Apart - 1; _Apart_i++ )	
		{
			APARTGET( _lpPrivApart, _Apart_i, 0 )->RecountCount = lpCobj_All_->RecountCount / lpCobj_All_->No_Apart;
		}
		_RecountCount = lpCobj_All_->RecountCount / lpCobj_All_->No_Apart * ( lpCobj_All_->No_Apart - 1 );
		APARTGET( _lpPrivApart, ( lpCobj_All_->No_Apart - 1 ), 0 )->RecountCount = lpCobj_All_->RecountCount - _RecountCount;
	}
	_bReset = 0;
	RESETGDATA( lpCobj_All_->MakeParaCount, MAXNUMBER, RESETNUMBER, &_bReset )
	if( _bReset )
	{
		for( _Apart_i = 0; _Apart_i < lpCobj_All_->No_Apart - 1; _Apart_i++ )	
		{
			APARTGET( _lpPrivApart, _Apart_i, 0 )->MakeParaCount = lpCobj_All_->MakeParaCount / lpCobj_All_->No_Apart;
		}
		_MakeParaCount = lpCobj_All_->MakeParaCount / lpCobj_All_->No_Apart * ( lpCobj_All_->No_Apart - 1 );
		APARTGET( _lpPrivApart, ( lpCobj_All_->No_Apart - 1 ), 0 )->MakeParaCount = lpCobj_All_->MakeParaCount - _MakeParaCount;
	}
	_bReset = 0;
	RESETGDATA( lpCobj_All_->MParaCount, MAXNUMBER, RESETNUMBER, &_bReset )
	if( _bReset )
	{
		for( _Apart_i = 0; _Apart_i < lpCobj_All_->No_Apart - 1; _Apart_i++ )	
		{
			APARTGET( _lpPrivApart, _Apart_i, 0 )->MParaCount = lpCobj_All_->MParaCount / lpCobj_All_->No_Apart;
		}
		_MParaCount = lpCobj_All_->MParaCount / lpCobj_All_->No_Apart * ( lpCobj_All_->No_Apart - 1 );
		APARTGET( _lpPrivApart, ( lpCobj_All_->No_Apart - 1 ), 0 )->MParaCount = lpCobj_All_->MParaCount - _MParaCount;
	}
	WRITEALLPARAMETER( lpCobj_All_, MDMCIRVD_MPARACOUNT,		WORD, lpCobj_All_->MParaCount );
	WRITEALLPARAMETER( lpCobj_All_, MDMCIRVD_MAKEPARACOUNT,		WORD, lpCobj_All_->MakeParaCount );
	WRITEALLPARAMETER( lpCobj_All_, MDMCIRVD_RECOUNTCOUNT,		WORD, lpCobj_All_->RecountCount );

	WRITEALLPARAMETER( lpCobj_All_, MDMCIRVD_ERRORCODE,			WORD, lpCobj_All_->ErrorCode );
	
	return ;
} 