//===================================================================
//
//Zheng zhikai	95-9-30
//===================================================================

#include "prehead.h"

BOOL ProvideStamps(void);
BOOL ReleaseStamps(void);

BOOL StampsOnLoad(void);
BOOL StampsOnExit(void);

HINSTANCE	ghInst;

static BOOL AccessDefaultParam( BOOL bSave_ );
BOOL AccessStampDefaultParam( LPCSTR lpszDefName_, BOOL bSave_ );

#ifndef _WIN32
//...................................................................
//...................................................................
#pragma argsused
int FAR PASCAL LibMain (HINSTANCE hInstance_,
						WORD wDataSeg_,
						WORD cbHeapSize_,
						LPSTR lpCmdLine_)
{
	ghInst = hInstance_;

	if(cbHeapSize_ != 0)
		UnlockData(0);

	if( !ProvideStamps() )
		return 0;
	if( !StampsOnLoad() )
		return 0;
	AccessDefaultParam( FALSE );	//load it

	return 1;
}

//...................................................................
//...................................................................
#pragma argsused
int FAR PASCAL WEP(int nParameter_)
{
	AccessDefaultParam( TRUE );	//save it
	if( !StampsOnExit() )
		return 0;

	ReleaseStamps();

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
			if( !ProvideStamps() )
			{
				LOGERR0( "ProvideStamps" );
				return 0;
			}
			if( !StampsOnLoad() )
				LOGERR0( "StampsOnLoad" );

			AccessDefaultParam( FALSE );	//load it
			break;
        
		case DLL_THREAD_ATTACH:
			break;
        
		case DLL_THREAD_DETACH:
			break;
        
		case DLL_PROCESS_DETACH:
			AccessDefaultParam( TRUE );	//save it
			if( !StampsOnExit() )
				return 0;
			ReleaseStamps();
			break;
	}
    return TRUE;
}

#endif	//_WIN32 or _WIN16

static BOOL AccessDefaultParam( BOOL bSave_ )
{
	char	_szModuleName[_MAX_PATH];
	char	_szBuffer[_MAX_PATH];
	LPSTR	_pszFileName;

    GetModuleFileName( ghInst, _szModuleName, sizeof(_szModuleName) );
	GetFullPathName( _szModuleName, sizeof(_szBuffer), _szBuffer, &_pszFileName );
	strcpy( _pszFileName, "CORESTMP.INI" );

	AccessStampDefaultParam( _szBuffer, bSave_ );

	return TRUE;
}

