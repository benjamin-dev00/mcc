////////////////////////////////////////////////////////////////
//Mtsys.c
//S.D.Chen 97-9-11                                            //
////////////////////////////////////////////////////////////////
#include "Mtstruct.h"
#include <stdlib.h>

MTHANDLE ReadSampleTime(double*);
MTHANDLE WriteSampleTime(double);
MTHANDLE ReadMccRunFlag(BOOL*);
MTHANDLE WriteMccRunFlag(BOOL);
MTHANDLE ReadMccAutoFlag(BOOL*);
MTHANDLE WriteMccAutoFlag(BOOL);

MTHANDLE ReadSampleTime(double* _lp_SampTime)
{
	MTHANDLE _h_MtHandle;
	BYTE _b_Status;
	_h_MtHandle=readmem(EUR,SAMPTIME_LOCATION,_lp_SampTime,&_b_Status);
	return _h_MtHandle;
}

MTHANDLE WriteSampleTime(double SampTime)
{
	MTHANDLE _h_MtHandle;
	BYTE _b_Status;
	_h_MtHandle=writemem(EUR,SAMPTIME_LOCATION,SampTime,_b_Status);
	return _h_MtHandle;
}

MTHANDLE ReadMccRunFlag(BOOL* _lp_IsRun)
{
	MTHANDLE _h_MtHandle;
	BYTE	_b_Status;
	double  _dbTemp;
	if(_h_MtHandle=readmem(DII,MCCRUNFLAG_LOCATION,&_dbTemp,&_b_Status))
	   return _h_MtHandle;
	else
	{
		if((_b_Status&ON)==ON)
			*_lp_IsRun=RUN;
		else
			*_lp_IsRun=STOP;
		return NOERROR;
	}
}

MTHANDLE WriteMccRunFlag(BOOL _b_IsRun)
{
	MTHANDLE _h_MtHandle;
	BYTE	_b_Status;
	double  _dbTemp;

	if(_b_IsRun==RUN)
		_b_Status=ON;
	else
		_b_Status=OFF;

	_h_MtHandle=writemem(DII,MCCRUNFLAG_LOCATION,_dbTemp,_b_Status);
	return _h_MtHandle;
}

MTHANDLE ReadMccAutoFlag(BOOL* _lp_IsAuto)
{
	MTHANDLE _h_MtHandle;
	BYTE	_b_Status;
	double  _dbTemp;
	
	if (_h_MtHandle=readmem(DII,MCCAUTOFLAG_LOCATION,&_dbTemp,&_b_Status))
			return _h_MtHandle;
	else
	{
		if((_b_Status&ON)==ON)
			*_lp_IsAuto=AUTO;
		else
			*_lp_IsAuto=MANU;
		return NOERROR;
	}
}

MTHANDLE WriteMccAutoFlag(BOOL _b_IsAuto)
{
	MTHANDLE _h_MtHandle;
	BYTE	_b_Status;
	double  _dbTemp;

	if(_b_IsAuto==AUTO)
		_b_Status=ON;
	else
		_b_Status=OFF;

	_h_MtHandle=writemem(DII,MCCAUTOFLAG_LOCATION,_dbTemp,_b_Status);
	return _h_MtHandle;
}
