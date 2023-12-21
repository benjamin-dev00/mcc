//===================================================================
//
//Zheng zhikai	95-4-22
//===================================================================
#include "prehead.h"

#include "core.h"
#include "cmodu.h"
#include "calgo.h"
#include "corerw.h"
#include "cpu.h"

#include "algo.h"
#include "algodef.h"

#include "AlgoRes.h"

HINSTANCE	ghInst;

//#define	ADDREALN_SUM		0
//#define	ADDREALN_R			0
//#define ADDREALN_SUM_PDT	PDT_REAL
//#define ADDREALN_R_PDT		PDT_REAL


////////////  RandN  //////////////////////////////////////////////
//RandN parameter order
#define RANDN_RAND			0
//RandN parameter type
#define RANDN_RAND_PDT		PDT_TINT
//...................................................................
//...................................................................
_DLLEXP DWORD _DLLFUNC RandNParam(PIC wCmd_, WRD wMTU_, WORD wNo_, LONG lParam_)
{
	static PDU _sPDU[] =
	{
		_ELE_PDU(RANDN_RAND,	"Rand",	VSOF_TINT, PU_NORMAL,	PRI_ALL  )
	};

	if(	wCmd_ == PIC_GETICON )
		return (DWORD)LoadIcon( ghInst, MAKEINTRESOURCE(IDI_RNDN) );

	return _DefAlgoParam( wCmd_,
						  wMTU_,
						  wNo_,
						  lParam_,
						  sizeof(_sPDU)/sizeof(PDU),
						  IDA_RANDN,
						  &(_sPDU[0]) );
}

//...................................................................
//Description:
//	Algorithm:	rnd = RAND()
//...................................................................
_DLLEXP ACRET _DLLFUNC RandNCalcu(WRD wMTU_)
{
	SimplePU _spu;

	CORE_ASSERT( ModuGetAlgo(wMTU_) == IDA_RANDN);
	CORE_ASSERT( ModuGetNumParam(wMTU_) == 1);

	_spu.prefix = PF_IMM|IMM_TINT;
	_spu.du.zzk__TINT = rand();

	PUWrite( ModuGetParameter(wMTU_), &_spu);

	return  ACRET_TRUE;
}

////////////  RandR  ////////////////////////////////////////////////
//RandR parameter order
#define RANDR_RAND				0
#define RANDR_MIN				1
#define RANDR_MAX				2
//RandR parameter type
#define RANDR_RAND_PDT			PDT_REAL
#define RANDR_MIN_PDT			PDT_REAL
#define RANDR_MAX_PDT			PDT_REAL

//...................................................................
//...................................................................
_DLLEXP DWORD _DLLFUNC RandRParam(PIC wCmd_, WRD wMTU_, WORD wNo_, LONG lParam_)
{
	static PDU _sPDU[] =
	{
		_ELE_PDU(RANDR_RAND,	"Rand",		VSOF_REAL, PU_NORMAL,	PRI_ALL  ),
		_ELE_PDU(RANDR_MIN,		"Min",		VSOF_REAL, PU_NORMAL,	PRI_ALL  ),
		_ELE_PDU(RANDR_MAX,		"Max",		VSOF_REAL, PU_NORMAL,	PRI_ALL  )
	};

	if(	wCmd_ == PIC_GETICON )
		return (DWORD)LoadIcon( ghInst, MAKEINTRESOURCE(IDI_RNDR) );

	return _DefAlgoParam( wCmd_,
						  wMTU_,
						  wNo_,
						  lParam_,
						  sizeof(_sPDU)/sizeof(PDU),
						  IDA_RANDR,
						  &(_sPDU[0]) );
}

//...................................................................
//Description:
//	Algorithm:	rnd = RAND()*range/RAND_MAX + offset
//				range = max - min
//				offset = min
//...................................................................
_DLLEXP ACRET _DLLFUNC RandRCalcu(WRD wMTU_)
{
	REAL 		_range, _offset, _min, _max;
	SimplePU 	_spu;
	LPPU		_lppu;

	CORE_ASSERT( ModuGetAlgo(wMTU_) == IDA_RANDR );
	CORE_ASSERT( ModuGetNumParam(wMTU_) == 3 );

	_spu.prefix = PF_IMM|IMM_REAL;

	_lppu = ModuGetParameter(wMTU_);

	PURead( &_spu, _lppu+1 );
	_min = _spu.du.zzk__REAL;
	PURead( &_spu, _lppu+2 );
	_max = _spu.du.zzk__REAL;

	_range = _max - _min;
	_offset = _min;
	_spu.du.zzk__REAL = rand()*_range/((float)RAND_MAX) + _offset;
	PUWrite( _lppu, &_spu );

	return  ACRET_TRUE;
}

