//////////////////////////////////////////////////////////////////////////
//MtIdchec.c check if the ID is valid									//
//S.D.Chen 97-9-11														//
//////////////////////////////////////////////////////////////////////////
#include "windows.h"
#include "tfalgo.h"
#include "Mtstruct.h"

MEMTYPE  IsWhatTypeCvMem(IDCVMEM);
MEMTYPE  IsWhatTypeMvMem(IDMVMEM);
MEMTYPE  IsWhatTypeDvMem(IDDVMEM);
MEMTYPE  IsWhatTypeGvMem(IDGVMEM);

MEMTYPE IsWhatTypeCvMem(IDCVMEM _i_CvMemIndex)
{
	MEMTYPE _Memtype;
	//if((_i_CvMemIndex>=IDCV_FIRSTCHARMEM)&&(_i_CvMemIndex<=IDCV_LASTCHARMEM))
	//	_Memtype=ISCVCHARMEM;
	if((_i_CvMemIndex>=IDCV_FIRSTDOUBLEMEM)&&(_i_CvMemIndex<=IDCV_LASTDOUBLEMEM))
		_Memtype=ISCVDOUBLEMEM;
	else if((_i_CvMemIndex>=IDCV_FIRSTINTMEM)&&(_i_CvMemIndex<=IDCV_LASTINTMEM))
		_Memtype=ISCVINTMEM;
	else if((_i_CvMemIndex>=IDCV_FIRSTBOOLMEM-1)&&(_i_CvMemIndex<=IDCV_LASTBOOLMEM))
		_Memtype=ISCVBOOLMEM;
	else
		_Memtype=ISNOTCVMEM;
	return _Memtype;
}

MEMTYPE IsWhatTypeMvMem(IDMVMEM _i_MvMemIndex)
{
	MEMTYPE _Memtype;
	//if((_i_MvMemIndex>=IDMV_FIRSTCHARMEM)&&(_i_MvMemIndex<=IDMV_LASTCHARMEM))
	//	_Memtype=ISMVCHARMEM;
	if((_i_MvMemIndex>=IDMV_FIRSTDOUBLEMEM)&&(_i_MvMemIndex<=IDMV_LASTDOUBLEMEM))
		_Memtype=ISMVDOUBLEMEM;
	else if((_i_MvMemIndex>=IDMV_FIRSTINTMEM)&&(_i_MvMemIndex<=IDMV_LASTINTMEM))
		_Memtype=ISMVINTMEM;
	else if((_i_MvMemIndex>=IDMV_FIRSTBOOLMEM-1)&&(_i_MvMemIndex<=IDMV_LASTBOOLMEM))
		_Memtype=ISMVBOOLMEM;
	else
		_Memtype=ISNOTMVMEM;
	return _Memtype;
}

MEMTYPE IsWhatTypeDvMem(IDDVMEM _i_DvMemIndex)
{
	MEMTYPE _Memtype;
//	if((_i_DvMemIndex>=IDDV_FIRSTCHARMEM)&&(_i_DvMemIndex<=IDDV_LASTCHARMEM))
//		_Memtype=ISDVCHARMEM;
	if((_i_DvMemIndex>=IDDV_FIRSTDOUBLEMEM)&&(_i_DvMemIndex<=IDDV_LASTDOUBLEMEM))
		_Memtype=ISDVDOUBLEMEM;
/* following codes used when DV has int member
	else if((_i_DvMemIndex>=IDDV_FIRSTINTMEM)&&(_i_DvMemIndex<=IDDV_LASTINTMEM))
		_Memtype=ISDVINTMEM;
*/
	else if((_i_DvMemIndex>=IDDV_FIRSTBOOLMEM)&&(_i_DvMemIndex<=IDDV_LASTBOOLMEM))
		_Memtype=ISDVBOOLMEM;
	else
		_Memtype=ISNOTDVMEM;
	return _Memtype;
}


MEMTYPE IsWhatTypeGvMem(IDGVMEM _i_GvMemIndex)
{
	MEMTYPE _Memtype;
	if((_i_GvMemIndex>=IDGV_FIRSTDOUBLEMEM)&&(_i_GvMemIndex<=IDGV_LASTDOUBLEMEM))
		_Memtype=ISGVDOUBLEMEM;
	else if((_i_GvMemIndex>=IDGV_FIRSTINTMEM)&&(_i_GvMemIndex<=IDGV_LASTINTMEM))
		_Memtype=ISGVINTMEM;
	else if((_i_GvMemIndex>=IDGV_FIRSTBOOLMEM)&&(_i_GvMemIndex<=IDGV_LASTBOOLMEM))
		_Memtype=ISGVBOOLMEM;
	else
		_Memtype=ISNOTGVMEM;
	return _Memtype;
}