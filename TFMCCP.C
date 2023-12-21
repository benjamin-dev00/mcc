//===================================================================
//
//Zhang 1.3, 1998
//===================================================================

#include "tfalgo.h"
#include "tfcmdmc.h"
#ifdef __ONSPEC
#include "Mtmfun.h"
#include "Mtdllfun.h"
#endif



//Multi-variable DMC with IRV parameter order
///////////////  ONLINE DMC  //////////////////////////////////////////
//...................................................................
//...................................................................
static VSOFNAME _mode_vsof[] =	//DrawText
{
	{FALSE,	"手动"},
	{TRUE,	"自动"},
};

#ifdef __ONSPEC
char	LocalDirectory[128];
#endif
#ifdef __ZZK
char	LocalDirectory[128];
#endif

_DLLEXP	DWORD _ZZKDECL MDMCIRVDParam( PIC wCmd_, WRD wMTU_, WORD wNo_, LONG lParam_ )
{
	static PDU _sPDU[] =
	{
		_ELE_PDU( MDMCIRVD_FILENO,			"File_No",		VSOF_WORD, PU_NORMAL, PRI_ALL ),
		_ELE_PDU( MDMCIRVD_N,				"_N",			VSOF_WORD, PU_NORMAL, PRI_ALL ),
		_ELE_PDU( MDMCIRVD_P,				"_P",			VSOF_WORD, PU_NORMAL, PRI_ALL ),
		_ELE_PDU( MDMCIRVD_M,				"_M",			VSOF_WORD, PU_NORMAL, PRI_ALL ),
		_ELE_PDU( MDMCIRVD_p,				"p",			VSOF_WORD,	PU_NORMAL, PRI_ALL ),
		_ELE_PDU( MDMCIRVD_m,				"m",			VSOF_WORD,	PU_NORMAL, PRI_ALL ),
		_ELE_PDU( MDMCIRVD_nd,				"nd",			VSOF_WORD,	PU_NORMAL, PRI_ALL ),
		_ELE_PDU( MDMCIRVD_count,			"count",		VSOF_WORD,	PU_NORMAL, PRI_ALL ),
		_ELE_PDU( MDMCIRVD_MAKEPARACOUNT,	"M_ParaCount",	VSOF_WORD, PU_NORMAL, PRI_ALL ),
		_ELE_PDU( MDMCIRVD_RECOUNTCOUNT,	"RecountCount",	VSOF_WORD, PU_NORMAL, PRI_ALL ),
		_ELE_PDU( MDMCIRVD_LPCOUNT,			"LPCount",		VSOF_WORD, PU_NORMAL, PRI_ALL ),
		_ELE_PDU( MDMCIRVD_NOAPART,			"No_Apart",		VSOF_WORD, PU_NORMAL, PRI_ALL ),
		_ELE_PDU( MDMCIRVD_MPARACOUNT,		"FreeParaCount",VSOF_WORD, PU_NORMAL, PRI_ALL ),
		_ELE_PDU( MDMCIRVD_MPARAMAX,		"FreeParaMax",	VSOF_WORD, PU_NORMAL, PRI_ALL ),
		_ELE_PDU( MDMCIRVD_STEADYSTEP,		"SteadyStep",	VSOF_WORD, PU_NORMAL, PRI_ALL ),
		_ELE_PDU( MDMCIRVD_ERRORCODE,		"ErrorCode",	VSOF_WORD, PU_NORMAL, PRI_ALL ),
		_ELE_PDU( MDMCIRVD_FLAGAM,			"A/M",			VSOF_BOOL,	 PU_NORMAL, PRI_ALL ),
		_ELE_PDU( MDMCIRVD_FREECOUNT,		"FreeCount",	VSOF_int,	PU_NORMAL, PRI_ALL ),
		_ELE_PDU( MDMCIRVD_MAXDELAY,		"MaxDelay",		VSOF_WORD,	 PU_NORMAL, PRI_ALL ),
		_ELE_PDU( MDMCIRVD_Modu,			"Modu",			VSOF_WORD,		PU_NORMAL, PRI_ALL ),
		_ELE_PDU( MDMCIRVD_N1,				"N1",			VSOF_WORD, PU_NORMAL, PRI_ALL ),
		_ELE_PDU( MDMCIRVD_N2,				"N2",			VSOF_WORD, PU_NORMAL, PRI_ALL ),
		_ELE_PDU( MDMCIRVD_STEADYCOUNT,		"SteadyCount",	VSOF_WORD, PU_NORMAL, PRI_ALL ),
		_ELE_PDU( MDMCIRVD_CALSTEADYMAX,	"SteadyMaxCond",VSOF_WORD, PU_NORMAL, PRI_ALL ),
		_ELE_PDU( MDMCIRVD_APARTI,			"Apart_i",		VSOF_WORD, PU_NORMAL, PRI_ALL ),
		_ELE_PDU( MDMCIRVD_BADMAX,			"BadMax",		VSOF_WORD, PU_NORMAL, PRI_ALL ),
		_ELE_PDU( MDMCIRVD_CCLASSIF,		"CVClassif",	VSOF_BOOL,		PU_NORMAL, PRI_ALL ),
		_ELE_PDU( MDMCIRVD_MCLASSIF,		"MVClassif",	VSOF_BOOL,		PU_NORMAL, PRI_ALL ),
		_ELE_PDU( MDMCIRVD_FLAGFAULT,		"Flag_Fault",	VSOF_BOOL, PU_NORMAL, PRI_ALL ),
		_ELE_PDU( MDMCIRVD_FLAGLP,			"Flag_LP",		VSOF_BOOL,	PU_NORMAL, PRI_ALL ),
		_ELE_PDU( MDMCIRVD_FLAGALARM,		"ALARM",		VSOF_BOOL,	PU_NORMAL, PRI_ALL ),
		_ELE_PDU( MDMCIRVD_FLAGRESP,		"ReSetPara",	VSOF_BOOL, PU_NORMAL, PRI_ALL ),
		_ELE_PDU( MDMCIRVD_SRMODE,			"SRMode",		VSOF_BOOL, PU_NORMAL, PRI_ALL ),
		_ELE_PDU( MDMCIRVD_FLAGMCHANGE,		"ModelChange",	VSOF_BOOL, PU_NORMAL, PRI_ALL ),
		_ELE_PDU( MDMCIRVD_FLAGLPCHANGE,	"LPChange",		VSOF_BOOL, PU_NORMAL, PRI_ALL ),
		_ELE_PDU( MDMCIRVD_FLAGHOPEAM,		"Hope_AM",		VSOF_BOOL_USER, PU_NORMAL, PRI_ALL ),
		_ELE_PDU( MDMCIRVD_FLAGRECOUNT,		"Recount",		VSOF_BOOL, PU_NORMAL, PRI_ALL ),
		_ELE_PDU( MDMCIRVD_FLAGLPRECOUNT,	"LPRecount",	VSOF_BOOL, PU_NORMAL, PRI_ALL ),
		_ELE_PDU( MDMCIRVD_STARTMODE,		"StartMode",	VSOF_BOOL, PU_NORMAL, PRI_ALL ),
		_ELE_PDU( MDMCIRVD_FLAGWEIGHT,		"WeightMode",	VSOF_BOOL, PU_NORMAL, PRI_ALL ),
		_ELE_PDU( MDMCIRVD_FLAGDEBUG,		"DebugMode",	VSOF_BOOL, PU_NORMAL, PRI_ALL ),
		_ELE_PDU( MDMCIRVD_FLAGHMODE,		"HMode",		VSOF_BOOL, PU_NORMAL, PRI_ALL ),
		_ELE_PDU( MDMCIRVD_FLAGAM1,			"AM1",			VSOF_BOOL, PU_NORMAL, PRI_ALL ),
		_ELE_PDU( MDMCIRVD_FLAGAM2,			"AM2",			VSOF_BOOL, PU_NORMAL, PRI_ALL ),
		_ELE_PDU( MDMCIRVD_FLAGAM3,			"AM3",			VSOF_BOOL, PU_NORMAL, PRI_ALL ),
		_ELE_PDU( MDMCIRVD_FLAGRUN,			"Run",			VSOF_BOOL, PU_NORMAL, PRI_ALL ),
		_ELE_PDU( MDMCIRVD_STEADY,			"SteadyState",	VSOF_BOOL, PU_NORMAL, PRI_ALL ),
		_ELE_PDU( MDMCIRVD_MCVRECOUNT,		"MCVRecout",	VSOF_BOOL, PU_NORMAL, PRI_ALL ),
		_ELE_PDU( MDMCIRVD_FLAGRDEBUG,		"RDebug",		VSOF_BOOL, PU_NORMAL, PRI_ALL ),
		_ELE_PDU( MDMCIRVD_Save,			"Save",			VSOF_BOOL,	PU_NORMAL, PRI_ALL ),
		_ELE_PDU( MDMCIRVD_INISTART,		"IniStart",		VSOF_BOOL, PU_NORMAL, PRI_ALL ),
		_ELE_PDU( MDMCIRVD_FLAGCORSTEADY,	"Flag_CorSteady",	VSOF_BOOL, PU_NORMAL, PRI_ALL ),
		_ELE_PDU( MDMCIRVD_CALSTEADY,		"CalSteady",	VSOF_BOOL, PU_NORMAL, PRI_ALL ),
		_ELE_PDU( MDMCIRVD_ERROR,			"Error",		VSOF_BOOL, PU_NORMAL, PRI_ALL ),
		_ELE_PDU( MDMCIRVD_LPK,				"LPNUM_K",		VSOF_REAL,		PU_NORMAL, PRI_ALL ),
		_ELE_PDU( MDMCIRVD_alpha,			"alpha",		VSOF_REAL,	PU_NORMAL, PRI_ALL ),
		_ELE_PDU( MDMCIRVD_BIGCOND,			"Big_Cond",     VSOF_REAL,		PU_NORMAL, PRI_ALL ),
		_ELE_PDU( MDMCIRVD_PBROKEN,			"PBroken",	    VSOF_REAL,	PU_NORMAL, PRI_ALL ),
		_ELE_PDU( MDMCIRVD_QBROKEN,			"QBroken",	    VSOF_REAL,		PU_NORMAL, PRI_ALL ),
		_ELE_PDU( MDMCIRVD_RBROKEN,			"RBroken",	    VSOF_REAL,		PU_NORMAL, PRI_ALL ),
		_ELE_PDU( MDMCIRVD_SRERROR,			"SRError",		VSOF_REAL, PU_NORMAL, PRI_ALL ),
		_ELE_PDU( MDMCIRVD_DSRERROR,		"DSRError",		VSOF_REAL, PU_NORMAL, PRI_ALL ),
		_ELE_PDU( MDMCIRVD_CORSTEADY,		"Cor_steady",	VSOF_REAL, PU_NORMAL, PRI_ALL ),
		_ELE_PDU( MDMCIRVD_SAMPLETIME,		"SampleT",		VSOF_REAL, PU_NORMAL, PRI_ALL ),
		_ELE_PDU( MDMCIRVD_DFILTER,			"DFilter",		VSOF_REAL, PU_NORMAL, PRI_ALL ),
		_ELE_PDU( MDMCIRVD_mG,				"mG",			VSOF_LPCOBJ,	PU_NORMAL, PRI_ALL ),
		_ELE_PDU( MDMCIRVD_mA,				"mA",			VSOF_LPCOBJ, PU_NORMAL, PRI_ALL ),
		_ELE_PDU( MDMCIRVD_mAb,				"mAb",			VSOF_LPCOBJ, PU_NORMAL, PRI_ALL ),
		_ELE_PDU( MDMCIRVD_mmul2,			"mul2",			VSOF_LPCOBJ, PU_NORMAL, PRI_ALL ),
		_ELE_PDU( MDMCIRVD_mmul3,			"mul3",			VSOF_LPCOBJ, PU_NORMAL, PRI_ALL ),
		_ELE_PDU( MDMCIRVD_mSR,				"mSR",			VSOF_LPCOBJ, PU_NORMAL, PRI_ALL ),
		_ELE_PDU( MDMCIRVD_mDSR,			"mDSR",			VSOF_LPCOBJ, PU_NORMAL, PRI_ALL ),
		_ELE_PDU( MDMCIRVD_mTempSR,			"mTempSR",			VSOF_LPCOBJ, PU_NORMAL, PRI_ALL ),
		_ELE_PDU( MDMCIRVD_mTempDSR,		"mTempDSR",			VSOF_LPCOBJ, PU_NORMAL, PRI_ALL ),
		_ELE_PDU( MDMCIRVD_mypredN,			"mypredN",		VSOF_LPCOBJ, PU_NORMAL, PRI_ALL ),
		_ELE_PDU( MDMCIRVD_mH,				"mH",			VSOF_LPCOBJ, PU_NORMAL, PRI_ALL ),
		_ELE_PDU( MDMCIRVD_MV,				"U",			VSOF_LPCOBJ, PU_NORMAL, PRI_ALL ),
		_ELE_PDU( MDMCIRVD_CV,				"Y",			VSOF_LPCOBJ, PU_NORMAL, PRI_ALL ),
		_ELE_PDU( MDMCIRVD_DV,				"Dist",			VSOF_LPCOBJ, PU_NORMAL, PRI_ALL ),
		_ELE_PDU( MDMCIRVD_mP,				"mP",			VSOF_LPCOBJ, PU_NORMAL, PRI_ALL ),
		_ELE_PDU( MDMCIRVD_mQ,				"mQ",			VSOF_LPCOBJ, PU_NORMAL, PRI_ALL ),
		_ELE_PDU( MDMCIRVD_mR,				"mR",			VSOF_LPCOBJ, PU_NORMAL, PRI_ALL ),
		_ELE_PDU( MDMCIRVD_LPPRIV,			"LPHis",		VSOF_LPCOBJ, PU_NORMAL, PRI_ALL ),
		_ELE_PDU( MDMCIRVD_HISTORY,			"HistData",		VSOF_LPCOBJ, PU_NORMAL, PRI_ALL ),
		_ELE_PDU( MDMCIRVD_CVSort,			"CVSort",		VSOF_LPCOBJ, PU_NORMAL, PRI_ALL ),
		_ELE_PDU( MDMCIRVD_MVSort,			"MVSort",		VSOF_LPCOBJ, PU_NORMAL, PRI_ALL ),
		_ELE_PDU( MDMCIRVD_LPDeluM,			"DeluM",		VSOF_LPCOBJ, PU_NORMAL, PRI_ALL ),
		_ELE_PDU( MDMCIRVD_YUHis,			"YUHist",		VSOF_LPCOBJ, PU_NORMAL, PRI_ALL ),
		_ELE_PDU( MDMCIRVD_YDHis,			"DUHist",		VSOF_LPCOBJ, PU_NORMAL, PRI_ALL ),
		_ELE_PDU( MDMCIRVD_ApartMV,			"ApartMV",		VSOF_LPCOBJ, PU_NORMAL, PRI_ALL ),
		_ELE_PDU( MDMCIRVD_ApartCV,			"ApartCV",		VSOF_LPCOBJ, PU_NORMAL, PRI_ALL ),
		_ELE_PDU( MDMCIRVD_ApartDV,			"ApartDV",		VSOF_LPCOBJ, PU_NORMAL, PRI_ALL ),
		_ELE_PDU( MDMCIRVD_Apart,			"Apart",		VSOF_LPCOBJ, PU_NORMAL, PRI_ALL ),
		_ELE_PDU( MDMCIRVD_OUTSR,			"OutSR",		VSOF_LPCOBJ, PU_NORMAL, PRI_ALL ),
		_ELE_PDU( MDMCIRVD_OUTDSR,			"OutDSR",		VSOF_LPCOBJ, PU_NORMAL, PRI_ALL ),
		_ELE_PDU( MDMCIRVD_LPCOBJGLOBAL,	"LPGlobal",		VSOF_LPCOBJ, PU_NORMAL, PRI_ALL ),
		_ELE_PDU( MDMCIRVD_INMV,			"InerMV",		VSOF_LPCOBJ, PU_NORMAL, PRI_ALL ),
		_ELE_PDU( MDMCIRVD_INCV,			"InerCV",		VSOF_LPCOBJ, PU_NORMAL, PRI_ALL ),
		_ELE_PDU( MDMCIRVD_INDV,			"InerDV",		VSOF_LPCOBJ, PU_NORMAL, PRI_ALL ),
		_ELE_PDU( MDMCIRVD_LPPredU,			"lpPredU",		VSOF_LPCOBJ, PU_NORMAL, PRI_ALL ),
		_ELE_PDU( MDMCIRVD_LPPredD,			"lpPredD",		VSOF_LPCOBJ, PU_NORMAL, PRI_ALL ),
		_ELE_PDU( MDMCIRVD_LPISSTEADY,		"IsSteady",		VSOF_LPCOBJ, PU_NORMAL, PRI_ALL ),
		_ELE_PDU( MDMCIRVD_LPSTEADY,		"LPSteady",		VSOF_LPCOBJ, PU_NORMAL, PRI_ALL ),
	};
switch( wCmd_ )
	{
	case PIC_GETVSOFENUMSTRUCT:
		{
			if( wNo_ == MDMCIRVD_FLAGHOPEAM )
			{
				LPVSOFENUMSTRUCT _lpes = (LPVSOFENUMSTRUCT)lParam_;
				
				_lpes->lpVsofName = _mode_vsof;
				_lpes->nItem = sizeof(_mode_vsof)/sizeof(_mode_vsof[0]);
				return TRUE;
			}
			break;
		}
	case PIC_INITIALIZEPARAM:
		{

			_DefAlgoParam( wCmd_,
			 			  wMTU_,
			 			  wNo_,
						  lParam_,
						  sizeof(_sPDU)/sizeof(PDU),
						  IDA_MDMCIRVD,
						  &(_sPDU[0]) );
			ZPrivParamInitialize( wMTU_, wNo_);
			return TRUE;
		}
	case PIC_INITIATE:
		{
			LPCOBJ		_lpCobj_Priv;
			LPCOBJPRIV	_lpCobjGlobalAll;

			_lpCobj_Priv = CobjAlloc( IDCOBJ_MATRIX, sizeof(COBJ) + sizeof( CobjPriv ) );
			_lpCobjGlobalAll = COBJ_2_PRIV( _lpCobj_Priv );
			_lpCobjGlobalAll->wModu = wMTU_;
			WRITEALLPARAMETER( _lpCobjGlobalAll, MDMCIRVD_LPCOBJGLOBAL, LPCOBJ, _lpCobj_Priv );
			if ( !PrivInitiate( _lpCobjGlobalAll ) )
			{
				CobjFree( _lpCobj_Priv );
				return ACRET_FALSE;
			}
			return TRUE;
		}
	case PIC_STOP:
		{
			LPCOBJPRIV		_lpCobjGlobalAll;
			LPCOBJPRIVMV	_lpGlobalMV;
			LPCOBJPRIVCV	_lpGlobalCV;
			LPCOBJPRIVDV	_lpCobjPrivDV;
			LPCOBJ			_lpCobj_Priv;
			LPPU			_lppu = ModuGetParameter( wMTU_ );
			
			MFPUREAD( _lppu, MDMCIRVD_LPCOBJGLOBAL, LPCOBJ, _lpCobj_Priv );
			_lpCobjGlobalAll = COBJ_2_PRIV( _lpCobj_Priv );
			_lpGlobalMV = COBJ_2_PRIVMV( _lpCobjGlobalAll->LPINMV );
			_lpGlobalCV = COBJ_2_PRIVCV( _lpCobjGlobalAll->LPINCV );
			_lpCobjPrivDV = COBJ_2_PRIVDV( _lpCobjGlobalAll->LPINDV );
			PrivStop( _lpCobjGlobalAll, _lpGlobalMV, _lpGlobalCV, _lpCobjPrivDV );
			WRITEALLPARAMETER( _lpCobjGlobalAll, MDMCIRVD_LPPredU, LPCOBJ, NULL );
			WRITEALLPARAMETER( _lpCobjGlobalAll, MDMCIRVD_LPPredD, LPCOBJ, NULL );
#ifdef __Simul
			WRITEALLPARAMETER( _lpCobjGlobalAll, MDMCIRVD_YUHis, LPCOBJ, NULL );
			if( _lpCobjGlobalAll->nd )
				WRITEALLPARAMETER( _lpCobjGlobalAll, MDMCIRVD_YDHis, LPCOBJ, NULL );
#endif
			WRITEALLPARAMETER( _lpCobjGlobalAll, MDMCIRVD_LPCOBJGLOBAL, LPCOBJ, NULL );
		}
	}
	return _DefAlgoParam( wCmd_,
						  wMTU_,
						  wNo_,
						  lParam_,
						  sizeof(_sPDU)/sizeof(PDU),
						  IDA_MDMCIRVD,
						  &(_sPDU[0]) );
}
