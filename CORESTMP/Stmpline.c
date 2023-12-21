//===================================================================
//
//Zheng zhikai	96-2-28
//===================================================================
#include "prehead.h"

#include "coreall.h"
#include "stmpdef.h"

#include <math.h>

#pragma warning( disable : 4244 )	//'conversion' conversion from 'type1' to 'type2', possible loss of data

#define LINE_PENSTYLE			(FACE_FIRSTPARAM + 0)
#define LINE_PENWIDTH			(FACE_FIRSTPARAM + 1)
#define LINE_PENCLR				(FACE_FIRSTPARAM + 2)
//Line() parameter type
#define LINE_PENSTYLE_PDT		PDT_int
#define LINE_PENWIDTH_PDT		PDT_int
#define LINE_PENCLR_PDT			PDT_COLORREF

typedef struct _tagLINESTRUCT{
	int		oc;			//operation code: 0:move; 1:Edit
	int		wHandle;	//at(x, y): handle 0x8000; at(cx+x, cy+y): handle 0x8001
	int		iOldMode;
	HPEN	hOldPen;
}LINESTRUCT, _near* NPLINESTRUCT;

static HRGN _near _LineGetRgn( WRD wFTU_, WORD wSHSize_ );
static WORD _near _LineToggleSelectHandle(WRD wFTU_, HWND hwnd_, WORD wSHSize_, HDC hdc_ );
static WORD _near _Line_SCMDMouseOnHandle( WRD wFTU_, WORD wSHSize_, POINT pt_ );
static WORD _near _Line_SCMDMouseOnBody( WRD wFTU_, WORD wPrecision_, POINT pt_ );
static WORD _near _Line_SCMDMove(WRD wFTU_, HWND hwnd_, WORD wParam_, LONG lParam_);
static WORD _near _Line_SCMDEditMSGMove(WRD wFTU_, HWND hwnd_, LONG lParam_);
static WORD _near _Line_SCMDEditMSGEdit(WRD wFTU_, HWND hwnd_, LONG lParam_);

static WORD _near _Line_SCMDGetInvalidRgn( WRD wFTU_, HWND hwnd_, WORD wParam_, LONG lParam_ );

//...................................................................
//...................................................................
_DLLEXP BOOL _DLLFUNC LinePaint(WRD wFTU_, HDC hdc_)
{
	COLORREF	_clr;
	int			_width;
	int			_style;
	//RECT		_rect;
	XYCXCY		_cc;
	HPEN		_hPen;

	LPPU		_lpPrm = FaceGetParameter(wFTU_);
	CORE_ASSERT(_lpPrm);

	GetFaceXycxcyInWinCoor( wFTU_, &_cc );

	MFPUREAD( _lpPrm, LINE_PENCLR, COLORREF, _clr );
	MFPUREAD( _lpPrm, LINE_PENWIDTH, int, _width );
	MFPUREAD( _lpPrm, LINE_PENSTYLE, int, _style );

	_hPen = CoreCreatePen(_style, _width, _clr);
	_hPen = SelectObject(hdc_, _hPen);
	MoveTo(hdc_, _cc.x, _cc.y);
	LineTo(hdc_, _cc.cx+_cc.x, _cc.cy+_cc.y);
	_hPen = SelectObject(hdc_, _hPen);
	CoreDeletePen(_hPen);

	return TRUE;
}

//...................................................................
//...................................................................
_DLLEXP DWORD _DLLFUNC LineParam(PIC wCmd_, WRD wFTU_, WORD wNo_, LONG lParam_)
{
	static PDU _sPDU[] =
	{
		_PDU_DEFAULT,
		_ELE_PDU(LINE_PENSTYLE, "LineStyle", VSOF_int_LNSTYLE,		PU_NORMAL,	PRI_ALL ),
		_ELE_PDU(LINE_PENWIDTH, "LineWidth", VSOF_int,				PU_NORMAL,	PRI_ALL ),
		_ELE_PDU(LINE_PENCLR,   "LineColor", VSOF_DWORD_CLR,		PU_NORMAL,	PRI_ALL )
	};

	return _DefStampParam(wCmd_,
						  wFTU_,
						  wNo_,
						  lParam_,
						  sizeof(_sPDU)/sizeof(PDU),
						  IDS_LINE, 
						  &(_sPDU[0]) );
}

//...................................................................
//...................................................................
_DLLEXP DWORD _DLLFUNC LineBuild(SCMD wCmd_, WRD wFTU_, HWND hwnd_, WORD wParam_, LONG lParam_)
{
	LPPU			_lpPrm = FaceGetParameter( wFTU_ );

	switch(wCmd_)
	{
		case SCMD_INIT:
		{
			LPFINIT			_lpFinit = (LPFINIT)lParam_;

			MFPUWRITE(_lpPrm, LINE_PENWIDTH, int, _lpFinit->line_pen.lopnWidth.x);
			MFPUWRITE(_lpPrm, LINE_PENSTYLE, int, _lpFinit->line_pen.lopnStyle);
			MFPUWRITE(_lpPrm, LINE_PENCLR, COLORREF, _lpFinit->line_pen.lopnColor);
			return TRUE;
		}

		case SCMD_GETINVALIDRGN:
			return _Line_SCMDGetInvalidRgn( wFTU_, hwnd_, wParam_, lParam_ );

		case SCMD_MOUSEONHANDLE: //tests whether mouse is on handle
		{
			POINT	_pt;
			ASSIGN_POINT( _pt, lParam_ )
			return _Line_SCMDMouseOnHandle( wFTU_, wParam_, _pt );
		}
		
		case SCMD_MOUSEONBODY:
		{
			POINT	_pt;
			ASSIGN_POINT( _pt, lParam_ )
			return _Line_SCMDMouseOnBody( wFTU_, wParam_, _pt );
		}

		case SCMD_TOGGLESELECTHANDLES:
		{
			return _LineToggleSelectHandle(wFTU_, hwnd_, wParam_, (HDC)lParam_ );
		}

		case SCMD_MOVE:
			return _Line_SCMDMove( wFTU_, hwnd_, wParam_, lParam_ );

		case SCMD_GETREGION:
			return (DWORD)_LineGetRgn( wFTU_, wParam_ );

		case SCMD_CONSTRUCT:
		{
			NPLINESTRUCT _npLS = (NPLINESTRUCT)malloc( sizeof(LINESTRUCT) );

			if( !_npLS )
				return FALSE;

			((LPFIA)lParam_)->lpPrivate = _npLS;
			return TRUE;
		}

		case SCMD_CONSTRUCT_INPUTMSG:
		{
			LPFIA			_lpFia = (LPFIA)lParam_;
			WORD			_ret = FIA_PROCESSED;
			NPLINESTRUCT 	_npLS = (NPLINESTRUCT)_lpFia->lpPrivate;

			switch( _lpFia->msg )
			{
				case WM_LBUTTONDOWN:
					//init. _lpFia->dtPrimary and _lpFia->dtLast
					_lpFia->dtPrimary.x = X(_lpFia->lParam);
					_lpFia->dtPrimary.y = Y(_lpFia->lParam);
					_lpFia->dtLast.x = X(_lpFia->lParam);
					_lpFia->dtLast.y =Y(_lpFia->lParam);

					//init. hdc
					_npLS->iOldMode = SetROP2( _lpFia->hdc, R2_NOT );
					_npLS->hOldPen = SelectObject( _lpFia->hdc, GetStockObject( BLACK_PEN ) );
					break;

				case WM_MOUSEMOVE:
					//clear old line
					MoveTo( _lpFia->hdc, _lpFia->dtPrimary.x, _lpFia->dtPrimary.y );
					LineTo( _lpFia->hdc, _lpFia->dtLast.x, _lpFia->dtLast.y );
					//set _lpFia->dtLast
					_lpFia->dtLast.x = X(_lpFia->lParam);
					_lpFia->dtLast.y = Y(_lpFia->lParam);
					//draw new line
					MoveTo( _lpFia->hdc, _lpFia->dtPrimary.x, _lpFia->dtPrimary.y );
					LineTo( _lpFia->hdc, _lpFia->dtLast.x, _lpFia->dtLast.y );
					break;

				case WM_LBUTTONUP:
				{
					XYCXCY	_cc;

					//clear old line
					MoveTo( _lpFia->hdc, _lpFia->dtPrimary.x, _lpFia->dtPrimary.y );
					LineTo( _lpFia->hdc, _lpFia->dtLast.x, _lpFia->dtLast.y );

					//restore the settings of HDC
					SetROP2( _lpFia->hdc, _npLS->iOldMode );
					SelectObject( _lpFia->hdc, _npLS->hOldPen );

					if(  (_lpFia->dtLast.x == _lpFia->dtPrimary.x)
					   &&(_lpFia->dtLast.y == _lpFia->dtPrimary.y) )
					{
						_ret = FIA_DELETEFACE;	//delete zero-size object
					}
					else
					{
						RECT	_rc;

						_cc.x = _lpFia->dtPrimary.x;
						_cc.y = _lpFia->dtPrimary.y;
						_cc.cx = _lpFia->dtLast.x - _lpFia->dtPrimary.x;
						_cc.cy = _lpFia->dtLast.y - _lpFia->dtPrimary.y;

						//write the face rect
						_rc.left	= _lpFia->dtPrimary.x;
						_rc.top		= _lpFia->dtPrimary.y;
						_rc.right	= _lpFia->dtLast.x;
						_rc.bottom	= _lpFia->dtLast.y;
						PropertyRect( &_rc );
						if( !(_rc.right - _rc.left) ) _rc.right ++;
						if( !(_rc.bottom - _rc.top) ) _rc.bottom ++;
						FaceSetRect( wFTU_, &_rc );

						WindowCoorToFaceCoor(wFTU_, &_cc.x, &_cc.y);
						WindowCoorToFaceCoor(wFTU_, &_cc.cx, &_cc.cy);

						MFPUWRITE(_lpPrm, FACE_X, FCCRD, _cc.x);
						MFPUWRITE(_lpPrm, FACE_Y, FCCRD, _cc.y);
						MFPUWRITE(_lpPrm, FACE_CX, FCCRD, _cc.cx);
						MFPUWRITE(_lpPrm, FACE_CY, FCCRD, _cc.cy);

						LinePaint( wFTU_, _lpFia->hdc );
						_ret = FIA_PROCESSEND;
					}

					free( _npLS );
					_lpFia->lpPrivate = NULL;
					break;
				}

				default:
					_ret = FIA_NOTPROCESSED;
			}
			return _ret;
		}

		case SCMD_EDIT:
		{
			WORD			_wMouseOn;
			LPFIA			_lpFia = (LPFIA)lParam_;
			NPLINESTRUCT	_npLS;
			POINT			_pt;

			ASSIGN_POINT( _pt, _lpFia->lParam )

			if( _lpFia->msg != WM_LBUTTONDOWN )
				return FALSE;

			_wMouseOn = _Line_SCMDMouseOnHandle( wFTU_, _lpFia->wSHSize, _pt );

			if( _wMouseOn == MOUSEON_NULL)	// can't locate it yet
				_wMouseOn = _Line_SCMDMouseOnBody( wFTU_, _lpFia->wPrecision, _pt );

			if( _wMouseOn == MOUSEON_NULL )
				return FALSE;

			_npLS = (NPLINESTRUCT)malloc( sizeof(LINESTRUCT) );
			if( !_npLS )
				return FALSE;

			((LPFIA)lParam_)->lpPrivate = _npLS;

			if(_wMouseOn == MOUSEON_BODY)
				_npLS->oc = 0;
			else	// on handle
			{
				_npLS->wHandle = _wMouseOn;
				_npLS->oc = 1;
			}

			return TRUE;
		}

		case SCMD_EDIT_INPUTMSG:
		{
			NPLINESTRUCT	_npLS;
			_npLS = (NPLINESTRUCT)( (LPFIA)lParam_ )->lpPrivate;

			if( _npLS->oc == 0 )
				return _Line_SCMDEditMSGMove(wFTU_, hwnd_, lParam_);
			else
				return _Line_SCMDEditMSGEdit(wFTU_, hwnd_, lParam_);
		}

		default:
			return DefStampBuild( wCmd_, wFTU_, hwnd_, wParam_, lParam_ );
	}

	return TRUE;
}

//...................................................................
//Description:
//...................................................................
static WORD _near _Line_SCMDMouseOnHandle( WRD wFTU_, WORD wSHSize_, POINT pt_ )
{
	XYCXCY		_cc;
	int			_x = pt_.x;
	int			_y = pt_.y;


	//get XYCXCY and transform for window coor
	GetFaceXycxcyInWinCoor( wFTU_, &_cc );

	if( (abs(_x-_cc.x) < wSHSize_) && (abs(_y-_cc.y) < wSHSize_) )
		return 0x8000;	//on handle 0, user type handle
	if( (abs(_x-(_cc.cx+_cc.x)) < wSHSize_) && (abs(_y-(_cc.cy+_cc.y)) < wSHSize_) )
		return 0x8001;	//on handle 1, user type handle

	return MOUSEON_NULL;
}

//...................................................................
//Description:
//...................................................................
static WORD _near _Line_SCMDMouseOnBody( WRD wFTU_, WORD wPrecision_, POINT pt_ )
{
	float	_x = pt_.x;
	float	_y = pt_.y;
	float	_hrz_dis, _ver_dis;
	XYCXCY	_cc;
	RECT	_rect;

	//get XYCXCY and transform for window coor
	GetFaceXycxcyInWinCoor( wFTU_, &_cc );

	_rect.left = _cc.x; 
	_rect.top = _cc.y;
	_rect.right = _cc.x + _cc.cx;
	_rect.bottom = _cc.y + _cc.cy;
	PropertyRect( &_rect );
	InflateRect(&_rect, wPrecision_, wPrecision_);	//add the select precision
	if( !PtInRect(&_rect, pt_) )
		return MOUSEON_NULL;

	if( (_cc.cx == 0) || (_cc.cy == 0) )
	{
		return MOUSEON_BODY;
	}

	//calculate HRZ and VER distance from point to line
	_hrz_dis = fabs( _x - (_y - _cc.y)*_cc.cx/_cc.cy - _cc.x );
	_ver_dis = fabs( _y - (_x - _cc.x)*_cc.cy/_cc.cx - _cc.y );
	if( (_hrz_dis < wPrecision_) || (_ver_dis < wPrecision_) )
		return MOUSEON_BODY;
	else
		return MOUSEON_NULL;
}

//...................................................................
//Description:
//...................................................................
static WORD _near _Line_SCMDMove(WRD wFTU_, HWND hwnd_, WORD wParam_, LONG lParam_)
{
	XYCXCY		_cc;
	LPPU		_lpPrm = FaceGetParameter( wFTU_ );
	HRGN		_hrgn;
	RECT		_rc;
	FCCRD		_delta_x, _delta_y;

	//get old region before changing parameter and invalidate it
	_hrgn = _LineGetRgn( wFTU_, wParam_ );
	InvalidateRgn(hwnd_, _hrgn, TRUE );
	DeleteObject( _hrgn );

	//calculate the deltaXY in face coor.
	_delta_x = (short)LOWORD(lParam_);
	_delta_y = (short)HIWORD(lParam_);
	WindowCoorToFaceCoor( wFTU_, &_delta_x, &_delta_y );

	//read the old position
	MFPUREAD(_lpPrm, FACE_X, FCCRD, _cc.x);
	MFPUREAD(_lpPrm, FACE_Y, FCCRD, _cc.y);

	_cc.x += _delta_x;
	_cc.y += _delta_y;

	//write new position
	MFPUWRITE(_lpPrm, FACE_X, FCCRD, _cc.x);
	MFPUWRITE(_lpPrm, FACE_Y, FCCRD, _cc.y);

	//invalidate new rgn
	_hrgn = _LineGetRgn( wFTU_, wParam_ );
	GetRgnBox( _hrgn, &_rc );
	InvalidateRgn(hwnd_, _hrgn, FALSE );
	DeleteObject( _hrgn );

	//write new face-rect
	FaceSetRect( wFTU_, &_rc );
	return TRUE;
}

static HRGN _near _LineGetRgn( WRD wFTU_, WORD wSHSize_ )
{
	int			_width;
	XYCXCY		_cc;
	LPPU		_lpPrm = FaceGetParameter(wFTU_);

	//get XYCXCY and transform for window coor
	GetFaceXycxcyInWinCoor( wFTU_, &_cc );

	MFPUREAD( _lpPrm, LINE_PENWIDTH, int, _width );

	if( FaceGetStatus(wFTU_) & FTUBIT_SELECT )
		_width = M_SQRT2 * max(_width, wSHSize_+1);

	if( (fabs(_cc.cx)<=wSHSize_) || (fabs(_cc.cy)<=wSHSize_) )
	{	//vertical line, horizontal line or small line
		RECT	_rect;

		_rect.left = _cc.x;
		_rect.top = _cc.y;
		_rect.right = _cc.cx + _cc.x;
		_rect.bottom = _cc.cy + _cc.y;
		PropertyRect( &_rect );
		InflateRect( &_rect, _width, _width );
		return CreateRectRgnIndirect( &_rect );
	}
	else
	{
		double z, /* l, */ zz, ll;		//(z, l) in polar coor
		double x, y, x0, y0, xx, yy;
		POINT	pt[4];

		ll = M_SQRT2 * _width;
		x0 = _cc.x;
		y0 = _cc.y;
		x = _cc.cx;
		y = _cc.cy;

		z = atan2( _cc.cy, _cc.cx );
		//l = hypot( _cc.cx, _cc.cy );

		zz = z - M_PI_4;
		xx = cos(zz)*ll;
		yy = sin(zz)*ll;
		pt[0].x = xx + x + x0;
		pt[0].y = yy + y + y0;

		zz = z + M_PI_4;
		xx = cos(zz)*ll;
		yy = sin(zz)*ll;
		pt[1].x = xx + x + x0;
		pt[1].y = yy + y + y0;

		z += M_PI;

		zz = z - M_PI_4;
		xx = cos(zz)*ll;
		yy = sin(zz)*ll;
		pt[2].x = xx + x0;
		pt[2].y = yy + y0;

		zz = z + M_PI_4;
		xx = cos(zz)*ll;
		yy = sin(zz)*ll;
		pt[3].x = xx + x0;
		pt[3].y = yy + y0;

		return CreatePolygonRgn( &(pt[0]), 4, WINDING );
	}
}


#pragma argsused
static WORD _near _LineToggleSelectHandle(WRD wFTU_, HWND hwnd_, WORD wSHSize_, HDC hdc_ )
{
	int		_old_mode;
	LPPU	_lpPrm = FaceGetParameter( wFTU_ );
	XYCXCY	_cc;
	HPEN	_hPen;

	_old_mode = SetROP2(hdc_, R2_NOT);
	SelectObject(hdc_, GetStockObject(BLACK_BRUSH));

	//get XYCXCY and transform for window coor
	MFPUREAD(_lpPrm, FACE_X, FCCRD, _cc.x);
	MFPUREAD(_lpPrm, FACE_Y, FCCRD, _cc.y);
	MFPUREAD(_lpPrm, FACE_CX, FCCRD, _cc.cx);
	MFPUREAD(_lpPrm, FACE_CY, FCCRD, _cc.cy);
	FaceCoorToWindowCoor( wFTU_, &_cc.x, &_cc.y );
	FaceCoorToWindowCoor( wFTU_, &_cc.cx, &_cc.cy );
	_cc.cx += _cc.x;
	_cc.cy += _cc.y;

	//start point handle
	Rectangle(hdc_, _cc.x - wSHSize_,
					_cc.y - wSHSize_,
					_cc.x + wSHSize_+1,
					_cc.y + wSHSize_+1);

	//end point handle
	Rectangle(hdc_, _cc.cx - wSHSize_-1,
					_cc.cy - wSHSize_,
					_cc.cx + wSHSize_,
					_cc.cy + wSHSize_+1);

	_hPen = SelectObject( hdc_, GetStockObject( BLACK_PEN ) );
	MoveTo( hdc_, _cc.cx-wSHSize_, _cc.cy-wSHSize_+1 );
	LineTo( hdc_, _cc.cx+wSHSize_-1, _cc.cy+wSHSize_ );
	MoveTo( hdc_, _cc.cx+wSHSize_-2, _cc.cy-wSHSize_+1 );
	LineTo( hdc_, _cc.cx-wSHSize_-1, _cc.cy+wSHSize_ );
	SelectObject( hdc_, _hPen );
	SetROP2( hdc_, _old_mode );

	return TRUE;
}

//...................................................................
//Description:
//...................................................................
static WORD _near _Line_SCMDEditMSGMove(WRD wFTU_, HWND hwnd_, LONG lParam_)
{
	LPFIA 			_lpFia = (LPFIA)lParam_;
	NPLINESTRUCT	_npLS = (NPLINESTRUCT)_lpFia->lpPrivate;
	XYCXCY			_cc;
	WORD			_ret = FIA_PROCESSED;

	switch( _lpFia->msg )
	{
		case WM_LBUTTONDOWN:
			//hide select handle
			_LineToggleSelectHandle(wFTU_, hwnd_, _lpFia->wSHSize, _lpFia->hdc );

			//initialize FIA.dtLast and FIA.dtPrimary
			_lpFia->dtLast.x = X(_lpFia->lParam);
			_lpFia->dtLast.y = Y(_lpFia->lParam);
			_lpFia->dtPrimary.x = X(_lpFia->lParam);
			_lpFia->dtPrimary.y = Y(_lpFia->lParam);

			//init. hdc
			_npLS->iOldMode = SetROP2( _lpFia->hdc, R2_NOT );
			_npLS->hOldPen = SelectObject( _lpFia->hdc, GetStockObject( BLACK_PEN ) );

			//get XYCXCY and transform for window coor
			GetFaceXycxcyInWinCoor( wFTU_, &_cc );

			MoveTo( _lpFia->hdc, _cc.x, _cc.y );
			LineTo( _lpFia->hdc, _cc.x+_cc.cx, _cc.y+_cc.cy );
			break;

		case WM_MOUSEMOVE:
		{
			int	_deltaX, _deltaY;

			//get XYCXCY and transform for window coor
			GetFaceXycxcyInWinCoor( wFTU_, &_cc );

			//clear old line
			_deltaX = _lpFia->dtLast.x - _lpFia->dtPrimary.x;
			_deltaY = _lpFia->dtLast.y - _lpFia->dtPrimary.y;
			MoveTo( _lpFia->hdc, _cc.x+_deltaX, _cc.y+_deltaY );
			LineTo( _lpFia->hdc, _cc.x+_cc.cx+_deltaX, _cc.y+_cc.cy+_deltaY );

			//set _lpFia->dtLast
			_lpFia->dtLast.x = X(_lpFia->lParam);
			_lpFia->dtLast.y = Y(_lpFia->lParam);

			//draw new line
			_deltaX = _lpFia->dtLast.x - _lpFia->dtPrimary.x;
			_deltaY = _lpFia->dtLast.y - _lpFia->dtPrimary.y;
			MoveTo( _lpFia->hdc, _cc.x+_deltaX, _cc.y+_deltaY );
			LineTo( _lpFia->hdc, _cc.x+_cc.cx+_deltaX, _cc.y+_cc.cy+_deltaY );
			break;
		}

		case WM_LBUTTONUP:
		{
			int	_deltaX, _deltaY;

			//get XYCXCY and transform for window coor
			GetFaceXycxcyInWinCoor( wFTU_, &_cc );

			//clear old line
			_deltaX = _lpFia->dtLast.x - _lpFia->dtPrimary.x;
			_deltaY = _lpFia->dtLast.y - _lpFia->dtPrimary.y;
			MoveTo( _lpFia->hdc, _cc.x+_deltaX, _cc.y+_deltaY );
			LineTo( _lpFia->hdc, _cc.x+_cc.cx+_deltaX, _cc.y+_cc.cy+_deltaY );

			_Line_SCMDMove(wFTU_, hwnd_, _lpFia->wSHSize, MAKELONG(_deltaX, _deltaY) );
			UpdateWindow( hwnd_ );

			//restore the settings of HDC
			SetROP2( _lpFia->hdc, _npLS->iOldMode );
			SelectObject( _lpFia->hdc, _npLS->hOldPen );

			free( _npLS );
			_lpFia->lpPrivate = NULL;

			_ret = FIA_PROCESSEND;
			break;
		}

		default:
			_ret = FIA_NOTPROCESSED;
			break;
	}
	return _ret;
}

//...................................................................
//Description:
//...................................................................
static WORD _near _Line_SCMDEditMSGEdit(WRD wFTU_, HWND hwnd_, LONG lParam_)
{
	WORD			_ret = FIA_PROCESSED;
	LPFIA 			_lpFia = (LPFIA)lParam_;
	NPLINESTRUCT	_npLS = (NPLINESTRUCT)_lpFia->lpPrivate;
	XYCXCY			_cc;

	switch( _lpFia->msg )
	{
		case WM_LBUTTONDOWN:
			//hide select handle
			_LineToggleSelectHandle(wFTU_, hwnd_, _lpFia->wSHSize, _lpFia->hdc );

			//initialize FIA.dtLast and FIA.dtPrimary
			_lpFia->dtLast.x = X(_lpFia->lParam);
			_lpFia->dtLast.y = Y(_lpFia->lParam);
			_lpFia->dtPrimary.x = X(_lpFia->lParam);
			_lpFia->dtPrimary.y = Y(_lpFia->lParam);

			//init. hdc
			_npLS->iOldMode = SetROP2( _lpFia->hdc, R2_NOT );
			_npLS->hOldPen = SelectObject( _lpFia->hdc, GetStockObject( BLACK_PEN ) );

			//get XYCXCY and transform for window coor
			GetFaceXycxcyInWinCoor( wFTU_, &_cc );

			MoveTo( _lpFia->hdc, _cc.x, _cc.y );
			LineTo( _lpFia->hdc, _cc.x+_cc.cx, _cc.y+_cc.cy );
			break;

		case WM_MOUSEMOVE:
		{
			int	_deltaX, _deltaY;

			//get XYCXCY and transform for window coor
			GetFaceXycxcyInWinCoor( wFTU_, &_cc );

			//clear old line
			_deltaX = _lpFia->dtLast.x - _lpFia->dtPrimary.x;
			_deltaY = _lpFia->dtLast.y - _lpFia->dtPrimary.y;

			if( _npLS->wHandle == 0x8000 )
			{
				MoveTo( _lpFia->hdc, _cc.x+_deltaX, _cc.y+_deltaY );
				LineTo( _lpFia->hdc, _cc.x+_cc.cx, _cc.y+_cc.cy );
			}
			else		//== 0x8001
			{
				MoveTo( _lpFia->hdc, _cc.x, _cc.y );
				LineTo( _lpFia->hdc, _cc.x+_cc.cx+_deltaX, _cc.y+_cc.cy+_deltaY );
			}

			//set _lpFia->dtLast
			_lpFia->dtLast.x = X(_lpFia->lParam);
			_lpFia->dtLast.y = Y(_lpFia->lParam);

			//draw new line
			_deltaX = _lpFia->dtLast.x - _lpFia->dtPrimary.x;
			_deltaY = _lpFia->dtLast.y - _lpFia->dtPrimary.y;
			if( _npLS->wHandle == 0x8000 )
			{
				MoveTo( _lpFia->hdc, _cc.x+_deltaX, _cc.y+_deltaY );
				LineTo( _lpFia->hdc, _cc.x+_cc.cx, _cc.y+_cc.cy );
			}
			else		//== 0x8001
			{
				MoveTo( _lpFia->hdc, _cc.x, _cc.y );
				LineTo( _lpFia->hdc, _cc.x+_cc.cx+_deltaX, _cc.y+_cc.cy+_deltaY );
			}
			break;
		}

		case WM_LBUTTONUP:
		{
			int			_deltaX, _deltaY;
			HRGN		_hrgnOld, _hrgnNew;
			RECT		_rc;

			//get XYCXCY and transform for window coor
			GetFaceXycxcyInWinCoor( wFTU_, &_cc );

			_hrgnOld = _LineGetRgn( wFTU_, _lpFia->wSHSize );

			_deltaX = _lpFia->dtLast.x - _lpFia->dtPrimary.x;
			_deltaY = _lpFia->dtLast.y - _lpFia->dtPrimary.y;
			if( _npLS->wHandle == 0x8000 )
			{
				LPPU	_lpPrm = FaceGetParameter( wFTU_ );
				
				//clear old line
				MoveTo( _lpFia->hdc, _cc.x+_deltaX, _cc.y+_deltaY );
				LineTo( _lpFia->hdc, _cc.x+_cc.cx, _cc.y+_cc.cy );

				_cc.x += _deltaX;
				_cc.y += _deltaY;
				_cc.cx = _cc.cx - _deltaX;
				_cc.cy = _cc.cy - _deltaY;

				WindowCoorToFaceCoor(wFTU_, &_cc.x, &_cc.y);
				WindowCoorToFaceCoor(wFTU_, &_cc.cx, &_cc.cy);

				MFPUWRITE(_lpPrm, FACE_X, FCCRD, _cc.x);
				MFPUWRITE(_lpPrm, FACE_Y, FCCRD, _cc.y);
				MFPUWRITE(_lpPrm, FACE_CX, FCCRD, _cc.cx);
				MFPUWRITE(_lpPrm, FACE_CY, FCCRD, _cc.cy);
			}
			else		//== 1
			{
				MoveTo( _lpFia->hdc, _cc.x, _cc.y );
				LineTo( _lpFia->hdc, _cc.x+_cc.cx+_deltaX, _cc.y+_cc.cy+_deltaY );
				WriteFaceCXCY(wFTU_, _cc.cx+_deltaX, _cc.cy+_deltaY);
			}

			_hrgnNew = _LineGetRgn( wFTU_, _lpFia->wSHSize );
			GetRgnBox( _hrgnNew, &_rc );
			FaceSetRect( wFTU_, &_rc );

			//restore the settings of HDC
			SetROP2( _lpFia->hdc, _npLS->iOldMode );
			SelectObject( _lpFia->hdc, _npLS->hOldPen );

			free( _npLS );
			_lpFia->lpPrivate = NULL;

			InvalidateRgn(hwnd_, _hrgnOld, TRUE );
			InvalidateRgn(hwnd_, _hrgnNew, FALSE );
			UpdateWindow( hwnd_ );
			DeleteObject( _hrgnOld );
			DeleteObject( _hrgnNew );

			_ret = FIA_PROCESSEND;
			break;
		}

		default:
			_ret = FIA_NOTPROCESSED;
			break;
	}
	return _ret;
}

//...................................................................
//Description:
//	The command is used at biuld-time.
//...................................................................
static WORD _near _Line_SCMDGetInvalidRgn( WRD wFTU_, HWND hwnd_, WORD wParam_, LONG lParam_ )
{
	LPGIR		_lpgir = (LPGIR)lParam_;
	RECT		_rcNew, _rcOld;
	HRGN		_rgnNew, _rgnOld;

	if( !wParam_ ) 	//no dirty
	{
		//need do nothing
	}
	else //position changed or parameter dirty
	{
		LPPU		_lpPrm = FaceGetParameter(wFTU_);
		int			_width;
		XYCXCY		_cc;

		MFPUREAD( _lpPrm, LINE_PENWIDTH, int, _width );

		GetFaceXycxcyInWinCoor(wFTU_, &_cc);
		_rcNew.left = _cc.x; 
		_rcNew.top = _cc.y;
		_rcNew.right = _cc.x + _cc.cx;
		_rcNew.bottom = _cc.y + _cc.cy;
		PropertyRect( &_rcNew );

		//adjust the rectangle by the line width 
		if( (_rcNew.right - _rcNew.left) <= _width )
			InflateRect( &_rcNew, _width/2 + 1, 0 );

		if( (_rcNew.bottom - _rcNew.top) <= _width )
			InflateRect( &_rcNew, 0, _width/2 + 1 );

		FaceGetRect( wFTU_, &_rcOld );
		FaceSetRect( wFTU_, &_rcNew );		//set new face rect

		_rgnOld = CreateRectRgnIndirect( &_rcOld );
		_rgnNew = CreateRectRgnIndirect( &_rcNew );
		CombineRgn( _lpgir->girDirtyRgn, _lpgir->girDirtyRgn, _rgnOld, RGN_OR);
		CombineRgn( _lpgir->girDirtyRgn, _lpgir->girDirtyRgn, _rgnNew, RGN_OR);
		DeleteObject( _rgnOld );
		DeleteObject( _rgnNew );
		FaceSetRedraw( wFTU_ );
	}
	return TRUE;
}
