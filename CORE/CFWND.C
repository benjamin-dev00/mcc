//===================================================================
//
//Zheng zhikai	95-12-01
//===================================================================
#include "prehead.h"

#include "coreall.h"
#include "cfwnd.h"
#include "csysstmp.h"
#include "stmpdef.h"

extern FTInfo		gFTInfo;
static HWND 		shwndFaceHost = NULL;

//////////////////////////////////////////////////////////
//this message notify the host app to open FaceWindow
//
//WParam:	n/a
//LParam:	WRD of the Face
//Return:	HWND of the Face
//
static UINT	WM_OPENFWND;

//////////////////////////////////////////////////////////
//WParam:	Dirty type:		TBX_RT, TT, BT, MT, FT, AT, ST
//LParam:	n/a
//Return:	n/a
static UINT	WM_COREDIRTY;

//////////////////////////////////////////////////////////
//WParam:	0:	initiate 
//			1:	run
//			2:	stop
//
//LParam:	core ticks
//Return:	n/a
static UINT	WM_CORERUN;

//...................................................................
//Description:
//	the FTUs in Face Table is as the following:
//		BOOT:	_lpBase[1]
//	and its children is the HWND Face, and HWND faces's children
//	are the FTUs in these window
//		root
//		+---Window1
//		|   +---FTU1.1
//		|	+---FTU1.2
//		+---Window2
//		|	+---FTU2.1
//		... ...
//...................................................................
_DLLEXP WRD _DLLFUNC GetFirstFaceInFwnd(HWND hwnd_)
{
	WRD	_i = GetFwndWRD(hwnd_);

	if(_i)
		return FaceGetChild(_i);
	else
		return NULL;
}

//...................................................................
//Description:
//	Finds FaceWindow for the specified hwnd_.
//Return:
//	the subscription for the FaceWindow
//...................................................................
_DLLEXP WRD _DLLFUNC GetFwndWRD(HWND hwnd_)
{
	LPPU 		_lpPrm;
	SimplePU	_spu;
	WRD			_i;

	for(_i = FaceGetChild(FACE_ROOT);
		_i;
		_i = FaceGetNext(_i) )
	{
		_lpPrm = FaceGetParameter(_i);

		_spu.prefix = PF_IMM|IMM_HANDLE;
		PURead(&_spu, &(_lpPrm[FACEWND_HWND]));
		if((HWND)_spu.du.zzk__HANDLE == hwnd_)
			return _i;
	}

	return 0;
}

//...................................................................
//Description:
//	Finds FaceWindow for the specified face.
//Return:
//	the subscription for the FaceWindow
//...................................................................
_DLLEXP HWND _DLLFUNC GetFwndHWND(WRD wFTU_)
{
	HWND		_hwnd;

	if( wFTU_ == FACE_ROOT )
		return NULL;
		
	while( FaceGetStamp(wFTU_) != IDS_FACEWND )
	{
		wFTU_ = FaceGetParent(wFTU_);
		if(!wFTU_)
		{
			CORE_ASSERT(0);
			return NULL;
		}
	}

	MFPUREAD( FaceGetParameter(wFTU_), FACEWND_HWND, HANDLE, _hwnd);
	return _hwnd;
}

//...................................................................
//...................................................................
_DLLEXP WRD _DLLFUNC FindFwndByName( LPCSTR lpszName_ )
{
	LPPU 		_lpPrm;
	LPCOBJ		_lpCobj;
	WRD			_i;

	for(_i = FaceGetChild(FACE_ROOT);
		_i;
		_i = FaceGetNext(_i) )
	{
		_lpPrm = FaceGetParameter(_i);

		MFPUREAD( _lpPrm, FACEWND_NAME, LPCOBJ, _lpCobj );

		if( GET_COBJ_DATA(_lpCobj) )
		{
			//compare without case sensitivity
			if( !strcmpi( lpszName_, (LPSTR)GET_COBJ_DATA(_lpCobj) ) )
				return _i;
		}
	}

	return 0;

}

//...................................................................
//...................................................................
_DLLEXP BOOL _DLLFUNC IsNewFwndName( LPCSTR lpszName_ )
{
	LPPU 		_lpPrm;
	LPCOBJ		_lpCobj;
	WRD			_i;

	for(_i = FaceGetChild(FACE_ROOT);
		_i;
		_i = FaceGetNext(_i) )
	{
		_lpPrm = FaceGetParameter(_i);

		MFPUREAD( _lpPrm, FACEWND_NAME, LPCOBJ, _lpCobj );

		if( GET_COBJ_DATA(_lpCobj) )
		{
			//compare without case sensitivity
			if( !strcmpi( lpszName_, (LPSTR)GET_COBJ_DATA(_lpCobj) ) )
				return FALSE;
		}
	}

	return TRUE;

}

//...................................................................
//...................................................................
_DLLEXP WRD _DLLFUNC GetFirstFwndWRD(void)
{
	return FaceGetChild(FACE_ROOT);
}

//...................................................................
//Description:
//...................................................................
_DLLEXP BOOL _DLLFUNC IsFaceSizeZero(WRD wFTU_)
{
	RECT	_rect;
	HWND	_hwnd;

	_hwnd = GetFwndHWND(wFTU_);
	FaceXycxcyToRect(wFTU_, &_rect);

	if( FaceGetStamp(wFTU_) == IDS_LINE )
	{
		return IsRectEmpty(&_rect);
	}
	else
	{
		if( !(_rect.left - _rect.right) && 
			!(_rect.bottom - _rect.top) )
			return TRUE;
		else
			return FALSE;
	}
}

//...................................................................
//Description:
//...................................................................
_DLLEXP BOOL _DLLFUNC CloseFwnd(WRD wFTU_)
{
	HWND	_hwnd;
	LPPU	_lpPrm;
	
	if( FaceGetStamp(wFTU_) != IDS_FACEWND )
	{
		CORE_ASSERT( FALSE );
		return FALSE;
	}

	_lpPrm = FaceGetParameter(wFTU_);
	MFPUREAD( _lpPrm, FACEWND_HWND, HANDLE, _hwnd );

	if(_hwnd)
	{
		if( IsWindow(_hwnd) )
		{
			//don't use DestroyWindow(_hwnd), it will cause that
			//MDI can't manage the child windows correctly;
			PostMessage( GetParent(_hwnd), WM_CLOSE, 0, 0);
		}
		else
		{
			//error here
			LOGWARN1("invalid HWND in Face: %d", wFTU_ );
			//correct it
			MFPUWRITE( _lpPrm, FACEWND_HWND, HANDLE, 0 );
		}
	}

	return TRUE;
}

//...................................................................
//Description:
//...................................................................
_DLLEXP HWND _DLLFUNC OpenFwnd(WRD wFTU_)
{
	return (HWND)SendMessage( shwndFaceHost, WM_OPENFWND, 0, (LPARAM)wFTU_ );
}

//...................................................................
//...................................................................
_DLLEXP LPSTR _DLLFUNC GetFwndTitle( WRD wFTU_, LPSTR szTitle_, WORD wSize_ )
{
	if( FaceGetStamp(wFTU_) != IDS_FACEWND )
		return FALSE;

	CORE_ASSERT(szTitle_);

	if( szTitle_ )
	{
		LPPU	_lpPrm;
		LPCOBJ	_lpCobj;

		_lpPrm = FaceGetParameter( wFTU_ );
		MFPUREAD( _lpPrm, FACEWND_NAME, LPCOBJ, _lpCobj );
		StringCopyn( szTitle_, (LPSTR)GET_COBJ_DATA(_lpCobj), wSize_ );
		return szTitle_;
	}
	else
		return NULL;
}

//...................................................................
//...................................................................
_DLLEXP void _DLLFUNC FwndSetFaceHostWindow( HWND hwnd_ )
{
	CORE_ASSERT( !shwndFaceHost );
	shwndFaceHost = hwnd_;
	return;
}

//...................................................................
//...................................................................
_DLLEXP void _DLLFUNC FwndClearFaceHostWindow()
{
	CORE_ASSERT( shwndFaceHost );
	shwndFaceHost = NULL;
	return;
}

_DLLEXP LRESULT _DLLFUNC DispatchCoreMessage( WPARAM wParam_ )
{
	if( shwndFaceHost )
		PostMessage( shwndFaceHost, WM_COREDIRTY, wParam_, 0L );

	return 0L;
}

_DLLEXP LRESULT _DLLFUNC DispatchCoreRunMessage( WPARAM wParam_, LPARAM lParam_ )
{
	if( shwndFaceHost )
		PostMessage( shwndFaceHost, WM_CORERUN, wParam_, lParam_ );

	return 0L;
}

void RegisterMessages()
{
	WM_OPENFWND = RegisterWindowMessage( STR_WMOPENFWND );
	WM_COREDIRTY = RegisterWindowMessage( STR_WMCOREDIRTY );
	WM_CORERUN = RegisterWindowMessage( STR_WMCORERUN );
}
