#define		epssr					1.0e-20

#define		MAKESR_SR			0
#define		MAKESR_RANK			1
#define		MAKESR_NUM0			2
#define		MAKESR_NUM1			3
#define		MAKESR_NUM2			4
#define		MAKESR_NUM3			5
#define		MAKESR_DOM0			6
#define		MAKESR_DOM1			7
#define		MAKESR_DOM2			8
#define		MAKESR_DOM3			9
#define		MAKESR_PRIV			10
#define		MAKESR_N			11
#define		MAKESR_ORDER		12
#define		MAKESR_TD			13
#define		MAKESR_TS			14

#define 		VARSTEP_NUM_PDT		PDT_REAL
#define 		VARSTEP_DOM_PDT		PDT_REAL
#define 		MAKESR_TD_PDT  		PDT_REAL
#define 		MAKESR_TS_PDT  		PDT_REAL
#define 		MAKESR_SR_PDT  		PDT_COBJ
#define 		MAKESR_PRIV_PDT 	PDT_COBJ
#define 		MAKESR_RANK_PDT 	PDT_WORD
#define 		MAKESR_N_PDT		PDT_WORD
#define 		MAKESR_ORDER_PDT	PDT_WORD
#define 		MAKESR_NUM0_PDT   PDT_REAL
#define 		MAKESR_NUM1_PDT   PDT_REAL
#define 		MAKESR_NUM2_PDT   PDT_REAL
#define 		MAKESR_NUM3_PDT   PDT_REAL
#define 		MAKESR_DOM0_PDT   PDT_REAL
#define 		MAKESR_DOM1_PDT   PDT_REAL
#define 		MAKESR_DOM2_PDT   PDT_REAL
#define 		MAKESR_DOM3_PDT   PDT_REAL

#define		MAKEOUT_Y				0
#define		MAKEOUT_NUM0			1
#define		MAKEOUT_NUM1			2
#define		MAKEOUT_NUM2			3
#define		MAKEOUT_NUM3			4
#define		MAKEOUT_DOM0			5
#define		MAKEOUT_DOM1			6
#define		MAKEOUT_DOM2			7
#define		MAKEOUT_DOM3			8
#define		MAKEOUT_RANK			9
#define		MAKEOUT_ORDER			10
#define		MAKEOUT_HISTORY			11
#define		MAKEOUT_U				12
#define		MAKEOUT_U0				13
#define		MAKEOUT_TD				14
#define		MAKEOUT_TS				15

#define 		MAKEOUT_TD_PDT		PDT_REAL
#define 		MAKEOUT_TS_PDT		PDT_REAL
#define 		MAKEOUT_RANK_PDT	PDT_WORD
#define 		MAKEOUT_ORDER_PDT	PDT_WORD
#define 		MAKEOUT_Y_PDT		PDT_REAL
#define 		MAKEOUT_U_PDT		PDT_REAL
#define 		MAKEOUT_U0_PDT		PDT_REAL
#define 		MAKEOUT_NUM0_PDT   PDT_REAL
#define 		MAKEOUT_NUM1_PDT   PDT_REAL
#define 		MAKEOUT_NUM2_PDT   PDT_REAL
#define 		MAKEOUT_NUM3_PDT   PDT_REAL
#define 		MAKEOUT_DOM0_PDT   PDT_REAL
#define 		MAKEOUT_DOM1_PDT   PDT_REAL
#define 		MAKEOUT_DOM2_PDT   PDT_REAL
#define 		MAKEOUT_DOM3_PDT   PDT_REAL
#define 		MAKEOUT_HISTORY_PDT   PDT_COBJ

typedef struct tagPRIV_MSR
{
	WORD    flags;
	REAL    PR_RANK;
	REAL    PR_NUM0;
	REAL	PR_NUM1;
	REAL    PR_NUM2;
	REAL	PR_NUM3;
	REAL    PR_DOM0;
	REAL	PR_DOM1;
	REAL	PR_DOM2;
	REAL	PR_DOM3;
	WORD	PR_N;
	REAL	PR_TD;
    REAL	PR_TS;
}MSR, FAR* LPMSR;
