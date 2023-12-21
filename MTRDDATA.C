
////////////////////////////////////////////////////////////////
//This program complete the function reading Cv from Onspec.  //  
//S.D.Chen 97-9-11                                            //
////////////////////////////////////////////////////////////////
#include <windows.h>
#include "tfalgo.h"
#include "Mtstruct.h"
#include "Mtexport.h"

DLLEXPORT MTHANDLE ReadDataFromOnspec( WORD _i_UnitIndex, double *_lpData)
{
	
	///////////////////////////
	//
	MTHANDLE _h_MtHandle;
	BYTE    _b_Status=OFF;
	double  _db_Temp;   
	
	if((_h_MtHandle=InitOnspec())!=NOERROR)
		return FAIL_INITIALIZE_ONSPEC;

	if((_h_MtHandle=readmem(EUR,_i_UnitIndex,&_db_Temp,&_b_Status))!=NOERROR)
		return _h_MtHandle;
   	else
		*_lpData=_db_Temp;

	UNINITONSPEC();
	return NOERROR;
}


