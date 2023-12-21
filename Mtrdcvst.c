////////////////////////////////////////////////////////////////
//This program complete the function reading Cv from Onspec.  //  
//S.D.Chen 97-9-11                                            //
////////////////////////////////////////////////////////////////
#include <windows.h>
#include "Mtstruct.h"
#include "tfalgo.h"
#include "Mtexport.h"

DLLEXPORT MTHANDLE ReadCvAllMember( LPVARCV _lp_VarCv, WORD _i_CvIndex )
{
	int		_i_Counter;
	int		_i_Index;
	BYTE    _b_Status=OFF;
	
    MTHANDLE _h_MtHandle;
		
	double  *_lp_CvDouble;
	WORD    *_lp_CvInt;	
//    char	*_lp_CvChar;
	BOOL	*_lp_CvBool;
	double  _db_Temp;    //As a template double variable while reads int or BOOL variable from Onspec.
	
    //Initialize Onspec
	if((_h_MtHandle=InitOnspec())!=NOERROR)
		return FAIL_INITIALIZE_ONSPEC;

	//Initialize the pointers
//	_lp_CvChar=_lp_VarCv->FIRST_CVCHARMEM;
	_lp_CvDouble=&(_lp_VarCv->FIRST_CVDOUBLEMEM);
	_lp_CvInt=&(_lp_VarCv->FIRST_CVINTMEM);
	_lp_CvBool=&(_lp_VarCv->FIRST_CVBOOLMEM);
	
	//Read data from the EUR table to the double variables of Cv.
	//
	_i_Index=CV_DOUBLE_TOP+(_i_CvIndex*MAXNUM_CV_DBMEM); //Initialize the index
	_i_Counter=0;
    while(_i_Counter<CV_DOUBLE_NUMBER)
	{
		if ((_h_MtHandle=readmem(EUR,_i_Index,_lp_CvDouble,&_b_Status))!=NOERROR)
			return _h_MtHandle;
		else
		{
			_lp_CvDouble++;
			_i_Index++;
			_i_Counter++;
		}
	}

    
	//Read data from the EUI table to the int variables of Cv.
	// 
	_i_Index=CV_INT_TOP+(_i_CvIndex*MAXNUM_CV_INTMEM);  //Initialize the index
	_i_Counter=0;  
    while(_i_Counter<=CV_INT_NUMBER)
	{
		if(_i_Counter!=CV_INT_NUMBER)
		{
			if ((_h_MtHandle=readmem(EUI,_i_Index,&_db_Temp,&_b_Status))!=NOERROR)
				return _h_MtHandle;
			else
			{
				*_lp_CvInt=(WORD)(_db_Temp);
				_lp_CvInt++;
				_i_Index++;
				_i_Counter++;
			}
		}
		else
		{
			_i_Index=CV_BOOL_TOP+(_i_CvIndex*MAXNUM_CV_BOOLMEM); //it's the index of CControl_type
			if ((_h_MtHandle=readmem(DII,_i_Index,&_db_Temp,&_b_Status))!=NOERROR)
				return _h_MtHandle;
			else
			{
				if((_b_Status&ON)==ON)
				    *_lp_CvInt=TRUE;
				else
					*_lp_CvInt=FALSE;
				_i_Counter++;
			}
		}
	}


    

    //Read data from the DII table to the BOOL variables of Cv.
	// 
	_i_Index=CV_BOOL_TOP+(_i_CvIndex*MAXNUM_CV_BOOLMEM)+1; //Initialize the index
	_i_Counter=0;  
    while(_i_Counter<CV_BOOL_NUMBER-1 )
	{
		if ((_h_MtHandle=readmem(DII,_i_Index,&_db_Temp,&_b_Status))!=NOERROR)
			return _h_MtHandle;
		else
		{
			if((_b_Status&ON)==ON)
			    *_lp_CvBool=TRUE;
			else
				*_lp_CvBool=FALSE;
			_lp_CvBool++;
			_i_Index++;
			_i_Counter++;
		}
	}
	
/*	//Read data from the STR table to the char variables of Cv.
	// 
	_i_Index=CV_CHAR_TOP+(_i_CvIndex*MAXNUM_CV_CHARMEM);  //Initialize the index
	_i_Counter=0;  
    while(_i_Counter<CV_CHAR_NUMBER)
	{
		if (_h_MtHandle=readstring(STRINGLEN,_lp_CvChar,_i_Index))
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


