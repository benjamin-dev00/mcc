#include <windows.h>
#include "Mtstruct.h"
#include <stdlib.h>

MTHANDLE InitVarCvObj(LPVARCVOBJ);
MTHANDLE InitVarMvObj(LPVARMVOBJ);
MTHANDLE InitVarDvObj(LPVARDVOBJ);


MTHANDLE InitVarCvObj(LPVARCVOBJ _lp_VarCvObj)
{
	int _i;
	LPVARCV _lp_VarCv;

	_lp_VarCvObj->CurrentCvIndex=0;
	_lp_VarCvObj->CurrentIDCvMem=ID_TAG;

	for(_i=0;_i<CV_NUMBER;_i++)
	{
		_lp_VarCv=(LPVARCV)malloc(sizeof(VARCV));
		if(_lp_VarCv!=NULL)
			_lp_VarCvObj->CvPointer[_i]=_lp_VarCv;
		else
			return FAIL_MEMORY_ALLOC;
	}
	for(_i=0;_i<CV_NUMBER;_i++)
	{
		_lp_VarCvObj->CvStatus[_i]=GOODDATA;
	}
	return NOERROR;
}


MTHANDLE InitVarMvObj(LPVARMVOBJ _lp_VarMvObj)
{
	int _i;
	LPVARMV _lp_VarMv;

	_lp_VarMvObj->CurrentMvIndex=0;
	_lp_VarMvObj->CurrentIDMvMem=ID_TAG;

	for(_i=0;_i<MV_NUMBER;_i++)
	{
		_lp_VarMv=(LPVARMV)malloc(sizeof(VARMV));
		if(_lp_VarMv!=NULL)
			_lp_VarMvObj->MvPointer[_i]=_lp_VarMv;
		else
			return FAIL_MEMORY_ALLOC;
	}
	for(_i=0;_i<MV_NUMBER;_i++)
	{
		_lp_VarMvObj->MvStatus[_i]=GOODDATA;
	}
	return NOERROR;
}

MTHANDLE InitVarDvObj(LPVARDVOBJ _lp_VarDvObj)
{
	int _i;
	LPVARDV _lp_VarDv;

	_lp_VarDvObj->CurrentDvIndex=0;
	_lp_VarDvObj->CurrentIDDvMem=ID_TAG;

	for(_i=0;_i<DV_NUMBER;_i++)
	{
		_lp_VarDv=(LPVARDV)malloc(sizeof(VARDV));
		if(_lp_VarDv!=NULL)
			_lp_VarDvObj->DvPointer[_i]=_lp_VarDv;
		else
			return FAIL_MEMORY_ALLOC;
	}
	for(_i=0;_i<DV_NUMBER;_i++)
	{
		_lp_VarDvObj->DvStatus[_i]=GOODDATA;
	}
	return NOERROR;
}