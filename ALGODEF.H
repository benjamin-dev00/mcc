//===================================================================
//
//Zheng zhikai	95-4-22
//===================================================================

#define _DefAlgoParam(wCmd_, wrd_, wNo_, lParam_, wNum_, id_, lpPDU_)		\
	DefProcessPIC(wCmd_, wrd_, wNo_, lParam_, TBX_MT, wNum_, (DWORD)id_, lpPDU_)

//BOOL ProvideAlgorithms(void);

#define DEF_DEF( _name_ )														\
_DLLEXP 	DWORD _ZZKDECL _name_##Param(PIC wCmd_, WRD wFTU_, WORD wNo_, LONG lParam_);	\
_DLLEXP 	ACRET _ZZKDECL _name_##Calcu(WRD wModu_);

DEF_DEF(AddRealn)
DEF_DEF(AddTintn)
DEF_DEF(RandInt)
DEF_DEF(Randx)
DEF_DEF(Stair)
DEF_DEF(Sine)
DEF_DEF(Timer)

DEF_DEF(CloseFwnd)
DEF_DEF(ShowFwnd)
DEF_DEF(ReplaceFwnd)

DEF_DEF(CBDXReal)
DEF_DEF(CBDXTint)
DEF_DEF(CBDXBool)

