
////////////////////////////////////////////////////////////////
//This program complete the function reading data from Onspec.  //  
//S.D.Chen 97-9-11                                            //
////////////////////////////////////////////////////////////////
#include <windows.h>
#include "tfalgo.h"
#include "Mtstruct.h"
#include "Mtexport.h"

DLLEXPORT MTHANDLE WriteDataToOnspec( WORD _i_UnitIndex, double *_lpData)
{
	
	///////////////////////////
	//
	MTHANDLE _h_MtHandle;
	BYTE    _b_Status=OFF;
		
	if((_h_MtHandle=InitOnspec())!=NOERROR)
		return FAIL_INITIALIZE_ONSPEC;
			
	if((_h_MtHandle=writemem(EUR,_i_UnitIndex,*_lpData,_b_Status))!=NOERROR)
		return _h_MtHandle;
   
	UNINITONSPEC();
	return NOERROR;
}


