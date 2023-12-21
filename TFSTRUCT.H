// ����ע�ͣ�00-12-09

typedef	struct	tagVARCV
{
	double	CData;				//��ǰ��������	the first double member for display
	double	CHScale;			//��һ������
	double	CLScale;			//��һ������
	double	CSetpoint;			//�趨ֵ
	double	CIni;				//��ʼֵ
	double	CMax;				//��������
	double	CMin;				//��������
	double	CHigh;				//��������
	double	CLow;				//��������
	double	CFault_max;			//�ϳ���
	double	CFault_min;			//�³���
	double	CFault_high;		//�ϳ���
	double	CFault_low;			//�³���
	double	CFault_xsetpoint;	//�ϳ��޶�ֵ
	double	CFault_nsetpoint;	//�³��޶�ֵ
	double	CFault_hsetpoint;	//�ϳ��޶�ֵ
	double	CFault_lsetpoint;	//�³��޶�ֵ
	double	CLPIni;				//LP��ʼ��
	double  CLPIndex;			//LPϵ��
	double	CSteadOptim;		//LP�Ż�ֵ
	double	CQ;					//��ֵQ
	double	CQL;				//����Q
	double	CQMax;				//���Q
	double  CDelQ;				//����Q
	double	CRealQ;				//ʵ��Q
	double	CHAlarm;			//���ޱ���
	double	CLAlarm;			//���ޱ���
	double	CError;				//ʧ��
	double	CDisplayH;			//��ʾ����
	double	CDisplayL;			//��ʾ����
	double	CDisplay;			//��ʾ
	double	CSteadyDelt;		//��̬�񵴷���
	double	CSteadyField;		//�ȶ���С����	the last double member for display
	double  CLastSet;			//��һʱ���趨ֵ
	double	CLastHigh;			//��һʱ����������
	double	CLastLow;			//��һʱ����������
	double	CIniError;			//���Զ��л�Ԥ���ֵƫ��
	double	CLowFault;			//���޳���
	double  CHighFault;			//���޳���
	double  CLowSet;			//�����趨
	double  CHighSet;			//�����趨
    WORD	CRControl_type;		//ʵ�ʿ�������	added by chen
	WORD	CPriority;			//����Ȩ
	WORD	CNoLP;				//��ֹLP
	WORD	CHOverCount;		//�����޼���
	WORD	CLOverCount;		//�����޼���
	WORD	CControl_type;		//���������趨	It's BOOL type in ONSPEC
							//0 ----- Free( cancelled this state)
							//1 ----- Setpoint control;
							//2 ----- Field control;
							//3 ----- No constrained;
							//4 ----- Bad
							//5 ----- Unresonable CV( not to be used )
							//6 ----- Having corrected 5( not to be used)
	WORD	CFault_Priority;	//�ڲ�����Ȩ
	WORD	CCT_history;		//��ʱ��ʵ�ʿ�������
	WORD	CDirty;				//������������
	WORD	CQDirtyCount;		//�������������
    WORD	CDelayCount;		//���ּ���
	WORD	CBad;				//���ϴ���
	WORD	CN;					//��ʼԤ���
	BOOL	CInvalid;			//��Чλ	Added by Chen
	BOOL	CAlarm;				//����λ
	BOOL	CKey;				//�ؼ�λ
	BOOL	CHOver;				//������
	BOOL	CLOver;				//������
    BOOL	CLPWrong;			//LP�޷�ʵ��
	BOOL	CNoIll;				//��ֹȥ����̬
	BOOL	CHEdge;				//�߿���
	BOOL	CLEdge;				//�Ϳ���
	BOOL	CQMode;				//QУ��ģʽ	Added by Chen
	BOOL	CErrorMode;			//ƫ��У��ģʽ	Added by Chen δ��
	BOOL	CLimit;				//����ģʽ Added by Chen δ��
	BOOL	CQBroken;			//Q����
	BOOL	CSDirty;			//LP�Ż�
	BOOL	CLastCM;			//�ϴο����趨
	BOOL	CCM;				//�����趨
    // _flag_AM
						// 0-------------A
						// 1-------------M--->A
						// 2-------------A--->M
                        // 3-------------M
	LPCOBJ	LPName;				//������
	LPCOBJ  LPTag;				//λ��
	LPCOBJ  LPUnit;				//��λ
	LPCOBJ  LPBaseName;			//���ݿ���
	LPCOBJ  LPPriv;				//��ʷ������
	}VARCV, * LPVARCV;

typedef	struct	tagVARMV
{
	double	MData;				//����ֵ	the first double member for display
	double	MHScale;			//��һ������
	double	MLScale;			//��һ������
	double	MDelt;				//��������
	double	MIrv;				//�趨ֵ
	double	MIni;				//��ʼֵ
	double	MDelmax;			//�������
	double	MDelmin;			//��С����
	double	MMax;				//��������
	double	MMin;				//��������
	double	MIrvHigh;			//��������
	double	MIrvLow;			//��������
	double	MFault_max;			//�ϳ���
	double	MFault_min;			//�³���
	double	MIrvFault_high;		//�ϳ���
	double	MIrvFault_low;		//�³���
	double	MFault_xirv;		//�ϳ����趨
	double	MFault_nirv;		//�³����趨
	double	MFault_hirv;		//�ϳ����趨
	double  MFault_lirv;		//�³����趨
	double	MLPIni;				//LP��ʼ��
	double  MLPIndex;			//LPϵ��
	double	MSteadOptim;		//LP�Ż�ֵ
	double	MR;					//R
	double	MRMax;				//���R
    double  MP;					//P
	double	MPL;				//����P
	double	MPMax;				//���P
	double	MDelR;				//����R
	double  MDelP;				//����P
	double	MRealR;				//ʵ��R
	double	MRealP;				//ʵ��P
	double	MHAlarm;			//���ޱ���
	double	MLAlarm;			//���ޱ���
	double	MDisplayH;			//��ʾ����
	double	MDisplayL;			//��ʾ����
	double	MDisplay;			//��ʾ
	double	MSample;			//����MV  The last double member for display
	double	MSteadyField;		//�ȶ���С����	
	double	MHistory;			//��ʷ����ֵ
	double	MLastIRV;			//��ʷʵ���趨ֵ
	double	MLastHigh;			//��ʷ��������
    double	MLastLow;			//��ʷ��������
    WORD	MRControl_type;		//ʵ�ʿ�������	added ty chen
	WORD	MPriority;			//����Ȩ
	WORD	MHOverCount;		//�����޼���
	WORD	MLOverCount;		//�����޼���
	WORD	MControl_type;		//��������	it's BOOL type in ONSPEC
							 //0 ----- Free;
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
	WORD	MFault_Priority;		//�ڲ�����Ȩ
	WORD	MPDirtyCount;			//���޼���
	WORD	MRDirtyCount;			//�������޼���
	WORD	MCT_history;			//��ʷʵ�ʿ�������
	WORD	MBad;					//���ϼ���
	BOOL	MInvalidate;			//��Чλ	0--------Validate
									//			1--------undetected MV
									//			2--------detected MV
	BOOL	MDist;					//�Ŷ�λ
	BOOL	MSatuation;				//���ͱ��
    BOOL	MAlarm;					//����λ
	BOOL	MKey;					//�ؼ�λ
	BOOL	MSetSatuation;			//���ñ���
	BOOL	MIRVWrong;				//�����趨
    BOOL    MApartAM;				//��ģ�����Զ�
	BOOL	MAM;					//���Զ�
	BOOL	MHope_AM;				//ϣ�����Զ�
	BOOL	MHEdge;					//�߿���
	BOOL	MLEdge;					//�Ϳ���
	BOOL	MRDirty;				//��������
	BOOL	MPDirty;				//����
    BOOL	MSDirty;				//LP�Ż�
	BOOL	MRBroken;				//R����
	BOOL	MPBroken;				//P����
	BOOL	MHOver;					//������
	BOOL	MLOver;					//������
	BOOL	MLastCM;				//��ʷ�����趨
	BOOL	MRCorrectMode;			//RУ��ģʽ	Added by Chen δ��
	BOOL	MSCorrectMode;			//PУ��ģʽ	Added by Chen δ��
	BOOL	MErrorMode;				//ƫ��У��ģʽ	Added by Chen δ��
	BOOL	MCM;					//�����趨
	LPCOBJ	LPName;					//������
	LPCOBJ  LPTag;					//λ��
	LPCOBJ  LPUnit;					//��λ
	LPCOBJ  LPBaseName;				//���ݿ���
	LPCOBJ  LPPriv;					//��ʷ������
}VARMV, * LPVARMV;

typedef	struct	tagVARDV
{
	double	DData;				//����ֵ
	double	DHScale;			//��һ������
	double	DLScale;			//��һ������
	double	DIni;				//��ʼֵ
	double	DDelt;				//����
	double	DHistory;			//��ʷ����ֵ
	double	DDisplayH;			//��ʾ����
	double	DDisplayL;			//��ʾ����
	double	DDisplay;			//��ʾ
	double	DSteadyField;		//��̬��С����
	BOOL	DInvalid;			//��Чλ
	BOOL	DAlarm;				//����λ
	WORD	DBad;				//���ϴ���
	LPCOBJ	LPName;				//������
	LPCOBJ  LPTag;				//λ��
	LPCOBJ  LPUnit;				//��λ
	LPCOBJ  LPBaseName;			//���ݿ���
	LPCOBJ  LPPriv;				//��ʷ������
}VARDV, * LPVARDV;

typedef struct _tagLPCOBJPRIVCV{
	int		row;
	int 	col;
#ifndef _WIN32
	BYTE		Nomeaning[4];
#endif
	LPVARCV 	data[1];
}CobjPrivCV, * LPCOBJPRIVCV;

typedef struct _tagLPCOBJPRIVMV{
	int		row;
	int 	col;
#ifndef _WIN32
	BYTE		Nomeaning[4];
#endif
	LPVARMV 	data[1];
}CobjPrivMV, * LPCOBJPRIVMV;

typedef struct _tagLPCOBJPRIVDV{
	int		row;
	int 	col;
#ifndef _WIN32
	BYTE		Nomeaning[4];
#endif
	LPVARDV 	data[1];
}CobjPrivDV, * LPCOBJPRIVDV;

typedef struct _tagLPCOBJPRIV{
WORD	File_No;		//�����ļ����
WORD	N;				//ģ�ͳ���
WORD	P;				//Ԥ��ʱ��
WORD	M;				//����ʱ��
WORD	p;				//���ر�������
WORD	m;				//���ݱ�������
WORD	nd;				//�Ŷ���������
WORD	Count;			//��������
WORD	MakeParaCount;  //�����������
WORD	RecountCount;	//��̬�������
WORD	LPCount;		//��̬�Ż�����
WORD	No_Apart;		//��ģ����
WORD	MParaCount;		//Ȩ���󱣳ּ���
WORD	MParaMax;		//���Ȩ���󱣳ִ���
WORD	SteadyStep;		//���Ȳ���
WORD	ErrorCode;		//�������
WORD	Flag_AM;		//���Զ�����		it's BOOL in ONSPEC
int		Freecount;		//���ɶ�
WORD	MaxDelay;		//���ά��Լ������	don't display in ONSPEC
WORD	wModu;			//ģ�����
WORD	N1;				//Ԥ��ʱ����ʼ��		����
WORD	N2;				//Ԥ��ʱ����ֹ��		����
WORD	SteadyCount;	//��̬����
WORD	CalSteadyMax;	//��̬�������������̬����
WORD	Apart_i;		//��ģ�����
WORD	BadMax;			//���ݻ�������̴���
BOOL	Flag_CControl;	//CV���ɶȿ��Ʒ�ʽ	the first BOOL member for display
BOOL	Flag_MControl;	//MV���ɶȿ��Ʒ�ʽ	
BOOL	Flag_Fault;		//�����޷�ʽ
BOOL	Flag_LP;		//�Ƿ�������LP��־
BOOL	Flag_Alarm;		//����
BOOL	Flag_ReSP;		//Ȩϵ������	this flag is for the fixed index matrix
BOOL	Flag_SRMode;	//��Խ��Ӧ���췽ʽ
BOOL	Flag_MChange;	//ģ�ͱ��
BOOL	Flag_LPChange;	//LPģ�ͱ��
BOOL	Flag_HopeAM;	//ϣ�����Զ���ʽ
BOOL	Flag_Recount;	//��̬�滮����
BOOL	Flag_LPRecount;	//LP����
BOOL	StartMode;		//������ʽ
BOOL	Flag_Weight;	//Ȩϵ��������ʽ
BOOL	Flag_Debug;		//SYSOP/OPERATORģʽ
BOOL	Flag_HMode;		//����У����ʽ
BOOL	Flag_AM1;		//�������Զ�
BOOL	Flag_AM2;		//�������Զ�
BOOL	Flag_AM3;		//�������Զ�
BOOL	Flag_Run;		//���б�־
BOOL	Flag_Steady;	//��̬ϵ�����ɱ��
BOOL	Flag_MCVRecount;//Ȩ��������	don't display in ONSPEC
BOOL	Flag_RDebug;	//�ڲ�����λ	don't display in ONSPEC
BOOL	Flag_Save;		//�洢λ
BOOL	IniStart;		//��ʼֵ����
BOOL	Flag_CorSteady;	//��Ч��̬
BOOL	CalSteady;		//��̬ģ�͸���
BOOL	Error;			//����
double	LpK;			//LPĿ�꺯������
double	Alpha;			//����У��ϵ��
double	Big_Cond;		//���������
double	PBroken;		//���P
double	QBroken;		//���Q
double	RBroken;		//���R
double	SRError;		//ģ��ƫ��
double	DSRError;		//�Ŷ�ģ��ƫ��
double	Cor_Steady;		//��̬У��ϵ��
double  SampleTime;		//����ʱ��
double	DFilter;		//�Ŷ��˲�ϵ��
LPCOBJ	LPG;			//��̬������
LPCOBJ	LPA;			//��̬����
LPCOBJ	LPAb;			//Ԥ��У����Ծ��Ӧ
LPCOBJ	LPMul2;			//��̬�滮CVƫ��ϵ������
LPCOBJ	LPMul3;			//��̬�滮MVƫ��ϵ������
LPCOBJ	LPSR;			//��Ծ��Ӧ��
LPCOBJ	LPDSR;			//�Ŷ�ͨ����Ծ��Ӧ����
LPCOBJ	LPTempSR;		//�ڲ���Ծ��Ӧ��
LPCOBJ	LPTempDSR;		//�ڲ��Ŷ�ͨ����Ծ��Ӧ����
LPCOBJ 	LPPredN;		//CVԤ�����
LPCOBJ	LPH;			//����У��ϵ������
LPCOBJ	LPMV;			//MV����
LPCOBJ	LPCV;			//CV����
LPCOBJ	LPDV;			//DV����
LPCOBJ	LPS;			//P����
LPCOBJ	LPQ;			//Q����
LPCOBJ	LPR;			//R����
LPCOBJ	LPLPPriv;		//LP����
LPCOBJ	LPHistory;		//��ʷ��������
LPCOBJ	LPCVSort;		//CV���ȼ��������
LPCOBJ	LPMVSort;		//CV���ȼ��������
LPCOBJ	LPDeluM;		//MV��������
LPCOBJ	LPYUHis;		//����ģ�����������Ӧ
LPCOBJ	LPYDHis;		//����ģ���Ŷ������Ӧ
LPCOBJ	LPApartMV;		//��ģ��MV
LPCOBJ  LPApartCV;		//��ģ��CV
LPCOBJ  LPApartDV;		//��ģ��DV
LPCOBJ		LPApart;	//�ֿ����
LPCOBJ		LPOutSR;	//����ģ�ͽ�Խ��Ӧ
LPCOBJ		LPOutDSR;	//����ģ���Ŷ���Խ��Ӧ
LPCOBJ		LPGlobal;	//ȫ�ֲ�������, ָ���Լ�
LPCOBJ		LPINMV;		//�ڲ�MV����
LPCOBJ		LPINCV;		//�ڲ�CV����
LPCOBJ		LPINDV;		//�ڲ�DV����
LPCOBJ		LPPredU;	//����ģ��MV�洢
LPCOBJ		LPPredD;	//����ģ��DV�洢
LPCOBJ		LPIsSteady; //��̬�洢����
LPCOBJ		LPSteady;	//��̬У������
}CobjPriv, * LPCOBJPRIV;

typedef struct _tagLPCOBJAPART{
	int		row;
	int 	col;
#ifndef _WIN32
	BYTE		Nomeaning[4];
#endif
	LPCOBJPRIV data[1];
}CobjApart, * LPCOBJAPART;