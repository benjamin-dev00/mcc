//===================================================================
//Stamp: Percent Fill
//Zheng zhikai	96-12-25
//===================================================================
#include "prehead.h"

#include "coreall.h"
#include "stmpdef.h"
#include <stdarg.h>

#include "StmpRes.h"

extern	HINSTANCE	ghInst;

enum _fill_param {
	FILL_BORDERSTYLE = FACE_FIRSTPARAM,
	FILL_BORDERWIDTH,
	FILL_BORDERCLR,
	FILL_BACKCLR,
	FILL_VERTICALBAR,
	FILL_INCREASEBAR,
	FILL_DATA,
	FILL_MIN,
	FILL_MAX,
	FILL_FILLCLR,
};

#define FILL_VERTICALBAR_PDT	PDT_BOOL
#define FILL_INCREASEBAR_PDT	PDT_BOOL
#define FILL_DATA_PDT			PDT_REAL
#define FILL_MIN_PDT			PDT_REAL
#define FILL_MAX_PDT           	PDT_REAL
#define FILL_BORDERSTYLE_PDT	PDT_int
#define FILL_BORDERWIDTH_PDT	PDT_DWORD
#define FILL_BORDERCLR_PDT		PDT_COLORREF
#define FILL_BACKCLR_PDT		PDT_COLORREF
#define FILL_FILLCLR_PDT		PDT_COLORREF

//horizontal bar; defaultly, vertical bar
#define _FS_HORIZBAR	0x0001

//defaultly, increase from bottom to top, or left to right,
//from right to left, or top to bottom
#define _FS_DECREASE		0x0002

//...................................................................
//...................................................................
_DLLEXP BOOL _DLLFUNC FillPaint(WRD wFTU_, HDC hdc_)
{
	LPPU		_lpPrm;

	COLORREF	_clrBack, _clrBorder, _clrFill;
	RECT		_rect, _rcData, _rcBack;

	int			_nPenStyle;
	DWORD 		_dwPenWidth;
	BOOL		_bVetical;
	BOOL		_bIncrease; 
	UINT		_style;

	REAL		_data, _min, _max;
	int			_x, _y;

	_lpPrm = FaceGetParameter(wFTU_);
	if( ! _lpPrm )
	{
		CORE_ASSERT( FALSE );
		return FALSE;
	}

	FaceXycxcyToRect(wFTU_, &_rect);

	MFPUREAD( _lpPrm, FILL_VERTICALBAR, BOOL, _bVetical );
	MFPUREAD( _lpPrm, FILL_INCREASEBAR, BOOL, _bIncrease );
	_style = 0;
	if( !_bVetical )	_style |= _FS_HORIZBAR;
	if( !_bIncrease )	_style |= _FS_DECREASE;

	//MFPUREAD( _lpPrm, FILL_STYLE, WORD, _style );
	MFPUREAD( _lpPrm, FILL_BORDERCLR, COLORREF, _clrBorder );
	MFPUREAD( _lpPrm, FILL_BORDERWIDTH, DWORD, _dwPenWidth );
	MFPUREAD( _lpPrm, FILL_BORDERSTYLE, int, _nPenStyle );
	MFPUREAD( _lpPrm, FILL_BACKCLR, COLORREF, _clrBack );
	MFPUREAD( _lpPrm, FILL_FILLCLR, COLORREF, _clrFill );

	MFPUREAD( _lpPrm, FILL_DATA, REAL, _data );
	MFPUREAD( _lpPrm, FILL_MIN, REAL, _min );
	MFPUREAD( _lpPrm, FILL_MAX, REAL, _max );

	if( _data > _max ) _data = _max;
	if( _data < _min ) _data = _min;

	CopyRect( &_rcData, &_rect );
	CopyRect( &_rcBack, &_rect );
	switch( _style & (_FS_DECREASE|_FS_HORIZBAR) )
	{
		case _FS_DECREASE|_FS_HORIZBAR:
		{
			_x = (int)(_rect.right - _data / (_max - _min) * (_rect.right - _rect.left));
			_rcData.left = _x;
			_rcBack.right = _x;
			break;
		}

		case _FS_HORIZBAR:
		{
			_x = (int)(_rect.left + _data / (_max - _min) * (_rect.right - _rect.left));
			_rcData.right = _x;
			_rcBack.left = _x;
			break;
		}

		case _FS_DECREASE:
		{
			_y = (int)(_rect.top + _data / (_max - _min) * (_rect.bottom - _rect.top));
			_rcData.bottom = _y;
			_rcBack.top = _y;
			break;
		}

		case 0:
		{
			_y = (int)(_rect.bottom - _data / (_max - _min) * (_rect.bottom - _rect.top));
			_rcData.top = _y;
			_rcBack.bottom = _y;
			break;
		}
	}

	CoreFillRect( hdc_, &_rcBack, _clrBack );
	CoreFillRect( hdc_, &_rcData, _clrFill );
	CoreDrawBox( hdc_, &_rect, _nPenStyle, _dwPenWidth, _clrBorder );

	return TRUE;
}

//...................................................................
//...................................................................
_DLLEXP DWORD _DLLFUNC FillParam(PIC wCmd_, WRD wFTU_, WORD wNo_, LONG lParam_)
{
	static PDU _sPDU[] =
	{
		_PDU_DEFAULT,

		_ELE_PDU(FILL_BORDERSTYLE, 	"BorderStyle", 	VSOF_int_RECTBS,	PU_NORMAL, PRI_ALL ),
		_ELE_PDU(FILL_BORDERWIDTH, 	"BorderWidth", 	VSOF_DWORD_BWIDTH,	PU_NORMAL, PRI_ALL ),
		_ELE_PDU(FILL_BORDERCLR, 	"BorderColor",	VSOF_DWORD_CLR,		PU_NORMAL, PRI_ALL ),
		_ELE_PDU(FILL_BACKCLR, 		"BackColor", 	VSOF_DWORD_CLR,		PU_NORMAL, PRI_ALL ),

		_ELE_PDU(FILL_VERTICALBAR,	"Vertical",		VSOF_BOOL,			PU_NORMAL, PRI_ALL ),
		_ELE_PDU(FILL_INCREASEBAR,	"Increase",		VSOF_BOOL,			PU_NORMAL, PRI_ALL ),
		//the data source
		_ELE_PDU(FILL_DATA,			"Data",			VSOF_REAL,			PU_NORMAL, PRI_ALL|PDUF_NODEFAULT ),
		_ELE_PDU(FILL_MIN,			"Min",			VSOF_REAL,			PU_NORMAL, PRI_ALL ),
		_ELE_PDU(FILL_MAX,			"Max",			VSOF_REAL,			PU_NORMAL, PRI_ALL ),

		_ELE_PDU(FILL_FILLCLR, 		"FillColor", 	VSOF_DWORD_CLR,		PU_NORMAL, PRI_ALL )
	};

	switch(wCmd_)
	{
		default:
			return _DefStampParam(wCmd_,
								  wFTU_,
								  wNo_,
								  lParam_,
								  sizeof(_sPDU)/sizeof(PDU),
								  IDS_FILL,
								  &(_sPDU[0]) );
	}
}


//...................................................................
//...................................................................
_DLLEXP DWORD _DLLFUNC FillBuild(SCMD wCmd_, WRD wFTU_, HWND hwnd_, WORD wParam_, LONG lParam_)
{
	switch(wCmd_)
	{
		case SCMD_INIT:
		{
			LPFINIT			_lpFinit = (LPFINIT)lParam_;
			LPPU			_lpPrm;

			_lpPrm  = FaceGetParameter( wFTU_ );

			MFPUWRITE(_lpPrm, FILL_BORDERWIDTH, DWORD, _lpFinit->border_pen.lopnWidth.x);
			MFPUWRITE(_lpPrm, FILL_BORDERSTYLE, int, _lpFinit->border_pen.lopnStyle);
			MFPUWRITE(_lpPrm, FILL_BORDERCLR, COLORREF, _lpFinit->border_pen.lopnColor);
			MFPUWRITE(_lpPrm, FILL_BACKCLR, COLORREF, _lpFinit->back_clr);
			MFPUWRITE(_lpPrm, FILL_FILLCLR, COLORREF, RGB(0,255,0) );
			MFPUWRITE(_lpPrm, FILL_MAX, REAL, 100);
			MFPUWRITE(_lpPrm, FILL_MIN, REAL, 0);
			MFPUWRITE(_lpPrm, FILL_DATA, REAL, 50);
			MFPUWRITE(_lpPrm, FILL_VERTICALBAR, BOOL, TRUE);
			MFPUWRITE(_lpPrm, FILL_INCREASEBAR, BOOL, TRUE);

			break;
		}

		default:
			return DefStampBuild( wCmd_, wFTU_, hwnd_, wParam_, lParam_ );
	}

	return TRUE;
}
