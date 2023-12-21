//===================================================================
//
//Zhang
//===================================================================

#include "tfalgo.h"
#include "tfcmdmc.h"

//Non Button
/////////////// //////////////////////////////////////////
//...................................................................
//...................................................................

#define		NON_NON		0

#define		NON_NON_PDT		PDT_BOOL

DWORD _DLLFUNC NonButtonParam(PIC wCmd_, WRD wMTU_, WORD wNo_, LONG lParam_)
{
	LPPU		_lppu;

	static PDU _sPDU[] =
	{
		_ELE_PDU( NON_NON, "Non", VSOF_BOOL, PU_NORMAL, PRI_ALL ),
	};

//	Switch is for auto-manul control

	switch(wCmd_)
	{
		case PIC_INITIALIZEPARAM:
		{
			_DefAlgoParam( wCmd_,
						  wMTU_,
						  wNo_,
						  lParam_,
						  sizeof(_sPDU)/sizeof(PDU),
						  IDA_NONBUTTON,
						  &(_sPDU[0]) );
			if ( wNo_ == NON_NON )
			{
				_lppu = ModuGetParameter( wMTU_ );
				MFPUWRITE( _lppu, NON_NON, 	BOOL, 0 );
			}
			return TRUE;
		}
	}

	return _DefAlgoParam( wCmd_,
						  wMTU_,
						  wNo_,
						  lParam_,
						  sizeof(_sPDU)/sizeof(PDU),
						  IDA_NONBUTTON,
						  &(_sPDU[0]) );
}

//...................................................................
//Description:
//	Algowrithm:
//...................................................................

#pragma argsused
ACRET _DLLFUNC NonButtonCalcu( WRD wMTU_ )
{
	LPPU			_lppu;
	WORD			_Non;

	CORE_ASSERT( ModuGetAlgo( wMTU_ ) == IDA_NONBUTTON );

	CORE_ASSERT( ModuGetNumParam( wMTU_ ) == 1 );
	_lppu = ModuGetParameter( wMTU_ );

	MFPUREAD( _lppu, NON_NON, BOOL, _Non );

	if( _Non == 0 )
		_Non = 1;
	else
    	_Non = 0;
	MFPUWRITE( _lppu, NON_NON, 		BOOL, _Non );

	return ACRET_TRUE;
}


