//===================================================================
//crun.c
//Zheng zhikai	96-2-5
//===================================================================

#include "prehead.h"

#include "coreall.h"
#include "cdbi.h"

//#define		_MY_DEBUG
#define		_USE_MEM_DC

/*
Note:
In my code, the "tree" and "branch" in iterator function
are two different concepts;

Tree:
		The data struct that has a root node and children nodes;
		The name of this kind of iterator is
		xxxTree( node_, ... )

Branch:
		The branch is a set of tree, on the other hand, it is a part of
	tree;
		The name of this kind of iterator is
		xxxBranch( node_, ... )
	here, node_ is the first node, which has the next nodes, all of this
	branch, include node_ self, will be iterated in body;
*/

#define	FOCUS_BORDER_X	(3)
#define	FOCUS_BORDER_Y	(3)

struct	_CoreFocus
{
	HWND	oldHwnd;		//At the last time, the window owned the focus
	WRD		oldFace;		//At the last time, the face owned the focus
	RECT	oldRect;		//At the last time, focus position
	BOOL	oldVisible;		//At the last time, if the face owned the focus is visible

	HWND	newHwnd;		//At this time, ...
	WRD		newFace;
	RECT	newRect;
	BOOL	newVisible;

	BOOL	bDirty;			//Whether the focus changed
	int		innerCounter;	//Whenever the focus changed between the different 
							// window, this ++, whenever the window once owned
							// the focus at the last time or now has updated,
							// this --, wheneven all windows update their focus 
							// states, this is forced to be reset.
}sCoreFocus;

static BOOL _near _UpdateCoreFocus(void);
static void _near _ClearCoreFocusDirty(void);

#define _STACK_DEPTH			64
#define _STACK_MAX				64

int GetFreeStack(void);
BOOL EndModuThread( int iStack_ );
BOOL ReturnModuThread( int iStack_ );
BOOL BreakModuThread( int iStack_ );
void ClearAllStacks(void);

void CoreStep_FirstTime(void);
void CoreStep_Continue(void);

typedef struct tagMODU_STACK_UNIT{
	WRD		wModu;
	WORD	wFlags;
}MODUSTACKUNIT;

typedef struct tagMODU_STACK{
	int				iStackPoint; 	//stack point( adding point of stack)
	MODUSTACKUNIT	msu[_STACK_DEPTH];
}MODUSTACK;

//#define LogCore(d)	((void)0)

void CALLBACK TimerProc(HWND hwnd_, UINT msg_, UINT idTimer_, DWORD dwTime_);

static void _near _CoreStep(void);
static void _near _BT_Update_FaceTree(HWND hwnd_, WRD wFTU_, WORD dirty_ );
static void _near _GetInvalidRgn_FaceTree(HWND hwnd_, WRD wFTU_, WORD wDirty_, LPGIR lpGir_ );
static void _near _RedrawAllFaceWindows();
static void _near _Redraw_FaceTree( WRD wrd_, HDC hdc_, HRGN hrgnDirty_ );
static WORD _near _Get_Fwnd_Dirty( WRD wFTU_ );

static BOOL _near _Initiate_ModuBranch(WRD wModu_);
static BOOL _near _Initiate_FaceBranch(WRD wFace_);
static BOOL _near _Stop_ModuBranch(WRD wModu_);
static BOOL _near _Stop_FaceBranch(WRD wFace_);

//modu stack pool
static MODUSTACK	sStackArray[_STACK_MAX];
static int			sCurStack;

extern WORD			gwSHSize;

extern MTInfo		gMTInfo;
extern FTInfo		gFTInfo;

//the options for core
extern CoreInfo		gCoreInfo;

//...................................................................
//Description:
//...................................................................
_DLLEXP BOOL _DLLFUNC CoreIsRunTime()
{
	return gCoreInfo.bRunTime;
}

//...................................................................
//...................................................................
_DLLEXP UINT _DLLFUNC CoreGetStepTime()
{
	return gCoreInfo.uInterval;
}

//...................................................................
//...................................................................
_DLLEXP DWORD _DLLFUNC CoreGetTicks()
{
	return gCoreInfo.tick;
}

//...................................................................
//...................................................................
_DLLEXP BOOL _DLLFUNC CoreIsPause()
{
	if( (gCoreInfo.idTimer == FALSE) && gCoreInfo.bRunTime )
		return TRUE;
	else
		return FALSE;
}

//...................................................................
//...................................................................
_DLLEXP BOOL _DLLFUNC CoreIsBlink()
{
	if( (!gCoreInfo.bBlink) && gCoreInfo.bRunTime )
		return FALSE;
	else
		return TRUE;
}

//...................................................................
//Description:
//	Set run-time timer step. if in run-time new, timer is restart
//	to use the new timer step.
//Return:
//	returns the old timer step if successful. If restart timer fails,
//	returns 0.
//Note:
//	if to restart the timer failed, the Core is stop.
//...................................................................
_DLLEXP UINT _DLLFUNC CoreSetStepTime(UINT uStepTime_ )
{
	UINT	_uTimeOut = gCoreInfo.uInterval;

	gCoreInfo.uInterval = uStepTime_;

	if(IS_RUN_MODE())
	{
		KillTimer(NULL, gCoreInfo.idTimer);
		gCoreInfo.idTimer = NULL;

		gCoreInfo.idTimer = SetTimer(NULL, 0, gCoreInfo.uInterval, TimerProc);
		if(!gCoreInfo.idTimer)
		{
			CoreStop();
			return FALSE;
		}
	}

	return _uTimeOut;
}

//...................................................................
//Description:
//	Initiate to run-time mode
//...................................................................
_DLLEXP BOOL _DLLFUNC CoreInitiate(BOOL bSetTimer_)
{
	WRD		_i;
	HWND	_hwnd;
	LPPU	_lppu;
	
	if( !IsCorebaseValid() )
		return FALSE;

	MEMCLR( &sCoreFocus, sizeof(sCoreFocus) );

	if( !_Initiate_ModuBranch(MODU_ROOT) )
		goto _ERROR;

	if( !_Initiate_FaceBranch(FACE_ROOT) )
		goto _ERROR;

	//set gbruntime before updatewindow because the WM_PAINT
	//depends on the gbRunTime;
	gCoreInfo.bRunTime = TRUE;
	gCoreInfo.tick = 0;

	//flush face windows at the first
	for(_i = FaceGetChild(FACE_ROOT);
		_i;
		_i = FaceGetNext(_i) )
	{
		CORE_ASSERT( FaceGetStamp(_i) == IDS_FACEWND );

		_lppu = FaceGetParameter(_i);
		MFPUREAD( _lppu, FACEWND_HWND, HANDLE, _hwnd );

		if( (!_hwnd) || (!IsWindowVisible(_hwnd)) )
			continue;

		InvalidateRect(_hwnd, NULL, TRUE);
		UpdateWindow(_hwnd);
	}

	//log history at initial state
	if( IS_CO_LOGHISTORY() )
	{
		char	_szBuffer[_MAX_PATH];

		strcpy( _szBuffer, gCoreInfo.szCoreFullName );

		//initialize history database
		if( !InitialDBI( _szBuffer ) )
			goto _ERROR;

		LogCore( 0 );
	}

	if(bSetTimer_)
	{
		gCoreInfo.idTimer = SetTimer(NULL, 0, gCoreInfo.uInterval*10/11, TimerProc);
		if(!gCoreInfo.idTimer)
			goto _ERROR;
	}

	//clear all modu stacks
    ClearAllStacks();
	return TRUE;

_ERROR:
	CoreStop();
	return FALSE;
}

//...................................................................
//Description:
//	stop run-time mode
//...................................................................
_DLLEXP BOOL _DLLFUNC CoreStop()
{
	WRD		_i;
	HWND	_hwnd;
	LPPU	_lppu;

	//clear all modu stacks
	ClearAllStacks();

	if( gCoreInfo.idTimer )
	{
		KillTimer(NULL, gCoreInfo.idTimer);
		gCoreInfo.idTimer = NULL;
	}

	//log history at end state
	if( IS_CO_LOGHISTORY() )
	{
		LogCore( -1 );
		FreeDBI();		//free interface to history database
	}

	_Stop_ModuBranch(MODU_ROOT);
	_Stop_FaceBranch(FACE_ROOT);

	gCoreInfo.bRunTime = FALSE;

	//flush face windows at the first
	for(_i = FaceGetChild(FACE_ROOT);
		_i;
		_i = FaceGetNext(_i) )
	{
		CORE_ASSERT( FaceGetStamp(_i) == IDS_FACEWND );

		_lppu = FaceGetParameter(_i);
		MFPUREAD( _lppu, FACEWND_HWND, HANDLE, _hwnd );

		if( (!_hwnd) || (!IsWindowVisible(_hwnd)) )
			continue;

		InvalidateRect(_hwnd, NULL, TRUE);
		UpdateWindow(_hwnd);
	}

	return TRUE;
}

//...................................................................
//Description:
//	step action of the Core at the run-time mode
//...................................................................
_DLLEXP void _DLLFUNC CoreStep()
{
	_CoreStep();
	return;
}

//...................................................................
//Description:
//	Pause the Core at Run-time mode
//...................................................................
_DLLEXP BOOL _DLLFUNC CorePause()
{
	if( gCoreInfo.idTimer )
	{
		CORE_ASSERT(gCoreInfo.bRunTime);
		KillTimer(NULL, gCoreInfo.idTimer);
		gCoreInfo.idTimer = NULL;
		return TRUE;
	}
	else
		return FALSE;
}

//...................................................................
//Description:
//	test escaped time for the specified steps
//...................................................................
_DLLEXP DWORD _DLLFUNC CoreTestTick(DWORD lSteps_)
{
	DWORD	_ticks, _l;
	BOOL	_bOldLog;

	_bOldLog = IS_CO_LOGHISTORY();
	gCoreInfo.CoreOptions &= (~CO_LOGHISTORY);	//disable it 

	CoreInitiate( FALSE );	//don't initiate timer

	_ticks = GetTickCount();
	for(_l=0;
		_l<lSteps_;
		_l++)
		_CoreStep();
	_ticks = GetTickCount() - _ticks;

	CoreStop();

	//restore it
	CoreSetData( CGD_LOGHISTORY, _bOldLog );
	return _ticks;
}

//...................................................................
//Description:
//	CallBack function for the run-time timer
//...................................................................
#pragma argsused
void CALLBACK TimerProc(HWND hwnd_, UINT msg_, UINT idTimer_, DWORD dwTime_)
{
	_CoreStep();
	return;
}

//...................................................................
//...................................................................
static void _near _CoreStep()
{
	WRD		_wrd;
	LPFTU	_lpFTU;

#ifdef _DEBUG
	DWORD	_tick_start = GetTickCount();
#endif

	gCoreInfo.tick ++;
	DispatchCoreRunMessage( 1, gCoreInfo.tick );

	if( ++(gCoreInfo.uBlinkCounter) > (gCoreInfo.uBlinkUpper) )
	{
		gCoreInfo.uBlinkCounter = 0;		//set to initiale value
		gCoreInfo.bBlink = gCoreInfo.bBlink? FALSE : TRUE;
	}

	if( gCoreInfo.tick == 1 )
		CoreStep_FirstTime();
	else
		CoreStep_Continue();

	for(_wrd=1, _lpFTU = gFTInfo.lpBase + 1;
		_wrd < gFTInfo.wAdding;
		_wrd++, _lpFTU++)
	{
		if( (_lpFTU->Status & (FTUBIT_VALID|FTUBIT_WANTTICK)) == (FTUBIT_VALID|FTUBIT_WANTTICK) )
			StampCallBuildProc(SCMD_TICK, _wrd, NULL, 0, gCoreInfo.tick );
	}

	//update face table
	_RedrawAllFaceWindows();

	if( IS_CO_LOGHISTORY() )	//if enable log
	{
		LogCore( 1 );
	}

	////////////////////
	//at last, clear the dirty flags
	CoreClearDirty();

#ifdef _DEBUG
	//CORE_DEBUG2( "Step: %lu time:%lu \r\n", gCoreInfo.tick, GetTickCount() - _tick_start );
#endif

	return;
}

//...................................................................
//...................................................................
static void _near _RedrawAllFaceWindows()
{
	WRD		_i;
	HWND	_hwnd;
	LPPU	_lppu;
	//RECT	_rc;
	HDC		_hdc;

	_UpdateCoreFocus();

	for(_i = FaceGetChild(FACE_ROOT);
		_i;
		_i = FaceGetNext(_i) )
	{
		CORE_ASSERT( FaceGetStamp(_i) == IDS_FACEWND );

		_lppu = FaceGetParameter(_i);
		MFPUREAD( _lppu, FACEWND_HWND, HANDLE, _hwnd );

		if( (!_hwnd) || (!IsWindowVisible(_hwnd)) )
			continue;

		_hdc = GetDC( _hwnd );
		RedrawFaceWindow( _hwnd, _hdc, TRUE /* call this internal */ );
		ReleaseDC( _hwnd, _hdc );
		//RedrawWindow( _hwnd, NULL, NULL, RDW_INTERNALPAINT|RDW_NOERASE|RDW_UPDATENOW|RDW_NOCHILDREN );
	}

	//force clearing the CoreFocus struct
	_ClearCoreFocusDirty();
}

//...................................................................
//Description:
//	this function determines whether the window specified by entry
//	paramter is a ASC(Auto-Sizing Children) FaceWindow, if it
//	is, then determines if its size changed, if it is too, the
//	function returns POSITION_DIRTY, otherwise, returns NO_DIRTY;
//...................................................................
static WORD _near _Get_Fwnd_Dirty( WRD wFTU_ )
{
	WORD    _ret = NO_DIRTY;
	LPPU    _lppu;
	LPFTU   _lpFTU = gFTInfo.lpBase + wFTU_;
	WORD    _i;

	//ONLY test PUs
	_lppu = _lpFTU->lpParam;
	CORE_ASSERT( _lppu );

	for(_i=0;
		_i<_lpFTU->NumParam;
		_i++, _lppu++ )
	{
		if( PUIsDirty(_lppu) )
		{
			if( _i < FACE_FIRSTPARAM )
				_ret |= POSITION_DIRTY;

			if(  ( _i == FACEWND_CLIENTCX )
			   ||( _i == FACEWND_CLIENTCY )
			   ||( _i == FACEWND_WNDCX )
			   ||( _i == FACEWND_WNDCY ) )
			{
				if( FaceGetStatus( wFTU_ ) & FTUBIT_AUTOSIZECHILDREN )
					_ret |= POSITION_DIRTY;
			}

			if(   (_i == FACEWND_BACKCLR)
				||(_i == FACEWND_PICTURE)
				||(_i == FACEWND_DISPLAY) )
				_ret |= BACKCLR_DIRTY;
		}
	}//end for

	return _ret;
}

//...................................................................
//Description:
//Entries:
//	WORD dirty_
//		the dirty operation comes from parent node, normally, is NO_DIRTY
//		or POSTION_DIRTY;
//		If the parent postion/size changed, the children faces should
//		update their FTU.rect; the function uses this to notify them that
//		their parent'd position/size changed, they should re-calculate
//		their FTU.rect again;
//...................................................................
static void _near _BT_Update_FaceTree(HWND hwnd_, WRD wFTU_, WORD dirty_ )
{
	WORD	_dirty;

	for(wFTU_ = FaceGetChild(wFTU_);	// first face of the Parent FTU
		wFTU_;
		wFTU_ = FaceGetNext(wFTU_) )
	{
		//if there are any dirty parameters, prepare invalid rectange
		_dirty = FaceDirty(wFTU_)|dirty_;
		if( _dirty )
			StampCallBuildProc( SCMD_UPDATE, wFTU_, hwnd_, gwSHSize, _dirty );

		if( FaceGetChild(wFTU_) )
		{
			//if parent node is position dirty, force children position dirty
			_BT_Update_FaceTree( hwnd_, wFTU_, (WORD)(_dirty & POSITION_DIRTY) );
		}
	}

	return;
}

//...................................................................
//Description:
//Entries:
//	BOOL bDirty_,
//		as same as the parameter "dirty" in function _BT_Update_FaceTree,
//		this parameter notifies children nodes to update their FTU.rect
//		if their position changed;
//...................................................................
static void _near _GetInvalidRgn_FaceTree(HWND hwnd_, WRD wFTU_, WORD wDirty_, LPGIR lpgir_ )
{
	WORD	_dirty;

	for(wFTU_ = FaceGetChild(wFTU_);	// first face of the Parent FTU
		wFTU_;
		wFTU_ = FaceGetNext(wFTU_) )
	{
		//if there are any dirty parameters, prapare the invalid rectange
		_dirty = IsFaceRedraw(wFTU_)? NORMAL_DIRTY: 0;
		_dirty |= FaceDirty(wFTU_)|wDirty_;

		StampCallBuildProc(SCMD_GETINVALIDRGN, wFTU_, hwnd_, _dirty, (LONG)lpgir_ );

		if( FaceGetChild(wFTU_) )
		{
			//if the parent node position changed, the children's position must
			//be updated too;
			//so mask the others bits but POSITION_DIRTY and HEAD_DIRTY
			_dirty &= DEEP_DIRTY;
			_GetInvalidRgn_FaceTree(hwnd_, wFTU_, _dirty, lpgir_);
		}
	}

	return;
}

//...................................................................
//Description:
//	this function iterates wModu_ and all succedent modu and
//	their children;
//...................................................................
static BOOL _near _Initiate_ModuBranch(WRD wModu_)
{
	WRD		_i, _wChild;

	for(_i = wModu_;
		_i;
		_i = ModuGetNext(_i) )
	{
		if( !AlgoInitiate( ModuGetAlgo(_i), _i, gCoreInfo.uInterval) )
			return FALSE;

		_wChild = ModuGetChild(_i);
		if( _wChild )
			_Initiate_ModuBranch( _wChild );
	}

	return TRUE;
}

//...................................................................
//Description:
//	this function iterates wModu_ and all succedent modu and
//	their children;
//...................................................................
static BOOL _near _Initiate_FaceBranch(WRD wFace_)
{
	WRD		_i, _wChild;

	for(_i = wFace_;
		_i;
		_i = FaceGetNext(_i) )
	{
		if( !StampInitiate( FaceGetStamp(_i), _i, gCoreInfo.uInterval ) )
			return FALSE;

		_wChild = FaceGetChild(_i);
		if( _wChild )
			_Initiate_FaceBranch( _wChild );
	}

	return TRUE;
}

//...................................................................
//Description:
//	this function iterates wModu_ and all succedent modu and
//	their children;
//...................................................................
static BOOL _near _Stop_ModuBranch(WRD wModu_)
{
	WRD		_i, _wChild;

	for(_i = wModu_;
		_i;
		_i = ModuGetNext(_i) )
	{
		if( !AlgoStop( ModuGetAlgo(_i), _i) )
			return FALSE;

		_wChild = ModuGetChild(_i);
		if(_wChild)
			_Stop_ModuBranch(_wChild);
	}

	return TRUE;
}

//...................................................................
//Description:
//	this function iterates wModu_ and all succedent modu and
//	their children;
//...................................................................
static BOOL _near _Stop_FaceBranch(WRD wFace_)
{
	WRD		_i, _wChild;

	for(_i = wFace_;
		_i;
		_i = FaceGetNext(_i) )
	{
		if( !StampStop( FaceGetStamp(_i), _i) )
			return FALSE;

		_wChild = FaceGetChild(_i);
		if(_wChild)
			_Stop_FaceBranch(_wChild);
	}

	return TRUE;
}

//...................................................................
//...................................................................
_DLLEXP void _DLLFUNC UpdateCoreWindow()
{
	if( !CoreIsRunTime )	//if build-time
	{
		//save dirty flag to core dirty flag
		if( IsCoreDirty() )
			CoreSetDirty();
	}

	_RedrawAllFaceWindows();

	/*

	WRD		_i;
	LPPU	_lppu;
	HWND	_hwnd;

	//for each face window
	for(_i = FaceGetChild(FACE_ROOT);
		_i;
		_i = FaceGetNext(_i) )
	{
		CORE_ASSERT( FaceGetStamp(_i) == IDS_FACEWND );

		_lppu = FaceGetParameter(_i);
		MFPUREAD( _lppu, FACEWND_HWND, HANDLE, _hwnd );

		if( (!_hwnd) || (!IsWindowVisible(_hwnd)) )
			continue;

		if( IS_RUN_MODE() )
		{
			GIR		_gir;

			//initialize the struct _gir
			memset( &_gir, 0, sizeof(_gir) );
			_gir.girLucidDirtyRgn = CreateRectRgn(0,0,0,0);
			_gir.girOpaqueDirtyRgn = CreateRectRgn(0,0,0,0);

			_GetInvalidRgn_FaceTree( _hwnd, _i, NO_DIRTY, &_gir );
			_RT_RedrawFaceWindow(_hwnd, &_gir );

			DeleteObject( _gir.girOpaqueDirtyRgn );
			DeleteObject( _gir.girLucidDirtyRgn );
		}
		else
		{	//build time

			//in order to update the faces rectangles, whenever, iterate 
			//the whole of face tree is necessary 
			_BT_Update_FaceTree(_hwnd, _i, NO_DIRTY );

			if( FaceDirty(_i) )		//check this window
			{
				//the whole window need to be invalidated
				InvalidateRect( _hwnd, NULL, TRUE );
			}

			UpdateWindow(_hwnd);
		}
	}

	*/

	//last, clear the dirty flags
	CoreClearDirty();
	return;
}



//...................................................................
//...................................................................
static void _near _Prepare_RedrawFaceWindow(HWND hwnd_, LPGIR lpgir_ )
{
	WRD		_wrd;
	WORD	_dirty;

	memset( lpgir_, 0, sizeof(GIR) );

	_wrd = GetFwndWRD(hwnd_);
	_dirty = _Get_Fwnd_Dirty( _wrd );
	if( _dirty & BACKCLR_DIRTY )
	{
		RECT	_rc;

		GetClientRect( hwnd_, &_rc );
		lpgir_->girDirtyRgn = CreateRectRgnIndirect( &_rc );
	}
	else
		lpgir_->girDirtyRgn = CreateRectRgn(0,0,0,0);

	_GetInvalidRgn_FaceTree( hwnd_, _wrd, _dirty, lpgir_ );

	return;
}

//...................................................................
//Description:
//	This function checks whether the face
//
//Returns:
//...................................................................
_DLLEXP void _DLLFUNC CoreGetFocusRect( WRD wrdFace_, LPRECT lpRect_ )
{
	StampCallBuildProc(	SCMD_GETFOCUSRECT,
						wrdFace_,
						NULL,
						0,
						(LPARAM)lpRect_ );
	return;
}

//...................................................................
//Description:
//	This function checks whether the face
//
//Returns:
//...................................................................
_DLLEXP BOOL _DLLFUNC MouseInCoreFocus( POINT pt_ )
{
	return PtInRect( &(sCoreFocus.newRect), pt_ ) && IsFaceVisible( gCoreInfo.wCurFace );
}

//...................................................................
//Description:
//	This function checks whether the face
//
//Returns:
//...................................................................
_DLLEXP BOOL _DLLFUNC RedrawCoreFocus()
{
	RECT	_rc;

	if( _UpdateCoreFocus() )
	{
		if( sCoreFocus.oldHwnd )
		{
			CopyRect( &_rc, &(sCoreFocus.oldRect) );
			InflateRect( &_rc, 	FOCUS_BORDER_X, FOCUS_BORDER_Y );
			InvalidateRect( sCoreFocus.oldHwnd, &_rc, TRUE );
		}
		if( sCoreFocus.newHwnd )
		{
			CopyRect( &_rc, &(sCoreFocus.newRect) );
			InflateRect( &_rc, FOCUS_BORDER_X, FOCUS_BORDER_Y );
			InvalidateRect( sCoreFocus.newHwnd, &_rc, TRUE );
		}

		if( sCoreFocus.oldHwnd == sCoreFocus.newHwnd )
		{
			if( sCoreFocus.oldHwnd )	
				UpdateWindow( sCoreFocus.oldHwnd );
		}
		else
		{
			if( sCoreFocus.oldHwnd )	
				UpdateWindow( sCoreFocus.oldHwnd );
			if( sCoreFocus.newHwnd )
				UpdateWindow( sCoreFocus.newHwnd );
		}

		return TRUE;
	}
	else
		return FALSE;
}


//...................................................................
//...................................................................
static void _near _ClearCoreFocusDirty()
{
	CORE_ASSERT( !sCoreFocus.innerCounter );

	sCoreFocus.oldHwnd = sCoreFocus.newHwnd;
	sCoreFocus.oldFace = sCoreFocus.newFace;
	sCoreFocus.oldVisible = sCoreFocus.newVisible;
	CopyRect( &(sCoreFocus.oldRect), &(sCoreFocus.newRect) );

	sCoreFocus.bDirty = FALSE;
	sCoreFocus.innerCounter = 0;

	
	return;
}

//...................................................................
//...................................................................
static BOOL _near _UpdateCoreFocus()
{
	//set newFace
	sCoreFocus.newFace = gCoreInfo.wCurFace;

	if( !FaceIsValid(sCoreFocus.newFace) )
	{
		gCoreInfo.wCurFace = 0;
		sCoreFocus.newFace = 0;
	}

	//LOGMSG2( "Old: %d, New:%d ", (int)(sCoreFocus.oldFace), (int)(sCoreFocus.newFace) );

	//update the CoreFocus struct
	sCoreFocus.bDirty = FALSE;
	if( sCoreFocus.newFace )
	{
		sCoreFocus.newHwnd = GetFwndHWND( sCoreFocus.newFace );
		sCoreFocus.newVisible = IsFaceVisible(sCoreFocus.newFace);
		//CORE_ASSERT( sCoreFocus.newHwnd ); maybe the window closed
		CoreGetFocusRect( sCoreFocus.newFace, &(sCoreFocus.newRect) );
	}
	else
	{
		sCoreFocus.newHwnd = NULL;
		SetRectEmpty( &(sCoreFocus.newRect) );
	}

	if( sCoreFocus.newFace == sCoreFocus.oldFace )
	{
		if( !sCoreFocus.oldFace )
			return FALSE;	//no dirty

		if( sCoreFocus.oldHwnd == sCoreFocus.newHwnd )
		{
			if( !EqualRect( &(sCoreFocus.newRect), &(sCoreFocus.oldRect) ) )
				sCoreFocus.bDirty = TRUE;
		}
		else
			sCoreFocus.bDirty = TRUE;

		if( sCoreFocus.newVisible != sCoreFocus.oldVisible )
			sCoreFocus.bDirty = TRUE;
	}
	else 
		sCoreFocus.bDirty = TRUE;

	if( sCoreFocus.bDirty )
	{
		//the clear dirty counter
		sCoreFocus.innerCounter = 0;
		if( sCoreFocus.oldHwnd && IsWindow(sCoreFocus.oldHwnd) )
			sCoreFocus.innerCounter ++;
		if( sCoreFocus.newHwnd && IsWindow(sCoreFocus.newHwnd) )
			sCoreFocus.innerCounter ++;
	}

	//LOGMSG1( "sCoreFocus.bDirty = %d", (int)(sCoreFocus.bDirty) );
	return sCoreFocus.bDirty;
}

//...................................................................
//Returns
//	BOOL bInternal_		Reset it if this function is called 
//						by WM_PAINT routine
//						
//...................................................................
_DLLEXP void _DLLFUNC RedrawFaceWindow(HWND hwnd_, HDC hdc_, BOOL bInternal_ )
{
	GIR			_gir;

	HDC			_hdc, _hdcDraw;
	LPPU		_lpPU;
	COLORREF	_clrBk;
	HBRUSH		_hBr;
	WRD			_wrd;
	RECT		_rc;			//client rect 
	RECT		_rcClip;
	DWORD		_mode;			//picture mode
	LPCOBJ		_lpCobjPicture;

#ifdef _MY_DEBUG
	char		_szBuffer[32];
	HBRUSH		_hBrF, _hBrB;
#endif

#ifdef _USE_MEM_DC
	HDC			_hdcMem;
	HBITMAP		_hbmpMem, _hbmpMemOld;
#endif

	_Prepare_RedrawFaceWindow( hwnd_, &_gir );

	_hdc = hdc_;
	_wrd = GetFwndWRD(hwnd_);

	if( !CoreIsRunTime() )	
	{
		//Adjust dirty rgn at build-time: include dimensions of select-handles 
		RECT	_rcBox;

		GetRgnBox(_gir.girDirtyRgn, &_rcBox);
		InflateRect( &_rcBox, gwSHSize, gwSHSize );

		DeleteObject(_gir.girDirtyRgn);	//delete the old rgn
		_gir.girDirtyRgn = CreateRectRgnIndirect( &_rcBox );
	}
	
	//combine dirty core focus rectangle
	if( sCoreFocus.bDirty )
	{
		HRGN	_hrgnFocus;
		RECT	_rcTemp;

		if( sCoreFocus.oldHwnd == hwnd_ )	//then, try to erase the focus-box
		{
			CopyRect( &_rcTemp, &(sCoreFocus.oldRect) );
			InflateRect( &_rcTemp, FOCUS_BORDER_X, FOCUS_BORDER_Y );
			_hrgnFocus = CreateRectRgnIndirect( &_rcTemp );
			CombineRgn( _gir.girDirtyRgn, _hrgnFocus, _gir.girDirtyRgn, RGN_OR );
			DeleteObject( _hrgnFocus );
			--sCoreFocus.innerCounter;
		}

		if( sCoreFocus.newHwnd == hwnd_ )	//then, try to draw the focus-box
		{
			CopyRect( &_rcTemp, &(sCoreFocus.newRect) );
			InflateRect( &_rcTemp, FOCUS_BORDER_X, FOCUS_BORDER_Y );
			_hrgnFocus = CreateRectRgnIndirect( &_rcTemp );
			CombineRgn( _gir.girDirtyRgn, _hrgnFocus, _gir.girDirtyRgn, RGN_OR );
			DeleteObject( _hrgnFocus );
			--sCoreFocus.innerCounter;
		}

		if( sCoreFocus.innerCounter <= 0 )
			_ClearCoreFocusDirty();
	}

	if( !bInternal_ )	//adjust girLucidDirtyRgn when the caller is WM_PAINT routine
	{	
		RECT	_rcHdcClipBox;
		HRGN	_hrgnHdc;

		GetClipBox( _hdc, &_rcHdcClipBox );
		_hrgnHdc = CreateRectRgnIndirect( &_rcHdcClipBox );
		CombineRgn( _gir.girDirtyRgn, _hrgnHdc, _gir.girDirtyRgn, RGN_OR );
		DeleteObject( _hrgnHdc );
	}

	//calculate the clip region and box of the HDC by dirty rgns
	GetRgnBox( _gir.girDirtyRgn, &_rcClip );
	GetClientRect( hwnd_, &_rc );
	IntersectRect( &_rcClip, &_rc, &_rcClip );

	//then select dirty rgn to HDC
	ExtSelectClipRgn( _hdc, _gir.girDirtyRgn, RGN_OR );

#ifdef _USE_MEM_DC
	_hdcMem = CreateCompatibleDC( _hdc );

	//Note:
	//the width and height is _rcClip.right and _rcClip.bottom
	//not (_rcClip.right - _rcClip.left) and (_rcClip.bottom - _rcClip.top)
	_hbmpMem = CreateCompatibleBitmap( _hdc, _rcClip.right, _rcClip.bottom );
	_hbmpMemOld = SelectObject( _hdcMem, _hbmpMem );

	BitBlt( _hdcMem,
			_rcClip.left, _rcClip.top,
			_rcClip.right - _rcClip.left, _rcClip.bottom - _rcClip.top,
			_hdc,
			_rcClip.left, _rcClip.top,
			SRCCOPY );

	_hdcDraw = _hdcMem;
#else
	_hdcDraw = _hdc;
#endif	//_USE_MEM_DC

	SetBkMode( _hdcDraw, TRANSPARENT );

	//erase background by the background color
	_lpPU = FaceGetParameter( _wrd );
	MFPUREAD( _lpPU, FACEWND_BACKCLR, BITS32, _clrBk );
	_hBr = CoreCreateBrush( _clrBk );
	FillRgn( _hdcDraw, _gir.girDirtyRgn, _hBr );
	CoreDeleteBrush(_hBr);

	//draw background picture
	MFPUREAD( _lpPU, FACEWND_PICTURE, LPCOBJ, _lpCobjPicture );
	MFPUREAD( _lpPU, FACEWND_DISPLAY, DWORD, _mode );

	if(  ( _lpCobjPicture )
	   &&( GET_COBJ_DATA(_lpCobjPicture) ) )
	{
		char	_szError[128];

		DrawPicture( (LPCSTR)GET_COBJ_DATA(_lpCobjPicture), 
					  &_rc, 
					  _szError, 
					  DPFLAG_REPORTERROR,
					  _mode, 
					  _hdcDraw );
	}

#ifdef _MY_DEBUG
	//debug code: display
	//		1. 	gCoreInfo.tick
	//		2.	dirty region
	ultoa( gCoreInfo.tick, _szBuffer, 16 );
	TextOut( _hdcDraw, 0, 0, _szBuffer, strlen(_szBuffer) );
	_hBrB = CreateHatchBrush( HS_BDIAGONAL, 0 );
	FillRgn( _hdcDraw, _gir.girDirtyRgn, _hBrB );
	DeleteObject( _hBrB );
	//end debug
#endif

	_Redraw_FaceTree( _wrd, _hdcDraw, _gir.girDirtyRgn );

	//need not draw faces select handles at run-time
	if( !CoreIsRunTime() )
	{
		WRD				_i;
		LPFTU			_lpFTU;
		FTInfo FAR* 	_lpFTInfo = GetFaceTableInformation();

		//draw faces select handles
		for(_i = FaceGetChild( _wrd );
			_i;
			_i = _lpFTU->Next)
		{
			_lpFTU = _lpFTInfo->lpBase + _i;
			if(    (_lpFTU->Status & FTUBIT_SELECT) 
				&& RectInRegion(_gir.girDirtyRgn, &(_lpFTU->Rect) ) )
				StampCallBuildProc(	SCMD_TOGGLESELECTHANDLES,
									_i,	hwnd_, gwSHSize, (LONG)_hdcDraw );
		}
	}
	else
	{
		if( gCoreInfo.wCurFace && 
			(sCoreFocus.newHwnd == hwnd_) && 
			IsFaceVisible(gCoreInfo.wCurFace) )
		{
			RECT		_rcCur;
			HGDIOBJ		_hbrush, _hpen;

			CoreGetFocusRect( gCoreInfo.wCurFace, &_rcCur );

			_hbrush = SelectObject( _hdcDraw, GetStockObject( NULL_BRUSH ) );
			_hpen = SelectObject( _hdcDraw, GetStockObject( BLACK_PEN ) );

			InflateRect( &_rcCur, 1, 1);
			Rectangle( _hdcDraw, _rcCur.left, _rcCur.top, _rcCur.right, _rcCur.bottom );
			SelectObject( _hdcDraw, GetStockObject( WHITE_PEN ) );
			InflateRect( &_rcCur, 1, 1);
			Rectangle( _hdcDraw, _rcCur.left, _rcCur.top, _rcCur.right, _rcCur.bottom );
			SelectObject( _hdcDraw, GetStockObject( BLACK_PEN ) );
			InflateRect( &_rcCur, 1, 1);
			Rectangle( _hdcDraw, _rcCur.left, _rcCur.top, _rcCur.right, _rcCur.bottom );

			SelectObject( _hdcDraw, _hbrush );
			SelectObject( _hdcDraw, _hpen );
		}
	}

#ifdef _USE_MEM_DC
	BitBlt( _hdc,
			_rcClip.left, _rcClip.top,
			_rcClip.right - _rcClip.left, _rcClip.bottom - _rcClip.top,
			_hdcMem,
			_rcClip.left, _rcClip.top,
			SRCCOPY );

	SelectObject( _hdcMem, _hbmpMemOld );
	DeleteObject( _hbmpMem );
	DeleteDC( _hdcMem );
#endif

	DeleteObject( _gir.girDirtyRgn );
	return;
}

//...................................................................
//Description:	
//	redraw a face-tree, the parameter wrd_ is specified the root of
//	face-tree; the root of the face-tree, i.e. wrd_ is not redrawn;
//Note:
//	This function is called by RedrawFaceWindow;
//...................................................................
static void _near _Redraw_FaceTree( WRD wrd_, HDC hdc_, HRGN hrgnDirty_ )
{
	WRD			_i;
	RECT		_rc;
	BOOL		_bVisible;
	LPPU		_lpPrm;
	
	//draw faces objects
	for(_i = FaceGetChild( wrd_ );
		_i;
		_i = FaceGetNext(_i) )
	{
		if( IS_RUN_MODE() )
		{
			_lpPrm = FaceGetParameter( _i );
			if( !_lpPrm )
				continue;		//because all tree is invisible!
			MFPUREAD( _lpPrm, FACE_VISIBLE, BOOL, _bVisible );
			if( !_bVisible )	
				continue;		//because all tree is invisible!
		}

		if( IsFaceRedraw(_i) )
		{
			FacePaint( _i, hdc_ );
		}
		else
		{
			FaceGetRect( _i, &_rc );
			if( RectInRegion( hrgnDirty_, &_rc ) )
			{
				FacePaint( _i, hdc_ );
			}
		}

		_Redraw_FaceTree( _i, hdc_, hrgnDirty_ );
	}

	return;
}

//...................................................................
//Description:
//...................................................................
void CoreStep_FirstTime()
{
	WRD		_wrd;

	//initiate all Start threads
	for( _wrd = ModuGetChild(1); 	//root node
		 _wrd;
		 _wrd = ModuGetNext( _wrd ) )
	{
		if( ModuGetAlgo( _wrd ) == IDA_START )
			InitiateModuThread( _wrd );
	}

	return;
}

//...................................................................
//Description:
//...................................................................
void CoreStep_Continue()
{
	WRD		_wrd;
	WORD	_flags;
	int		_i;

	//for each thread, do contine
	for( _i=0; _i<_STACK_MAX; _i++ )
	{
		//if this tread is using
		if( sStackArray[_i].iStackPoint )
		{
			_wrd = PopModuThread( _i, &_flags );

			//continue this tread
			SetCurrentModuThread( _i );
			RunModuTree( _i, _wrd, _flags );
            SetCurrentModuThread( -1 );
		}
	}

	return;
}

//...................................................................
//Description:
//	The function does run-function of each MTU in the modu
//	tree specified by wMTU.
//	The modu tree includes wMTU_ and it's all children;
//Entry:
//	WRD wMTU_		the entry Modu Unit
//Note:
//	At first, the function runs the self AlgoRun(...); then
//	its children's AlgoRun;
//	this function iterates the root node, i.e. wMTU_, and all
//	children;
//...................................................................
_DLLEXP ACRET _DLLFUNC RunModuTree(int iStack_, WRD wMTU_, WORD wFlags_ )
{
	WRD		_wrd, _tmp;
	UINT	_ret;
	WORD	_flags;

	PushModuThread( iStack_, wMTU_, wFlags_ );
	_wrd = wMTU_;
	goto _AFTER_PUSH;

_AGAIN:
	PushModuThread( iStack_, _wrd, STACKFLAG_NORMAL );

_AFTER_PUSH:
	if( !ModuIsValid(_wrd) )
	{
		//CORE_ASSERT( FALSE );
		goto _POPUP;
	}

#ifdef WIN32
	__try
	{
#endif
	_ret = AlgoRun( ModuGetAlgo(_wrd), _wrd );
#ifdef WIN32
	}
	__except( CoreGetData(CGD_EXCEPTIONTRAP)? EXCEPTION_EXECUTE_HANDLER : EXCEPTION_CONTINUE_SEARCH )
	{
		LOGERR1( "Exception occurred while running the modu %d", _wrd );
		_ret = TRUE;	//exception occurred, return FALSE;
	}
#endif

	/*
#ifndef	NDEBUG
	_ret = AlgoRun( ModuGetAlgo(_wrd), _wrd );
#else
	__try{
		_ret = AlgoRun( ModuGetAlgo(_wrd), _wrd );
	}catch{...}
#endif
	*/

	if( _ret == ACRET_BLOCK )
		return _ret;		//not pop

	if(	_ret == ACRET_END )
	{
		//clear stack until thread root stack frame
		EndModuThread( iStack_ );
		goto _POPUP;	//the parent node is thread root
	}

	if( _ret == ACRET_BREAK )
	{
		//clear stack until loop stack frame if loop stack frame is found
		if( BreakModuThread( iStack_ ) )
		{
			_ret = ACRET_BLOCK;
            goto _RETURN;
		}

		//else, on loop stack frame is found,
		//and this "break" modu is omitted
		LOGWARN2( "Break modu without loop: Modu:%d, Stack:%d", _wrd, iStack_ );
	}

	if( _ret == ACRET_RETURN )
	{
		//clear stack until call stack frame if call stack frame is found
		if( ReturnModuThread( iStack_ ) )
			goto _POPUP;	//here, the parent is caller

		//else, no action does, and this "break" modu is omitted
		LOGWARN2( "Break modu without loop: Modu:%d, Stack:%d", _wrd, iStack_ );
	}

	if( ! ( ModuGetStatus( _wrd ) & MTUBIT_SKIPCHILD ) )
	{
		_tmp = ModuGetChild( _wrd );
		if( _tmp )
		{
			_wrd = _tmp;
			goto _AGAIN;
		}
	}

	//no child or this is a skip-child node
	//

_POPUP:
	//pop the parent node
	_tmp = PopModuThread( iStack_, &_flags );
	if( !ModuIsValid(_tmp) )
	{
		if( _tmp == NULL )
			goto _RETURN;
		else
		{
			//CORE_ASSERT( FALSE );
			goto _POPUP;
		}
	}

	if(  ( _flags & STACKFLAG_THREADROOT )
	   ||( _tmp == NULL ) )		//no modu in stack
	{
		goto _RETURN;
	}

	if( _flags & STACKFLAG_LOOP )
	{
		//this is a loop node, and the executing tread should
		//be block here, so we can't continue to pop up node;
		//
		//push it back!
		PushModuThread( iStack_, _tmp, _flags );
		_ret = ACRET_BLOCK;
		goto _RETURN;
	}

	if( _flags & STACKFLAG_ROOT )
		goto _POPUP;

	_tmp = ModuGetNext( _tmp );
	if ( _tmp )
	{
		_wrd = _tmp;
		goto _AGAIN;
	}
	//no next
	goto _POPUP;

_RETURN:
	return _ret;
}

//...................................................................
//Description:
//...................................................................
_DLLEXP BOOL _DLLFUNC PushModuThread( int iStack_, WRD wMTU_, WORD wFlags_ )
{
	int _spt;

	if( iStack_ >= _STACK_MAX )
	{
		CORE_ASSERT( FALSE );
		return FALSE;
	}

	_spt = sStackArray[iStack_].iStackPoint;
	if( _spt < _STACK_DEPTH )
	{
		((sStackArray[iStack_]).msu)[_spt].wModu = wMTU_;
		((sStackArray[iStack_]).msu)[_spt].wFlags = wFlags_;

		//point to next adding point
		sStackArray[iStack_].iStackPoint ++ ;
		return TRUE;
	}
	else
	{
		CORE_ASSERT( FALSE );
		LOGERR0( "modu run stack overflow" );
		return FALSE;
	}

}

//...................................................................
//Description:
//...................................................................
_DLLEXP WRD _DLLFUNC PopModuThread( int iStack_, WORD _FAR* lpFlags_ )
{
	int _spt;

	if( iStack_ >= _STACK_MAX )
	{
		CORE_ASSERT( FALSE );
		return FALSE;
	}

	if( sStackArray[iStack_].iStackPoint )
	{
		sStackArray[iStack_].iStackPoint --;
		_spt = sStackArray[iStack_].iStackPoint;

		*lpFlags_ = ((sStackArray[iStack_]).msu)[_spt].wFlags;
		return ((sStackArray[iStack_]).msu)[_spt].wModu;
	}
	else
	{
		CORE_ASSERT( FALSE );
		return NULL;
	}
}

//...................................................................
//Description:
//...................................................................
int GetFreeStack()
{
	int	_i;

	for( _i=0; _i<_STACK_MAX; _i++ )
		if( !sStackArray[_i].iStackPoint )
		{
			//clear it
			memset( sStackArray +_i, 0, sizeof(sStackArray[0]) );
			return _i;
		}

	//no free stack
	return -1;
}

//...................................................................
//Description:
//...................................................................
void ClearAllStacks()
{
	int	_i;

	for( _i=0; _i<_STACK_MAX; _i++ )
	{
		//clear it
		memset( sStackArray +_i, 0, sizeof(sStackArray[0]) );
		return;
	}

	return;
}

//...................................................................
//Description:
//...................................................................
int	_ZZKDECL GetCurrentModuThread()
{
	return sCurStack;
}

//...................................................................
//Description:
//...................................................................
void _ZZKDECL SetCurrentModuThread( int iStack_ )
{
	sCurStack = iStack_;
	return;
}

//...................................................................
//Description:
//...................................................................
_DLLEXP ACRET _DLLFUNC InitiateModuThread( WRD wMTU_ )
{
	int		_stack;
	ACRET	_ret;

	//continue this tread
	_stack = GetFreeStack();
	if( _stack == -1 )
	{
		LOGERR0( "Maybe too many modu thread initiate!" );
		_ret = ACRET_FALSE;
	}
	else
	{
		SetCurrentModuThread( _stack );
		_ret = RunModuTree( _stack, wMTU_, STACKFLAG_THREADROOT|STACKFLAG_ROOT );
		SetCurrentModuThread( -1 );
	}

	return _ret;
}

//...................................................................
//Description:
//...................................................................
BOOL EndModuThread( int iStack_ )
{
	if( iStack_ >= _STACK_MAX )
	{
		CORE_ASSERT( FALSE );
		return FALSE;
	}

	sStackArray[iStack_].iStackPoint = 1;
	return TRUE;
}

//...................................................................
//Description:
//...................................................................
BOOL BreakModuThread( int iStack_ )
{
	int _spt;

	if( iStack_ >= _STACK_MAX )
	{
		CORE_ASSERT( FALSE );
		return FALSE;
	}

	for(_spt=sStackArray[iStack_].iStackPoint - 1;
		_spt > 0;
		_spt-- )
	{
		if( ((sStackArray[iStack_]).msu)[_spt].wFlags & STACKFLAG_LOOP )
		{
			sStackArray[iStack_].iStackPoint = _spt + 1;
			return TRUE;
		}
	}

	return FALSE;	//no found loop stack frame
}

//...................................................................
//Description:
//...................................................................
BOOL ReturnModuThread( int iStack_ )
{
	int _spt;

	if( iStack_ >= _STACK_MAX )
	{
		CORE_ASSERT( FALSE );
		return FALSE;
	}

	for(_spt=sStackArray[iStack_].iStackPoint - 1;
		_spt > 0;
		_spt-- )
	{
		if( ((sStackArray[iStack_]).msu)[_spt].wFlags & STACKFLAG_CALL )
		{
			sStackArray[iStack_].iStackPoint = _spt + 1;
			return TRUE;
		}
	}

	return FALSE;	//no found call stack frame
}

