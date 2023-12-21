//===================================================================
//
//Zheng zhikai	96-2-5
//===================================================================

#ifndef __CRUN_H
#define __CRUN_H

_DLLEXP BOOL _ZZKDECL CoreIsRunTime(void);
_DLLEXP BOOL _ZZKDECL CoreIsPause(void);

_DLLEXP BOOL _ZZKDECL CoreInitiate(BOOL bSetTimer_);
_DLLEXP BOOL _ZZKDECL CoreStop(void);
_DLLEXP void _ZZKDECL CoreStep(void);

_DLLEXP BOOL _ZZKDECL CorePause(void);

_DLLEXP BOOL _ZZKDECL CoreIsBlink(void);

_DLLEXP UINT _ZZKDECL CoreGetStepTime(void);
_DLLEXP UINT _ZZKDECL CoreSetStepTime( UINT uStepTime_ );

_DLLEXP DWORD _ZZKDECL CoreGetTicks(void);

_DLLEXP DWORD _ZZKDECL CoreTestTick(DWORD _lSteps);
_DLLEXP void _ZZKDECL UpdateChildrenFaces( HWND hwnd_, WRD wFTU_, WORD wSHSize_ );
_DLLEXP void _ZZKDECL UpdateCoreWindow(void);

////////////////
//stack flags

//normal 
#define STACKFLAG_NORMAL		0x0000

//this is a thread root
#define STACKFLAG_THREADROOT	0x0001

//this is a tree root
#define STACKFLAG_ROOT			0x0002

//this is a loop node, for building loop stack frame
#define STACKFLAG_LOOP			0x0004

//this is a call node, i.e. caller, for building call stack frame
#define STACKFLAG_CALL			0x0008

_DLLEXP ACRET _ZZKDECL RunModuTree( int wStack_, WRD wMTU_, WORD flags_ );
_DLLEXP WRD  _ZZKDECL PopModuThread( int iStack_, WORD _FAR* lpFlags_ );
_DLLEXP BOOL _ZZKDECL PushModuThread( int iStack_, WRD wMTU_, WORD wFlags_ );
_DLLEXP ACRET _ZZKDECL InitiateModuThread( WRD wMTU_ );

_DLLEXP int	_ZZKDECL GetCurrentModuThread(void);
_DLLEXP void _ZZKDECL SetCurrentModuThread( int iStack_ );

_DLLEXP void _ZZKDECL RedrawFaceWindow(HWND hwnd_, HDC hdc_, BOOL bInternal_ );
_DLLEXP BOOL _ZZKDECL RedrawCoreFocus(void);
_DLLEXP void _ZZKDECL CoreGetFocusRect( WRD wrdFace_, LPRECT lpRect_ );
_DLLEXP BOOL _ZZKDECL MouseInCoreFocus( POINT pt_ );

#endif	//__CRUN_H


