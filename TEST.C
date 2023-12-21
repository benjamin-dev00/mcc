#include <windows.h>
#include "mtdllpv.h"

int PASCAL WinMain( HANDLE hInstance, HANDLE hPrevInstance,
					LPSTR lpszCmdParam, int nCmdShow )
{
	char *_cTag="MCC_RUN_FLAG";
	BOOL Data;
	if(CreateMutex(NULL,TRUE,"onmcc.exe")==NULL || 
		GetLastError()==ERROR_ALREADY_EXISTS)
		return 1;
	do
	{
		ReadTagBoolDataFromOnspec(  _cTag, &Data);
	}while(1);

	
	return TRUE;
}