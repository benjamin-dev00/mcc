//===================================================================
//
//Zheng zhikai	98-4-15
//===================================================================

#include "_acr.h"

#define _SF_NAME	"ZZK"

BOOL APIENTRY DllMain(HANDLE hModule,
					  DWORD  ul_reason_for_call,
					  LPVOID lpReserved)
{
	switch( ul_reason_for_call )
	{
		case DLL_PROCESS_ATTACH:
			if( !_InitializeAccessCorebase( _SF_NAME ) )
				return FALSE;
			break;

		case DLL_THREAD_ATTACH:
			break;

		case DLL_THREAD_DETACH:
			break;

		case DLL_PROCESS_DETACH:
			_ExitAccessCorebase( _SF_NAME );
			break;
	}
	return TRUE;
}


