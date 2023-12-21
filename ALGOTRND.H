//===================================================================
//Parameter defination for algorithm trend
//Zheng zhikai	97-1-9
//===================================================================

//Trend parameter order	- fixed partion
#define TRENDA_SAMPLETIME		0
#define	TRENDA_TOTALTIME		1
#define	TRENDA_SC				2		//step counter( decease counter )
#define	TRENDA_SCINIT			3		//initialize value of step counter
#define TRENDA_TOTALITEM		4
#define TRENDA_VALIDITEM		5
#define TRENDA_ADDING			6
#define	TRENDA_DCDIRTY			7
//Trend parameter type - fixed partion
#define TRENDA_SAMPLETIME_PDT	PDT_DWORD
#define	TRENDA_TOTALTIME_PDT	PDT_DWORD
#define	TRENDA_SC_PDT			PDT_DWORD
#define	TRENDA_SCINIT_PDT		PDT_DWORD
#define	TRENDA_TOTALITEM_PDT	PDT_WORD
#define	TRENDA_VALIDITEM_PDT	PDT_WORD
#define	TRENDA_ADDING_PDT		PDT_WORD
#define	TRENDA_DCDIRTY_PDT		PDT_DWORD

//Trend parameter order - varible partion
#define	TRENDA_DATASOURCE		0
#define	TRENDA_DATACACHE		1
#define	TRENDA_PENSTYLE			2
#define	TRENDA_PENWIDTH			3
#define	TRENDA_PENCLR			4
//Trend parameter type - varible partion
#define	TRENDA_DATASOURCE_PDT	PDT_int
#define	TRENDA_DATACACHE_PDT	PDT_FPTR
#define	TRENDA_PENSTYLE_PDT		PDT_int
#define	TRENDA_PENWIDTH_PDT		PDT_int
#define	TRENDA_PENCLR_PDT		PDT_COLORREF

_DLLEXP WORD _ZZKDECL TrendGetVarParameter(void);
_DLLEXP WORD _ZZKDECL TrendGetFixParameter(void);
