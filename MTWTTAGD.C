
////////////////////////////////////////////////////////////////
//This program complete the function reading data from Onspec.  //  
//S.D.Chen 97-9-11                                            //
////////////////////////////////////////////////////////////////
#include <windows.h>
#include "tfalgo.h"
#include "Mtstruct.h"
#include "Mtexport.h"

DLLEXPORT MTHANDLE WriteTagDataToOnspec( char *_cTag, double *_lpData)
{
	
	MTHANDLE _h_MtHandle;
	BYTE    _b_Status=OFF;
		
	if((_h_MtHandle=InitOnspec())!=NOERROR)
		return FAIL_INITIALIZE_ONSPEC;
			
	if((_h_MtHandle=writetag(_cTag,*_lpData,_b_Status))!=NOERROR)
		return _h_MtHandle;
   
	UNINITONSPEC();
	return NOERROR;
}

DLLEXPORT MTHANDLE WriteTagBoolDataToOnspec( char *_cTag, BOOL *_lpData)
{
	
	///////////////////////////
	//
	MTHANDLE _h_MtHandle;
	BYTE    _b_Status;
	double _db_Temp=0;
			
	if((_h_MtHandle=InitOnspec())!=NOERROR)
		return FAIL_INITIALIZE_ONSPEC;
			
	if(*_lpData==FALSE)
		_b_Status=OFF;
	else
		_b_Status=ON;
	if((_h_MtHandle=writetag(_cTag,_db_Temp,_b_Status))!=NOERROR)
		return _h_MtHandle;
   
	UNINITONSPEC();
	return NOERROR;
}
