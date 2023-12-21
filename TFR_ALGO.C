//===================================================================
//
//Zheng zhikai	95-7-8
//===================================================================

#include "tfalgo.h"
static ATU		sATUtfs[] = {
		{"MCC",   IDA_MDMCIRVD, MDMCIRVDCalcu, MDMCIRVDParam},
		{"NonButton",   IDA_NONBUTTON, NonButtonCalcu, NonButtonParam},
//		{"Soft", IDA_SOFT, SoftRBF, SoftParam },
	};

ACRET ProvideTFAlgorithms()
{
	int		_i;

	for(_i=0; _i < sizeof(sATUtfs)/sizeof(ATU); _i++)
		if(!RegisterAlgorithm(&sATUtfs[_i]) )
		{
			//ASSERT( FALSE );
			MessageBox(NULL, "error", "tfr_algo", MB_OK );
			return FALSE;
		}

	return TRUE;
}

//...................................................................
//Description:
//...................................................................
BOOL ReleaseTFAlgorithms()
{
	int		_i;

	for(_i=0; _i < sizeof(sATUtfs)/sizeof(ATU); _i++)
		UnregisterAlgorithm( sATUtfs[_i].wID );

	return TRUE;
}

BOOL AccessAlgoDefaultTFParam( LPCSTR lpszDefName_, BOOL bSave_ )
{
	BOOL	_ret = TRUE;	
	int		_wrd;
	char	_szBuffer[_MAX_PATH + LENGTH_NAME_ALGO + 2];

	for(_wrd=0; _wrd < sizeof(sATUtfs)/sizeof(ATU); _wrd++)
	{
		strcpy( _szBuffer, lpszDefName_ );
		strcpy( _szBuffer + strlen(_szBuffer) + 1, sATUtfs[_wrd].szName );
		if( !(*sATUtfs[_wrd].lpfnParam)( bSave_? PIC_SAVEDEFAULT: PIC_LOADDEFAULT, 
										0, 0, (LPARAM)_szBuffer ) )
		{
			LOGWARN2( "ZMCC.DLL %s defParam failed: %s", bSave_?"Save": "Load", sATUtfs[_wrd].szName );
			_ret = FALSE;
		}
	}

	return _ret;
}
