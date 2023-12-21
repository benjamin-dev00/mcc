//////////////////////////////////////////////////////////////////////////
//This program complete the function writing a member of DV to Onspec.  //  
//S.D.Chen 97-9-11                                                      //
//////////////////////////////////////////////////////////////////////////
#include <windows.h>
#include "Mtstruct.h"
#include "tfalgo.h"
#include "Mtexport.h"

DLLEXPORT MTHANDLE WriteDvMember(LPVARDV _lp_VarDv,WORD _i_DvIndex,WORD _i_ttfID)
{
	
	///////////////////////////
	//
	IDDVMEM	_i_DvMemIndex;
	
	int		_i_Index;
	BYTE    _b_Status=OFF;
	
    MTHANDLE _h_MtHandle;
	MEMTYPE  _Memtype;
	
	double  *_lp_DvDouble;
//	WORD    *_lp_DvInt;	
// char	*_lp_DvChar;
	BOOL	*_lp_DvBool;
	double  _db_Temp=0;    //As a template variable while writing BOOL value to Onspec.
	
	if ((_h_MtHandle=TransDvID(&_i_ttfID,&_i_DvMemIndex))!=NOERROR)
		return INVALID_TTFID;
	
	if((_h_MtHandle=InitOnspec())!=NOERROR)
		return FAIL_INITIALIZE_ONSPEC;
	
	//Initialize the pointers
//	_lp_DvChar=_lp_VarDv->FIRST_DVCHARMEM;
	_lp_DvDouble=&(_lp_VarDv->FIRST_DVDOUBLEMEM);
//	_lp_DvInt=NULL;
	_lp_DvBool=&(_lp_VarDv->FIRST_DVBOOLMEM);
	
	switch(_Memtype=IsWhatTypeDvMem(_i_DvMemIndex))
	{
	case ISDVDOUBLEMEM:
		_lp_DvDouble+=(_i_DvMemIndex-IDDV_FIRSTDOUBLEMEM);
		_i_Index=DV_DOUBLE_TOP+(_i_DvIndex*MAXNUM_DV_DBMEM)+_i_DvMemIndex-IDDV_FIRSTDOUBLEMEM;
		_h_MtHandle=writemem(EUR,_i_Index,*_lp_DvDouble,_b_Status);
		break;

/* the following Codes used when DV has int member 
	case ISDVINTMEM:
		_lp_DvInt+=(_i_DvMemIndex-IDDV_FIRSTINTMEM);
		_i_Index=DV_INT_TOP+(_i_DvIndex*MAXNUM_DV_INTMEM)+_i_DvMemIndex-IDDV_FIRSTINTMEM;
		_h_MtHandle=readmem(EUI,_i_Index,*_lp_DvInt,_b_Status);
		break;
*/
	case ISDVBOOLMEM:
		_lp_DvBool+=(_i_DvMemIndex-IDDV_FIRSTBOOLMEM);
		_i_Index=DV_BOOL_TOP+(_i_DvIndex*MAXNUM_DV_BOOLMEM)+_i_DvMemIndex-IDDV_FIRSTBOOLMEM;
		if(*_lp_DvBool==FALSE)
			_b_Status=OFF;
		else
			_b_Status=ON;
		_h_MtHandle=writemem(DII,_i_Index,_db_Temp,_b_Status);
		break;
/*		
	case ISDVCHARMEM:
		_lp_DvChar+=(_i_DvMemIndex-IDDV_FIRSTCHARMEM)*STRINGLEN;
		_i_Index=DV_CHAR_TOP+(_i_DvIndex*MAXNUM_DV_CHARMEM)+_i_DvMemIndex-IDDV_FIRSTCHARMEM;
		_h_MtHandle=writestring(STRINGLEN,_lp_DvChar,_i_Index);
		break;
*/		
	default:
		_h_MtHandle=INVALID_DVMEMINDEX;
	}
	UNINITONSPEC();
	return _h_MtHandle;
}



