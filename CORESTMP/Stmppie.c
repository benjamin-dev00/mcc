//===================================================================
//
//Zheng zhikai	96-3-3
//===================================================================
#include "prehead.h"

#include "coreall.h"
#include <math.h>

#pragma warning( disable : 4244 )	//'conversion' conversion from 'type1' to 'type2', possible loss of data
#pragma warning( disable : 4305 )	//truncation from 'const double ' to 'float '

#include "stmpdef.h"

//Arc() parameter order
#define ARC_PENSTYLE				(FACE_FIRSTPARAM + 0)
#define ARC_PENWIDTH				(FACE_FIRSTPARAM + 1)
#define ARC_PENCLR					(FACE_FIRSTPARAM + 2)
#define ARC_STARTANGLE				(FACE_FIRSTPARAM + 3)
#define ARC_ENDANGLE				(FACE_FIRSTPARAM + 4)

//Arc() parameter type
#define ARC_PENSTYLE_PDT			PDT_int
#define ARC_PENWIDTH_PDT			PDT_int
#define ARC_PENCLR_PDT				PDT_COLORREF
#define ARC_STARTANGLE_PDT          PDT_REAL
#define ARC_ENDANGLE_PDT			PDT_REAL

static void _near CaculateArcRect( LPRECT lprcFace_, LPRECT lprc_, REAL aStart_, REAL aEnd_ );

//...................................................................
//...................................................................
_DLLEXP BOOL _DLLFUNC ArcPaint(WRD wFTU_, HDC hdc_)
{
	COLORREF	_clrBorder;
	RECT		_rc;
	REAL		_aStart, _aEnd;
	int			_nPenWidth, _nPenStyle;
	int			_xStart, _yStart, _xEnd, _yEnd, _x0, _y0;
	HPEN		_hPen;

	LPPU		_lpPrm;

	_lpPrm = FaceGetParameter(wFTU_);
	CORE_ASSERT(_lpPrm);

	FaceXycxcyToRect(wFTU_, &_rc);

	//read start angle and end angle, then transform to radians
	MFPUREAD( _lpPrm, ARC_STARTANGLE, REAL, _aStart);
	MFPUREAD( _lpPrm, ARC_ENDANGLE, REAL, _aEnd);
	_aStart *= M_PI/180;
	_aEnd *= M_PI/180;

	_x0 = (_rc.left+_rc.right)/2;
	_y0 = (_rc.top+_rc.bottom)/2;
	_xStart = 100 * cos(_aStart)+_x0;
	_yStart = 100 * sin(_aStart)+_y0;
	_xEnd = 100 * cos(_aEnd)+_x0;
	_yEnd = 100 * sin(_aEnd)+_y0;

	MFPUREAD( _lpPrm, ARC_PENCLR, COLORREF, _clrBorder );
	MFPUREAD( _lpPrm, ARC_PENWIDTH, int, _nPenWidth );
	MFPUREAD( _lpPrm, ARC_PENSTYLE, int, _nPenStyle );

	_hPen = CoreCreatePen(_nPenStyle, _nPenWidth, _clrBorder);
	_hPen = SelectObject(hdc_, _hPen);

	Arc(hdc_,
		_rc.left, _rc.top, _rc.right, _rc.bottom,
		_xStart, _yStart, _xEnd, _yEnd );

	_hPen = SelectObject(hdc_, _hPen);
	CoreDeletePen(_hPen);

	return TRUE;
}

//...................................................................
//...................................................................
_DLLEXP DWORD _DLLFUNC ArcParam(PIC wCmd_, WRD wFTU_, WORD wNo_, LONG lParam_)
{
	static PDU _sPDU[] =
	{
		_PDU_DEFAULT,
		_ELE_PDU(ARC_PENSTYLE,	"LineStyle",	VSOF_int,		PU_NORMAL, PRI_ALL ),
		_ELE_PDU(ARC_PENWIDTH,	"LineWidth",	VSOF_int,		PU_NORMAL, PRI_ALL ),
		_ELE_PDU(ARC_PENCLR, 	"LineColor",	VSOF_DWORD_CLR,	PU_NORMAL, PRI_ALL ),
		_ELE_PDU(ARC_STARTANGLE,"StartAngle",	VSOF_REAL,		PU_NORMAL, PRI_ALL ),
		_ELE_PDU(ARC_ENDANGLE, 	"EndAngle",		VSOF_REAL,		PU_NORMAL, PRI_ALL )
	};

	return _DefStampParam(wCmd_,
						  wFTU_,
						  wNo_,
						  lParam_,
						  sizeof(_sPDU)/sizeof(PDU),
						  IDS_ARC,
						  &(_sPDU[0]) );
}

//...................................................................
//...................................................................
_DLLEXP DWORD _DLLFUNC ArcBuild(SCMD wCmd_, WRD wFTU_, HWND hwnd_, WORD wParam_, LONG lParam_)
{
	switch(wCmd_)
	{
		case SCMD_INIT:
		{
			LPFINIT	_lpFinit = (LPFINIT)lParam_;
			LPPU			_lpPrm;

			_lpPrm  = FaceGetParameter( wFTU_ );

			MFPUWRITE(_lpPrm, ARC_PENWIDTH, int, _lpFinit->line_pen.lopnWidth.x);
			MFPUWRITE(_lpPrm, ARC_PENSTYLE, int, _lpFinit->line_pen.lopnStyle);
			MFPUWRITE(_lpPrm, ARC_PENCLR, COLORREF, _lpFinit->line_pen.lopnColor);

			MFPUWRITE(_lpPrm, ARC_STARTANGLE, REAL, 0);
			MFPUWRITE(_lpPrm, ARC_ENDANGLE, REAL, 180);

			break;
		}

		case SCMD_MOUSEONBODY:
		{
			WORD	_ret;
			HRGN _hrgn = (HRGN)ArcBuild(SCMD_GETREGION, wFTU_, hwnd_, wParam_, 0L);

			HDC _hdc = GetDC( hwnd_ );
			FillRgn(_hdc, _hrgn, GetStockObject(GRAY_BRUSH) );
			ReleaseDC( hwnd_, _hdc );

			//note:POINT  
			if( PtInRegion( _hrgn, (short)LOWORD(lParam_), (short)HIWORD(lParam_) ) )
				_ret = MOUSEON_BODY;
			else
			{
				_ret = MOUSEON_NULL;
			}

			DeleteObject( _hrgn );
			return _ret;
		}

		case SCMD_GETREGION:
		{
			REAL		_aStart, _aEnd;
			RECT		_rc, _rcFace;
			int			_nWidth;
			HRGN		_hRgn, _hRgnDiff, _hRgn1, _hRgn2, _hRgnRct;

			LPPU		_lpPrm = FaceGetParameter(wFTU_);

			FaceXycxcyToRect(wFTU_, &_rc);
			MFPUREAD( _lpPrm, ARC_PENWIDTH, int, _nWidth );
			//read start angle and end angle, then transform to radians
			MFPUREAD( _lpPrm, ARC_STARTANGLE, REAL, _aStart);
			MFPUREAD( _lpPrm, ARC_ENDANGLE, REAL, _aEnd);
			_aStart *= M_PI/180;
			_aEnd *= M_PI/180;

			_nWidth += wParam_;

			CaculateArcRect( &_rcFace, &_rc, _aStart, _aEnd );
			InflateRect(&_rcFace, _nWidth, _nWidth);
			_hRgnRct = CreateRectRgnIndirect( &_rcFace );

			InflateRect(&_rc, _nWidth, _nWidth);
			_hRgn1 = CreateEllipticRgnIndirect( &_rc );

			InflateRect(&_rc, -2*_nWidth, -2*_nWidth);
			_hRgn2 = CreateEllipticRgnIndirect( &_rc );

			_hRgnDiff = CreateRectRgn(0, 0, 0, 0);
			_hRgn = CreateRectRgn(0, 0, 0, 0);
			CombineRgn(_hRgnDiff, _hRgn1, _hRgn2, RGN_DIFF);
			CombineRgn(_hRgn, _hRgnDiff, _hRgnRct, RGN_AND);

			DeleteObject( _hRgn1 );
			DeleteObject( _hRgn2 );
			DeleteObject( _hRgnRct );
			DeleteObject( _hRgnDiff );

			return (DWORD)_hRgn;
		}

		case SCMD_UPDATE:
		{
			RECT		_rc, _rcFace;
			REAL		_aStart, _aEnd;
			int			_nPenWidth;

			LPPU		_lpPrm;

            wParam_++;
			//invalidate the old FTU
			FaceGetRect( wFTU_, &_rc);
			InflateRect( &_rc, wParam_, wParam_ );
			InvalidateRect( hwnd_, &_rc, TRUE );

			_lpPrm = FaceGetParameter(wFTU_);
			CORE_ASSERT(_lpPrm);
			FaceXycxcyToRect(wFTU_, &_rc);

			//read start angle and end angle, then transform to radians
			MFPUREAD( _lpPrm, ARC_STARTANGLE, REAL, _aStart);
			MFPUREAD( _lpPrm, ARC_ENDANGLE, REAL, _aEnd);
			_aStart *= M_PI/180;
			_aEnd *= M_PI/180;

			MFPUREAD( _lpPrm, ARC_PENWIDTH, int, _nPenWidth );

			CaculateArcRect( &_rcFace, &_rc, _aStart, _aEnd );
			InflateRect(&_rcFace, _nPenWidth, _nPenWidth);
			FaceSetRect( wFTU_, &_rcFace );		//set new face rect

			//invalidate the new RECT
			//Note: not erase background
			InvalidateRect( hwnd_, &_rcFace, FALSE );
			return TRUE;
		}

		default:
			return DefStampBuild( wCmd_, wFTU_, hwnd_, wParam_, lParam_ );
	}

	return TRUE;
}

//...................................................................
//Description:
//	Calculate the min rectangle bounding the specified arc
//Entry:
//	LPRECT lprcFace_	the result
//	LPRECT lprc_		specifies the rectangle bounding the ellipse
//	REAL aStart_		specifies the angle of arc starting
//	REAL aEnd_			specifies the angle of arc end
//Note:
//	The arc is beginning at the starting angle and end at the end angle
//	award countclockwise direction in window client area, BUT
//	award clockwise direction in D-coor.
//...................................................................
static void _near CaculateArcRect( LPRECT lprcFace_, LPRECT lprc_, REAL aStart_, REAL aEnd_ )
{
	float	x0, y0;				//ellipse center
	float	xs, ys, xe, ye;		//start-point and end-point at the arc
	float	cs, ce;				//start angle and end angle
	float	a, b;				//radius
	float	r, tmp;
	float	csine, sine;
	WORD	ws, we;
	WORD	_flag;

	//calculate ellipse center
	x0 = (lprc_->left + lprc_->right)/2;
	y0 = (lprc_->top + lprc_->bottom)/2;

	cs = aStart_;
	ce = aEnd_;

	//calculate a, b
	a = (lprc_->right - lprc_->left)/2;
	b = (lprc_->bottom - lprc_->top)/2;

	//calculate the xs, ys
	csine = cos(cs);
	sine = sin(cs);
	tmp = csine*csine/(a*a) + sine*sine/(b*b);
	r = sqrt(1/tmp);
	xs = r*csine+x0;
	ys = r*sine+y0;

	if( csine >= 0 )
	{
		if( sine >= 0 )
			ws = 1;
		else
			ws = 4;
	}
	else
	{
		if( sine >= 0 )
			ws = 2;
		else
			ws = 3;
	}

	//calculate the xe, ye
	csine = cos(ce);
	sine = sin(ce);
	tmp = csine*csine/(a*a) + sine*sine/(b*b);
	r = sqrt(1/tmp);
	xe = r*csine+x0;
	ye = r*sine+y0;

	if( csine >= 0 )
	{
		if( sine >= 0 )
			we = 1;
		else
			we = 4;
	}
	else
	{
		if( sine >= 0 )
			we = 2;
		else
			we = 3;
	}

	switch( (ws << 4) | we )
	{
		case 0X11:
			_flag = 0x0000;
			break;

		case 0X12:
			_flag = 0x1011;
			break;

		case 0X13:
			_flag = 0x0011;
			break;

		case 0X14:
			_flag = 0x0010;
			break;

		case 0X21:
			_flag = 0x0100;
			break;

		case 0X22:
			_flag = 0x0000;
			break;

		case 0X23:
			_flag = 0x0111;
			break;

		case 0X24:
			_flag = 0x0110;
			break;

		case 0X31:
			_flag = 0x1100;
			break;

		case 0X32:
			_flag = 0x1000;
			break;

		case 0X33:
			_flag = 0x0000;
			break;

		case 0X34:
			_flag = 0x1110;
			break;

		case 0X41:
			_flag = 0x1101;
			break;

		case 0X42:
			_flag = 0x1001;
			break;

		case 0X43:
			_flag = 0x0001;
			break;

		case 0X44:
			_flag = 0x0000;
			break;
	}

	if( _flag & 0x1000 )
		lprcFace_->left = lprc_->left;
	else
		lprcFace_->left = min( xs, xe );

	if( _flag & 0x0001 )
		lprcFace_->top = lprc_->top;
	else
		lprcFace_->top = min( ys, ye );

	if( _flag & 0x0010 )
		lprcFace_->right = lprc_->right;
	else
		lprcFace_->right = max( xs, xe );

	if( _flag & 0x0100 )
		lprcFace_->bottom = lprc_->bottom;
	else
		lprcFace_->bottom = max( ys, ye );

	return;
}



