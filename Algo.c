//===================================================================
//
//Zheng zhikai	95-4-22
//===================================================================
#include "Mccprehead.h"

#include "core.h"
#include "cmodu.h"
#include "calgo.h"
#include "corerw.h"
#include "cpu.h"

#include "algo.h"
#include "algodef.h"

#define	ADDREALN_SUM		0
#define	ADDREALN_R			0
#define ADDREALN_SUM_PDT	PDT_REAL
#define ADDREALN_R_PDT		PDT_REAL

///////////////  AddRealn  //////////////////////////////////////////
//...................................................................
//...................................................................
_DLLEXP DWORD _DLLFUNC AddRealnParam(PIC wCmd_, WRD wMTU_, WORD wNo_, LONG lParam_)
{
	static PDU	_sfixPDU[] = {
		_ELE_PDU(ADDREALN_SUM,	"Sum",	VSOF_REAL,	PU_NORMAL,	PRI_ALL  )
	};

	static PDU	_svarPDU[] = {
		_ELE_PDU(ADDREALN_R,	"R",	VSOF_REAL,	PU_NORMAL,	PRI_ALL  )
	};

	static VPSTRUCT	_sVPD = {
		2,			//the default number of groups of var parameters
		0,			//the low bound of var counter
		sizeof( _sfixPDU ) / sizeof( _sfixPDU[0] ),
		&( _sfixPDU[0] ),
		sizeof( _svarPDU ) / sizeof( _svarPDU[0] ),
		&( _svarPDU[0] )
	};

	return DefProcessVarPIC(wCmd_, wMTU_, wNo_, lParam_, TBX_MT, &_sVPD);
}

//...................................................................
//Description:
//	Algorithm:	sum = r0 + r1 + r2 + ... + rn
//	ai transforms to REAL and sum is REAL at default case.
//...................................................................
_DLLEXP ACRET _DLLFUNC AddRealnCalcu(WRD wMTU_)
{
	REAL 		_sum;
	SimplePU 	_spu;
	unsigned 	_i;
	WORD		_wNumParam;
	LPPU		_lppu0, _lppu;

	CORE_ASSERT( ModuGetAlgo(wMTU_) == IDA_ADDREALN );

	_spu.prefix = PF_IMM|IMM_REAL;

	_wNumParam = ModuGetNumParam(wMTU_);
	_lppu0 = ModuGetParameter(wMTU_);

	_sum = 0;
	for(_i=1, _lppu = _lppu0 + 1;
		_i<_wNumParam;
		_i++, _lppu++ )	//Note: _i is from 1
	{
		PURead( &_spu, _lppu );
		_sum += _spu.du.zzk__REAL;
	}

	_spu.du.zzk__REAL = _sum;
	PUWrite( _lppu0, &_spu);

	return  ACRET_TRUE;
}

///////////////  AddTintn   /////////////////////////////////////////
#define	ADDTINTN_SUM		0
#define	ADDTINTN_N			0
#define ADDTINTN_SUM_PDT	PDT_REAL
#define ADDTINTN_N_PDT		PDT_REAL
//...................................................................
//...................................................................
_DLLEXP DWORD _DLLFUNC AddTintnParam(PIC wCmd_, WRD wMTU_, WORD wNo_, LONG lParam_)
{
	static PDU	_sfixPDU[] = {
		_ELE_PDU(ADDTINTN_SUM,	"Sum",	VSOF_TINT,	PU_NORMAL,	PRI_ALL  )
	};

	static PDU	_svarPDU[] = {
		_ELE_PDU(ADDTINTN_N,	"N",	VSOF_TINT,	PU_NORMAL,	PRI_ALL  )
	};

	static VPSTRUCT	_sVPD = {
		2,			//the default number of groups of var parameters
		0,			//the low bound of var counter
		sizeof( _sfixPDU ) / sizeof( _sfixPDU[0] ),
		&( _sfixPDU[0] ),
		sizeof( _svarPDU ) / sizeof( _svarPDU[0] ),
		&( _svarPDU[0] )
	};

	return DefProcessVarPIC(wCmd_, wMTU_, wNo_, lParam_, TBX_MT, &_sVPD);
}

//...................................................................
//Description:
//	Algorithm:	sum = n0 + n1 + n2 + ... + nn
//	ai transforms to TINT and 'sum' is TINT at default case.
//...................................................................
_DLLEXP ACRET _DLLFUNC AddTintnCalcu(WRD wMTU_)
{
	TINT 		_sum;
	SimplePU	_spu;
	unsigned 	_i;
	WORD		_wNumParam;
	LPPU		_lpPrm, _lppu;

	CORE_ASSERT( ModuGetAlgo(wMTU_) == IDA_ADDTINTN );

	_spu.prefix = PF_IMM|IMM_TINT;

	_wNumParam = ModuGetNumParam(wMTU_);
	_lpPrm = ModuGetParameter(wMTU_);

	_sum = 0;
	for(_i=1, _lppu = _lpPrm + 1;
		_i<_wNumParam;
		_i++, _lppu++ )	//Note: _i is from 1
	{
		PURead( &_spu, _lppu );
		_sum += _spu.du.zzk__TINT;
	}

	_spu.du.zzk__TINT += _sum;
	PUWrite( _lpPrm, &_spu );

	return  ACRET_TRUE;
}

////////////  RandInt  //////////////////////////////////////////////
//RandInt parameter order
#define RANDINT_RAND			0
//RandInt parameter type
#define RANDINT_RAND_PDT		PDT_TINT
//...................................................................
//...................................................................
_DLLEXP DWORD _DLLFUNC RandIntParam(PIC wCmd_, WRD wMTU_, WORD wNo_, LONG lParam_)
{
	static PDU _sPDU[] =
	{
		_ELE_PDU(RANDINT_RAND,	"Rand",	VSOF_TINT, PU_NORMAL,	PRI_ALL  )
	};

	return _DefAlgoParam( wCmd_,
						  wMTU_,
						  wNo_,
						  lParam_,
						  sizeof(_sPDU)/sizeof(PDU),
						  &(_sPDU[0]) );
}

//...................................................................
//Description:
//	Algorithm:	rnd = RAND()
//...................................................................
_DLLEXP ACRET _DLLFUNC RandIntCalcu(WRD wMTU_)
{
	SimplePU _spu;

	CORE_ASSERT( ModuGetAlgo(wMTU_) == IDA_RANDINT);
	CORE_ASSERT( ModuGetNumParam(wMTU_) == 1);

	_spu.prefix = PF_IMM|IMM_TINT;
	_spu.du.zzk__TINT = rand();

	PUWrite( ModuGetParameter(wMTU_), &_spu);

	return  ACRET_TRUE;
}

////////////  Randx  ////////////////////////////////////////////////
//Randx parameter order
#define RANDX_RAND				0
#define RANDX_RANGE				1
#define RANDX_OFFSET			2
//Randx parameter type
#define RANDX_RAND_PDT			PDT_REAL
#define RANDX_RANGE_PDT			PDT_REAL
#define RANDX_OFFSET_PDT		PDT_REAL

//...................................................................
//...................................................................
_DLLEXP DWORD _DLLFUNC RandxParam(PIC wCmd_, WRD wMTU_, WORD wNo_, LONG lParam_)
{
	static PDU _sPDU[] =
	{
		_ELE_PDU(RANDX_RAND,	"Rand",		VSOF_REAL, PU_NORMAL,	PRI_ALL  ),
		_ELE_PDU(RANDX_RANGE,	"Range",	VSOF_REAL, PU_NORMAL,	PRI_ALL  ),
		_ELE_PDU(RANDX_OFFSET,	"Offset",	VSOF_REAL, PU_NORMAL,	PRI_ALL  )
	};

	return _DefAlgoParam( wCmd_,
						  wMTU_,
						  wNo_,
						  lParam_,
						  sizeof(_sPDU)/sizeof(PDU),
						  &(_sPDU[0]) );
}

//...................................................................
//Description:
//	Algorithm:	rnd = RAND()*range/RAND_MAX + offset
//...................................................................
_DLLEXP ACRET _DLLFUNC RandxCalcu(WRD wMTU_)
{
	REAL 		_range, _offset;
	SimplePU 	_spu;
	LPPU		_lppu;

	CORE_ASSERT( ModuGetAlgo(wMTU_) == IDA_RANDX );
	CORE_ASSERT( ModuGetNumParam(wMTU_) == 3 );

	_spu.prefix = PF_IMM|IMM_REAL;

	_lppu = ModuGetParameter(wMTU_);

	PURead( &_spu, _lppu+1 );
	_range = _spu.du.zzk__REAL;
	PURead( &_spu, _lppu+2 );
	_offset = _spu.du.zzk__REAL;

	_spu.du.zzk__REAL = rand()*_range/((float)RAND_MAX) + _offset;
	PUWrite( _lppu, &_spu );

	return  ACRET_TRUE;
}

