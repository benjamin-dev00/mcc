////////////////////////////////////////////////////////////////
//This program complete the function reading a member of Mv from Onspec.  //  
//S.D.Chen 97-9-11                                            //
////////////////////////////////////////////////////////////////
#include <windows.h>
#include "Mtstruct.h"
#include "tfalgo.h"
#include "Mtexport.h"

DLLEXPORT MTHANDLE ReadMvMember(LPVARMV _lp_VarMv,WORD _i_MvIndex,WORD _i_ttfID)
{
	
	IDMVMEM	_i_MvMemIndex;
	int		_i_Index;
	BYTE    _b_Status=OFF;
	
    MTHANDLE _h_MtHandle;
	MEMTYPE  _Memtype;
	
	double  *_lp_MvDouble;
	WORD    *_lp_MvInt;	
    //char	*_lp_MvChar;
	BOOL	*_lp_MvBool;
	double  _db_Temp;    //As a template double variable while reads int or BOOL variable from Onspec.
	
	if((_h_MtHandle=TransMvID(&_i_ttfID,&_i_MvMemIndex))!=NOERROR)
		return INVALID_TTFID;
	
	if((_h_MtHandle=InitOnspec())!=NOERROR)
		return FAIL_INITIALIZE_ONSPEC;

	//Initialize the pointers
	//_lp_MvChar=_lp_VarMv->FIRST_MVCHARMEM;
	_lp_MvDouble=&(_lp_VarMv->FIRST_MVDOUBLEMEM);
	_lp_MvInt=&(_lp_VarMv->FIRST_MVINTMEM);
	_lp_MvBool=&(_lp_VarMv->FIRST_MVBOOLMEM);
	
	switch(_Memtype=IsWhatTypeMvMem(_i_MvMemIndex))
	{
	case ISMVDOUBLEMEM:
		_lp_MvDouble+=(_i_MvMemIndex-IDMV_FIRSTDOUBLEMEM);
		_i_Index=MV_DOUBLE_TOP+(_i_MvIndex*MAXNUM_MV_DBMEM)+_i_MvMemIndex-IDMV_FIRSTDOUBLEMEM;
		_h_MtHandle=readmem(EUR,_i_Index,_lp_MvDouble,&_b_Status);
		break;

	case ISMVINTMEM:
		_lp_MvInt+=(_i_MvMemIndex-IDMV_FIRSTINTMEM);
		_i_Index=MV_INT_TOP+(_i_MvIndex*MAXNUM_MV_INTMEM)+_i_MvMemIndex-IDMV_FIRSTINTMEM;
		if((_h_MtHandle=readmem(EUI,_i_Index,&_db_Temp,&_b_Status))!=NOERROR)
		    break;
		else
			(*_lp_MvInt)=(WORD)(_db_Temp);
		break;

	case ISMVBOOLMEM:
		_i_Index=MV_BOOL_TOP+(_i_MvIndex*MAXNUM_MV_BOOLMEM)+_i_MvMemIndex-IDMV_FIRSTBOOLMEM+1;
		if ((_h_MtHandle=readmem(DII,_i_Index,&_db_Temp,&_b_Status))!=NOERROR)
			break;
		else if(_i_MvMemIndex!=ID_MControl_type)
		{
			_lp_MvBool+=(_i_MvMemIndex-IDMV_FIRSTBOOLMEM);
			if((_b_Status&ON)==ON)
			    *_lp_MvBool=TRUE;
			else
				*_lp_MvBool=FALSE;
		}
		else
		{
			_lp_MvInt+=MV_INT_NUMBER;
			if((_b_Status&ON)==ON)
			    *_lp_MvInt=TRUE;
			else
				*_lp_MvInt=FALSE;
		
		}
		break;
/*
	case ISMVCHARMEM:
		_lp_MvChar+=(_i_MvMemIndex-IDMV_FIRSTCHARMEM)*STRINGLEN;
		_i_Index=MV_CHAR_TOP+(_i_MvIndex*MAXNUM_MV_CHARMEM)+_i_MvMemIndex-IDMV_FIRSTCHARMEM;
		_h_MtHandle=readstring(STRINGLEN,_lp_MvChar,_i_Index);
		break;
*/
	default:
		_h_MtHandle=ISNOTMVMEM;
	}
	UNINITONSPEC();
	return _h_MtHandle;
}


