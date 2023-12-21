//===================================================================
//
//Zheng zhikai	95-10-4
//===================================================================

#ifndef __CALGO_H
#define __CALGO_H

#ifndef __CPU_H
#include "cpu.h"
#endif

/////////////////////////////
//Algorithm Table
#define LENGTH_NAME_ALGO	32
//algorithm run-time proc
typedef WORD	ACRET;		//algorithm calculate function return type

typedef ACRET (_ZZKDECL _FAR* ARTPROC)( WRD wMTU_ );

//define ARTPROC return values
#define	ACRET_BLOCK			0x1111		//return if block
#define ACRET_END			0x2222
#define ACRET_BREAK			0x3333
#define ACRET_RETURN		0x4444
#define ACRET_TRUE			TRUE		//return true(successful)
#define ACRET_FALSE			FALSE		//return false(failed)

typedef struct tagALGO_TABLE_UNIT
{
	char		szName[LENGTH_NAME_ALGO];
	IDALGO		wID;
	ARTPROC		lpfnCalcu;		//algorithm routine
	PARAMPROC	lpfnParam;		//parameter info access proc
	DWORD		Flags;
}ATU, FAR* LPATU;

#define AT_NAME			0x0001
#define AT_ID    		0x0002
#define AT_CALCUPROC	0x0003
#define AT_PARAMPROC	0x0004
#define AT_FLAGS		0x0005

//define ATU.Flags bit
#define ATF_HIDEN		0x00000001L		//hiden algorithm

/////////////
//Detail information of IDs of algorithms references to csysalgo.h
/////////////

typedef struct tagALGO_TABLE_INFO
{
	WRD			wCount;
	WRD			wAdding;
	HGLOBAL		hBase;
	LPATU 		lpBase;
}ATInfo;

_DLLEXP ATInfo FAR* _ZZKDECL GetAlgoTableInformation(void);
_DLLEXP BOOL _ZZKDECL RegisterAlgorithm(ATU FAR* lp_);
_DLLEXP BOOL _ZZKDECL UnregisterAlgorithm( IDALGO wID_ );

_DLLEXP BOOL _ZZKDECL IsAlgo(IDALGO id_);
_DLLEXP BOOL _ZZKDECL AlgoGetName(IDALGO id_, LPSTR szBuffer_, WORD wBufferSize_);

_DLLEXP DWORD _ZZKDECL AlgoGetFlags(IDALGO id_);
_DLLEXP void _ZZKDECL AlgoSetFlags(IDALGO id_, DWORD dwFlags_ );

_DLLEXP WORD _ZZKDECL AlgoGetNumParam(IDALGO id_);
_DLLEXP BOOL _ZZKDECL AlgoGetVarParamStruct(IDALGO id_, LPVPS lpVps_ );

_DLLEXP WORD _ZZKDECL AlgoGetParamPrivilege(IDALGO id_, WORD wNo_);

_DLLEXP PDT _ZZKDECL AlgoGetParamType(IDALGO id_, WORD wParam_);
_DLLEXP BOOL _ZZKDECL AlgoGetParamName(IDALGO id_, WORD wNo_, LPGETPARAMNAME lpGPN_);
_DLLEXP BOOL _ZZKDECL AlgoGetParamOrder(IDALGO id_, LPGETPARAMORDER lpGPO_);
_DLLEXP BOOL _ZZKDECL AlgoGetParamTypeName(IDALGO id_, WORD wNo_, LPGETPARAMTYPENAME lpGPTN_);

_DLLEXP BOOL _ZZKDECL AlgoEncode(IDALGO id_, WRD wModu_, WORD wNo_, LPENCODE lpDecode_);
_DLLEXP BOOL _ZZKDECL AlgoDecode(IDALGO id_, WRD wModu_, WORD wNo_, LPDECODE lpDecode_);

_DLLEXP DWORD _ZZKDECL AlgoGetValueString(IDALGO id_, WRD wModu_, WORD wNo_, LPGETVALUESTRING lpvs_);
_DLLEXP BOOL _ZZKDECL AlgoSetValueString(IDALGO id_, WRD wModu_, WORD wNo_, LPCSTR lpsz_);

_DLLEXP BOOL _ZZKDECL AlgoInitiate(IDALGO id_, WRD wModu_, LONG lStepInterval_);
_DLLEXP BOOL _ZZKDECL AlgoStop(IDALGO id_, WRD wModu_);

_DLLEXP BOOL _ZZKDECL AlgoPICCreate(IDALGO id_, WRD wModu_, LONG lData_);
_DLLEXP BOOL _ZZKDECL AlgoPICPaste(IDALGO id_, WRD wModu_);
_DLLEXP BOOL _ZZKDECL AlgoPICDelete(IDALGO id_, WRD wModu_);

//_DLLEXP PREFIX _ZZKDECL AlgoGetParamDefaultPrefix(IDALGO id_, WORD wParam_);

_DLLEXP BOOL _ZZKDECL AlgoRun(IDALGO id_, WRD wModu_);

_DLLEXP LPSTR _ZZKDECL AlgoGetData(LPSTR lpsz_, int size_, WRD wrd_, WORD order_);
_DLLEXP BOOL _ZZKDECL AlgoSetData(LPCSTR lpsz_, WRD wrd_, WORD order_);
_DLLEXP BOOL _ZZKDECL AlgoGetElementName(WORD element_, LPSTR szName_, WORD wSize_);
_DLLEXP WORD _ZZKDECL AlgoGetElement(LPSTR szName_);

_DLLEXP DWORD _ZZKDECL AlgoCallParamProc(IDALGO id_, PIC wCmd_, WRD wrd_, WORD wNo_, LONG lParam_);

//declear inner functions
BOOL RegisterSystemAlgos(void);

#endif	//__CALGO_H

