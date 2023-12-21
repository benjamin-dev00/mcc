////////////////////////////////////////////////////////////////
//This program complete the function reading Gv from Onspec.  //  
//S.D.Chen 97-9-11                                            //
////////////////////////////////////////////////////////////////
#include <windows.h>
#include "Mtstruct.h"
#include "tfalgo.h"
#include "Mtexport.h"
DLLEXPORT MTHANDLE ReadGvAllMember(LPCOBJPRIV _lp_CobjPriv)
{
	int		_i_Counter;
	int		_i_Index;
	BYTE    _b_Status=OFF;
	
    MTHANDLE _h_MtHandle;
		
	double  *_lp_GvDouble;
	WORD    *_lp_GvInt;	
    BOOL	*_lp_GvBool;
	double  _db_Temp;    //As a template double variable while reads int or BOOL variable from Onspec.
	

	//Initialize Onspec
	if((_h_MtHandle=InitOnspec())!=NOERROR)
		return FAIL_INITIALIZE_ONSPEC;

	//Initialize the pointers
	_lp_GvDouble=&(_lp_CobjPriv->FIRST_GVDOUBLEMEM);
	_lp_GvInt=&(_lp_CobjPriv->FIRST_GVINTMEM);
	_lp_GvBool=&(_lp_CobjPriv->FIRST_GVBOOLMEM);
	
	//Read data from the EUR table to the double variables of Gv.
	//
	_i_Index=GV_DOUBLE_TOP; //Initialize the index
	_i_Counter=0;
    while(_i_Counter<GV_DOUBLE_NUMBER)
	{
		if ((_h_MtHandle=readmem(EUR,_i_Index,_lp_GvDouble,&_b_Status))!=NOERROR)
			return _h_MtHandle;
		else
		{
			_lp_GvDouble++;
			_i_Index++;
			_i_Counter++;
		}
	}

    
	//Read data from the EUI table to the int variables of Gv.
	// 
	_i_Index=GV_INT_TOP;  //Initialize the index
	_i_Counter=0;  
    while(_i_Counter<GV_INT_NUMBER)
	{
		if(_i_Counter<GV_INT_NUMBER-1)
		{
			if ((_h_MtHandle=readmem(EUI,_i_Index,&_db_Temp,&_b_Status))!=NOERROR)
				return _h_MtHandle;
			else
			{
				*_lp_GvInt=(WORD)(_db_Temp);
				_lp_GvInt++;
				_i_Index++;
			}
		}
		else if(_i_Counter==GV_INT_NUMBER-1)
		{
			if ((_h_MtHandle=readmem(EUI,_i_Index,&_db_Temp,&_b_Status))!=NOERROR)
				return _h_MtHandle;
			else
				_lp_CobjPriv->Freecount=(int)(_db_Temp);
		}
		_i_Counter++;
	}
    
    //Read data from the DII table to the BOOL variables of Gv.
	// 
	_i_Index=GV_BOOL_TOP;  //Initialize the index
	_i_Counter=0;  
    while(_i_Counter<GV_BOOL_NUMBER)
	{
		if ((_h_MtHandle=readmem(DII,_i_Index,&_db_Temp,&_b_Status))!=NOERROR)
			return _h_MtHandle;
		else if(_i_Counter<GV_BOOL_NUMBER-1)
		{
			if((_b_Status&ON)==ON)
			    *_lp_GvBool=TRUE;
			else
				*_lp_GvBool=FALSE;
			_lp_GvBool++;
			_i_Index++;
		}
		else if(_i_Counter==GV_BOOL_NUMBER-1)
		{
			if((_b_Status&ON)==ON)
			    _lp_CobjPriv->Flag_AM=TRUE;
			else
				_lp_CobjPriv->Flag_AM=FALSE;
		}
		_i_Counter++;
	}		
	UNINITONSPEC();
	return _h_MtHandle;
}


