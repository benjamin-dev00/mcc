//////////////////////////////////////////////////////////////////////////
//This program complete the function writing a member of Gv to Onspec.  //  
//S.D.Chen 97-9-11                                                      //
//////////////////////////////////////////////////////////////////////////
#include <windows.h>
#include "Mtstruct.h"
#include "tfalgo.h"
#include "Mtexport.h"

DLLEXPORT MTHANDLE  WriteGvMember(LPCOBJPRIV _lpCobj_Priv,WORD _i_ttfID)
{
	
	///////////////////////////
	//
	IDGVMEM	_i_GvMemIndex;
	
	int		_i_Index;
	BYTE    _b_Status=OFF;
	
    MTHANDLE _h_MtHandle=NOERROR;
	MEMTYPE  _Memtype;
	
	double  *_lp_GvDouble;
	WORD    *_lp_GvInt;	
    BOOL	*_lp_GvBool;
	double  _db_Temp=0;    //As a template variable while writing BOOL value to Onspec.
	
	if((_h_MtHandle=TransGvID(&_i_ttfID,&_i_GvMemIndex))!=NOERROR)
		return INVALID_TTFID;
	if((_h_MtHandle=InitOnspec())!=NOERROR)
		return FAIL_INITIALIZE_ONSPEC;

	//Initialize the pointers
	_lp_GvDouble=&(_lpCobj_Priv->FIRST_GVDOUBLEMEM);
	_lp_GvInt=&(_lpCobj_Priv->FIRST_GVINTMEM);
	_lp_GvBool=&(_lpCobj_Priv->FIRST_GVBOOLMEM);
	
	
	switch(_Memtype=IsWhatTypeGvMem(_i_GvMemIndex))
	{
	case ISGVDOUBLEMEM:
		_lp_GvDouble+=(_i_GvMemIndex-IDGV_FIRSTDOUBLEMEM);
		_i_Index=GV_DOUBLE_TOP+_i_GvMemIndex-IDGV_FIRSTDOUBLEMEM;
		_h_MtHandle=writemem(EUR,_i_Index,*_lp_GvDouble,_b_Status);
		break;

	case ISGVINTMEM:
		if (_i_GvMemIndex!=ID_Freecount)
		{
			_lp_GvInt+=(_i_GvMemIndex-IDGV_FIRSTINTMEM);
			_i_Index=GV_INT_TOP+_i_GvMemIndex-IDGV_FIRSTINTMEM;
			_h_MtHandle=writemem(EUI,_i_Index,*_lp_GvInt,_b_Status);
		}
		else
		{
			_i_Index=GV_INT_TOP+_i_GvMemIndex-IDGV_FIRSTINTMEM;
			_h_MtHandle=writemem(EUI,_i_Index,_lpCobj_Priv->Freecount,_b_Status);
		}
		break;

	case ISGVBOOLMEM:
		_i_Index=GV_BOOL_TOP+_i_GvMemIndex-IDGV_FIRSTBOOLMEM;
		if(_i_GvMemIndex!=ID_Flag_AM)
		{
			_lp_GvBool+=(_i_GvMemIndex-IDGV_FIRSTBOOLMEM);
		    if(*_lp_GvBool==FALSE)
				_b_Status=OFF;
			else
				_b_Status=ON;
			_h_MtHandle=writemem(DII,_i_Index,_db_Temp,_b_Status);
		}
		else
		{
			if(_lpCobj_Priv->Flag_AM==FALSE)
				_b_Status=OFF;
			else
				_b_Status=ON;
			_h_MtHandle=writemem(DII,_i_Index,_db_Temp,_b_Status);
		}
		break;
	default:
		_h_MtHandle=INVALID_GVMEMINDEX;
	}
	UNINITONSPEC();
	return _h_MtHandle;
}
