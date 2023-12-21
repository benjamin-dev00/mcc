//===================================================================
//
//Zheng zhikai	95-4-22
//===================================================================
#include "prehead.h"

#include "coreall.h"
#include "stmpdef.h"

#pragma warning( disable : 4244 )	//'conversion' conversion from 'type1' to 'type2', possible loss of data

#define ML_PENSTYLE				(FACE_FIRSTPARAM + 0)
#define ML_PENWIDTH   	    	(FACE_FIRSTPARAM + 1)
#define ML_PENCLR				(FACE_FIRSTPARAM + 2)
#define ML_FLAG					(FACE_FIRSTPARAM + 3)

#define ML_PX					0
#define ML_PY					1

#define ML_PENSTYLE_PDT			PDT_int
#define ML_PENWIDTH_PDT			PDT_int
#define ML_PENCLR_PDT			PDT_COLORREF
#define ML_FLAG_PDT				PDT_WORD
#define ML_PX_PDT				PDT_FCCRD
#define ML_PY_PDT				PDT_FCCRD

//define operation code
#define _ML_EDIT				1
#define _ML_APPEND				2
#define _ML_NOTHING				0

static BOOL _DrawMultiLine(WRD wFTU_, 
						   HDC hdc_, 
						   BOOL bTestMode_, 
						   int iPresion_ /* ONLY valid while bTestMode_ == TRUE */);

WORD _near _ML_SCMDConstructMSG(WRD wFTU_, HWND hwnd_, LONG lParam_);
WORD _near _ML_SCMDEditMSG(WRD wFTU_, HWND hwnd_, LONG lParam_);
static void near _Construct_WM_MOUSEMOVE(WRD wFTU_, HWND hwnd_, LPFIA lpFma_ );
static void near _Construct_WM_LBUTTONUP( WRD wFTU_, HWND hwnd_,  LPFIA lpFma_ );
static void near _Construct_WM_LBUTTONDBLCLK( WRD wFTU_, HWND hwnd_, LPFIA lpFma_ );

static void near _ToggleMLSH( HDC hdc_, WRD wFTU_, WORD wSHSize_ );
static int near _MouseOnESH( WRD wFTU_, WORD wSHSize_, LONG point_ );

static PDU sfixPDU[] =
{
	_PDU_DEFAULT,
	_ELE_PDU(ML_PENSTYLE, 	"LineStyle",  	VSOF_int_LNSTYLE,	PU_NORMAL,	PRI_ALL ),
	_ELE_PDU(ML_PENWIDTH, 	"LineWidth",  	VSOF_int,			PU_NORMAL,	PRI_ALL ),
	_ELE_PDU(ML_PENCLR, 	"LineColor",	VSOF_DWORD_CLR,		PU_NORMAL,	PRI_ALL ),
	_ELE_PDU(ML_FLAG, 		"InnFlag",		VSOF_WORD,			PU_NORMAL,	PRI_DEV|PDUF_NODEFAULT )
};

static PDU	svarPDU[] = {
	_ELE_PDU(ML_PX, 		"Px",	 		VSOF_FCCRD,		PU_NORMAL,	PRI_ALL|PDUF_NODEFAULT ),
	_ELE_PDU(ML_PY, 		"Py",	 		VSOF_FCCRD,		PU_NORMAL,	PRI_ALL|PDUF_NODEFAULT )
};

static const WORD gcnumVar = sizeof(svarPDU)/sizeof(svarPDU[0]);
static const WORD gcnumFix = sizeof(sfixPDU)/sizeof(sfixPDU[0]);

typedef struct _tagMLS{
	WORD	wNumDot;		//the number of dots in ML
	int		iOldMode;		//the old mode in HDC
	HPEN	hOldPen;		//the old pen in HDC
	WORD	wSESH;
	WORD	wMode;			//_ML_NOTHING, _ML_EDIT, _ML_APPEND
	WORD	wDbl;			//the flag of the DBLCLK times
} ML, near* NPML;

//...................................................................
//...................................................................
_DLLEXP BOOL _DLLFUNC MultiLinePaint(WRD wFTU_, HDC hdc_)
{
	return _DrawMultiLine(wFTU_, hdc_, FALSE /* not test mode*/, 0 /* no meaning */ );
}

//...................................................................
//...................................................................
_DLLEXP DWORD _DLLFUNC MultiLineParam(PIC wCmd_, WRD wFTU_, WORD wNo_, LONG lParam_)
{
	static VPSTRUCT	_sVPD = {
		1,			//the default number of var parameters groups
		0,			//the low bound of var counter
		sizeof( sfixPDU ) / sizeof( sfixPDU[0] ),
		&( sfixPDU[0] ),
		sizeof( svarPDU ) / sizeof( svarPDU[0] ),
		&( svarPDU[0] )
	};

	return DefProcessVarPIC(wCmd_, wFTU_, wNo_, lParam_, TBX_FT, IDS_MULTILINE, &_sVPD);
}

//...................................................................
//...................................................................
_DLLEXP DWORD _DLLFUNC MultiLineBuild(SCMD wCmd_, WRD wFTU_, HWND hwnd_, WORD wParam_, LONG lParam_)
{

	switch(wCmd_)
	{
		case SCMD_INIT:
		{
			LPFINIT		_lpFinit = (LPFINIT)lParam_;
			LPPU		_lpPrm;

			_lpPrm  = FaceGetParameter( wFTU_ );

			MFPUWRITE(_lpPrm, ML_PENWIDTH, int,      _lpFinit->line_pen.lopnWidth.x);
			MFPUWRITE(_lpPrm, ML_PENSTYLE, int, 	 _lpFinit->line_pen.lopnStyle);
			MFPUWRITE(_lpPrm, ML_PENCLR,   COLORREF, _lpFinit->line_pen.lopnColor);

			return TRUE;
		}

		case SCMD_CONSTRUCT:
		{
			NPML _npML;

			CORE_ASSERT( ((LPFIA)lParam_)->lpPrivate == NULL );

			_npML = (NPML)LocalAlloc( LPTR, sizeof(ML) );

			if( !_npML )
				return FALSE;
			else
			{
				((LPFIA)lParam_)->lpPrivate = _npML;
				return TRUE;
			}
		}

		case SCMD_EDIT:
		{
			LPFIA	_lpFma = (LPFIA)lParam_;

			if( _lpFma->msg == WM_LBUTTONDBLCLK )
			{
				RECT	_rc;
				LPPU	_lpPrm;
				NPML 	_npML = (NPML)LocalAlloc( LPTR, sizeof(ML) );

				if( !_npML )
					return FALSE;

				((LPFIA)lParam_)->lpPrivate = _npML;

				_lpPrm  = FaceGetParameter( wFTU_ );

				//Notify: trap the following messages by this routine
				((LPFIA)lParam_)->wPid = 1;

				//write flag and redraw the face in edit-mode
				MFPUWRITE( _lpPrm, ML_FLAG, WORD, 1 );
				FaceGetRect( wFTU_, &_rc );
				InflateRect( &_rc, _lpFma->wSHSize, _lpFma->wSHSize );
				InvalidateRect( hwnd_, &_rc, TRUE );
				UpdateWindow( hwnd_ );

				return TRUE;
			}
			else
				return DefStampBuild( wCmd_, wFTU_, hwnd_, wParam_, lParam_ );
		}

		case SCMD_CONSTRUCT_INPUTMSG:
			return _ML_SCMDConstructMSG(wFTU_, hwnd_, lParam_);

		case SCMD_EDIT_INPUTMSG:
			if( ((LPFIA)lParam_)->wPid == 1 )
				return _ML_SCMDEditMSG(wFTU_, hwnd_, lParam_);
			else
				return DefStampBuild( wCmd_, wFTU_, hwnd_, wParam_, lParam_ );

#ifdef _WIN32
		case SCMD_MOUSEONBODY:
		{
			DWORD	_ret = MOUSEON_NULL;

			HRGN	_hrgn;
			HDC		_hdc;
			HPEN	_hPen;

			_hdc = GetDC( hwnd_ );

			//keep a thick pen in HDC
			_hPen = CreatePen( PS_SOLID, 10, RGB(255,0,0) );
			_hPen = SelectObject( _hdc, _hPen );

			BeginPath( _hdc );

			_DrawMultiLine(wFTU_, _hdc, TRUE, wParam_ );

			EndPath( _hdc );
			WidenPath( _hdc );

			_hPen = SelectObject( _hdc, _hPen );
			DeleteObject( _hPen );

			_hrgn = PathToRegion( _hdc );

			// {{ debug code
			// PaintRgn( _hdc, _hrgn );
			// }}

			_ret = PtInRegion( _hrgn, X(lParam_), Y(lParam_) )? MOUSEON_BODY: MOUSEON_NULL;

			DeleteObject( _hrgn );
			ReleaseDC( hwnd_, _hdc );

			return _ret;
		}
#else
		case SCMD_MOUSEONBODY:	//use default implementation
#endif

		case SCMD_GETREGION:
		case SCMD_UPDATE:
		case SCMD_MOUSEONHANDLE: //tests whether mouse is on handle
		case SCMD_TOGGLESELECTHANDLES:
		case SCMD_MOVE:
		default:
			return DefStampBuild( wCmd_, wFTU_, hwnd_, wParam_, lParam_ );
	}
	return TRUE;
}

//...................................................................
//Description:
//...................................................................
#pragma argsused
WORD _near _ML_SCMDEditMSG(WRD wFTU_, HWND hwnd_, LONG lParam_)
{
	WORD	_ret = FIA_PROCESSED;
	LPFIA	_lpFma = (LPFIA)lParam_;
	NPML	_npML = (NPML)_lpFma->lpPrivate;

	// _npML should be allocated when receiving SCMD_CONSTRUCT
	if( !_npML )
	{
		CORE_ASSERT( FALSE );
		return FIA_PROCESSEND;
	}

	switch( _lpFma->msg )
	{
		case WM_LBUTTONDOWN:
		{
			FCCRD	_dx, _dy;
			LPPU	_lpPrm, _lppuVar;
			_lpPrm = FaceGetParameter( wFTU_ );

			if( _lpFma->wParam & MK_CONTROL )
			{
				// if control is pressed
				_npML->wMode = _ML_APPEND;

				//get coor. of the last dot
				_lppuVar = _lpPrm + gcnumFix + (_npML->wNumDot - 1)*gcnumVar ;
				MFPUREAD( _lppuVar, ML_PX, FCCRD, _dx );
				MFPUREAD( _lppuVar, ML_PY, FCCRD, _dy );

				//update _lpFma->dtPrimary and _lpFma->dtLast
				_lpFma->dtPrimary.x = _dx;
				_lpFma->dtPrimary.y = _dy;
				_lpFma->dtLast.x = X(_lpFma->lParam);
				_lpFma->dtLast.y = Y(_lpFma->lParam);

				//draw an appending line
				MoveTo( _lpFma->hdc, _dx, _dy );
				LineTo( _lpFma->hdc, _lpFma->dtLast.x, _lpFma->dtLast.y );
			}
			else	//else test whether the mouse is on a edit-select-handle
			{
				//can't call defaultStampBuild with SCMD_MOUSEONHANDLE
				//because it processes as the rectangle
				_npML->wSESH = _MouseOnESH( wFTU_, _lpFma->wSHSize, _lpFma->lParam);
				if( _npML->wSESH == MOUSEON_NULL )
				{
					/*
					_npML->wMode = _ML_NOTHING; 	//do nothing

					//update _lpFma->dtPrimary and _lpFma->dtLast
					_lpFma->dtPrimary.x = X(_lpFma->lParam);
					_lpFma->dtPrimary.y = Y(_lpFma->lParam);
					_lpFma->dtLast.x = X(_lpFma->lParam);
					_lpFma->dtLast.y = Y(_lpFma->lParam);
					*/

					//End edit here:
					//
					//this function adjust the screen coor. to relative coor.
					//and release the private data and restore HDC
					_Construct_WM_LBUTTONDBLCLK( wFTU_, hwnd_, _lpFma );

					_lpFma->wPid = 0;	//retore the Pid
					_ret = FIA_PROCESSEND;
				}
				else
				{
					_npML->wMode = _ML_EDIT;
					//wipe the line
					_lppuVar = _lpPrm + gcnumFix + (_npML->wSESH)*gcnumVar ;
					MFPUREAD( _lppuVar, ML_PX, FCCRD, _dx );
					MFPUREAD( _lppuVar, ML_PY, FCCRD, _dy );

					//update _lpFma->dtPrimary and _lpFma->dtLast
					_lpFma->dtPrimary.x = _dx;
					_lpFma->dtPrimary.y = _dy;
					_lpFma->dtLast.x = _dx;
					_lpFma->dtLast.y = _dy;
				}
			}
			break;
		}

		case WM_LBUTTONUP:
		{
			if( _npML->wMode == _ML_EDIT )
			{
				LPPU	_lpPrm, _lppuVar;

				_lpPrm = FaceGetParameter( wFTU_ );

				//wipe the old edit-select-handles
				_ToggleMLSH( _lpFma->hdc, wFTU_, _lpFma->wSHSize );

				//change the point coor.
				_lppuVar = _lpPrm + gcnumFix + _npML->wSESH * gcnumVar;
				MFPUWRITE( _lppuVar, ML_PX, FCCRD, X(_lpFma->lParam) );
				MFPUWRITE( _lppuVar, ML_PY, FCCRD, Y(_lpFma->lParam) );

				//show the new edit-select-handles
				_ToggleMLSH( _lpFma->hdc, wFTU_, _lpFma->wSHSize );
			}
			else if( _npML->wMode == _ML_APPEND )
			{
				if( _lpFma->wParam & MK_CONTROL )	//check the Ctrl key again
				{
					//wipe the old edit-select-handles
					_ToggleMLSH( _lpFma->hdc, wFTU_, _lpFma->wSHSize );

					//if the key is pressed still treat as same as CONSTRUCT
					_Construct_WM_LBUTTONUP( wFTU_, hwnd_, _lpFma );

					//draw the new edit-select-handles
					_ToggleMLSH( _lpFma->hdc, wFTU_, _lpFma->wSHSize );
				}
				else	//ctrl key is released and abort action
				{
					FCCRD	_dx, _dy;
					LPPU	_lpPrm, _lppuVar;

					//wipe the line
					_lpPrm = FaceGetParameter( wFTU_ );
					_lppuVar = _lpPrm + gcnumFix + (_npML->wNumDot - 1)*gcnumVar ;
					MFPUREAD( _lppuVar, ML_PX, FCCRD, _dx );
					MFPUREAD( _lppuVar, ML_PY, FCCRD, _dy );
					MoveTo( _lpFma->hdc, _dx, _dy );
					LineTo( _lpFma->hdc, _lpFma->dtLast.x, _lpFma->dtLast.y );
				}
			}

			_npML->wMode = _ML_NOTHING;
			break;
		}

		case WM_MOUSEMOVE:
		{
			if( _npML->wMode == _ML_EDIT )
			{
				LPPU	_lpPrm, _lppuVar;
				FCCRD	_dx, _dy;

				_lpPrm = FaceGetParameter( wFTU_ );

				if( _npML->wSESH )	//not the first dot
				{
					_lppuVar = _lpPrm + gcnumFix + (_npML->wSESH - 1)*gcnumVar;
					MFPUREAD( _lppuVar, ML_PX, FCCRD, _dx );
					MFPUREAD( _lppuVar, ML_PY, FCCRD, _dy );

					//wipe the old line
					MoveTo( _lpFma->hdc, _dx, _dy );
					LineTo( _lpFma->hdc, _lpFma->dtLast.x, _lpFma->dtLast.y );
					//draw the new line
					MoveTo( _lpFma->hdc, _dx, _dy );
					LineTo( _lpFma->hdc, X(_lpFma->lParam), Y(_lpFma->lParam) );
				}

				if( _npML->wSESH != _npML->wNumDot - 1 )	//not the last dot
				{
					_lppuVar = _lpPrm + gcnumFix + (_npML->wSESH + 1)*gcnumVar;
					MFPUREAD( _lppuVar, ML_PX, FCCRD, _dx );
					MFPUREAD( _lppuVar, ML_PY, FCCRD, _dy );
					//wipe the old line
					MoveTo( _lpFma->hdc, _lpFma->dtLast.x, _lpFma->dtLast.y );
					LineTo( _lpFma->hdc, _dx, _dy );
					//draw the new line
					MoveTo( _lpFma->hdc, X(_lpFma->lParam), Y(_lpFma->lParam) );
					LineTo( _lpFma->hdc, _dx, _dy );
				}

				_lpFma->dtLast.x = X(_lpFma->lParam);
				_lpFma->dtLast.y = Y(_lpFma->lParam);
			}
			else if( _npML->wMode == _ML_APPEND )
			{
				_Construct_WM_MOUSEMOVE( wFTU_, hwnd_, _lpFma );
			}
			else	//_ML_NOTHING
			{
				//update lpFma_->dtPrimary and lpFma_->dtLast
				_lpFma->dtPrimary.x = X(_lpFma->lParam);
				_lpFma->dtPrimary.y = Y(_lpFma->lParam);
				_lpFma->dtLast.x = X(_lpFma->lParam);
				_lpFma->dtLast.y = Y(_lpFma->lParam);
			}
			break;
		}

		case WM_LBUTTONDBLCLK:
			if(_npML->wDbl == 0)	//the first DBLCLK, editing begins now!
			{
				LPPU	_lpPrm, _lppuVar;
				WORD	_wNumParam;
				FCCRD	_w, _h, _x0, _y0;
				RECT	_rect;
				int		_i;
				FCCRD	_dx, _dy;

				_npML->wDbl ++;

				//toggle off the default select-handles
				DefStampBuild(SCMD_TOGGLESELECTHANDLES,
							  wFTU_,
							  hwnd_,
							  _lpFma->wSHSize,
							  MAKELONG( _lpFma->hdc, NULL ) );

				//init. hdc
				_npML->iOldMode = SetROP2( _lpFma->hdc, R2_NOT );
				_npML->hOldPen = SelectObject( _lpFma->hdc, GetStockObject( BLACK_PEN ) );

				//update _lpFma->dtPrimary and _lpFma->dtLast
				_lpFma->dtPrimary.x = X(_lpFma->lParam);
				_lpFma->dtPrimary.y = Y(_lpFma->lParam);
				_lpFma->dtLast.x = X(_lpFma->lParam);
				_lpFma->dtLast.y = Y(_lpFma->lParam);

				//restore the dot coor.
				_lpPrm = FaceGetParameter( wFTU_ );
				_wNumParam = FaceGetNumParam( wFTU_ );
				CORE_ASSERT(_lpPrm);

				FaceXycxcyToRect(wFTU_, &_rect);
				_w = _rect.right - _rect.left;
				_h = _rect.bottom - _rect.top;
				_x0 = _rect.left;
				_y0 = _rect.top;

				_npML->wNumDot = (_wNumParam - gcnumFix)/gcnumVar;

				for( _i=0, _lppuVar = _lpPrm + gcnumFix;
					 _i < _npML->wNumDot;
					 _i++, _lppuVar += gcnumVar )
				{
					MFPUREAD( _lppuVar, ML_PX, FCCRD, _dx );
					MFPUREAD( _lppuVar, ML_PY, FCCRD, _dy );

					_dx = _dx*_w + _x0;
					_dy = _dy*_h + _y0;

					MFPUWRITE( _lppuVar, ML_PX, FCCRD, _dx );
					MFPUWRITE( _lppuVar, ML_PY, FCCRD, _dy );
				}

				//show edit-select-handles
				_ToggleMLSH( _lpFma->hdc, wFTU_, _lpFma->wSHSize );
			}
			else
			{
				//the second DBLCLK, editing edit;
				//end construction

				//this function adjust the screen coor. to relative coor.
				//and release the private data and restore HDC
				_Construct_WM_LBUTTONDBLCLK( wFTU_, hwnd_, _lpFma );

				_lpFma->wPid = 0;	//retore the Pid
				_ret = FIA_PROCESSEND;
			}
			break;

		default:
			_ret = FIA_NOTPROCESSED;
	}

	return _ret;


}

//...................................................................
//Description:
//...................................................................
#pragma argsused
WORD _near _ML_SCMDConstructMSG(WRD wFTU_, HWND hwnd_, LONG lParam_)
{
	WORD	_ret = FIA_PROCESSED;
	LPFIA	_lpFma = (LPFIA)lParam_;
	NPML	_npML = (NPML)_lpFma->lpPrivate;

	// _npML should be allocated when receiving SCMD_CONSTRUCT
	if( !_npML )
	{
		CORE_ASSERT( FALSE );
		return FIA_DELETEFACE;
	}

	switch( _lpFma->msg )
	{
		case WM_LBUTTONDOWN:
		{
			if( _npML->wNumDot == 0 )
			{
				//this is beginning point to construct a new multiline object
				//init. hdc
				_npML->iOldMode = SetROP2( _lpFma->hdc, R2_NOT );
				_npML->hOldPen = SelectObject( _lpFma->hdc, GetStockObject( BLACK_PEN ) );

				//update _lpFma->dtPrimary and _lpFma->dtLast
				_lpFma->dtPrimary.x = X(_lpFma->lParam);
				_lpFma->dtPrimary.y = Y(_lpFma->lParam);
				_lpFma->dtLast.x = X(_lpFma->lParam);
				_lpFma->dtLast.y = Y(_lpFma->lParam);
			}
			break;
		}

		case WM_LBUTTONUP:
		{
			_Construct_WM_LBUTTONUP( wFTU_, hwnd_, _lpFma );
			break;
		}

		case WM_MOUSEMOVE:
		{
			_Construct_WM_MOUSEMOVE( wFTU_, hwnd_, _lpFma );
			break;
		}

		case WM_LBUTTONDBLCLK:
		{
			//end construction
			//adjust the screen coor. to relative coor.

			_Construct_WM_LBUTTONDBLCLK( wFTU_, hwnd_, _lpFma );
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
//	the coor. of dots is edit-time coor. (screen coor.)
//...................................................................
static void near _ToggleMLSH( HDC hdc_, WRD wFTU_, WORD wSHSize_ )
{
	int			_old_mode;
	HBRUSH		_hBrush;
	HPEN		_hPen;

	int			_i;

	FCCRD		_dx, _dy;
	LPPU		_lpPrm, _lppuVar;
	WORD		_wNumParam;

	_lpPrm = FaceGetParameter( wFTU_ );
	_wNumParam = FaceGetNumParam( wFTU_ );
	CORE_ASSERT(_lpPrm);

	_old_mode = SetROP2( hdc_, R2_NOT );
	_hBrush = SelectObject( hdc_, GetStockObject(BLACK_BRUSH) );
	_hPen = SelectObject( hdc_, GetStockObject(NULL_PEN) );

	for( _i=0, _lppuVar = _lpPrm + gcnumFix;
		 _i < (_wNumParam - gcnumFix)/gcnumVar;
		 _i++, _lppuVar += gcnumVar )
	{
		MFPUREAD( _lppuVar, ML_PX, FCCRD, _dx );
		MFPUREAD( _lppuVar, ML_PY, FCCRD, _dy );

		Rectangle( hdc_,
				   _dx - wSHSize_,
				   _dy - wSHSize_,
				   _dx + wSHSize_,
				   _dy + wSHSize_ );
	}

	SetROP2(hdc_, _old_mode);
	SelectObject( hdc_, _hBrush );
	SelectObject( hdc_, _hPen );

	return ;
}

//...................................................................
//inner function
//Note:
//	the coor of the is edit-time coor.( screen coor. )
//...................................................................
static int near _MouseOnESH( WRD wFTU_, WORD wSHSize_, LONG point_ )
{
	int			_i;

	FCCRD		_dx, _dy;
	LPPU		_lpPrm, _lppuVar;
	WORD		_wNumParam;

	_lpPrm = FaceGetParameter( wFTU_ );
	_wNumParam = FaceGetNumParam( wFTU_ );
	CORE_ASSERT(_lpPrm);

	for( _i=0, _lppuVar = _lpPrm + gcnumFix;
		 _i < (_wNumParam - gcnumFix)/gcnumVar;
		 _i++, _lppuVar += gcnumVar )
	{
		MFPUREAD( _lppuVar, ML_PX, FCCRD, _dx );
		MFPUREAD( _lppuVar, ML_PY, FCCRD, _dy );

		if(   (abs(_dx - X(point_) ) <= wSHSize_)
		   && (abs(_dy - Y(point_) ) <= wSHSize_) )
		   return _i;
	}

	return MOUSEON_NULL;
}

//...................................................................
//...................................................................
static void near _Construct_WM_LBUTTONDBLCLK( WRD wFTU_, HWND hwnd_, LPFIA lpFma_ )
{
	int		_i;
	LPPU	_lpPrm, _lppuVar;
	FCCRD	_x, _y;
	RECT	_rc;
	NPML	_npML = (NPML)lpFma_->lpPrivate;

	//end construction / edit
	//adjust the screen coor. to relative coor.

	CORE_ASSERT( FaceGetNumParam(wFTU_) == gcnumFix + _npML->wNumDot * gcnumVar );

	_lpPrm = FaceGetParameter( wFTU_ );

	for( _i=0, _lppuVar = _lpPrm + gcnumFix;
		 _i < _npML->wNumDot;
		 _i++, _lppuVar += gcnumVar )
	{
		MFPUREAD( _lppuVar, ML_PX, FCCRD, _x);
		MFPUREAD( _lppuVar, ML_PY, FCCRD, _y);

		if( _i == 0)	//set init. value for _rc
		{
			_rc.left = _rc.right = _x;
			_rc.top = _rc.bottom = _y;
		}
		else
		{
			if( _rc.left > _x )	_rc.left = _x;
			if( _rc.top > _y ) _rc.top = _y;
			if( _rc.right < _x )	_rc.right = _x;
			if( _rc.bottom < _y ) _rc.bottom = _y;
		}
	}
	WriteFaceXYCXCY( wFTU_, &_rc );	//write the x, y, cx, cy

	for( _i=0, _lppuVar = _lpPrm + gcnumFix;
		 _i<_npML->wNumDot;
		 _i++, _lppuVar += gcnumVar )
	{
		MFPUREAD( _lppuVar, ML_PX, FCCRD, _x);
		MFPUREAD( _lppuVar, ML_PY, FCCRD, _y);

		_x = (_x - _rc.left) / (_rc.right - _rc.left);
		_y = (_y - _rc.top) / (_rc.bottom - _rc.top);

		MFPUWRITE( _lppuVar, ML_PX, FCCRD, _x);
		MFPUWRITE( _lppuVar, ML_PY, FCCRD, _y);
	}

	FaceSetRect( wFTU_, &_rc );
	
	//update FTU.rect
	if( IsFaceSelect( wFTU_ ) )
		InflateRect( &_rc, lpFma_->wSHSize, lpFma_->wSHSize );

	//free the private data
	SetROP2( lpFma_->hdc, _npML->iOldMode );
	SelectObject( lpFma_->hdc, _npML->hOldPen );
	LocalFree( (HLOCAL)_npML );
	lpFma_->lpPrivate = NULL;

	InvalidateRect( hwnd_, &_rc, TRUE );
	UpdateWindow( hwnd_ );

	return;
}


//...................................................................
//...................................................................
#pragma argsused
static void near _Construct_WM_LBUTTONUP( WRD wFTU_, HWND hwnd_, LPFIA lpFma_ )
{
	LPPU	_lpPrm, _lppuVar;
	NPML	_npML = (NPML)lpFma_->lpPrivate;
	WORD	_wNumParam;
	int		_temp;

	_npML->wNumDot++;

	_wNumParam = FaceGetNumParam( wFTU_ );

	_temp = (_wNumParam - gcnumFix) - _npML->wNumDot * gcnumVar; 
	CORE_ASSERT( (_temp==0) || (_temp==(-1*gcnumVar)) );

	if( _temp < 0 )
		if( !FaceAppendParam( wFTU_, gcnumVar ) ) 
			return;

	_lpPrm = FaceGetParameter( wFTU_ );
	_lppuVar = _lpPrm + gcnumFix + (_npML->wNumDot-1) * gcnumVar;

	//initialize the new parameters
	_lppuVar->prefix = PF_IMM|IMM_FCCRD;
	(_lppuVar+1)->prefix = PF_IMM|IMM_FCCRD;

	//update lpFma_->dtPrimary and lpFma_->dtLast
	lpFma_->dtPrimary.x = X(lpFma_->lParam);
	lpFma_->dtPrimary.y = Y(lpFma_->lParam);
	lpFma_->dtLast.x = X(lpFma_->lParam);
	lpFma_->dtLast.y = Y(lpFma_->lParam);

	//record the screen coor. of a point in multiline
	MFPUWRITE( _lppuVar, ML_PX, FCCRD, X(lpFma_->lParam) );
	MFPUWRITE( _lppuVar, ML_PY, FCCRD, Y(lpFma_->lParam) );

	return;
}

//...................................................................
//...................................................................
#pragma argsused
static void near _Construct_WM_MOUSEMOVE(WRD wFTU_, HWND hwnd_, LPFIA lpFma_ )
{
	NPML	_npML = (NPML)lpFma_->lpPrivate;

	if( _npML->wNumDot )	//if _npML->wNumDot > 0
	{
		//clear old line
		MoveTo( lpFma_->hdc, lpFma_->dtPrimary.x, lpFma_->dtPrimary.y );
		LineTo( lpFma_->hdc, lpFma_->dtLast.x, lpFma_->dtLast.y );

		//set lpFma_->dtLast
		lpFma_->dtLast.x = X(lpFma_->lParam);
		lpFma_->dtLast.y = Y(lpFma_->lParam);

		//draw new line
		MoveTo( lpFma_->hdc, lpFma_->dtPrimary.x, lpFma_->dtPrimary.y );
		LineTo( lpFma_->hdc, lpFma_->dtLast.x, lpFma_->dtLast.y );
	}

	return;
}

static BOOL _DrawMultiLine(WRD wFTU_, 
						   HDC hdc_, 
						   BOOL bTestMode_, 
						   int iPresion_ /* ONLY valid while bTestMode_ == TRUE */)
{
	COLORREF	_clr;
	int			_width;
	int			_style;
	RECT		_rect;
	int			_i;
	WORD		_flag;	//for test whether normal or edit-mode

	int			_old_mode;

	int			_w, _h, _x0, _y0;
	FCCRD		_dx, _dy;
	HPEN		_hPen;
	LPPU		_lpPrm, _lppuVar;
	WORD		_wNumParam;

	_lpPrm = FaceGetParameter( wFTU_ );
	_wNumParam = FaceGetNumParam( wFTU_ );
	CORE_ASSERT(_lpPrm);

	FaceXycxcyToRect(wFTU_, &_rect);
	_w = _rect.right - _rect.left;
	_h = _rect.bottom - _rect.top;
	_x0 = _rect.left;
	_y0 = _rect.top;

	MFPUREAD( _lpPrm, ML_FLAG, WORD, _flag );
	if( _flag )//edit flag
	{
		CORE_ASSERT( !bTestMode_ );

		_old_mode = SetROP2( hdc_, R2_NOT );
		_hPen = SelectObject( hdc_, GetStockObject(BLACK_PEN) );
	}
	else	//normal drawing
	{
		if( bTestMode_ )
		{
			CORE_ASSERT( iPresion_ > 1 );

			_hPen = CoreCreatePen(PS_SOLID, 10 /* iPresion_*2 */, RGB(255,0,0) );
		}
		else
		{
			MFPUREAD( _lpPrm, ML_PENCLR, COLORREF, _clr );
			MFPUREAD( _lpPrm, ML_PENWIDTH, int, _width );
			MFPUREAD( _lpPrm, ML_PENSTYLE, int, _style );

			_hPen = CoreCreatePen(_style, _width, _clr);
		}
		_hPen = SelectObject(hdc_, _hPen);
	}

	_lppuVar = _lpPrm + gcnumFix;
	MFPUREAD( _lppuVar, ML_PX, FCCRD, _dx );
	MFPUREAD( _lppuVar, ML_PY, FCCRD, _dy );
	_dx = _dx*_w + _x0;
	_dy = _dy*_h + _y0;
	MoveToEx( hdc_, _dx, _dy, NULL );

	for( _i=1, _lppuVar += gcnumVar;	//Note: beginning at 1 because param0 has been read!
		 _i < (int)((_wNumParam - gcnumFix)/gcnumVar);
		 _i++, _lppuVar += gcnumVar )
	{
		MFPUREAD( _lppuVar, ML_PX, FCCRD, _dx );
		MFPUREAD( _lppuVar, ML_PY, FCCRD, _dy );

		_dx = _dx*_w + _x0;
		_dy = _dy*_h + _y0;
		LineTo( hdc_, _dx, _dy );
	}

	_hPen = SelectObject(hdc_, _hPen);
	if( _flag )
	{   //edit mode
		SetROP2( hdc_, _old_mode );
		//the pen is stock object, need no to delete
		//the flag is valid only one time, so recover it
		MFPUWRITE( _lpPrm, ML_FLAG, WORD, 0 );
	}
	else
		CoreDeletePen(_hPen);

	return TRUE;
}
