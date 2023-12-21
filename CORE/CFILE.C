//===================================================================
//cfile.c
//Zheng zhikai	95-4-16
//
//Note:
//	RestoreCore function has been deleted, and the functions
//	are obsoleted too;
//===================================================================

#include "prehead.h"
#include "coreall.h"
#include <io.h>

typedef struct _tagCOBJ_READ_WRITE_STRUCT{
	HFILE	hfile;			
	LONG	posit_cobj;		//the next position
}CRWS, _FAR* LPCRWS;

BOOL _Init_Write_Cobj_File( LPCRWS, LPCSTR lpszFullName_ );
LONG _Save_Cobj( LPCRWS, LPCOBJ lpCobj_ );
BOOL _Cobj_File_AfterSave( LPCRWS, LPCSTR lpszFullName_ );

BOOL _Init_Read_Cobj_File( LPCRWS, LPCSTR lpszFullName_ );
LPCOBJ _Read_Cobj( LPCRWS lpCrws_, LONG pos_ );
BOOL _Cobj_File_AfterRead( LPCRWS, LPCSTR lpszFullName_ );


_DLLEXP BOOL _DLLFUNC _ReadRealTable( LPCSTR szName_, LPCRWS lpCrws_ );
_DLLEXP BOOL _ZZKDECL _ReadTintTable( LPCSTR szName_, LPCRWS lpCrws_ );
_DLLEXP BOOL _ZZKDECL _ReadBoolTable( LPCSTR szName_, LPCRWS lpCrws_ );
_DLLEXP BOOL _ZZKDECL _ReadModuTable( LPCSTR szName_, LPCRWS lpCrws_ );
_DLLEXP BOOL _ZZKDECL _ReadFaceTable( LPCSTR szName_, LPCRWS lpCrws_ );

/*
	obsolete functions
_DLLEXP BOOL _ZZKDECL _RestoreModuTable( LPCSTR szName_, LPCRWS lpCrws_  );
_DLLEXP BOOL _ZZKDECL _RestoreFaceTable( LPCSTR szName_, LPCRWS lpCrws_  );
*/

typedef struct tagMFSAVE
{
	HFILE			htu;
	HFILE			hpu;
	unsigned long	posit_tu;
	unsigned long	posit_pu;
	LPCRWS			lpCrws;
}MFSAVE, FAR* LPMFSAVE;

static BOOL _near _SRT_AfterSave(LPCSTR szName_);
static BOOL _near _STT_AfterSave(LPCSTR szName_);
static BOOL _near _SBT_AfterSave(LPCSTR szName_);
static BOOL _near _SMT_AfterSave(LPCSTR szName_);
static BOOL _near _SFT_AfterSave(LPCSTR szName_);

static BOOL _near _SRT_New(LPCSTR szName_, LPCRWS lpCrws_);
static BOOL _near _STT_New(LPCSTR szName_, LPCRWS lpCrws_);
static BOOL _near _SBT_New(LPCSTR szName_, LPCRWS lpCrws_);
static BOOL _near _SMT_New(LPCSTR szName_, LPCRWS lpCrws_);
static BOOL _near _SFT_New(LPCSTR szName_, LPCRWS lpCrws_);

extern CoreInfo		gCoreInfo;

//...................................................................
//...................................................................
_DLLEXP BOOL _DLLFUNC SaveAsCore( LPCSTR lpszCoreFullName_ )
{
	if( *lpszCoreFullName_ == 0 )
		return SaveCore();
	else
	{
		char	_szBuffer[_MAX_PATH];

		//back up the old path
		StringCopyn( _szBuffer, gCoreInfo.szCoreFullName, _MAX_PATH );

		//set new path to corebase
		StringCopyn( gCoreInfo.szCoreFullName, lpszCoreFullName_, _MAX_PATH );

		if( !SaveCore() )
		{
			//restore the corebase path
			StringCopyn( gCoreInfo.szCoreFullName, _szBuffer, _MAX_PATH );
			return FALSE;
		}
		else
			return TRUE;
	}
}

//...................................................................
//...................................................................
_DLLEXP BOOL _DLLFUNC SaveCore()
{
	char 			_szBuffer[_MAX_PATH];
	OFSTRUCT		_of;
	HFILE			_hFile;
	CoreInfo		_CoreInfo;
	CRWS			_crws; 

	if( !IsCorebaseValid() )
		return FALSE;		//no core

	sprintf(_szBuffer, "%s.cor", gCoreInfo.szCoreFullName);
	memset( &_of, 0, sizeof(_of) );
	_of.cBytes = sizeof(_of);
	_hFile = OpenFile(_szBuffer, &_of, OF_CREATE|OF_SHARE_EXCLUSIVE|OF_WRITE);
	if(_hFile == -1) return FALSE;

	//copy gCoreInfo and clear the non-saveable information
	memcpy( &_CoreInfo, &gCoreInfo, sizeof(CoreInfo) );
	memset( &(_CoreInfo.dynamic_data_begin),
			0,
			(size_t)( (LPBYTE)&_CoreInfo + sizeof(CoreInfo) - (LPBYTE)&(_CoreInfo.dynamic_data_begin)) );

	if( _lwrite( _hFile, (LPCSTR)&_CoreInfo, sizeof(CoreInfo) )
		!= sizeof(CoreInfo) )
		goto _BAD;

	_lclose(_hFile);

	if( _Init_Write_Cobj_File( &_crws, gCoreInfo.szCoreFullName ) == -1)
		goto _BAD;

	if( !_SRT_New(gCoreInfo.szCoreFullName, &_crws ) ) return FALSE;
	if( !_STT_New(gCoreInfo.szCoreFullName, &_crws ) ) return FALSE;
	if( !_SBT_New(gCoreInfo.szCoreFullName, &_crws ) ) return FALSE;
	if( !_SMT_New(gCoreInfo.szCoreFullName, &_crws ) ) return FALSE;
	if( !_SFT_New(gCoreInfo.szCoreFullName, &_crws ) ) return FALSE;

	if( !_Cobj_File_AfterSave( &_crws, gCoreInfo.szCoreFullName ) ) return FALSE;

	if( !_SRT_AfterSave(gCoreInfo.szCoreFullName) ) return FALSE;
	if( !_STT_AfterSave(gCoreInfo.szCoreFullName) ) return FALSE;
	if( !_SBT_AfterSave(gCoreInfo.szCoreFullName) ) return FALSE;
	if( !_SMT_AfterSave(gCoreInfo.szCoreFullName) ) return FALSE;
	if( !_SFT_AfterSave(gCoreInfo.szCoreFullName) ) return FALSE;

	return TRUE;

_BAD:
	_lclose(_hFile);
	return FALSE;
}

//...................................................................
//...................................................................
#define RTF_OFFSET_VERSION 		0
#define RTF_OFFSET_RANDMARK		(sizeof(BITS32))
#define RTF_OFFSET_RTINFO		(sizeof(BITS32)*2)
#define RTF_OFFSET_RTU0			(sizeof(BITS32)*2+sizeof(RTInfo))
static BOOL _near _SRT_Check( HFILE hFile_, RTInfo FAR* lpRTInfo_ );

static BOOL _near _SRT_AfterSave(LPCSTR szName_)
{
	extern RTInfo		gRTInfo;

	LPRTU 				_lpRTU;
	WRD					_i;
	char 				_szName[_MAX_PATH], _szTemp[_MAX_PATH];
	OFSTRUCT			_of;

	gRTInfo.lAccessTimes++;

	//the dirty bits of RTUs are cleared after saving
	for(_i=0, _lpRTU = gRTInfo.lpBase;
		_i<gRTInfo.wAdding;
		_i++, _lpRTU++ )
	{
		if( _lpRTU->Status & RTUBIT_VALID )
			_lpRTU->Dirty &= ~(RTU_DIRTY);
		else
		{
			//Here, a RTU that is deleted just before this
			//saving is being released room;
			memset( _lpRTU, 0, sizeof(RTU) );
		}
	}

	RealClearTableDirty();

	//prepare the file names
	sprintf( _szTemp, "%s.r~t", szName_ );
	sprintf( _szName, "%s.rtu", szName_ );

	//delete the old file
	OpenFile(_szName, &_of, OF_DELETE);
	//and then rename the temp. file to normal file
	if( rename(_szTemp, _szName) )
		return FALSE;

	return TRUE;
}

//...................................................................
//Description:
//	Save RealTable to file, and the format of Real Table file is:
//		lVersion	size = sizeof(gCoreInfo.lVersion)
//					value = gCoreInfo.lVersion
//		lRandMark	size = sizeof(gCoreInfo.lRandMark)
//					value = gCoreInfo.lRandMark
//		RTinfo		size = sizeof(gRTInfo)
//					value = gRTInfo
//		maping RealTable from RTU#0 to RTU#(gRTInfo.wAdding - 1)
//
//	The function saves the Real Table to a specified temp. file
//	with the extension ".R~T"; it returns TRUE if successful, or
//	FALSE if failed and deletes the temp. file at this case;
//Note:
//	This function should be used with care. It should be called
//	by SaveCore function;
//...................................................................
static BOOL _near _SRT_New(LPCSTR szName_, LPCRWS lpCrws_ )
{
	extern RTInfo	gRTInfo;
	RTInfo			_RTInfo;
	OFSTRUCT		_of;
	HFILE			_hNew = -1;
	char			_szName[_MAX_PATH];
	WRD				_i;
	LPRTU 			_lpRTU;

	//prepare the file name and open(create) it
	sprintf(_szName, "%s.r~t", szName_);
	_hNew = OpenFile(_szName, &_of, OF_CREATE|OF_SHARE_EXCLUSIVE|OF_WRITE);
	if(_hNew == -1)
		return FALSE;

	//write the core.version to file
	if(   _lwrite(_hNew, (LPCSTR)&(gCoreInfo.lVersion), sizeof(gCoreInfo.lVersion) )
	   != sizeof(gCoreInfo.lVersion) )
		goto _BAD;

	//write the core.RandMark to file
	if(   _lwrite(_hNew, (LPCSTR)&(gCoreInfo.lRandMark), sizeof(gCoreInfo.lRandMark) )
	   != sizeof(gCoreInfo.lRandMark) )
		goto _BAD;

	//copy gRTInfo to buffer and then increase the AccessTimes by one
	//and clear non-saveable information
	memcpy( &_RTInfo, &gRTInfo, sizeof(RTInfo) );
	_RTInfo.lAccessTimes++;
	_RTInfo.hBase = 0;
	_RTInfo.lpBase = 0;

	//write the RTInfo buffer to the file
	if( sizeof(RTInfo) != _lwrite(_hNew, (LPCSTR)&_RTInfo, sizeof(RTInfo) ) )
		goto _BAD;

	for(_i=0, _lpRTU = gRTInfo.lpBase;
		_i<gRTInfo.wAdding;
		_i++, _lpRTU++)
	{
		RTU			_rtusv;

		memcpy( &_rtusv, _lpRTU, sizeof(RTU) );

		_rtusv.Dirty = 0;
		_rtusv.lpTag = (LPCOBJ)_Save_Cobj( lpCrws_, _lpRTU->lpTag );
		_rtusv.lpUnit =	(LPCOBJ)_Save_Cobj( lpCrws_, _lpRTU->lpUnit );
		_rtusv.lpComment = (LPCOBJ)_Save_Cobj( lpCrws_, _lpRTU->lpComment );

		if( sizeof(RTU) != _lwrite( _hNew, (LPCSTR)&_rtusv, sizeof(RTU) ) )
			goto _BAD;
	}

	_lclose(_hNew);
	return TRUE;

_BAD:
	if(_hNew != -1)
	{
		_lclose(_hNew);
		//delete the temp file
		OpenFile(_szName, &_of, OF_DELETE);
	}
	return FALSE;
}

//...................................................................
//Note:
//	the struction gCoreInfo must be valid;
//...................................................................
static BOOL _near _SRT_Check(HFILE hFile_, RTInfo FAR* lpRTInfo_ )
{
	extern RTInfo	gRTInfo;
	BITS32			_lVersion, _lRandMark;
	RTInfo			_RTInfo;

	if(HFILE_ERROR == _llseek(hFile_, RTF_OFFSET_VERSION, 0) )
		return FALSE;
	if(sizeof(_lVersion) != _lread( hFile_, &_lVersion, sizeof(_lVersion) ) )
		return FALSE;
	if( _lVersion != gCoreInfo.lVersion )
		return FALSE;

	if(HFILE_ERROR == _llseek(hFile_, RTF_OFFSET_RANDMARK, 0) )
		return FALSE;
	if(sizeof(_lRandMark) != _lread( hFile_, &_lRandMark, sizeof(_lRandMark) ) )
		return FALSE;
	if( _lRandMark != (DWORD)gCoreInfo.lRandMark )
		return FALSE;

	if(HFILE_ERROR == _llseek(hFile_, RTF_OFFSET_RTINFO, 0) )
		return FALSE;
	if(sizeof(RTInfo) != _lread( hFile_, &_RTInfo, sizeof(_RTInfo) ) )
		return FALSE;
	if( _RTInfo.lAccessTimes != gRTInfo.lAccessTimes )
		return FALSE;

	if( lpRTInfo_ )
		memcpy( lpRTInfo_, &_RTInfo, sizeof(RTInfo) );

	return TRUE;
}

//...................................................................
//...................................................................
#define TTF_OFFSET_VERSION 		0
#define TTF_OFFSET_RANDMARK		sizeof(BITS32)
#define TTF_OFFSET_TTINFO		(sizeof(BITS32)*2)
#define TTF_OFFSET_TTU0			(sizeof(BITS32)*2+sizeof(TTInfo))
static BOOL _near _STT_Check(HFILE hFile_, TTInfo FAR* lpTTInfo_ );

static BOOL _near _STT_AfterSave( LPCSTR szName_ )
{
	extern TTInfo		gTTInfo;

	LPTTU 				_lpTTU;
	WRD					_i;
	char 				_szName[_MAX_PATH], _szTemp[_MAX_PATH];
	OFSTRUCT			_of;

	gTTInfo.lAccessTimes ++;

	//the dirty bits of TTUs are cleared after saving
	for(_i=0, _lpTTU = gTTInfo.lpBase;
		_i<gTTInfo.wAdding;
		_i++, _lpTTU++ )
	{
		if( _lpTTU->Status & TTUBIT_VALID )
			_lpTTU->Dirty &= ~(TTU_DIRTY);
		else
			memset( _lpTTU, 0, sizeof(TTU) );
	}

	TintClearTableDirty();

	//prepare the file names
	sprintf( _szTemp, "%s.t~t", szName_ );
	sprintf( _szName, "%s.ttu", szName_ );

	//delete the old file
	OpenFile(_szName, &_of, OF_DELETE);
	//and then rename the temp. file to normal file
	if( rename(_szTemp, _szName) )
		return FALSE;

	return TRUE;
}


//...................................................................
//Description:
//	Save TintTable to file, and the format of Tint Table file is:
//		lVersion	size = sizeof(gCoreInfo.lVersion)
//					value = gCoreInfo.lVersion
//		lRandMark	size = sizeof(gCoreInfo.lRandMark)
//					value = gCoreInfo.lRandMark
//		TTinfo		size = sizeof(gTTInfo)
//					value = gTTInfo
//		maping TintTable from TTU#0 to TTU#(gTTInfo.wAdding - 1)
//
//	The function saves the Tint Table to a specified temp. file
//	with the extension ".T~T"; it returns TRUE if successful, or
//	FALSE if failed and deletes the temp. file at this case;
//Note:
//	This function should be used with care. It should be called
//	by SaveCore function.
//...................................................................
static BOOL near _STT_New(LPCSTR szName_, LPCRWS lpCrws_ )
{
	extern TTInfo	gTTInfo;
	TTInfo			_TTInfo;
	OFSTRUCT		_of;
	HFILE			_hNew = -1;
	char			_szName[_MAX_PATH];
	WRD				_i;
	//TTU			_ttu;
	LPTTU 			_lpTTU;

	sprintf(_szName, "%s.t~t", szName_);
	_hNew = OpenFile(_szName, &_of, OF_CREATE|OF_SHARE_EXCLUSIVE|OF_WRITE);
	if(_hNew == -1) return FALSE;

	if(   _lwrite(_hNew, (LPCSTR)&(gCoreInfo.lVersion), sizeof(gCoreInfo.lVersion) )
	   != sizeof(gCoreInfo.lVersion) )
		goto _BAD;
	if(   _lwrite(_hNew, (LPCSTR)&(gCoreInfo.lRandMark), sizeof(gCoreInfo.lRandMark) )
	   != sizeof(gCoreInfo.lRandMark) )
		goto _BAD;

	memcpy(&_TTInfo, &gTTInfo, sizeof(TTInfo) );
	_TTInfo.lAccessTimes ++;
	_TTInfo.hBase = 0;
	_TTInfo.lpBase = 0;
	if( sizeof(TTInfo) != _lwrite(_hNew, (LPCSTR)&_TTInfo, sizeof(TTInfo) ) )
		goto _BAD;

	for(_i=0, _lpTTU = gTTInfo.lpBase;
		_i<gTTInfo.wAdding;
		_i++, _lpTTU++)
	{
		TTU			_ttusv;

		memcpy( &(_ttusv), _lpTTU, sizeof(TTU) );
		
		_ttusv.Dirty = 0;
		_ttusv.lpTag = (LPCOBJ)_Save_Cobj( lpCrws_, _lpTTU->lpTag );
		_ttusv.lpUnit =	(LPCOBJ)_Save_Cobj( lpCrws_, _lpTTU->lpUnit );
		_ttusv.lpComment = (LPCOBJ)_Save_Cobj( lpCrws_, _lpTTU->lpComment );

		if( sizeof(TTU) != _lwrite( _hNew, (LPCSTR)&_ttusv, sizeof(TTU) ) )
			goto _BAD;
	}
	_lclose(_hNew);
	return TRUE;

_BAD:
	if(_hNew != -1)
	{
		_lclose(_hNew);
		//delete the temp file
		OpenFile(_szName, &_of, OF_DELETE);
	}
	return FALSE;
}

//...................................................................
//Note:
//	the struction gCoreInfo must be valid;
//...................................................................
static BOOL _near _STT_Check(HFILE hFile_, TTInfo FAR* lpTTInfo_ )
{
	extern TTInfo	gTTInfo;
	BITS32			_lVersion, _lRandMark;
	TTInfo			_TTInfo;

	if(HFILE_ERROR == _llseek(hFile_, TTF_OFFSET_VERSION, 0) )
		return FALSE;
	if(sizeof(_lVersion) != _lread( hFile_, &_lVersion, sizeof(_lVersion) ) )
		return FALSE;
	if( _lVersion != gCoreInfo.lVersion )
		return FALSE;

	if(HFILE_ERROR == _llseek(hFile_, TTF_OFFSET_RANDMARK, 0) )
		return FALSE;
	if(sizeof(_lRandMark) != _lread( hFile_, &_lRandMark, sizeof(_lRandMark) ) )
		return FALSE;
	if( _lRandMark != (DWORD)gCoreInfo.lRandMark )
		return FALSE;

	if(HFILE_ERROR == _llseek(hFile_, TTF_OFFSET_TTINFO, 0) )
		return FALSE;
	if(sizeof(TTInfo) != _lread( hFile_, &_TTInfo, sizeof(_TTInfo) ) )
		return FALSE;
	if( _TTInfo.lAccessTimes != gTTInfo.lAccessTimes )
		return FALSE;

	if( lpTTInfo_ )
		memcpy( lpTTInfo_, &_TTInfo, sizeof(TTInfo) );
	return TRUE;
}

//...................................................................
//...................................................................
#define BTF_OFFSET_VERSION 		0
#define BTF_OFFSET_RANDMARK		sizeof(BITS32)
#define BTF_OFFSET_BTINFO		(sizeof(BITS32)*2)
#define BTF_OFFSET_BTU0			(sizeof(BITS32)*2+sizeof(BTInfo))
static BOOL _near _SBT_Check(HFILE hFile_, BTInfo FAR* lpBTInfo_ );
static BOOL _near _SBT_AfterSave(LPCSTR szName_)
{
	extern BTInfo		gBTInfo;

	LPBTU 				_lpBTU;
	WRD					_i;
	char 				_szName[_MAX_PATH], _szTemp[_MAX_PATH];
	OFSTRUCT			_of;

	gBTInfo.lAccessTimes ++;

	//the dirty bits of BTUs are cleared after saving
	for(_i=0, _lpBTU = gBTInfo.lpBase;
		_i<gBTInfo.wAdding;
		_i++, _lpBTU++ )
	{
		if( _lpBTU->Status & BTUBIT_VALID )
			_lpBTU->Dirty &= ~(BTU_DIRTY);
		else
			memset( _lpBTU, 0, sizeof(BTU) );
	}

	BoolClearTableDirty();

	//prepare the file names
	sprintf( _szTemp, "%s.b~t", szName_ );
	sprintf( _szName, "%s.btu", szName_ );

	//delete the old file
	OpenFile(_szName, &_of, OF_DELETE);
	//and then rename the temp. file to normal file
	if( rename(_szTemp, _szName) )
		return FALSE;

	return TRUE;
}


//...................................................................
//Description:
//	Save BoolTable to file, and the format of Bool Table file is:
//		lVersion	size = sizeof(gCoreInfo.lVersion)
//					value = gCoreInfo.lVersion
//		lRandMark	size = sizeof(gCoreInfo.lRandMark)
//					value = gCoreInfo.lRandMark
//		BTinfo		size = sizeof(gBTInfo)
//					value = gBTInfo
//		maping BoolTable from BTU#0 to BTU#(gBTInfo.wAdding - 1)
//
//	The function saves the Bool Table to a specified temp. file
//	with the extension ".B~T"; it returns TRUE if successful, or
//	FALSE if failed and deletes the temp. file at this case;
//Note:
//	This function should be used with care. It should be called
//	by SaveCore function.
//...................................................................
static BOOL near _SBT_New(LPCSTR szName_, LPCRWS lpCrws_ )
{
	extern BTInfo	gBTInfo;
	BTInfo			_BTInfo;
	OFSTRUCT		_of;
	HFILE			_hNew = -1;
	WRD				_i;
	//BTU				_btu;
	LPBTU 			_lpBTU;
	char			_szName[_MAX_PATH];

	sprintf(_szName, "%s.b~t", szName_);
	_hNew = OpenFile(_szName, &_of, OF_CREATE|OF_SHARE_EXCLUSIVE|OF_WRITE);
	if(_hNew == -1) return FALSE;

	if(   _lwrite(_hNew, (LPCSTR)&(gCoreInfo.lVersion), sizeof(gCoreInfo.lVersion) )
	   != sizeof(gCoreInfo.lVersion) )
		goto _BAD;

	if(   _lwrite(_hNew, (LPCSTR)&(gCoreInfo.lRandMark), sizeof(gCoreInfo.lRandMark) )
	   != sizeof(gCoreInfo.lRandMark) )
		goto _BAD;

	memcpy( &_BTInfo, &gBTInfo, sizeof(BTInfo) );
	_BTInfo.lAccessTimes++;
	_BTInfo.hBase = 0;
	_BTInfo.lpBase = 0;
	if( sizeof(BTInfo) != _lwrite(_hNew, (LPCSTR)&_BTInfo, sizeof(BTInfo) ) )
		goto _BAD;

	for(_i=0, _lpBTU = gBTInfo.lpBase;
		_i<gBTInfo.wAdding;
		_i++, _lpBTU++)
	{
		BTU			_btusv;

		memcpy( &(_btusv), _lpBTU, sizeof(BTU) );

		_btusv.Dirty = 0;
		_btusv.lpTag = (LPCOBJ)_Save_Cobj( lpCrws_, _lpBTU->lpTag );
		_btusv.lpUnit =	(LPCOBJ)_Save_Cobj( lpCrws_, _lpBTU->lpUnit );
		_btusv.lpComment = (LPCOBJ)_Save_Cobj( lpCrws_, _lpBTU->lpComment );

		if( sizeof(BTU) != _lwrite( _hNew, (LPCSTR)&_btusv, sizeof(BTU) ) )
			goto _BAD;
	}
	_lclose(_hNew);
	return TRUE;

_BAD:
	if(_hNew != -1)
	{
		_lclose(_hNew);
		//delete the temp file
		OpenFile(_szName, &_of, OF_DELETE);
	}
	return FALSE;
}

//...................................................................
//Note:
//	the struction gCoreInfo must be valid;
//...................................................................
static BOOL _near _SBT_Check(HFILE hFile_, BTInfo FAR* lpBTInfo_ )
{
	extern BTInfo	gBTInfo;
	BITS32			_lVersion, _lRandMark;
	BTInfo			_BTInfo;

	if(HFILE_ERROR == _llseek(hFile_, BTF_OFFSET_VERSION, 0) )
		return FALSE;
	if(sizeof(_lVersion) != _lread( hFile_, &_lVersion, sizeof(_lVersion) ) )
		return FALSE;
	if( _lVersion != gCoreInfo.lVersion )
		return FALSE;

	if(HFILE_ERROR == _llseek(hFile_, BTF_OFFSET_RANDMARK, 0) )
		return FALSE;
	if(sizeof(_lRandMark) != _lread( hFile_, &_lRandMark, sizeof(_lRandMark) ) )
		return FALSE;
	if( _lRandMark != (DWORD)gCoreInfo.lRandMark )
		return FALSE;

	if(HFILE_ERROR == _llseek(hFile_, BTF_OFFSET_BTINFO, 0) )
		return FALSE;
	if(sizeof(BTInfo) != _lread( hFile_, &_BTInfo, sizeof(_BTInfo) ) )
		return FALSE;
	if( _BTInfo.lAccessTimes != gBTInfo.lAccessTimes )
		return FALSE;

	if( lpBTInfo_ )
		memcpy( lpBTInfo_, &_BTInfo, sizeof(BTInfo) );

	return TRUE;
}

/////////////////////////////////////////////////////////////////////
//function both for saving Modu table and Face table
static BOOL near _SavePU_New( LPMFSAVE lpmfs_, LPPU	lppu_);

/////////////////////////////////////////////////////////////////////
//for saving Modu table
static BOOL near _SMT_SaveMTUParam_New(LPMFSAVE lpmfs_, LPMTU lpMTU_);

static BOOL near _SMT_Check(HFILE hFile_, MTInfo FAR* lpMTInfo_ );
static BOOL near _SMT_SaveHead(LPMFSAVE lpmfs_);

#define MTF_OFFSET_MTINFO		(sizeof(BITS32)*2)
#define MTF_OFFSET_MTU0			(sizeof(BITS32)*2+sizeof(MTInfo))
//...................................................................
//Description:
//...................................................................
static BOOL _near _SMT_AfterSave(LPCSTR szName_)
{
	extern MTInfo	gMTInfo;

	WRD				_i;
	//WORD			_j;
	LPMTU 			_lpMTU;
	char 			_szName[_MAX_PATH], _szTemp[_MAX_PATH];
	OFSTRUCT		_of;

	gMTInfo.lAccessTimes++;

	//the dirty bits of MTUs are cleared after saving
	for(_lpMTU = gMTInfo.lpBase, _i=0;
		_i<gMTInfo.wAdding;
		_i++, _lpMTU++)
	{
		if( _lpMTU->Status & MTUBIT_VALID )
			_lpMTU->Dirty &= ~(MTU_DIRTY);
		else
			memset( _lpMTU, 0, sizeof(MTU) );
	}

	//the dirty bits of FTUs are cleared after saving
	for(_i=1; _i<gMTInfo.wAdding; _i++)
		ModuClearDirty( _i );

	//prepare the file names
	sprintf( _szTemp, "%s.m~t", szName_ );
	sprintf( _szName, "%s.mtu", szName_ );

	//delete the old file
	OpenFile(_szName, &_of, OF_DELETE);
	//and then rename the temp. file to normal file
	if( rename(_szTemp, _szName) )
		return FALSE;

	//prepare the file names
	sprintf( _szTemp, "%s.m~p", szName_ );
	sprintf( _szName, "%s.mpu", szName_ );

	//delete the old file
	OpenFile(_szName, &_of, OF_DELETE);
	//and then rename the temp. file to normal file
	if( rename(_szTemp, _szName) )
		return FALSE;

	return TRUE;
}

//...................................................................
//Description:
//	Save ModuTable to file, and the format ModuTable file is:
//		lVersion	size = sizeof(gCoreInfo.lVersion)
//					value = gCoreInfo.lVersion
//		lRandMark	size = sizeof(gCoreInfo.lRandMark)
//					value = gCoreInfo.lRandMark
//		MTinfo		size = sizeof(gMTInfo)
//					value = gMTInfo
//	The function saves the Modu Table to a specified temp. file
//	with the extension ".M~T"; it returns TRUE if successful, or
//	FALSE if failed and deletes the temp. file at this case;
//		maping ModuTable from MTU#0 to MTU#(gMTInfo.wAdding - 1)
//	The file extension name is .MTU
//	and the another file is created too:
//			.M~P		saving PUs of Modus
//
//Note:
//	This function should be used with care. It should be called
//	by SaveCore function.
//...................................................................
static BOOL near _SMT_New(LPCSTR szName_, LPCRWS lpCrws_)
{
	extern MTInfo	gMTInfo;
	char 			_szBuffer[_MAX_PATH];
	OFSTRUCT		_of;
	WRD				_i;
	MFSAVE			_mfs;
	LPMTU 			_lpMTU;

	if( !gCoreInfo.lVersion )
		return FALSE;				//no core

	sprintf(_szBuffer, "%s.m~t", szName_);
	_mfs.htu = OpenFile(_szBuffer, &_of, OF_CREATE|OF_SHARE_EXCLUSIVE|OF_WRITE);
	if(_mfs.htu == -1) return FALSE;

	sprintf(_szBuffer, "%s.m~p", szName_);
	_mfs.hpu = OpenFile(_szBuffer, &_of, OF_CREATE|OF_SHARE_EXCLUSIVE|OF_WRITE);
	if(_mfs.hpu == -1)
	{
		_lclose(_mfs.htu);
		return FALSE;
	}

	_mfs.lpCrws = lpCrws_;
	
	if( !_SMT_SaveHead(&_mfs) ) goto _BAD;

	for(_i=0, _lpMTU = gMTInfo.lpBase;
		_i < gMTInfo.wAdding;
		_i++, _lpMTU++)
	{
		MTU		_mtusv;

		if(  ( (_lpMTU->Status & MTUBIT_VALID) == NULL )
		   ||( _lpMTU->Status & MTUBIT_TEMP) )
		{
			//here, the invalid or temp. MTU
			memset( &_mtusv, 0, sizeof(MTU) );
		}
		else
		{
			if( !AlgoCallParamProc( _lpMTU->Algo, PIC_BEFORESAVE, _i, 0, 0 ) )
				goto _BAD;

			memcpy( &_mtusv, _lpMTU, sizeof(MTU) );
			if( _lpMTU->NumParam )
			{
				CORE_ASSERT( _lpMTU->lpParam);
				_mtusv.lpParam = (LPPU)_mfs.posit_pu;
				if( !_SMT_SaveMTUParam_New(&_mfs, _lpMTU) )
					goto _BAD;
			}
		}

		_mtusv.Dirty = 0;
		_mtusv.lpTag = (LPCOBJ)_Save_Cobj( lpCrws_, _lpMTU->lpTag );
		_mtusv.lpComment = (LPCOBJ)_Save_Cobj( lpCrws_, _lpMTU->lpComment );

		if( _lwrite(_mfs.htu, (LPCSTR)&_mtusv, sizeof(MTU)) != sizeof(MTU))
			return FALSE;
	}	//end for(...)

	_lclose(_mfs.htu);
	_lclose(_mfs.hpu);
	return TRUE;

_BAD:
	_lclose(_mfs.htu);
	_lclose(_mfs.hpu);

	sprintf(_szBuffer, "%s.m~t", szName_);
	OpenFile(_szBuffer, &_of, OF_DELETE);		//delete the old file

	sprintf(_szBuffer, "%s.m~p", szName_);
	OpenFile(_szBuffer, &_of, OF_DELETE);		//delete the old file

	return FALSE;
}

//...................................................................
//Description:
//...................................................................
static BOOL near _SMT_SaveMTUParam_New(LPMFSAVE lpmfs_, LPMTU lpMTU_)
{
	WORD		_j;
	LPPU		_lppu;

	CORE_ASSERT( lpMTU_->NumParam );
	CORE_ASSERT( lpMTU_->lpParam );

	for(_j=0, _lppu = lpMTU_->lpParam;
		_j < lpMTU_->NumParam;
		_j++, _lppu++)
	{
		if(!_SavePU_New(lpmfs_, _lppu))
			return FALSE;
	}//end for

	return TRUE;
}

//...................................................................
//Description:
//...................................................................
static BOOL near _SavePU_New( LPMFSAVE lpmfs_, LPPU	lppu_)
{
	PU		_pu;

	memcpy(&_pu, lppu_, sizeof(PU) );

	if( (lppu_->prefix & PF_MASK) == PF_IMM )
	{
		if( lppu_->flags & PU_NIV )
			_pu.du.zzk__FULL = 0;
		else
		{   //here, PU_NIV bit does NOT set
			if( (lppu_->prefix & IMM_MASK) == IMM_LPCOBJ )
			{
				_pu.du.zzk__long = _Save_Cobj( lpmfs_->lpCrws, lppu_->du.zzk__LPCOBJ );
			} //endif the PU is a COBJ
			else if( (lppu_->prefix & IMM_MASK) == IMM_FPTR )
			{
				_pu.du.zzk__FPTR = NULL;
			}
			else if( (lppu_->prefix & IMM_MASK) == IMM_NPTR )
			{
				_pu.du.zzk__NPTR = NULL;
			}
		}//end-else the PU is un-NIV
	}//end if the PU is PF_IMM

	if( sizeof(PU) != _hwrite(lpmfs_->hpu, (LPCSTR)&_pu, sizeof(PU) ) )
		return FALSE;

	lpmfs_->posit_pu += sizeof(PU);

	return TRUE;
}

//...................................................................
//Description:
//	writes the head of .MTU, .MPU file
//...................................................................
static BOOL near _SMT_SaveHead(LPMFSAVE lpmfs_)
{
	extern MTInfo	gMTInfo;
	MTInfo			_MTInfo;

	memcpy(&_MTInfo, &gMTInfo, sizeof(MTInfo) );
	_MTInfo.lAccessTimes++;
	_MTInfo.hBase = 0;
	_MTInfo.lpBase = 0;

	if(   _lwrite( lpmfs_->htu, (LPCSTR)&(gCoreInfo.lVersion), sizeof(gCoreInfo.lVersion))
	   != sizeof(gCoreInfo.lVersion) ) return FALSE;
	if(   _lwrite( lpmfs_->htu, (LPCSTR)&(gCoreInfo.lRandMark), sizeof(gCoreInfo.lRandMark))
	   != sizeof(gCoreInfo.lRandMark) ) return FALSE;
	if(   _hwrite( lpmfs_->htu, (LPCSTR)&_MTInfo, sizeof(MTInfo) )
	   != sizeof(MTInfo) ) return FALSE;

	if(   _lwrite( lpmfs_->hpu, (LPCSTR)&(gCoreInfo.lVersion), sizeof(gCoreInfo.lVersion))
	   != sizeof(gCoreInfo.lVersion) ) return FALSE;
	if(   _lwrite( lpmfs_->hpu, (LPCSTR)&(gCoreInfo.lRandMark), sizeof(gCoreInfo.lRandMark))
	   != sizeof(gCoreInfo.lRandMark) ) return FALSE;
	if(   _hwrite( lpmfs_->hpu, (LPCSTR)&_MTInfo, sizeof(MTInfo) )
	   != sizeof(MTInfo) ) return FALSE;

	lpmfs_->posit_tu = lpmfs_->posit_pu
					 = sizeof(BITS32)*2 + sizeof(MTInfo);

	return TRUE;
}

//...................................................................
//Description:
//	Checks .MTU, .MPU file
//Note:
//	the struction gCoreInfo must be valid;
//...................................................................
static BOOL near _SMT_Check(HFILE hFile_, MTInfo FAR* lpMTInfo_ )
{
	extern MTInfo	gMTInfo;
	BITS32		_lVersion;
	long		_lrm;
	MTInfo		_MTInfo;

	if(sizeof(_lVersion) != _lread( hFile_, &_lVersion, sizeof(_lVersion) ) )
		return FALSE;
	if( _lVersion != gCoreInfo.lVersion )
		return FALSE;	//version unmatch

	if(sizeof(_lrm) != _lread( hFile_, &_lrm, sizeof(_lrm) ) )
		return FALSE;	//read failed
	if( _lrm != gCoreInfo.lRandMark )
		return FALSE;	//RandMark unmatch

	if(sizeof(MTInfo) != _lread( hFile_, &_MTInfo, sizeof(_MTInfo) ) )
		return FALSE;
	if( _MTInfo.lAccessTimes != gMTInfo.lAccessTimes )
		return FALSE;	//access times unmatch

	if( lpMTInfo_ )
		memcpy( lpMTInfo_, &_MTInfo, sizeof(MTInfo) );
	return TRUE;
}

/////////////////////////////////////////////////////////////////////
//for saving Face table
static BOOL near _SFT_SaveFTUParam_New( LPMFSAVE lpmfs_, LPFTU lpFTU_ );

static BOOL near _SFT_Check( HFILE hFile_, FTInfo FAR* lpFTInfo_ );
static BOOL near _SFT_SaveHead( LPMFSAVE lpmfs_ );


#define FTF_OFFSET_FTINFO		(sizeof(BITS32)*2)
#define FTF_OFFSET_FTU0			(sizeof(BITS32)*2+sizeof(FTInfo))
//...................................................................
//Description:
//...................................................................
static BOOL _near _SFT_AfterSave(LPCSTR szName_)
{
	extern FTInfo		gFTInfo;

	WRD					_i;
	//WORD				_j;
	LPFTU 				_lpFTU;
	char 				_szName[_MAX_PATH], _szTemp[_MAX_PATH];
	OFSTRUCT			_of;

	//increase AccessTimes
	gFTInfo.lAccessTimes ++;

	for(_lpFTU = gFTInfo.lpBase, _i=0;
		_i<gFTInfo.wAdding;
		_i++, _lpFTU++)
	{
		if( _lpFTU->Status & FTUBIT_VALID )
			_lpFTU->Dirty &= (~(FTU_DIRTY));
		else
			memset( _lpFTU, 0, sizeof(FTU) );
	}

	//the dirty bits of FTUs are cleared after saving
	for(_i=1; _i<gFTInfo.wAdding; _i++)
		FaceClearDirty( _i );

	//prepare the file names
	sprintf( _szTemp, "%s.f~t", szName_ );
	sprintf( _szName, "%s.ftu", szName_ );

	//delete the old file
	OpenFile(_szName, &_of, OF_DELETE);
	//and then rename the temp. file to normal file
	if( rename(_szTemp, _szName) )
		return FALSE;

	//prepare the file names
	sprintf( _szTemp, "%s.f~p", szName_ );
	sprintf( _szName, "%s.fpu", szName_ );

	//delete the old file
	OpenFile(_szName, &_of, OF_DELETE);
	//and then rename the temp. file to normal file
	if( rename(_szTemp, _szName) )
		return FALSE;

	return TRUE;
}

//...................................................................
//Description:
//	Save FaceTable to file, and the format FaceTable file is:
//		lVersion	size = sizeof(gCoreInfo.lVersion)
//					value = gCoreInfo.lVersion
//		lRandMark	size = sizeof(gCoreInfo.lRandMark)
//					value = gCoreInfo.lRandMark
//		FTinfo		size = sizeof(gFTInfo)
//					value = gFTInfo
//	The function saves the Modu Table to a specified temp. file
//	with the extension ".M~T"; it returns TRUE if successful, or
//	FALSE if failed and deletes the temp. file at this case;
//		maping FaceTable from FTU#0 to FTU#(gFTInfo.wAdding - 1)
//	The file extension name is .FTU
//	and the another file is created too:
//			.FPU		saving PUs of Faces
//
//Note:
//	This function should be used with care. It should be called
//	by SaveCore function.
//...................................................................
static BOOL near _SFT_New(LPCSTR szName_, LPCRWS lpCrws_)
{
	extern FTInfo	gFTInfo;
	char 			_szBuffer[_MAX_PATH];
	OFSTRUCT		_of;
	FTU				_ftu;
	WRD				_i;
	MFSAVE			_mfs;
	LPFTU 			_lpFTU;

	if( !gCoreInfo.lVersion )
		return FALSE;				//no core

	sprintf(_szBuffer, "%s.f~t", szName_);
	_mfs.htu = OpenFile(_szBuffer, &_of, OF_CREATE|OF_SHARE_EXCLUSIVE|OF_WRITE);
	if(_mfs.htu == -1) return FALSE;

	sprintf(_szBuffer, "%s.f~p", szName_);
	_mfs.hpu = OpenFile(_szBuffer, &_of, OF_CREATE|OF_SHARE_EXCLUSIVE|OF_WRITE);
	if(_mfs.hpu == -1)
	{
		_lclose(_mfs.htu);
		return FALSE;
	}

	_mfs.lpCrws = lpCrws_;

	if( !_SFT_SaveHead(&_mfs) ) goto _BAD;

	for(_i=0, _lpFTU = gFTInfo.lpBase;
		_i < gFTInfo.wAdding;
		_i++, _lpFTU++)
	{
		if(  ( (_lpFTU->Status & FTUBIT_VALID) == NULL )
		   ||( _lpFTU->Status & FTUBIT_TEMP) )
		{
			memset( &_ftu, 0, sizeof(FTU) );
		}
		else
		{	//here, is a valid FTU
			if( !StampCallParamProc( _lpFTU->Stamp, PIC_BEFORESAVE, _i, 0, 0 ) )
				goto _BAD;

			memcpy( &_ftu, _lpFTU, sizeof(FTU) );
			if( _lpFTU->NumParam )
			{
				CORE_ASSERT( _lpFTU->lpParam);
				_ftu.lpParam = (LPPU)_mfs.posit_pu;
				if( !_SFT_SaveFTUParam_New(&_mfs, _lpFTU) )
					goto _BAD;
			}
		}
		_ftu.Dirty = 0;
		if( _lwrite(_mfs.htu, (LPCSTR)&_ftu, sizeof(FTU)) != sizeof(FTU))
			return FALSE;
	}	//end for(...)

	_lclose(_mfs.htu);
	_lclose(_mfs.hpu);
	return TRUE;

_BAD:
	_lclose(_mfs.htu);
	_lclose(_mfs.hpu);

	sprintf(_szBuffer, "%s.f~t", szName_);
	OpenFile(_szBuffer, &_of, OF_DELETE);		//delete the old file

	sprintf(_szBuffer, "%s.f~p", szName_);
	OpenFile(_szBuffer, &_of, OF_DELETE);		//delete the old file

	return FALSE;
}

//...................................................................
//Description:
//...................................................................
static BOOL near _SFT_SaveFTUParam_New(LPMFSAVE lpmfs_, LPFTU lpFTU_)
{
	WORD		_j;
	LPPU		_lppu;

	CORE_ASSERT( lpFTU_->NumParam );
	CORE_ASSERT( lpFTU_->lpParam );

	for(_j = 0, _lppu = lpFTU_->lpParam;
		_j<lpFTU_->NumParam;
		_j++, _lppu++)
	{
		if(!_SavePU_New(lpmfs_, _lppu))
			return FALSE;
	}//end for

	return TRUE;
}

//...................................................................
//Description:
//	writes the head of .FTU, .MPU file
//...................................................................
static BOOL near _SFT_SaveHead(LPMFSAVE lpmfs_)
{
	extern FTInfo	gFTInfo;
	FTInfo			_FTInfo;

	memcpy(&_FTInfo, &gFTInfo, sizeof(FTInfo) );
	_FTInfo.lAccessTimes++;
	_FTInfo.hBase = 0;
	_FTInfo.lpBase = 0;

	if(   _lwrite( lpmfs_->htu, (LPCSTR)&(gCoreInfo.lVersion), sizeof(gCoreInfo.lVersion))
	   != sizeof(gCoreInfo.lVersion) ) return FALSE;
	if(   _lwrite( lpmfs_->htu, (LPCSTR)&(gCoreInfo.lRandMark), sizeof(gCoreInfo.lRandMark))
	   != sizeof(gCoreInfo.lRandMark) ) return FALSE;
	if(   _hwrite( lpmfs_->htu, (LPCSTR)&_FTInfo, sizeof(FTInfo) )
	   != sizeof(FTInfo) ) return FALSE;

	if(   _lwrite( lpmfs_->hpu, (LPCSTR)&(gCoreInfo.lVersion), sizeof(gCoreInfo.lVersion))
	   != sizeof(gCoreInfo.lVersion) ) return FALSE;
	if(   _lwrite( lpmfs_->hpu, (LPCSTR)&(gCoreInfo.lRandMark), sizeof(gCoreInfo.lRandMark))
	   != sizeof(gCoreInfo.lRandMark) ) return FALSE;
	if(   _hwrite( lpmfs_->hpu, (LPCSTR)&_FTInfo, sizeof(FTInfo) )
	   != sizeof(FTInfo) ) return FALSE;

	lpmfs_->posit_tu = lpmfs_->posit_pu
					 = sizeof(BITS32)*2 + sizeof(FTInfo);

	return TRUE;
}

//...................................................................
//Description:
//	Checks .FTU, .MPU, file
//Note:
//	the struction gCoreInfo must be valid;
//...................................................................
static BOOL near _SFT_Check(HFILE hFile_, FTInfo FAR* lpFTInfo_ )
{
	extern FTInfo	gFTInfo;
	BITS32		_lVersion;
	long		_lrm;
	FTInfo		_FTInfo;

	if(sizeof(_lVersion) != _lread( hFile_, &_lVersion, sizeof(_lVersion) ) )
		return FALSE;
	if( _lVersion != gCoreInfo.lVersion )
		return FALSE;	//version unmatch

	if(sizeof(_lrm) != _lread( hFile_, &_lrm, sizeof(_lrm) ) )
		return FALSE;	//read failed
	if( _lrm != gCoreInfo.lRandMark )
		return FALSE;	//RandMark unmatch

	if(sizeof(FTInfo) != _lread( hFile_, &_FTInfo, sizeof(_FTInfo) ) )
		return FALSE;
	if( _FTInfo.lAccessTimes != gFTInfo.lAccessTimes )
		return FALSE;	//access times unmatch

	if( lpFTInfo_ )
		memcpy( lpFTInfo_, &_FTInfo, sizeof(FTInfo) );
	return TRUE;
}

/////////////////////////////////////////////////////////////////////
//			Read Core Tables                                       //
/////////////////////////////////////////////////////////////////////

//...................................................................
//Description:
//	Reads the whole of core database from the disk files.
//Note:
//	(1)	This function is not an exported function, and should
//		be called by LoadCore(...)
//	(2)	Before the _ReadCore, the corebase must be free!
//...................................................................
BOOL _NONEXPORTED _ReadCore( LPCSTR szName_ )
{
	extern	RTInfo	gRTInfo;
	extern	TTInfo	gTTInfo;
	extern	BTInfo	gBTInfo;
	extern	MTInfo	gMTInfo;
	extern	FTInfo	gFTInfo;

	char 			_szBuffer[_MAX_PATH];
	OFSTRUCT		_of;
	HFILE			_hFile;
	CoreInfo		_CoreInfo;
	RTInfo			_RTInfo;
	BTInfo			_BTInfo;
	TTInfo			_TTInfo;
	MTInfo			_MTInfo;
	FTInfo			_FTInfo;

	CRWS			_crws; 
	UINT			_readbyte;

	CORE_ASSERT( !IsCorebaseValid() );

	sprintf( _szBuffer, "%s.cor", szName_ );
	_hFile = OpenFile(_szBuffer, &_of, OF_SHARE_EXCLUSIVE|OF_READ);
	if(_hFile == -1) return FALSE;

	//Note:
	//	Here, the size of CoreInfo between different versions may be 
	//	different, in order to avoid this problem, here only check 
	//	whether the error occurred 
	_readbyte = _lread( _hFile, &_CoreInfo, sizeof(CoreInfo) );
	if( _readbyte == HFILE_ERROR )
		goto _BAD;
	else if( _readbyte != sizeof(CoreInfo) )
	{
		LOGWARN0( "CoreInfo size is different" );
	}

	if( _CoreInfo.lVersion != COREVERSION )
	{
		MessageBox( NULL,
					"The incorrect version of the core database", "Error", MB_ERROR );
		goto _BAD;
	}
	_lclose(_hFile);

	//check .RTU file and get RTInfo.wCount
	sprintf( _szBuffer, "%s.rtu", szName_ );
	_hFile = OpenFile( _szBuffer, &_of, OF_SHARE_EXCLUSIVE|OF_READ );
	if( _hFile == -1 )
		return FALSE;
	if( HFILE_ERROR == _llseek( _hFile, RTF_OFFSET_RTINFO, 0 ) )
		return FALSE;
	if( sizeof(RTInfo) != _lread( _hFile, &_RTInfo, sizeof(_RTInfo) ) )
		return FALSE;
	_lclose(_hFile);

	//check .TTU file and get TTInfo.wCount
	sprintf(_szBuffer, "%s.ttu", szName_);
	_hFile = OpenFile(_szBuffer, &_of, OF_SHARE_EXCLUSIVE|OF_READ);
	if(_hFile == -1) return FALSE;
	if( HFILE_ERROR == _llseek( _hFile, TTF_OFFSET_TTINFO, 0 ) )
		return FALSE;
	if( sizeof(TTInfo) != _lread( _hFile, &_TTInfo, sizeof(_TTInfo) ) )
		return FALSE;
	_lclose(_hFile);

	//check .BTU file and get BTInfo.wCount
	sprintf(_szBuffer, "%s.btu", szName_);
	_hFile = OpenFile(_szBuffer, &_of, OF_SHARE_EXCLUSIVE|OF_READ);
	if(_hFile == -1) return FALSE;
	if( HFILE_ERROR == _llseek( _hFile, BTF_OFFSET_BTINFO, 0 ) )
		return FALSE;
	if( sizeof(BTInfo) != _lread( _hFile, &_BTInfo, sizeof(_BTInfo) ) )
		return FALSE;
	_lclose(_hFile);

	//check .MTU file and get MTInfo.wCount
	sprintf(_szBuffer, "%s.mtu", szName_);
	_hFile = OpenFile(_szBuffer, &_of, OF_SHARE_EXCLUSIVE|OF_READ);
	if(_hFile == -1) return FALSE;
	if( HFILE_ERROR == _llseek( _hFile, MTF_OFFSET_MTINFO, 0 ) )
		return FALSE;
	if( sizeof(MTInfo) != _lread( _hFile, &_MTInfo, sizeof(_MTInfo) ) )
		return FALSE;
	_lclose(_hFile);

	//check .FTU file and get FTInfo.wCount
	sprintf(_szBuffer, "%s.ftu", szName_);
	_hFile = OpenFile(_szBuffer, &_of, OF_SHARE_EXCLUSIVE|OF_READ);
	if(_hFile == -1) return FALSE;
	if( HFILE_ERROR == _llseek( _hFile, FTF_OFFSET_FTINFO, 0 ) )
		return FALSE;
	if( sizeof(FTInfo) != _lread( _hFile, &_FTInfo, sizeof(_FTInfo) ) )
		return FALSE;
	_lclose(_hFile);

	if(!_AllocCore( _RTInfo.wCount,
					_TTInfo.wCount,
					_BTInfo.wCount,
					_MTInfo.wCount,
					_FTInfo.wCount ) )
		return FALSE;

    //set gCoreInfo
	memcpy( &gCoreInfo, &_CoreInfo, sizeof(CoreInfo) );
	//the member of gCoreInfo.szCoreFullName will set at LoadCore()

	gRTInfo.lAccessTimes = _RTInfo.lAccessTimes;
	gTTInfo.lAccessTimes = _TTInfo.lAccessTimes;
	gBTInfo.lAccessTimes = _BTInfo.lAccessTimes;
	gMTInfo.lAccessTimes = _MTInfo.lAccessTimes;
	gFTInfo.lAccessTimes = _FTInfo.lAccessTimes;

	if( !_Init_Read_Cobj_File( &_crws, szName_ ) )
		goto _BAD;

	if( !_ReadRealTable(szName_, &_crws) ) goto _BAD;
	if( !_ReadTintTable(szName_, &_crws) ) goto _BAD;
	if( !_ReadBoolTable(szName_, &_crws) ) goto _BAD;
	if( !_ReadModuTable(szName_, &_crws) ) goto _BAD;
	if( !_ReadFaceTable(szName_, &_crws) ) goto _BAD;

	_Cobj_File_AfterRead( &_crws, szName_ );

	return TRUE;

_BAD:
	_lclose(_hFile);

	//free the core
	FreeCore();
	return FALSE;
}

//...................................................................
//
//Note:
//	The enough memory for RealTable must have been allocated, but
//	the table must be empty;
//	This function should be used with care. It should be called
//	by ReadCore function.
//...................................................................
_DLLEXP BOOL _DLLFUNC _ReadRealTable( LPCSTR szName_, LPCRWS lpCrws_ )
{
	extern RTInfo	gRTInfo;
	RTInfo			_RTInfo;
	char 			_szBuffer[_MAX_PATH];
	OFSTRUCT		_of;
	HFILE			_hFile;
	WRD				_i;
	LPRTU 			_lpRTU;

	sprintf(_szBuffer, "%s.rtu", szName_);
	_hFile = OpenFile(_szBuffer, &_of, OF_SHARE_EXCLUSIVE|OF_READ);
	if(_hFile == -1) return FALSE;

	if( !_SRT_Check(_hFile, &_RTInfo) )
		return FALSE;

	//here, suppose the ReadTable has been allocated and its size is >= _RTInfo.wCount
	if( gRTInfo.wCount < _RTInfo.wAdding ) goto _BAD;

	for(_i=0, _lpRTU = gRTInfo.lpBase;
		_i<_RTInfo.wAdding;
		_i++, _lpRTU++)
	{
		RTU				_rtusv;

		//read the RTU from file
		if( _hread( _hFile, &_rtusv, sizeof(RTU) )
		   != sizeof(RTU)	) goto _BAD;

		//delete the current RTU 
		if( _lpRTU->Status & RTUBIT_VALID )
		{
			CobjFree ( _lpRTU->lpTag );
			CobjFree ( _lpRTU->lpUnit );
			CobjFree ( _lpRTU->lpComment );
			_lpRTU->lpTag = 0;
			_lpRTU->lpUnit = 0;
			_lpRTU->lpComment = 0;
		}

		memcpy( _lpRTU, &(_rtusv), sizeof(RTU) );
		_lpRTU->lpTag = _Read_Cobj( lpCrws_, (LONG)_rtusv.lpTag );
		_lpRTU->lpUnit = _Read_Cobj( lpCrws_, (LONG)_rtusv.lpUnit );
		_lpRTU->lpComment = _Read_Cobj( lpCrws_, (LONG)_rtusv.lpComment );
	}

	gRTInfo.wAdding = _RTInfo.wAdding;
	_lclose(_hFile);
	return TRUE;

_BAD:
	_lclose(_hFile);
	return FALSE;
}

//...................................................................
//
//Note:
//	The enough memory for TintTable must have been allocated, but
//	the table must be empty;
//	This function should be used with care. It should be called
//	by ReadCore function.
//...................................................................
_DLLEXP BOOL _DLLFUNC _ReadTintTable( LPCSTR szName_, LPCRWS lpCrws_ )
{
	extern TTInfo	gTTInfo;
	TTInfo			_TTInfo;
	char 			_szBuffer[_MAX_PATH];
	OFSTRUCT		_of;
	HFILE			_hFile;
	WRD				_i;
	LPTTU 			_lpTTU;

	if( !gCoreInfo.lVersion )
		return FALSE;				//core must be opened first

	sprintf(_szBuffer, "%s.ttu", szName_);
	_hFile = OpenFile(_szBuffer, &_of, OF_SHARE_EXCLUSIVE|OF_READ);

	if(_hFile == -1) return FALSE;

	if( !_STT_Check(_hFile, &_TTInfo) )
		return FALSE;

	//here, suppose the TintTable has been allocated and its size is >= _TTInfo.wCount
	if(gTTInfo.wCount < _TTInfo.wAdding ) goto _BAD;

	for(_i=0, _lpTTU = gTTInfo.lpBase;
		_i<_TTInfo.wAdding;
		_i++, _lpTTU++)
	{
		TTU				_ttusv;

		//read the TTU from file
		if( _hread( _hFile, &_ttusv, sizeof(TTU) )
		   != sizeof(TTU)	) goto _BAD;

		//delete the current TTU 
		if( _lpTTU->Status & TTUBIT_VALID )
		{
			CobjFree ( _lpTTU->lpTag );
			CobjFree ( _lpTTU->lpUnit );
			CobjFree ( _lpTTU->lpComment );
			_lpTTU->lpTag = 0;
			_lpTTU->lpUnit = 0;
			_lpTTU->lpComment = 0;
		}

		memcpy( _lpTTU, &(_ttusv), sizeof(TTU) );
		_lpTTU->lpTag = _Read_Cobj( lpCrws_, (LONG)_ttusv.lpTag );
		_lpTTU->lpUnit = _Read_Cobj( lpCrws_, (LONG)_ttusv.lpUnit );
		_lpTTU->lpComment = _Read_Cobj( lpCrws_, (LONG)_ttusv.lpComment );
	}

	gTTInfo.wAdding = _TTInfo.wAdding;
	_lclose(_hFile);
	return TRUE;

_BAD:
	_lclose(_hFile);
	return FALSE;
}

//...................................................................
//
//Note:
//	The enough memory for BoolTable must have been allocated, but
//	the table must be empty;
//	This function should be used with care. It should be called
//	by ReadCore function.
//...................................................................
_DLLEXP BOOL _DLLFUNC _ReadBoolTable( LPCSTR szName_, LPCRWS lpCrws_ )
{
	extern BTInfo	gBTInfo;
	BTInfo			_BTInfo;
	char 			_szBuffer[_MAX_PATH];
	OFSTRUCT		_of;
	HFILE			_hFile;
	WRD				_i;
	LPBTU 			_lpBTU;

	if( !gCoreInfo.lVersion )
		return FALSE;				//core must be opened first

	sprintf(_szBuffer, "%s.btu", szName_);
	_hFile = OpenFile(_szBuffer, &_of, OF_SHARE_EXCLUSIVE|OF_READ);

	if(_hFile == -1) return FALSE;

	if( !_SBT_Check(_hFile, &_BTInfo) )
		return FALSE;

	//here, suppose the BoolTable has been allocated and its size is >= _BTInfo.wCount
	if(gBTInfo.wCount < _BTInfo.wAdding ) goto _BAD;

	for(_i=0, _lpBTU = gBTInfo.lpBase;
		_i<_BTInfo.wAdding;
		_i++, _lpBTU++)
	{
		BTU				_btusv;

		//read the BTU from file
		if( _hread( _hFile, &_btusv, sizeof(BTU) )
		   != sizeof(BTU)	) goto _BAD;

		//delete the current BTU 
		if( _lpBTU->Status & BTUBIT_VALID )
		{
			CobjFree ( _lpBTU->lpTag );
			CobjFree ( _lpBTU->lpUnit );
			CobjFree ( _lpBTU->lpComment );
			_lpBTU->lpTag = 0;
			_lpBTU->lpUnit = 0;
			_lpBTU->lpComment = 0;
		}

		memcpy( _lpBTU, &(_btusv), sizeof(BTU) );
		_lpBTU->lpTag = _Read_Cobj( lpCrws_, (LONG)_btusv.lpTag );
		_lpBTU->lpUnit = _Read_Cobj( lpCrws_, (LONG)_btusv.lpUnit );
		_lpBTU->lpComment = _Read_Cobj( lpCrws_, (LONG)_btusv.lpComment );
	}

	gBTInfo.wAdding = _BTInfo.wAdding;
	_lclose(_hFile);
	return TRUE;

_BAD:
	_lclose(_hFile);
	return FALSE;
}


//...................................................................
//
//Note:
//	The enough memory for ModuTable must have been allocated, but
//	the table must be empty;
//	This function should be used with care. It should be called
//	by ReadCore function.
//...................................................................
int _CheckMtu( WRD wrd_ );

_DLLEXP BOOL _DLLFUNC _ReadModuTable( LPCSTR szName_, LPCRWS lpCrws_ )
{
	extern MTInfo	gMTInfo;
	char 			_szBuffer[_MAX_PATH];
	OFSTRUCT		_of;
	HFILE			_htu, _hpu;
	WRD				_i;
	MTInfo			_MTInfo;
	LPMTU 			_lpMTU;

	if( !gCoreInfo.lVersion )
		return FALSE;				//no core

	sprintf(_szBuffer, "%s.mtu", szName_);
	_htu = OpenFile(_szBuffer, &_of, OF_READ);
	if(_htu == -1) return FALSE;

	sprintf(_szBuffer, "%s.mpu", szName_);
	_hpu = OpenFile(_szBuffer, &_of, OF_READ);
	if(_hpu == -1)
	{
		_lclose(_htu);
		return FALSE;
	}

	if( !_SMT_Check(_htu, &_MTInfo) ) goto _BAD;
	if( !_SMT_Check(_hpu, NULL) ) goto _BAD;

	//after check, the read pointers to the files xxxx.MTU
	//is set correctly.
	for(_i=0, _lpMTU = gMTInfo.lpBase;
		_i < _MTInfo.wAdding;
		_i++, _lpMTU++)
	{
		MTU			_mtusv;

		if( _lread(_htu,  &_mtusv, sizeof(MTU) )
			!= sizeof(MTU) ) goto _BAD;

		//delete the current MTU tag and comment
		if( _lpMTU->Status & MTUBIT_VALID )
		{
			CobjFree ( _lpMTU->lpTag );
			CobjFree ( _lpMTU->lpComment );
			_lpMTU->lpTag = NULL;
			_lpMTU->lpComment = NULL;
		}

		memcpy( _lpMTU, &(_mtusv), sizeof(MTU) );
		_lpMTU->lpTag = _Read_Cobj( lpCrws_, (LONG)_mtusv.lpTag );

		if(  ( _lpMTU->Status & MTUBIT_VALID )
		   && _lpMTU->lpParam )
		{
			WORD	_j;
			LPPU	_lppu;

			CORE_ASSERT( _lpMTU->NumParam );

			if( _llseek(_hpu, (long)_lpMTU->lpParam, 0) == HFILE_ERROR )
				goto _BAD;

			_lpMTU->lpParam = ParamAlloc( _lpMTU->NumParam );
			if(!_lpMTU->lpParam) goto _BAD;

			for(_j=0, _lppu = _lpMTU->lpParam;
				_j<_lpMTU->NumParam;
				_j++, _lppu++)
			{
				if( _lread(_hpu, _lppu, sizeof(PU) ) != sizeof(PU) )
					goto _BAD;

				if( (_lppu->prefix & (PF_MASK|IMM_MASK)) == (PF_IMM|IMM_LPCOBJ) )
				{
					_lppu->du.zzk__LPCOBJ = _Read_Cobj( lpCrws_, (LONG)_lppu->du.zzk__LPCOBJ );
				}
			}	//end for(...)
		}
		else	//invalid MTU or no parameters
		{
			CORE_ASSERT( (gMTInfo.lpBase)[_i].lpParam == NULL );
		}

		if( _CheckMtu( _i ) == -1 )
			goto _BAD;	

		if( _lpMTU->Status & MTUBIT_VALID )
			if( !AlgoCallParamProc( _lpMTU->Algo, PIC_AFTERLOAD, _i, 0, 0 ) )
				goto _BAD;
	}	//end for(...)

	gMTInfo.wAdding = _MTInfo.wAdding;

	_lclose(_htu);
	_lclose(_hpu);
	return TRUE;

_BAD:
	_lclose(_htu);
	_lclose(_hpu);
	return FALSE;
}

//...................................................................
//if error occurred, this MTU is deleted
//...................................................................
int _CheckMtu( WRD wrd_ )	
{
	extern MTInfo	gMTInfo;

	char	_szBuffer[128];
	LPMTU	_lpMTU = gMTInfo.lpBase + wrd_;
	WORD	_var, _default;
	BOOL	_bError = FALSE;
	
	_var = (WORD)AlgoCallParamProc( _lpMTU->Algo, PIC_CANAPPEND, wrd_, 0, 0L);
	_default = (WORD)AlgoCallParamProc( _lpMTU->Algo, PIC_GETNUMPARAM, 0, 0, 0L);

	if( _var )
	{
		div_t		_divt;
		_divt = div( _lpMTU->NumParam - _default, _var );
		if( _divt.rem )
			_bError = TRUE;
	}
	else
	{
		if( _default != _lpMTU->NumParam )
			_bError = TRUE;
	}

	if(_bError)
	{
		FreeParameter( _lpMTU->lpParam, _lpMTU->NumParam );

		_lpMTU->lpParam = ParamAlloc( _default );
		_lpMTU->NumParam = _default;
		if(_default && (_lpMTU->lpParam == NULL) )
			return -1;	//critical error

		if( !AlgoPICCreate( _lpMTU->Algo, wrd_, 0L) )
			return -1;	//critical error

		sprintf( _szBuffer, "MTU #%d: find error and fix", (int)wrd_ );
		MessageBox( NULL, _szBuffer, "Loading corebase", MB_OK|MB_ICONEXCLAMATION );
		return 0;	//error fixed
	}

	return 1;	//successful
}

//...................................................................
//
//Note:
//	The enough memory for FaceTable must have been allocated, but
//	the table must be empty;
//	This function should be used with care. It should be called
//	by ReadCore function.
//...................................................................
int _CheckFtu( WRD wrd_ );
_DLLEXP BOOL _DLLFUNC _ReadFaceTable( LPCSTR szName_, LPCRWS lpCrws_ )
{
	extern FTInfo	gFTInfo;
	char 			_szBuffer[_MAX_PATH];
	OFSTRUCT		_of;
	HFILE			_htu, _hpu;
	WRD				_i;
	FTInfo			_FTInfo;
	LPFTU 			_lpFTU;

	if( !gCoreInfo.lVersion )
		return FALSE;				//no core

	sprintf(_szBuffer, "%s.ftu", szName_);
	_htu = OpenFile(_szBuffer, &_of, OF_READ);
	if(_htu == -1) return FALSE;

	sprintf(_szBuffer, "%s.fpu", szName_);
	_hpu = OpenFile(_szBuffer, &_of, OF_READ);
	if(_hpu == -1)
	{
		_lclose(_htu);
		return FALSE;
	}

	if( !_SFT_Check(_htu, &_FTInfo) ) goto _BAD;
	if( !_SFT_Check(_hpu, NULL) ) goto _BAD;

	//after check, the read pointers to the files xxxx.MTU
	//is set correctly.
	for(_i=0, _lpFTU = gFTInfo.lpBase;
		_i < _FTInfo.wAdding;
		_i++, _lpFTU++)
	{
		if( _lread(_htu,  _lpFTU, sizeof(FTU) )
			!= sizeof(FTU) ) goto _BAD;

		if(  ( _lpFTU->Status & FTUBIT_VALID )
		   && _lpFTU->lpParam )
		{
			WORD	_j;
			LPPU	_lppu;

			CORE_ASSERT( _lpFTU->NumParam );

			if( _llseek(_hpu, (long)_lpFTU->lpParam, 0) == HFILE_ERROR )
				goto _BAD;

			_lpFTU->lpParam = ParamAlloc( _lpFTU->NumParam );
			if(!_lpFTU->lpParam) goto _BAD;

			for(_j=0, _lppu = _lpFTU->lpParam;
				_j<_lpFTU->NumParam;
				_j++, _lppu++)
			{
				if( _lread(_hpu, _lppu, sizeof(PU) ) != sizeof(PU) )
					goto _BAD;

				if( (_lppu->prefix & (PF_MASK|IMM_MASK)) == (PF_IMM|IMM_LPCOBJ) )
				{
					_lppu->du.zzk__LPCOBJ = _Read_Cobj( lpCrws_, (LONG)_lppu->du.zzk__LPCOBJ );
				}
			}	//end for(...)
		}
		else	//invalid FTU or no parameters
		{
			CORE_ASSERT( (gFTInfo.lpBase)[_i].lpParam == NULL );
		}

		if( _CheckFtu( _i ) == -1 )
			goto _BAD;	

		if( _lpFTU->Status & FTUBIT_VALID )
			if( !StampCallParamProc( _lpFTU->Stamp, PIC_AFTERLOAD, _i, 0, 0 ) )
				goto _BAD;
	}	//end for(...)

	gFTInfo.wAdding = _FTInfo.wAdding;

	_lclose(_htu);
	_lclose(_hpu);
	return TRUE;

_BAD:
	_lclose(_htu);
	_lclose(_hpu);
	return FALSE;
}

//...................................................................
//if error occurred, this FTU is deleted
//...................................................................
int _CheckFtu( WRD wrd_ )	
{
	extern FTInfo	gFTInfo;

	char	_szBuffer[128];
	LPFTU	_lpFTU = gFTInfo.lpBase + wrd_;
	WORD	_var, _default;
	BOOL	_bError = FALSE;
	
	_var = (WORD)StampCallParamProc( _lpFTU->Stamp, PIC_CANAPPEND, wrd_, 0, 0L);
	_default = (WORD)StampCallParamProc( _lpFTU->Stamp, PIC_GETNUMPARAM, 0, 0, 0L);

	if( _var )
	{
		div_t		_divt;
		_divt = div( _lpFTU->NumParam - _default, _var );
		if( _divt.rem )
			_bError = TRUE;
	}
	else
	{
		if( _default != _lpFTU->NumParam )
			_bError = TRUE;
	}

	if(_bError)
	{
		FreeParameter( _lpFTU->lpParam, _lpFTU->NumParam );

		_lpFTU->lpParam = ParamAlloc( _default );
		_lpFTU->NumParam = _default;
		if(_default && (_lpFTU->lpParam == NULL) )
			return -1;	//critical error

		if( !StampPICCreate( _lpFTU->Stamp, wrd_, 0L) )
			return -1;	//critical error

		sprintf( _szBuffer, "FTU #%d: find error and fix", (int)wrd_ );
		MessageBox( NULL, _szBuffer, "Loading corebase", MB_OK|MB_ICONEXCLAMATION );
		return 0;	//error fixed
	}

	return 1;	//successful
}

BOOL _Init_Write_Cobj_File( LPCRWS lpCrws_, LPCSTR lpszFullName_ )
{
	OFSTRUCT		_of;
	char			_szBuffer[_MAX_PATH];
	DWORD			_sign = 'COBJ';

	//initial value
	lpCrws_->hfile = HFILE_ERROR;
	lpCrws_->posit_cobj = 0;	

	sprintf( _szBuffer, "%s.c~~", lpszFullName_ );
	lpCrws_->hfile = OpenFile( _szBuffer, &_of, OF_CREATE|OF_SHARE_EXCLUSIVE|OF_WRITE );
	if( lpCrws_->hfile == HFILE_ERROR )
		return FALSE; 

	//for avoid beginning position is zero, some head information
	//is written to this file

	if( _lwrite(lpCrws_->hfile, (LPCSTR)&_sign, sizeof(_sign) )
		!= sizeof(_sign) )
	{
		_close( lpCrws_->hfile ); 
		lpCrws_->hfile = HFILE_ERROR;
		return FALSE;
	}

	lpCrws_->posit_cobj += sizeof(_sign);	//initial value

	return TRUE; 
}

LONG _Save_Cobj( LPCRWS lpCrws_ , LPCOBJ lpCobj_ )
{
	DWORD	_len;
	IDCOBJ	_idCobj;
	LONG	_ret = lpCrws_->posit_cobj;

	if( !lpCobj_ )	//is also a valid entry
		return 0;	

	_len = CobjSize( lpCobj_ );
	_idCobj = CobjGetID( lpCobj_ );

	//save the size of object
	if( _lwrite(lpCrws_->hfile, (LPCSTR)&_len, sizeof(DWORD) )
		!= sizeof(DWORD) )
		return FALSE;

	//then save the id of object
	if( _lwrite(lpCrws_->hfile, (LPCSTR)&_idCobj, sizeof(_idCobj) )
		!= sizeof(_idCobj) )
		return FALSE;

	//save the body of object
	if( _hwrite( lpCrws_->hfile, GET_COBJ_DATA(lpCobj_), _len )
		!= (long)_len )
		return FALSE;

	lpCrws_->posit_cobj += ( sizeof(_len) + sizeof(_idCobj) + _len );
	
	return _ret;
}

BOOL _Cobj_File_AfterSave( LPCRWS lpCrws_, LPCSTR lpszFullName_ )
{
	char 				_szName[_MAX_PATH], _szTemp[_MAX_PATH];
	OFSTRUCT			_of;

	//close the file first
	_lclose( lpCrws_->hfile );

	//prepare the file names
	sprintf( _szTemp, "%s.c~~", lpszFullName_ );
	sprintf( _szName, "%s.cbj", lpszFullName_ );

	//delete the old file
	OpenFile(_szName, &_of, OF_DELETE);
	//and then rename the temp. file to normal file
	if( rename(_szTemp, _szName) )
		return FALSE;

	return TRUE;
}

BOOL _Init_Read_Cobj_File( LPCRWS lpCrws_, LPCSTR lpszFullName_ )
{
	OFSTRUCT		_of;
	char			_szBuffer[_MAX_PATH];
	DWORD			_sign;

	//initial value
	lpCrws_->hfile = HFILE_ERROR;
	lpCrws_->posit_cobj = 0;	

	sprintf( _szBuffer, "%s.cbj", lpszFullName_ );
	lpCrws_->hfile = OpenFile( _szBuffer, &_of, OF_SHARE_EXCLUSIVE|OF_READ );
	if( lpCrws_->hfile == HFILE_ERROR )
		return FALSE;

	//read the head information

	if( _lread( lpCrws_->hfile, &_sign, sizeof(_sign) )
		!= sizeof(_sign) )
	{
		_close( lpCrws_->hfile );
		lpCrws_->hfile = HFILE_ERROR;
		return FALSE;
	}

	if( _sign != 'COBJ' )
		return FALSE;

	lpCrws_->posit_cobj += sizeof(_sign);	//initial value

	return TRUE; 
}

LPCOBJ _Read_Cobj( LPCRWS lpCrws_, LONG pos_ )
{
	DWORD		_dwSize;
	IDCOBJ		_idCobj;

	if( !pos_ )			// is also a valid entry
		return NULL;

	if( _llseek(lpCrws_->hfile, pos_, 0) == HFILE_ERROR )
		return NULL;

	if( _lread( lpCrws_->hfile, &_dwSize, sizeof(DWORD) )
		!= sizeof(DWORD) ) return NULL;
	if( _lread( lpCrws_->hfile, &_idCobj, sizeof(_idCobj) )
		!= sizeof(_idCobj) ) return NULL;

	if( _dwSize )
	{
		LPCOBJ _lpCobj;

		_lpCobj = CobjAlloc( _idCobj, _dwSize );
		if( !_lpCobj ) 
			return NULL;

		if( _hread( lpCrws_->hfile, _lpCobj, _dwSize )
			!= (long)_dwSize ) 
		{
			//free the cobject allocated just now
			CobjFree( _lpCobj );	 
			return NULL;
		}

		return _lpCobj;
	}
	else
	{
		CORE_ASSERT( _dwSize );
		return NULL;
	}
}

BOOL _Cobj_File_AfterRead( LPCRWS lpCrws_, LPCSTR lpszFullName_ )
{
	_lclose( lpCrws_->hfile );
	lpCrws_->hfile = HFILE_ERROR;
	return TRUE;
}
