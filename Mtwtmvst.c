//////////////////////////////////////////////////////////////////
//This program complete the function writing  Mv to Onspec.		//  
//S.D.Chen 97-9-11                                              //
//////////////////////////////////////////////////////////////////
#include <windows.h>
#include "Mtstruct.h"
#include "tfalgo.h"
#include "Mtexport.h"

DLLEXPORT MTHANDLE WriteMvAllMember( LPVARMV _lp_VarMv, WORD _i_MvIndex )
{
	int		_i_Index;
	int		_i_Counter;
	BYTE    _b_Status=OFF;

	MTHANDLE _h_MtHandle;
	
	double  *_lp_MvDouble;
	WORD    *_lp_MvInt;	
    //char	*_lp_MvChar;
	BOOL	*_lp_MvBool;
	double  _db_Temp = 0;    //As a template variable while writing BOOL value to Onspec.
	
	if((_h_MtHandle=InitOnspec())!=NOERROR)
		return FAIL_INITIALIZE_ONSPEC;

	//Initialize the pointers
	//_lp_MvChar=_lp_VarMv->FIRST_MVCHARMEM;
	_lp_MvDouble=&(_lp_VarMv->FIRST_MVDOUBLEMEM);
	_lp_MvInt=&(_lp_VarMv->FIRST_MVINTMEM);
	_lp_MvBool=&(_lp_VarMv->FIRST_MVBOOLMEM);
	
	//Write the double variables of Mv to the EUR table.
	//
	_i_Index=MV_DOUBLE_TOP+(_i_MvIndex*MAXNUM_MV_DBMEM); //Initialize the index
	_i_Counter=0;
    while(_i_Counter<MV_DOUBLE_NUMBER)
	{
		if ((_h_MtHandle=writemem(EUR,_i_Index,*_lp_MvDouble,_b_Status))!=NOERROR)
			return _h_MtHandle;
		else
		{
			_lp_MvDouble++;
			_i_Index++;
			_i_Counter++;
		}
	}

    //Write the int variables of Mv to the EUI table.
	// 
	_i_Index=MV_INT_TOP+(_i_MvIndex*MAXNUM_MV_INTMEM);  //Initialize the index
	_i_Counter=0;  
    while(_i_Counter<=MV_INT_NUMBER)
	{
		if(_i_Counter!=MV_INT_NUMBER)
		{
			if ((_h_MtHandle=writemem(EUI,_i_Index,(double)*_lp_MvInt,_b_Status))!=NOERROR)
				return (_h_MtHandle);
			else
			{
				_lp_MvInt++;
				_i_Index++;
				_i_Counter++;
			}
		}
		else
		{
			_i_Index=MV_BOOL_TOP+(_i_MvIndex*MAXNUM_MV_BOOLMEM); 
			if(*_lp_MvInt==FALSE)
				_b_Status=OFF;
			else
				_b_Status=ON;
			if ((_h_MtHandle=writemem(DII,_i_Index,_db_Temp,_b_Status))!=NOERROR)
				return _h_MtHandle;
			else
			{
				_i_Counter++;
			}
		}
	}

    //Write the BOOL variables of Mv to the DII table.
	// 
	_i_Index=MV_BOOL_TOP+(_i_MvIndex*MAXNUM_MV_BOOLMEM)+1; //Initialize the index
	_i_Counter=0;  
    while(_i_Counter<MV_BOOL_NUMBER-1)
	{
		if(*_lp_MvBool==FALSE)
			_b_Status=OFF;
		else
			_b_Status=ON;
		if ((_h_MtHandle=writemem(DII,_i_Index,_db_Temp,_b_Status))!=NOERROR)
			return _h_MtHandle;
		else
		{
			_lp_MvBool++;
			_i_Index++;
			_i_Counter++;
		}
	}
    
/*
	//Write the char variables of Mv to the STR table.
	// 
	_i_Index=MV_CHAR_TOP+(_i_MvIndex*MAXNUM_MV_CHARMEM);  //Initialize the index
	_i_Counter=0;  
    while(_i_Counter<MV_CHAR_NUMBER)
	{
		if (_h_MtHandle=writestring(STRINGLEN,_lp_MvChar,_i_Index))
			return _h_MtHandle;
		else
		{
			_lp_MvChar+=STRINGLEN;
			_i_Index++;
			_i_Counter++;
		}
	}
*/
	UNINITONSPEC();
	return _h_MtHandle;
}

