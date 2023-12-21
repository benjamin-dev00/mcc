//===================================================================
//
//Zheng zhikai	95-4-22
//
//Description:
//	This API implements for CGDI extend;
//===================================================================

#include "prehead.h"

#include "coreall.h"
#include "cgdi.h"

//...................................................................
//...................................................................
_DLLEXP HPEN _DLLFUNC CoreCreatePen( int fnPenStyle, int nWidth, CCLR clr_ )
{
	switch( clr_ & CLR_MASK )
	{
		case CLR_RGB:
			return CreatePen( fnPenStyle, nWidth, L24B(clr_) );

		case CLR_SYS:
			return CreatePen( fnPenStyle, nWidth, GetSysColor( L24B(clr_) ) );

		case CLR_CORE:
			if( clr_ == CCLR_NULL )
				return GetStockObject( NULL_PEN );
			else
			{
				LOGWARN1( "illegal CoreCreatePen param:0x%8X", clr_ ); //printf
				return GetStockObject( BLACK_BRUSH );
			}

//there is only NULL color now
		default:
		{
			LOGWARN1( "illegal CoreCreatePen param:0x%8X", clr_ ); //printf
			return GetStockObject( BLACK_PEN );
		}
	}
}

//...................................................................
//...................................................................
_DLLEXP BOOL _DLLFUNC CoreDeletePen( HPEN hPen_ )
{
	return DeleteObject( hPen_ );
}

//...................................................................
//...................................................................
_DLLEXP HBRUSH _DLLFUNC CoreCreateBrush( CCLR clr_ )
{
	switch( clr_ & CLR_MASK )
	{
		case CLR_RGB:
			return CreateSolidBrush( L24B(clr_) );

		case CLR_SYS:
			return GetSysColorBrush( L24B(clr_)  );

		case CLR_CORE:
			if( CCLR_NULL == clr_ )
				return GetStockObject( NULL_BRUSH );
			else
			{
				LOGWARN1( "illegal CoreCreateBrush param:0x%8X", clr_ ); //printf
				return GetStockObject( BLACK_BRUSH );
			}

//there is only NULL color now
		default:
		{
			LOGWARN1( "illegal CoreCreateBrush param:0x%8X", clr_ ); //printf
			return GetStockObject( BLACK_BRUSH );
		}
	}
}

//...................................................................
//...................................................................
_DLLEXP BOOL _DLLFUNC CoreDeleteBrush( HBRUSH hbr_ )
{
	return DeleteObject( hbr_ );
}

//...................................................................
//...................................................................
_DLLEXP BOOL _DLLFUNC IsTransparentColor( COLORREF clr_ )
{
	switch( clr_ & CLR_MASK )
	{
		case CLR_CORE:
		{
			if( CCLR_NULL == clr_ )
				return TRUE;
		}
	}
	return FALSE;
}

//...................................................................
//Description:
//	Gets the pen real width in pixel;
//...................................................................
_DLLEXP DWORD _DLLFUNC CoreGetPenWidth( int nPenStyle_, CCLR clrPen_, DWORD width_org_ )
{
	if( nPenStyle_ < ZLS_BEGIN )
	{
		if( clrPen_ == CCLR_NULL )
			return 0;

		if( nPenStyle_ == PS_NULL )
			return 0;
		else
			return width_org_;
	}

	switch( nPenStyle_ )
	{
		case ZLS_BUMP:
		case ZLS_DIP:
			return HIWORD(width_org_) + 2*LOWORD(width_org_);

		case ZLS_RAISED:
		case ZLS_SUNKEN:
			return HIWORD(width_org_) + LOWORD(width_org_);

		default:
			//invalid pen style
			return width_org_;
	}
}

//...................................................................
//...................................................................
_DLLEXP BOOL _DLLFUNC CoreFillRect( HDC hdc_, LPRECT lprc_, CCLR clrFill_ )
{
	HBRUSH	_hbr;
	//int		_mode;

	//_mode = SetBkMode( hdc_, TRANSPARENT );
	_hbr = CoreCreateBrush( clrFill_ );
	if( !_hbr )
		return FALSE;

	FillRect( hdc_, lprc_, _hbr );
	CoreDeleteBrush( _hbr );
	//_mode = SetBkMode( hdc_, _mode );

	return TRUE;
}

//...................................................................
//...................................................................
_DLLEXP BOOL _DLLFUNC CoreDrawBox( HDC hdc_, LPRECT lprc_, int nPenStyle_, DWORD dwPenWidth_, CCLR clr_ )
{
	HBRUSH		_hBrushOld;
	HPEN		_hPen, _hPenOld;
	WORD		_verW, _gougeW;
	RECT		_rc;
	int			_i;

	CopyRect( &_rc, lprc_ );

	if( nPenStyle_ < ZLS_BEGIN )
	{
		//standard windows style pen
		if( clr_ == CCLR_NULL )
			_hPen = GetStockObject( NULL_PEN );
		else
			_hPen = CreatePen( nPenStyle_, (int)dwPenWidth_, L24B(clr_) );

		_hPenOld = SelectObject(hdc_, _hPen);
		_hBrushOld = SelectObject(hdc_, GetStockObject(NULL_BRUSH) );

		Rectangle( hdc_, _rc.left, _rc.top, _rc.right, _rc.bottom );

		SelectObject(hdc_, _hBrushOld);
		SelectObject(hdc_, _hPenOld);
		DeleteObject(_hPen);

		return TRUE;
	}

	_verW = LOWORD( dwPenWidth_ );
	_gougeW = HIWORD( dwPenWidth_ );
	if( !_verW ) _verW++;

	switch( nPenStyle_ )
	{
		case ZLS_BUMP:
		case ZLS_DIP:
		{
			WORD	_3dmode1, _3dmode2;

			//draw 3D style rectanglar box with standard MS Windows 3D color
			_3dmode1 = (nPenStyle_ == ZLS_DIP)? RECT3D_DOWN|RECT3D_BH_BS : RECT3D_UP|RECT3D_BH_BS;
			_3dmode2 = (nPenStyle_ == ZLS_DIP)? RECT3D_UP|RECT3D_BH_BS : RECT3D_DOWN|RECT3D_BH_BS;

			//draw out-side side
			for( _i=0; _i<_verW; _i++)
			{
				Draw3DRect( hdc_, &_rc, _3dmode1 );
				InflateRect( &_rc, -1, -1 );
			}

			//draw mid gouge
			if( _gougeW && (clr_ != CCLR_NULL) )
			{
				HRGN	_hrgnD, _hrgn1, _hrgn2;
				HBRUSH	_hbr;

				_hrgnD = CreateRectRgn( 0, 0, 0, 0 );
				_hrgn1 = CreateRectRgnIndirect( &_rc );

				_hrgn2 = CreateRectRgn( _rc.left + _gougeW,
										_rc.top + _gougeW,
										_rc.right - _gougeW,
										_rc.bottom - _gougeW);

				CombineRgn( _hrgnD, _hrgn1, _hrgn2, RGN_DIFF );
				_hbr = CoreCreateBrush( clr_ );
				FillRgn( hdc_, _hrgnD, _hbr );
				DeleteObject( _hrgnD );
				DeleteObject( _hrgn1 );
				DeleteObject( _hrgn2 );
				DeleteObject( _hbr );
			}

			//draw in-side side
			InflateRect( &_rc, -1*_gougeW, -1*_gougeW );
			for( _i=0; _i<_verW; _i++)
			{
				Draw3DRect( hdc_, &_rc, _3dmode2 );
				InflateRect( &_rc, -1, -1 );
			}
			return TRUE;
		}

		case ZLS_RAISED:
		case ZLS_SUNKEN:
		{
			WORD	_3dmode;

			_3dmode = (nPenStyle_ == ZLS_RAISED)? RECT3D_UP|RECT3D_BH_BS : RECT3D_DOWN|RECT3D_BH_BS;

			//draw outside border
			if( _gougeW && (clr_ != CCLR_NULL) )
			{
				HRGN	_hrgnD, _hrgn1, _hrgn2;
				HBRUSH	_hbr;

				_hrgnD = CreateRectRgn( 0, 0, 0, 0 );
				_hrgn1 = CreateRectRgnIndirect( &_rc );

				_hrgn2 = CreateRectRgn( _rc.left + _gougeW,
										_rc.top + _gougeW,
										_rc.right - _gougeW,
										_rc.bottom - _gougeW);

				CombineRgn( _hrgnD, _hrgn1, _hrgn2, RGN_DIFF );
				_hbr = CoreCreateBrush( clr_ );
				FillRgn( hdc_, _hrgnD, _hbr );
				DeleteObject( _hrgnD );
				DeleteObject( _hrgn1 );
				DeleteObject( _hrgn2 );
				DeleteObject( _hbr );
			}

			//draw raised or retuse
			InflateRect( &_rc, -1*_gougeW, -1*_gougeW );
			for( _i=0; _i<_verW; _i++)
			{
				Draw3DRect( hdc_, &_rc, _3dmode );
				InflateRect( &_rc, -1, -1 );
			}
			return TRUE;
		}

		default:
		{
			LOGWARN1("illegal line style:%d", nPenStyle_ );
			return FALSE;
		}
	}
}

_DLLEXP BOOL _DLLFUNC CclrGetRGB( CCLR clr_, COLORREF FAR* lpRgb_ )
{
	switch( clr_ & CLR_MASK )
	{
		case CLR_RGB:
			*lpRgb_ = L24B(clr_);
			break;

		case CLR_SYS:
			*lpRgb_ = GetSysColor( L24B(clr_) );
			break;

		case CLR_CORE:
			if( clr_ == CCLR_NULL )
				return FALSE;

			//else...
			//NO break here, 

		default:
		{
			*lpRgb_ = 0;	//set to black
			LOGWARN1( "illegal CclrGetRGB param:0x%8X", clr_ ); 
			break;
		}
	}

	return TRUE;
}
