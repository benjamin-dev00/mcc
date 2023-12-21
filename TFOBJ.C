//===================================================================
//csysobj.c
//	System Objects
//Zheng zhikai	96-8-6
//===================================================================

#include "tfalgo.h"
#include "tfcmdmc.h"
#include "tfres.h"
#include "csysobj.h"


extern HINSTANCE	ghInst;
DWORD ObjectMVStruct(LPPU lpPU_, WORD wMode_, HWND hwnd_);
DWORD ObjectCVStruct(LPPU lpPU_, WORD wMode_, HWND hwnd_);
DWORD ObjectDVStruct(LPPU lpPU_, WORD wMode_, HWND hwnd_);
//extern OTInfo		gOTInfo;

//...................................................................
//...................................................................
ACRET _DLLFUNC RegisterTFObjects()
{
	static OTU _sOTUs[] = {
		{"Data Matrix", IDCOBJ_MATRIX, 		(void* )ObjectMatrix},
		{"Link Matrix", IDCOBJ_LINKMATRIX,	(void* )ObjectLinkMatrix},
		{"MV Struct", 	IDCOBJ_MVSTRUCT,	(void* )ObjectMVStruct},
		{"CV Struct", 	IDCOBJ_CVSTRUCT,	(void* )ObjectCVStruct},
		{"DV Struct", 	IDCOBJ_DVSTRUCT,	(void* )ObjectDVStruct},
	};

	int		_i;

	for(_i=0; _i < sizeof(_sOTUs)/sizeof(OTU); _i++)
		if(!RegisterCobj( &_sOTUs[_i]) )
			return FALSE;

	return TRUE;

}


//...................................................................
//...................................................................
#pragma argsused
DWORD ObjectMVStruct(LPPU lpPU_, WORD wMode_, HWND hwnd_)
{
	SimplePU		_spu;
	LPCOBJ			_lpCobj, _lpCobjEdit;
	BOOL			_bOk;
	int				_ret;
	HWND			_hwndDesktop;

	memset( &_spu, 0, sizeof(_spu) );
	_spu.prefix = PF_IMM|IMM_LPCOBJ;
	_spu.du.zzk__LPCOBJ = 0;
	if( !PURead(&_spu, lpPU_) )
		return FALSE;

	_lpCobj = _spu.du.zzk__LPCOBJ;
	if( !_lpCobj )
	{
		CORE_ASSERT( FALSE );
		return FALSE;
	}

	_lpCobjEdit = TFCobjCopy( _lpCobj, &_bOk );
	_hwndDesktop = GetDesktopWindow();
	if( !_bOk )
	{
		MessageBox( _hwndDesktop, "Core object copy error!", "Error", MB_OK );
		return FALSE;
	}

	if( !hwnd_ )
		return FALSE;

	_ret = DialogBoxParam( ghInst,
						   MAKEINTRESOURCE(IDD_DLGMATRIX),
						   hwnd_, 
						   (DLGPROC)DlgMVStructProc,
						   (LPARAM)(LPCOBJ FAR*)(&_lpCobj) );

	if( _ret == IDOK )
		CobjFree( _lpCobjEdit );
	else if( _ret == IDCANCEL )
	{
		memset( &_spu, 0, sizeof(_spu) );
		_spu.prefix = PF_IMM|IMM_LPCOBJ;
		_spu.du.zzk__LPCOBJ = _lpCobjEdit;
		if( !PUWrite( lpPU_, &_spu ) )
		{
			CobjFree( _lpCobjEdit );
			return FALSE;
		}
	}
	else
	{
		CORE_ASSERT( FALSE );
		return FALSE;
	}

	return TRUE;
}
//...................................................................
//...................................................................
#pragma argsused
DWORD ObjectCVStruct(LPPU lpPU_, WORD wMode_, HWND hwnd_)
{
	SimplePU		_spu;
	LPCOBJ			_lpCobj, _lpCobjEdit;
	BOOL			_bOk;
	int				_ret;
	HWND			_hwndDesktop;

	memset( &_spu, 0, sizeof(_spu) );
	_spu.prefix = PF_IMM|IMM_LPCOBJ;
	_spu.du.zzk__LPCOBJ = 0;
	if( !PURead(&_spu, lpPU_) )
		return FALSE;

	_lpCobj = _spu.du.zzk__LPCOBJ;
	if( !_lpCobj )
	{
		CORE_ASSERT( FALSE );
		return FALSE;
	}

	_lpCobjEdit = TFCobjCopy( _lpCobj, &_bOk );
	_hwndDesktop = GetDesktopWindow();
	if( !_bOk )
	{
		MessageBox( _hwndDesktop, "Core object copy error!", "Error", MB_OK );
		return FALSE;
	}

	if( !hwnd_ )
		return FALSE;

	_ret = DialogBoxParam( ghInst,
						   //	GetCurrentTask(), //ghInst,
						   MAKEINTRESOURCE(IDD_DLGMATRIX),
						   hwnd_,
						   (DLGPROC)DlgCVStructProc,
						   (LPARAM)(LPCOBJ FAR*)(&_lpCobj) );

	if( _ret == IDOK )
		CobjFree( _lpCobjEdit );
	else if( _ret == IDCANCEL )
	{
		memset( &_spu, 0, sizeof(_spu) );
		_spu.prefix = PF_IMM|IMM_LPCOBJ;
		_spu.du.zzk__LPCOBJ = _lpCobjEdit;
		if( !PUWrite( lpPU_, &_spu ) )
		{
			CobjFree( _lpCobjEdit );
			return FALSE;
		}
	}
	else
	{
		CORE_ASSERT( FALSE );
		return FALSE;
	}

	return TRUE;
}

//...................................................................
//...................................................................
#pragma argsused
DWORD ObjectDVStruct(LPPU lpPU_, WORD wMode_, HWND hwnd_)
{
	SimplePU		_spu;
	LPCOBJ			_lpCobj, _lpCobjEdit;
	BOOL			_bOk;
	int				_ret;
	HWND			_hwndDesktop;

	memset( &_spu, 0, sizeof(_spu) );
	_spu.prefix = PF_IMM|IMM_LPCOBJ;
	_spu.du.zzk__LPCOBJ = 0;
	if( !PURead(&_spu, lpPU_) )
		return FALSE;

	_lpCobj = _spu.du.zzk__LPCOBJ;
	if( !_lpCobj )
	{
		CORE_ASSERT( FALSE );
		return FALSE;
	}

	_lpCobjEdit = TFCobjCopy( _lpCobj, &_bOk );
	_hwndDesktop = GetDesktopWindow();
	if( !_bOk )
	{
		MessageBox( _hwndDesktop, "Core object copy error!", "Error", MB_OK );
		return FALSE;
	}
	if( !hwnd_ )
		return FALSE;

	_ret = DialogBoxParam( ghInst,
						   MAKEINTRESOURCE(IDD_DLGMATRIX),
						   hwnd_,
						   (DLGPROC)DlgDVStructProc,
						   (LPARAM)(LPCOBJ FAR*)(&_lpCobj) );

	if( _ret == IDOK )
		CobjFree( _lpCobjEdit );
	else if( _ret == IDCANCEL )
	{
		memset( &_spu, 0, sizeof(_spu) );
		_spu.prefix = PF_IMM|IMM_LPCOBJ;
		_spu.du.zzk__LPCOBJ = _lpCobjEdit;
		if( !PUWrite( lpPU_, &_spu ) )
		{
			CobjFree( _lpCobjEdit );
			return FALSE;
		}
	}
	else
	{
		CORE_ASSERT( FALSE );
		return FALSE;
	}

	return TRUE;
}

void _DLLFUNC CobjKeepCopy( LPCOBJ lpCobjD_, LPCOBJ lpCobjS_ )
{
	DWORD 	_dwSize;

	_dwSize = CobjSize( lpCobjS_ );
	memcpy( lpCobjD_, lpCobjS_, (size_t)_dwSize );
	return;
}
LPCOBJ TFCobjCopy( LPCOBJ lpCobjS_, BOOL FAR* lpbOk_ )
{
#ifdef __ONSPEC
	LPCOBJBLK	_lp;
	WORD		_SaveID;
	LPCOBJ		_lpCobjD;

	_lp = LPTO_COBJBLK(lpCobjS_);
	_SaveID = _lp->id;
	_lp->id = IDCOBJ_STRING;
	_lpCobjD = CobjCopy( lpCobjS_, lpbOk_ );
	_lp->id = _SaveID;
	_lp = LPTO_COBJBLK( _lpCobjD );
	_lp->id = _SaveID;
	return _lpCobjD;
#endif
#ifdef __ZZK
	return CobjCopy( lpCobjS_, lpbOk_ );
#endif
}