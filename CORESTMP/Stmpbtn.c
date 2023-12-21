//===================================================================
//
//Zheng zhikai	96-10-28
//===================================================================
#include "prehead.h"

#include "coreall.h"
#include "stmpdef.h"

static BOOL _near _DrawButton( WRD wFTU_, HDC hdc_, WORD wMode_ );

#define BUTTON_TITLE				(FACE_FIRSTPARAM + 0)
#define BUTTON_TITLE_PDT			PDT_LPCOBJ

//...................................................................
//...................................................................
_DLLEXP BOOL _DLLFUNC ButtonPaint(WRD wFTU_, HDC hdc_)
{
	//draw the button at normal state
	return _DrawButton( wFTU_, hdc_, 0 );
}

//...................................................................
//...................................................................
_DLLEXP DWORD _DLLFUNC ButtonParam(PIC wCmd_, WRD wFTU_, WORD wNo_, LONG lParam_)
{
	static PDU _sPDU[] =
	{
		_PDU_DEFAULT,
		_ELE_PDU(BUTTON_TITLE,   "Title",   VSOF_LPCOBJ_STR,	PU_NORMAL, PRI_ALL|PDUF_NODEFAULT ),
		//_ELE_PDU(BUTTON_TRIGGER, "Trigger", VSOF_WRDMTU,		PU_NORMAL, PRI_ALL|PDUF_NODEFAULT ),
		//_ELE_PDU(BUTTON_ENABLE,  "Enable", 	VSOF_BOOL,			PU_NORMAL, PRI_ALL )
	};

	switch(wCmd_)
	{
		default:
		{
			DWORD _ret;
			_ret = _DefStampParam(wCmd_,
								  wFTU_,
								  wNo_,
								  lParam_,
								  sizeof(_sPDU)/sizeof(PDU),
								  IDS_BUTTON,
								  &(_sPDU[0]) );
			if( _ret && (wCmd_ == PIC_INITIALIZEPARAM) && (wNo_ == BUTTON_TITLE) )
			{
				LPPU		_lpPrm;
				LPCOBJ		_lpCobjDefault;
				BOOL		_bOk;

				_lpPrm  = FaceGetParameter( wFTU_ );
				_lpCobjDefault = AllocCString( "Button", &_bOk );
				if( !_bOk )
					return FALSE;
				MFPUWRITE(_lpPrm, BUTTON_TITLE, LPCOBJ, _lpCobjDefault );
			}

			return _ret;
		}
	}
}

//...................................................................
//...................................................................
_DLLEXP DWORD _DLLFUNC ButtonBuild(SCMD wCmd_, WRD wFTU_, HWND hwnd_, WORD wParam_, LONG lParam_)
{
	switch(wCmd_)
	{
		case SCMD_RUNTIME:
			return (RT_CAPTURE|RT_HDC|RT_WANTTRIGGERS);

		case SCMD_RUNTIME_INPUTMSG:
		{
			LPFIA			_lpFia = (LPFIA)lParam_;
			WORD			_ret = FIA_PROCESSED;

			switch( _lpFia->msg )
			{
				case WM_LBUTTONDOWN:
					_DrawButton( wFTU_, _lpFia->hdc, 1 );
					break;

				case WM_LBUTTONUP:
				{
					LPPU	_lpPrm;
					WRD		_trigger;
					RECT	_rect;
					POINT	_pt;

					_DrawButton( wFTU_, _lpFia->hdc, 0 );

					FaceXycxcyToRect(wFTU_, &_rect);
					_lpPrm  = FaceGetParameter( wFTU_ );
					MFPUREAD( _lpPrm, FACE_TRIGGER, WRDMTU, _trigger );

					//initiate a new modu thread
					ASSIGN_POINT( _pt, _lpFia->lParam )
					if( _trigger && PtInRect( &_rect, _pt ) )
						InitiateModuThread( _trigger );

					_ret = FIA_PROCESSEND;
					break;
				}

				case WM_KEYDOWN:
					_DrawButton( wFTU_, _lpFia->hdc, 1 );

					if( _lpFia->MsgCode & MSG_TRIGGER )
					{
						LPPU	_lpPrm;
						WRD		_trigger;

						_lpPrm  = FaceGetParameter( wFTU_ );
						MFPUREAD( _lpPrm, FACE_TRIGGER, WRDMTU, _trigger );

						//initiate a new modu thread
						if( _trigger  )
							InitiateModuThread( _trigger );
					}
					FaceSetRedraw( wFTU_ );
					_ret = FIA_PROCESSEND;
					break;

				default:
					break;
			}
			return _ret;
		}

		default:
			return DefStampBuild( wCmd_, wFTU_, hwnd_, wParam_, lParam_ );
	}

	return TRUE;
}

//...................................................................
//Entry:
//	if wMode_ == 1
//		draw the button under clicking( push down ) state
//...................................................................
static BOOL _near _DrawButton( WRD wFTU_, HDC hdc_, WORD wMode_ )
{
	RECT		_rect;
	HPEN		_hPen;
	HBRUSH		_hBrush;
	WORD		_3d_mode;
	COLORREF	_rgbText;

	LPPU		_lpPrm;
	LPCOBJ		_lpCobj;

	_lpPrm = FaceGetParameter(wFTU_);
	if( ! _lpPrm )
	{
		CORE_ASSERT( FALSE );
		return FALSE;
	}

	FaceXycxcyToRect(wFTU_, &_rect);
	MFPUREAD( _lpPrm, BUTTON_TITLE, LPCOBJ, _lpCobj );

	_hPen = SelectObject(hdc_, GetStockObject(BLACK_PEN) );
	_hBrush = CoreCreateBrush( CCLR_3D );
	_hBrush = SelectObject(hdc_, _hBrush );

	CclrGetRGB( CCLR_3DTEXT, &_rgbText );
	SetTextColor( hdc_, _rgbText );
	
	Rectangle( hdc_, _rect.left, _rect.top, _rect.right, _rect.bottom );

	if( _lpCobj )
	{
		int	_old_bk = SetBkMode( hdc_, TRANSPARENT );

		if( wMode_ == 1 )
		{
			_rect.left += 2;
			_rect.top += 2;
		}

		DrawText( hdc_,
				  (LPSTR)GET_COBJ_DATA( _lpCobj ),
				  -1,
				  &_rect,
				  DT_CENTER|DT_VCENTER|DT_SINGLELINE );
		SetBkMode( hdc_, _old_bk );

		if( wMode_ == 1 )
		{
			_rect.left -= 2;
			_rect.top -= 2;
		}
	}

	if( wMode_ == 1 )
		_3d_mode = RECT3D_DOWN|RECT3D_BH_BS;
	else
		_3d_mode = RECT3D_UP|RECT3D_BH_BS;

	InflateRect( &_rect, -1, -1 );
	Draw3DRect( hdc_, &_rect, _3d_mode);
	InflateRect( &_rect, -1, -1 );
	Draw3DRect( hdc_, &_rect, _3d_mode );

	_hPen = SelectObject(hdc_, _hPen);
	_hBrush = SelectObject(hdc_, _hBrush);
	CoreDeleteBrush( _hBrush );

	return TRUE;
}

