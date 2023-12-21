//===================================================================
//
//Zheng zhikai	95-4-22
//===================================================================

#ifndef __ALGODEF_H
#define __ALGODEF_H

#ifdef __cplusplus
extern "C" {
#endif

#define _DefAlgoParam(wCmd_, wrd_, wNo_, lParam_, wNum_, id_, lpPDU_)		\
	DefProcessPIC(wCmd_, wrd_, wNo_, lParam_, TBX_MT, wNum_, (DWORD)id_, lpPDU_)

//BOOL ProvideAlgorithms(void);

#define DEF_DEF( _name_ )														\
_DLLEXP 	DWORD _ZZKDECL _name_##Param(PIC wCmd_, WRD wFTU_, WORD wNo_, LONG lParam_);	\
_DLLEXP 	ACRET _ZZKDECL _name_##Calcu(WRD wModu_);


DEF_DEF(RandN)
DEF_DEF(RandR)
DEF_DEF(Stair)
DEF_DEF(Sin)
DEF_DEF(Timer)

//////////////////////////////////////////////////////
//Note	:	Add, Minus, Multiple and Divide are 
//		:	defined in Arithmetic.c
//////////////////////////////////////////////////////
DEF_DEF(AddR)
DEF_DEF(AddN)
DEF_DEF(MinusR)
DEF_DEF(MinusN)
DEF_DEF(MultiR)
DEF_DEF(DivideR)
DEF_DEF(OrB)
DEF_DEF(AndB)

DEF_DEF(CloseFwnd)
DEF_DEF(ShowFwnd)
DEF_DEF(ReplaceFwnd)

DEF_DEF(ExchangeR)
DEF_DEF(ExchangeT)
DEF_DEF(ExchangeB)

#ifdef  __cplusplus
}
#endif

#endif	//__ALGODEF_H
