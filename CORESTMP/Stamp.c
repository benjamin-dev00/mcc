//===================================================================
//
//Zheng zhikai	95-4-22
//===================================================================
#include "prehead.h"

#include "coreall.h"
#include "stmpdef.h"
//#include <stdlib.h>

//Rect() parameter order
#define RECT_BORDERSTYLE			(FACE_FIRSTPARAM + 0)
#define RECT_BORDERWIDTH   	    	(FACE_FIRSTPARAM + 1)
#define RECT_BORDERCLR				(FACE_FIRSTPARAM + 2)
#define RECT_BACKCLR				(FACE_FIRSTPARAM + 3)
//Rect() parameter type
#define RECT_BORDERSTYLE_PDT		PDT_int
#define RECT_BORDERWIDTH_PDT		PDT_DWORD
#define RECT_BORDERCLR_PDT			PDT_COLORREF
#define RECT_BACKCLR_PDT			PDT_COLORREF
//...................................................................
//...................................................................
_DLLEXP BOOL _DLLFUNC RectPaint(WRD wFTU_, HDC hdc_)
{
	COLORREF	_clrFill, _clrBorder;
	RECT		_rect;
	DWORD		_dwPenWidth;
	int			_nPenStyle;
	LPPU		_lpPrm;

	_lpPrm = FaceGetParameter(wFTU_);
	if( ! _lpPrm )
	{
		CORE_ASSERT( FALSE );
		return FALSE;
	}

	FaceXycxcyToRect(wFTU_, &_rect);

	MFPUREAD( _lpPrm, RECT_BORDERCLR, COLORREF, _clrBorder );
	MFPUREAD( _lpPrm, RECT_BACKCLR, COLORREF, _clrFill );
	MFPUREAD( _lpPrm, RECT_BORDERWIDTH, DWORD, _dwPenWidth );
	MFPUREAD( _lpPrm, RECT_BORDERSTYLE, int, _nPenStyle );

	CoreFillRect( hdc_, &_rect, _clrFill );
	CoreDrawBox( hdc_, &_rect, _nPenStyle, _dwPenWidth, _clrBorder );

	return TRUE;
}

//...................................................................
//...................................................................
_DLLEXP DWORD _DLLFUNC RectParam(PIC wCmd_, WRD wFTU_, WORD wNo_, LONG lParam_)
{
	static PDU _sPDU[] =
	{
		_PDU_DEFAULT,
		_ELE_PDU(RECT_BORDERSTYLE,	"BorderStyle",	VSOF_int_RECTBS,	PU_NORMAL, PRI_ALL ),
		_ELE_PDU(RECT_BORDERWIDTH,	"BorderWidth",	VSOF_DWORD_BWIDTH,	PU_NORMAL, PRI_ALL ),
		_ELE_PDU(RECT_BORDERCLR, 	"BorderColor",	VSOF_DWORD_CLR,		PU_NORMAL, PRI_ALL ),
		_ELE_PDU(RECT_BACKCLR, 		"BackColor",	VSOF_DWORD_CLR,		PU_NORMAL, PRI_ALL )
	};

	return _DefStampParam(wCmd_,
						  wFTU_,
						  wNo_,
						  lParam_,
						  sizeof(_sPDU)/sizeof(PDU),
						  IDS_RECT,
						  &(_sPDU[0]) );
}

//...................................................................
//...................................................................
_DLLEXP DWORD _DLLFUNC RectBuild(SCMD wCmd_, WRD wFTU_, HWND hwnd_, WORD wParam_, LONG lParam_)
{
	switch(wCmd_)
	{
		case SCMD_INIT:
		{
			LPFINIT		_lpFinit = (LPFINIT)lParam_;
			LPPU		_lpPrm;

			_lpPrm  = FaceGetParameter( wFTU_ );

			MFPUWRITE(_lpPrm, RECT_BORDERWIDTH, DWORD, _lpFinit->border_pen.lopnWidth.x);
			MFPUWRITE(_lpPrm, RECT_BORDERSTYLE, int, _lpFinit->border_pen.lopnStyle);
			MFPUWRITE(_lpPrm, RECT_BORDERCLR, COLORREF, _lpFinit->border_pen.lopnColor);
			MFPUWRITE(_lpPrm, RECT_BACKCLR, COLORREF, _lpFinit->back_clr);

			break;
		}

		default:
			return DefStampBuild( wCmd_, wFTU_, hwnd_, wParam_, lParam_ );
	}

	return TRUE;
}

//RectFrame() parameter order
#define RECTFRAME_BORDERSTYLE		(FACE_FIRSTPARAM + 0)
#define RECTFRAME_BORDERWIDTH		(FACE_FIRSTPARAM + 1)
#define RECTFRAME_BORDERCLR			(FACE_FIRSTPARAM + 2)
//RectFrame() parameter type
#define RECTFRAME_BORDERSTYLE_PDT		PDT_int
#define RECTFRAME_BORDERWIDTH_PDT		PDT_DWORD
#define RECTFRAME_BORDERCLR_PDT			PDT_COLORREF

//...................................................................
//...................................................................
_DLLEXP BOOL _DLLFUNC RectFramePaint(WRD wFTU_, HDC hdc_)
{
	COLORREF	_clr;
	DWORD		_width;
	int			_style;
	RECT		_rect;

	LPPU		_lpPrm;
	_lpPrm = FaceGetParameter(wFTU_);
	CORE_ASSERT(_lpPrm);

	FaceXycxcyToRect(wFTU_, &_rect);

	MFPUREAD( _lpPrm, RECTFRAME_BORDERCLR, COLORREF, _clr );
	MFPUREAD( _lpPrm, RECTFRAME_BORDERWIDTH, DWORD, _width );
	MFPUREAD( _lpPrm, RECTFRAME_BORDERSTYLE, int, _style );

	CoreDrawBox( hdc_, &_rect, _style, _width, _clr );

	return TRUE;
}

//...................................................................
//...................................................................
_DLLEXP DWORD _DLLFUNC RectFrameParam(PIC wCmd_, WRD wFTU_, WORD wNo_, LONG lParam_)
{
	static PDU _sPDU[] =
	{
		_PDU_DEFAULT,
		_ELE_PDU(RECTFRAME_BORDERSTYLE, "BorderStyle", VSOF_int_RECTBS,		PU_NORMAL, PRI_ALL ),
		_ELE_PDU(RECTFRAME_BORDERWIDTH, "BorderWidth", VSOF_DWORD_BWIDTH,	PU_NORMAL, PRI_ALL ),
		_ELE_PDU(RECTFRAME_BORDERCLR,   "BorderColor", VSOF_DWORD_CLR,		PU_NORMAL, PRI_ALL )
	};

	return _DefStampParam(wCmd_,
						  wFTU_,
						  wNo_,
						  lParam_,
						  sizeof(_sPDU)/sizeof(PDU),
						  IDS_RECTFRAME,
						  &(_sPDU[0]) );
}

///////////////////////
static HRGN _near _RectFrame_GetRgn(WRD wFTU_, HWND hwnd_, WORD wParam_, LONG lParam_);

//...................................................................
//...................................................................
_DLLEXP DWORD _DLLFUNC RectFrameBuild(SCMD wCmd_, WRD wFTU_, HWND hwnd_, WORD wParam_, LONG lParam_)
{
	//POINT			_point;
	//RECT			_rect;
	LPPU			_lpPrm;
	//XYCXCY			_xycxcy;
	LPFTU 			_lpFTU;

	_lpFTU = &((GetFaceTableInformation()->lpBase)[wFTU_]);
	_lpPrm  = _lpFTU->lpParam;

	switch(wCmd_)
	{
		case SCMD_INIT:
		{
			LPFINIT			_lpFinit = (LPFINIT)lParam_;

			MFPUWRITE(_lpPrm, RECTFRAME_BORDERWIDTH, DWORD, _lpFinit->border_pen.lopnWidth.x);
			MFPUWRITE(_lpPrm, RECTFRAME_BORDERSTYLE, int, _lpFinit->border_pen.lopnStyle);
			MFPUWRITE(_lpPrm, RECTFRAME_BORDERCLR, COLORREF, _lpFinit->border_pen.lopnColor);
			break;
		}

		case SCMD_MOUSEONBODY:
		{
			WORD 		_ret = MOUSEON_NULL;
			RECT		_rcNew;
			LPPU		_lpPrm = FaceGetParameter(wFTU_);
			UINT		_realwidth;

			COLORREF	_clr; 
			DWORD		_width; 
			int			_style;

			POINT	_pt;
			ASSIGN_POINT( _pt, lParam_ )

			FaceXycxcyToRect(wFTU_, &_rcNew);

			InflateRect(&_rcNew, wParam_, wParam_);
			if( PtInRect(&_rcNew, _pt) )
				_ret = MOUSEON_BODY;

			MFPUREAD( _lpPrm, RECTFRAME_BORDERCLR, COLORREF, _clr );
			MFPUREAD( _lpPrm, RECTFRAME_BORDERWIDTH, DWORD, _width );
			MFPUREAD( _lpPrm, RECTFRAME_BORDERSTYLE, int, _style );

			_realwidth = CoreGetPenWidth( _style, _clr, _width );

			InflateRect( &_rcNew, -2*wParam_ - _realwidth, -2*wParam_ - _realwidth );

			if( PtInRect( &_rcNew, _pt) )
				_ret = MOUSEON_NULL;

			return _ret;
		}

		case SCMD_GETREGION:
			return (DWORD)_RectFrame_GetRgn( wFTU_, hwnd_, wParam_, lParam_ );

		default:
			return DefStampBuild( wCmd_, wFTU_, hwnd_, wParam_, lParam_ );
	}

	return TRUE;
}

//...................................................................
//Note:
//	wParam_ is a delta to enlarge the result region
//...................................................................
#pragma argsused
static HRGN _near _RectFrame_GetRgn(WRD wFTU_, HWND hwnd_, WORD wParam_, LONG lParam_)
{
	DWORD		_width_org;
	WORD		_width;
	int			_style;
	RECT		_rect;
	HRGN		_hrgnDest, _hrgn1, _hrgn2;
	LPPU		_lpPrm;
	CCLR		_clr;

	_lpPrm = FaceGetParameter(wFTU_);
	CORE_ASSERT(_lpPrm);

	FaceXycxcyToRect(wFTU_, &_rect);
	MFPUREAD( _lpPrm, RECTFRAME_BORDERWIDTH, DWORD, _width_org );
	MFPUREAD( _lpPrm, RECTFRAME_BORDERSTYLE, int, _style );
	MFPUREAD( _lpPrm, RECTFRAME_BORDERCLR,	CCLR, _clr );
	_width = (WORD)CoreGetPenWidth( _style, _clr, _width_org );

	_hrgn1 = CreateRectRgn(_rect.left - wParam_,
						   _rect.top - wParam_,
						   _rect.right + wParam_,
						   _rect.bottom + wParam_);

	if(  (_rect.left + _width >= _rect.right - _width)
	   ||(_rect.top + _width >= _rect.bottom - _width) )
	{
		return _hrgn1;
	}
	else
	{
		_hrgn2 = CreateRectRgn(_rect.left + _width + wParam_,
							   _rect.top + _width + wParam_,
							   _rect.right - _width - wParam_,
							   _rect.bottom - _width - wParam_);

		_hrgnDest = CreateRectRgn(0, 0, 0, 0);

		CombineRgn(_hrgnDest, _hrgn1, _hrgn2, RGN_DIFF);
		DeleteObject(_hrgn1);
		DeleteObject(_hrgn2);
		return _hrgnDest;
	}
}


//Ellipse() parameter order
#define ELLIPSE_BORDERSTYLE				(FACE_FIRSTPARAM + 0)
#define ELLIPSE_BORDERWIDTH   	    	(FACE_FIRSTPARAM + 1)
#define ELLIPSE_BORDERCLR				(FACE_FIRSTPARAM + 2)
#define ELLIPSE_BACKCLR					(FACE_FIRSTPARAM + 3)
//Ellipse() parameter type
#define ELLIPSE_BORDERSTYLE_PDT		PDT_int
#define ELLIPSE_BORDERWIDTH_PDT		PDT_int
#define ELLIPSE_BORDERCLR_PDT			PDT_COLORREF
#define ELLIPSE_BACKCLR_PDT			PDT_COLORREF
//...................................................................
//...................................................................
_DLLEXP BOOL _DLLFUNC EllipsePaint(WRD wFTU_, HDC hdc_)
{
	COLORREF	_clrFill, _clrBorder;
	RECT		_rect;
	int			_dwPenWidth, _nPenStyle;
	HPEN		_hPen;
	HBRUSH		_hBrush;

	LPPU		_lpPrm;

	_lpPrm = FaceGetParameter(wFTU_);
	CORE_ASSERT(_lpPrm);

	FaceXycxcyToRect(wFTU_, &_rect);

	MFPUREAD( _lpPrm, ELLIPSE_BORDERCLR, COLORREF, _clrBorder );
	MFPUREAD( _lpPrm, ELLIPSE_BACKCLR, COLORREF, _clrFill );
	MFPUREAD( _lpPrm, ELLIPSE_BORDERWIDTH, int, _dwPenWidth );
	MFPUREAD( _lpPrm, ELLIPSE_BORDERSTYLE, int, _nPenStyle );

	_hPen = CoreCreatePen(_nPenStyle, _dwPenWidth, _clrBorder);
	_hBrush = CoreCreateBrush(_clrFill);

	_hPen = SelectObject(hdc_, _hPen);
	_hBrush = SelectObject(hdc_, _hBrush);

	Ellipse( hdc_, _rect.left, _rect.top, _rect.right, _rect.bottom );

	_hPen = SelectObject(hdc_, _hPen);
	_hBrush = SelectObject(hdc_, _hBrush);

	CoreDeleteBrush(_hBrush);
	CoreDeletePen(_hPen);

	return TRUE;
}

//...................................................................
//...................................................................
_DLLEXP DWORD _DLLFUNC EllipseParam(PIC wCmd_, WRD wFTU_, WORD wNo_, LONG lParam_)
{
	static PDU _sPDU[] =
	{
		_PDU_DEFAULT,
		_ELE_PDU(ELLIPSE_BORDERSTYLE,	"BorderStyle",	VSOF_int_GBS,		PU_NORMAL, PRI_ALL ),
		_ELE_PDU(ELLIPSE_BORDERWIDTH,	"BorderWidth",	VSOF_int,			PU_NORMAL, PRI_ALL ),
		_ELE_PDU(ELLIPSE_BORDERCLR, 	"BorderColor",	VSOF_DWORD_CLR,		PU_NORMAL, PRI_ALL ),
		_ELE_PDU(ELLIPSE_BACKCLR, 		"BackColor", 	VSOF_DWORD_CLR,		PU_NORMAL, PRI_ALL )
	};

	return _DefStampParam(wCmd_,
						  wFTU_,
						  wNo_,
						  lParam_,
						  sizeof(_sPDU)/sizeof(PDU),
						  IDS_ELLIPSE,
						  &(_sPDU[0]) );
}

//...................................................................
//...................................................................
_DLLEXP DWORD _DLLFUNC EllipseBuild(SCMD wCmd_, WRD wFTU_, HWND hwnd_, WORD wParam_, LONG lParam_)
{

	switch(wCmd_)
	{
		case SCMD_INIT:
		{
			LPFINIT	_lpFinit = (LPFINIT)lParam_;
			LPPU			_lpPrm;

			_lpPrm  = FaceGetParameter( wFTU_ );

			MFPUWRITE(_lpPrm, ELLIPSE_BORDERWIDTH, int, _lpFinit->border_pen.lopnWidth.x);
			MFPUWRITE(_lpPrm, ELLIPSE_BORDERSTYLE, int, _lpFinit->border_pen.lopnStyle);
			MFPUWRITE(_lpPrm, ELLIPSE_BORDERCLR, COLORREF, _lpFinit->border_pen.lopnColor);
			MFPUWRITE(_lpPrm, ELLIPSE_BACKCLR, COLORREF, _lpFinit->back_clr);

			break;
		}

		case SCMD_MOUSEONBODY:
		{
			WORD	_ret;
			HRGN _hrgn = (HRGN)EllipseBuild(SCMD_GETREGION, wFTU_, hwnd_, wParam_, 0);

			if( PtInRegion( _hrgn, X(lParam_), Y(lParam_) ) )
				_ret = MOUSEON_BODY;
			else
				_ret = MOUSEON_NULL;

			DeleteObject( _hrgn );
			return _ret;
		}

		case SCMD_GETREGION:
		{
			RECT		_rect;
			//int			_nWidth;

			//LPPU		_lpPrm = FaceGetParameter(wFTU_);

			FaceXycxcyToRect(wFTU_, &_rect);

			if( IsFaceSelect(wFTU_) && (!CoreIsRunTime()) )
				InflateRect( &_rect, wParam_, wParam_ );

			return (WORD)CreateEllipticRgnIndirect( &_rect );
		}

		default:
			return DefStampBuild( wCmd_, wFTU_, hwnd_, wParam_, lParam_ );
	}

	return TRUE;
}

//EllipseFrame() parameter order
#define ELLIPSEFRAME_BORDERSTYLE		(FACE_FIRSTPARAM + 0)
#define ELLIPSEFRAME_BORDERWIDTH		(FACE_FIRSTPARAM + 1)
#define ELLIPSEFRAME_BORDERCLR			(FACE_FIRSTPARAM + 2)
//EllipseFrame() parameter type
#define ELLIPSEFRAME_BORDERSTYLE_PDT	PDT_int
#define ELLIPSEFRAME_BORDERWIDTH_PDT	PDT_int
#define ELLIPSEFRAME_BORDERCLR_PDT		PDT_COLORREF

//...................................................................
//...................................................................
_DLLEXP BOOL _DLLFUNC EllipseFramePaint(WRD wFTU_, HDC hdc_)
{
	COLORREF	_clrBorder;
	RECT		_rect;
	int			_dwPenWidth, _nPenStyle;
	HPEN		_hPen;
	HBRUSH		_hBrush;
	LPPU		_lpPrm;

	_lpPrm = FaceGetParameter(wFTU_);
	CORE_ASSERT(_lpPrm);

	FaceXycxcyToRect(wFTU_, &_rect);

	MFPUREAD( _lpPrm, ELLIPSEFRAME_BORDERCLR, COLORREF, _clrBorder );
	MFPUREAD( _lpPrm, ELLIPSEFRAME_BORDERWIDTH, int, _dwPenWidth );
	MFPUREAD( _lpPrm, ELLIPSEFRAME_BORDERSTYLE, int, _nPenStyle );

	_hPen = CoreCreatePen(_nPenStyle, _dwPenWidth, _clrBorder);
	_hBrush = GetStockObject( NULL_BRUSH );

	_hPen = SelectObject(hdc_, _hPen);
	_hBrush = SelectObject(hdc_, _hBrush);

	Ellipse( hdc_, _rect.left, _rect.top, _rect.right, _rect.bottom );

	_hPen = SelectObject(hdc_, _hPen);
	CoreDeletePen(_hPen);

	return TRUE;
}

//...................................................................
//...................................................................
_DLLEXP DWORD _DLLFUNC EllipseFrameParam(PIC wCmd_, WRD wFTU_, WORD wNo_, LONG lParam_)
{
	static PDU _sPDU[] =
	{
		_PDU_DEFAULT,
		_ELE_PDU(ELLIPSEFRAME_BORDERSTYLE, "BorderStyle", 	VSOF_int_GBS,		PU_NORMAL, PRI_ALL ),
		_ELE_PDU(ELLIPSEFRAME_BORDERWIDTH, "BorderWidth", 	VSOF_int,		PU_NORMAL, PRI_ALL ),
		_ELE_PDU(ELLIPSEFRAME_BORDERCLR, 	"BorderColor",	VSOF_DWORD_CLR,	PU_NORMAL, PRI_ALL )
	};

	return _DefStampParam(wCmd_,
						  wFTU_,
						  wNo_,
						  lParam_,
						  sizeof(_sPDU)/sizeof(PDU),
						  IDS_ELLIPSEFRAME,
						  &(_sPDU[0]) );
}

////////////////////////
static HRGN _near _EllipseFrame_GetRgn(WRD wFTU_, HWND hwnd_, WORD wParam_, LONG lParam_);

//...................................................................
//...................................................................
_DLLEXP DWORD _DLLFUNC EllipseFrameBuild(SCMD wCmd_, WRD wFTU_, HWND hwnd_, WORD wParam_, LONG lParam_)
{
	switch(wCmd_)
	{
		case SCMD_INIT:
		{
			LPFINIT	_lpFinit = (LPFINIT)lParam_;
			LPPU			_lpPrm;

			_lpPrm  = FaceGetParameter( wFTU_ );

			MFPUWRITE(_lpPrm, ELLIPSEFRAME_BORDERWIDTH, int, _lpFinit->border_pen.lopnWidth.x);
			MFPUWRITE(_lpPrm, ELLIPSEFRAME_BORDERSTYLE, int, _lpFinit->border_pen.lopnStyle);
			MFPUWRITE(_lpPrm, ELLIPSEFRAME_BORDERCLR, COLORREF, _lpFinit->border_pen.lopnColor);

			break;
		}

		case SCMD_MOUSEONBODY:	
		{
			WORD	_ret;
			HRGN _hrgn = (HRGN)EllipseFrameBuild(SCMD_GETREGION, wFTU_, hwnd_, wParam_, 0);

			if( PtInRegion( _hrgn, X(lParam_), Y(lParam_) ) )
				_ret = MOUSEON_BODY;
			else
				_ret = MOUSEON_NULL;

			DeleteObject( _hrgn );
			return _ret;
		}

		case SCMD_GETREGION:
			return (DWORD)_EllipseFrame_GetRgn(wFTU_, hwnd_, wParam_, lParam_);

		default:
			return DefStampBuild( wCmd_, wFTU_, hwnd_, wParam_, lParam_ );
	}

	return TRUE;
}

//...................................................................
//...................................................................
#pragma argsused
static HRGN _near _EllipseFrame_GetRgn(WRD wFTU_, HWND hwnd_, WORD wParam_, LONG lParam_)
{
	RECT		_rect;
	int			_nWidth;
	HRGN		_hRgn, _hRgn1, _hRgn2;

	LPPU		_lpPrm = FaceGetParameter(wFTU_);

	FaceXycxcyToRect(wFTU_, &_rect);
	MFPUREAD( _lpPrm, ELLIPSEFRAME_BORDERWIDTH, int, _nWidth );

	InflateRect( &_rect, wParam_, wParam_ );
	_hRgn1 = CreateEllipticRgnIndirect( &_rect );

	InflateRect(&_rect, -1*_nWidth + -2*wParam_, -1*_nWidth + -2*wParam_ );
	_hRgn2 = CreateEllipticRgnIndirect( &_rect );

	_hRgn = CreateRectRgn(0, 0, 0, 0);
	CombineRgn(_hRgn, _hRgn1, _hRgn2, RGN_DIFF);

	DeleteObject( _hRgn1 );
	DeleteObject( _hRgn2 );

	return _hRgn;
}

//RoundRect() parameter order
#define RNDRCT_BORDERSTYLE			(FACE_FIRSTPARAM + 0)
#define RNDRCT_BORDERWIDTH   	    (FACE_FIRSTPARAM + 1)
#define RNDRCT_BORDERCLR			(FACE_FIRSTPARAM + 2)
#define RNDRCT_BACKCLR				(FACE_FIRSTPARAM + 3)
#define RNDRCT_ELLWIDTH				(FACE_FIRSTPARAM + 4)
#define RNDRCT_ELLHEIGHT			(FACE_FIRSTPARAM + 5)
//RoundRect() parameter type
#define RNDRCT_BORDERSTYLE_PDT		PDT_int
#define RNDRCT_BORDERWIDTH_PDT		PDT_DWORD
#define RNDRCT_BORDERCLR_PDT		PDT_COLORREF
#define RNDRCT_BACKCLR_PDT			PDT_COLORREF
#define RNDRCT_ELLWIDTH_PDT			PDT_int
#define RNDRCT_ELLHEIGHT_PDT		PDT_int

//...................................................................
//...................................................................
_DLLEXP BOOL _DLLFUNC RndRctPaint(WRD wFTU_, HDC hdc_)
{
	COLORREF	_clrFill, _clrBorder;
	RECT		_rect;
	int			_dwPenWidth, _nPenStyle;
	int			_nEllWidth, _nEllHeight;
	HPEN		_hPen;
	HBRUSH		_hBrush;

	LPPU		_lpPrm;

	_lpPrm = FaceGetParameter(wFTU_);
	CORE_ASSERT(_lpPrm);

	FaceXycxcyToRect(wFTU_, &_rect);

	MFPUREAD( _lpPrm, RNDRCT_BORDERCLR, COLORREF, _clrBorder );
	MFPUREAD( _lpPrm, RNDRCT_BACKCLR, COLORREF, _clrFill );
	MFPUREAD( _lpPrm, RNDRCT_BORDERWIDTH, DWORD, _dwPenWidth );
	MFPUREAD( _lpPrm, RNDRCT_BORDERSTYLE, int, _nPenStyle );
	MFPUREAD( _lpPrm, RNDRCT_ELLWIDTH, int, _nEllWidth );
	MFPUREAD( _lpPrm, RNDRCT_ELLHEIGHT, int, _nEllHeight );

	_hPen = CoreCreatePen(_nPenStyle, _dwPenWidth, _clrBorder);
	_hBrush = CoreCreateBrush(_clrFill);

	_hPen = SelectObject(hdc_, _hPen);
	_hBrush = SelectObject(hdc_, _hBrush);

	RoundRect( hdc_, _rect.left, _rect.top, _rect.right, _rect.bottom,
			   _nEllWidth, _nEllHeight );

	_hPen = SelectObject(hdc_, _hPen);
	_hBrush = SelectObject(hdc_, _hBrush);

	CoreDeleteBrush(_hBrush);
	CoreDeletePen(_hPen);

	return TRUE;
}

//...................................................................
//...................................................................
_DLLEXP DWORD _DLLFUNC RndRctParam(PIC wCmd_, WRD wFTU_, WORD wNo_, LONG lParam_)
{
	static PDU _sPDU[] =
	{
		_PDU_DEFAULT,
		_ELE_PDU(RNDRCT_BORDERSTYLE,	"BorderStyle",	VSOF_int_GBS,		PU_NORMAL, PRI_ALL ),
		_ELE_PDU(RNDRCT_BORDERWIDTH,	"BorderWidth", 	VSOF_int,			PU_NORMAL, PRI_ALL ),
		_ELE_PDU(RNDRCT_BORDERCLR,		"BorderColor",	VSOF_DWORD_CLR,		PU_NORMAL, PRI_ALL ),
		_ELE_PDU(RNDRCT_BACKCLR, 		"BackColor",	VSOF_DWORD_CLR,		PU_NORMAL, PRI_ALL ),
		_ELE_PDU(RNDRCT_ELLWIDTH,		"EllWidth", 	VSOF_int,			PU_NORMAL, PRI_ALL ),
		_ELE_PDU(RNDRCT_ELLHEIGHT,		"EllHeight", 	VSOF_int,			PU_NORMAL, PRI_ALL ),
	};

	return _DefStampParam(wCmd_,
						  wFTU_,
						  wNo_,
						  lParam_,
						  sizeof(_sPDU)/sizeof(PDU),
						  IDS_ROUNDRECT,
						  &(_sPDU[0]) );
}

//...................................................................
//...................................................................
_DLLEXP DWORD _DLLFUNC RndRctBuild(SCMD wCmd_, WRD wFTU_, HWND hwnd_, WORD wParam_, LONG lParam_)
{

	switch(wCmd_)
	{
		case SCMD_INIT:
		{
			LPFINIT	_lpFinit = (LPFINIT)lParam_;
			LPPU			_lpPrm;

			_lpPrm  = FaceGetParameter( wFTU_ );

			MFPUWRITE(_lpPrm, RNDRCT_BORDERWIDTH, DWORD, _lpFinit->border_pen.lopnWidth.x);
			MFPUWRITE(_lpPrm, RNDRCT_BORDERSTYLE, int, _lpFinit->border_pen.lopnStyle);
			MFPUWRITE(_lpPrm, RNDRCT_BORDERCLR, COLORREF, _lpFinit->border_pen.lopnColor);
			MFPUWRITE(_lpPrm, RNDRCT_BACKCLR, COLORREF, _lpFinit->back_clr);

			MFPUWRITE(_lpPrm, RNDRCT_ELLWIDTH, int, 20);
			MFPUWRITE(_lpPrm, RNDRCT_ELLHEIGHT, int, 20);

			break;
		}

		case SCMD_MOUSEONBODY:
		{
			WORD	_ret;
			HRGN _hrgn = (HRGN)RndRctBuild(SCMD_GETREGION, wFTU_, hwnd_, wParam_, 0L);

			if( PtInRegion( _hrgn, X(lParam_), Y(lParam_) ) )
				_ret = MOUSEON_BODY;
			else
				_ret = MOUSEON_NULL;

			DeleteObject( _hrgn );
			return _ret;
		}

		case SCMD_GETREGION:
		{
			RECT		_rc;
			int			_nEllWidth, _nEllHeight;
			LPPU		_lpPrm = FaceGetParameter(wFTU_);

			FaceXycxcyToRect(wFTU_, &_rc);
			MFPUREAD( _lpPrm, RNDRCT_ELLWIDTH, int, _nEllWidth );
			MFPUREAD( _lpPrm, RNDRCT_ELLHEIGHT, int, _nEllHeight );

			InflateRect(&_rc, wParam_, wParam_ );
			return (WORD)CreateRoundRectRgn(_rc.left, _rc.top, _rc.right, _rc.bottom,
											_nEllWidth, _nEllHeight);
		}

		default:
			return DefStampBuild( wCmd_, wFTU_, hwnd_, wParam_, lParam_ );
	}

	return TRUE;
}

//RoundRectFrame() parameter order
#define RNDRCTFRAME_BORDERSTYLE		(FACE_FIRSTPARAM + 0)
#define RNDRCTFRAME_BORDERWIDTH		(FACE_FIRSTPARAM + 1)
#define RNDRCTFRAME_BORDERCLR		(FACE_FIRSTPARAM + 2)
#define RNDRCTFRAME_ELLWIDTH		(FACE_FIRSTPARAM + 3)
#define RNDRCTFRAME_ELLHEIGHT		(FACE_FIRSTPARAM + 4)
//RoundRectFrame() parameter type
#define RNDRCTFRAME_BORDERSTYLE_PDT	PDT_int
#define RNDRCTFRAME_BORDERWIDTH_PDT	PDT_DWORD
#define RNDRCTFRAME_BORDERCLR_PDT	PDT_COLORREF
#define RNDRCTFRAME_ELLWIDTH_PDT	PDT_int
#define RNDRCTFRAME_ELLHEIGHT_PDT	PDT_int

//...................................................................
//...................................................................
_DLLEXP BOOL _DLLFUNC RndRctFramePaint(WRD wFTU_, HDC hdc_)
{
	COLORREF	_clrBorder;
	RECT		_rect;
	int			_dwPenWidth, _nPenStyle;
	int			_nEllWidth, _nEllHeight;
	HPEN		_hPen;
	HBRUSH		_hBrush;
	LPPU		_lpPrm;
	BOOL		_ret;

	_lpPrm = FaceGetParameter(wFTU_);
	CORE_ASSERT(_lpPrm);

	FaceXycxcyToRect(wFTU_, &_rect);

	MFPUREAD( _lpPrm, RNDRCTFRAME_BORDERCLR, COLORREF, _clrBorder );
	MFPUREAD( _lpPrm, RNDRCTFRAME_BORDERWIDTH, DWORD, _dwPenWidth );
	MFPUREAD( _lpPrm, RNDRCTFRAME_BORDERSTYLE, int, _nPenStyle );
	MFPUREAD( _lpPrm, RNDRCTFRAME_ELLWIDTH, int, _nEllWidth );
	MFPUREAD( _lpPrm, RNDRCTFRAME_ELLHEIGHT, int, _nEllHeight );

	_hPen = CoreCreatePen(_nPenStyle, _dwPenWidth, _clrBorder);
	_hBrush = GetStockObject( NULL_BRUSH );

	_hPen = SelectObject(hdc_, _hPen);
	_hBrush = SelectObject(hdc_, _hBrush);

	_ret = RoundRect( hdc_, _rect.left, _rect.top, _rect.right, _rect.bottom,
			   _nEllWidth, _nEllHeight );

	_hPen = SelectObject(hdc_, _hPen);
	CoreDeletePen(_hPen);

	return _ret;
}

//...................................................................
//...................................................................
_DLLEXP DWORD _DLLFUNC RndRctFrameParam(PIC wCmd_, WRD wFTU_, WORD wNo_, LONG lParam_)
{
	static PDU _sPDU[] =
	{
		_PDU_DEFAULT,
		_ELE_PDU(RNDRCTFRAME_BORDERSTYLE,	"BorderStyle",	VSOF_int_GBS,		PU_NORMAL, PRI_ALL ),
		_ELE_PDU(RNDRCTFRAME_BORDERWIDTH,	"BorderWidth",	VSOF_int,			PU_NORMAL, PRI_ALL ),
		_ELE_PDU(RNDRCTFRAME_BORDERCLR, 	"BorderColor",	VSOF_DWORD_CLR,		PU_NORMAL, PRI_ALL ),
		_ELE_PDU(RNDRCTFRAME_ELLWIDTH,		"EllWidth",		VSOF_int,			PU_NORMAL, PRI_ALL ),
		_ELE_PDU(RNDRCTFRAME_ELLHEIGHT,		"EllHeight",	VSOF_int,			PU_NORMAL, PRI_ALL )
	};

	return _DefStampParam(wCmd_,
						  wFTU_,
						  wNo_,
						  lParam_,
						  sizeof(_sPDU)/sizeof(PDU),
						  IDS_ROUNDRECTFRAME,
						  &(_sPDU[0]) );
}

///////////////////////
static HRGN _near _RndRctFrame_GetRgn( WRD wFTU_, HWND hwnd_, WPARAM wParam_, LPARAM lParam_ );
//...................................................................
//...................................................................
_DLLEXP DWORD _DLLFUNC RndRctFrameBuild(SCMD wCmd_, WRD wFTU_, HWND hwnd_, WORD wParam_, LONG lParam_)
{

	switch(wCmd_)
	{
		case SCMD_INIT:
		{
			LPFINIT	_lpFinit = (LPFINIT)lParam_;
			LPPU			_lpPrm;

			_lpPrm  = FaceGetParameter( wFTU_ );

			MFPUWRITE(_lpPrm, RNDRCTFRAME_BORDERWIDTH, DWORD, _lpFinit->border_pen.lopnWidth.x);
			MFPUWRITE(_lpPrm, RNDRCTFRAME_BORDERSTYLE, int, _lpFinit->border_pen.lopnStyle);
			MFPUWRITE(_lpPrm, RNDRCTFRAME_BORDERCLR, COLORREF, _lpFinit->border_pen.lopnColor);

			MFPUWRITE(_lpPrm, RNDRCTFRAME_ELLWIDTH, int, 20);
			MFPUWRITE(_lpPrm, RNDRCTFRAME_ELLHEIGHT, int, 20);

			break;
		}

		case SCMD_MOUSEONBODY:
		{
			WORD	_ret;
			HRGN _hrgn = (HRGN)RndRctFrameBuild(SCMD_GETREGION, wFTU_, hwnd_, wParam_, 0L);

			if( PtInRegion( _hrgn, X(lParam_), Y(lParam_) ) )
				_ret = MOUSEON_BODY;
			else
				_ret = MOUSEON_NULL;

			DeleteObject( _hrgn );
			return _ret;
		}

		case SCMD_GETREGION:
			return _RndRctFrame_GetRgn(wFTU_, hwnd_, wParam_, lParam_);

		default:
			return DefStampBuild( wCmd_, wFTU_, hwnd_, wParam_, lParam_ );
	}

	return TRUE;
}

#pragma argsused
static HRGN _near _RndRctFrame_GetRgn( WRD wFTU_, HWND hwnd_, WPARAM wParam_, LPARAM lParam_ )
{
	RECT		_rc;
	DWORD		_nWidth;
	int			_nEllWidth, _nEllHeight;
	int			_delta;
	HRGN		_hRgn, _hRgn1, _hRgn2;

	LPPU		_lpPrm = FaceGetParameter(wFTU_);

	FaceXycxcyToRect(wFTU_, &_rc);
	MFPUREAD( _lpPrm, RNDRCTFRAME_BORDERWIDTH, DWORD, _nWidth );
	MFPUREAD( _lpPrm, RNDRCTFRAME_ELLWIDTH, int, _nEllWidth );
	MFPUREAD( _lpPrm, RNDRCTFRAME_ELLHEIGHT, int, _nEllHeight );
	_nWidth &= 0xFFFF;

	InflateRect(&_rc, wParam_, wParam_);
	_hRgn1 = CreateRoundRectRgn(_rc.left, _rc.top, _rc.right, _rc.bottom,
								_nEllWidth, _nEllHeight);

	_delta = wParam_ > _nWidth ? wParam_ : _nWidth;
	InflateRect( &_rc, -1*(_delta + wParam_), -1*(_delta + wParam_) );
	_nEllWidth -= _nWidth;
	_nEllHeight -= _nWidth;
	_hRgn2 = CreateRoundRectRgn(_rc.left, _rc.top, _rc.right, _rc.bottom,
								_nEllWidth, _nEllHeight);

	_hRgn = CreateRectRgn(0, 0, 0, 0);
	CombineRgn(_hRgn, _hRgn1, _hRgn2, RGN_DIFF);

	DeleteObject( _hRgn1 );
	DeleteObject( _hRgn2 );

	return _hRgn;
}

