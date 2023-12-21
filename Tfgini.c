// 程序初始化运行，完成程序整体所需内存的分配

#include "tfalgo.h"
#include "tfcmdmc.h"
#include "tfmat.h"
#ifdef __ONSPEC
#include "Mtmfun.h"
#include "Mtdllfun.h"
#endif

// __SJZ or __SJZ__?
#ifndef __SJZ__
#define __SJZ__
#endif
#ifdef debug
#include "tfdebug.h"
#endif

extern HINSTANCE ghInst;
extern char	LocalDirectory[128];

ACRET GlobalInitiate(LPCOBJPRIV lpCobjPrivAll_ )
{
//The followings modified by Chen 1-7-98
 	WORD			_N, _P, _M, _pp, _mm, _nd;
	WORD			_i, _j, _kk;
	LPCOBJ			_lpCobj_SR, _lpCobj_DSR;
	LPCOBJ			_lpCobj_MV, _lpCobj_CV, _lpCobj_DV;
	char _szPathBuffer[_MAX_PATH];
	char _szDrive[_MAX_DRIVE];
	char _szDir[_MAX_DIR];
	char _szFName[_MAX_FNAME];
	char _szExt[_MAX_EXT];
#ifdef __ONSPEC
	char			_GlobalFile[128], _szBuffer[128];
#endif
	double          _Ini;
	LPCOBJ			_lpCobj_Priv;
	LPDIMMATRIX		_lpDimSR, _lpDimDSR;
	LPCOBJPRIVCV	_lpCobj_PrivCV;
	LPCOBJPRIVMV	_lpCobj_PrivMV;
	LPCOBJPRIVDV	_lpCobj_PrivDV;
	BOOL			_Okay = TRUE;
	LPCOBJ			_lpCobj_OutSR, _lpCobj_OutDSR;
	LPMATRIX		_lpmG, _lpmLPPriv;
	LPCOBJ			_lpCobj_LPPriv;
// ZZK would get the global parameters here, and the module directory is determined here
#ifdef __ZZK
	Display_2_PrivAll( lpCobjPrivAll_ );
	lpCobjPrivAll_->Flag_Run = 1;
	WRITEALLPARAMETER( lpCobjPrivAll_, MDMCIRVD_FLAGRUN, BOOL, 1 );
#endif
    GetModuleFileName( ghInst, _szPathBuffer, sizeof(_szPathBuffer) );
	_splitpath( _szPathBuffer, _szDrive, _szDir, _szFName, _szExt );
	strcpy( LocalDirectory, _szDrive );
//.DLL is not in the same directory with .EXE
	//	strcat( LocalDirectory, _szDir );
	strcat( LocalDirectory, _szDir );
	//Here get the location of the initial file
//	global parameters from the file or ONSPEC
//ZZK would get this flag from the control display board

#ifdef __ONSPEC
	strcpy( _GlobalFile, LocalDirectory ); 
	strcat( _GlobalFile, "tfmcc.ini" );
	GetPrivateProfileString( "MCC",
							 "StartMode",
							 "0",
							 _szBuffer,
							 sizeof(_szBuffer),
							_GlobalFile );
	sscanf( _szBuffer, "%d", &( lpCobjPrivAll_->StartMode ) ); 
#endif
#ifdef __ONSPEC
	if( lpCobjPrivAll_->StartMode != 1 ) 
	{
		Read_FileAll( lpCobjPrivAll_ );
		lpCobjPrivAll_->Flag_Run = 1;
		WRITEALLSTRUCT( lpCobjPrivAll_ );
	}
	else
	{
		lpCobjPrivAll_->Flag_Run = 1;
		WRITEALLPARAMETER( lpCobjPrivAll_, MDMCIRVD_FLAGRUN, 		BOOL, 1 );
		READALLSTRUCT( lpCobjPrivAll_ );
	}
#endif
	READALLPARAMETER( lpCobjPrivAll_, MDMCIRVD_MV, LPCOBJ, lpCobjPrivAll_->LPMV );
	_lpCobj_PrivMV = COBJ_2_PRIVMV( lpCobjPrivAll_->LPMV );
	READALLPARAMETER( lpCobjPrivAll_, MDMCIRVD_CV, LPCOBJ, lpCobjPrivAll_->LPCV );
	READALLPARAMETER( lpCobjPrivAll_, MDMCIRVD_DV, LPCOBJ, lpCobjPrivAll_->LPDV );
	_mm = lpCobjPrivAll_->m;
	_pp = lpCobjPrivAll_->p;
	_nd = lpCobjPrivAll_->nd;
	_M = lpCobjPrivAll_->M;
	_P = lpCobjPrivAll_->P;
	_N = lpCobjPrivAll_->N;
	if( _mm == 0 || _pp == 0 )
		goto _BAD;

	_lpCobj_PrivCV = PrivCVAlloc( _pp, 1 );	
	_lpCobj_PrivMV = PrivMVAlloc( _mm, 1 ); 
	if( _nd == 0 )
		_lpCobj_PrivDV = NULL;
	else
		_lpCobj_PrivDV = PrivDVAlloc( _nd, 1 );
#ifdef __ONSPEC
	WRITEALLPARAMETER( lpCobjPrivAll_, MDMCIRVD_CV, LPCOBJ, PRIVCV_2_COBJ(_lpCobj_PrivCV ) );
	WRITEALLPARAMETER( lpCobjPrivAll_, MDMCIRVD_MV, LPCOBJ, PRIVMV_2_COBJ(_lpCobj_PrivMV ) );
	WRITEALLPARAMETER( lpCobjPrivAll_, MDMCIRVD_DV, LPCOBJ, PRIVDV_2_COBJ(_lpCobj_PrivDV ) );
	WRITEALLPARAMETER( lpCobjPrivAll_, MDMCIRVD_INCV, LPCOBJ, PRIVCV_2_COBJ(_lpCobj_PrivCV ) );
	WRITEALLPARAMETER( lpCobjPrivAll_, MDMCIRVD_INMV, LPCOBJ, PRIVMV_2_COBJ(_lpCobj_PrivMV ) );
	WRITEALLPARAMETER( lpCobjPrivAll_, MDMCIRVD_INDV, LPCOBJ, PRIVDV_2_COBJ(_lpCobj_PrivDV ) );
#endif
	for( _i = 0; _i < _pp; _i++ )
	{
		_lpCobj_Priv = CobjAlloc( IDCOBJ_MATRIX, sizeof( COBJ ) + sizeof( VARCV ) );
		PRIVCVGET( _lpCobj_PrivCV, _i, 0 ) = ( LPVARCV )((void far*)&( _lpCobj_Priv ->data[0]));
		WRITECVPARAMETER( lpCobjPrivAll_->LPCV, _i, CV_PRIV, LPCOBJ,  _lpCobj_Priv );
		PRIVCVGET( _lpCobj_PrivCV, _i, 0 )->LPPriv = _lpCobj_Priv;
	}
	for( _j = 0; _j < _mm; _j++ )
	{
		_lpCobj_Priv = CobjAlloc( IDCOBJ_MATRIX, sizeof( COBJ ) + sizeof( VARMV ) );
		PRIVMVGET( _lpCobj_PrivMV, _j, 0 ) = ( LPVARMV)((void far*)&( _lpCobj_Priv ->data[0]));
		WRITEMVPARAMETER( lpCobjPrivAll_->LPMV, _j, MV_PRIV, LPCOBJ, _lpCobj_Priv );
		PRIVMVGET( _lpCobj_PrivMV, _j, 0 )->LPPriv = _lpCobj_Priv;
	}
	for( _kk = 0; _kk < _nd; _kk++ )
	{
		_lpCobj_Priv = CobjAlloc( IDCOBJ_MATRIX, sizeof( COBJ ) + sizeof( VARDV ) );
		PRIVDVGET( _lpCobj_PrivDV, _kk, 0 ) = ( LPVARDV )((void far*)&( _lpCobj_Priv ->data[0]));
		WRITEDVPARAMETER( lpCobjPrivAll_->LPDV, _kk, DV_PRIV, LPCOBJ, _lpCobj_Priv );
		PRIVDVGET( _lpCobj_PrivDV, _kk, 0 )->LPPriv = _lpCobj_Priv;
	}
	_lpCobj_MV = PRIVMV_2_COBJ( _lpCobj_PrivMV );
	_lpCobj_CV = PRIVCV_2_COBJ( _lpCobj_PrivCV );
	_lpCobj_DV = PRIVDV_2_COBJ( _lpCobj_PrivDV);
//ONSPEC would not use board-structure, but ZZK would
	WRITEALLPARAMETER(lpCobjPrivAll_, MDMCIRVD_INMV,			LPCOBJ, _lpCobj_MV );
	WRITEALLPARAMETER(lpCobjPrivAll_, MDMCIRVD_INCV,			LPCOBJ, _lpCobj_CV );
	WRITEALLPARAMETER(lpCobjPrivAll_, MDMCIRVD_INDV,			LPCOBJ, _lpCobj_DV );
	WRITEALLPARAMETER(lpCobjPrivAll_, MDMCIRVD_FLAGALARM,	BOOL, 0 );
	lpCobjPrivAll_->Flag_Alarm = 0;

//  ZZK has saved all the parameters of MV, CV, DV, so it needn't to be read again
//  But should be done again on ONSPEC platform
	if( lpCobjPrivAll_->StartMode != 1 ) 
	{
		Read_FileMV( lpCobjPrivAll_, _lpCobj_PrivMV );
		Read_FileCV( lpCobjPrivAll_, _lpCobj_PrivCV );
		Read_FileDV( lpCobjPrivAll_, _lpCobj_PrivDV );
#ifdef __ONSPEC
		
		for(_i=0;_i<lpCobjPrivAll_->m;_i++)
		{
#ifndef __Simul
			READMVPARAMETER( lpCobjPrivAll_->LPMV, _i, MV_SAMPLE, REAL, PRIVMVGET( _lpCobj_PrivMV, _i, 0 )->MSample );

//because the 5th MV of A_R System in SJZ has no sample data...
/*  zck date: 2001-3-21
#ifdef __SJZ__
			if( _i == 5 && ( PRIVMVGET( _lpCobj_PrivMV, _i, 0 )->MSample == 0.0 ||
								PRIVMVGET( _lpCobj_PrivMV, _i, 0 )->MSample == 100.0 ) )
				PRIVMVGET( _lpCobj_PrivMV, _i, 0 )->MSample = 50.0;
#endif
*/
			PRIVMVGET( _lpCobj_PrivMV, _i, 0 )->MData = PRIVMVGET( _lpCobj_PrivMV, _i, 0 )->MSample;
			PRIVMVGET( _lpCobj_PrivMV, _i, 0 )->MIni = PRIVMVGET( _lpCobj_PrivMV, _i, 0 )->MSample;
#else
			PRIVMVGET( _lpCobj_PrivMV, _i, 0 )->MSample = PRIVMVGET( _lpCobj_PrivMV, _i, 0 )->MIni;
#endif
			if( PRIVMVGET( _lpCobj_PrivMV, _i, 0 )->MControl_type != 1 )
				PRIVMVGET( _lpCobj_PrivMV, _i, 0 )->MControl_type = 0;
			PRIVMVGET( _lpCobj_PrivMV, _i, 0 )->MData = 
				PRIVMVGET( _lpCobj_PrivMV, _i, 0 )->MSample;
			WRITEMVSTRUCT( lpCobjPrivAll_, _lpCobj_PrivMV,_i);
		}
		for(_i=0;_i<lpCobjPrivAll_->p;_i++)
		{
#ifndef __Simul
			READCVPARAMETER( lpCobjPrivAll_->LPCV, _i, CV_VALUE, REAL, PRIVCVGET( _lpCobj_PrivCV, _i, 0 )->CData );
			PRIVCVGET( _lpCobj_PrivCV, _i, 0 )->CIni = PRIVCVGET( _lpCobj_PrivCV, _i, 0 )->CData;
#else
			PRIVCVGET( _lpCobj_PrivCV, _i, 0 )->CData = PRIVCVGET( _lpCobj_PrivCV, _i, 0 )->CIni;
#endif			
			if( PRIVCVGET( _lpCobj_PrivCV, _i, 0 )->CControl_type !=  1 )
				PRIVCVGET( _lpCobj_PrivCV, _i, 0 )->CControl_type = 0;
			WRITECVSTRUCT( lpCobjPrivAll_, _lpCobj_PrivCV,_i);
		}
		for(_i=0;_i<lpCobjPrivAll_->nd;_i++)
		{
#ifndef __Simul
			READDVPARAMETER( lpCobjPrivAll_->LPDV, _i, DV_VALUE, REAL, PRIVDVGET( _lpCobj_PrivDV, _i, 0 )->DData );
#endif
			WRITEDVSTRUCT( lpCobjPrivAll_, _lpCobj_PrivDV,_i);
		}
#endif
	}
#ifdef __ONSPEC
	else
	{
		for(_i=0;_i<lpCobjPrivAll_->m;_i++)
			READMVSTRUCT( lpCobjPrivAll_, _lpCobj_PrivMV, _i );
		for(_i=0;_i<lpCobjPrivAll_->p;_i++)
			READCVSTRUCT( lpCobjPrivAll_, _lpCobj_PrivCV, _i );
		for(_i=0;_i<lpCobjPrivAll_->nd;_i++)
			READDVSTRUCT( lpCobjPrivAll_, _lpCobj_PrivDV, _i );
}
#endif
//Modified by Chen
//
	_lpDimSR = DimAlloc( _pp, _mm, lpCobjPrivAll_->N );
	_lpCobj_SR = MAKE_COBJ( _lpDimSR );
	_lpCobj_OutSR = TFCobjCopy( _lpCobj_SR, &_Okay );
	if( _nd == 0 )
	{
		_lpDimDSR = NULL;
		_lpCobj_DSR = NULL;
		_lpCobj_OutDSR = NULL;
	}
	else
	{	_lpDimDSR = DimAlloc( _pp, _nd, lpCobjPrivAll_->N );
		_lpCobj_DSR = MAKE_COBJ( _lpDimDSR );
		_lpCobj_OutDSR = TFCobjCopy( _lpCobj_DSR, &_Okay );
	}
	WRITEALLPARAMETER(lpCobjPrivAll_, MDMCIRVD_mSR,LPCOBJ,_lpCobj_SR );	
	WRITEALLPARAMETER(lpCobjPrivAll_, MDMCIRVD_mDSR,LPCOBJ,_lpCobj_DSR );	
	WRITEALLPARAMETER(lpCobjPrivAll_, MDMCIRVD_OUTSR,LPCOBJ,_lpCobj_OutSR );	
	WRITEALLPARAMETER(lpCobjPrivAll_, MDMCIRVD_OUTDSR,LPCOBJ,_lpCobj_OutDSR );	
	lpCobjPrivAll_->LPSR = _lpCobj_SR ;
	lpCobjPrivAll_->LPDSR = _lpCobj_DSR ;
	lpCobjPrivAll_->LPOutSR = _lpCobj_OutSR;
	lpCobjPrivAll_->LPOutDSR = _lpCobj_OutDSR;
	if( !lpCobjPrivAll_->Flag_SRMode )
	{
		File_SR( lpCobjPrivAll_ );
		File_Out_SR( lpCobjPrivAll_ );
	}
	else
	{	
		ReadFileSR( lpCobjPrivAll_ );
		ReadFileOutSR( lpCobjPrivAll_ );
	}
	if( _lpCobj_SR == NULL  )
	{
//Modified by Chen		
		lpCobjPrivAll_->Flag_HopeAM = 0;
		lpCobjPrivAll_->Flag_Alarm = 1;
		LogError( "SR Alloc", "tfgini.c, line 227" );
		lpCobjPrivAll_->Flag_Run = 0;
		WRITEALLPARAMETER( lpCobjPrivAll_, MDMCIRVD_FLAGRUN, BOOL, 0 );
		WRITEALLPARAMETER( lpCobjPrivAll_, MDMCIRVD_FLAGALARM,BOOL,1);
		WRITEALLPARAMETER( lpCobjPrivAll_, MDMCIRVD_FLAGHOPEAM,BOOL,0);
		CobjFree( _lpCobj_SR );
		CobjFree( _lpCobj_DSR );
		MessageBox( NULL, "ERROR", "NO SR", MB_OK );
		return ACRET_FALSE;
	}
	lpCobjPrivAll_->Count = 1;
	lpCobjPrivAll_->Flag_HopeAM = 1;
	lpCobjPrivAll_->Flag_AM = 0;
	WRITEALLPARAMETER(lpCobjPrivAll_, MDMCIRVD_FLAGAM,	BOOL, 0 );
	WRITEALLPARAMETER(lpCobjPrivAll_, MDMCIRVD_count,	WORD, 1 );
	WRITEALLPARAMETER(lpCobjPrivAll_, MDMCIRVD_FLAGHOPEAM,	BOOL, 1 );

	// here is just for the simulation, if not, it needn't.
	for( _i = 0; _i < _nd; _i++ )
	{
//Modified by Chen
		READALLPARAMETER( lpCobjPrivAll_, MDMCIRVD_DV, LPCOBJ, _lpCobj_DV );
		READDVPARAMETER( _lpCobj_DV, _i, DV_INI, REAL, _Ini );
		PRIVDVGET( _lpCobj_PrivDV, _i, 0 )->DIni = _Ini;
		WRITEDVPARAMETER( _lpCobj_DV, _i, DV_HISTORY, REAL, (float)_Ini );
		PRIVDVGET( _lpCobj_PrivDV, _i, 0 )->DHistory = _Ini;
	}
	for( _i = 0; _i < _mm; _i++ )
	{
//Modified by Chen
		READALLPARAMETER(lpCobjPrivAll_, MDMCIRVD_MV, LPCOBJ, _lpCobj_MV );
		WRITEMVPARAMETER( _lpCobj_MV, _i, MV_DELT, REAL, 0.0 );
		PRIVMVGET( _lpCobj_PrivMV, _i, 0 )->MDelt = 0.0;
		PRIVMVGET( _lpCobj_PrivMV, _i, 0 )->MApartAM = 1;
		WRITEMVPARAMETER( _lpCobj_MV, _i, MV_MAPARTAM, BOOL, 1 );
	}
	
	_lpmG = MatrixAlloc( lpCobjPrivAll_->p, lpCobjPrivAll_->m );
	_lpmLPPriv	  = MatrixAlloc((4*_pp+3*_mm+2), (_mm+_pp)+1 );
	_lpCobj_LPPriv = MATRIX_2_COBJ( _lpmLPPriv );
	lpCobjPrivAll_->LPG = MATRIX_2_COBJ( _lpmG );
	lpCobjPrivAll_->LPLPPriv = _lpCobj_LPPriv;
	lpCobjPrivAll_->LPMVSort = ( MATRIX_2_COBJ( MatrixAlloc( _mm, 1 ) ) );
	lpCobjPrivAll_->LPCVSort = ( MATRIX_2_COBJ( MatrixAlloc( _pp, 1 ) ) );
	lpCobjPrivAll_->Flag_AM1 = 0;
	lpCobjPrivAll_->Flag_AM2 = 0;
	lpCobjPrivAll_->Flag_AM3 = 0;
	lpCobjPrivAll_->LPHistory = TFCobjCopy( PRIV_2_COBJ( lpCobjPrivAll_ ), &_Okay );
	WRITEALLPARAMETER( lpCobjPrivAll_, MDMCIRVD_HISTORY, LPCOBJ, lpCobjPrivAll_->LPHistory );
	WRITEALLPARAMETER( lpCobjPrivAll_, MDMCIRVD_mG, LPCOBJ, lpCobjPrivAll_->LPG );
	WRITEALLPARAMETER( lpCobjPrivAll_, MDMCIRVD_LPPRIV, LPCOBJ, _lpCobj_LPPriv );
	WRITEALLPARAMETER( lpCobjPrivAll_, MDMCIRVD_FLAGAM1, BOOL, 0 );
	WRITEALLPARAMETER( lpCobjPrivAll_, MDMCIRVD_FLAGAM2, BOOL, 0 );
	WRITEALLPARAMETER( lpCobjPrivAll_, MDMCIRVD_FLAGAM3, BOOL, 0 );
//	WRITEALLPARAMETER( lpCobjPrivAll_, MDMCIRVD_LPSTEADY, LPCOBJ, lpCobjPrivAll_->LPSteady );

	return	ACRET_TRUE;
_BAD:
	lpCobjPrivAll_->Flag_Run = 0;
	WRITEALLPARAMETER( lpCobjPrivAll_, MDMCIRVD_FLAGRUN, BOOL, 0 );

	return	ACRET_FALSE;
}
