////////////////////////////////////////////////////////////////
//This program complete the function reading Gv from Onspec.  //  
//S.D.Chen 97-9-11                                            //
////////////////////////////////////////////////////////////////
#include <windows.h>
#include "Mtstruct.h"
#include "tfalgo.h"
#include "Mtexport.h"
DLLEXPORT MTHANDLE ReadGvMember(LPCOBJPRIV _lpCobj_Priv,WORD _i_ttfID)
{ 
	
	///////////////////////////
	//
	
	int		_i_Index;
	BYTE    _b_Status=0;
	IDGVMEM	_i_GvMemIndex;

    MTHANDLE _h_MtHandle=NOERROR;
	MEMTYPE  _Memtype;
	
	double  *_lp_GvDouble;
	WORD    *_lp_GvInt;	
    BOOL	*_lp_GvBool;
	double  _db_Temp;    //As a template double variable while reads int or BOOL variable from Onspec.
	
	if ((_h_MtHandle=TransGvID(&_i_ttfID,&_i_GvMemIndex))!=NOERROR)
		return INVALID_TTFID;
	//initialize Onspec
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
		_h_MtHandle=readmem(EUR,_i_Index,_lp_GvDouble,&_b_Status);
		break;

	case ISGVINTMEM:
		_lp_GvInt+=(_i_GvMemIndex-IDGV_FIRSTINTMEM);
		_i_Index=GV_INT_TOP+_i_GvMemIndex-IDGV_FIRSTINTMEM;
		if((_h_MtHandle=readmem(EUI,_i_Index,&_db_Temp,&_b_Status))==NOERROR);
		{
			if (_i_GvMemIndex!=ID_Freecount)
				*_lp_GvInt=(WORD)_db_Temp;
			else 
				_lpCobj_Priv->Freecount=(int)_db_Temp;
		}
		break;
	case ISGVBOOLMEM:
		_i_Index=GV_BOOL_TOP+_i_GvMemIndex-IDGV_FIRSTBOOLMEM;
		if ((_h_MtHandle=readmem(DII,_i_Index,&_db_Temp,&_b_Status))!=NOERROR)
			break;
		else if(_i_GvMemIndex!=ID_Flag_AM)
		{
			_lp_GvBool+=(_i_GvMemIndex-IDGV_FIRSTBOOLMEM);
			if((_b_Status&ON)==ON)
			    *_lp_GvBool=TRUE;
			else
				*_lp_GvBool=FALSE;
		}
		else
		{
			if((_b_Status&ON)==ON)
			    _lpCobj_Priv->Flag_AM=TRUE;
			else
				_lpCobj_Priv->Flag_AM=FALSE;
		
		}

		break;
/*
	case ISGVCHARMEM:
		_lp_GvChar+=(_i_GvMemIndex-IDGV_FIRSTCHARMEM)*STRINGLEN;
		_i_Index=GV_CHAR_TOP+(_i_GvIndex*MAXNUM_GV_CHARMEM)+_i_GvMemIndex-IDGV_FIRSTCHARMEM;
		_h_MtHandle=readstring(STRINGLEN,_lp_GvChar,_i_Index);
		break;
*/
	default: _h_MtHandle=INVALID_GVMEMINDEX;
	}
	UNINITONSPEC();
	return _h_MtHandle;
}


