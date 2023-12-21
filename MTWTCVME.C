//////////////////////////////////////////////////////////////////////////
//This program complete the function writing a member of Cv to Onspec.  //  
//S.D.Chen 97-9-11                                                      //
//////////////////////////////////////////////////////////////////////////
#include <windows.h>
#include "Mtstruct.h"
#include "tfalgo.h"
#include "Mtexport.h"

DLLEXPORT MTHANDLE WriteCvMember(LPVARCV _lp_VarCv,WORD _i_CvIndex,WORD _i_ttfID)
{
	
	///////////////////////////
	//
	IDCVMEM	_i_CvMemIndex;
	
	int		_i_Index;
	BYTE    _b_Status=OFF;
	
    MTHANDLE _h_MtHandle;
	MEMTYPE  _Memtype;
	
	double  *_lp_CvDouble;
	WORD    *_lp_CvInt;	
    //char	*_lp_CvChar;
	BOOL	*_lp_CvBool;
	double  _db_Temp=0;    //As a template variable while writing BOOL value to Onspec.
	
	if ((_h_MtHandle=TransCvID(&_i_ttfID,&_i_CvMemIndex))!=NOERROR)
		return INVALID_TTFID;
	
	if((_h_MtHandle=InitOnspec())!=NOERROR)
		return FAIL_INITIALIZE_ONSPEC;

	//Initialize the pointers
	_lp_CvDouble=&(_lp_VarCv->FIRST_CVDOUBLEMEM);
	_lp_CvInt=&(_lp_VarCv->FIRST_CVINTMEM);
	_lp_CvBool=&(_lp_VarCv->FIRST_CVBOOLMEM);
	
	switch(_Memtype=IsWhatTypeCvMem(_i_CvMemIndex))
	{
	case ISCVDOUBLEMEM:
		_lp_CvDouble+=(_i_CvMemIndex-IDCV_FIRSTDOUBLEMEM);
		_i_Index=CV_DOUBLE_TOP+(_i_CvIndex*MAXNUM_CV_DBMEM)+_i_CvMemIndex-IDCV_FIRSTDOUBLEMEM;
		_h_MtHandle=writemem(EUR,_i_Index,*_lp_CvDouble,_b_Status);
		break;

	case ISCVINTMEM:
		_lp_CvInt+=(_i_CvMemIndex-IDCV_FIRSTINTMEM);
		_i_Index=CV_INT_TOP+(_i_CvIndex*MAXNUM_CV_INTMEM)+_i_CvMemIndex-IDCV_FIRSTINTMEM;
		_h_MtHandle=writemem(EUI,_i_Index,*_lp_CvInt,_b_Status);
		break;

	case ISCVBOOLMEM:
		_i_Index=CV_BOOL_TOP+(_i_CvIndex*MAXNUM_CV_BOOLMEM)+_i_CvMemIndex-IDCV_FIRSTBOOLMEM+1;
		if(_i_CvMemIndex!=ID_CControl_type)
		{
			_lp_CvBool+=(_i_CvMemIndex-IDCV_FIRSTBOOLMEM);
			if(*_lp_CvBool==FALSE)
				_b_Status=OFF;
			else
				_b_Status=ON;
			_h_MtHandle=writemem(DII,_i_Index,_db_Temp,_b_Status);
		}
		else
		{
			_lp_CvInt+=CV_INT_NUMBER;
			if(*_lp_CvInt==FALSE)
				_b_Status=OFF;
			else
				_b_Status=ON;
			_h_MtHandle=writemem(DII,_i_Index,_db_Temp,_b_Status);
		
		}
		break;
/*
	case ISCVCHARMEM:
		_lp_CvChar+=(_i_CvMemIndex-IDCV_FIRSTCHARMEM)*STRINGLEN;
		_i_Index=CV_CHAR_TOP+(_i_CvIndex*MAXNUM_CV_CHARMEM)+_i_CvMemIndex-IDCV_FIRSTCHARMEM;
		_h_MtHandle=writestring(STRINGLEN,_lp_CvChar,_i_Index);
		break;
*/
	default:
		_h_MtHandle=INVALID_CVMEMINDEX;
	}
	UNINITONSPEC();
	return _h_MtHandle;
}
