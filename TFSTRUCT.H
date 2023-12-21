// 添加注释，00-12-09

typedef	struct	tagVARCV
{
	double	CData;				//当前采样数据	the first double member for display
	double	CHScale;			//规一化上限
	double	CLScale;			//规一化下限
	double	CSetpoint;			//设定值
	double	CIni;				//初始值
	double	CMax;				//区域上限
	double	CMin;				//区域下限
	double	CHigh;				//区域上限
	double	CLow;				//区域下限
	double	CFault_max;			//上超限
	double	CFault_min;			//下超限
	double	CFault_high;		//上超限
	double	CFault_low;			//下超限
	double	CFault_xsetpoint;	//上超限定值
	double	CFault_nsetpoint;	//下超限定值
	double	CFault_hsetpoint;	//上超限定值
	double	CFault_lsetpoint;	//下超限定值
	double	CLPIni;				//LP初始点
	double  CLPIndex;			//LP系数
	double	CSteadOptim;		//LP优化值
	double	CQ;					//定值Q
	double	CQL;				//超限Q
	double	CQMax;				//最大Q
	double  CDelQ;				//增量Q
	double	CRealQ;				//实际Q
	double	CHAlarm;			//高限报警
	double	CLAlarm;			//低限报警
	double	CError;				//失配
	double	CDisplayH;			//显示上限
	double	CDisplayL;			//显示下限
	double	CDisplay;			//显示
	double	CSteadyDelt;		//稳态振荡幅度
	double	CSteadyField;		//稳定最小区域	the last double member for display
	double  CLastSet;			//上一时刻设定值
	double	CLastHigh;			//上一时刻区域上限
	double	CLastLow;			//上一时刻区域下限
	double	CIniError;			//手自动切换预测初值偏差
	double	CLowFault;			//低限超限
	double  CHighFault;			//高限超限
	double  CLowSet;			//低限设定
	double  CHighSet;			//高限设定
    WORD	CRControl_type;		//实际控制类型	added by chen
	WORD	CPriority;			//优先权
	WORD	CNoLP;				//禁止LP
	WORD	CHOverCount;		//超高限计数
	WORD	CLOverCount;		//超低限计数
	WORD	CControl_type;		//控制类型设定	It's BOOL type in ONSPEC
							//0 ----- Free( cancelled this state)
							//1 ----- Setpoint control;
							//2 ----- Field control;
							//3 ----- No constrained;
							//4 ----- Bad
							//5 ----- Unresonable CV( not to be used )
							//6 ----- Having corrected 5( not to be used)
	WORD	CFault_Priority;	//内部优先权
	WORD	CCT_history;		//上时刻实际控制类型
	WORD	CDirty;				//矩阵逆重算标记
	WORD	CQDirtyCount;		//矩阵逆重算计数
    WORD	CDelayCount;		//保持计数
	WORD	CBad;				//故障次数
	WORD	CN;					//起始预测点
	BOOL	CInvalid;			//无效位	Added by Chen
	BOOL	CAlarm;				//报警位
	BOOL	CKey;				//关键位
	BOOL	CHOver;				//超高限
	BOOL	CLOver;				//超低限
    BOOL	CLPWrong;			//LP无法实现
	BOOL	CNoIll;				//禁止去除病态
	BOOL	CHEdge;				//高卡边
	BOOL	CLEdge;				//低卡边
	BOOL	CQMode;				//Q校正模式	Added by Chen
	BOOL	CErrorMode;			//偏差校正模式	Added by Chen 未用
	BOOL	CLimit;				//超限模式 Added by Chen 未用
	BOOL	CQBroken;			//Q饱和
	BOOL	CSDirty;			//LP优化
	BOOL	CLastCM;			//上次控制设定
	BOOL	CCM;				//控制设定
    // _flag_AM
						// 0-------------A
						// 1-------------M--->A
						// 2-------------A--->M
                        // 3-------------M
	LPCOBJ	LPName;				//变量名
	LPCOBJ  LPTag;				//位号
	LPCOBJ  LPUnit;				//单位
	LPCOBJ  LPBaseName;			//数据库名
	LPCOBJ  LPPriv;				//历史参数表
	}VARCV, * LPVARCV;

typedef	struct	tagVARMV
{
	double	MData;				//采样值	the first double member for display
	double	MHScale;			//规一化上限
	double	MLScale;			//规一化下限
	double	MDelt;				//控制增量
	double	MIrv;				//设定值
	double	MIni;				//初始值
	double	MDelmax;			//最大增量
	double	MDelmin;			//最小增量
	double	MMax;				//区域上限
	double	MMin;				//区域下限
	double	MIrvHigh;			//区域上限
	double	MIrvLow;			//区域下限
	double	MFault_max;			//上超限
	double	MFault_min;			//下超限
	double	MIrvFault_high;		//上超限
	double	MIrvFault_low;		//下超限
	double	MFault_xirv;		//上超限设定
	double	MFault_nirv;		//下超限设定
	double	MFault_hirv;		//上超限设定
	double  MFault_lirv;		//下超限设定
	double	MLPIni;				//LP初始点
	double  MLPIndex;			//LP系数
	double	MSteadOptim;		//LP优化值
	double	MR;					//R
	double	MRMax;				//最大R
    double  MP;					//P
	double	MPL;				//超限P
	double	MPMax;				//最大P
	double	MDelR;				//增量R
	double  MDelP;				//增量P
	double	MRealR;				//实际R
	double	MRealP;				//实际P
	double	MHAlarm;			//高限报警
	double	MLAlarm;			//低限报警
	double	MDisplayH;			//显示上限
	double	MDisplayL;			//显示下限
	double	MDisplay;			//显示
	double	MSample;			//采样MV  The last double member for display
	double	MSteadyField;		//稳定最小区域	
	double	MHistory;			//历史采样值
	double	MLastIRV;			//历史实际设定值
	double	MLastHigh;			//历史区域上限
    double	MLastLow;			//历史区域下限
    WORD	MRControl_type;		//实际控制类型	added ty chen
	WORD	MPriority;			//有限权
	WORD	MHOverCount;		//超高限计数
	WORD	MLOverCount;		//超低限计数
	WORD	MControl_type;		//控制类型	it's BOOL type in ONSPEC
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
	WORD	MFault_Priority;		//内部有限权
	WORD	MPDirtyCount;			//超限计数
	WORD	MRDirtyCount;			//增量超限计数
	WORD	MCT_history;			//历史实际控制类型
	WORD	MBad;					//故障计数
	BOOL	MInvalidate;			//无效位	0--------Validate
									//			1--------undetected MV
									//			2--------detected MV
	BOOL	MDist;					//扰动位
	BOOL	MSatuation;				//饱和标记
    BOOL	MAlarm;					//报警位
	BOOL	MKey;					//关键位
	BOOL	MSetSatuation;			//设置饱和
	BOOL	MIRVWrong;				//错误设定
    BOOL    MApartAM;				//子模块手自动
	BOOL	MAM;					//手自动
	BOOL	MHope_AM;				//希望手自动
	BOOL	MHEdge;					//高卡边
	BOOL	MLEdge;					//低卡边
	BOOL	MRDirty;				//增量超限
	BOOL	MPDirty;				//超限
    BOOL	MSDirty;				//LP优化
	BOOL	MRBroken;				//R饱和
	BOOL	MPBroken;				//P饱和
	BOOL	MHOver;					//超高限
	BOOL	MLOver;					//超低限
	BOOL	MLastCM;				//历史控制设定
	BOOL	MRCorrectMode;			//R校正模式	Added by Chen 未用
	BOOL	MSCorrectMode;			//P校正模式	Added by Chen 未用
	BOOL	MErrorMode;				//偏差校正模式	Added by Chen 未用
	BOOL	MCM;					//控制设定
	LPCOBJ	LPName;					//变量名
	LPCOBJ  LPTag;					//位号
	LPCOBJ  LPUnit;					//单位
	LPCOBJ  LPBaseName;				//数据库名
	LPCOBJ  LPPriv;					//历史参数表
}VARMV, * LPVARMV;

typedef	struct	tagVARDV
{
	double	DData;				//采样值
	double	DHScale;			//规一化上限
	double	DLScale;			//规一化下限
	double	DIni;				//初始值
	double	DDelt;				//增量
	double	DHistory;			//历史采样值
	double	DDisplayH;			//显示上限
	double	DDisplayL;			//显示下限
	double	DDisplay;			//显示
	double	DSteadyField;		//稳态最小区域
	BOOL	DInvalid;			//无效位
	BOOL	DAlarm;				//报警位
	WORD	DBad;				//故障次数
	LPCOBJ	LPName;				//变量名
	LPCOBJ  LPTag;				//位号
	LPCOBJ  LPUnit;				//单位
	LPCOBJ  LPBaseName;			//数据库名
	LPCOBJ  LPPriv;				//历史参数表
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
WORD	File_No;		//仿真文件序号
WORD	N;				//模型长度
WORD	P;				//预测时域
WORD	M;				//控制时域
WORD	p;				//被控变量个数
WORD	m;				//操纵变量个数
WORD	nd;				//扰动变量个数
WORD	Count;			//控制周期
WORD	MakeParaCount;  //矩阵重算计数
WORD	RecountCount;	//动态重算计数
WORD	LPCount;		//稳态优化次数
WORD	No_Apart;		//字模块数
WORD	MParaCount;		//权矩阵保持计数
WORD	MParaMax;		//最大权矩阵保持次数
WORD	SteadyStep;		//判稳步数
WORD	ErrorCode;		//错误代码
WORD	Flag_AM;		//手自动开关		it's BOOL in ONSPEC
int		Freecount;		//自由度
WORD	MaxDelay;		//最大维持约束次数	don't display in ONSPEC
WORD	wModu;			//模块序号
WORD	N1;				//预测时域起始点		不用
WORD	N2;				//预测时域中止点		不用
WORD	SteadyCount;	//稳态次数
WORD	CalSteadyMax;	//稳态计算的条件数病态下限
WORD	Apart_i;		//字模块序号
WORD	BadMax;			//数据坏最大容忍次数
BOOL	Flag_CControl;	//CV自由度控制方式	the first BOOL member for display
BOOL	Flag_MControl;	//MV自由度控制方式	
BOOL	Flag_Fault;		//虚拟限方式
BOOL	Flag_LP;		//是否进行组合LP标志
BOOL	Flag_Alarm;		//报警
BOOL	Flag_ReSP;		//权系数重设	this flag is for the fixed index matrix
BOOL	Flag_SRMode;	//阶越响应构造方式
BOOL	Flag_MChange;	//模型变更
BOOL	Flag_LPChange;	//LP模型变更
BOOL	Flag_HopeAM;	//希望手自动方式
BOOL	Flag_Recount;	//动态规划重算
BOOL	Flag_LPRecount;	//LP重算
BOOL	StartMode;		//启动方式
BOOL	Flag_Weight;	//权系数调整方式
BOOL	Flag_Debug;		//SYSOP/OPERATOR模式
BOOL	Flag_HMode;		//反馈校正方式
BOOL	Flag_AM1;		//反再手自动
BOOL	Flag_AM2;		//分馏手自动
BOOL	Flag_AM3;		//吸稳手自动
BOOL	Flag_Run;		//运行标志
BOOL	Flag_Steady;	//稳态系数生成标记
BOOL	Flag_MCVRecount;//权矩阵重组	don't display in ONSPEC
BOOL	Flag_RDebug;	//内部调试位	don't display in ONSPEC
BOOL	Flag_Save;		//存储位
BOOL	IniStart;		//初始值启动
BOOL	Flag_CorSteady;	//有效稳态
BOOL	CalSteady;		//稳态模型更正
BOOL	Error;			//出错
double	LpK;			//LP目标函数常数
double	Alpha;			//反馈校正系数
double	Big_Cond;		//最大条件数
double	PBroken;		//最大P
double	QBroken;		//最大Q
double	RBroken;		//最大R
double	SRError;		//模型偏差
double	DSRError;		//扰动模型偏差
double	Cor_Steady;		//稳态校正系数
double  SampleTime;		//采样时间
double	DFilter;		//扰动滤波系数
LPCOBJ	LPG;			//稳态增益阵
LPCOBJ	LPA;			//动态矩阵
LPCOBJ	LPAb;			//预测校正阶跃响应
LPCOBJ	LPMul2;			//动态规划CV偏差系数矩阵
LPCOBJ	LPMul3;			//动态规划MV偏差系数矩阵
LPCOBJ	LPSR;			//阶跃响应阵
LPCOBJ	LPDSR;			//扰动通道阶跃响应矩阵
LPCOBJ	LPTempSR;		//内部阶跃响应阵
LPCOBJ	LPTempDSR;		//内部扰动通道阶跃响应矩阵
LPCOBJ 	LPPredN;		//CV预测矩阵
LPCOBJ	LPH;			//反馈校正系数矩阵
LPCOBJ	LPMV;			//MV矩阵
LPCOBJ	LPCV;			//CV矩阵
LPCOBJ	LPDV;			//DV矩阵
LPCOBJ	LPS;			//P矩阵
LPCOBJ	LPQ;			//Q矩阵
LPCOBJ	LPR;			//R矩阵
LPCOBJ	LPLPPriv;		//LP矩阵
LPCOBJ	LPHistory;		//历史参数矩阵
LPCOBJ	LPCVSort;		//CV优先级排序矩阵
LPCOBJ	LPMVSort;		//CV优先级排序矩阵
LPCOBJ	LPDeluM;		//MV增量矩阵
LPCOBJ	LPYUHis;		//仿真模型输入输出相应
LPCOBJ	LPYDHis;		//仿真模型扰动输出相应
LPCOBJ	LPApartMV;		//子模块MV
LPCOBJ  LPApartCV;		//子模块CV
LPCOBJ  LPApartDV;		//子模块DV
LPCOBJ		LPApart;	//分块矩阵
LPCOBJ		LPOutSR;	//仿真模型阶越响应
LPCOBJ		LPOutDSR;	//仿真模型扰动阶越响应
LPCOBJ		LPGlobal;	//全局参数矩阵, 指向自己
LPCOBJ		LPINMV;		//内部MV矩阵
LPCOBJ		LPINCV;		//内部CV矩阵
LPCOBJ		LPINDV;		//内部DV矩阵
LPCOBJ		LPPredU;	//仿真模型MV存储
LPCOBJ		LPPredD;	//仿真模型DV存储
LPCOBJ		LPIsSteady; //稳态存储矩阵
LPCOBJ		LPSteady;	//稳态校正矩阵
}CobjPriv, * LPCOBJPRIV;

typedef struct _tagLPCOBJAPART{
	int		row;
	int 	col;
#ifndef _WIN32
	BYTE		Nomeaning[4];
#endif
	LPCOBJPRIV data[1];
}CobjApart, * LPCOBJAPART;
