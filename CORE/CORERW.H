//===================================================================
//
//Zheng zhikai	95-04-22
//	.h file for corerw.h
//		Core data Read/write
//===================================================================

#ifndef	__CORERW_H
#define __CORERW_H

#include "cpu.h"

#define	READPU(__lpPU_, __type_, __value_)				\
	do{                                            		\
		SimplePU	__pu_;                       		\
		__pu_.prefix = PF_IMM|IMM_##__type_;			\
		__pu_.du.zzk__FULL = NULL;						\
		PURead( &__pu_, (__lpPU_) );       				\
		(__value_) = __pu_.du.zzk__##__type_;			\
	}while(0)

#define	WRITEPU(__lpPU_, __type_, __value_)				\
	do{                                            		\
		SimplePU	__pu_;                       		\
		__pu_.prefix = PF_IMM|IMM_##__type_;			\
		__pu_.du.zzk__##__type_ = (__type_)__value_;    \
		PUWrite( (__lpPU_), &__pu_);       				\
	}while(0)

#define	MFPUREAD(__lpPUBase_, __param_, __type_, __value_)		\
	do{                                            				\
		SimplePU	__pu_;                       				\
		CORE_ASSERT( __param_##_PDT == PDT_##__type_ );			\
		__pu_.prefix = PF_IMM|IMM_##__type_;       				\
		__pu_.du.zzk__FULL = NULL;  /* while read cobj, this must be clear */ \
		PURead( &__pu_, (__lpPUBase_ + __param_) );       		\
		(__value_) = __pu_.du.zzk__##__type_;					\
	}while(0)

#define	MFPUWRITE(__lpPUBase_, __param_, __type_, __value_)		\
	do{                                            				\
		SimplePU	__pu_;                       				\
		CORE_ASSERT( __param_##_PDT == PDT_##__type_ );			\
		__pu_.prefix = PF_IMM|IMM_##__type_;       				\
		__pu_.du.zzk__##__type_ = (__type_)__value_;          	\
		PUWrite( (__lpPUBase_ + __param_), &__pu_);       		\
	}while(0)

#define PUSET(__lppu_, __prefix_, __type_, __value_)			\
	do{															\
		(__lppu_)->prefix = __prefix_;							\
		(__lppu_)->du.zzk__##__type_ = __value_;				\
		(__lppu_)->flags |= PU_RUNDIRTY;						\
		if( !CoreIsRunTime() )									\
			__lppu_->flags |= PU_DIRTY;							\
	}while(0)


_DLLEXP BOOL _ZZKDECL PUFree(LPPU lpPU);	//free the resource of PU
_DLLEXP BOOL _ZZKDECL PUCopy(LPPU to_, LPPU from_);
_DLLEXP BOOL _ZZKDECL PURead(SimplePU FAR* to_, LPPU from_);
_DLLEXP BOOL _ZZKDECL PUWrite(LPPU to_, SimplePU FAR* from_);
_DLLEXP BOOL _ZZKDECL PUReadString(LPPU lpPU_, LPSTR szBuffer_, WORD wSize_);
_DLLEXP BOOL _ZZKDECL PUWriteString(LPPU lpPU_, LPCSTR szBuffer_);

_DLLEXP BOOL _ZZKDECL PUDecode(LPPU lpPU_, LPDECODE lpDecode_);
_DLLEXP BOOL _ZZKDECL PUEncode(LPPU lpPU_, LPENCODE lpEncode_);

_DLLEXP BOOL _ZZKDECL PUIsDirty(LPPU lppu_);

BOOL _NONEXPORTED PUCast(SimplePU FAR* to_, SimplePU FAR* from_);

#endif __CORERW_H

