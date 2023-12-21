//===================================================================
//
//Zheng zhikai	97-1-9
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

#define STAIR_RESULT		0
#define STAIR_DATA			1
#define STAIR_DEFAULT		2

#define STAIR_RESULT_PDT	PDT_REAL
#define STAIR_DATA_PDT		PDT_REAL
#define STAIR_DEFAULT_PDT	PDT_REAL

//Var Parameter group
#define STAIR_BORDER		0
#define STAIR_VALUE			1

#define STAIR_BORDER_PDT	PDT_REAL
#define STAIR_VALUE_PDT		PDT_REAL


static PDU	sfixPDU[] = {
	_ELE_PDU(STAIR_RESULT,	"Result",	VSOF_REAL, PU_NORMAL,	PRI_ALL ),
	_ELE_PDU(STAIR_DATA,	"Data",		VSOF_REAL, PU_NORMAL,	PRI_ALL ),
	_ELE_PDU(STAIR_DEFAULT,	"Default",	VSOF_REAL, PU_NORMAL,	PRI_ALL )
};

static PDU	svarPDU[] = {
	//if data < condition , return value
	_ELE_PDU(STAIR_BORDER,	"Border",	VSOF_REAL, PU_NORMAL,	PRI_ALL ),
	_ELE_PDU(STAIR_VALUE,	"Value",	VSOF_REAL, PU_NORMAL,	PRI_ALL )
};

static const unsigned scnumVar = sizeof(svarPDU)/sizeof(svarPDU[0]);
static const unsigned scnumFix = sizeof(sfixPDU)/sizeof(sfixPDU[0]);

static VPSTRUCT	_sVPD = {
	1,			//the default number of groups of var parameters
	0,			//the low bound of var counter
	sizeof(sfixPDU)/sizeof(sfixPDU[0]),
	&( sfixPDU[0] ),
	sizeof(svarPDU)/sizeof(svarPDU[0]),
	&( svarPDU[0] )
};

//...................................................................
//...................................................................
_DLLEXP DWORD _DLLFUNC StairParam(PIC wCmd_, WRD wMTU_, WORD wNo_, LONG lParam_)
{
	if(	wCmd_ == PIC_GETICON )
		return (DWORD)LoadIcon( ghInst, MAKEINTRESOURCE(IDI_STAIR) );

	return DefProcessVarPIC(wCmd_, wMTU_, wNo_, lParam_, TBX_MT, IDA_STAIR, &_sVPD);
}

//...................................................................
//Description:
//	Algorithm:
//
//	switch ( data )
//	{
//		case ( < IfLess0 )
//			result := value0;
//			break;
//		case ( < IfLess1 )
//			result := value1;
//			break;
//		case ( < IfLess2 )
//			result := value2;
//			break;
//		... ...
//		case ( < IfLessN )
//			result := valueN;
//			break;
//		... ...
//		default:
//			result := default;
//			break;
//	}
//
//...................................................................
_DLLEXP ACRET _DLLFUNC StairCalcu(WRD wMTU_)
{
	REAL 		_data, _border, _result, _default, _value;
	unsigned 	_i;
	WORD		_wNumParam;
	LPPU		_lpPrm, _lpPrmVar;

	CORE_ASSERT( ModuGetAlgo(wMTU_) == IDA_STAIR );

	_wNumParam = ModuGetNumParam(wMTU_);
	_lpPrm = ModuGetParameter(wMTU_);

	MFPUREAD( _lpPrm, STAIR_DATA, REAL, _data );
	MFPUREAD( _lpPrm, STAIR_DEFAULT, REAL, _default );
	_result = _default;

	for(_i=0, _lpPrmVar = _lpPrm + scnumFix;
		_i < (_wNumParam - scnumFix)/ scnumVar;
		_i++, _lpPrmVar += scnumVar )
	{
		MFPUREAD( _lpPrmVar, STAIR_BORDER, REAL, _border );
		if( _data >= _border )
			continue;
		else
		{
			MFPUREAD( _lpPrmVar, STAIR_VALUE, REAL, _value );
			_result = _value;
			break;
		}
	}

	MFPUWRITE( _lpPrm, STAIR_RESULT, REAL, _result );
	return  ACRET_TRUE;
}

