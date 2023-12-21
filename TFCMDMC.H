#ifndef __MCC_COMPILED
#define __MCC_COMPILED
#ifdef __cplusplus
extern "C"
{
#endif

#include "tfmcv.h"
//#ifndef __debug
//#define __debug
//#endif
//#ifndef __Simul
//#define __Simul
//#endif
#ifndef __NOHIGH
#define __NOHIGH
#endif
#define		NUMBER_GLOBALPARAMWORD	  26
#define		NUMBER_GLOBALPARAMBOOL	  28
#define		NUMBER_GLOBALPARAMCOBJ	  38
#define		NUMBER_GLOBALPARAMDOUBLE  11
#define     NUMBER_GLOBALPARAMDISPLAY 103
#define     NUMBER_GLOBAL             103
#ifndef _SetIni
#define	_SetIni
#endif

#define repeatcount				1
#define	Big_Priority			10000.0
#define	SATUATION				1000
#define	epsnorm					1.0e-20
#define	NUM_HISTORY				20
#define MDMCIRVD_FILENO			0
#define MDMCIRVD_N	   		1
#define MDMCIRVD_P		  	2
#define MDMCIRVD_M			3
#define MDMCIRVD_p	   		4
#define MDMCIRVD_m	   		5
#define MDMCIRVD_nd	   		6
#define MDMCIRVD_count			7
#define MDMCIRVD_MAKEPARACOUNT	8
#define MDMCIRVD_RECOUNTCOUNT	9
#define MDMCIRVD_LPCOUNT		10
#define MDMCIRVD_NOAPART		11
#define MDMCIRVD_MPARACOUNT		12
#define MDMCIRVD_MPARAMAX		13
#define MDMCIRVD_STEADYSTEP		14
#define MDMCIRVD_ERRORCODE		15
#define MDMCIRVD_FLAGAM			16
#define MDMCIRVD_FREECOUNT		17
#define MDMCIRVD_MAXDELAY		18
#define MDMCIRVD_Modu			19
#define MDMCIRVD_N1				20
#define MDMCIRVD_N2				21
#define MDMCIRVD_STEADYCOUNT	22
#define MDMCIRVD_CALSTEADYMAX	23
#define MDMCIRVD_APARTI			24
#define MDMCIRVD_BADMAX			25
#define MDMCIRVD_CCLASSIF 		26
#define MDMCIRVD_MCLASSIF 		27
#define MDMCIRVD_FLAGFAULT 		28
#define MDMCIRVD_FLAGLP  		29//SETPOINT CONTROL OR FIELD CONTROL
#define MDMCIRVD_FLAGALARM		30
#define MDMCIRVD_FLAGRESP		31
#define MDMCIRVD_SRMODE			32
#define MDMCIRVD_FLAGMCHANGE	33
#define MDMCIRVD_FLAGLPCHANGE	34
#define MDMCIRVD_FLAGHOPEAM		35
#define MDMCIRVD_FLAGRECOUNT	36
#define MDMCIRVD_FLAGLPRECOUNT	37
#define MDMCIRVD_STARTMODE		38
#define MDMCIRVD_FLAGWEIGHT		39
#define MDMCIRVD_FLAGDEBUG		40
#define MDMCIRVD_FLAGHMODE		41
#define MDMCIRVD_FLAGAM1		42
#define MDMCIRVD_FLAGAM2		43
#define MDMCIRVD_FLAGAM3		44
#define MDMCIRVD_FLAGRUN		45
#define MDMCIRVD_STEADY			46
#define MDMCIRVD_MCVRECOUNT		47
#define	MDMCIRVD_FLAGRDEBUG		48
#define MDMCIRVD_Save			49
#define MDMCIRVD_INISTART		50
#define MDMCIRVD_FLAGCORSTEADY	51
#define MDMCIRVD_CALSTEADY		52
#define MDMCIRVD_ERROR			53
#define	MDMCIRVD_LPK			54
#define MDMCIRVD_alpha			55
#define MDMCIRVD_BIGCOND		56
#define MDMCIRVD_PBROKEN		57
#define MDMCIRVD_QBROKEN		58
#define MDMCIRVD_RBROKEN		59
#define MDMCIRVD_SRERROR		60
#define MDMCIRVD_DSRERROR		61
#define MDMCIRVD_CORSTEADY		62
#define MDMCIRVD_SAMPLETIME		63
#define MDMCIRVD_DFILTER		64
#define MDMCIRVD_mG  			65
#define MDMCIRVD_mA  			66
#define MDMCIRVD_mAb  			67
#define MDMCIRVD_mmul2  		68
#define MDMCIRVD_mmul3  		69
#define MDMCIRVD_mSR            70
#define	MDMCIRVD_mDSR			71
#define MDMCIRVD_mTempSR		72
#define MDMCIRVD_mTempDSR		73
#define MDMCIRVD_mypredN		74
#define MDMCIRVD_mH   		  	75
#define MDMCIRVD_MV  			76
#define MDMCIRVD_CV   			77
#define MDMCIRVD_DV   			78
#define MDMCIRVD_mP   		  	79
#define MDMCIRVD_mQ  	 		80
#define MDMCIRVD_mR	  			81
#define MDMCIRVD_LPPRIV			82
#define MDMCIRVD_HISTORY		83
#define MDMCIRVD_CVSort			84
#define MDMCIRVD_MVSort			85
#define MDMCIRVD_LPDeluM		86
#define MDMCIRVD_YUHis			87
#define MDMCIRVD_YDHis			88
#define MDMCIRVD_ApartMV		89
#define MDMCIRVD_ApartCV		90
#define MDMCIRVD_ApartDV		91
#define MDMCIRVD_Apart			92
#define MDMCIRVD_OUTSR			93
#define MDMCIRVD_OUTDSR			94
#define MDMCIRVD_LPCOBJGLOBAL 	95
#define MDMCIRVD_INMV			96
#define MDMCIRVD_INCV			97
#define MDMCIRVD_INDV			98
#define MDMCIRVD_LPPredU		99
#define MDMCIRVD_LPPredD		100
#define MDMCIRVD_LPISSTEADY		101
#define	MDMCIRVD_LPSTEADY		102

#define MDMCIRVD_ApartDV_PDT		PDT_LPCOBJ
#define MDMCIRVD_ERROR_PDT		PDT_BOOL
#define MDMCIRVD_BADMAX_PDT		PDT_WORD
#define MDMCIRVD_FLAGCORSTEADY_PDT	PDT_BOOL
#define MDMCIRVD_STEADYCOUNT_PDT	PDT_WORD
#define MDMCIRVD_LPSTEADY_PDT		PDT_LPCOBJ
#define MDMCIRVD_APARTI_PDT			PDT_WORD
#define MDMCIRVD_CALSTEADY_PDT		PDT_BOOL
#define MDMCIRVD_CALSTEADYMAX_PDT	PDT_WORD
#define MDMCIRVD_LPISSTEADY_PDT		PDT_LPCOBJ
#define MDMCIRVD_STEADYSTEP_PDT		PDT_WORD
#define MDMCIRVD_CORSTEADY_PDT		PDT_REAL
#define MDMCIRVD_STEADY_PDT			PDT_BOOL
#define MDMCIRVD_ERRORCODE_PDT		PDT_WORD
#define MDMCIRVD_INISTART_PDT		PDT_BOOL
#define MDMCIRVD_N1_PDT				PDT_WORD
#define MDMCIRVD_N2_PDT				PDT_WORD
#define MDMCIRVD_LPPredD_PDT		PDT_LPCOBJ			
#define MDMCIRVD_LPPredU_PDT		PDT_LPCOBJ			
#define MDMCIRVD_INMV_PDT			PDT_LPCOBJ			
#define MDMCIRVD_INCV_PDT			PDT_LPCOBJ			
#define MDMCIRVD_INDV_PDT			PDT_LPCOBJ			
#define MDMCIRVD_MAXDELAY_PDT		PDT_WORD
#define MDMCIRVD_MCVRECOUNT_PDT		PDT_BOOL
#define MDMCIRVD_FLAGRDEBUG_PDT		PDT_BOOL
#define MDMCIRVD_mTempSR_PDT		PDT_LPCOBJ
#define MDMCIRVD_mTempDSR_PDT		PDT_LPCOBJ
#define MDMCIRVD_LPDeluM_PDT		PDT_LPCOBJ
#define MDMCIRVD_Save_PDT			PDT_BOOL

#define MDMCIRVD_Modu_PDT			PDT_WORD
#define MDMCIRVD_Apart_PDT			PDT_LPCOBJ
#define MDMCIRVD_ApartMV_PDT		PDT_LPCOBJ
#define MDMCIRVD_ApartCV_PDT		PDT_LPCOBJ
#define MDMCIRVD_YUHis_PDT			PDT_LPCOBJ
#define MDMCIRVD_YDHis_PDT			PDT_LPCOBJ
#define MDMCIRVD_DeluM_PDT			PDT_LPCOBJ
#define MDMCIRVD_MVSort_PDT			PDT_LPCOBJ
#define MDMCIRVD_CVSort_PDT			PDT_LPCOBJ
#define MDMCIRVD_DFILTER_PDT		PDT_REAL

#define MDMCIRVD_LPCOBJGLOBAL_PDT	PDT_LPCOBJ
#define MDMCIRVD_OUTSR_PDT			PDT_LPCOBJ
#define MDMCIRVD_OUTDSR_PDT			PDT_LPCOBJ
#define MDMCIRVD_MAKEPARACOUNT_PDT	PDT_WORD
#define MDMCIRVD_RECOUNTCOUNT_PDT	PDT_WORD
#define MDMCIRVD_LPCOUNT_PDT		PDT_WORD
#define MDMCIRVD_FREECOUNT_PDT		PDT_int
#define MDMCIRVD_MPARACOUNT_PDT	PDT_WORD
#define MDMCIRVD_MPARAMAX_PDT	PDT_WORD
#define MDMCIRVD_NOAPART_PDT	PDT_WORD
#define MDMCIRVD_FLAGRECOUNT_PDT	PDT_BOOL
#define MDMCIRVD_FLAGLPRECOUNT_PDT	PDT_BOOL
#define MDMCIRVD_FLAGWEIGHT_PDT		PDT_BOOL
#define MDMCIRVD_FLAGDEBUG_PDT		PDT_BOOL
#define MDMCIRVD_FLAGHMODE_PDT		PDT_BOOL
#define MDMCIRVD_FLAGAM1_PDT		PDT_BOOL
#define MDMCIRVD_FLAGAM2_PDT		PDT_BOOL
#define MDMCIRVD_FLAGAM3_PDT		PDT_BOOL
#define MDMCIRVD_SRERROR_PDT		PDT_REAL
#define MDMCIRVD_DSRERROR_PDT		PDT_REAL
#define MDMCIRVD_STARTMODE_PDT		PDT_BOOL
#define MDMCIRVD_SAMPLETIME_PDT		PDT_REAL
//Multi-variable DMC with IRV  parameter type
#define MDMCIRVD_SRMODE_PDT		PDT_BOOL
#define MDMCIRVD_FLAGLP_PDT		PDT_BOOL
#define MDMCIRVD_FLAGRUN_PDT	PDT_BOOL
#define MDMCIRVD_FLAGRESP_PDT	PDT_BOOL
#define MDMCIRVD_FLAGINTER_PDT	PDT_BOOL
#define MDMCIRVD_FLAGHOPEAM_PDT	PDT_BOOL
#define MDMCIRVD_MCLASSIF_PDT   	PDT_BOOL
#define MDMCIRVD_CCLASSIF_PDT  	 	PDT_BOOL
#define MDMCIRVD_FLAGFAULT_PDT 		PDT_BOOL
#define MDMCIRVD_FLAGMCHANGE_PDT	PDT_BOOL
#define MDMCIRVD_FLAGLPCHANGE_PDT	PDT_BOOL
#define MDMCIRVD_FLAGAM_PDT			PDT_BOOL
#define MDMCIRVD_FLAGALARM_PDT		PDT_BOOL
#define MDMCIRVD_p_PDT 				PDT_WORD
#define MDMCIRVD_m_PDT 				PDT_WORD
#define MDMCIRVD_nd_PDT				PDT_WORD
#define MDMCIRVD_MV_PDT 			PDT_LPCOBJ
#define MDMCIRVD_CV_PDT 			PDT_LPCOBJ
#define MDMCIRVD_DV_PDT 			PDT_LPCOBJ
#define MDMCIRVD_mDSR_PDT 			PDT_LPCOBJ
#define MDMCIRVD_LPK_PDT 	   		PDT_REAL
#define MDMCIRVD_PBROKEN_PDT   		PDT_REAL
#define MDMCIRVD_QBROKEN_PDT   		PDT_REAL
#define MDMCIRVD_RBROKEN_PDT   		PDT_REAL
#define MDMCIRVD_BIGCOND_PDT    	PDT_REAL
#define MDMCIRVD_P_PDT   		PDT_WORD
#define MDMCIRVD_N_PDT    		PDT_WORD
#define MDMCIRVD_M_PDT    		PDT_WORD
#define MDMCIRVD_mQ_PDT  		PDT_LPCOBJ
#define MDMCIRVD_mR_PDT  		PDT_LPCOBJ
#define MDMCIRVD_mP_PDT  		PDT_LPCOBJ
#define MDMCIRVD_mH_PDT   		PDT_LPCOBJ
#define MDMCIRVD_mSR_PDT  		PDT_LPCOBJ
#define MDMCIRVD_alpha_PDT		PDT_REAL
#define MDMCIRVD_count_PDT 		PDT_WORD
#define MDMCIRVD_mmul2_PDT 		PDT_LPCOBJ
#define MDMCIRVD_mmul3_PDT 		PDT_LPCOBJ
#define MDMCIRVD_mypredN_PDT  	PDT_LPCOBJ
#define MDMCIRVD_mAb_PDT  		PDT_LPCOBJ
#define MDMCIRVD_mA_PDT  		PDT_LPCOBJ
#define MDMCIRVD_LPPRIV_PDT  	PDT_LPCOBJ
#define MDMCIRVD_mG_PDT  		PDT_LPCOBJ
#define MDMCIRVD_HISTORY_PDT 	PDT_LPCOBJ
#define MDMCIRVD_FILENO_PDT		PDT_WORD
void 		RealAll( LPCOBJPRIV _lpCobj_Priv );
void 		ReadPriv( LPCOBJPRIV lpCobj_All_, LPCOBJPRIVMV lpCobj_PrivMV_,
					LPCOBJPRIVCV lpCobj_PrivCV_, LPCOBJPRIVDV lpCobj_PrivDV_ );
void 		ReadMCV( LPCOBJPRIV lpCobj_All, LPCOBJPRIVMV lpPrivMV_,
						LPCOBJPRIVCV lpPrivCV_, LPCOBJPRIVDV lpPrivDV_ );
void 		UnitMCV( LPCOBJPRIV lpCobj_Priv, LPCOBJPRIVMV lpPrivMV_, LPCOBJPRIVCV lpPrivCV_, LPCOBJPRIVDV lpPrivDV_ );
void 		DeltMCV( LPCOBJPRIV lpCobj_Priv, LPCOBJPRIVMV lpCobj_PrivMV_,
				LPCOBJPRIVCV lpCobj_PrivCV_, LPCOBJPRIVDV lpCobj_PrivDV_ );
ACRET 		GET_SR( LPCOBJPRIV lpCobj_Priv_, LPCOBJPRIVMV lpPrivMV_,
						LPCOBJPRIVCV lpPrivCV_,	LPCOBJPRIVDV lpPrivDV );
ACRET 		GET_H( LPCOBJPRIV lpCobj_Priv_ );
void 		CorrectParam( LPCOBJPRIV lpCobj_All_, LPCOBJPRIVMV lpPrivMV_,
						LPCOBJPRIVCV lpPrivCV_ );
WORD		Initiate( LPCOBJPRIV lpCobj_All_, LPCOBJPRIVCV lpCobj_PrivCV_, LPCOBJPRIVDV lpPrivDV_ );
ACRET		MAKE_PARAM( LPCOBJPRIV lpCobj_All_, LPCOBJPRIVMV lpPrivMV_,
				LPCOBJPRIVCV lpPrivCV_ );
ACRET		Correct_y( LPCOBJPRIV lpCobj_All_, LPCOBJPRIVMV lpPrivMV_,
				LPCOBJPRIVCV lpPrivCV_, LPCOBJPRIVDV lpPrivDV_ );
//WORD		_P, _M, _N, count, flag_recount;
//WORD		flag_Qbroken, flag_Rbroken, flag_Pbroken;
//double		delQ[p], delP[m], delR[m];
//LPMATRIX	lpmu, lpmu0, lpmdelu;
//int			freecount, FLAG, recount;
//LPCOBJ		lpCobj_PrivCV[p], lpCobj_PrivMV[m];
double		GetConditionNumber( LPCOBJPRIV lpCobj_All_, LPCOBJPRIVMV lpPrivMV_,
								LPCOBJPRIVCV lpPrivCV_ );
void 		CVBegin( LPCOBJPRIV lpCobj_All_, LPCOBJPRIVCV lpPrivCV_ );
void 		MVBeginErr( LPCOBJPRIV lpCobj_All_, LPCOBJPRIVMV lpPrivMV_ );
void 		MVBeginPri( LPCOBJPRIV lpCobj_All_, LPCOBJPRIVMV lpPrivMV_ );
int 		Isrecount( LPCOBJPRIV lpCobj_All_, LPCOBJPRIVMV lpPrivMV_,
						LPCOBJPRIVCV lpPrivCV_, WORD recount_ );
void 		RemoveIll( LPCOBJPRIV lpCobj_All_, LPCOBJPRIVMV lpPrivMV_,
						LPCOBJPRIVCV lpPrivCV_ );
ACRET 		Seq( LPCOBJPRIV lpCobj_All_, LPCOBJPRIVMV lpPrivMV_, LPCOBJPRIVCV lpPrivCV_ );
WORD 		CompoundLP( LPCOBJPRIV lpCobj_All_, LPCOBJPRIVMV lpPrivMV_, LPCOBJPRIVCV lpPrivCV_, LPCOBJPRIVDV lpPrivDV_ );
void 		GetLPG( LPCOBJPRIV lpCobj_All_, LPCOBJPRIVMV lpCobj_PrivMV_,
				LPCOBJPRIVCV lpCobj_PrivCV_, LPMATRIX lpmGtemp_ );
ACRET 		CVPrioritySort( LPCOBJPRIV lpCobj_All_, LPCOBJPRIVCV lpPrivCV_ );
ACRET 		MVPrioritySort( LPCOBJPRIV lpCobj_All_, LPCOBJPRIVMV lpPrivMV_ );
void		FreeCV( LPCOBJPRIV lpCobj_All_, LPCOBJPRIVCV lpPrivCV_ );
void 		RecoverCV( LPCOBJPRIV lpCobj_All_, LPCOBJPRIVCV lpCobj_PrivCV_ );
void 		GetFreedom( LPCOBJPRIV lpCobj_All_, LPCOBJPRIVMV lpPrivMV_, LPCOBJPRIVCV lpPrivCV_ );
//WRD 		RescureCV( LPCOBJPRIVCV lpPrivCV_, LPMATRIX lpmCVSort, WRD freecount_ );
void		PrivParamInitialize( LPCOBJPRIV lpCobj_All_, WRD wNo_ );//modified by chen
ACRET		ZPrivParamInitialize( WRD wMTU_, WORD wNo_ );
ACRET		PrivInitiate(LPCOBJPRIV lpCobj_All_ ); //modified by chen
void		PrivStop( LPCOBJPRIV lpCobj_All_, LPCOBJPRIVMV lpCobjGlobaMV_, LPCOBJPRIVCV lpGlobaCV_, LPCOBJPRIVDV lpCobjPrivDV_ );
void 		CVCorrect( LPCOBJPRIV lpCobj_All_, LPCOBJPRIVCV lpPrivCV_ );
void 		MVCorrect( LPCOBJPRIV lpCobj_All_, LPCOBJPRIVMV lpPrivMV_ );
void 		Dyna_Opti( LPCOBJPRIV lpCobj_All_, LPCOBJPRIVMV lpPrivMV_, LPCOBJPRIVCV lpPrivCV_ );
void 		ForceMV( LPCOBJPRIV lpCobj_All_, LPCOBJPRIVMV lpPrivMV_ );
int 		IsmypredOver( LPCOBJPRIV lpCobj_All_, LPCOBJPRIVMV lpPrivMV_, LPCOBJPRIVCV lpPrivCV_ );
void 		CorrectPred( LPCOBJPRIV lpCobj_All_, LPCOBJPRIVMV lpPrivMV_ );
void 		UnUnit( LPCOBJPRIV lpCobj_All_, LPCOBJPRIVMV lpPrivMV_,
				LPCOBJPRIVCV lpCobj_PrivCV_, LPCOBJPRIVDV lpCobj_PrivDV_ );
ACRET 		ProvideTFAlgorithms( void );
ACRET   	_DLLFUNC RetrieveTFAlgorithms( void );
ACRET   	_DLLFUNC RegisterTFObjects(void);
#define 	UNIT( Var_, HScale, LScale )	Var_ = ( Var_ - LScale) / ( HScale - LScale )
#define     UNUNIT( Var_, HScale, LScale )	Var_ = Var_ * ( HScale - LScale ) + LScale
void 		WriteMCV( LPCOBJPRIV lpCobj_All_, LPCOBJPRIVMV lpPrivMV_, LPCOBJPRIVCV lpPrivCV_, LPCOBJPRIVDV lpCobj_PrivDV_ );
void 		READSCALE( LPCOBJPRIV lpCobj_All_, LPCOBJPRIVMV lpCobj_PrivMV_,
					LPCOBJPRIVCV lpCobj_PrivCV_, LPCOBJPRIVDV lpCobj_PrivDV_ );

ACRET		Read_FileMV( LPCOBJPRIV lpCobj_All, LPCOBJPRIVMV lpCobj_PrivMV_ );
ACRET		Read_FileCV( LPCOBJPRIV lpCobj_All, LPCOBJPRIVCV lpCobj_PrivCV_ );
ACRET		Read_FileDV( LPCOBJPRIV lpCobj_All, LPCOBJPRIVDV lpCobj_PrivDV_ );
ACRET		Read_FileAll( LPCOBJPRIV lpCobj_PrivAll_ );
ACRET		Write_FileMV( LPCOBJPRIV lpCobj_All, LPCOBJPRIVMV lpCobj_PrivMV_ );
ACRET		Write_FileCV( LPCOBJPRIV lpCobj_All, LPCOBJPRIVCV lpCobj_PrivCV_ );
ACRET		Write_FileDV( LPCOBJPRIV lpCobj_All, LPCOBJPRIVDV lpCobj_PrivDV_ );
ACRET		Write_FileAll( LPCOBJPRIV lpCobj_PrivAll_ );
ACRET		UnDeltFile( LPCOBJPRIV _lpCobjPrivAll, LPCOBJPRIVMV _lpCobjPrivMV, LPCOBJPRIVCV _lpCobjPrivCV, LPCOBJPRIVDV _lpCobjPrivDV );
ACRET		UnUnitFile( LPCOBJPRIV _lpCobjPrivAll, LPCOBJPRIVMV _lpCobjPrivMV, LPCOBJPRIVCV _lpCobjPrivCV, LPCOBJPRIVDV _lpCobjPrivDV );
double		power( double x_, int n_ );
ACRET		ReadFileSR( LPCOBJPRIV lpCobjPrivAll_ );
ACRET		ApartSR( LPCOBJPRIV lpCobjPrivAll_ );
ACRET		GetApartMCV( LPCOBJPRIV lpCobjGlobalAll_ );
ACRET		File_Out_SR( LPCOBJPRIV lpCobj_All_ );
ACRET		MergeLPG( LPCOBJPRIV lpCobj_All_, LPCOBJPRIVMV lpCobj_PrivMV_, LPCOBJPRIVCV lpCobj_PrivCV_,
				LPCOBJPRIVDV lpCobj_PrivDV_ );
ACRET		File_OUT( LPCOBJPRIV lpCobj_All_, LPCOBJPRIVMV lpCobj_PrivMV_,
				LPCOBJPRIVCV lpCobj_PrivCV_, LPCOBJPRIVDV lpCobj_PrivDV_ );
ACRET		File_SR( LPCOBJPRIV lpCobj_All_ );
ACRET		GlobalInitiate(LPCOBJPRIV _lpCobjPrivAll );
ACRET		Display_2_PrivAll( LPCOBJPRIV _lpCobjGlobalAll );
ACRET		PrivAll_2_Display( LPCOBJPRIV _lpCobjGlobalAll );
ACRET		MVStructWrite( LPCOBJPRIV lpCobj_All_, LPCOBJPRIVMV lpCobjPrivMV_, WORD	j_ );
ACRET		CVStructWrite( LPCOBJPRIV lpCobj_All_, LPCOBJPRIVCV lpCobjPrivCV_, WORD	j_ );
ACRET		DVStructWrite( LPCOBJPRIV lpCobj_All_, LPCOBJPRIVDV lpCobjPrivDV_, WORD	j_ );
ACRET		MVStructRead( LPCOBJPRIV lpCobj_All_, LPCOBJPRIVMV lpCobjPrivMV_, WORD	j_ );
ACRET		CVStructRead( LPCOBJPRIV lpCobj_All_, LPCOBJPRIVMV lpCobjPrivMV_, WORD	j_ );
ACRET		DVStructRead( LPCOBJPRIV lpCobj_All_, LPCOBJPRIVMV lpCobjPrivMV_, WORD	j_ );
ACRET		StructTableCreat( WRD wMTU_, WORD wNo_ );
ACRET		ZPrivParamSave( WRD wMTU_, WRD wNo_ );
ACRET		ReadFileOutSR( LPCOBJPRIV lpCobjPrivAll_ );
ACRET		LookOut( LPCOBJPRIV _lpCobjGlobalAll, LPCOBJPRIVMV _lpGlobalMV, 
					 LPCOBJPRIVCV _lpGlobalCV, LPCOBJPRIVDV _lpGlobalDV );
void		IsSteady( LPCOBJPRIV lpCobj_All_, LPCOBJPRIVCV lpCobj_PrivCV_, LPCOBJPRIVMV lpCobj_PrivMV_,
						LPCOBJPRIVDV lpCobj_PrivDV_ );
int			cagaus( LPMATRIX lpmA_, LPMATRIX lpmB_, LPMATRIX lpmX_ );
void		WriteSubGParameter( LPCOBJPRIV lpCobj_All_, int i_ );
void		CorSteady( LPCOBJPRIV _lpCobjPrivAll, LPCOBJPRIVMV _lpCobjPrivMV, LPCOBJPRIVCV _lpCobjPirvCV, LPCOBJPRIVDV _lpCobjPrivDV );
void		GetPrevUD( LPCOBJPRIV lpCobj_All_ );
void		Display( LPCOBJPRIV lpCobj_All_, LPCOBJPRIVMV lpCobj_PrivMV_, LPCOBJPRIVCV lpCobj_PrivCV_, LPCOBJPRIVDV lpCobj_PrivDV_ );
void		LPDebugFile( LPCOBJPRIV lpCobj_All_, LPCOBJPRIVMV lpCobj_PrivMV_, LPCOBJPRIVCV lpCobj_PrivCV_, LPCOBJPRIVDV lpCobj_PrivDV_ );
void		LogError( LPCTSTR lpSection_, LPCTSTR lpsz_ );
ACRET		GetFinalFreedom( LPCOBJPRIV _lpCobjPrivAll, LPCOBJPRIVMV _lpCobjPrivMV, LPCOBJPRIVCV _lpCobjPrivCV );
#ifdef __cplusplus
}
#endif
#endif