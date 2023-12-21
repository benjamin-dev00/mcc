////////////////////////////////////////////////////////////////
//This program complete the function reading Gv from Onspec.  //  
//S.D.Chen 97-9-11                                            //
////////////////////////////////////////////////////////////////
#include <windows.h>
#include "Mtstruct.h"
#include "tfalgo.h"
MTHANDLE  DLLEXPORT ReadFileNo(WORD *);

MTHANDLE  DLLEXPORT ReadFileNo(WORD* _lp_FileNo)
{ 
	int		_i_Index;
	BYTE    _b_Status=0;
	MTHANDLE _h_MtHandle;
	
	 double  _db_Temp;    //As a template double variable while reads int or BOOL variable from Onspec.
	
	//initialize Onspec
    if((_h_MtHandle=InitOnspec())!=NOERROR)
		return FAIL_INITIALIZE_ONSPEC;
    _i_Index=FILE_NO_LOCATION;
	_h_MtHandle=readmem(EUI,_i_Index,&_db_Temp,&_b_Status);
	*_lp_FileNo=(WORD)(_db_Temp);
	UNINITONSPEC();
	return _h_MtHandle;
}


