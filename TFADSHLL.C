//===================================================================
//
//Zheng zhikai	95-9-30
//===================================================================
#include "tfalgo.h"
#include "tfcmdmc.h"
BOOL ReleaseTFAlgorithms(void);

HINSTANCE	ghInst;

static BOOL AccessDefaultTFParam( BOOL bSave_ );
BOOL AccessAlgoDefaultTFParam( LPCSTR lpszDefName_, BOOL bSave_ );
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

	if( !ProvideTFAlgorithms() )
		return 0;

	AccessDefaultTFParam( FALSE );	//load it
	return TRUE;
}

//...................................................................
//...................................................................
#pragma argsused
int FAR PASCAL WEP(int nParameter_)
{
	AccessDefaultTFParam( TRUE );	//save it
	ReleaseTFAlgorithms();
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
		    ProvideTFAlgorithms(); 
			RegisterTFObjects();
			AccessDefaultTFParam( FALSE );	//load it
			break;
        
		case DLL_THREAD_ATTACH:
			break;
        
		case DLL_THREAD_DETACH:
			break;
        
		case DLL_PROCESS_DETACH:
			AccessDefaultTFParam( TRUE );	//save it
			ReleaseTFAlgorithms();
			break;
	}
    return TRUE;
}

#endif	//_WIN32 or _WIN16

static BOOL AccessDefaultTFParam( BOOL bSave_ )
{
	char _szPathBuffer[_MAX_PATH];
	char _szDrive[_MAX_DRIVE];
	char _szDir[_MAX_DIR];
	char _szFName[_MAX_FNAME];
	char _szExt[_MAX_EXT];

    GetModuleFileName( ghInst, _szPathBuffer, sizeof(_szPathBuffer) );
	_splitpath( _szPathBuffer, _szDrive, _szDir, _szFName, _szExt );

	strcpy( _szFName, "TFMCCCORE" );
	strcpy( _szExt, ".INI" );
	_makepath( _szPathBuffer, _szDrive, "\\sjzmcc\\", _szFName, _szExt );
	AccessAlgoDefaultTFParam( _szPathBuffer, bSave_ );

	return TRUE;
}
