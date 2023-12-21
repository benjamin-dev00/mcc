////////////////////////////////////////////////////////////////
//This program complete the function reading Mv from Onspec.  //  
//S.D.Chen 97-9-11                                            //
////////////////////////////////////////////////////////////////
#include <windows.h>
#include "Mtstruct.h"
#include "tfalgo.h"
#include "Mtexport.h"

DLLEXPORT MTHANDLE ReadMvAllMember( LPVARMV _lp_VarMv, WORD _i_MvIndex )
{
	int		_i_Counter;
	int		_i_Index;
	BYTE    _b_Status=OFF;
	
    MTHANDLE _h_MtHandle;
		
	double  *_lp_MvDouble;
	WORD    *_lp_MvInt;	
    //char	*_lp_MvChar;
	BOOL	*_lp_MvBool;
	double  _db_Temp;    //As a template double variable while reads int or BOOL variable from Onspec.
	
    //Initialize Onspec
	if((_h_MtHandle=InitOnspec())!=NOERROR)
		return FAIL_INITIALIZE_ONSPEC;

	//Initialize the pointers
	//_lp_MvChar=_lp_VarMv->FIRST_MVCHARMEM;
	_lp_MvDouble=&(_lp_VarMv->FIRST_MVDOUBLEMEM);
	_lp_MvInt=&(_lp_VarMv->FIRST_MVINTMEM);
	_lp_MvBool=&(_lp_VarMv->FIRST_MVBOOLMEM);
	
	//Read data from the EUR table to the double variables of Mv
	//
	_i_Index=MV_DOUBLE_TOP+(_i_MvIndex*MAXNUM_MV_DBMEM); //Initialize the index
	_i_Counter=0;
    while(_i_Counter<MV_DOUBLE_NUMBER)
	{
		if ((_h_MtHandle=readmem(EUR,_i_Index,_lp_MvDouble,&_b_Status))!=NOERROR)
			return _h_MtHandle;
		else
		{
			_lp_MvDouble++;
			_i_Index++;
			_i_Counter++;
		}
	}

    
	//Read data from the EUI table to the int variables of Mv.
	// 
	_i_Index=MV_INT_TOP+(_i_MvIndex*MAXNUM_MV_INTMEM);  //Initialize the index
	_i_Counter=0;  
    while(_i_Counter<=MV_INT_NUMBER)
	{
		if(_i_Counter!=MV_INT_NUMBER)
		{
			if ((_h_MtHandle=readmem(EUI,_i_Index,&_db_Temp,&_b_Status))!=NOERROR)
				return _h_MtHandle;
			else
			{
				*_lp_MvInt=(WORD)(_db_Temp);
				_lp_MvInt++;
				_i_Index++;
				_i_Counter++;
			}
		}
		else
		{
			_i_Index=MV_BOOL_TOP+(_i_MvIndex*MAXNUM_MV_BOOLMEM);  
			if ((_h_MtHandle=readmem(DII,_i_Index,&_db_Temp,&_b_Status))!=NOERROR)
				return _h_MtHandle;
			else
			{
				if((_b_Status&ON)==ON)
				    *_lp_MvInt=TRUE;
				else
					*_lp_MvInt=FALSE;
					_i_Counter++;
			}
		}
	}

    //Read data from the DII table to the BOOL variables of Mv.
	// 
	_i_Index=MV_BOOL_TOP+(_i_MvIndex*MAXNUM_MV_BOOLMEM)+1;  //Initialize the index
	_i_Counter=0;  
    while(_i_Counter<MV_BOOL_NUMBER-1)
	{
		if ((_h_MtHandle=readmem(DII,_i_Index,&_db_Temp,&_b_Status))!=NOERROR)
			return _h_MtHandle;
		else
		{
			if((_b_Status&ON)==ON)
			    *_lp_MvBool=TRUE;
			else
				*_lp_MvBool=FALSE;
			_lp_MvBool++;
			_i_Index++;
			_i_Counter++;
		}
	}
	
/*
	//Read data from the STR table to the char variables of Mv.
	// 
	_i_Index=MV_CHAR_TOP+(_i_MvIndex*MAXNUM_MV_CHARMEM);  //Initialize the index
	_i_Counter=0;  
    while(_i_Counter<MV_CHAR_NUMBER)
	{
		if (_h_MtHandle=readstring(STRINGLEN,_lp_MvChar,_i_Index))
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


