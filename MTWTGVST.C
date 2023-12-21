//////////////////////////////////////////////////////////////////
//This program complete the function writing  Gv to Onspec.		//  
//S.D.Chen 97-9-11                                              //
//////////////////////////////////////////////////////////////////
#include <windows.h>
#include "Mtstruct.h"
#include "tfalgo.h"
#include "Mtexport.h"


DLLEXPORT MTHANDLE WriteGvAllMember(LPCOBJPRIV _lpCobjPriv)
{
	
	int		_i_Index;
	int		_i_Counter;
	BYTE    _b_Status=OFF;

	MTHANDLE _h_MtHandle;
	
	double  *_lp_GvDouble;
	WORD	*_lp_GvInt;
	BOOL	*_lp_GvBool;
	double  _db_Temp = 0;    //As a template variable while writing BOOL value to Onspec.
	
	
    if((_h_MtHandle=InitOnspec())!=NOERROR)
		return FAIL_INITIALIZE_ONSPEC;

	//Initialize the pointers
	_lp_GvDouble=&(_lpCobjPriv->FIRST_GVDOUBLEMEM);
	_lp_GvInt=&(_lpCobjPriv->FIRST_GVINTMEM);
	_lp_GvBool=&(_lpCobjPriv->FIRST_GVBOOLMEM);
	
	//Write the double variables of Gv to the EUR table.
	//
	_i_Index=GV_DOUBLE_TOP; //Initialize the index
	_i_Counter=0;
    while(_i_Counter<GV_DOUBLE_NUMBER)
	{
		if ((_h_MtHandle=writemem(EUR,_i_Index,*_lp_GvDouble,_b_Status))!=NOERROR)
			return _h_MtHandle;
		else
		{
			_lp_GvDouble++;
			_i_Index++;
			_i_Counter++;
		}
	}

    //Write the int variables of gV to the EUI table.
	// 
	_i_Index=GV_INT_TOP;  //Initialize the index
	_i_Counter=0;  
    while(_i_Counter<GV_INT_NUMBER)
	{
		if(_i_Counter<GV_INT_NUMBER-2)
		{
			if ((_h_MtHandle=writemem(EUI,_i_Index,(double)*_lp_GvInt,_b_Status))!=NOERROR)
				return _h_MtHandle;
			else
			{
				_lp_GvInt++;
				_i_Index++;
			}
		}
		else if(_i_Counter==GV_INT_NUMBER-1)
		{
			if ((_h_MtHandle=writemem(EUI,_i_Index,(double)_lpCobjPriv->Freecount,_b_Status))!=NOERROR)
				return _h_MtHandle;
		}
		_i_Counter++;
	}

    //Write the BOOL variables of Gv to the DII table.
	// 
	_i_Index=GV_BOOL_TOP;  //Initialize the index
	_i_Counter=0;  
    while(_i_Counter<GV_BOOL_NUMBER)
	{
		if(_i_Counter<GV_BOOL_NUMBER-1)
		{
			if(*_lp_GvBool==FALSE)
				_b_Status=OFF;
			else
				_b_Status=ON;
			if ((_h_MtHandle=writemem(DII,_i_Index,_db_Temp,_b_Status))!=NOERROR)
				return _h_MtHandle;
			else
			{
				_lp_GvBool++;
				_i_Index++;
			}
		}
		else if(_i_Counter==GV_BOOL_NUMBER-1)
		{
			if(_lpCobjPriv->Flag_AM==FALSE)
				_b_Status=OFF;
			else
				_b_Status=ON;
			if ((_h_MtHandle=writemem(DII,_i_Index,_db_Temp,_b_Status))!=NOERROR)
				return _h_MtHandle;
		}
		_i_Counter++;
	}
    UNINITONSPEC();
	return _h_MtHandle;
}

