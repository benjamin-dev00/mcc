//===================================================================
//
//Zheng zhikai	95-9-30
//===================================================================

#include "Mccprehead.h"

HINSTANCE	ghInst;
BOOL ProvidetfAlgorithms(void);
BOOL RetrievetfAlgorithms(void);
BOOL RegisterTFObjects(void);
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

	if( !ProvidetfAlgorithms() )
		return 0;

	if( !RegisterTFObjects() )
		return 0;

	return TRUE;
}

//...................................................................
//...................................................................
#pragma argsused
int FAR PASCAL WEP(int nParameter_)
{
	RetrievetfAlgorithms();
	return 1;
}

