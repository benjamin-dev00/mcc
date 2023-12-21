//===================================================================
//
//Zheng zhikai	96-12-25
//===================================================================
#include "prehead.h"

#include "coreall.h"
#include "stmpdef.h"
#include <stdarg.h>
#include "StmpRes.h"

#pragma warning( disable : 4244 )	//'conversion' conversion from 'type1' to 'type2', possible loss of data

extern	HINSTANCE	ghInst;

enum _slide_param {
	SLIDE_BORDERSTYLE = FACE_FIRSTPARAM,
	SLIDE_BORDERWIDTH,
	SLIDE_BORDERCLR,
	SLIDE_BACKCLR,
	SLIDE_THUMBSTYLE,
	SLIDE_ORIENTATION,	//vertical, horizontal
	SLIDE_TICKMARK,		//NONE, LEFT, RIGHT, BOTH
	SLIDE_TICKDIV,
	SLIDE_TICKCLR,
	SLIDE_LABELMARK,
	SLIDE_LABELDIV,
	SLIDE_LABELCLR,
	SLIDE_DISCRETE,		//discrete or continuous
	SLIDE_TRANSMIT,
	SLIDE_DATA,
	SLIDE_MIN,
	SLIDE_MAX,
	SLIDE_GUTBORDERSTYLE,
	SLIDE_GUTBORDERWIDTH,
	SLIDE_GUTBORDERCLR,
	SLIDE_GUTBACKCLR,
	SLIDE_GUTWIDTH,
	SLIDE_FILLCLR
};

enum _thumb_style
{
	ts_LEFT = 0,
	ts_TOP,
	ts_RIGHT,
	ts_BOTTOM,
};

enum _slide_mark
{
	NONE = 0,
	LEFT,
	RIGHT,
	BOTH,
};

enum _slide_orientation
{
	VERTICAL = 0,
	HORIZONTAL,
};

#define SLIDE_BORDERSTYLE_PDT			PDT_int
#define SLIDE_BORDERWIDTH_PDT			PDT_DWORD
#define SLIDE_BORDERCLR_PDT				PDT_COLORREF
#define SLIDE_BACKCLR_PDT				PDT_COLORREF
#define SLIDE_THUMBSTYLE_PDT			PDT_DWORD
#define SLIDE_ORIENTATION_PDT			PDT_DWORD	
#define SLIDE_TICKMARK_PDT				PDT_DWORD
#define SLIDE_TICKDIV_PDT				PDT_DWORD
#define SLIDE_TICKCLR_PDT				PDT_COLORREF
#define SLIDE_LABELMARK_PDT				PDT_DWORD
#define SLIDE_LABELDIV_PDT				PDT_DWORD
#define SLIDE_LABELCLR_PDT				PDT_COLORREF
#define SLIDE_DISCRETE_PDT				PDT_BOOL
#define SLIDE_TRANSMIT_PDT				PDT_BOOL
#define SLIDE_DATA_PDT					PDT_REAL
#define SLIDE_MIN_PDT					PDT_REAL
#define SLIDE_MAX_PDT					PDT_REAL
#define SLIDE_GUTBORDERSTYLE_PDT		PDT_int
#define SLIDE_GUTBORDERWIDTH_PDT		PDT_DWORD
#define SLIDE_GUTBORDERCLR_PDT			PDT_COLORREF
#define SLIDE_GUTBACKCLR_PDT			PDT_COLORREF
#define SLIDE_GUTWIDTH_PDT				PDT_int
#define SLIDE_FILLCLR_PDT				PDT_COLORREF

typedef struct _tag_SLIDEINFO
{
	//position
	RECT		Rect;

	//parameter
	int			BorderStyle;
	DWORD		BorderWidth;
	COLORREF	BorderClr;
	COLORREF	BackClr;
	DWORD		ThumbStyle;
	DWORD		Orientation;
	DWORD		TickMark;
	DWORD		TickDiv;
	COLORREF	TickClr;
	DWORD		LabelMark;
	DWORD		LabelDiv;
	COLORREF	LabelClr;
	BOOL		Discrete;
	BOOL		Transmit;
	REAL		Data;
	REAL		Min;
	REAL		Max;
	int			GutBorderStyle;
	DWORD		GutBorderWidth;
	COLORREF	GutBorderClr;
	COLORREF	GutBackClr;
	int			GutWidth;
	COLORREF	FillClr;
}SLIDEINFO, FAR* LPSLIDEINFO;

//#define SLIDE_TEXTCLR		PDT_COLORREF

static struct _icons_info
{
	HICON	iiHandle;
	SIZE	iiSize;
	SIZE	iiAuctual;
} s_ii[] =	//
{
	{ NULL, { 32, 32 }, { 32, 32 } },
	{ NULL, { 32, 32 }, { 32, 32 } },
	{ NULL, { 32, 32 }, { 32, 32 } },
	{ NULL, { 32, 32 }, { 32, 32 } },
	{ NULL, { 32, 32 }, { 32, 32 } }
};

static VSOFNAME _orientation_mode_vsof[] =
{
	{VERTICAL,		"Vertical"},
	{HORIZONTAL,	"Horizontal"},
};

static VSOFNAME _label_mode_vsof[] =
{
	{NONE,		"None"},
	{LEFT,		"Left"},
	{RIGHT,		"Right"},
	{BOTH,		"Both"},
};

const unsigned c_ii_items = sizeof(s_ii)/sizeof(s_ii[0]);

void DrawTick( WRD wrd_, HDC hdc_, LPRECT lprcGut_, LPSLIDEINFO psi_ );
BOOL ReadParameter( WRD wrd_, LPSLIDEINFO psi_ );
void GetValuePoint( WRD wrd_, LPRECT lprcGut_, LPPOINT pPoint_, LPSLIDEINFO psi_ );
void GetGutPosition( WRD wrd_, HDC hdc_, LPRECT lpRect_, LPSLIDEINFO psi_ );
HICON GetThumbInfo( WRD wrd_, LPSIZE pszIcon_, LPSIZE pszActual_, LPSLIDEINFO psi_ );

//...................................................................
//...................................................................
_DLLEXP BOOL _DLLFUNC SlidePaint(WRD wFTU_, HDC hdc_)
{
	HICON		_hIcon; 
	RECT		_rect, _rcGut;
	SLIDEINFO	_si;
	SIZE		_szActual, _szIcon;
	POINT		_ptValue;

	//_lpPrm = FaceGetParameter(wFTU_);

	FaceXycxcyToRect(wFTU_, &_rect);
	ReadParameter( wFTU_, &_si );

	//draw background and gutter
	CoreFillRect( hdc_, &_si.Rect, _si.BackClr );
	CoreDrawBox( hdc_, &_si.Rect, _si.BorderStyle, _si.BorderWidth, _si.BorderClr );

	GetGutPosition( wFTU_, hdc_, &_rcGut, &_si );
	CoreFillRect( hdc_, &_rcGut, _si.GutBackClr );
	CoreDrawBox( hdc_, &_rcGut, _si.GutBorderStyle, _si.GutBorderWidth, _si.GutBorderClr );

	DrawTick( wFTU_, hdc_, &_rcGut, &_si );

	//draw bitmap
	_hIcon = GetThumbInfo( wFTU_, &_szIcon, &_szActual, &_si );
	GetValuePoint( wFTU_, &_rcGut, &_ptValue, &_si );
	_ptValue.x -= _szIcon.cx / 2;
	_ptValue.y -= _szIcon.cy / 2;
	DrawIcon( hdc_, _ptValue.x, _ptValue.y, _hIcon );

	return TRUE;
}

//...................................................................
//...................................................................
_DLLEXP DWORD _DLLFUNC SlideParam(PIC wCmd_, WRD wFTU_, WORD wNo_, LONG lParam_)
{
	static PDU _sPDU[] =
	{
		_PDU_DEFAULT,

		_ELE_PDU(SLIDE_BORDERSTYLE,		"BorderStyle", 	VSOF_int_RECTBS,	PU_NORMAL, PRI_ALL ),
		_ELE_PDU(SLIDE_BORDERWIDTH,		"BorderWidth", 	VSOF_DWORD_BWIDTH,	PU_NORMAL, PRI_ALL ),
		_ELE_PDU(SLIDE_BORDERCLR, 		"BorderColor",	VSOF_DWORD_CLR,		PU_NORMAL, PRI_ALL ),
		_ELE_PDU(SLIDE_BACKCLR, 		"BackColor", 	VSOF_DWORD_CLR,		PU_NORMAL, PRI_ALL ),

		_ELE_PDU(SLIDE_THUMBSTYLE,		"SlideStyle",	VSOF_DWORD_HEX,		PU_NORMAL, PRI_ALL ),
		_ELE_PDU(SLIDE_ORIENTATION,		"Orientation",	VSOF_DWORD_USER,	PU_NORMAL, PRI_ALL ),

		_ELE_PDU(SLIDE_TICKMARK,		"TickMark",		VSOF_DWORD_USER,	PU_NORMAL, PRI_ALL ),
		_ELE_PDU(SLIDE_TICKDIV,			"TickDiv",		VSOF_DWORD,			PU_NORMAL, PRI_ALL ),
		_ELE_PDU(SLIDE_TICKCLR, 		"TickColor", 	VSOF_DWORD_CLR,		PU_NORMAL, PRI_ALL ),
	
		_ELE_PDU(SLIDE_LABELMARK,		"LabelMark",	VSOF_DWORD_USER,	PU_NORMAL, PRI_ALL ),
		_ELE_PDU(SLIDE_LABELDIV,		"LabelDiv",		VSOF_DWORD,			PU_NORMAL, PRI_ALL ),
		_ELE_PDU(SLIDE_LABELCLR, 		"LabelColor", 	VSOF_DWORD_CLR,		PU_NORMAL, PRI_ALL ),

		_ELE_PDU(SLIDE_DISCRETE, 		"Discrete", 	VSOF_BOOL,			PU_NORMAL, PRI_ALL ),
		_ELE_PDU(SLIDE_TRANSMIT, 		"Transmit", 	VSOF_BOOL,			PU_NORMAL, PRI_ALL ),

		//the data source
		_ELE_PDU(SLIDE_DATA,			"Data",			VSOF_REAL,			PU_NORMAL, PRI_ALL|PDUF_NODEFAULT ),
		_ELE_PDU(SLIDE_MIN,				"Min",			VSOF_REAL,			PU_NORMAL, PRI_ALL ),
		_ELE_PDU(SLIDE_MAX,				"Max",			VSOF_REAL,			PU_NORMAL, PRI_ALL ),

		_ELE_PDU(SLIDE_GUTBORDERSTYLE,	"BarStyle", 	VSOF_int_RECTBS,	PU_NORMAL, PRI_ALL ),
		_ELE_PDU(SLIDE_GUTBORDERWIDTH,	"BarWidth", 	VSOF_DWORD_BWIDTH,	PU_NORMAL, PRI_ALL ),
		_ELE_PDU(SLIDE_GUTBORDERCLR,	"BarColor",		VSOF_DWORD_CLR,		PU_NORMAL, PRI_ALL ),
		_ELE_PDU(SLIDE_GUTBACKCLR, 		"BarBackColor", VSOF_DWORD_CLR,		PU_NORMAL, PRI_ALL ),

		_ELE_PDU(SLIDE_GUTWIDTH, 		"BarWidth", 	VSOF_DWORD,			PU_NORMAL, PRI_ALL ),
		_ELE_PDU(SLIDE_FILLCLR, 		"FillColor", 	VSOF_DWORD_CLR,		PU_NORMAL, PRI_ALL ),

	};

	switch(wCmd_)
	{
		case PIC_ONLOAD:
		{
			s_ii[0].iiHandle = LoadImage( ghInst, MAKEINTRESOURCE(IDI_LEFT), IMAGE_ICON, 0, 0, LR_LOADMAP3DCOLORS );
			s_ii[1].iiHandle = LoadImage( ghInst, MAKEINTRESOURCE(IDI_LEFT), IMAGE_ICON, 0, 0, LR_LOADMAP3DCOLORS );
			s_ii[2].iiHandle = LoadImage( ghInst, MAKEINTRESOURCE(IDI_LEFT), IMAGE_ICON, 0, 0, LR_LOADMAP3DCOLORS );
			s_ii[3].iiHandle = LoadImage( ghInst, MAKEINTRESOURCE(IDI_LEFT), IMAGE_ICON, 0, 0, LR_LOADMAP3DCOLORS );

			return TRUE;
		}

		case PIC_ONEXIT:
		{
			unsigned	_i;
			for( _i=0; _i< c_ii_items; _i++)
				DestroyIcon( s_ii[3].iiHandle );

			return TRUE;
		}

		case PIC_GETVSOFENUMSTRUCT:
		{
			LPVSOFENUMSTRUCT _lpes = (LPVSOFENUMSTRUCT)lParam_;

			if( (wNo_==SLIDE_TICKMARK) || (wNo_==SLIDE_LABELMARK) )
			{
				_lpes->lpVsofName = _label_mode_vsof;
				_lpes->nItem = sizeof(_label_mode_vsof)/sizeof(_label_mode_vsof[0]);
				return TRUE;
			}
			else if( wNo_== SLIDE_ORIENTATION )
			{
				_lpes->lpVsofName = _orientation_mode_vsof;
				_lpes->nItem = sizeof(_orientation_mode_vsof)/sizeof(_orientation_mode_vsof[0]);
				return TRUE;
			}
			else
				return _DefStampParam(wCmd_,
								  wFTU_,
								  wNo_,
								  lParam_,
								  sizeof(_sPDU)/sizeof(PDU),
								  IDS_SLIDE,
								  &(_sPDU[0]) );
		}

		default:
			return _DefStampParam(wCmd_,
								  wFTU_,
								  wNo_,
								  lParam_,
								  sizeof(_sPDU)/sizeof(PDU),
								  IDS_SLIDE,
								  &(_sPDU[0]) );
	}
}


//...................................................................
//...................................................................
_DLLEXP DWORD _DLLFUNC SlideBuild(SCMD wCmd_, WRD wFTU_, HWND hwnd_, WORD wParam_, LONG lParam_)
{
	switch(wCmd_)
	{
		case SCMD_INIT:
		{
			LPFINIT			_lpFinit = (LPFINIT)lParam_;
			LPPU			_lpPrm;

			_lpPrm  = FaceGetParameter( wFTU_ );

			MFPUWRITE(_lpPrm, SLIDE_BORDERWIDTH, DWORD, _lpFinit->border_pen.lopnWidth.x);
			MFPUWRITE(_lpPrm, SLIDE_BORDERSTYLE, int, _lpFinit->border_pen.lopnStyle);
			MFPUWRITE(_lpPrm, SLIDE_BORDERCLR, COLORREF, _lpFinit->border_pen.lopnColor);
			MFPUWRITE(_lpPrm, SLIDE_BACKCLR, COLORREF, _lpFinit->back_clr);
			MFPUWRITE(_lpPrm, SLIDE_MAX, REAL, 100);
			MFPUWRITE(_lpPrm, SLIDE_MIN, REAL, 0);
			MFPUWRITE(_lpPrm, SLIDE_DATA, REAL, 50);
			break;
		}

		case SCMD_RUNTIME:
			return (RT_CAPTURE|RT_HDC|RT_WANTARROWS);

		case SCMD_RUNTIME_INPUTMSG:
		{
			LPFIA			_lpFia = (LPFIA)lParam_;
			WORD			_ret = FIA_PROCESSED;

			switch( _lpFia->msg )
			{
				/****
				case WM_LBUTTONDOWN:
				{
					POINT	_point;
					RECT	_rc;
					HBITMAP	_hbmp;

					_Get_Rect_HBmp( wFTU_, &_rc, &_hbmp, &_point.x, &_point.y );
					ClientToScreen( hwnd_, &_point );

					SetCursorPos(_point.x, _point.y);
					break;
				}

				case WM_MOUSEMOVE:
				{
					int			_x, _y, _cur_x, _cur_y;
					LPPU		_lpPrm;
					RECT		_rc, _rcThomb;
					REAL		_max, _min, _factor, _val;
					BITMAP		_bm;
					SimplePU	_spu;
					WORD		_wStyle;
					HBITMAP		_hbmp;

					_lpPrm = FaceGetParameter( wFTU_ );
					MFPUREAD( _lpPrm, SLIDE_STYLE, WORD, _wStyle );
					MFPUREAD( _lpPrm, SLIDE_MIN, REAL, _min );
					MFPUREAD( _lpPrm, SLIDE_MAX, REAL, _max );
					_Get_Rect_HBmp( wFTU_, &_rc, &_hbmp, &_x, &_y );

					_cur_x = X(_lpFia->lParam);		//get cursor x
					_cur_y = Y(_lpFia->lParam);		//get cursor y
					switch( _wStyle & (_SS_DECREASE|_SS_HORZBAR) )
					{
						case _SS_DECREASE|_SS_HORZBAR:
						{
							_factor = (float)(_max - _min) / (float)(_rc.left - _rc.right);
							_val = ( _cur_x - _rc.right ) * _factor + _min;
							break;
						}

						case _SS_HORZBAR:
						{
							_factor = (float)(_max - _min) / (float)(_rc.right-_rc.left);
							_val = ( _cur_x - _rc.left ) * _factor + _min;
							break;
						}

						case _SS_DECREASE:
						{
							_factor = (float)(_max - _min) / (float)(_rc.bottom-_rc.top);
							_val = ( _cur_y - _rc.top ) * _factor + _min;
							break;
						}

						case 0:
						{
							_factor = (float)(_max - _min) / (float)(_rc.top-_rc.bottom);
							_val = ( _cur_y - _rc.bottom ) * _factor + _min;
							break;
						}
					}

					if ( _val > _max ) _val = _max;
					if ( _val < _min ) _val = _min;

					_spu.prefix = PF_IMM|IMM_REAL;
					_spu.du.zzk__REAL = _val;

					if( !PUWrite( _lpPrm + SLIDE_DATA, &_spu) )
					{
						//illegal data input!
						MessageBeep( MB_ICONEXCLAMATION );
						break;
					}

					GetObject( _hbmp, sizeof(BITMAP), &_bm );

					//invalidate the old position of the Slide-thomb
					_rcThomb.left = _x - _bm.bmWidth/2;
					_rcThomb.top = _y - _bm.bmHeight/2;
					_rcThomb.right = _x + _bm.bmWidth/2;
					_rcThomb.bottom = _y + _bm.bmHeight/2;
					InvalidateRect(hwnd_, &_rcThomb, TRUE);

					//invalidate the new position of the Slide-thomb
					_Get_Rect_HBmp( wFTU_, &_rc, &_hbmp, &_x, &_y );
					_rcThomb.left = _x - _bm.bmWidth/2;
					_rcThomb.top = _y - _bm.bmHeight/2;
					_rcThomb.right = _x + _bm.bmWidth/2;
					_rcThomb.bottom = _y + _bm.bmHeight/2;
					InvalidateRect(hwnd_, &_rcThomb, FALSE);
					UpdateWindow( hwnd_ );
					break;
				}

				case WM_LBUTTONUP:
				{
					_ret = FIA_PROCESSEND;
					break;
				}
				****/

				default:
					_ret = FIA_NOTPROCESSED;
					break;
			}
			return _ret;
		}

		default:
			return DefStampBuild( wCmd_, wFTU_, hwnd_, wParam_, lParam_ );
	}

	return TRUE;
}

//Retrieve the size and HANDLE of the icon
HICON GetThumbInfo( WRD wrd_, LPSIZE pszIcon_, LPSIZE pszActual_, LPSLIDEINFO psi_ )
{
	if( psi_->ThumbStyle >= c_ii_items )
		psi_->ThumbStyle = 0;	//set to default

	pszIcon_->cx = s_ii[psi_->ThumbStyle].iiSize.cx;
	pszIcon_->cy = s_ii[psi_->ThumbStyle].iiSize.cy;

	pszActual_->cx = s_ii[psi_->ThumbStyle].iiAuctual.cx;
	pszActual_->cy = s_ii[psi_->ThumbStyle].iiAuctual.cy;

	return s_ii[psi_->ThumbStyle].iiHandle;
}

//....................................................................
//Discription:
//	Calculate gutter height if the orientation is vertical; or the gutter
//	width if it is horizontal
//
//....................................................................
void GetGutPosition( WRD wrd_, HDC hdc_, LPRECT lpRect_, LPSLIDEINFO psi_ )
{
	SIZE	_szIcon, _szActual;

	GetThumbInfo( wrd_, &_szIcon, &_szActual, psi_ );

	if( psi_->Orientation == VERTICAL ) //calculate height
	{
		TEXTMETRIC _tm;
		GetTextMetrics( hdc_, &_tm );
		
		lpRect_->left = (psi_->Rect.left + psi_->Rect.right)/2 - psi_->GutWidth/2;
		lpRect_->right = lpRect_->left + psi_->GutWidth;
		lpRect_->top = psi_->Rect.top + __max( _tm.tmHeight, _szActual.cy );
		lpRect_->bottom = psi_->Rect.bottom - __max( _tm.tmHeight, _szActual.cy );
	}
	else			//calculate the width
	{
		char		_szBuffer[32];
		SIZE		_sz;

		lpRect_->top = (psi_->Rect.top + psi_->Rect.bottom)/2 - psi_->GutWidth/2;
		lpRect_->bottom = lpRect_->top + psi_->GutWidth;

		gcvt( psi_->Min, 5, _szBuffer );
		GetTextExtentPoint32( hdc_, _szBuffer, strlen(_szBuffer), &_sz );
		lpRect_->left = psi_->Rect.left + __max( _sz.cx, _szActual.cx ) / 2;

		gcvt( psi_->Max, 5, _szBuffer );
		GetTextExtentPoint32( hdc_, _szBuffer, strlen(_szBuffer), &_sz );
		lpRect_->right = psi_->Rect.right - __max( _sz.cx, _szActual.cx ) / 2;
	}

	return;
}

void GetValuePoint( WRD wrd_, LPRECT lprcGut_, LPPOINT pPoint_, LPSLIDEINFO psi_ )
{
	REAL	_f = (psi_->Max - psi_->Data) / (psi_->Max - psi_->Min);
	REAL	_value;

	if( psi_->Orientation == VERTICAL ) //calculate height
	{
		_value = ((REAL)(lprcGut_->bottom - lprcGut_->top)) * _f + lprcGut_->top; 
		pPoint_->x = (lprcGut_->left + lprcGut_->right)/2;
		pPoint_->y = (int)_value;
	}
	else
	{
		_value = ((REAL)(lprcGut_->left - lprcGut_->right)) * _f + lprcGut_->right; 
		pPoint_->x = (int)_value;
		pPoint_->y = (lprcGut_->top + lprcGut_->bottom)/2;
	}

	return;
}

void DrawTick( WRD wrd_, HDC hdc_, LPRECT lprcGut_, LPSLIDEINFO psi_ )
{
	/*
	if( psi_->TickMark & LEFT )
	{
		//draw left side ticks
		for(  
	}

	if( psi_->TickMark & RIGHT )
	{
		//draw right side ticks
	}
	*/

	return;
}

BOOL ReadParameter( WRD wrd_, LPSLIDEINFO psi_ )
{
	LPPU		_lpPrm;

	_lpPrm = FaceGetParameter(wrd_);
	if( ! _lpPrm )
	{
		CORE_ASSERT( FALSE );
		return FALSE;
	}

	FaceXycxcyToRect(wrd_, &(psi_->Rect));

	MFPUREAD( _lpPrm, SLIDE_BORDERSTYLE,	int,		psi_->BorderStyle );
	MFPUREAD( _lpPrm, SLIDE_BORDERWIDTH,	DWORD,		psi_->BorderWidth );
	MFPUREAD( _lpPrm, SLIDE_BORDERCLR,		COLORREF,	psi_->BorderClr );
	MFPUREAD( _lpPrm, SLIDE_BACKCLR,		COLORREF,	psi_->BackClr );
	MFPUREAD( _lpPrm, SLIDE_THUMBSTYLE,		DWORD,		psi_->ThumbStyle );
	MFPUREAD( _lpPrm, SLIDE_ORIENTATION,	DWORD,		psi_->Orientation );
	MFPUREAD( _lpPrm, SLIDE_TICKMARK,		DWORD,		psi_->TickMark );
	MFPUREAD( _lpPrm, SLIDE_TICKDIV,		DWORD,		psi_->TickDiv );
	MFPUREAD( _lpPrm, SLIDE_TICKCLR,		COLORREF,	psi_->TickClr );
	MFPUREAD( _lpPrm, SLIDE_LABELMARK,		DWORD,		psi_->LabelMark );
	MFPUREAD( _lpPrm, SLIDE_LABELDIV,		DWORD,		psi_->LabelDiv );
	MFPUREAD( _lpPrm, SLIDE_LABELCLR,		COLORREF,	psi_->LabelClr );
	MFPUREAD( _lpPrm, SLIDE_DISCRETE,		BOOL,		psi_->Discrete );
	MFPUREAD( _lpPrm, SLIDE_TRANSMIT,		BOOL,		psi_->Transmit );
	MFPUREAD( _lpPrm, SLIDE_DATA,			REAL,		psi_->Data );
	MFPUREAD( _lpPrm, SLIDE_MIN,			REAL,		psi_->Min );
	MFPUREAD( _lpPrm, SLIDE_MAX,			REAL,		psi_->Max );
	MFPUREAD( _lpPrm, SLIDE_GUTBORDERSTYLE,	int,		psi_->GutBorderStyle );
	MFPUREAD( _lpPrm, SLIDE_GUTBORDERWIDTH,	DWORD,		psi_->GutBorderWidth );
	MFPUREAD( _lpPrm, SLIDE_GUTBORDERCLR,	COLORREF,	psi_->GutBorderClr );
	MFPUREAD( _lpPrm, SLIDE_GUTBACKCLR,		COLORREF,	psi_->GutBackClr );
	MFPUREAD( _lpPrm, SLIDE_GUTWIDTH,		int,		psi_->GutWidth );
	MFPUREAD( _lpPrm, SLIDE_FILLCLR,		COLORREF,	psi_->FillClr );

	return TRUE;
}

