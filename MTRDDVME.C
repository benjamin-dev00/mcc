
////////////////////////////////////////////////////////////////
//This program complete the function reading Cv from Onspec.  //  
//S.D.Chen 97-9-11                                            //
////////////////////////////////////////////////////////////////
#include <windows.h>
#include <stdlib.h>
#include "Mtstruct.h"
#include "tfalgo.h"
#include "Mtexport.h"

DLLEXPORT MTHANDLE ReadDvMember(LPVARDV _lp_VarDv,WORD _i_DvIndex,WORD _i_ttfID)
{
	
	///////////////////////////
	//
	IDDVMEM	_i_DvMemIndex;
	int		_i_Index;
	BYTE    _b_Status=OFF;
	
    MTHANDLE _h_MtHandle=NOERROR;
	MEMTYPE  _Memtype;
	
	double  *_lp_DvDouble;
	//WORD    *_lp_DvInt;	
    //char	*_lp_DvChar;
	BOOL	*_lp_DvBool;
	double  _db_Temp;    //As a template double variable while reads int or BOOL variable from Onspec.
	
	if((_h_MtHandle=TransDvID(&_i_ttfID,&_i_DvMemIndex))!=NOERROR)
		return INVALID_TTFID;
	if((_h_MtHandle=InitOnspec())!=NOERROR)
		return FAIL_INITIALIZE_ONSPEC;

	//Initialize the pointers
	//_lp_DvChar=_lp_VarDv->FIRST_DVCHARMEM;
	_lp_DvDouble=&(_lp_VarDv->FIRST_DVDOUBLEMEM);
	//_lp_DvInt=&(_lp_VarDv->FIRST_DVINTMEM);
	_lp_DvBool=&(_lp_VarDv->FIRST_DVBOOLMEM);
	
	
	switch(_Memtype=IsWhatTypeDvMem(_i_DvMemIndex))
	{
	case ISDVDOUBLEMEM:
		_lp_DvDouble+=(_i_DvMemIndex-IDDV_FIRSTDOUBLEMEM);
		_i_Index=DV_DOUBLE_TOP+(_i_DvIndex*MAXNUM_DV_DBMEM)+_i_DvMemIndex-IDDV_FIRSTDOUBLEMEM;
		_h_MtHandle=readmem(EUR,_i_Index,_lp_DvDouble,&_b_Status);
   		break;
/*		
	case ISDVINTMEM:
		_lp_DvInt+=(_i_DvMemIndex-IDDV_FIRSTINTMEM);
		_i_Index=DV_INT_TOP+(_i_DvIndex*MAXNUM_DV_INTMEM)+_i_DvMemIndex-IDDV_FIRSTINTMEM;
		if((_h_MtHandle=readmem(EUI,_i_Index,&_db_Temp,&_b_Status))!=NOERROR)
		    break;
		else
			(*_lp_DvInt)=(WORD)(_db_Temp);
		break;
*/
	case ISDVBOOLMEM:
		_i_Index=DV_BOOL_TOP+(_i_DvIndex*MAXNUM_DV_BOOLMEM)+_i_DvMemIndex-IDDV_FIRSTBOOLMEM;
		if ((_h_MtHandle=readmem(DII,_i_Index,&_db_Temp,&_b_Status))!=NOERROR)
			break;
		else
		{
			_lp_DvBool+=(_i_DvMemIndex-IDDV_FIRSTBOOLMEM);
			if((_b_Status&ON)==ON)
			    *_lp_DvBool=TRUE;
			else
				*_lp_DvBool=FALSE;
		}
		
		break;
/*
	case ISDVCHARMEM:
		_lp_DvChar+=(_i_DvMemIndex-IDDV_FIRSTCHARMEM)*STRINGLEN;
		_i_Index=DV_CHAR_TOP+(_i_DvIndex*MAXNUM_DV_CHARMEM)+_i_DvMemIndex-IDDV_FIRSTCHARMEM;
		_h_MtHandle=readstring(STRINGLEN,_lp_DvChar,_i_Index);
		break;
*/	
	default: 
		_h_MtHandle=INVALID_DVMEMINDEX;
	}
	UNINITONSPEC();
	return _h_MtHandle;
}


