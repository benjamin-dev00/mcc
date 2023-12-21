//===================================================================
//
//Zheng zhikai	95-9-30
//===================================================================

#include "prehead.h"

BOOL ProvideAlgorithms(void);
BOOL ReleaseAlgorithms(void);

HINSTANCE	ghInst;

static BOOL AccessDefaultParam( BOOL bSave_ );
BOOL AccessAlgoDefaultParam( LPCSTR lpszDefName_, BOOL bSave_ );

#ifndef _WIN32
//...................................................................
//...................................................................
#pragma argsused
int FAR PASCAL LibMain (HINSTANCE hInstance_,
						WORD wDataSeg_,
						WORD cbHeapSize_,
						LPSTR lpCmdLine_)
{
	if(cbHeapSize_ != 0)
		UnlockData(0);

	if( !ProvideAlgorithms() )
		return 0;

	AccessDefaultParam( FALSE );	//load it
	return TRUE;
}

//...................................................................
//...................................................................
#pragma argsused
int FAR PASCAL WEP(int nParameter_)
{
	AccessDefaultParam( TRUE );	//save it
	ReleaseAlgorithms();
	return 1;
}

#else	//_WIN32

//...................................................................
//...................................................................
BOOL APIENTRY DllMain(HANDLE hModule, 
                      DWORD  ul_reason_for_call, 
                      LPVOID lpReserved)
{
    switch( ul_reason_for_call ) 
	{
	    case DLL_PROCESS_ATTACH:
			ghInst = hModule;
		    ProvideAlgorithms(); 
			AccessDefaultParam( FALSE );	//load it
			break;
        
		case DLL_THREAD_ATTACH:
			break;
        
		case DLL_THREAD_DETACH:
			break;
        
		case DLL_PROCESS_DETACH:
			AccessDefaultParam( TRUE );	//save it
			ReleaseAlgorithms();
			break;
	}
    return TRUE;
}

#endif	//_WIN32 or _WIN16

_DLLEXP BOOL _DLLFUNC TestAccessDefaultParam( BOOL bSave_ )
{
	return AccessDefaultParam( bSave_ );
}

static BOOL AccessDefaultParam( BOOL bSave_ )
{
	char	_szModuleName[_MAX_PATH];
	char	_szBuffer[_MAX_PATH];
	LPSTR	_pszFileName;

    GetModuleFileName( ghInst, _szModuleName, sizeof(_szModuleName) );
	GetFullPathName( _szModuleName, sizeof(_szBuffer), _szBuffer, &_pszFileName );
	strcpy( _pszFileName, "COREALGO.INI" );

	AccessAlgoDefaultParam( _szBuffer, bSave_ );

	return TRUE;
}

