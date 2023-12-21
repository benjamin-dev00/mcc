//////////////////////////////////////////////////////////////////////////
//Mtiniall.c  
//Allocate memory for VARCVOBJ,VARMVOBJ,VARDVOBJ,and VARGVOBJ,
// 
#include <windows.h>
//addtemp
#include "tfalgo.h"
#include "Mtstruct.h"
#include <stdlib.h>

MTHANDLE   InitOnspec(void);
/*
LPVARCVOBJ MallocAndInitVarCvObj(void);
LPVARMVOBJ MallocAndInitVarMvObj(void);
LPVARDVOBJ MallocAndInitVarDvObj(void);
LPVARGVOBJ MallocAndInitVarGvObj(void);
*/
double  *__lpeur_;
int		*__lpeui_;
BYTE	*__lpeuf_;
BYTE	*__lpeufi_;
BYTE	*__lpflg_;
BYTE	*__lpdii_;
BYTE	*__lpdoo_;
unsigned int *__lpscan_;
unsigned int *__lpout_;
CHAR8   *__lpstr_;
BYTE	*__lpstf_;



MTHANDLE InitOnspec()
{
    
    MTHANDLE _h_MtHandle;
	//Open transaction to ONSPEC,which return 0 if success,1 if error.
	_h_MtHandle=initialize(&__lpeur_,&__lpeui_,&__lpflg_,&__lpeuf_,&__lpeufi_,&__lpdii_,&__lpdoo_,&__lpscan_,&__lpout_,&__lpstr_,&__lpstf_);
	return _h_MtHandle;
}
/*
LPVARCVOBJ MallocAndInitVarCvObj()
{
	LPVARCVOBJ _lp_VarCvObj;
	LPVARCV    _lp_VarCv;
	int _i_Counter1=0;
	int _i_Counter2=0;
	int _i_Flag=0;
	

	_lp_VarCvObj=(LPVARCVOBJ)malloc(sizeof(VARCVOBJ));
	if(_lp_VarCvObj!=NULL)
	{
		_lp_VarCvObj->CurrentCvIndex=0;
		_lp_VarCvObj->CurrentIDCvMem=IDCV_FIRSTDOUBLEMEM;

		while(_i_Counter1<CV_NUMBER && _i_Flag==0)
		{
			_lp_VarCvObj->CvStatus[_i_Counter1]=GOODDATA;
			_lp_VarCv=(LPVARCV)malloc(sizeof(VARCV));
			if(_lp_VarCv!=NULL)
				_lp_VarCvObj->CvPointer[_i_Counter1]=_lp_VarCv;
			else
			{
				_i_Counter2=_i_Counter1-1;
				while(_i_Counter2>=0)
				{
					free(_lp_VarCvObj->CvPointer[_i_Counter2]);
					_i_Counter2--;
				}
				_i_Flag=1;
			}
			_i_Counter1++;
		}
	}
	if(_i_Flag==0)
		return _lp_VarCvObj;
	else
		return NULL;
}


LPVARMVOBJ MallocAndInitVarMvObj()
{
	LPVARMVOBJ _lp_VarMvObj;
	LPVARMV _lp_VarMv;
	int _i_Counter1=0;
	int _i_Counter2=0;
	int _i_Flag=0;

	_lp_VarMvObj=(LPVARMVOBJ)malloc(sizeof(VARMVOBJ));
	if(_lp_VarMvObj!=NULL)
	{
		_lp_VarMvObj->CurrentMvIndex=0;
		_lp_VarMvObj->CurrentIDMvMem=IDMV_FIRSTDOUBLEMEM;

		while(_i_Counter1<MV_NUMBER && _i_Flag==0)
		{
			_lp_VarMvObj->MvStatus[_i_Counter1]=GOODDATA;
			_lp_VarMv=(LPVARMV)malloc(sizeof(VARMV));
			if(_lp_VarMv!=NULL)
				_lp_VarMvObj->MvPointer[_i_Counter1]=_lp_VarMv;
			else
			{
				_i_Counter2=_i_Counter1-1;
				while(_i_Counter2>=0)
				{
					free(_lp_VarMvObj->MvPointer[_i_Counter2]);
					_i_Counter2--;
				}
				_i_Flag=1;
			}
			_i_Counter1++;
		}
	}
	if(_i_Flag==0)
		return _lp_VarMvObj;
	else
		return NULL;
}


LPVARDVOBJ MallocAndInitVarDvObj()
{
	LPVARDVOBJ _lp_VarDvObj;
	LPVARDV    _lp_VarDv;
	int _i_Counter1=0;
	int _i_Counter2=0;
	int _i_Flag=0;
	

	_lp_VarDvObj=(LPVARDVOBJ)malloc(sizeof(VARDVOBJ));
	if(_lp_VarDvObj!=NULL)
	{
		_lp_VarDvObj->CurrentDvIndex=0;
		_lp_VarDvObj->CurrentIDDvMem=IDDV_FIRSTDOUBLEMEM;

		while(_i_Counter1<DV_NUMBER && _i_Flag==0)
		{
			_lp_VarDvObj->DvStatus[_i_Counter1]=GOODDATA;
			_lp_VarDv=(LPVARDV)malloc(sizeof(VARDV));
			if(_lp_VarDv!=NULL)
				_lp_VarDvObj->DvPointer[_i_Counter1]=_lp_VarDv;
			else
			{
				_i_Counter2=_i_Counter1-1;
				while(_i_Counter2>=0)
				{
					free(_lp_VarDvObj->DvPointer[_i_Counter2]);
					_i_Counter2--;
				}
				_i_Flag=1;
			}
			_i_Counter1++;
		}
	}
	if(_i_Flag==0)
		return _lp_VarDvObj;
	else
		return NULL;
}

LPVARGVOBJ MallocAndInitVarGvObj()
{
	LPVARGVOBJ _lp_VarGvObj;
	LPVARGV _lp_VarGv;
	int _i_Flag=0;

	_lp_VarGvObj=(LPVARGVOBJ)malloc(sizeof(VARGVOBJ));
	if(_lp_VarGvObj!=NULL)
	{
		_lp_VarGvObj->CurrentIDGvMem=0;
		_lp_VarGv=(LPVARGV)malloc(sizeof(VARGV));
		if(_lp_VarGv!=NULL)
			_lp_VarGvObj->GvPointer=_lp_VarGv;
		else
			_i_Flag=1;
	}
	if(_i_Flag==0)
		return _lp_VarGvObj;
	else
		return NULL;
}

*/


	
