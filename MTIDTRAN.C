//Mtidtran.c
//transform the ID of ttf to ID of Chen

#include <windows.h>
#include "Mtstruct.h"
#include "tfalgo.h"
#include "tfcmdmc.h"


MTHANDLE TransGvID(WORD* _lp_ttfID,WORD* _lp_MtID)
{
	MTHANDLE _h_MtHandle=0;
	switch(*_lp_ttfID)
	{
	case MDMCIRVD_FILENO:
		*_lp_MtID=ID_File_No;
		break;
	case MDMCIRVD_N:
		*_lp_MtID=ID_N;
		break;
    case MDMCIRVD_P:
		*_lp_MtID=ID_P;
		break;
	case MDMCIRVD_M:
		*_lp_MtID=ID_M;
		break;
    case MDMCIRVD_p:
		*_lp_MtID=ID_p;
		break;
    case MDMCIRVD_m:
		*_lp_MtID=ID_m;
		break;
    case MDMCIRVD_nd:
		*_lp_MtID=ID_nd;
		break;
    case MDMCIRVD_alpha:
		*_lp_MtID=ID_Alpha;
		break;
    case MDMCIRVD_count:
		*_lp_MtID=ID_Count;
		break;
    case MDMCIRVD_FLAGLP:
		*_lp_MtID=ID_Flag_LP;
		break;
    case MDMCIRVD_PBROKEN:
		*_lp_MtID=ID_PBroken;
		break;
    case MDMCIRVD_QBROKEN:
		*_lp_MtID=ID_QBroken;
		break;
    case MDMCIRVD_RBROKEN:
		*_lp_MtID=ID_RBroken;
		break;
    case MDMCIRVD_BIGCOND:
		*_lp_MtID=ID_Big_Cond;
		break;
	case MDMCIRVD_MCLASSIF:
		*_lp_MtID=ID_Flag_MControl;
		break;
	case MDMCIRVD_CCLASSIF:
		*_lp_MtID=ID_Flag_CControl;
		break;
    case MDMCIRVD_LPK:
		*_lp_MtID=ID_LpK;
		break;
	case MDMCIRVD_FLAGAM:
		*_lp_MtID=ID_Flag_AM;
		break;
    case MDMCIRVD_FLAGALARM:
		*_lp_MtID=ID_Flag_Alarm;
		break;
    case MDMCIRVD_FLAGRUN:
		*_lp_MtID=ID_Flag_Run;
		break;
    case MDMCIRVD_FLAGRESP:
		*_lp_MtID=ID_Flag_ReSP;
		break;
	case MDMCIRVD_SRMODE:
		*_lp_MtID=ID_Flag_SRMode;
		break;
    case MDMCIRVD_FLAGHOPEAM:
		*_lp_MtID=ID_Flag_HopeAM;
		break;
    case MDMCIRVD_FLAGMCHANGE:
		*_lp_MtID=ID_Flag_MChange;
		break;
    case MDMCIRVD_FLAGLPCHANGE:
		*_lp_MtID=ID_Flag_LPChange;
		break;
    case MDMCIRVD_FLAGFAULT:
		*_lp_MtID=ID_Flag_Fault;
		break;
	case MDMCIRVD_MAKEPARACOUNT:
		*_lp_MtID=ID_MakeParaCount;
		break;
	case MDMCIRVD_RECOUNTCOUNT:
		*_lp_MtID=ID_RecountCount;
		break;
	case MDMCIRVD_LPCOUNT:		
		*_lp_MtID=ID_LPCount;
		break;
	case MDMCIRVD_FREECOUNT:			
		*_lp_MtID=ID_Freecount;
		break;
	case MDMCIRVD_MPARACOUNT:	
		*_lp_MtID=ID_MParaCount;
		break;
	case MDMCIRVD_MPARAMAX:		
		*_lp_MtID=ID_MParaMax;
		break;
	case MDMCIRVD_NOAPART:		
		*_lp_MtID=ID_No_Apart;
		break;
	case MDMCIRVD_FLAGRECOUNT:	
		*_lp_MtID=ID_Flag_Recount;
		break;
	case MDMCIRVD_FLAGLPRECOUNT:	
		*_lp_MtID=ID_Flag_LPRecount;
		break;
	case MDMCIRVD_FLAGWEIGHT:	
		*_lp_MtID=ID_Flag_Weight;
		break;
	case MDMCIRVD_FLAGDEBUG:		
		*_lp_MtID=ID_Flag_Debug;
		break;
	case MDMCIRVD_FLAGHMODE:	
		*_lp_MtID=ID_Flag_HMode;
		break;
	case MDMCIRVD_FLAGAM1:	
		*_lp_MtID=ID_Flag_AM1;
		break;
	case MDMCIRVD_FLAGAM2:		
		*_lp_MtID=ID_Flag_AM2;
		break;
	case MDMCIRVD_FLAGAM3:			
		*_lp_MtID=ID_Flag_AM3;
		break;
	case MDMCIRVD_SRERROR:		
		*_lp_MtID=ID_SRError;
		break;
	case MDMCIRVD_DSRERROR:		
		*_lp_MtID=ID_DSRError;
		break;
	case MDMCIRVD_STARTMODE:		
		*_lp_MtID=ID_StartMode;
		break;
	case MDMCIRVD_SAMPLETIME:		
		*_lp_MtID=ID_SampleTime;
		break;
	case MDMCIRVD_STEADY:		
		*_lp_MtID=ID_Flag_Steady;
		break;
	case MDMCIRVD_CORSTEADY:		
		*_lp_MtID=ID_Cor_Steady;
		break;
	case MDMCIRVD_STEADYSTEP:		
		*_lp_MtID=ID_SteadyStep;
		break;
	case MDMCIRVD_ERRORCODE:		
		*_lp_MtID=ID_ErrorCode;
		break;
	default:
		_h_MtHandle=INVALID_TTFID;
   	}
	return _h_MtHandle;
}

    
MTHANDLE TransCvID(WORD* _lp_ttfID,WORD* _lp_MtID)
{
	MTHANDLE _h_MtHandle=NOERROR;
	switch(*_lp_ttfID)
	{
	case CV_TAG:
		 *_lp_MtID=ID_TAG;
		 break;
	case CV_DATANAME:
		 *_lp_MtID=ID_DataName;
		 break;
    case CV_NAME:
		 *_lp_MtID=ID_Name;
		 break;
    case CV_VALUE:
		 *_lp_MtID=ID_CData;
		 break;
    case CV_UNIT:
		 *_lp_MtID=ID_Unit;
		 break;
    case CV_HSCALE:
		*_lp_MtID=ID_CHScale;
		break;
    case CV_LSCALE:
		*_lp_MtID=ID_CLScale;
		break;
    case CV_SET:
		*_lp_MtID=ID_CSetpoint;
		break;
    case CV_INI:
		*_lp_MtID=ID_CIni;
		break;
    case CV_CONTROLMODE :
		*_lp_MtID=ID_CControl_type;
		break;
    case CV_RCONTROLMODE:
		*_lp_MtID=ID_CRControl_type;
		break;
    case CV_MAX:
		*_lp_MtID=ID_CMax;
		break;
    case CV_MIN:
		*_lp_MtID=ID_CMin;
		break;
    case CV_HIGH:
		*_lp_MtID=ID_CHigh;
		break;
    case CV_LOW:
		*_lp_MtID=ID_CLow;
		break;
    case CV_FMAX:
		*_lp_MtID=ID_CFault_max;
		break;
    case CV_FMIN:
		*_lp_MtID=ID_CFault_min;
		break;
    case CV_FHIGH:
		*_lp_MtID=ID_CFault_high;
		break;
    case CV_FLOW:
		*_lp_MtID=ID_CFault_low;
		break;
    case CV_FXSET:
		*_lp_MtID=ID_CFault_xsetpoint;
		break;
    case CV_FNSET:
		*_lp_MtID=ID_CFault_nsetpoint;
		break;
    case CV_FHSET:
		*_lp_MtID=ID_CFault_hsetpoint;
		break;
    case CV_FLSET:
		*_lp_MtID=ID_CFault_lsetpoint;
		break;
    case CV_LPINI:
		*_lp_MtID=ID_CLPIni;
		break;
    case CV_LPINDEX:
		*_lp_MtID=ID_CLPIndex;
		break;
    case CV_LPOUT:
		*_lp_MtID=ID_CSteadOptim;
		break;
	case CV_PRIORITY:
		*_lp_MtID=ID_CPriority;
		break;
	case CV_STATE:
		*_lp_MtID=ID_CInvalid;
		break;
    case CV_ALARM:
		*_lp_MtID=ID_CAlarm;
		break;
    case CV_Q:
		*_lp_MtID=ID_CQ;
		break;
    case CV_QL:
		*_lp_MtID=ID_CQL;
		break;
	case CV_QBROKEN:
		*_lp_MtID=ID_CQMax;
		break;
    case CV_DELQ:
		*_lp_MtID=ID_CDelQ;
		break;
    case CV_REALQ:
		*_lp_MtID=ID_CRealQ;
		break;
   case CV_KEYCV:
		*_lp_MtID=ID_CKey;
		break;
    case CV_ALARMMAX:
		*_lp_MtID=ID_CHAlarm;
		break;
    case CV_ALARMMIN:
		*_lp_MtID=ID_CLAlarm;
		break;
	case CV_DISPLAYH:
		*_lp_MtID=ID_CDisplayH;
		break;
	case CV_DISPLAYL:	
		*_lp_MtID=ID_CDisplayL;
		break;
	case CV_DISPLAY:
		*_lp_MtID=ID_CDisplay;
		break;
	case CV_ERROR:
		*_lp_MtID=ID_CError;
		break;
	case CV_NOILL:
		*_lp_MtID=ID_CNoIll;
		break;
	case CV_LPWRONG:	
		*_lp_MtID=ID_CLPWrong;
		break;
	case CV_HOVERCOUNT:
		*_lp_MtID=ID_CHOverCount;
		break;
	case CV_LOVERCOUNT:
		*_lp_MtID=ID_CLOverCount;
		break;
	case CV_NOLP:
		*_lp_MtID=ID_CNoLP;
		break;
	case CV_HOVER:
		*_lp_MtID=ID_CHOver;
		break;
	case CV_LOVER:
		*_lp_MtID=ID_CLOver;
		break;
	case CV_HEDGE:
		*_lp_MtID=ID_CHEdge;
		break;
	case CV_LEDGE:
		*_lp_MtID=ID_CLEdge;
		break;
	case CV_STEADYDELT:
		*_lp_MtID=ID_CSteadyDelt;
		break;
	case CV_STEADYFIELD:
		*_lp_MtID=ID_CSteadyField;
		break;
	default:
		_h_MtHandle=INVALID_TTFID;
	}
	return _h_MtHandle;
}

MTHANDLE TransMvID(WORD* _lp_ttfID,WORD* _lp_MtID)
{
	MTHANDLE _h_MtHandle=NOERROR;
	switch(*_lp_ttfID)
	{
	case MV_TAG:
		 *_lp_MtID=ID_TAG;
		 break;
	case MV_DATANAME:
		 *_lp_MtID=ID_DataName;
		 break;
    case MV_NAME:
		 *_lp_MtID=ID_Name;
		 break;
    case MV_VALUE:
		 *_lp_MtID=ID_MData;
		 break;
    case MV_UNIT:
		 *_lp_MtID=ID_Unit;
		 break;
    case MV_HSCALE:
		 *_lp_MtID=ID_MHScale;
		 break;
    case MV_LSCALE:
		 *_lp_MtID=ID_MLScale;
		 break;
    case MV_DELT:
		 *_lp_MtID=ID_MDelt;
		 break;
    case MV_SET:
		 *_lp_MtID=ID_MIrv;
		 break;
    case MV_CONTROLMODE:
		 *_lp_MtID=ID_MControl_type;
		 break;
    case MV_RCONTROLMODE:
		 *_lp_MtID=ID_MRControl_type;
		 break;
    case MV_INI:
		 *_lp_MtID=ID_MIni;
		 break;
    case MV_DELMAX:
		 *_lp_MtID=ID_MDelmax;
		 break;
    case MV_DELMIN:
		 *_lp_MtID=ID_MDelmin;
		 break;
    case MV_MAX:
		 *_lp_MtID=ID_MMax;
		 break;
    case MV_MIN:
		 *_lp_MtID=ID_MMin;
		 break;
    case MV_HIGH:
		 *_lp_MtID=ID_MIrvHigh;
		 break;
    case MV_LOW:
		 *_lp_MtID=ID_MIrvLow;
		 break;
    case MV_FMAX:
		 *_lp_MtID=ID_MFault_max;
		 break;
    case MV_FMIN:
		 *_lp_MtID=ID_MFault_min;
		 break;
    case MV_FHIGH:
		 *_lp_MtID=ID_MIrvFault_high;
		 break;
    case MV_FLOW:
		 *_lp_MtID=ID_MIrvFault_low;
		 break;
    case MV_FXSET:
		 *_lp_MtID=ID_MFault_xirv;
		 break;
    case MV_FNSET:
		 *_lp_MtID=ID_MFault_nirv;
		 break;
    case MV_FHSET:
		 *_lp_MtID=ID_MFault_hirv;
		 break;
    case MV_FLSET:
		 *_lp_MtID=ID_MFault_lirv;
		 break;
    case MV_LPINI:
		 *_lp_MtID=ID_MLPIni;
		 break;
    case MV_LPINDEX:
		 *_lp_MtID=ID_MLPIndex;
		 break;
    case MV_LPOUT:
		 *_lp_MtID=ID_MSteadOptim;
		 break;
    case MV_PRIORITY:
		 *_lp_MtID=ID_MPriority;
	 	 break;
    case MV_STATE:
		 *_lp_MtID=ID_MInvalidate;
		 break; 
    case MV_DIST:
		 *_lp_MtID=ID_MDist;
		 break;
    case MV_SATUATION:
		 *_lp_MtID=ID_MSatuation;
		 break;
    case MV_ALARM:
		 *_lp_MtID=ID_MAlarm;
		 break;
    case MV_AM:
		 *_lp_MtID=ID_MAM;
		 break;
    case MV_HOPEAM:
		 *_lp_MtID=ID_MHope_AM;
		 break;
    case MV_R:
		 *_lp_MtID=ID_MR;
		 break;
    case MV_RBROKEN:
		 *_lp_MtID=ID_MRMax;
		 break;
    case MV_S:
		 *_lp_MtID=ID_MP;
		 break;
    case MV_SL:
		 *_lp_MtID=ID_MPL;
		 break;
    case MV_SBROKEN:
		 *_lp_MtID=ID_MPMax;
		 break;
    case MV_DELR:
		 *_lp_MtID=ID_MDelR;
		 break;
    case MV_DELS:
		 *_lp_MtID=ID_MDelP;
		 break;
    case MV_REALR:
		 *_lp_MtID=ID_MRealR;
		 break;
    case MV_REALS:
		 *_lp_MtID=ID_MRealP;
		 break;
    case MV_KEYMV:
		 *_lp_MtID=ID_MKey;
		 break;
    case MV_ALARMMAX:
		 *_lp_MtID=ID_MHAlarm;
		 break;
    case MV_ALARMMIN:
		 *_lp_MtID=ID_MLAlarm;
		 break;
	case MV_MSETSATUATION:
		*_lp_MtID=ID_MSetSatuation;
		break;
	case MV_MAPARTAM:
		*_lp_MtID=ID_MApartAM;
		break;
	case MV_IRVWRONG:
		*_lp_MtID=ID_MIRVWrong;
		break;
	case MV_DISPLAYH:
		*_lp_MtID=ID_MDisplayH;
		break;
	case MV_DISPLAYL:
		*_lp_MtID=ID_MDisplayL;
		break;
	case MV_DISPLAY:
		*_lp_MtID=ID_MDisplay;
		break;
	case MV_HOVERCOUNT:
		*_lp_MtID=ID_MHOverCount;
		break;
	case MV_LOVERCOUNT:
		*_lp_MtID=ID_MLOverCount;
		break;
	case MV_HEDGE:
		*_lp_MtID=ID_MHEdge;
		break;
	case MV_LEDGE:
		*_lp_MtID=ID_MLEdge;
		break;
	case MV_SAMPLE:
		*_lp_MtID=ID_MSample;
		break;
	default:
		_h_MtHandle=INVALID_TTFID;
	}
	return _h_MtHandle;
}

MTHANDLE TransDvID(WORD* _lp_ttfID,WORD* _lp_MtID)
{
	MTHANDLE _h_MtHandle=NOERROR;
	switch(*_lp_ttfID)
	{
	case DV_TAG:
		*_lp_MtID=ID_TAG;
		break;
	case DV_DATANAME:
		*_lp_MtID=ID_DataName;
		break;
	case DV_NAME:
		*_lp_MtID=ID_Name;
		break;
	case DV_VALUE:
		*_lp_MtID=ID_DData;
		break;
	case DV_UNIT:
		*_lp_MtID=ID_Unit;
		break;
	case DV_HSCALE:
		*_lp_MtID=ID_DHScale;
		break;
	case DV_LSCALE:
		*_lp_MtID=ID_DLScale;
		break;
	case DV_INI:
		*_lp_MtID=ID_DIni;
		break;
	case DV_DELT:
		*_lp_MtID=ID_DDelt;
		break;
	case DV_HISTORY:
		*_lp_MtID=ID_DHistory;
		break;
	case DV_STATE:
		*_lp_MtID=ID_DInvalid;
		break;
	case DV_DISPLAYH:
		*_lp_MtID=ID_DDisplayH;
		break;
	case DV_DISPLAYL:
		*_lp_MtID=ID_DDisplayL;
		break;
	case DV_DISPLAY:
		*_lp_MtID=ID_DDisplay;
		break;
	default:
		 _h_MtHandle=INVALID_TTFID;

	}
	return _h_MtHandle;
}