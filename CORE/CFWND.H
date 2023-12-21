//===================================================================
//
//Zheng zhikai	97-1-26
//===================================================================

#ifndef __CFWND_H
#define __CFWND_H

//registered messages for notifying the host window
#define	STR_WMCORERUN		"STR_WMCORERUN"
#define	STR_WMCOREDIRTY		"STR_WMCOREDIRTY"
#define	STR_WMOPENFWND		"STR_WMOPENFWND"

_DLLEXP LPSTR _ZZKDECL GetFwndTitle( WRD wFTU_, LPSTR szTitle_, WORD wSize_ );

 //old name: BOOL _ZZKDECL FaceCloseWindow(WRD _wFTU);
_DLLEXP BOOL _ZZKDECL CloseFwnd(WRD _wFTU);

_DLLEXP BOOL _ZZKDECL IsFaceSizeZero(WRD wFace_);

 //old name: WRD _ZZKDECL FindWindowFace(HWND hwnd_);
_DLLEXP WRD _ZZKDECL GetFwndWRD(HWND hwnd_);

//old name: WRD _ZZKDECL FaceGetFirstWindow(void);
_DLLEXP WRD _ZZKDECL GetFirstFwndWRD(void);

//old name WRD _ZZKDECL GetFirstFaceInWindow(HWND hwnd_);
_DLLEXP WRD _ZZKDECL GetFirstFaceInFwnd(HWND hwnd_);

 //old name: HWND _ZZKDECL FaceGetWindow(WRD wFTU_);
_DLLEXP HWND _ZZKDECL GetFwndHWND(WRD wFTU_);

_DLLEXP void _ZZKDECL FwndSetFaceHostWindow( HWND hwnd_ );
_DLLEXP void _ZZKDECL FwndClearFaceHostWindow(void);

_DLLEXP BOOL _ZZKDECL IsNewFwndName( LPCSTR lpszName_ );
_DLLEXP WRD _ZZKDECL FindFwndByName( LPCSTR lpszName_ );

_DLLEXP HWND _ZZKDECL OpenFwnd(WRD wFTU_);

#endif	//__CFWND_H
