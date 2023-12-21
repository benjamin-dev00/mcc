//===================================================================
//
//Zheng zhikai	95-11-22
//===================================================================
#include "prehead.h"

#include "coreall.h"

//private data struct for editing rectangle class stamps
typedef struct _tagRECTCLASSEDIT {
	int		oc;		//==0 MOVE operation; otherwise, SIZE operation
	RECT	rcSelected;
	int		wHandle;
}RCE;

static RECT	srcHandle[] =
{
	{1, 1, 0, 0},			//	0	1	2
	{0, 1, 0, 0},			//
	{0, 1, 1, 0},			//	7		3
	{0, 0, 1, 0},			//
	{0, 0, 1, 1},			//	6	5	4
	{0, 0, 0, 1},
	{1, 0, 0, 1},
	{1, 0, 0, 0}
};

//...................................................................
//Description:
//	Maps Window coordinate to Face coordinate.
//...................................................................
_DLLEXP void _DLLFUNC WindowCoorToFaceCoor(WRD wFTU_, LPFCCRD lpx_, LPFCCRD lpy_)
{
	LPPU			_lpPrm;
	XYCXCY			_xycxcy;

	wFTU_ = FaceGetParent(wFTU_);

	if( FaceGetParent( wFTU_ ) == FACE_ROOT )
	{
		//
		//transform coor (xw, yw) to (x0, y0)
		//
		//Read parent Face's XYCXCY
		_lpPrm =  FaceGetParameter(wFTU_);

		MFPUREAD(_lpPrm, FACE_X, FCCRD, _xycxcy.x);
		MFPUREAD(_lpPrm, FACE_Y, FCCRD, _xycxcy.y);
		if( FaceGetStatus(wFTU_) & FTUBIT_AUTOSIZECHILDREN )
		{
			FCCRD _width, _height;

			MFPUREAD(_lpPrm, FACE_CX, FCCRD, _xycxcy.cx);
			MFPUREAD(_lpPrm, FACE_CY, FCCRD, _xycxcy.cy);
			MFPUREAD(_lpPrm, FACEWND_CLIENTCX, FCCRD, _width);
			MFPUREAD(_lpPrm, FACEWND_CLIENTCY, FCCRD, _height);

			*lpx_ = *lpx_ * _xycxcy.cx / _width + _xycxcy.x;
			*lpy_ = *lpy_ * _xycxcy.cy / _height + _xycxcy.y;
		}
		else
		{
			*lpx_ = *lpx_ + _xycxcy.x;
			*lpy_ = *lpy_ + _xycxcy.y;
		}
	}
	else
	{
		WindowCoorToFaceCoor(wFTU_, lpx_, lpy_);

		_lpPrm = FaceGetParameter(wFTU_);
		MFPUREAD(_lpPrm, FACE_X, FCCRD, _xycxcy.x);
		MFPUREAD(_lpPrm, FACE_Y, FCCRD, _xycxcy.y);

		if( FaceGetStatus(wFTU_) & FTUBIT_AUTOSIZECHILDREN )
		{
			MFPUREAD(_lpPrm, FACE_CX, FCCRD, _xycxcy.cx);
			MFPUREAD(_lpPrm, FACE_CY, FCCRD, _xycxcy.cy);

			*lpx_ = (*lpx_ - _xycxcy.x) / _xycxcy.cx;
			*lpy_ = (*lpy_ - _xycxcy.y) / _xycxcy.cy;
		}
		else
		{
			*lpx_ = *lpx_  - _xycxcy.x;
			*lpy_ = *lpy_  - _xycxcy.y;
		}
	}

	return;
}

//...................................................................
//Description:
//	Maps Face coordinate to Window coordinate
//...................................................................
_DLLEXP void _DLLFUNC FaceCoorToWindowCoor(WRD wFTU_, LPFCCRD lpx_, LPFCCRD lpy_)
{
	LPPU			_lpPrm;
	WRD				_wParent;
	XYCXCY			_xycxcy;

	//transform coor (xi, yi) to (x0, y0)
	for(_wParent = FaceGetParent(wFTU_);
		FaceGetParent(_wParent) != FACE_ROOT;
		_wParent = FaceGetParent(_wParent) )
	{
		//Read parent Face's XYCXCY
		_lpPrm =  FaceGetParameter(_wParent);

		MFPUREAD(_lpPrm, FACE_X, FCCRD, _xycxcy.x);
		MFPUREAD(_lpPrm, FACE_Y, FCCRD, _xycxcy.y);

		if( FaceGetStatus(_wParent) & FTUBIT_AUTOSIZECHILDREN )
		{
			MFPUREAD(_lpPrm, FACE_CX, FCCRD, _xycxcy.cx);
			MFPUREAD(_lpPrm, FACE_CY, FCCRD, _xycxcy.cy);

			*lpx_ = *lpx_ * _xycxcy.cx + _xycxcy.x;
			*lpy_ = *lpy_ * _xycxcy.cy + _xycxcy.y;
		}
		else
		{
			*lpx_ = *lpx_  + _xycxcy.x;
			*lpy_ = *lpy_  + _xycxcy.y;
		}
	}

	//
	//transform coor (x0, y0) to (xw, yw)
	//
	//Read parent Face's XYCXCY
	_lpPrm =  FaceGetParameter(_wParent);

	MFPUREAD(_lpPrm, FACE_X, FCCRD, _xycxcy.x);
	MFPUREAD(_lpPrm, FACE_Y, FCCRD, _xycxcy.y);
	if( FaceGetStatus(_wParent) & FTUBIT_AUTOSIZECHILDREN )
	{
		FCCRD _width, _height;

		MFPUREAD(_lpPrm, FACE_CX, FCCRD, _xycxcy.cx);
		MFPUREAD(_lpPrm, FACE_CY, FCCRD, _xycxcy.cy);
		MFPUREAD(_lpPrm, FACEWND_CLIENTCX, FCCRD, _width);
		MFPUREAD(_lpPrm, FACEWND_CLIENTCY, FCCRD, _height);

		*lpx_ = (*lpx_  - _xycxcy.x) * _width / _xycxcy.cx;
		*lpy_ = (*lpy_  - _xycxcy.y) * _height / _xycxcy.cy;
	}
	else
	{
		*lpx_ = (*lpx_  - _xycxcy.x);
		*lpy_ = (*lpy_  - _xycxcy.y);
	}

	return;
}

//...................................................................
//...................................................................
_DLLEXP void _DLLFUNC GetFaceXycxcyInWinCoor(WRD wFTU_, LPXYCXCY lpcc_)
{
	LPPU	_lpPrm = FaceGetParameter( wFTU_ );
	FCCRD	_x, _y;

	MFPUREAD(_lpPrm, FACE_X, FCCRD, lpcc_->x);
	MFPUREAD(_lpPrm, FACE_Y, FCCRD, lpcc_->y);
	MFPUREAD(_lpPrm, FACE_CX, FCCRD, lpcc_->cx);
	MFPUREAD(_lpPrm, FACE_CY, FCCRD, lpcc_->cy);

	_x = lpcc_->x + lpcc_->cx;
	_y = lpcc_->y + lpcc_->cy;
	FaceCoorToWindowCoor( wFTU_, &(lpcc_->x), &(lpcc_->y) );
	FaceCoorToWindowCoor( wFTU_, &_x, &_y );
	lpcc_->cx = _x - lpcc_->x;
	lpcc_->cy = _y - lpcc_->y;

	return;
}

//...................................................................
//...................................................................
_DLLEXP void _DLLFUNC SetFaceXycxcyInWinCoor(WRD wFace_, LPXYCXCY lpcc_)
{
	XYCXCY		_cc;
	LPPU		_lpPrm = FaceGetParameter(wFace_);

	memcpy(&_cc, lpcc_, sizeof(_cc));

	_cc.cx =+ _cc.x;
	_cc.cy =+ _cc.y;

	WindowCoorToFaceCoor(wFace_, &_cc.x, &_cc.y);
	WindowCoorToFaceCoor(wFace_, &_cc.cx, &_cc.cy);

	_cc.cx = _cc.cx - _cc.x;
	_cc.cy = _cc.cy - _cc.y;

	MFPUWRITE(_lpPrm, FACE_X, FCCRD, _cc.x);
	MFPUWRITE(_lpPrm, FACE_Y, FCCRD, _cc.y);
	MFPUWRITE(_lpPrm, FACE_CX, FCCRD, _cc.cx);
	MFPUWRITE(_lpPrm, FACE_CY, FCCRD, _cc.cy);
}

//...................................................................
//Description:
//	Maps Xycxcy structure with Face coordinate to rectangle
//	structure with Window coordinate
//Note:
//	Here, do not and MUST NOT propertify lprc_ so that
//	will effect the macro XYCXCYTOTWOPOINTS
//...................................................................
_DLLEXP void _DLLFUNC XycxcyToRect(WRD wFace_, LPRECT lprc_, LPXYCXCY lpcc_)
{
	XYCXCY		_cc;

	memcpy(&_cc, lpcc_, sizeof(_cc));
	_cc.cx += _cc.x;
	_cc.cy += _cc.y;

	FaceCoorToWindowCoor(wFace_, &_cc.x, &_cc.y);
	FaceCoorToWindowCoor(wFace_, &_cc.cx, &_cc.cy);

	lprc_->left = (int)_cc.x;
	lprc_->top = (int)_cc.y;
	lprc_->right = (int)_cc.cx;
	lprc_->bottom = (int)_cc.cy;

	return;
}

//...................................................................
//Description:
//	Maps rectangle structure with Window coordinate  to
//	Xycxcy structure with Face coordinate
//Note:
//	Here, do not and MUST NOT the legality of lprc_ so that
//	will effect the macro TWOPOINTSTOXYCXCY
//...................................................................
_DLLEXP void _DLLFUNC RectToXycxcy(WRD wFace_, LPRECT lprc_, LPXYCXCY lpcc_)
{
	XYCXCY		_cc;

	_cc.x = (REAL)lprc_->left;
	_cc.y = (REAL)lprc_->top;
	_cc.cx = (REAL)lprc_->right;
	_cc.cy = (REAL)lprc_->bottom;

	WindowCoorToFaceCoor(wFace_, &_cc.x, &_cc.y);
	WindowCoorToFaceCoor(wFace_, &_cc.cx, &_cc.cy);

	_cc.cx = _cc.cx - _cc.x;
	_cc.cy = _cc.cy - _cc.y;

	memcpy(lpcc_, &_cc, sizeof(_cc));

	return;
}

//...................................................................
//Description:
//	Fills RECT using the FTU's first four parameter:
//		X, Y, CX, CY
//	after coordinate transformation,
//...................................................................
_DLLEXP void _DLLFUNC FaceXycxcyToRect(WRD wFace_, LPRECT lprc_)
{
	LPPU			_lpPrm;
	XYCXCY			_cc;

	_lpPrm = FaceGetParameter(wFace_);

	MFPUREAD(_lpPrm, FACE_X, FCCRD, _cc.x);
	MFPUREAD(_lpPrm, FACE_Y, FCCRD, _cc.y);
	MFPUREAD(_lpPrm, FACE_CX, FCCRD, _cc.cx);
	MFPUREAD(_lpPrm, FACE_CY, FCCRD, _cc.cy);

	XycxcyToRect(wFace_, lprc_, &_cc);

	return;
}

//...................................................................
//Description:
//	Maps the rectangle with window coor. to face coor. then fill FTU's
//		X, Y, cX, cY
//	with the result.
//...................................................................
_DLLEXP void _DLLFUNC WriteFaceXYCXCY(WRD wFace_, LPRECT lpRc_)
{
	LPPU			_lpPrm;
	XYCXCY			_cc;

	_lpPrm = FaceGetParameter(wFace_);

	RectToXycxcy(wFace_, lpRc_, &_cc);

	MFPUWRITE(_lpPrm, FACE_X, FCCRD, _cc.x);
	MFPUWRITE(_lpPrm, FACE_Y, FCCRD, _cc.y);
	MFPUWRITE(_lpPrm, FACE_CX, FCCRD, _cc.cx);
	MFPUWRITE(_lpPrm, FACE_CY, FCCRD, _cc.cy);

	return;
}

//...................................................................
//Note:
//	left_, top_ is by window coor.
//...................................................................
_DLLEXP void _DLLFUNC WriteFaceXY(WRD wFace_, FCCRD left_, FCCRD top_)
{
	LPPU			_lpPrm;
	FCCRD			_x, _y;

	_lpPrm = FaceGetParameter(wFace_);
	_x = (FCCRD)left_;
	_y = (FCCRD)top_;

	WindowCoorToFaceCoor(wFace_, &_x, &_y);

	MFPUWRITE( _lpPrm, FACE_X, FCCRD, _x);
	MFPUWRITE( _lpPrm, FACE_Y, FCCRD, _y);

	return;
}

//...................................................................
//Note:
//	width_, height_ is by window coor.
//...................................................................
_DLLEXP void _DLLFUNC WriteFaceCXCY(WRD wFace_, FCCRD width_, FCCRD height_ )
{
	LPPU			_lpPrm = FaceGetParameter(wFace_);
	FCCRD			_x, _y, _x1, _y1, _cx, _cy;

	//get face original x, y,
	MFPUREAD( _lpPrm, FACE_X, FCCRD, _x);
	MFPUREAD( _lpPrm, FACE_Y, FCCRD, _y);

	//calculate new cx, cy
	_x1 = _x;
	_y1 = _y;
	FaceCoorToWindowCoor( wFace_, &_x1, &_y1 );
	_x1 += (FCCRD)width_;	
	_y1 += (FCCRD)height_;
	WindowCoorToFaceCoor( wFace_, &_x1, &_y1 );
	_cx = _x1 - _x;
	_cy = _y1 - _y;

	MFPUWRITE( _lpPrm, FACE_CX, FCCRD, _cx);
	MFPUWRITE( _lpPrm, FACE_CY, FCCRD, _cy);

	return;
}

static WORD _near _SCMDUpdate( WRD wFTU_, HWND hwnd_, WORD wParam_, LONG lParam_ );
static WORD _near _SCMDGetInvalidRgn( WRD wFTU_, HWND hwnd_, WORD wParam_, LONG lParam_ );
static WORD _near _SCMDMouseOnBody( WRD wFTU_, WORD wPrecision_, POINT pt_ );
static WORD _near _SCMDMouseOnHandle( WRD wFTU_, WORD wSHSize_, POINT pt_ );
static WORD _near _SCMDMove(WRD wFTU_, HWND hwnd_, WORD wParam_, LONG lParam_);
static WORD _near _SCMDConstructMSG(WRD wFTU_, HWND hwnd_, LONG lParam_);
static WORD _near _SCMDEditMSGMove(WRD wFTU_, HWND hwnd_, LONG lParam_);
static WORD _near _SCMDEditMSGSize(WRD wFTU_, HWND hwnd_, LONG lParam_);

//...................................................................
//...................................................................
_DLLEXP DWORD _DLLFUNC DefStampBuild(SCMD wCmd_, WRD wFTU_, HWND hwnd_, WORD wParam_, LONG lParam_)
{
	switch(wCmd_)
	{
		case SCMD_MOUSEONHANDLE: //tests whether mouse is on handle
		{
			POINT		pt;
			ASSIGN_POINT( pt, lParam_ )
			return _SCMDMouseOnHandle( wFTU_, wParam_, pt );
		}

		case SCMD_MOUSEONBODY:
		{
			POINT		_pt;
			ASSIGN_POINT( _pt, lParam_ )

			return _SCMDMouseOnBody( wFTU_, wParam_, _pt );
		}

		case SCMD_TOGGLESELECTHANDLES:
		{
			RECT	_rect;

			FaceXycxcyToRect(wFTU_, &_rect);
			ToggleRSH( (HDC)lParam_, &_rect, wParam_ );
			return TRUE;
		}

		case SCMD_MOVE:
			return _SCMDMove(wFTU_, hwnd_, wParam_, lParam_);

		case SCMD_CONSTRUCT:
		{
			NPRECT _npRect = (NPRECT)LocalAlloc( LPTR, sizeof(RECT) );

			if( !_npRect )
				return FALSE;

			((LPFIA)lParam_)->lpPrivate = _npRect;
			return TRUE;
		}

		case SCMD_CONSTRUCT_INPUTMSG:
			return _SCMDConstructMSG(wFTU_, hwnd_, lParam_);

		case SCMD_EDIT:
		{
			RCE _near* 		_npRCE;
			WORD			_wMouseOn;
			LPFIA			_lpFma = (LPFIA)lParam_;
			POINT			_pt;

			if( _lpFma->msg != WM_LBUTTONDOWN )
				return FALSE;

			ASSIGN_POINT( _pt, _lpFma->lParam )

			//Don't call stamp defined SCMD_MOUSEONHANDLE routine, because
			//this function don't know how to process the stamp defined 
			//selection handles.
			_wMouseOn = _SCMDMouseOnHandle( wFTU_, _lpFma->wSHSize, _pt );

			if( _wMouseOn == MOUSEON_NULL)	// not on the select handle
			{
				//Call stamp defined routine, not _SCMDMouseOnBody
				//

				/** 
					_wMouseOn = _SCMDMouseOnBody( wFTU_, _lpFma->wPrecision, _pt );
				**/

				_wMouseOn = (WORD)StampCallBuildProc(SCMD_MOUSEONBODY,
												wFTU_,
												hwnd_,
												_lpFma->wPrecision,
												_lpFma->lParam );

			}

			if( _wMouseOn == MOUSEON_NULL )	//neither on the select handle nor on body
				return FALSE;

			_npRCE = (RCE _near*)LocalAlloc( LPTR, sizeof(RCE) );
			if( !_npRCE )
				return FALSE;
			((LPFIA)lParam_)->lpPrivate = _npRCE;

			if(_wMouseOn == MOUSEON_BODY)
				_npRCE->oc = 0;		//0: prepare to perform MOVE action
			else	// on handle
			{
				_npRCE->wHandle = _wMouseOn;
				_npRCE->oc = 1;		//1: prepare to perform SIZE action
			}

			return TRUE;
		}

		case SCMD_EDIT_INPUTMSG:
		{
			RCE near*	_npRce = (RCE near*)((LPFIA)lParam_)->lpPrivate;

			if( _npRce->oc == 0 )	//perform MOVE action
				return _SCMDEditMSGMove(wFTU_, hwnd_, lParam_);
			else
			{
				CORE_ASSERT( _npRce->oc == 1 );	//perform SIZE action
				return _SCMDEditMSGSize(wFTU_, hwnd_, lParam_);
			}
		}

		case SCMD_GETREGION:	
		{
			RECT		_rect;

			FaceXycxcyToRect(wFTU_, &_rect);

			//wParam_ ++;
			if( IsFaceSelect(wFTU_) )
				InflateRect(&_rect, wParam_, wParam_);

			return (DWORD)CreateRectRgnIndirect( &_rect );
		}

		case SCMD_GETINVALIDRGN:
			return _SCMDGetInvalidRgn( wFTU_, hwnd_, wParam_, lParam_ );

		case SCMD_GETFOCUSRECT:
		{
			FaceXycxcyToRect(wFTU_, (LPRECT)lParam_ );
			//FaceGetRect( wFTU_, (LPRECT)lParam_ );
			return TRUE;
		}

		case SCMD_RUNTIME:
			return FALSE;

		case SCMD_RUNTIME_INPUTMSG:
			return FIA_PROCESSEND;

		default:
			CORE_ASSERT(0);
			return FALSE;
	}

}

//...................................................................
//Description:
//...................................................................
static WORD _near _SCMDMouseOnHandle( WRD wFTU_, WORD wSHSize_, POINT pt_ )
{
	RECT	_rect;

	//default action: process as a rectangle FTU
	FaceXycxcyToRect(wFTU_, &_rect);
	return TestMouseOnRSH(&_rect , pt_, wSHSize_ );
}

//...................................................................
//Description:
//...................................................................
static WORD _near _SCMDMouseOnBody( WRD wFTU_, WORD wPrecision_, POINT pt_ )
{
	RECT	_rect;

	FaceXycxcyToRect(wFTU_, &_rect);
	InflateRect(&_rect, wPrecision_, wPrecision_);	//add the select precision
	if( PtInRect(&_rect, pt_) )
		return MOUSEON_BODY;
	else
		return MOUSEON_NULL;
}

//...................................................................
//Description:
//	The command is used at biuld-time.
//...................................................................
static WORD _near _SCMDGetInvalidRgn( WRD wFTU_, HWND hwnd_, WORD wParam_, LONG lParam_ )
{
	LPGIR		_lpgir = (LPGIR)lParam_;
	RECT		_rcNew, _rcOld;
	HRGN		_rgnNew, _rgnOld;

	if( !wParam_ ) 	//no dirty
	{
		//need do nothing
	}
	else if( wParam_ & DEEP_DIRTY )	//position changed
	{
		FaceXycxcyToRect(wFTU_, &_rcNew);

		FaceGetRect( wFTU_, &_rcOld );
		FaceSetRect( wFTU_, &_rcNew );		//set new face rect

		//for active face, the focus rectangle cant be treated very well,
		//so, here in order to avoid the problem, we enlarge the rectangles
		//InflateRect( &_rcOld, 3, 3 );
		//InflateRect( &_rcNew, 3, 3 );

		_rgnOld = CreateRectRgnIndirect( &_rcOld );
		_rgnNew = CreateRectRgnIndirect( &_rcNew );
		CombineRgn( _lpgir->girDirtyRgn, _lpgir->girDirtyRgn, _rgnOld, RGN_OR);
		CombineRgn( _lpgir->girDirtyRgn, _lpgir->girDirtyRgn, _rgnNew, RGN_OR);
		DeleteObject( _rgnOld );
		DeleteObject( _rgnNew );
		FaceSetRedraw( wFTU_ );
	}
	else
	{
		//no sizing or moving
		FaceGetRect( wFTU_, &_rcNew );
		_rgnNew = CreateRectRgnIndirect( &_rcNew );
		CombineRgn( _lpgir->girDirtyRgn, _lpgir->girDirtyRgn, _rgnNew, RGN_OR);
		DeleteObject( _rgnNew );
		FaceSetRedraw( wFTU_ );
	}
	return TRUE;
}

//...................................................................
//Description:
//	The command is used at biuld-time.
//...................................................................
static WORD _near _SCMDUpdate( WRD wFTU_, HWND hwnd_, WORD wParam_, LONG lParam_ )
{
	RECT		_rcNew, _rcOld;
	//LPPU		_lpPrm = FaceGetParameter(wFTU_);

	//the means of lParam_ references to the return value of
	//FaceRunTimeDirty(WRD wFTU_)
	//
	if( lParam_ )	//has dirty paramter
	{
		wParam_ ++;		//wParam_: the half-size of select handle

		FaceGetRect( wFTU_, &_rcOld );

		//here, the face is moved or sized
		FaceXycxcyToRect( wFTU_, &_rcNew );
		FaceSetRect( wFTU_, &_rcNew );		//set new face rect

		if( IsFaceSelect(wFTU_) )
		{
			InflateRect( &_rcNew, wParam_, wParam_ );
			InflateRect( &_rcOld, wParam_, wParam_ );
		}

		InvalidateRect( hwnd_, &_rcOld, TRUE );
		InvalidateRect( hwnd_, &_rcNew, TRUE );
	}
	else
	{
		FaceGetRect( wFTU_, &_rcOld );

		//?? UNCERTAIN action
		//position and size are no change
		//maybe need not erase background
		//BUT, some stamp still need to erase
		//background, such as Trend
		InvalidateRect( hwnd_, &_rcOld, TRUE );
	}

	return TRUE;
}

//...................................................................
//Description:
//...................................................................
#pragma argsused
static WORD _near _SCMDConstructMSG(WRD wFTU_, HWND hwnd_, LONG lParam_)
{
	WORD	_ret = FIA_PROCESSED;
	LPFIA	_lpFma = (LPFIA)lParam_;
	NPRECT	_npRect = (NPRECT)_lpFma->lpPrivate;

	// _npRect should be allocated when receiving SCMD_CONSTRUCT
	if( !_npRect )
	{
		CORE_ASSERT( FALSE );
		return FIA_DELETEFACE;
	}

	switch( _lpFma->msg )
	{
		case WM_LBUTTONDOWN:
			//init. _lpFma->dtPrimary and _lpFma->dtLast
			_lpFma->dtPrimary.x = X(_lpFma->lParam);
			_lpFma->dtPrimary.y = Y(_lpFma->lParam);
			_lpFma->dtLast.x = X(_lpFma->lParam);
			_lpFma->dtLast.y =Y(_lpFma->lParam);

			//init. rcSelected
			_npRect->left = _npRect->right = X(_lpFma->lParam);
			_npRect->top = _npRect->bottom = Y(_lpFma->lParam);
			break;

		case WM_MOUSEMOVE:
			//clear old focus rect
			DrawFocusRect( _lpFma->hdc, _npRect );

			_npRect->left = _lpFma->dtPrimary.x;
			_npRect->top = _lpFma->dtPrimary.y;
			_npRect->right = X(_lpFma->lParam);
			_npRect->bottom = Y(_lpFma->lParam);
			PropertyRect( _npRect );
			DrawFocusRect( _lpFma->hdc, _npRect );	//draw new focus rect

			//set _lpFma->dtLast
			_lpFma->dtLast.x = X(_lpFma->lParam);
			_lpFma->dtLast.y = Y(_lpFma->lParam);
			break;

		case WM_LBUTTONUP:
			//clear old focus rect
			DrawFocusRect(_lpFma->hdc, _npRect );

			if(  (_lpFma->dtLast.x == _lpFma->dtPrimary.x)
			   &&(_lpFma->dtLast.y == _lpFma->dtPrimary.y) )
			{
				_ret = FIA_DELETEFACE;	//delete zero-size object
			}
			else
			{
				WriteFaceXYCXCY( wFTU_, _npRect );	//write the x, y, cx, cy
				FaceSetRect( wFTU_, _npRect );

				FacePaint( wFTU_, _lpFma->hdc );	//paint the face
				_ret = FIA_PROCESSEND;
			}

			//free the private data
			LocalFree( (HLOCAL)_npRect );
			_lpFma->lpPrivate = NULL;
			break;

		default:
			_ret = FIA_NOTPROCESSED;
	}
	return _ret;
}

//...................................................................
//Description:
//...................................................................
static WORD _near _SCMDEditMSGMove(WRD wFTU_, HWND hwnd_, LONG lParam_)
{
	WORD		_ret = FIA_PROCESSED;
	LPFIA 		_lpFma = (LPFIA)lParam_;
	RCE near*	_npRce = (RCE near*)_lpFma->lpPrivate;

	// _npRce should be allocated when receiving SCMD_EDIT
	if( !_npRce )
	{
		CORE_ASSERT( FALSE );
		return FIA_PROCESSEND;
	}

	switch( _lpFma->msg )
	{
		case WM_LBUTTONDOWN:
			//get the FTU's rectangle
			FaceXycxcyToRect( wFTU_, &(_npRce->rcSelected) );

			DrawFocusRect(_lpFma->hdc, &(_npRce->rcSelected));

			//initialize FIA.dtLast and FIA.dtPrimary
			_lpFma->dtLast.x = X(_lpFma->lParam);
			_lpFma->dtLast.y = Y(_lpFma->lParam);
			_lpFma->dtPrimary.x = X(_lpFma->lParam);
			_lpFma->dtPrimary.y = Y(_lpFma->lParam);
			break;

		case WM_MOUSEMOVE:
		{
			int	_deltaX, _deltaY;

			//clear the old selected rectangle
			DrawFocusRect(_lpFma->hdc, &(_npRce->rcSelected));

			_deltaX = X(_lpFma->lParam) - _lpFma->dtLast.x;
			_deltaY = Y(_lpFma->lParam) - _lpFma->dtLast.y;
			_npRce->rcSelected.left += _deltaX;
			_npRce->rcSelected.right += _deltaX;
			_npRce->rcSelected.top += _deltaY;
			_npRce->rcSelected.bottom += _deltaY;
			PropertyRect(&(_npRce->rcSelected));

			//draw the new selected rectangle
			DrawFocusRect(_lpFma->hdc, &(_npRce->rcSelected));

			//set _lpFma->dtLast
			_lpFma->dtLast.x = X(_lpFma->lParam);
			_lpFma->dtLast.y = Y(_lpFma->lParam);

			CORE_ASSERT( abs(_lpFma->dtLast.x) < 30000 );
			CORE_ASSERT( abs(_lpFma->dtLast.y) < 30000 );

			break;
		}

		case WM_LBUTTONUP:
		{
			POINT	_pt;

			//clear old focus rect
			DrawFocusRect(_lpFma->hdc, &(_npRce->rcSelected));

			_pt.x = _lpFma->dtLast.x - _lpFma->dtPrimary.x;
			_pt.y = _lpFma->dtLast.y - _lpFma->dtPrimary.y;

			_SCMDMove(wFTU_, hwnd_, _lpFma->wSHSize, MAKELONG(_pt.x, _pt.y) );
			UpdateWindow( hwnd_ );

			LocalFree( (HLOCAL)_npRce );
			_lpFma->lpPrivate = NULL;
			_ret = FIA_PROCESSEND;
			break;
		}

		default:
			_ret = FIA_NOTPROCESSED;
	}

	return _ret;
}

//...................................................................
//Description:
//...................................................................
#pragma argsused
static WORD _near _SCMDEditMSGSize(WRD wFTU_, HWND hwnd_, LONG lParam_)
{
	WORD		_ret = FIA_PROCESSED;
	LPFIA 		_lpFma = (LPFIA)lParam_;
	RCE near*	_npRce = (RCE near*)_lpFma->lpPrivate;

	// _npRce should be allocated when receiving SCMD_EDIT
	if( !_npRce )
	{
		CORE_ASSERT( FALSE );
		return FIA_PROCESSEND;
	}

	switch( _lpFma->msg )
	{
		case WM_LBUTTONDOWN:
			FaceXycxcyToRect( wFTU_, &(_npRce->rcSelected) );

			DrawFocusRect(_lpFma->hdc, &(_npRce->rcSelected));

			_lpFma->dtLast.x = X(_lpFma->lParam);
			_lpFma->dtLast.y = Y(_lpFma->lParam);
			_lpFma->dtPrimary.x = X(_lpFma->lParam);
			_lpFma->dtPrimary.y = Y(_lpFma->lParam);
			break;

		case WM_MOUSEMOVE:
		{
			int	_deltaX, _deltaY;

			if( _npRce->wHandle > 7 )
			{
				CORE_ASSERT( FALSE );

				//clear the select-ractangle
				DrawFocusRect(_lpFma->hdc, &(_npRce->rcSelected));

				LocalFree( (HLOCAL)_npRce );
				_lpFma->lpPrivate = NULL;
				return FIA_PROCESSEND;
			}

			DrawFocusRect(_lpFma->hdc, &(_npRce->rcSelected));
			_deltaX = X(_lpFma->lParam) - _lpFma->dtLast.x;
			_deltaY = Y(_lpFma->lParam) - _lpFma->dtLast.y;
			_npRce->rcSelected.left += _deltaX * srcHandle[_npRce->wHandle].left;
			_npRce->rcSelected.top += _deltaY * srcHandle[_npRce->wHandle].top;
			_npRce->rcSelected.right += _deltaX * srcHandle[_npRce->wHandle].right;
			_npRce->rcSelected.bottom += _deltaY * srcHandle[_npRce->wHandle].bottom;
			if(_npRce->rcSelected.right <= _npRce->rcSelected.left)
				_npRce->rcSelected.right = _npRce->rcSelected.left + 1;
			if(_npRce->rcSelected.bottom <= _npRce->rcSelected.top)
				_npRce->rcSelected.bottom = _npRce->rcSelected.top + 1;
			DrawFocusRect(_lpFma->hdc, &(_npRce->rcSelected));

			//set _lpFma->dtLast
			_lpFma->dtLast.x = X(_lpFma->lParam);
			_lpFma->dtLast.y = Y(_lpFma->lParam);
			break;
		}

		case WM_LBUTTONUP:
		{
			//clear old focus rect
			DrawFocusRect(_lpFma->hdc, &(_npRce->rcSelected));

			//write new size
			WriteFaceXYCXCY( wFTU_, &(_npRce->rcSelected) );

			//redraw this core window to show sizing-action
			UpdateCoreWindow();

			//free private data
			LocalFree( (HLOCAL)_npRce );
			_lpFma->lpPrivate = NULL;

			_ret = FIA_PROCESSEND;
			break;
		}

		default:
			_ret = FIA_NOTPROCESSED;
	}


	return _ret;
}

//...................................................................
//Description:
//...................................................................
#pragma argsused
static WORD _near _SCMDMove(WRD wFTU_, HWND hwnd_, WORD wParam_, LONG lParam_)
{
	RECT	_rect;
	POINT	_pt;

	ASSIGN_POINT( _pt, lParam_ )

	//calculate the new the position of the face
	FaceXycxcyToRect(wFTU_, &_rect);
	_rect.left += _pt.x;
	_rect.top += _pt.y;
	_rect.right += _pt.x;
	_rect.bottom += _pt.y;

	//write new position, NOTE: ONLY update X and Y
	WriteFaceXY(wFTU_, (FCCRD)_rect.left, (FCCRD)_rect.top );

	//redraw this core window to show moving-action
	UpdateCoreWindow();

	//do not call UpdateWindow since there may be serv faces to be moved,
	//and the UpdateWindow should be called by caller after all face moved;
	return TRUE;
}

//...................................................................
//Description:
//	Test on which handle the mouse is, and the handle order is:
//	RSH	:Rectangle selecting handle
//
//		0	1	2
//		7		3
//		6	5	4
//
//Return:
//	MOUSEON_NULL or the handle ID
//...................................................................
_DLLEXP WORD _DLLFUNC TestMouseOnRSH(LPRECT lpRect_, POINT point_, WORD wSHSize_)
{
	int		_inX = 0,
			_inY = 0;
	WORD	_Handle;

	if(abs(lpRect_->left - point_.x) <= wSHSize_)
		_inX = 1;
	else if(abs( (lpRect_->left + lpRect_->right)/2 - point_.x) <= wSHSize_)
		_inX = 2;
	else if(abs(lpRect_->right - point_.x) <= wSHSize_)
		_inX = 3;
	else
		_inX = 0;

	if(abs(lpRect_->top - point_.y) <= wSHSize_)
		_inY = 1;
	else if(abs( (lpRect_->top + lpRect_->bottom)/2 - point_.y) <= wSHSize_)
		_inY = 2;
	else if(abs(lpRect_->bottom - point_.y) <= wSHSize_)
		_inY = 3;
	else
		_inY = 0;

	if(_inX * _inY == 0)
		return MOUSEON_NULL;

	switch( (_inX<<2) | (_inY) )
	{
		case (1<<2)|1:
			_Handle = 0;
			break;

		case (2<<2)|1:
			_Handle = 1;
			break;

		case (3<<2)|1:
			_Handle = 2;
			break;

		case (3<<2)|2:
			_Handle = 3;
			break;

		case (3<<2)|3:
			_Handle = 4;
			break;

		case (2<<2)|3:
			_Handle = 5;
			break;

		case (1<<2)|3:
			_Handle = 6;
			break;

		case (1<<2)|2:
			_Handle = 7;
			break;

		default:
			return MOUSEON_NULL;
	}

	return _Handle;
}

//...................................................................
//Description:
//	Formats the RECT struct pointed by lpRect_:
//		makes .left <= .right
//		makes .top <= .bottom
//...................................................................
_DLLEXP void _DLLFUNC PropertyRect(LPRECT lpRect_)
{
	int _tmp;

	if(lpRect_->bottom < lpRect_->top)
	{
		_tmp = lpRect_->top;
		lpRect_->top = lpRect_->bottom;
		lpRect_->bottom = _tmp;
	}

	if(lpRect_->right < lpRect_->left)
	{
		_tmp = lpRect_->left;
		lpRect_->left = lpRect_->right;
		lpRect_->right = _tmp;
	}

	return;
}

//...................................................................
//Description:
//	Toggles a Rect-Selecting-Handle , with 8 handles, for
//	identify rect. pointed by lpRect_.
//...................................................................
_DLLEXP void _DLLFUNC ToggleRSH(HDC hdc_, LPRECT lpRect_, WORD wSHSize_)
{
	int		_old_mode;
	HBRUSH	_hBrush;
	HPEN	_hPen;

	_old_mode = SetROP2(hdc_, R2_NOT);
	_hBrush = SelectObject( hdc_, GetStockObject(BLACK_BRUSH) );
	_hPen = SelectObject( hdc_, GetStockObject(NULL_PEN) );

	//debug code
	//Rectangle(hdc_, 10, 10, 100, 100 );

	Rectangle(hdc_, lpRect_->left - wSHSize_, lpRect_->top-wSHSize_, lpRect_->left+wSHSize_, lpRect_->top+wSHSize_);
	Rectangle(hdc_, lpRect_->right - wSHSize_, lpRect_->top-wSHSize_, lpRect_->right+wSHSize_, lpRect_->top+wSHSize_);
	Rectangle(hdc_, lpRect_->left - wSHSize_, lpRect_->bottom-wSHSize_, lpRect_->left+wSHSize_, lpRect_->bottom+wSHSize_);
	Rectangle(hdc_, lpRect_->right - wSHSize_, lpRect_->bottom-wSHSize_, lpRect_->right+wSHSize_, lpRect_->bottom+wSHSize_);

	Rectangle(hdc_, (lpRect_->left + lpRect_->right)/2 - wSHSize_,
					lpRect_->top - wSHSize_,
					(lpRect_->left + lpRect_->right)/2 + wSHSize_,
					lpRect_->top + wSHSize_);

	Rectangle(hdc_, (lpRect_->left + lpRect_->right)/2 - wSHSize_,
					lpRect_->bottom - wSHSize_,
					(lpRect_->left + lpRect_->right)/2 + wSHSize_,
					lpRect_->bottom + wSHSize_);

	Rectangle(hdc_, lpRect_->left - wSHSize_,
					(lpRect_->top + lpRect_->bottom)/2 - wSHSize_,
					lpRect_->left + wSHSize_,
					(lpRect_->top + lpRect_->bottom)/2 + wSHSize_);

	Rectangle(hdc_, lpRect_->right - wSHSize_,
					(lpRect_->top + lpRect_->bottom)/2 - wSHSize_,
					lpRect_->right + wSHSize_,
					(lpRect_->top + lpRect_->bottom)/2 + wSHSize_);

	SetROP2(hdc_, _old_mode);
	SelectObject( hdc_, _hBrush );
	SelectObject( hdc_, _hPen );

	//Rectangle(hdc_, 10, 10, 100, 100 );
	return;
}


