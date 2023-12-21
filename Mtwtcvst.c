//////////////////////////////////////////////////////////////////
//This program complete the function writing  Cv to Onspec.		//  
//S.D.Chen 97-9-11                                              //
//////////////////////////////////////////////////////////////////
#include <windows.h>
#include "Mtstruct.h"
#include "tfalgo.h"
#include "Mtexport.h"

DLLEXPORT MTHANDLE WriteCvAllMember( LPVARCV _lp_VarCv, WORD _i_CvIndex )
{
	int		_i_Index;
	int		_i_Counter;
	BYTE    _b_Status=OFF;

	MTHANDLE _h_MtHandle;
	
	double  *_lp_CvDouble;
	WORD    *_lp_CvInt;	
    //char	*_lp_CvChar;
	BOOL	*_lp_CvBool;
	double  _db_Temp = 0;    //As a template variable while writing BOOL value to Onspec.
	
	if((_h_MtHandle=InitOnspec())!=NOERROR)
		return FAIL_INITIALIZE_ONSPEC;

	//Initialize the pointers
	//_lp_CvChar=_lp_VarCv->FIRST_CVCHARMEM;
	_lp_CvDouble=&(_lp_VarCv->FIRST_CVDOUBLEMEM);
	_lp_CvInt=&(_lp_VarCv->FIRST_CVINTMEM);
	_lp_CvBool=&(_lp_VarCv->FIRST_CVBOOLMEM);
	
	//Write the double variables of Cv to the EUR table.
	//
	_i_Index=CV_DOUBLE_TOP+(_i_CvIndex*MAXNUM_CV_DBMEM); //Initialize the index
	_i_Counter=0;
    while(_i_Counter<CV_DOUBLE_NUMBER)
	{
		if ((_h_MtHandle=writemem(EUR,_i_Index,*_lp_CvDouble,_b_Status))!=NOERROR)
			return _h_MtHandle;
		else
		{
			_lp_CvDouble++;
			_i_Index++;
			_i_Counter++;
		}
	}

    //Write the int variables of Cv to the EUI table.
	// 
	_i_Index=CV_INT_TOP+(_i_CvIndex*MAXNUM_CV_INTMEM);  //Initialize the index
	_i_Counter=0;  
    while(_i_Counter<=CV_INT_NUMBER)
	{
		if(_i_Counter!=CV_INT_NUMBER)
		{
			if ((_h_MtHandle=writemem(EUI,_i_Index,(double)*_lp_CvInt,_b_Status))!=NOERROR)
				return (_h_MtHandle);
			else
			{
				_lp_CvInt++;
				_i_Index++;
				_i_Counter++;
			}
		}
		else
		{
			_i_Index=CV_BOOL_TOP+(_i_CvIndex*MAXNUM_CV_BOOLMEM);  //Initialize the index
			if(*_lp_CvInt==FALSE)
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

    //Write the BOOL variables of Cv to the DII table.
	// 
	_i_Index=CV_BOOL_TOP+(_i_CvIndex*MAXNUM_CV_BOOLMEM)+1;  //Initialize the index
	_i_Counter=0;  
    while(_i_Counter<CV_BOOL_NUMBER-1)
	{
		if(*_lp_CvBool==FALSE)
			_b_Status=OFF;
		else
			_b_Status=ON;
		if ((_h_MtHandle=writemem(DII,_i_Index,_db_Temp,_b_Status))!=NOERROR)
			return _h_MtHandle;
		else
		{
			_lp_CvBool++;
			_i_Index++;
			_i_Counter++;
		}
	}
/*    
	//Write the char variables of Cv to the STR table.
	// 
	_i_Index=CV_CHAR_TOP+(_i_CvIndex*MAXNUM_CV_CHARMEM);  //Initialize the index
	_i_Counter=0;  
    while(_i_Counter<CV_CHAR_NUMBER)
	{
		if (_h_MtHandle=writestring(STRINGLEN,_lp_CvChar,_i_Index))
			return _h_MtHandle;
		else
		{
			_lp_CvChar+=STRINGLEN;
			_i_Index++;
			_i_Counter++;
		}
	}

*/
	UNINITONSPEC();
	return _h_MtHandle;
}

