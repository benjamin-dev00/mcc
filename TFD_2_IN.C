#include "tfalgo.h"
#include "tfcmdmc.h"

ACRET		Display_2_PrivAll( LPCOBJPRIV lpCobjGlobalAll_ )
{
	LPPU lppu_ = ModuGetParameter( lpCobjGlobalAll_->wModu );

	MFPUREAD( lppu_, MDMCIRVD_FILENO,		WORD,  lpCobjGlobalAll_->File_No );
	MFPUREAD( lppu_, MDMCIRVD_p,  			WORD,  lpCobjGlobalAll_->p );
	MFPUREAD( lppu_, MDMCIRVD_m,  			WORD,  lpCobjGlobalAll_->m );
	MFPUREAD( lppu_, MDMCIRVD_nd,  			WORD, lpCobjGlobalAll_->nd );
	MFPUREAD( lppu_, MDMCIRVD_N,  			WORD, lpCobjGlobalAll_->N );
	MFPUREAD( lppu_, MDMCIRVD_P,  			WORD, lpCobjGlobalAll_->P );
	MFPUREAD( lppu_, MDMCIRVD_M,  			WORD, lpCobjGlobalAll_->M );
	MFPUREAD( lppu_, MDMCIRVD_count,		WORD,  lpCobjGlobalAll_->Count );
	MFPUREAD( lppu_, MDMCIRVD_MAKEPARACOUNT,	WORD,  lpCobjGlobalAll_->MakeParaCount );
	MFPUREAD( lppu_, MDMCIRVD_LPCOUNT,	WORD,  lpCobjGlobalAll_->LPCount );
	MFPUREAD( lppu_, MDMCIRVD_RECOUNTCOUNT,	WORD,  lpCobjGlobalAll_->RecountCount );
	MFPUREAD( lppu_, MDMCIRVD_MPARACOUNT,	WORD,  lpCobjGlobalAll_->MParaCount );
	MFPUREAD( lppu_, MDMCIRVD_NOAPART,	WORD,  lpCobjGlobalAll_->No_Apart );
	MFPUREAD( lppu_, MDMCIRVD_MPARAMAX,	WORD,  lpCobjGlobalAll_->MParaMax );
	MFPUREAD( lppu_, MDMCIRVD_STEADYSTEP,	WORD,  lpCobjGlobalAll_->SteadyStep );
	MFPUREAD( lppu_, MDMCIRVD_ERRORCODE,	WORD,  lpCobjGlobalAll_->ErrorCode );
	MFPUREAD( lppu_, MDMCIRVD_FREECOUNT,	int,  lpCobjGlobalAll_->Freecount );
	MFPUREAD( lppu_, MDMCIRVD_MAXDELAY,	WORD,  lpCobjGlobalAll_->MaxDelay );
	MFPUREAD( lppu_, MDMCIRVD_N1,	WORD,  lpCobjGlobalAll_->N1 );
	MFPUREAD( lppu_, MDMCIRVD_N2,	WORD,  lpCobjGlobalAll_->N2 );
	MFPUREAD( lppu_, MDMCIRVD_STEADYCOUNT,	WORD,  lpCobjGlobalAll_->SteadyCount );
	MFPUREAD( lppu_, MDMCIRVD_CALSTEADYMAX,	WORD,  lpCobjGlobalAll_->CalSteadyMax );
	MFPUREAD( lppu_, MDMCIRVD_APARTI,	WORD,  lpCobjGlobalAll_->Apart_i );
	MFPUREAD( lppu_, MDMCIRVD_BADMAX,	WORD,  lpCobjGlobalAll_->BadMax );
	MFPUREAD( lppu_, MDMCIRVD_FLAGFAULT,  	BOOL, lpCobjGlobalAll_->Flag_Fault );
	MFPUREAD( lppu_, MDMCIRVD_FLAGLP,  		BOOL, lpCobjGlobalAll_->Flag_LP );
	MFPUREAD( lppu_, MDMCIRVD_FLAGAM,  		BOOL, lpCobjGlobalAll_->Flag_AM );
	MFPUREAD( lppu_, MDMCIRVD_FLAGHOPEAM,  	BOOL, lpCobjGlobalAll_->Flag_HopeAM );
	MFPUREAD( lppu_, MDMCIRVD_FLAGALARM,  	BOOL, lpCobjGlobalAll_->Flag_Alarm );
	MFPUREAD( lppu_, MDMCIRVD_FLAGRUN,  	BOOL, lpCobjGlobalAll_->Flag_Run );
	MFPUREAD( lppu_, MDMCIRVD_FLAGRESP,  	BOOL, lpCobjGlobalAll_->Flag_ReSP );
	MFPUREAD( lppu_, MDMCIRVD_SRMODE, 		BOOL, lpCobjGlobalAll_->Flag_SRMode );
	MFPUREAD( lppu_, MDMCIRVD_FLAGMCHANGE, 	BOOL, lpCobjGlobalAll_->Flag_MChange );
	MFPUREAD( lppu_, MDMCIRVD_FLAGLPCHANGE, BOOL, lpCobjGlobalAll_->Flag_LPChange );
	MFPUREAD( lppu_, MDMCIRVD_CCLASSIF,  	BOOL, lpCobjGlobalAll_->Flag_CControl );
	MFPUREAD( lppu_, MDMCIRVD_MCLASSIF,  	BOOL, lpCobjGlobalAll_->Flag_MControl );
	MFPUREAD( lppu_, MDMCIRVD_FLAGRECOUNT,  	BOOL, lpCobjGlobalAll_->Flag_Recount );
	MFPUREAD( lppu_, MDMCIRVD_FLAGLPRECOUNT,  	BOOL, lpCobjGlobalAll_->Flag_LPRecount );
	MFPUREAD( lppu_, MDMCIRVD_STARTMODE,  	BOOL, lpCobjGlobalAll_->StartMode );
	MFPUREAD( lppu_, MDMCIRVD_FLAGWEIGHT,	  	BOOL, lpCobjGlobalAll_->Flag_Weight );
	MFPUREAD( lppu_, MDMCIRVD_FLAGDEBUG,	  	BOOL, lpCobjGlobalAll_->Flag_Debug );
	MFPUREAD( lppu_, MDMCIRVD_FLAGHMODE,	  	BOOL, lpCobjGlobalAll_->Flag_HMode );
	MFPUREAD( lppu_, MDMCIRVD_FLAGAM1,	  	BOOL, lpCobjGlobalAll_->Flag_AM1 );
	MFPUREAD( lppu_, MDMCIRVD_FLAGAM2,	  	BOOL, lpCobjGlobalAll_->Flag_AM2 );
	MFPUREAD( lppu_, MDMCIRVD_FLAGAM3,	  	BOOL, lpCobjGlobalAll_->Flag_AM3 );
	MFPUREAD( lppu_, MDMCIRVD_STEADY,	  	BOOL, lpCobjGlobalAll_->Flag_Steady );
	MFPUREAD( lppu_, MDMCIRVD_MCVRECOUNT,	  	BOOL, lpCobjGlobalAll_->Flag_MCVRecount );
	MFPUREAD( lppu_, MDMCIRVD_FLAGRDEBUG,	  	BOOL, lpCobjGlobalAll_->Flag_RDebug );
	MFPUREAD( lppu_, MDMCIRVD_Save,	  	BOOL, lpCobjGlobalAll_->Flag_Save );
	MFPUREAD( lppu_, MDMCIRVD_INISTART,	  	BOOL, lpCobjGlobalAll_->IniStart );
	MFPUREAD( lppu_, MDMCIRVD_FLAGCORSTEADY,	  	BOOL, lpCobjGlobalAll_->Flag_CorSteady );
	MFPUREAD( lppu_, MDMCIRVD_CALSTEADY,	  	BOOL, lpCobjGlobalAll_->CalSteady );
	MFPUREAD( lppu_, MDMCIRVD_ERROR,	  	BOOL, lpCobjGlobalAll_->Error );
	MFPUREAD( lppu_, MDMCIRVD_LPK,  		REAL, lpCobjGlobalAll_->LpK );
	MFPUREAD( lppu_, MDMCIRVD_alpha,  		REAL, lpCobjGlobalAll_->Alpha );
	MFPUREAD( lppu_, MDMCIRVD_BIGCOND,  	REAL, lpCobjGlobalAll_->Big_Cond );
	MFPUREAD( lppu_, MDMCIRVD_PBROKEN,  	REAL, lpCobjGlobalAll_->PBroken );
	MFPUREAD( lppu_, MDMCIRVD_QBROKEN,  	REAL, lpCobjGlobalAll_->QBroken );
	MFPUREAD( lppu_, MDMCIRVD_RBROKEN,  	REAL, lpCobjGlobalAll_->RBroken );
	MFPUREAD( lppu_, MDMCIRVD_SRERROR,  	REAL, lpCobjGlobalAll_->SRError );
	MFPUREAD( lppu_, MDMCIRVD_DSRERROR,  	REAL, lpCobjGlobalAll_->DSRError );
	MFPUREAD( lppu_, MDMCIRVD_DFILTER,  	REAL, lpCobjGlobalAll_->DFilter );
	MFPUREAD( lppu_, MDMCIRVD_SAMPLETIME,  	REAL, lpCobjGlobalAll_->SampleTime );
	MFPUREAD( lppu_, MDMCIRVD_CORSTEADY,  	REAL, lpCobjGlobalAll_->Cor_Steady );
	
	return	ACRET_TRUE;
}
