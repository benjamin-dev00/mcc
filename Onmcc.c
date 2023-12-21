#include <windows.h>
#include "tfalgo.h"
#include "tfcmdmc.h"

int PASCAL WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance,
					LPSTR lpszCmdParam, int nCmdShow )
{
	HANDLE	_hMCC;

	_hMCC = CreateMutex(NULL,TRUE,"s_mcc.exe");
	if(_hMCC ==NULL || GetLastError()==ERROR_ALREADY_EXISTS)
		return 1;
#ifdef __Simul
#ifdef __FIX
	DynInitialize();
	GetGroup();
	GetBlock();
#endif
#endif
	MDMCIRVDCalcu( 0 );
#ifdef __Simul
#ifdef __FIX
	DynUninitialize();
#endif
#endif
	ReleaseMutex( _hMCC );
	return TRUE;
}
