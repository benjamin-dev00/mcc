//===================================================================
//
//Kevin	98-9-29
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

////////////////////////////////////////////////////////////////
//Note	:	All the suffix 'R' descript as REAL operation.
//		:	Define REAL type Macro
////////////////////////////////////////////////////////////////
#define	ARITHM_RESULT_R		0
#define	OPERAND_R			0
#define ARITHM_RESULT_R_PDT	PDT_REAL
#define OPERAND_R_PDT		PDT_REAL
////////////////////////////////////////////////////////////////
//Note	:	All the suffix 'N' descript as INTEGER operation.
//		:	Define INTEGER type Macro
////////////////////////////////////////////////////////////////
#define	ARITHM_RESULT_N		0
#define	OPERAND_N			0
#define ARITHM_RESULT_N_PDT	PDT_TINT
#define OPERAND_N_PDT		PDT_TINT
////////////////////////////////////////////////////////////////
//Note	:	All the suffix 'B' descript as BOOLEAN operation.
//		:	Define BOOLEAN type Macro
////////////////////////////////////////////////////////////////
#define	ARITHM_RESULT_B		0
#define	OPERAND_B			0
#define ARITHM_RESULT_B_PDT	PDT_TINT
#define OPERAND_B_PDT		PDT_TINT

///////////////  AddR  //////////////////////////////////////////
//...............................................................
//...............................................................
_DLLEXP DWORD _DLLFUNC AddRParam(PIC wCmd_, WRD wMTU_, WORD wNo_, LONG lParam_)
{
	static PDU	_sfixPDU[] = {
		_ELE_PDU(ARITHM_RESULT_R,	"Sum",	VSOF_REAL,	PU_NORMAL,	PRI_ALL  )
	};

	static PDU	_svarPDU[] = {
		_ELE_PDU(OPERAND_R,	"R",	VSOF_REAL,	PU_NORMAL,	PRI_ALL  )
	};

	static VPSTRUCT	_sVPD = {
		2,			//the default number of groups of var parameters
		0,			//the low bound of var counter
		sizeof( _sfixPDU ) / sizeof( _sfixPDU[0] ),
		&( _sfixPDU[0] ),
		sizeof( _svarPDU ) / sizeof( _svarPDU[0] ),
		&( _svarPDU[0] )
	};

	if(	wCmd_ == PIC_GETICON )
		return (DWORD)LoadIcon( ghInst, MAKEINTRESOURCE(IDI_ADDR) );

	return DefProcessVarPIC(wCmd_, wMTU_, wNo_, lParam_, TBX_MT, IDA_ADDR, &_sVPD);
}

//...................................................................
//Description:
//	Algorithm:	result = r0 + r1 + r2 + ... + rn
//	ai transforms to REAL and resutl is REAL at default case.
//...................................................................
_DLLEXP ACRET _DLLFUNC AddRCalcu(WRD wMTU_)
{
	REAL 		_result;
	SimplePU 	_spu;
	unsigned 	_i;
	WORD		_wNumParam;
	LPPU		_lppu0, _lppu;

	CORE_ASSERT( ModuGetAlgo(wMTU_) == IDA_ADDR );

	_spu.prefix = PF_IMM|IMM_REAL;

	_wNumParam = ModuGetNumParam(wMTU_);
	_lppu0 = ModuGetParameter(wMTU_);

	_result = 0;
	for(_i=1, _lppu = _lppu0 + 1;
		_i<_wNumParam;
		_i++, _lppu++ )	//Note: _i is from 1
	{
		PURead( &_spu, _lppu );
		_result += _spu.du.zzk__REAL;
	}

	_spu.du.zzk__REAL = _result;
	PUWrite( _lppu0, &_spu);

	return  ACRET_TRUE;
}

///////////////  AddN  //////////////////////////////////////////
//...............................................................
//...............................................................
_DLLEXP DWORD _DLLFUNC AddNParam(PIC wCmd_, WRD wMTU_, WORD wNo_, LONG lParam_)
{
	static PDU	_sfixPDU[] = {
		_ELE_PDU(ARITHM_RESULT_N,	"Sum",	VSOF_TINT,	PU_NORMAL,	PRI_ALL  )
	};

	static PDU	_svarPDU[] = {
		_ELE_PDU(OPERAND_N,	"N",	VSOF_TINT,	PU_NORMAL,	PRI_ALL  )
	};

	static VPSTRUCT	_sVPD = {
		2,			//the default number of groups of var parameters
		0,			//the low bound of var counter
		sizeof( _sfixPDU ) / sizeof( _sfixPDU[0] ),
		&( _sfixPDU[0] ),
		sizeof( _svarPDU ) / sizeof( _svarPDU[0] ),
		&( _svarPDU[0] )
	};

	if(	wCmd_ == PIC_GETICON )
		return (DWORD)LoadIcon( ghInst, MAKEINTRESOURCE(IDI_ADDN) );

	return DefProcessVarPIC(wCmd_, wMTU_, wNo_, lParam_, TBX_MT, IDA_ADDN, &_sVPD);
}

//...................................................................
//Description:
//	Algorithm:	result = n0 + n1 + n2 + ... + nn
//	ai transforms to TINT and 'result' is TINT at default case.
//...................................................................
_DLLEXP ACRET _DLLFUNC AddNCalcu(WRD wMTU_)
{
	TINT 		_result;
	SimplePU	_spu;
	unsigned 	_i;
	WORD		_wNumParam;
	LPPU		_lpPrm, _lppu;

	CORE_ASSERT( ModuGetAlgo(wMTU_) == IDA_ADDN );

	_spu.prefix = PF_IMM|IMM_TINT;

	_wNumParam = ModuGetNumParam(wMTU_);
	_lpPrm = ModuGetParameter(wMTU_);

	_result = 0;
	for(_i=1, _lppu = _lpPrm + 1;
		_i<_wNumParam;
		_i++, _lppu++ )	//Note: _i is from 1
	{
		PURead( &_spu, _lppu );
		_result += _spu.du.zzk__TINT;
	}

	_spu.du.zzk__TINT += _result;
	PUWrite( _lpPrm, &_spu );

	return  ACRET_TRUE;
}

///////////////  MinusR  //////////////////////////////////////////
//...............................................................
//...............................................................
_DLLEXP DWORD _DLLFUNC MinusRParam(PIC wCmd_, WRD wMTU_, WORD wNo_, LONG lParam_)
{
	static PDU	_sfixPDU[] = {
		_ELE_PDU(ARITHM_RESULT_R,	"Remainder",	VSOF_REAL,	PU_NORMAL,	PRI_ALL  )
	};

	static PDU	_svarPDU[] = {
		_ELE_PDU(OPERAND_R,	"R",	VSOF_REAL,	PU_NORMAL,	PRI_ALL  )
	};

	static VPSTRUCT	_sVPD = {
		2,			//the default number of groups of var parameters
		0,			//the low bound of var counter
		sizeof( _sfixPDU ) / sizeof( _sfixPDU[0] ),
		&( _sfixPDU[0] ),
		sizeof( _svarPDU ) / sizeof( _svarPDU[0] ),
		&( _svarPDU[0] )
	};

	if(	wCmd_ == PIC_GETICON )
		return (DWORD)LoadIcon( ghInst, MAKEINTRESOURCE(IDI_MINUSR) );

	return DefProcessVarPIC(wCmd_, wMTU_, wNo_, lParam_, TBX_MT, IDA_MULTIR, &_sVPD);
}

//...................................................................
//Description:
//	Algorithm:	result = r0 - r1 - ... - rn
//	ai transforms to REAL and result is REAL at default case.
//...................................................................
_DLLEXP ACRET _DLLFUNC MinusRCalcu(WRD wMTU_)
{
	REAL 		_result;
	SimplePU 	_spu;
	unsigned 	_i;
	WORD		_wNumParam;
	LPPU		_lppu0, _lppu;

	CORE_ASSERT( ModuGetAlgo(wMTU_) == IDA_MINUSR );

	_spu.prefix = PF_IMM|IMM_REAL;

	_wNumParam = ModuGetNumParam(wMTU_);
	_lppu0 = ModuGetParameter(wMTU_);

	_lppu = _lppu0 + 1;
	PURead( &_spu, _lppu );
	_result =_spu.du.zzk__REAL;

	for(_i=2 , _lppu++;
		_i<_wNumParam;
		_i++, _lppu++ )	//Note: _i is from 2
	{
		PURead( &_spu, _lppu );
		_result = _result - _spu.du.zzk__REAL;
	}

	_spu.du.zzk__REAL = _result;
	PUWrite( _lppu0, &_spu);

	return  ACRET_TRUE;
}

///////////////  MinusN  //////////////////////////////////////////
//...............................................................
//...............................................................
_DLLEXP DWORD _DLLFUNC MinusNParam(PIC wCmd_, WRD wMTU_, WORD wNo_, LONG lParam_)
{
	static PDU	_sfixPDU[] = {
		_ELE_PDU(ARITHM_RESULT_N,	"Remainder",	VSOF_TINT,	PU_NORMAL,	PRI_ALL  )
	};

	static PDU	_svarPDU[] = {
		_ELE_PDU(OPERAND_N,	"N",	VSOF_TINT,	PU_NORMAL,	PRI_ALL  )
	};

	static VPSTRUCT	_sVPD = {
		2,			//the default number of groups of var parameters
		0,			//the low bound of var counter
		sizeof( _sfixPDU ) / sizeof( _sfixPDU[0] ),
		&( _sfixPDU[0] ),
		sizeof( _svarPDU ) / sizeof( _svarPDU[0] ),
		&( _svarPDU[0] )
	};

	if(	wCmd_ == PIC_GETICON )
		return (DWORD)LoadIcon( ghInst, MAKEINTRESOURCE(IDI_MINUSN) );

	return DefProcessVarPIC(wCmd_, wMTU_, wNo_, lParam_, TBX_MT, IDA_MULTIR, &_sVPD);
}

//...................................................................
//Description:
//	Algorithm:	result = n0 - n1 - ... - nn
//	ai transforms to TINT and result is TINT at default case.
//...................................................................
_DLLEXP ACRET _DLLFUNC MinusNCalcu(WRD wMTU_)
{
	TINT 		_result;
	SimplePU 	_spu;
	unsigned 	_i;
	WORD		_wNumParam;
	LPPU		_lppu0, _lppu;

	CORE_ASSERT( ModuGetAlgo(wMTU_) == IDA_MINUSN );

	_spu.prefix = PF_IMM|IMM_TINT;

	_wNumParam = ModuGetNumParam(wMTU_);
	_lppu0 = ModuGetParameter(wMTU_);

	_lppu = _lppu0 + 1;
	PURead( &_spu, _lppu );
	_result =_spu.du.zzk__TINT;

	for(_i=2 , _lppu++;
		_i<_wNumParam;
		_i++, _lppu++ )	//Note: _i is from 2
	{
		PURead( &_spu, _lppu );
		_result = _result - _spu.du.zzk__TINT;
	}

	_spu.du.zzk__TINT = _result;
	PUWrite( _lppu0, &_spu);

	return  ACRET_TRUE;
}

///////////////  MultiR  //////////////////////////////////////////
//.................................................................
//.................................................................
_DLLEXP DWORD _DLLFUNC MultiRParam(PIC wCmd_, WRD wMTU_, WORD wNo_, LONG lParam_)
{
	static PDU	_sfixPDU[] = {
		_ELE_PDU(ARITHM_RESULT_R,	"Product",	VSOF_REAL,	PU_NORMAL,	PRI_ALL  )
	};

	static PDU	_svarPDU[] = {
		_ELE_PDU(OPERAND_R,	"R",	VSOF_REAL,	PU_NORMAL,	PRI_ALL  )
	};

	static VPSTRUCT	_sVPD = {
		2,			//the default number of groups of var parameters
		0,			//the low bound of var counter
		sizeof( _sfixPDU ) / sizeof( _sfixPDU[0] ),
		&( _sfixPDU[0] ),
		sizeof( _svarPDU ) / sizeof( _svarPDU[0] ),
		&( _svarPDU[0] )
	};

	if(	wCmd_ == PIC_GETICON )
		return (DWORD)LoadIcon( ghInst, MAKEINTRESOURCE(IDI_MULTIR) );

	return DefProcessVarPIC(wCmd_, wMTU_, wNo_, lParam_, TBX_MT, IDA_MULTIR, &_sVPD);
}

//...................................................................
//Description:
//	Algorithm:	result = r0 * r1 * ... * rn
//	ai transforms to REAL and result is REAL at default case.
//...................................................................
_DLLEXP ACRET _DLLFUNC MultiRCalcu(WRD wMTU_)
{
	REAL 		_result;
	SimplePU 	_spu;
	unsigned 	_i;
	WORD		_wNumParam;
	LPPU		_lppu0, _lppu;

	CORE_ASSERT( ModuGetAlgo(wMTU_) == IDA_MULTIR );

	_spu.prefix = PF_IMM|IMM_REAL;

	_wNumParam = ModuGetNumParam(wMTU_);
	_lppu0 = ModuGetParameter(wMTU_);

	_result = 1;
	for(_i=1, _lppu = _lppu0 + 1;
		_i<_wNumParam;
		_i++, _lppu++ )	//Note: _i is from 1
	{
		PURead( &_spu, _lppu );
		_result = _result * _spu.du.zzk__REAL;
	}

	_spu.du.zzk__REAL = _result;
	PUWrite( _lppu0, &_spu);

	return  ACRET_TRUE;
}

///////////////  DivideR  //////////////////////////////////////////
//.................................................................
//.................................................................
_DLLEXP DWORD _DLLFUNC DivideRParam(PIC wCmd_, WRD wMTU_, WORD wNo_, LONG lParam_)
{
	static PDU	_sfixPDU[] = {
		_ELE_PDU(ARITHM_RESULT_R,	"Quotient",	VSOF_REAL,	PU_NORMAL,	PRI_ALL  )
	};

	static PDU	_svarPDU[] = {
		_ELE_PDU(OPERAND_R,	"R",	VSOF_REAL,	PU_NORMAL,	PRI_ALL  )
	};

	static VPSTRUCT	_sVPD = {
		2,			//the default number of groups of var parameters
		0,			//the low bound of var counter
		sizeof( _sfixPDU ) / sizeof( _sfixPDU[0] ),
		&( _sfixPDU[0] ),
		sizeof( _svarPDU ) / sizeof( _svarPDU[0] ),
		&( _svarPDU[0] )
	};

	if(	wCmd_ == PIC_GETICON )
		return (DWORD)LoadIcon( ghInst, MAKEINTRESOURCE(IDI_DIVIDER) );

	return DefProcessVarPIC(wCmd_, wMTU_, wNo_, lParam_, TBX_MT, IDA_MULTIR, &_sVPD);
}

//...................................................................
//Description:
//	Algorithm:	result = r0 / r1 / ... / rn
//	ai transforms to REAL and result is REAL at default case.
//...................................................................
_DLLEXP ACRET _DLLFUNC DivideRCalcu(WRD wMTU_)
{
	REAL 		_result;
	SimplePU 	_spu;
	unsigned 	_i;
	WORD		_wNumParam;
	LPPU		_lppu0, _lppu;

	CORE_ASSERT( ModuGetAlgo(wMTU_) == IDA_DIVIDER );

	_spu.prefix = PF_IMM|IMM_REAL;

	_wNumParam = ModuGetNumParam(wMTU_);
	_lppu0 = ModuGetParameter(wMTU_);

	_lppu = _lppu0 + 1;
	PURead( &_spu, _lppu );
	_result = _spu.du.zzk__REAL;

	for(_i=2 , _lppu++;
		_i<_wNumParam;
		_i++ , _lppu++ )	//Note: _i is from 2
	{
		PURead( &_spu, _lppu );
		if( _spu.du.zzk__REAL == 0 )
		{
			LOGERR1( "MTU(%d): try to be divided by zero", wMTU_ );
			break;
		}
		else
			_result = _result / _spu.du.zzk__REAL;
	}

	_spu.du.zzk__REAL = _result;
	PUWrite( _lppu0, &_spu);

	return  ACRET_TRUE;
}

///////////////  ORB  //////////////////////////////////////////
//.................................................................
//.................................................................
_DLLEXP DWORD _DLLFUNC OrBParam(PIC wCmd_, WRD wMTU_, WORD wNo_, LONG lParam_)
{
	static PDU	_sfixPDU[] = {
		_ELE_PDU(ARITHM_RESULT_B,	"Or",	VSOF_BOOL,	PU_NORMAL,	PRI_ALL )
	};

	static PDU	_svarPDU[] = {
		_ELE_PDU(OPERAND_B,	"B",	VSOF_TINT,	PU_NORMAL,	PRI_ALL  )
	};

	static VPSTRUCT	_sVPD = {
		2,			//the default number of groups of var parameters
		0,			//the low bound of var counter
		sizeof( _sfixPDU ) / sizeof( _sfixPDU[0] ),
		&( _sfixPDU[0] ),
		sizeof( _svarPDU ) / sizeof( _svarPDU[0] ),
		&( _svarPDU[0] )
	};

	if(	wCmd_ == PIC_GETICON )
		return (DWORD)LoadIcon( ghInst, MAKEINTRESOURCE(IDI_ORB) );

	return DefProcessVarPIC(wCmd_, wMTU_, wNo_, lParam_, TBX_MT, IDA_ORB, &_sVPD);
}

//...................................................................
//Description:
//	Algorithm:	result = b0 || b1 || ... || bn
//	ai transforms to BOOL and result is BOOL at default case.
//...................................................................
_DLLEXP ACRET _DLLFUNC OrBCalcu(WRD wMTU_)
{
	BOOL 		_result;
	SimplePU 	_spu;
	unsigned 	_i;
	WORD		_wNumParam;
	LPPU		_lppu0, _lppu;

	CORE_ASSERT( ModuGetAlgo(wMTU_) == IDA_ORB );

	_spu.prefix = PF_IMM|IMM_BOOL;

	_wNumParam = ModuGetNumParam(wMTU_);
	_lppu0 = ModuGetParameter(wMTU_);

	_lppu = _lppu0 + 1;
	PURead( &_spu, _lppu );
	_result = _spu.du.zzk__BOOL;

	for(_i=2 , _lppu++;
		_i<_wNumParam;
		_i++ , _lppu++ )	//Note: _i is from 2
	{
		PURead( &_spu, _lppu );
		_result = _result || _spu.du.zzk__BOOL;
	}

	_spu.du.zzk__BOOL = _result;
	PUWrite( _lppu0, &_spu);

	return  ACRET_TRUE;
}


///////////////  AndB  //////////////////////////////////////////
//.................................................................
//.................................................................
_DLLEXP DWORD _DLLFUNC AndBParam(PIC wCmd_, WRD wMTU_, WORD wNo_, LONG lParam_)
{
	static PDU	_sfixPDU[] = {
		_ELE_PDU(ARITHM_RESULT_B,	"And",	VSOF_BOOL,	PU_NORMAL,	PRI_ALL  )
	};

	static PDU	_svarPDU[] = {
		_ELE_PDU(OPERAND_B,	"B",	VSOF_TINT,	PU_NORMAL,	PRI_ALL  )
	};

	static VPSTRUCT	_sVPD = {
		2,			//the default number of groups of var parameters
		0,			//the low bound of var counter
		sizeof( _sfixPDU ) / sizeof( _sfixPDU[0] ),
		&( _sfixPDU[0] ),
		sizeof( _svarPDU ) / sizeof( _svarPDU[0] ),
		&( _svarPDU[0] )
	};

	if(	wCmd_ == PIC_GETICON )
		return (DWORD)LoadIcon( ghInst, MAKEINTRESOURCE(IDI_ANDB) );

	return DefProcessVarPIC(wCmd_, wMTU_, wNo_, lParam_, TBX_MT, IDA_ORB, &_sVPD);
}

//...................................................................
//Description:
//	Algorithm:	result = b0 && b1 && ... && bn
//	ai transforms to BOOL and result is BOOL at default case.
//...................................................................
_DLLEXP ACRET _DLLFUNC AndBCalcu(WRD wMTU_)
{
	BOOL 		_result;
	SimplePU 	_spu;
	unsigned 	_i;
	WORD		_wNumParam;
	LPPU		_lppu0, _lppu;

	CORE_ASSERT( ModuGetAlgo(wMTU_) == IDA_ORB );

	_spu.prefix = PF_IMM|IMM_BOOL;

	_wNumParam = ModuGetNumParam(wMTU_);
	_lppu0 = ModuGetParameter(wMTU_);

	_lppu = _lppu0 + 1;
	PURead( &_spu, _lppu );
	_result = _spu.du.zzk__BOOL;

	for(_i=2 , _lppu++;
		_i<_wNumParam;
		_i++ , _lppu++ )	//Note: _i is from 2
	{
		PURead( &_spu, _lppu );
		_result = _result && _spu.du.zzk__BOOL;
	}

	_spu.du.zzk__BOOL = _result;
	PUWrite( _lppu0, &_spu);

	return  ACRET_TRUE;
}
