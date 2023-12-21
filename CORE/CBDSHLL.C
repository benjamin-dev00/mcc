//===================================================================
//
//Zheng zhikai	95-5-11
//===================================================================

#include "prehead.h"
#include "coreall.h"

HINSTANCE	ghInst;


BOOL InitializeSystemTables(void);
void FreeSystemTables(void);

_DLLEXP BOOL _DLLFUNC AccessDefaultParameter( BOOL bSave_ );

BOOL AccessSystemAlgoDefaultParameter( LPCSTR lpszDefName_, BOOL bSave_ );
BOOL AccessSystemStampDefaultParameter( LPCSTR lpszDefName_, BOOL bSave_ );
void RegisterMessages();

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

	RegisterMessages();
	if( !InitializeSystemTables() )
		return 0;

	AccessDefaultParameter( FALSE );	//load it

	//set time zone to "zero zone"
	_putenv( "TZ=BJT-8" );
	_tzset();

	CORE_DEBUG1( "_daylight = %d\n", _daylight );
	CORE_DEBUG1( "_timezone = %ld\n", _timezone );
	CORE_DEBUG1( "_tzname[0] = %s\n", _tzname[0] );


	return 1;
}

//...................................................................
//...................................................................
#pragma argsused
int FAR PASCAL WEP(int nParameter_)
{
	if( CoreIsRunTime() )
		CoreStop();

	FreeCore();

	AccessDefaultParameter( TRUE );	//save it
	FreeSystemTables();
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

			RegisterMessages();
			if( !InitializeSystemTables() )
				return 0;

			AccessDefaultParameter( FALSE );	//load it

			//set time zone to "zero zone"
			_putenv( "TZ=EST0" );
			_tzset();

			CORE_DEBUG1( "_daylight = %d\n", _daylight );
			CORE_DEBUG1( "_timezone = %ld\n", _timezone );
			CORE_DEBUG1( "_tzname[0] = %s\n", _tzname[0] );

			break;

		case DLL_THREAD_ATTACH:
			break;

		case DLL_THREAD_DETACH:
			break;

		case DLL_PROCESS_DETACH:
			AccessDefaultParameter( TRUE );	//save it
			FreeSystemTables();
			break;
	}
    return TRUE;
}

#endif	//_WIN32 or _WIN16

_DLLEXP BOOL _DLLFUNC AccessDefaultParameter( BOOL bSave_ )
{
	char	_szModule[_MAX_PATH];
	char	_szBuffer[_MAX_PATH];
	LPSTR	_pszFileName;

    GetModuleFileName( ghInst, _szModule, sizeof(_szModule) );
	GetFullPathName( _szModule, sizeof(_szBuffer), _szBuffer, &_pszFileName );

	strcpy( _pszFileName, "csysalgo.ini" );
	AccessSystemAlgoDefaultParameter( _szBuffer, bSave_ );

	strcpy( _pszFileName, "csysstmp.ini" );
	AccessSystemStampDefaultParameter( _szBuffer, bSave_ );

	return TRUE;
}
