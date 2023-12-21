//////////////////////////////////////////////////////////////////
//This program complete the function writing  Dv to Onspec.		//  
//S.D.Chen 97-9-11                                              //
//////////////////////////////////////////////////////////////////
#include <windows.h>
#include "Mtstruct.h"
#include "tfalgo.h"
#include "Mtexport.h"


DLLEXPORT MTHANDLE WriteDvAllMember( LPVARDV _lp_VarDv, WORD _i_DvIndex )
{
	int		_i_Index;
	int		_i_Counter;
	BYTE    _b_Status=OFF;

	MTHANDLE _h_MtHandle;
	
	double  *_lp_DvDouble;
	//WORD    *_lp_DvInt;	
    //char	*_lp_DvChar;
	BOOL	*_lp_DvBool;
	double  _db_Temp = 0.0;    //As a template variable while writing BOOL value to Onspec.	
	if((_h_MtHandle=InitOnspec())!=NOERROR)
		return FAIL_INITIALIZE_ONSPEC;

	//Initialize the pointers
	//_lp_DvChar=_lp_VarDv->FIRST_DVCHARMEM;
	_lp_DvDouble=&(_lp_VarDv->FIRST_DVDOUBLEMEM);
	//_lp_DvInt=NULL;
	_lp_DvBool=&(_lp_VarDv->FIRST_DVBOOLMEM);
	
	//Write the double variables of Dv to the EUR table.
	//
	_i_Index=DV_DOUBLE_TOP+(_i_DvIndex*MAXNUM_DV_DBMEM); //Initialize the index
	_i_Counter=0;
    while(_i_Counter<DV_DOUBLE_NUMBER)
	{
		if ((_h_MtHandle=writemem(EUR,_i_Index,*_lp_DvDouble,_b_Status))!=NOERROR)
			return _h_MtHandle;
		else
		{
			_lp_DvDouble++;
			_i_Index++;
			_i_Counter++;
		}
	}

    /*The following codes are used when _lp_DvInt is not NULL.
	//Write the int variables of Dv to the EUI table.
	// 
	_i_Index=DV_INT_TOP+(_i_DvIndex*MAXNUM_DV_INTMEM);  //Initialize the index
	_i_Counter=0;  
    while(_i_Counter<DV_INT_NUMBER)
	{
		if (_h_MtHandle=writemem(EUI,_i_Index,(double)*_lp_DvInt,_b_Status))
			return (_h_MtHandle);
		else
		{
			_lp_DvInt++;
			_i_Index++;
			_i_Counter++;
		}
	}
*/
    //Write the BOOL variables of Dv to the DII table.
	// 
	_i_Index=DV_BOOL_TOP+(_i_DvIndex*MAXNUM_DV_BOOLMEM);  //Initialize the index
	_i_Counter=0;  
    while(_i_Counter<DV_BOOL_NUMBER)
	{
		if(*_lp_DvBool==FALSE)
			_b_Status=OFF;
		else
			_b_Status=ON;
		if ((_h_MtHandle=writemem(DII,_i_Index,_db_Temp,_b_Status))!=NOERROR)
			return _h_MtHandle;
		else
		{
			_lp_DvBool++;
			_i_Index++;
			_i_Counter++;
		}
	}
/*    
	//Write the char variables of Dv to the STR table.
	// 
	_i_Index=DV_CHAR_TOP+(_i_DvIndex*MAXNUM_DV_CHARMEM);  //Initialize the index
	_i_Counter=0;  
    while(_i_Counter<DV_CHAR_NUMBER)
	{
		if (_h_MtHandle=writestring(STRINGLEN,_lp_DvChar,_i_Index))
			return _h_MtHandle;
		else
		{
			_lp_DvChar+=STRINGLEN;
			_i_Index++;
			_i_Counter++;
		}
	}
*/
	UNINITONSPEC();
	return _h_MtHandle;
}

