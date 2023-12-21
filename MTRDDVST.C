////////////////////////////////////////////////////////////////
//This program complete the function reading Dv from Onspec.  //  
//S.D.Chen 97-9-11                                            //
////////////////////////////////////////////////////////////////
#include <windows.h>
#include "Mtstruct.h"
#include "tfalgo.h"
#include "Mtexport.h"

DLLEXPORT MTHANDLE ReadDvAllMember( LPVARDV _lp_VarDv, WORD _i_DvIndex )
{
	int		_i_Counter;
	int		_i_Index;
	BYTE    _b_Status=OFF;
	
    MTHANDLE _h_MtHandle;
		
	double  *_lp_DvDouble;
//	WORD    *_lp_DvInt;	
    //char	*_lp_DvChar;
	BOOL	*_lp_DvBool;
	double  _db_Temp;    //As a template double variable while reads int or BOOL variable from Onspec.
	
 //Initialize Onspec
	if((_h_MtHandle=InitOnspec())!=NOERROR)
		return FAIL_INITIALIZE_ONSPEC;

	//Initialize the pointers
//	_lp_DvChar=_lp_VarDv->FIRST_DVCHARMEM;
	_lp_DvDouble=&(_lp_VarDv->FIRST_DVDOUBLEMEM);
//	_lp_DvInt=NULL;
	_lp_DvBool=&(_lp_VarDv->FIRST_DVBOOLMEM);
	
	//Read data from the EUR table to the double variables of Dv.
	//
	_i_Index=DV_DOUBLE_TOP+(_i_DvIndex*MAXNUM_DV_DBMEM); //Initialize the index
	_i_Counter=0;
    while(_i_Counter<DV_DOUBLE_NUMBER)
	{
		if ((_h_MtHandle=readmem(EUR,_i_Index,_lp_DvDouble,&_b_Status))!=NOERROR)
			return _h_MtHandle;
		else
		{
			_lp_DvDouble++;
			_i_Index++;
			_i_Counter++;
		}
	}

    /*The following codes are used while _lp_DvInt is not NULL.
	//Read data from the EUI table to the int variables of Dv.
	// 
	_i_Index=DV_INT_TOP+(_i_DvIndex*MAXNUM_DV_INTMEM);  //Initialize the index
	_i_Counter=0;  
    while(_i_Counter<DV_INT_NUMBER)
	{
		if (_h_MtHandle=readmem(EUI,_i_Index,&_db_Temp,&_b_Status))
			return _h_MtHandle;
		else
		{
			*_lp_DvInt=(WORD)(_db_Temp);
			_lp_DvInt++;
			_i_Index++;
			_i_Counter++;
		}
	}


    //Read data from the STR table to the char variables of Dv.
	// 
	_i_Index=DV_CHAR_TOP+(_i_DvIndex*MAXNUM_DV_CHARMEM);  //Initialize the index
	_i_Counter=0;  
    while(_i_Counter<DV_CHAR_NUMBER)
	{
		if (_h_MtHandle=readstring(STRINGLEN,_lp_DvChar,_i_Index))
			return _h_MtHandle;
		else
		{
			_lp_DvChar+=STRINGLEN;
			_i_Index++;
			_i_Counter++;
		}
		
	}
*/
    //Read data from the DII table to the BOOL variables of Dv.
	// 
	_i_Index=DV_BOOL_TOP+(_i_DvIndex*MAXNUM_DV_BOOLMEM);  //Initialize the index
	_i_Counter=0;  
    while(_i_Counter<DV_BOOL_NUMBER)
	{
		if ((_h_MtHandle=readmem(DII,_i_Index,&_db_Temp,&_b_Status))!=NOERROR)
			return _h_MtHandle;
		else
		{
			if((_b_Status&ON)==ON)
			    *_lp_DvBool=TRUE;
			else
				*_lp_DvBool=FALSE;
			_lp_DvBool++;
			_i_Index++;
			_i_Counter++;
		}
	}		
	UNINITONSPEC();
	return _h_MtHandle;
}


