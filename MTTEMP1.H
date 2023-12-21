typedef	struct	tagVARCV
{
	double	CData;
	double	CHScale;
	double	CLScale;
	double	CSetpoint;
	double	CIni;
	double	CMax;
	double	CMin;
	double	CHigh;
	double	CLow;
	double	CFault_max;
	double	CFault_min;
	double	CFault_high;
	double	CFault_low;
	double	CFault_xsetpoint;
	double	CFault_nsetpoint;
	double	CFault_hsetpoint;
	double	CFault_lsetpoint;
	double	CLPIni;
    double  CLPIndex;
	double	CSteadOptim;
	double	CQ;
	double	CQL;
	double	CQMax;
	double  CDelQ;
	double	CRealQ;	//Added by Chen
	double	CHAlarm;
	double	CLAlarm;
	double  CLastSet;
	double	CLastHigh;
	double	CLastLow;
    WORD	CControl_type;  //0 ----- Free( cancelled this state)
							//1 ----- Setpoint control;
							//2 ----- Field control;
							//3 ----- No constrained;
							//4 ----- Bad
							//5 ----- Unresonable CV( not to be used )
							//6 ----- Having corrected 5( not to be used)
	WORD	CRControl_type;	//added by chen
	WORD		CPriority;
	WORD		CNoLP;
	WORD		CFault_Priority;
	WORD		CCT_history;
	WORD		CHOverCount;
	WORD		CLOverCount;
	WORD		CDirty;
	WORD		CQDirtyCount;
    WORD		CDelayCount;
	BOOL		CConstrain_type;
	BOOL		CHOver;
	BOOL		CLOver;
    BOOL		CKey;
	BOOL		CQBroken;
	BOOL		CSDirty;
	BOOL		CAlarm;
	BOOL		CInvalid;
	BOOL		CLPWrong;
	BOOL		CLastCM;
    BOOL		CNoIll;
	// _flag_AM
						// 0-------------A
						// 1-------------M--->A
						// 2-------------A--->M
                        // 3-------------M
	BYTE		Nomeaning[6];
}VARCV, * LPVARCV;
typedef	struct	tagVARMV
{
	double	MData;
	double	MHistory;
	double	MDelt;
	double	MMax;
	double	MMin;
	double	MHScale;
	double	MLScale;
	double	MHAlarm;
	double	MLAlarm;
	double	MIrvHigh;
	double	MIrvLow;
	double	MIrvFault_high;
	double	MIrvFault_low;
	double	MFault_max;
	double	MFault_min;
	double	MFault_xirv;
	double	MFault_nirv;
	double	MFault_hirv;
	double  MFault_lirv;
	double	MDelmax;
	double	MDelmin;
	double	MIrv;
	double	MIni;
	double	MLPIni;
	double  MLPIndex;
	double	MSteadOptim;
	double	MR;
	double  MP;
	double	MPL;
	double	MDelR;
	double  MDelP;
	double	MRealR;	//added by chen
	double	MRealP;	//added by chen
	double	MRMax;
    double	MPMax;
	WORD	MControl_type;   //0 ----- Free;
							 //1 ----- IRV control;
							 //2 ----- field control;
							 //3 ----- No constrained;
							 //4 ----- Bad
							 //5 ----- SATUATION( M )
							 //6 ----- SEQ( not to be used )
							 //7 ----- DV
							 //8 ----- Fault-satuation
							 //9 ----- Unreasonable IRV,
							 //			changed from MControl_type = 1
	WORD	MRControl_type;	//added ty chen
	WORD	MPriority;
	WORD	MFault_Priority;
	WORD	MPDirtyCount;
	WORD	MRDirtyCount;
	WORD	MCT_history;
	BOOL	MConstrain_type;
	BOOL	MRDirty;
	BOOL	MPDirty;
    BOOL	MSDirty;
	BOOL	MRBroken;
	BOOL	MPBroken;
	BOOL	MInValidate;			//0--------Validate
									//1--------undetected MV
									//2--------detected MV
	BOOL	MDist;
	BOOL	MSatuation;
    BOOL	MSetSatuation;
	BOOL	MAM;
	BOOL	MHope_AM;
	BOOL	MAlarm;
	BOOL	MIRVWrong;
	BOOL	MHOver;
	BOOL	MLOver;
	BOOL	MLastCM;
	BOOL	MKey;
	WORD	MHOverCount;
	WORD	MLOverCount;
	double	MLastIRV;
	double	MLastHigh;
    double	MLastLow;
    BOOL	MRealAM;
	BYTE	Nomeaning[2];
}VARMV, * LPVARMV;
typedef	struct	tagVARDV
{
	double	DData;
	double	DIni;
	double	DHistory;
	double	DDelt;
	double	DHScale;
	double	DLScale;
	BOOL	DInvalid;
	BYTE	Nomeaning[3];
}VARDV, * LPVARDV;

typedef struct _tagLPCOBJPRIVCV{
	int		row;
	int 	col;
#ifndef __WIN32__
	BYTE		Nomeaning[4];
#endif
	LPVARCV 	data[1];
}CobjPrivCV, * LPCOBJPRIVCV;
typedef struct _tagLPCOBJPRIVMV{
	int		row;
	int 	col;
#ifndef __WIN32__
	BYTE		Nomeaning[4];
#endif
	LPVARMV 	data[1];
}CobjPrivMV, * LPCOBJPRIVMV;
typedef struct _tagLPCOBJPRIVDV{
	int		row;
	int 	col;
#ifndef __WIN32__
	BYTE		Nomeaning[4];
#endif
	LPVARDV 	data[1];
}CobjPrivDV, * LPCOBJPRIVDV;

typedef struct _tagLPCOBJPRIV{
WORD	N;
WORD	P;
WORD	M;
WORD	p;
WORD	m;
WORD	nd;
WORD	Count;
WORD	MakeParaCount;
WORD	RecountCount;
WORD	LPCount;
TINT	Freecount;
BOOL	Flag_MCVRecount;
BOOL	Flag_CControl;
BOOL	Flag_MControl;
BOOL	Flag_Fault;
BOOL	Flag_LP;
WORD	Flag_AM;
BOOL	Flag_HopeAM;
BOOL	Flag_Alarm;
BOOL	Flag_ReSP;//this flag is for the fixed index matrix
BOOL	Flag_Run;
BOOL	Flag_InterAction;
BOOL	Flag_MChange;
BOOL	Flag_LPChange;
BOOL	Flag_Recount;
BOOL	LPRecount;
double	LpK;
double	Alpha;
double	Big_Cond;
double	PBroken;
double	QBroken;
double	RBroken;
LPCOBJ	LPG;
LPCOBJ	LPA;
LPCOBJ	LPAb;
LPCOBJ	LPMul2;
LPCOBJ	LPMul3;
LPCOBJ	LPSR;
LPCOBJ	LPDSR;
LPCOBJ	LPTempSR;
LPCOBJ	LPTempDSR;
LPCOBJ 	LPPredN;
LPCOBJ	LPH;
LPCOBJ	LPMV;
LPCOBJ	LPCV;
LPCOBJ	LPDV;
LPCOBJ	LPS;
LPCOBJ	LPQ;
LPCOBJ	LPR;
LPCOBJ	LPLPPriv;
LPCOBJ	LPHistory;
LPCOBJ	LPOutSR;
LPCOBJ	LPOutDSR;
LPCOBJ	LPGlobal;
LPMATRIX	LPCVSort;
LPMATRIX	LPMVSort;
LPMATRIX	LPDeluM;
}CobjPriv, * LPCOBJPRIV;
//jasmine     (22:12):ok.:)pls water my board!!!!thanks.
