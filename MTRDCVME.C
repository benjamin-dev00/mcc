
////////////////////////////////////////////////////////////////
//This program complete the function reading Cv from Onspec.  //  
//S.D.Chen 97-9-11                                            //
////////////////////////////////////////////////////////////////
#include <windows.h>
#include <stdlib.h>
#include "Mtstruct.h"
#include "tfalgo.h"
#include "Mtexport.h"

DLLEXPORT MTHANDLE ReadCvMember(LPVARCV _lp_VarCv,WORD _i_CvIndex,WORD _i_ttfID)
{
	
	///////////////////////////
	//
	IDCVMEM	_i_CvMemIndex;
	int		_i_Index;
	BYTE    _b_Status=OFF;
	
    MTHANDLE _h_MtHandle=NOERROR;
	MEMTYPE  _Memtype;
	
	double  *_lp_CvDouble;
	WORD    *_lp_CvInt;	
    //char	*_lp_CvChar;
	BOOL	*_lp_CvBool;
	double  _db_Temp;    //As a template double variable while reads int or BOOL variable from Onspec.
	
	if((_h_MtHandle=TransCvID(&_i_ttfID,&_i_CvMemIndex))!=NOERROR)
		return INVALID_TTFID;
	if((_h_MtHandle=InitOnspec())!=NOERROR)
		return FAIL_INITIALIZE_ONSPEC;

	//Initialize the pointers
	//_lp_CvChar=_lp_VarCv->FIRST_CVCHARMEM;
	_lp_CvDouble=&(_lp_VarCv->FIRST_CVDOUBLEMEM);
	_lp_CvInt=&(_lp_VarCv->FIRST_CVINTMEM);
	_lp_CvBool=&(_lp_VarCv->FIRST_CVBOOLMEM);
	
	
	switch(_Memtype=IsWhatTypeCvMem(_i_CvMemIndex))
	{
	case ISCVDOUBLEMEM:
		_lp_CvDouble+=(_i_CvMemIndex-IDCV_FIRSTDOUBLEMEM);
		_i_Index=CV_DOUBLE_TOP+(_i_CvIndex*MAXNUM_CV_DBMEM)+_i_CvMemIndex-IDCV_FIRSTDOUBLEMEM;
		_h_MtHandle=readmem(EUR,_i_Index,_lp_CvDouble,&_b_Status);
   		break;
		
	case ISCVINTMEM:
		_lp_CvInt+=(_i_CvMemIndex-IDCV_FIRSTINTMEM);
		_i_Index=CV_INT_TOP+(_i_CvIndex*MAXNUM_CV_INTMEM)+_i_CvMemIndex-IDCV_FIRSTINTMEM;
		if((_h_MtHandle=readmem(EUI,_i_Index,&_db_Temp,&_b_Status))!=NOERROR)
		    break;
		else
			(*_lp_CvInt)=(WORD)(_db_Temp);
		break;

	case ISCVBOOLMEM:
		_i_Index=CV_BOOL_TOP+(_i_CvIndex*MAXNUM_CV_BOOLMEM)+_i_CvMemIndex-IDCV_FIRSTBOOLMEM+1;
		if ((_h_MtHandle=readmem(DII,_i_Index,&_db_Temp,&_b_Status))!=NOERROR)
			break;
		else if(_i_CvMemIndex!=ID_CControl_type)
		{
			_lp_CvBool+=(_i_CvMemIndex-IDCV_FIRSTBOOLMEM);
			if((_b_Status&ON)==ON)
			    *_lp_CvBool=TRUE;
			else
				*_lp_CvBool=FALSE;
		}
		else
		{
			_lp_CvInt+=CV_INT_NUMBER;
			if((_b_Status&ON)==ON)
			    *_lp_CvInt=TRUE;
			else
				*_lp_CvInt=FALSE;
		
		}
		break;
/*
	case ISCVCHARMEM:
		_lp_CvChar+=(_i_CvMemIndex-IDCV_FIRSTCHARMEM)*STRINGLEN;
		_i_Index=CV_CHAR_TOP+(_i_CvIndex*MAXNUM_CV_CHARMEM)+_i_CvMemIndex-IDCV_FIRSTCHARMEM;
		_h_MtHandle=readstring(STRINGLEN,_lp_CvChar,_i_Index);
		break;
*/	
	default: _h_MtHandle=INVALID_CVMEMINDEX;
	}
	UNINITONSPEC();
	return _h_MtHandle;
}


