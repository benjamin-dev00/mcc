#ifdef __ZZK
//===================================================================
//
//Zheng zhikai	95-4-22
//===================================================================
#include "gu.h"

////////////  RandInt  //////////////////////////////////////////////
//RandInt parameter order
//	char  tag_in[nin1][16]={"G205","G204","T101"};
//	char  tag_out[nout1][16]={"NAPH","LCO","LPG"};
//	char  tag_Ana[nout1][16]={"NAPH_A","LCO_A","LPG_A"};
#define SOFT_NAPH			0
#define SOFT_LCO			1
#define SOFT_LPG			2
#define SOFT_G205			3
#define SOFT_G204			4
#define SOFT_T101			5
#define SOFT_NAPHA			6
#define SOFT_LCOA			7
#define SOFT_LPGA			8

#define SOFT_NAPH_PDT			PDT_REAL
#define SOFT_LCO_PDT			PDT_REAL
#define SOFT_LPG_PDT			PDT_REAL
#define SOFT_G205_PDT			PDT_REAL
#define SOFT_G204_PDT			PDT_REAL
#define SOFT_T101_PDT			PDT_REAL
#define SOFT_NAPHA_PDT			PDT_REAL
#define SOFT_LCOA_PDT			PDT_REAL
#define SOFT_LPGA_PDT			PDT_REAL

//...................................................................
//...................................................................
_DLLEXP DWORD _DLLFUNC SoftParam(PIC wCmd_, WRD wMTU_, WORD wNo_, LONG lParam_)
{
	static PDU _sPDU[] =
	{
		_ELE_PDU(SOFT_NAPH,	"NAPH",	VSOF_REAL, PU_NORMAL,	PRI_ALL  ),
		_ELE_PDU(SOFT_LCO,	"LCO"",	VSOF_REAL, PU_NORMAL,	PRI_ALL  ),
		_ELE_PDU(SOFT_LPG,	"LPG",	VSOF_REAL, PU_NORMAL,	PRI_ALL  ),
		_ELE_PDU(SOFT_G205,	"G205",	VSOF_REAL, PU_NORMAL,	PRI_ALL  ),
		_ELE_PDU(SOFT_G204,	"G204",	VSOF_REAL, PU_NORMAL,	PRI_ALL  ),
		_ELE_PDU(SOFT_T101,	"T101",	VSOF_REAL, PU_NORMAL,	PRI_ALL  ),
		_ELE_PDU(SOFT_NAPHA,	"NAPHA",VSOF_REAL, PU_NORMAL,	PRI_ALL  ),
		_ELE_PDU(SOFT_LCOA,	"LCOA",	VSOF_REAL, PU_NORMAL,	PRI_ALL  ),
		_ELE_PDU(SOFT_LPGA,	"LPGA",	VSOF_REAL, PU_NORMAL,	PRI_ALL  ),
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
//	Algorithm:	
//...................................................................
_DLLEXP ACRET _DLLFUNC SoftCalcu(WRD wMTU_)
{

	CORE_ASSERT( ModuGetAlgo(wMTU_) == IDA_SOFT);
	CORE_ASSERT( ModuGetNumParam(wMTU_) == 9 );

	_lppu = ModuGetParameter( wMTU_ );

	SoftRBF( _lppu );
	return  ACRET_TRUE;
}
#endif