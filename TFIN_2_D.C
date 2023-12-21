#include "tfalgo.h"
#include "tfcmdmc.h"
#ifdef __ZZK
ACRET		PrivAll_2_Display( LPCOBJPRIV lpCobjGlobalAll_ )
{
	LPPU lppu_ = ModuGetParameter( lpCobjGlobalAll_->wModu );

	MFPUWRITE( lppu_, MDMCIRVD_FILENO,		WORD,  lpCobjGlobalAll_->File_No );
	MFPUWRITE( lppu_, MDMCIRVD_p,  			WORD,  lpCobjGlobalAll_->p );
	MFPUWRITE( lppu_, MDMCIRVD_m,  			WORD,  lpCobjGlobalAll_->m );
	MFPUWRITE( lppu_, MDMCIRVD_nd,  			WORD, lpCobjGlobalAll_->nd );
	MFPUWRITE( lppu_, MDMCIRVD_N,  			WORD, lpCobjGlobalAll_->N );
	MFPUWRITE( lppu_, MDMCIRVD_P,  			WORD, lpCobjGlobalAll_->P );
	MFPUWRITE( lppu_, MDMCIRVD_M,  			WORD, lpCobjGlobalAll_->M );
	MFPUWRITE( lppu_, MDMCIRVD_count,		WORD,  lpCobjGlobalAll_->Count );
	MFPUWRITE( lppu_, MDMCIRVD_MAKEPARACOUNT,	WORD,  lpCobjGlobalAll_->MakeParaCount );
	MFPUWRITE( lppu_, MDMCIRVD_LPCOUNT,	WORD,  lpCobjGlobalAll_->LPCount );
	MFPUWRITE( lppu_, MDMCIRVD_RECOUNTCOUNT,	WORD,  lpCobjGlobalAll_->RecountCount );
	MFPUWRITE( lppu_, MDMCIRVD_MPARACOUNT,	WORD,  lpCobjGlobalAll_->MParaCount );
	MFPUWRITE( lppu_, MDMCIRVD_NOAPART,	WORD,  lpCobjGlobalAll_->No_Apart );
	MFPUWRITE( lppu_, MDMCIRVD_MPARAMAX,	WORD,  lpCobjGlobalAll_->MParaMax );
	MFPUWRITE( lppu_, MDMCIRVD_STEADYSTEP,	WORD,  lpCobjGlobalAll_->SteadyStep );
	MFPUWRITE( lppu_, MDMCIRVD_ERRORCODE,	WORD,  lpCobjGlobalAll_->ErrorCode );
	MFPUWRITE( lppu_, MDMCIRVD_FREECOUNT,	WORD,  lpCobjGlobalAll_->Freecount );
	MFPUWRITE( lppu_, MDMCIRVD_MAXDELAY,	WORD,  lpCobjGlobalAll_->MaxDelay );
	MFPUWRITE( lppu_, MDMCIRVD_Modu,	WORD,  lpCobjGlobalAll_->wModu );
	MFPUWRITE( lppu_, MDMCIRVD_N1,	WORD,  lpCobjGlobalAll_->N1 );
	MFPUWRITE( lppu_, MDMCIRVD_N2,	WORD,  lpCobjGlobalAll_->N2 );
	MFPUWRITE( lppu_, MDMCIRVD_STEADYCOUNT,	WORD,  lpCobjGlobalAll_->SteadyCount );
	MFPUWRITE( lppu_, MDMCIRVD_CALSTEADYMAX,	WORD,  lpCobjGlobalAll_->CalSteadyMax );
	MFPUWRITE( lppu_, MDMCIRVD_APARTI,	WORD,  lpCobjGlobalAll_->Apart_i );
	MFPUWRITE( lppu_, MDMCIRVD_BADMAX,	WORD,  lpCobjGlobalAll_->BadMax );
	MFPUWRITE( lppu_, MDMCIRVD_FLAGFAULT,  	BOOL, lpCobjGlobalAll_->Flag_Fault );
	MFPUWRITE( lppu_, MDMCIRVD_FLAGLP,  		BOOL, lpCobjGlobalAll_->Flag_LP );
	MFPUWRITE( lppu_, MDMCIRVD_FLAGAM,  		BOOL, lpCobjGlobalAll_->Flag_AM );
	MFPUWRITE( lppu_, MDMCIRVD_FLAGHOPEAM,  	BOOL, lpCobjGlobalAll_->Flag_HopeAM );
	MFPUWRITE( lppu_, MDMCIRVD_FLAGALARM,  	BOOL, lpCobjGlobalAll_->Flag_Alarm );
	MFPUWRITE( lppu_, MDMCIRVD_FLAGRUN,  	BOOL, lpCobjGlobalAll_->Flag_Run );
	MFPUWRITE( lppu_, MDMCIRVD_FLAGRESP,  	BOOL, lpCobjGlobalAll_->Flag_ReSP );
	MFPUWRITE( lppu_, MDMCIRVD_SRMODE, 		BOOL, lpCobjGlobalAll_->Flag_SRMode );
	MFPUWRITE( lppu_, MDMCIRVD_FLAGMCHANGE, 	BOOL, lpCobjGlobalAll_->Flag_MChange );
	MFPUWRITE( lppu_, MDMCIRVD_FLAGLPCHANGE, BOOL, lpCobjGlobalAll_->Flag_LPChange );
	MFPUWRITE( lppu_, MDMCIRVD_CCLASSIF,  	BOOL, lpCobjGlobalAll_->Flag_CControl );
	MFPUWRITE( lppu_, MDMCIRVD_MCLASSIF,  	BOOL, lpCobjGlobalAll_->Flag_MControl );
	MFPUWRITE( lppu_, MDMCIRVD_FLAGRECOUNT,  	BOOL, lpCobjGlobalAll_->Flag_Recount );
	MFPUWRITE( lppu_, MDMCIRVD_FLAGLPRECOUNT,  	BOOL, lpCobjGlobalAll_->Flag_LPRecount );
	MFPUWRITE( lppu_, MDMCIRVD_STARTMODE,  	BOOL, lpCobjGlobalAll_->StartMode );
	MFPUWRITE( lppu_, MDMCIRVD_FLAGWEIGHT,	  	BOOL, lpCobjGlobalAll_->Flag_Weight );
	MFPUWRITE( lppu_, MDMCIRVD_FLAGDEBUG,	  	BOOL, lpCobjGlobalAll_->Flag_Debug );
	MFPUWRITE( lppu_, MDMCIRVD_FLAGHMODE,	  	BOOL, lpCobjGlobalAll_->Flag_HMode );
	MFPUWRITE( lppu_, MDMCIRVD_FLAGAM1,	  	BOOL, lpCobjGlobalAll_->Flag_AM1 );
	MFPUWRITE( lppu_, MDMCIRVD_FLAGAM2,	  	BOOL, lpCobjGlobalAll_->Flag_AM2 );
	MFPUWRITE( lppu_, MDMCIRVD_FLAGAM3,	  	BOOL, lpCobjGlobalAll_->Flag_AM3 );
	MFPUWRITE( lppu_, MDMCIRVD_STEADY,	  	BOOL, lpCobjGlobalAll_->Flag_Steady );
	MFPUWRITE( lppu_, MDMCIRVD_MCVRECOUNT,	  	BOOL, lpCobjGlobalAll_->Flag_MCVRecount );
	MFPUWRITE( lppu_, MDMCIRVD_FLAGRDEBUG,	  	BOOL, lpCobjGlobalAll_->Flag_RDebug );
	MFPUWRITE( lppu_, MDMCIRVD_Save,	  	BOOL, lpCobjGlobalAll_->Flag_Save );
	MFPUWRITE( lppu_, MDMCIRVD_INISTART,	  	BOOL, lpCobjGlobalAll_->IniStart );
	MFPUWRITE( lppu_, MDMCIRVD_FLAGCORSTEADY,	  	BOOL, lpCobjGlobalAll_->Flag_CorSteady );
	MFPUWRITE( lppu_, MDMCIRVD_CALSTEADY,	  	BOOL, lpCobjGlobalAll_->CalSteady );
	MFPUWRITE( lppu_, MDMCIRVD_ERROR,	  	BOOL, lpCobjGlobalAll_->Error );
	MFPUWRITE( lppu_, MDMCIRVD_LPK,  		REAL, lpCobjGlobalAll_->LpK );
	MFPUWRITE( lppu_, MDMCIRVD_alpha,  		REAL, lpCobjGlobalAll_->Alpha );
	MFPUWRITE( lppu_, MDMCIRVD_BIGCOND,  	REAL, lpCobjGlobalAll_->Big_Cond );
	MFPUWRITE( lppu_, MDMCIRVD_PBROKEN,  	REAL, lpCobjGlobalAll_->PBroken );
	MFPUWRITE( lppu_, MDMCIRVD_QBROKEN,  	REAL, lpCobjGlobalAll_->QBroken );
	MFPUWRITE( lppu_, MDMCIRVD_RBROKEN,  	REAL, lpCobjGlobalAll_->RBroken );
	MFPUWRITE( lppu_, MDMCIRVD_SRERROR,  	REAL, lpCobjGlobalAll_->SRError );
	MFPUWRITE( lppu_, MDMCIRVD_DSRERROR,  	REAL, lpCobjGlobalAll_->DSRError );
	MFPUWRITE( lppu_, MDMCIRVD_DFILTER,  	REAL, lpCobjGlobalAll_->DFilter );
	MFPUWRITE( lppu_, MDMCIRVD_SAMPLETIME,  	REAL, lpCobjGlobalAll_->SampleTime );
	MFPUWRITE( lppu_, MDMCIRVD_CORSTEADY,  	REAL, lpCobjGlobalAll_->Cor_Steady );
	
	return	ACRET_TRUE;
}
#endif