
/////////////////////////////////////////////////////////////////////////
//This program complete the function reading double data from Onspec.  //  
//S.D.Chen 97-9-11                                                     //
/////////////////////////////////////////////////////////////////////////
#include <windows.h>
#include "tfalgo.h"
#include "Mtstruct.h"
#include "Mtexport.h"

DLLEXPORT MTHANDLE ReadTagDataFromOnspec( char *_cTag, double *_lpData)
{
	
	///////////////////////////
	//
	MTHANDLE _h_MtHandle;
	BYTE    _b_Status=OFF;
	double  _db_Temp=*_lpData;   
	
	if((_h_MtHandle=InitOnspec())!=NOERROR)
		return FAIL_INITIALIZE_ONSPEC;

	if((_h_MtHandle=readtag(_cTag,&_db_Temp,&_b_Status))==NOERROR)
		*_lpData=_db_Temp;

	UNINITONSPEC();
	return _h_MtHandle;
	
}

DLLEXPORT MTHANDLE ReadTagBoolDataFromOnspec( char *_cTag, BOOL *_lpData)
{
	
	MTHANDLE _h_MtHandle;
	BYTE    _b_Status=OFF;
	double  _db_Temp;   
	
	if((_h_MtHandle=InitOnspec())!=NOERROR)
		return FAIL_INITIALIZE_ONSPEC;

	
	if((_h_MtHandle=readtag(_cTag,&_db_Temp,&_b_Status))==NOERROR)
	{
		if(_b_Status==ON)
			*_lpData=TRUE;
		else
			*_lpData=FALSE;
	}

	UNINITONSPEC();
	return _h_MtHandle;
	
}

