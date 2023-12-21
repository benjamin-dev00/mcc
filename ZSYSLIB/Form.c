//===================================================================
//
//Zheng zhikai	97-1-9
//===================================================================

#include "prehead.h"
#include "form.h"

#include "zsysres.h"

#define _ID_EDITOR	0X100

BOOL _GetColTitle( LPFORM lpForm_, LPSTR lpText_, DWORD size_, UINT col_ );
BOOL _GetRowTitle( LPFORM lpForm_, LPSTR lpText_, DWORD size_, UINT row_ );

LRESULT CALLBACK ZzkFormWndProc(HWND hwnd_, UINT msg_, WPARAM wParam_, LPARAM lParam_);

void _InvalidateCell(HWND hwnd_, int row_, int colomn_);
BOOL _UpdateScroll( HWND hwnd_, BOOL bUpdate_ );
BOOL _UpdateHorScroll( HWND hwnd_, BOOL bUpdate_ );
BOOL _UpdateVerScroll( HWND hwnd_, BOOL bUpdate_ );
int _GetLeftVisibleColomn(HWND hwnd_, int col_);
int _GetRightVisibleColomn(HWND hwnd_, int col_);
BOOL _GetCellRect( HWND hwnd_, int row_, int col_, LPRECT lprc_ );
SIZE _GetFormSize( HWND hwnd_ );
void _HideEdit( HWND hwnd_ );

static int _GetLeftColBorder(HWND hwnd_, UINT colomn_);

static void _ValidateFocus( HWND hwnd_ );
static void _ToggleFormFocus(HWND hwnd, UINT OnOff_ /* WM_KILLFOCUS or not */ );
static BOOL _ChangeFocus( HWND hwnd_, int NewFocusRow_, int NewFocusColomn_ );
static void _EditFormCell(HWND hwnd_);
static int _IsFocusVisible(HWND hwnd_);
static void _ScrollFocusVisible(HWND hwnd_);
static BOOL _GetFormRect( HWND hwnd_, LPRECT lprc_ );
static void _InvalidateTopGap( HWND hwnd_ );
static void _InvalidateLeftGap( HWND hwnd_ );
static void _SetColWidth(LPFORM lpForm_, UINT colomn_, int width_ );
static int  _GetColWidth(LPFORM lpForm_, UINT colomn_);
static void _AdjustColWidth( HWND hwnd_, POINT pt_ );

static void _WMLButtonDown(HWND hwnd_, WPARAM wParam_, LPARAM lParam_);
static void _WMKeyDown(HWND hwnd_, WPARAM wParam_, LPARAM lParam_);
static void _WMPaint(HWND hwnd_);
static void _WMVScroll(HWND hwnd_, WPARAM wParam_, LPARAM lParam_);
static void _WMHScroll(HWND hwnd_, WPARAM wParam_, LPARAM lParam_);
static void _WMLButtonDblClk(HWND hwnd_, WPARAM wParam_, LPARAM lParam_);

static void _InvalidateFormZone(HWND hwnd_, UINT row_, UINT colomn_);

static int	sFormLineHeight = 18;

#define		HORZ_SCALE	(1)

extern HINSTANCE ghInst;

//...................................................................
//Description:
//...................................................................
LRESULT CALLBACK ZzkFormWndProc(HWND hwnd_, UINT msg_, WPARAM wParam_, LPARAM lParam_)
{
	LPFORM		_lpForm;

	switch (msg_)
	{
		case WM_CREATE:
		{
			FORM FAR*		_lpse;

			//create an accosiation between this window and its table
			_lpse = (FORM FAR*)( ((CREATESTRUCT FAR*)lParam_)->lpCreateParams);
			_lpForm = (LPFORM) AllocGlobalPtr( sizeof(FORM) );
			if( !_lpForm )
				return -1;	//creating failed

			//copy the data and reset the inner elements value
			memcpy( _lpForm, _lpse, sizeof(FORM) );
			_lpForm->left = 0;
			_lpForm->FirstRow = 0;
			_lpForm->FocusRow = NO_SEL;
			_lpForm->FocusCol = NO_SEL;
			_lpForm->bFocus = FALSE;
			_lpForm->hwndEditor = NULL;
			_lpForm->bEditLoseFocus = FALSE;

			if( _lpForm->WidthRowTitle < 1 )
				_lpForm->WidthRowTitle = 1;	

			//create a pop editor window and hide it
			_lpForm->hwndEditor = CreateWindow( "Edit", "",
						  WS_CHILD|ES_LEFT|ES_AUTOHSCROLL|WS_BORDER,
						  0, 0, 0, 0,
						  hwnd_,
						  (HMENU)_ID_EDITOR,	//menu
						  ((CREATESTRUCT FAR*)lParam_)->hInstance,
						  NULL );
			if( !_lpForm->hwndEditor )
			{
				FreeGlobalPtr( _lpForm );
				return -1;	//creating failed!
			}
			//set the font
			SendMessage( _lpForm->hwndEditor, WM_SETFONT, (WPARAM)GetStockObject(ANSI_VAR_FONT), 0 );
			SendMessage( _lpForm->hwndEditor, EM_LIMITTEXT, 255, 0 );

			if( _lpForm->bEqualWidthCol )
				_lpForm->ColWidth = _lpse->ColWidth;
			else
			{
				_lpForm->pColWidth = (int FAR*)AllocGlobalPtr( _lpse->Cols*sizeof(int) );
				if( !_lpForm->pColWidth )
				{
					FreeGlobalPtr( _lpForm );
					return -1;	//creating failed!
				}
				memcpy( _lpForm->pColWidth, _lpse->pColWidth, _lpse->Cols*sizeof(int) );
			}

			_lpForm->hwnd = hwnd_;
			_lpForm->left = _lpForm->WidthRowTitle;	//initialize it

			SetWindowLong( hwnd_, 0, (LONG)_lpForm );
			_ValidateFocus( hwnd_ );
			break;
		}

		case WM_SIZE:
		{
			_UpdateScroll(hwnd_, FALSE);
			//_AdjustLastColomnWidth( hwnd_ );
			break;
		}

		case WM_HSCROLL:
			_WMHScroll(hwnd_, wParam_, lParam_);
			break;

		case WM_VSCROLL:
			_WMVScroll(hwnd_, wParam_, lParam_);
			break;

		case WM_SETFOCUS:
		{
			HWND	_hwndParent;
			UINT	_id;

			//notify parent window
			_hwndParent = GetParent( hwnd_ );
			if( !_hwndParent )
				break;	//no parent; 
			_id = GET_WINDOW_ID(hwnd_);

			PostCommandMessage( _hwndParent, _id, SHN_SETFOCUS, hwnd_ );

			_lpForm = (LPFORM)GetWindowLong(hwnd_, 0);
			if( !_lpForm )
				break;

			if( IsWindowVisible( _lpForm->hwndEditor ) )
				SetFocus( _lpForm->hwndEditor );
			else
				_ToggleFormFocus(hwnd_, msg_);
			break;
		}

		case WM_KILLFOCUS:
		{
			HWND	_hwndParent;
			UINT	_id;

			_ToggleFormFocus(hwnd_, msg_);

			//notify parent window
			_hwndParent = GetParent( hwnd_ );
			if( !_hwndParent )
				break;	//no parent;
			_id = GET_WINDOW_ID( hwnd_ );

			PostCommandMessage( _hwndParent, _id, SHN_KILLFOCUS, hwnd_ );
			break;
		}

		case WM_CLOSE:
			_lpForm = (LPFORM)GetWindowLong(hwnd_, 0);
			_lpForm->hwnd = NULL;
			LocalFree( (HLOCAL)_lpForm->pColWidth );
			LocalFree( (HLOCAL)_lpForm );
			SetWindowWord(hwnd_, 0, 0L);

			//TBEditCheck();
			return DefWindowProc (hwnd_, msg_, wParam_, lParam_);

		case WM_PAINT:
			_WMPaint(hwnd_);
			break;

		case WM_KEYDOWN:
			//process keyboard interface
			//Arrow keys:	move the focus of the form
			//Return:		enter edit mode
			_WMKeyDown(hwnd_, wParam_, lParam_);
			break;

		case WM_LBUTTONDOWN:	//this msg can change focus of the form
			_WMLButtonDown(hwnd_, wParam_, lParam_);
			break;

		case WM_LBUTTONDBLCLK:
			_WMLButtonDblClk(hwnd_, wParam_, lParam_);
			break;

		case WM_GETDLGCODE:	//for this window used for DialogBox
			return DLGC_WANTARROWS;	//DLGC_WANTALLKEYS;	//
			//break;

		case WM_COMMAND:
		{
			UINT _id = GET_WM_COMMAND_CTRL_ID( wParam_, lParam_ );
			UINT _nc = GET_WM_COMMAND_NOTIFY_CODE( wParam_, lParam_ );

			if( _id == _ID_EDITOR )
			{
				if( _nc == EN_KILLFOCUS )
				{
					_lpForm = (LPFORM)GetWindowLong(hwnd_, 0);
					_lpForm->bEditLoseFocus = TRUE;
					break;
					/*
					if( IsWindowVisible( _lpForm->hwndEditor ) )
					{
						break;
					}
					*/
				}

				if( _nc == EN_SETFOCUS )
				{
					_lpForm = (LPFORM)GetWindowLong(hwnd_, 0);
					if( !IsWindowVisible( _lpForm->hwndEditor ) )
					{
						SetFocus( hwnd_ );
						break;
					}
				}
			}
			return DefWindowProc(hwnd_, msg_, wParam_, lParam_);
		}

		//user defined messages:
		case SM_GET_DATA:
		{
			LPFORM	_lpForm = (LPFORM)GetWindowLong(hwnd_, 0);
			return _lpForm->lData;
		}

		case SM_GET_SEL:
		case SM_GET_ACTIVECELL:
		{
			LPFORM	_lpForm = (LPFORM)GetWindowLong(hwnd_, 0);
			return MAKELRESULT( _lpForm->FocusRow, _lpForm->FocusCol );
		}

		case SM_SET_ACTIVECELL:
		{
			LPFORM	_lpForm = (LPFORM)GetWindowLong(hwnd_, 0);
			UINT	_col, _row;

			_row = LOWORD( lParam_ );
			_col = HIWORD( lParam_ ); 
			if( _ChangeFocus( hwnd_, _row, _col ) )
			{
				_ScrollFocusVisible(hwnd_);
				return TRUE;
			}
			else
				return FALSE;
		}

		case SM_INVALIDATE:
			_InvalidateFormZone( hwnd_, wParam_, (WORD)lParam_ );
			break;

		case SM_GET_NUMBER_OF_ROW:
			_lpForm = (LPFORM)GetWindowLong(hwnd_, 0);
			return _lpForm->Rows;

		case SM_GET_NUMBER_OF_COL:
			_lpForm = (LPFORM)GetWindowLong(hwnd_, 0);
			return _lpForm->Cols;

		case SM_SET_NUMBER_OF_ROW:
			_lpForm = (LPFORM)GetWindowLong(hwnd_, 0);
			if( LOWORD( lParam_ ) == _lpForm->Rows )
				return TRUE;	//as same as the old value
			_lpForm->Rows = LOWORD( lParam_ );
			_ValidateFocus( hwnd_ );
			_UpdateScroll( hwnd_, TRUE );
			InvalidateRect( hwnd_, NULL, TRUE );
			UpdateWindow( hwnd_ );
			return TRUE;

		case SM_SET_NUMBER_OF_COL:
			_lpForm = (LPFORM)GetWindowLong(hwnd_, 0);
			if( LOWORD( lParam_ ) == _lpForm->Cols )
				return TRUE;	//as same as the old value
			if( LOWORD( lParam_ ) >= 32767 )
				return FALSE;
			else
			{
				//_wColsOld = _lpForm->Cols;
				if( !_lpForm->bEqualWidthCol )
				{
					//rerange the width of colomns
					int _FAR*	_lpiColWidthNew;

					_lpiColWidthNew = ReallocGlobalPtr( _lpForm->pColWidth, LOWORD(lParam_)*sizeof(WORD) );
					if( !_lpiColWidthNew )
						return FALSE;

					if( LOWORD( lParam_ ) > _lpForm->Cols )
					{
						WORD	_i;
						//colomns increased, the width of append colomns
						//initializes to width of the last colomn in the old form
						for( _i = _lpForm->Cols; _i<LOWORD( lParam_ ); _i++ )
							_lpiColWidthNew[_i] = _lpiColWidthNew[_lpForm->Cols - 1];
					}
					//set to new value
					_lpForm->pColWidth = _lpiColWidthNew;
				}

				_lpForm->Cols = LOWORD( lParam_ );
				_lpForm->left = _lpForm->WidthRowTitle;	//re-initialize it
				_ValidateFocus( hwnd_ );
				_UpdateScroll( hwnd_, TRUE );
				InvalidateRect( hwnd_, NULL, TRUE );
				UpdateWindow(hwnd_ );
				return TRUE;
			}

		case SM_GET_WIDTH_OF_COL:
			_lpForm = (LPFORM)GetWindowLong(hwnd_, 0);
			if( !_lpForm->bEqualWidthCol )
			{
				if( wParam_ >= (WPARAM)_lpForm->Cols )
					return FALSE;
				else
					return (_lpForm->pColWidth)[wParam_];
			}
			else
				return _lpForm->ColWidth;

		case SM_SET_WIDTH_OF_COL:
			_lpForm = (LPFORM)GetWindowLong(hwnd_, 0);
			if( LOWORD( lParam_ ) >= 32767 )
				return FALSE;
			if( !_lpForm->bEqualWidthCol )
			{
				if( wParam_ >= (WPARAM)_lpForm->Cols )
					return FALSE;
				else
				{
					(_lpForm->pColWidth)[wParam_] = LOWORD( lParam_ );
					_UpdateScroll( hwnd_, TRUE );
					return TRUE;
				}
			}
			else
			{
				_lpForm->ColWidth = LOWORD( lParam_ );
				_UpdateScroll( hwnd_, TRUE );
				return TRUE;
			}

		case WM_NCDESTROY:
		{
			//free memory
			_lpForm = (LPFORM)GetWindowLong(hwnd_, 0);
			if( !_lpForm->bEqualWidthCol )
				FreeGlobalPtr( _lpForm->pColWidth );

			FreeGlobalPtr( _lpForm );

			return DefWindowProc (hwnd_, msg_, wParam_, lParam_);
		}

		default:
			return DefWindowProc (hwnd_, msg_, wParam_, lParam_);
	}
	return FALSE;
}

//...................................................................
//Description:
//	Paint the form
//...................................................................
static void _WMPaint(HWND hwnd_)
{
	PAINTSTRUCT		_ps;
	HPEN 			_hPenOld, _hPenShadow, _hPenHrzBd;
	HBRUSH 			_hBrushOld, _hBrushTitle;
	int				_i;
	int				_posit;

#ifndef WIN32				// add by sc because of gettextextent function
	DWORD			_extent;
#else
	SIZE			_extent;
#endif
		
	RECT 			_rectClient;
	RECT 			_rect, _rect2;
	LPFORM			_lpForm;
	char			_szBuffer[32];
	BOOL			_ret;
	COLORREF		_clrOldFore, _clrBtnText;
	HDC				_hdc;
	//int			_width;
	DWORD			_style;		//window style
	CELLINFO		_cell;

	_cell.hwnd = hwnd_;

	_style = GetWindowLong( hwnd_, GWL_STYLE );
	_lpForm = (LPFORM)GetWindowLong(hwnd_, 0);

	_hPenShadow = CreatePen( PS_SOLID, 1, GetSysColor(COLOR_BTNSHADOW) );
	_hPenHrzBd = CreatePen( PS_SOLID, 1, GetSysColor(COLOR_WINDOWFRAME) );
	_hBrushTitle = CreateSolidBrush( GetSysColor(COLOR_BTNFACE) );
	_clrBtnText = GetSysColor(COLOR_BTNTEXT);

	GetClientRect(hwnd_, &_rectClient);
	BeginPaint(hwnd_, &_ps);
		_hdc = _ps.hdc;

		SetTextAlign( _hdc, TA_LEFT|TA_BOTTOM );
		SetBkMode( _hdc, TRANSPARENT );
		_hBrushOld = SelectObject( _hdc, _hBrushTitle );
		_hPenOld = SelectObject( _hdc, _hPenShadow );

		//draw the colomn title
		SetRect(&_rect, _lpForm->WidthRowTitle, 0, _ps.rcPaint.right, _lpForm->HeightColTitle );
		FillRect(_hdc, &_rect, _hBrushTitle);

		_rect.right ++;	_rect.bottom --;
		Draw3DRect(_hdc, &_rect, RECT3D_BH_BS|RECT3D_UP);
		_rect.right --;	_rect.bottom ++;

		//draw a black horz. border at the bottom of the title line
		SelectObject(_hdc, _hPenHrzBd);
		MoveTo(_hdc, _ps.rcPaint.left, _lpForm->HeightColTitle-1);
		LineTo(_hdc, _ps.rcPaint.right, _lpForm->HeightColTitle-1);

		_rect.bottom = _lpForm->HeightColTitle;
		_rect.top = 0;

		//draw the colomns titles
		//clip out the left-top rect;
		ExcludeClipRect( _hdc, 0, 0, _lpForm->WidthRowTitle-1, _lpForm->HeightColTitle-1 );
		SetTextColor( _hdc, _clrBtnText );
		for(_i=0; _i < (int)_lpForm->Cols; _i++)
		{
			int		_left = _GetLeftColBorder(hwnd_, _i);
			_rect.left = max( _left, _lpForm->WidthRowTitle );
			_rect.right = _GetLeftColBorder(hwnd_, _i+1);
			if(_rect.left >= _rect.right)
				continue;		//the colomn is at left of the row title
			if( _GetColTitle( _lpForm, _szBuffer, sizeof(_szBuffer), _i) )
			{
				ExtTextOut(_hdc,
					_left+2, _rect.bottom - 1,
					ETO_CLIPPED, &_rect,
					_szBuffer, strlen(_szBuffer),
					NULL);
			}

			//draw short vertical line between colomns
			CopyRect( &_rect2, &_rect );
			_rect2.left = _rect2.right - 1;
			_rect2.right += 1;
			Draw3DRect(_hdc, &_rect2, RECT3D_BH_BS|RECT3D_DOWN);
		}

		//draw Row-title colomn background
		SetRect(&_rect, 0, _lpForm->HeightColTitle, _lpForm->WidthRowTitle, _rectClient.bottom);
		FillRect(_hdc, &_rect, _hBrushTitle);

		//draw Row-title colomn 3D border  
		_rect.right --;	_rect.bottom ++;	//hide the bottom of 3D rect
		Draw3DRect(_hdc, &_rect, RECT3D_BH_BS|RECT3D_UP);
		_rect.right ++;	_rect.bottom --;

		//draw a black vertical border
		SelectObject(_hdc, _hPenHrzBd);
		MoveTo(_hdc, _rect.right-1, _ps.rcPaint.top);
		LineTo(_hdc, _rect.right-1, _ps.rcPaint.bottom);

		////////////////
		//the following code: draw data area

		//draw the horz. lines and fill data
		SelectObject(_hdc, _hPenShadow);	//select gray pen
		SelectObject(_hdc, GetStockObject(ANSI_VAR_FONT));
		SetTextColor( _hdc, GetSysColor(COLOR_WINDOWTEXT) );

		/////////////////////////////////////////////////////
		//draw the row title text and the contents of the row
		//

		_cell.Size = sizeof(_cell);
		_cell.hwnd = hwnd_;
		_cell.Event = FORMEV_GETTEXT|FORMEV_GETCOLOR;
		//	_cell.Row = ;	
		//	_cell.Col = ;
		_cell.lData = _lpForm->lData;
		_cell.lpText = _szBuffer;
		_cell.TextSize = sizeof(_szBuffer);
		//	_cell.clrText = ;
		//	_cell.clrBack = ;

		_i = _lpForm->FirstRow;
		_posit = _lpForm->HeightColTitle;
		while(  ( _posit <= _ps.rcPaint.bottom )
			  &&( _i < (int)_lpForm->Rows ) )
		{
			if( (long)(_posit + sFormLineHeight) >= _ps.rcPaint.top)
			{
				int	_j;	//_j is col loop var.

				_rect.top = _posit;
				_rect.bottom = _rect.top + sFormLineHeight;
				for(_j=0; _j<(int)_lpForm->Cols; _j++)
				{
					int		_leftj;

					_leftj = _GetLeftColBorder( hwnd_, _j);
					_rect.left = max(_leftj, _lpForm->WidthRowTitle );
					_rect.right = _GetLeftColBorder(hwnd_, _j + 1);
					if(_rect.right <= _rect.left)	//at this case, rect is invisible .
						continue;

					//initialize cell info struct
					_cell.Row = _i;
					_cell.Col = _j;
					_cell.clrText = FORM_DEFAULT_CLR;
					_cell.clrBack = FORM_DEFAULT_CLR;
					_ret = (_lpForm->pfnFormData)( &_cell );
					if( !_ret )
					{
						*_szBuffer = 0;
						//continue;
					}

					if(   ( ( _style & SHS_ROWSELECT ) && (_lpForm->FocusRow == _i) )	//ROWSELECT mode
						||( (_lpForm->FocusRow == _i) && (_lpForm->FocusCol == _j) ) )	//non-ROWSELECT mode
					{
						//draw focus area
						HBRUSH _hbrFillFocus = CreateSolidBrush( GetSysColor(COLOR_HIGHLIGHT) );
						FillRect(_hdc, &_rect, _hbrFillFocus );
						DeleteObject( _hbrFillFocus );
						_clrOldFore = SetTextColor(_hdc, GetSysColor(COLOR_HIGHLIGHTTEXT) );
						ExtTextOut(_hdc,
							_leftj+2, _rect.bottom+1,
							ETO_CLIPPED, &_rect,
							_szBuffer, strlen(_szBuffer),
							NULL);
						SetTextColor(_hdc, _clrOldFore);

						//form focus rectangle always be around the focus cell whether ROWSELECT mode or not
						if(    (_lpForm->FocusRow == _i) 
							&& (_lpForm->FocusCol == _j)  
							&& ( _lpForm->bFocus ) )
						{
							_rect.left++; _rect.top++;
							DrawFocusRect( _hdc, &_rect );
							_rect.left--; _rect.top--;
						}
					}
					else
					{
						if( _cell.clrText != FORM_DEFAULT_CLR )
							_clrOldFore = SetTextColor( _hdc, _cell.clrText );
						if( _cell.clrBack != FORM_DEFAULT_CLR )
						{
							HBRUSH	_brush = CreateSolidBrush( _cell.clrBack ); 
							FillRect( _hdc, &_rect, _brush );
							DeleteObject( _brush );
						}
						ExtTextOut(_hdc,
							_leftj+2, _rect.bottom+1,
							ETO_CLIPPED, &_rect,
							_szBuffer, strlen(_szBuffer),
							NULL);
						if( _cell.clrText != FORM_DEFAULT_CLR )
							_clrOldFore = SetTextColor( _hdc, _clrOldFore );
					}
				}

				//paint the row title
				if( (_ps.rcPaint.left <= _lpForm->WidthRowTitle) )
				{
					if( _GetRowTitle( _lpForm, _szBuffer, sizeof(_szBuffer), _i ) )
					{
#ifndef WIN32
						_extent = GetTextExtent( _hdc, _szBuffer, strlen(_szBuffer) );
						_rect.left = _lpForm->WidthRowTitle - LOWORD(_extent) - 4;	
#else
						GetTextExtentPoint32( _hdc, _szBuffer, strlen(_szBuffer),&_extent );
						_rect.left = _lpForm->WidthRowTitle - (_extent.cx) - 4;
#endif
						_rect.right = _lpForm->WidthRowTitle;
						_rect.bottom = _posit + sFormLineHeight;
						_rect.top = _posit;
						_clrOldFore = SetTextColor(_hdc, _clrBtnText);
						ExtTextOut(_hdc,
							_rect.left, _rect.bottom,
							ETO_CLIPPED, &_rect,
							_szBuffer, strlen(_szBuffer),
							NULL);
						SetTextColor(_hdc, _clrOldFore);
					}
				}

				//draw the horiz. lines
				if( _style & SHS_HORLINE )
				{
					MoveTo(_hdc, _ps.rcPaint.left, _posit + sFormLineHeight);
					LineTo(_hdc, _ps.rcPaint.right, _posit + sFormLineHeight);
				}
			}
			_i++;
			_posit += sFormLineHeight;
		}

		//draw the ver. lines
		if( _style & SHS_VERLINE )
			for(_i = 1; _i <= (int)_lpForm->Cols; _i++)
			{
				int	_border = _GetLeftColBorder(hwnd_, _i);
				if( _border > _lpForm->WidthRowTitle )
				{
					int _top = max( _lpForm->HeightColTitle, _ps.rcPaint.top );
					MoveTo(_hdc, _border, _top);
					LineTo(_hdc, _border, _ps.rcPaint.bottom);
				}
			}
		SelectObject(_hdc, _hPenOld);
		SelectObject(_hdc, _hBrushOld);

	EndPaint(hwnd_, &_ps);

	DeleteObject(_hPenShadow);
	DeleteObject(_hPenHrzBd);
	DeleteObject(_hBrushTitle);

	return;
}

//...................................................................
//Description:
//	Set/seset a focus zone in a table window.
//...................................................................
#pragma argsused
static void _ToggleFormFocus( HWND hwnd_, UINT OnOff_ )
{
	LPFORM		_lpForm = (LPFORM)GetWindowLong(hwnd_, 0);
	if( !_lpForm )
		return;

	if( OnOff_ == WM_KILLFOCUS )
		_lpForm->bFocus = FALSE;
	else	//WM_SETFOCUS
		_lpForm->bFocus = TRUE;

	_InvalidateCell( hwnd_, _lpForm->FocusRow, _lpForm->FocusCol );
	UpdateWindow(hwnd_);

	return;
}

//...................................................................
//...................................................................
#pragma argsused
static void _WMLButtonDown(HWND hwnd_, WPARAM wParam_, LPARAM lParam_)
{
	LPFORM		_lpForm = (LPFORM)GetWindowLong(hwnd_, 0);
	int			_xPos = (int)LOWORD(lParam_);
	int			_yPos = (int)HIWORD(lParam_);
	UINT		_FocusRow;
	UINT		_FocusColomn;
	RECT		_rcForm;
	POINT		_pt;

	if( !_lpForm->Rows || !_lpForm->Cols )
		return;
	else
		_ValidateFocus( hwnd_ );

	ASSIGN_POINT( _pt, lParam_ )
	
	_GetFormRect( hwnd_, &_rcForm );
	if( !PtInRect( &_rcForm, _pt ) )
	{
		//not in data area
		_AdjustColWidth( hwnd_, _pt );
		return;
	}

	SetFocus(hwnd_);
	if((_xPos <= _lpForm->WidthRowTitle) || (_yPos <= _lpForm->HeightColTitle))
		return;

	_FocusRow = (_yPos - _lpForm->HeightColTitle)/sFormLineHeight + _lpForm->FirstRow;
	for(_FocusColomn = 0; _FocusColomn < _lpForm->Cols; _FocusColomn++)
	{
		if( _GetLeftColBorder(hwnd_, _FocusColomn) >= _xPos )
			break;
	}
	_FocusColomn --;

	_ChangeFocus(hwnd_, _FocusRow, _FocusColomn );

	return;
}

static void _AdjustColWidth( HWND hwnd_, POINT pt_ )
{
	HCURSOR 	_hCursor;

	RECT		_rcClient;
	LPFORM		_lpForm = (LPFORM)GetWindowLong(hwnd_, 0);
	HDC			_hdc;
	HPEN		_hPen, _hPenOld;

	int			_x, _x0, _delta_width;
	UINT		_i;	//loop var.
	MSG			_msg;

	//calculate form col title area
	GetClientRect( hwnd_, &_rcClient );

	/*
	_rc.left = _lpForm->WidthRowTitle;
	_rc.top = 0;
	_rc.right = _rcClient.right;
	_rc.bottom = _lpForm->HeightColTitle;
	*/

	for( _i=0; _i<=_lpForm->Cols; _i++ )
	{
		_x = _GetLeftColBorder(hwnd_, _i );
		if( abs(_x - pt_.x) <= 3 )	//precise is 3
			goto _ADJUST;
	}

	return;	//mouse is not on colomn border

_ADJUST:

	_x0 = _x;
	SetCapture( hwnd_ );

	_hCursor = LoadCursor( ghInst, MAKEINTRESOURCE( IDC_ADJUST1 ) );

	SetCursor( _hCursor );

	{	//clip cursor
		RECT	_rcMouse;
		CopyRect( &_rcMouse, &_rcClient );
		ClientToScreen( hwnd_, (LPPOINT)&_rcMouse );
		ClientToScreen( hwnd_, ((LPPOINT)&_rcMouse) + 1 );
		_rcMouse.right -= 4;
		ClipCursor(&_rcMouse);
	}

	_hdc = GetDC( hwnd_ );
	SetROP2( _hdc, R2_NOT );
	_hPen = CreatePen( PS_SOLID, 1, 0 );
	_hPenOld = SelectObject( _hdc, _hPen );

	//local message loop
	while( TRUE )
	{
		if( !PeekMessage(&_msg, NULL, 0, 0, PM_REMOVE) )
			continue;

		//when receive a WM_QUIT, end msg. loop and return
		if(_msg.message == WM_QUIT)
		{
			//break this local message loop and sent the WM_QUIT to
			//the main message loop
			PostQuitMessage( _msg.wParam );
			break;              //break local msg. loop
		}
		else if( _msg.message == WM_MOUSEMOVE )
		{
			//clear the old line
			MoveTo( _hdc, _x, 0 );
			LineTo( _hdc, _x, _rcClient.bottom );

			_x = LOWORD( _msg.lParam );

			//draw the new line
			MoveTo( _hdc, _x, 0 );
			LineTo( _hdc, _x, _rcClient.bottom );
			continue;
		}
		else if( _msg.message == WM_LBUTTONUP )
		{
			//clear the old line
			MoveTo( _hdc, _x, 0 );
			LineTo( _hdc, _x, _rcClient.bottom );

			break;
		}

		//other message
		TranslateMessage(&_msg);
		DispatchMessage(&_msg);
	}

	SelectObject( _hdc, _hPenOld );
	DeleteObject( _hPen );
	ReleaseDC( hwnd_, _hdc );

	ClipCursor( NULL );
	ReleaseCapture();
	DestroyCursor( _hCursor );

	//change data

	_delta_width = _x - _x0;

	//if the next colomn width is 0
	//this adjust is explained to show the next colomn
	if(  ( _GetColWidth( _lpForm, _i ) == 0 )
	   &&( _delta_width > 0 )
	   &&( _i < _lpForm->Cols ) )
	{
		//the next col is hiden and delta > 0
		//extend the next colomn
		_SetColWidth( _lpForm, _i, _delta_width );
	}
	else if( _i == 0 )
	{
		//change row title width
		if( _lpForm->WidthRowTitle > 30000 )		//max width
			return;	//do nothing
		else
		{
			int _wWidthRowTitleOld = _lpForm->WidthRowTitle;

			_lpForm->WidthRowTitle += _delta_width;
			if( _lpForm->WidthRowTitle < 1 )
				_lpForm->WidthRowTitle = 1;

			_lpForm->left += _lpForm->WidthRowTitle - _wWidthRowTitleOld;
		}
	}
	else
	{
		if( _lpForm->bEqualWidthCol )
		{
			if( _lpForm->ColWidth > 30000 )		//max width
				return;	//do nothing
			else
			{
				_lpForm->ColWidth += _delta_width;
				if( _lpForm->ColWidth < 0 )
					_lpForm->ColWidth = 0;
			}
		}
		else
		{
			if( _lpForm->pColWidth[_i-1] > 30000 )		//max width
				return;	//do nothing
			else
				_lpForm->pColWidth[_i-1] += _delta_width;
				if( _lpForm->pColWidth[_i-1] < 0 )
					_lpForm->pColWidth[_i-1] = 0;
		}
	}

	//AdjustLastColomnWidth();
	_UpdateHorScroll( hwnd_, FALSE );

	//redraw it
	InvalidateRect( hwnd_, NULL, TRUE );
	UpdateWindow( hwnd_ );
	return;
}

//...................................................................
//...................................................................
#pragma argsused
static void _WMLButtonDblClk(HWND hwnd_, WPARAM wParam_, LPARAM lParam_)
{
	LPFORM		_lpForm = (LPFORM)GetWindowLong(hwnd_, 0);
	RECT		_rcForm;
	POINT		_pt;
	int			_row, _colomn;
	int			_xPos = (int)LOWORD(lParam_);
	int			_yPos = (int)HIWORD(lParam_);

	ASSIGN_POINT( _pt, lParam_ )
	
	_GetFormRect( hwnd_, &_rcForm );
	if( !PtInRect( &_rcForm, _pt ) )
		return;		//not in data area

	_row = (_yPos - _lpForm->HeightColTitle)/sFormLineHeight + _lpForm->FirstRow;
	for(_colomn = 0; _colomn < (int)_lpForm->Cols; _colomn++)
	{
		if( _GetLeftColBorder(hwnd_, _colomn) >= _xPos )
			break;
	}
	_colomn --;

	if( (_row == _lpForm->FocusRow) && (_colomn == _lpForm->FocusCol) )
		_EditFormCell(hwnd_);

	return;
}

//...................................................................
//Entry:
//	WORD	colomn_
//			if colomn_ = (WORD)(-1), invalidate the whole line.
//...................................................................
static void _InvalidateFormZone(HWND hwnd_, UINT row_, UINT colomn_)
{
	RECT		_rect;
	LPFORM		_lpForm = (LPFORM)GetWindowLong(hwnd_, 0);
	int			_d;

	if( !_lpForm ) 	//the windows has been closed
		return;

	//check for parameter legality
	if(  (row_ >= _lpForm->Rows)
	   ||(colomn_ > (_lpForm->Cols)) )
	{
		CORE_ASSERT(0);
		return ;
	}

	GetClientRect(hwnd_, &_rect);
	if( colomn_ != (WORD)(-1) )
	{
		_rect.left = _GetLeftColBorder(hwnd_, colomn_);
		_rect.right = _GetLeftColBorder(hwnd_, colomn_ + 1);
	}

	_d = (row_ - _lpForm->FirstRow);
	if((_d<0)||(_d > (_rect.bottom-_lpForm->HeightColTitle)/sFormLineHeight))
		return;

	_rect.top = _lpForm->HeightColTitle + sFormLineHeight * _d;
	_rect.bottom = _rect.top + sFormLineHeight;

	InvalidateRect(hwnd_, &_rect, TRUE);
	return;
}

//...................................................................
//...................................................................
static void _WMHScroll(HWND hwnd_, WPARAM wParam_, LPARAM lParam_)
{
	RECT 	_rc;
	int		_p;
	UINT	_i;
	LPFORM	_lpForm;
	BOOL	_bUpdate = TRUE;

	_lpForm = (LPFORM)GetWindowLong(hwnd_, 0);

	if( !_lpForm->Rows || !_lpForm->Cols )
		return;
	else
		_ValidateFocus( hwnd_ );

	GetClientRect(hwnd_, &_rc);

	switch( (int)LOWORD(wParam_) )
	{
		case SB_LINEDOWN:
		{
			RECT	_rcForm;
			int		_width;

			_GetFormRect( hwnd_, &_rcForm );
			_width = _rcForm.right - _rc.left + _lpForm->WidthRowTitle;

			if( _GetLeftColBorder(hwnd_, _lpForm->Cols) > _width )
			{
				_lpForm->left -= HORZ_SCALE;

				_rc.left = _lpForm->WidthRowTitle + HORZ_SCALE + 1;
				ScrollWindow(hwnd_, -HORZ_SCALE, 0, &_rc, NULL);
				_InvalidateLeftGap( hwnd_ );
				_bUpdate = TRUE;
			}
			break;
		}

		case SB_LINEUP:
		{
			if( _GetLeftColBorder(hwnd_, 0) < _lpForm->WidthRowTitle )
			{
				_lpForm->left += HORZ_SCALE;
				_rc.left = _lpForm->WidthRowTitle + 1;
				_rc.right -= HORZ_SCALE;
				ScrollWindow(hwnd_, HORZ_SCALE, 0, &_rc, NULL);
				_InvalidateLeftGap(hwnd_);
				_bUpdate = TRUE;
			}
			break;
		}

		case SB_PAGEDOWN:
			if( _GetLeftColBorder(hwnd_, _lpForm->Cols) > _lpForm->WidthRowTitle )
			{
				_i = 0;
				while( _GetLeftColBorder(hwnd_, _i) <= _lpForm->WidthRowTitle ) _i++;

				_p = _GetLeftColBorder(hwnd_, _i) - _lpForm->WidthRowTitle;
				_lpForm->left -= _p;

				if(_p < _rc.right)
				{
					_rc.left = _p + _lpForm->WidthRowTitle;
					ScrollWindow(hwnd_, -1* (_p), 0, &_rc, NULL);

					//Note: must invalidate the right of scroll area manually here
					//because the middle zone is invalid if the right of scroll area
					//scrolls to the left of its left side;
					_rc.left = _rc.right - _p;
					_rc.right = _rc.right;
					InvalidateRect( hwnd_, &_rc, TRUE );
					_InvalidateLeftGap(hwnd_);
				}
				else	//invalidate the whole client rect.
				{
					_rc.left = _lpForm->WidthRowTitle;
					InvalidateRect(hwnd_, &_rc, TRUE);
				}
				_bUpdate = TRUE;
			}
			break;

		case SB_PAGEUP:
			if( _GetLeftColBorder(hwnd_, 0) < _lpForm->WidthRowTitle )
			{
				_i = _lpForm->Cols;
				while( _GetLeftColBorder(hwnd_, _i ) >= _lpForm->WidthRowTitle ) _i--;

				_p = _lpForm->WidthRowTitle - _GetLeftColBorder(hwnd_, _i );
				_lpForm->left += _p;

				if(_p < _rc.right)
				{
					_rc.right -= _p;
					_rc.left = _lpForm->WidthRowTitle;
					ScrollWindow(hwnd_, _p, 0, &_rc, NULL);

					//need not call _InvalidateLeftGap() here because it has been
					//included into the following code;
					_rc.left = _lpForm->WidthRowTitle;
					_rc.right = _lpForm->WidthRowTitle + _p + 1;
					InvalidateRect(hwnd_, &_rc, TRUE);
					/*
					if((_p + _lpForm->WidthRowTitle) >= (_rc.right - _p))
					{
						_rc.left = (_rc.right - _p);
						_rc.right = _p + _lpForm->WidthRowTitle;
						InvalidateRect(hwnd_, &_rc, TRUE);
					}
					*/
				}
				else	//invalidate the whole client rect.
				{
					_rc.left = _lpForm->WidthRowTitle;
					InvalidateRect(hwnd_, &_rc, TRUE);
				}

				_bUpdate = TRUE;
			}
			break;

		case SB_THUMBPOSITION:
		{
#ifdef _WIN32
			short int _nPos = (short int) HIWORD(wParam_);  
#endif
			_p = _lpForm->WidthRowTitle - _nPos*HORZ_SCALE - _GetLeftColBorder(hwnd_, 0);
			_lpForm->left += _p;

			InvalidateRect(hwnd_, &_rc, TRUE);
			_bUpdate = TRUE;
			break;
		}

		//case SB_ENDSCROLL:
		default:
			break;
	}

	if( _bUpdate )
	{
		_UpdateHorScroll( hwnd_, TRUE );
		UpdateWindow( hwnd_ );
	}

	return;
}


//...................................................................
//...................................................................
static void _WMVScroll(HWND hwnd_, WPARAM wParam_, LPARAM lParam_)
{
	RECT 			_rc;
	unsigned		_uPageRecord;
	LPFORM			_lpForm;
	BOOL			_bUpdate = FALSE;

	_lpForm = (LPFORM)GetWindowLong(hwnd_, 0);

	GetClientRect(hwnd_, &_rc);
	_uPageRecord = (_rc.bottom - _lpForm->HeightColTitle) / sFormLineHeight - 1;

	switch( (int)LOWORD(wParam_) )
	{
		case SB_LINEDOWN:
			if( (int)(_lpForm->FirstRow + _uPageRecord + 1) < _lpForm->Rows )
			{
				_lpForm->FirstRow++;

				_rc.top = _lpForm->HeightColTitle + sFormLineHeight + 1;
				ScrollWindow(hwnd_, 0, -1* sFormLineHeight, &_rc, NULL);
				_InvalidateTopGap(hwnd_);
				_bUpdate = TRUE;
			}
			break;

		case SB_LINEUP:
			if(_lpForm->FirstRow > 0)
			{
				_lpForm->FirstRow--;

				_rc.top = _lpForm->HeightColTitle + 1;
				_rc.bottom -= sFormLineHeight;
				ScrollWindow(hwnd_, 0, sFormLineHeight, &_rc, NULL);
				_InvalidateTopGap(hwnd_);
				_bUpdate = TRUE;
			}
			break;

		case SB_PAGEDOWN:
			if(_lpForm->FirstRow < (int)_lpForm->Rows)
			{
				_lpForm->FirstRow += _uPageRecord;

				if(_lpForm->FirstRow > (int)_lpForm->Rows - 1)
					_lpForm->FirstRow = _lpForm->Rows - 1;

				_rc.top = _lpForm->HeightColTitle;
				InvalidateRect(hwnd_, &_rc, TRUE);
				_bUpdate = TRUE;
			}

			break;

		case SB_PAGEUP:
			if(_lpForm->FirstRow > 0)
			{
				if((unsigned)_lpForm->FirstRow < _uPageRecord)
					_lpForm->FirstRow = 0;
				else
					_lpForm->FirstRow -= _uPageRecord;

				_rc.top = _lpForm->HeightColTitle;
				InvalidateRect(hwnd_, &_rc, TRUE);
				_bUpdate = TRUE;
			}

			break;

		case SB_THUMBPOSITION:
		{
#ifdef _WIN32
			short int _nPos = (short int) HIWORD(wParam_);  
#endif
			if(_lpForm->FirstRow != _nPos )
			{
				_lpForm->FirstRow = _nPos;

				if(_lpForm->FirstRow > (int)_lpForm->Rows - 1)
					_lpForm->FirstRow = (int)_lpForm->Rows - 1;

				_rc.top = _lpForm->HeightColTitle;
				InvalidateRect(hwnd_, &_rc, TRUE);
				_bUpdate = TRUE;
			}
			break;
		}

		default:
			break;
	}

	if( _bUpdate )
	{
		_UpdateVerScroll( hwnd_, TRUE );
		UpdateWindow( hwnd_ );
	}
	return;
}

//...................................................................
//...................................................................
static int _GetColWidth(LPFORM lpForm_, UINT colomn_)
{
	if(colomn_ >= lpForm_->Cols)
	{
		return NULL;
	}

	if( lpForm_->bEqualWidthCol )
		return lpForm_->ColWidth;
	else
		return lpForm_->pColWidth[colomn_];
}

//...................................................................
//...................................................................
static void _SetColWidth(LPFORM lpForm_, UINT colomn_, int width_ )
{
	if(colomn_ >= lpForm_->Cols)
	{
		CORE_ASSERT( FALSE );
		return;
	}

	if( lpForm_->bEqualWidthCol )
		lpForm_->ColWidth = width_;
	else
		lpForm_->pColWidth[colomn_] = width_;

	return;
}

//...................................................................
//Description:
//	Retrieves the left border of the colomn_;
//Note:
//	If there are N colomns in form, (the colomn order is from 0 to N-1),
//	the caller can use colomn_ == N to get the right border of the last
//	colomn.
//...................................................................
static int _GetLeftColBorder(HWND hwnd_, UINT colomn_)
{
	LPFORM	_lpForm = (LPFORM)GetWindowLong(hwnd_, 0);
	int		_border = _lpForm->left;
	UINT	_i;

	CORE_ASSERT(colomn_ <= _lpForm->Cols);

	if(colomn_ > _lpForm->Cols)
		return NULL;

	if( _lpForm->bEqualWidthCol )
		_border += colomn_ * _lpForm->ColWidth;
	else
		for(_i=0; _i<colomn_; _i++)
			_border += _lpForm->pColWidth[_i];

	return _border;
}

//...................................................................
//Description:
//	Keyboard interface
//...................................................................
#pragma argsused
static void _WMKeyDown(HWND hwnd_, WPARAM wParam_, LPARAM lParam_)
{
	LPFORM	_lpForm = (LPFORM)GetWindowLong(hwnd_, 0);

	int		_NewFocusRow = _lpForm->FocusRow;
	int		_NewFocusCol = _lpForm->FocusCol;

	RECT 			_rc;
	unsigned		_uPageRecord;

	if( !_lpForm->Rows || !_lpForm->Cols )
		return;
	else
		_ValidateFocus( hwnd_ );

	GetClientRect(hwnd_, &_rc);
	_uPageRecord = (_rc.bottom - _lpForm->HeightColTitle) / sFormLineHeight - 1;

	//Note:
	//Need not consider the validities of _NewFocusRow or _NewFocusCol
	//after the change, because _ChangeFocus function will determine
	//their validities.
	switch( wParam_ )
	{
		case VK_UP:
			if( _NewFocusRow == 0 ) 
				return;
			_ScrollFocusVisible(hwnd_);
			_NewFocusRow -- ;
			_ChangeFocus(hwnd_, _NewFocusRow, _NewFocusCol);
			if( _IsFocusVisible(hwnd_) != 1 )
				SendMessage( hwnd_, WM_VSCROLL, SB_LINEUP, 0L );
			break;

		case VK_DOWN:
			_ScrollFocusVisible(hwnd_);
			_NewFocusRow ++ ;
			_ChangeFocus(hwnd_, _NewFocusRow, _NewFocusCol);
			if( _IsFocusVisible(hwnd_) != 1 )
				SendMessage( hwnd_, WM_VSCROLL, SB_LINEDOWN, 0L );
			break;

		case VK_LEFT:
			_NewFocusCol = _GetLeftVisibleColomn(hwnd_, _NewFocusCol);
			if( _NewFocusCol < 0 ) 
				return;

			_ScrollFocusVisible(hwnd_);
			_ChangeFocus(hwnd_, _NewFocusRow, _NewFocusCol);
			if( _IsFocusVisible(hwnd_) != 1 )
				SendMessage( hwnd_, WM_HSCROLL, SB_PAGEUP, 0L );
			break;

		case VK_RIGHT:
			_NewFocusCol = _GetRightVisibleColomn( hwnd_, _NewFocusCol);
			if( _NewFocusCol < 0 ) 
				return;

			_ScrollFocusVisible(hwnd_);
			_ChangeFocus(hwnd_, _NewFocusRow, _NewFocusCol);
			if( _IsFocusVisible(hwnd_) != 1 )
				SendMessage( hwnd_, WM_HSCROLL, SB_PAGEDOWN, 0L );
			break;

		case VK_PRIOR:
			_NewFocusRow -= _uPageRecord;
			if( _NewFocusRow < 0 )
				_NewFocusRow = 0;
			_ChangeFocus(hwnd_, _NewFocusRow, _NewFocusCol);
			if( _IsFocusVisible(hwnd_) != 1 )
				SendMessage( hwnd_, WM_VSCROLL, SB_PAGEUP, 0L );
			break;
				
		case VK_NEXT:
			_NewFocusRow += _uPageRecord;
			if( (UINT)_NewFocusRow >= _lpForm->Rows )
				_NewFocusRow = _lpForm->Rows - 1;
			_ChangeFocus(hwnd_, _NewFocusRow, _NewFocusCol);
			if( _IsFocusVisible(hwnd_) != 1 )
				SendMessage( hwnd_, WM_VSCROLL, SB_PAGEDOWN, 0L );
			break;
	
		case VK_RETURN:
		case VK_SPACE:
			_ScrollFocusVisible(hwnd_);
			_EditFormCell( hwnd_ );
			break;

		default:
			return;
	}

	return;
}

static void _ValidateFocus( HWND hwnd_ )
{
	LPFORM	_lpForm = (LPFORM)GetWindowLong(hwnd_, 0);
	int		_FocusRow = _lpForm->FocusRow;
	int		_FocusCol = _lpForm->FocusCol;

	if( _lpForm->Rows == 0 )
		_lpForm->FocusRow = NO_SEL;
	else if ( _lpForm->FocusRow == NO_SEL )
		_lpForm->FocusRow = 0;  
	else if( _lpForm->FocusRow >= (int)_lpForm->Rows )
		_lpForm->FocusRow = _lpForm->Rows - 1;

	if( _lpForm->Cols == 0 )
		_lpForm->FocusCol = NO_SEL;
	else if ( _lpForm->FocusCol == NO_SEL )
		_lpForm->FocusCol = 0;  
	else if( _lpForm->FocusCol >= (int)_lpForm->Cols )
		_lpForm->FocusCol = _lpForm->Cols - 1;

	if( (_lpForm->FirstRow<0) || (_lpForm->FirstRow > (int)_lpForm->Rows - 1) )
		_lpForm->FirstRow = 0;

	if( (_FocusRow != _lpForm->FocusRow) || (_FocusCol != _lpForm->FocusCol) )
	{
		PostCommandMessage( GetParent(hwnd_), 
							GetDlgCtrlID(hwnd_), 
							SHN_SELCHANGED, 
							(LPARAM)hwnd_ );
	}

	return;
}

//...................................................................
//Description:
//	Saves the new focus to the FORM, and redraws it
//Entries:
//	HWND hwnd_		the handle of the form window to be changed the focus
//	UINT NewFocusRow_
//					the WORD new focus record
//	UINT NewFocusColomn_
//					the new focus colomn
//...................................................................
static BOOL _ChangeFocus( HWND hwnd_, int NewFocusRow_, int NewFocusColomn_ )
{
	DWORD	_style = GetWindowLong( hwnd_, GWL_STYLE ); 
	LPFORM	_lpForm = (LPFORM)GetWindowLong(hwnd_, 0);

	//this is also a case to verify the validity of the current focus!
	_ValidateFocus( hwnd_ );

	if( !_lpForm->Rows || !_lpForm->Cols )
	{
		return (NewFocusRow_ == NO_SEL) && (NewFocusColomn_ == NO_SEL);
	}
	
	if(  (NewFocusRow_ >= (int)_lpForm->Rows)
	   ||(NewFocusColomn_ >= (int)_lpForm->Cols) )
	{
		return FALSE;
	}

	if(  (_lpForm->FocusRow != NewFocusRow_)
	   ||(_lpForm->FocusCol != NewFocusColomn_) )
	{
		RECT		_rcClient;

		GetClientRect( hwnd_, &_rcClient );

		//invalidate the old rect.
		if( _style & SHS_ROWSELECT )
			_InvalidateCell( hwnd_, _lpForm->FocusRow, -1 );
		else
			_InvalidateCell( hwnd_, _lpForm->FocusRow, _lpForm->FocusCol );

		//invalidate the new rect.
		if( _style & SHS_ROWSELECT )
			_InvalidateCell( hwnd_, NewFocusRow_, -1 );
		else
			_InvalidateCell( hwnd_, NewFocusRow_, NewFocusColomn_ );

		//save the new focus
		_lpForm->FocusRow = NewFocusRow_;
		_lpForm->FocusCol = NewFocusColomn_;
		UpdateWindow(hwnd_);

		//Notify parent
		PostCommandMessage( GetParent(hwnd_), 
							GetDlgCtrlID(hwnd_), 
							SHN_SELCHANGED, 
							hwnd_ );
	}

	return TRUE;
}

//...................................................................
//Description:
//...................................................................
static int _IsFocusVisible(HWND hwnd_)
{
	LPFORM	_lpForm = (LPFORM)GetWindowLong(hwnd_, 0);
	RECT			_rcForm, _rcFocus, _rcSect;

	if( !_GetFormRect( hwnd_, &_rcForm ) )
		return FALSE;

	_GetCellRect( hwnd_, _lpForm->FocusRow, _lpForm->FocusCol, &_rcFocus );

	if( !IntersectRect( &_rcSect, &_rcForm, &_rcFocus ) )
		return 0;	//thorough invisible

	if( EqualRect( &_rcSect, &_rcFocus ) )
		return 1;	//thorough visible
	else
		return 2;	//partly visible
}

//...................................................................
//Description:
//...................................................................
static void _ScrollFocusVisible(HWND hwnd_)
{
	LPFORM	_lpForm = (LPFORM)GetWindowLong(hwnd_, 0);
	RECT	_rcForm, _rcFocus;
	//int		_p;

	if( !_GetFormRect( hwnd_, &_rcForm ) )
		return;

	if( _IsFocusVisible(hwnd_) == 1 )
		return;		//the focus is thorough visible

	_GetCellRect( hwnd_, _lpForm->FocusRow, _lpForm->FocusCol, &_rcFocus );

	if( _rcFocus.right > _rcForm.right )
	{
		_lpForm->left += _rcForm.right - _rcFocus.right;

		//cell rectangle has been changed, update it!
		_GetCellRect( hwnd_, _lpForm->FocusRow, _lpForm->FocusCol, &_rcFocus );
	}
	if( _rcFocus.left < _rcForm.left )
		_lpForm->left += _rcForm.left - _rcFocus.left;

	if( _rcFocus.top < _rcForm.top )
		_lpForm->FirstRow = _lpForm->FocusRow;
	if( _rcFocus.bottom > _rcForm.bottom )
		_lpForm->FirstRow += (_rcFocus.bottom - _rcForm.bottom)/sFormLineHeight + 1;

	_UpdateScroll( hwnd_, FALSE );
	InvalidateRect( hwnd_, NULL, TRUE );
	UpdateWindow(hwnd_);
	return;
}

//return TRUE if the window has been updated
BOOL _UpdateScroll( HWND hwnd_, BOOL bUpdate_ )
{
	BOOL	_ret = 0;

	_ret |= _UpdateHorScroll( hwnd_, bUpdate_ );
	_ret |= _UpdateVerScroll( hwnd_, bUpdate_ );

	return _ret;
}

//return TRUE if the window has been updated
BOOL _UpdateHorScroll( HWND hwnd_, BOOL bUpdate_ )
{
	LPFORM		_lpForm = (LPFORM)GetWindowLong(hwnd_, 0);
	SIZE		_size = _GetFormSize(hwnd_);
	BOOL		_update = FALSE;
	SCROLLINFO	_si;
	RECT		_rc;

	if( _lpForm->bInHorScroll )
		return FALSE;
	_lpForm->bInHorScroll = TRUE;

	_HideEdit( hwnd_ );

	GetClientRect( hwnd_, &_rc );
	_rc.right -= _lpForm->WidthRowTitle;
	_rc.bottom -= _lpForm->HeightColTitle;

	//update horizontal scroll bar
	_si.cbSize = sizeof(_si);
	_si.fMask = SIF_ALL;
	_si.nMin = 0;
	_si.nMax = _size.cx / HORZ_SCALE;
	_si.nPage = _rc.right / HORZ_SCALE; 
	_si.nPos = -1 * ( ( _lpForm->left - _lpForm->WidthRowTitle ) / HORZ_SCALE );

	if( ((int)_si.nPage > 0) && ((int)_si.nPage < _si.nMax ) )
	{
		if( _si.nPos > (int)(_si.nMax - _si.nPage + 1) )		//pos is not in range
		{
			_si.nPos = _si.nMax - _si.nPage + 1;
			_lpForm->left = _lpForm->WidthRowTitle - _si.nPos * HORZ_SCALE;
			_update = TRUE;
		}
		SetScrollInfo( hwnd_, SB_HORZ, &_si, bUpdate_ );
	}
	else
	{
		_lpForm->left = _lpForm->WidthRowTitle;
		_update = TRUE;
		SetScrollRange( hwnd_, SB_HORZ, 0, 0, bUpdate_ );	//hide it
	}

	if( _update == TRUE )
	{
		//invalidate the whole window since the position of the form is changed.
		InvalidateRect( hwnd_, NULL, TRUE );
	}

	if( bUpdate_ )
		UpdateWindow(hwnd_);

	_lpForm->bInHorScroll = FALSE;
	return _update;
}

//return TRUE if the window has invalid rectangle
BOOL _UpdateVerScroll( HWND hwnd_, BOOL bUpdate_ )
{
	LPFORM		_lpForm = (LPFORM)GetWindowLong(hwnd_, 0);
	//SIZE		_size = _GetFormSize(hwnd_);
	BOOL		_update = FALSE;
	SCROLLINFO	_si;
	RECT		_rc;

	if( _lpForm->bInVerScroll )
		return FALSE;
	_lpForm->bInVerScroll = TRUE;

	_HideEdit( hwnd_ );

	GetClientRect( hwnd_, &_rc );
	_rc.right -= _lpForm->WidthRowTitle;
	_rc.bottom -= _lpForm->HeightColTitle;

	//update vertical scroll bar
	_si.cbSize = sizeof(_si);
	_si.fMask = SIF_ALL;
	_si.nMin = 0;
	_si.nMax = _lpForm->Rows - 1;
	_si.nPage = _rc.bottom / sFormLineHeight; 
	_si.nPos = _lpForm->FirstRow;
	if( ((int)_si.nPage > 0) && ((int)_si.nPage < _si.nMax ) )
	{
		if( _si.nPos > (int)(_si.nMax - _si.nPage + 1) )		//pos is not in range
		{
			_si.nPos = _si.nMax - _si.nPage + 1;
			_lpForm->FirstRow = _si.nPos;
			_update = TRUE;
		}
		SetScrollInfo( hwnd_, SB_VERT, &_si, bUpdate_ );
	}
	else
	{
		_lpForm->FirstRow = 0;
		_update = TRUE;
		SetScrollRange( hwnd_, SB_VERT, 0, 0, bUpdate_ ); //hide it
	}

	if( _update == TRUE )
	{
		//invalidate the whole window since the position of the form is changed.
		InvalidateRect( hwnd_, NULL, TRUE );
	}

	if( bUpdate_ )
		UpdateWindow(hwnd_);

	_lpForm->bInVerScroll = FALSE;
	return _update;
}

//...................................................................
//...................................................................
SIZE _GetFormSize( HWND hwnd_ )
{
	LPFORM	_lpForm = (LPFORM)GetWindowLong(hwnd_, 0);
	SIZE	_size;

	_size.cx = _GetLeftColBorder( hwnd_, _lpForm->Cols) - _lpForm->left;
	_size.cy = _lpForm->Rows * sFormLineHeight;

	return _size;
}

//...................................................................
//Description:
//Return:
//	if the form rectangle is empty, returns FALSE; otherwise
//	returns TRUE;
//...................................................................
static BOOL _GetFormRect( HWND hwnd_, LPRECT lprc_ )
{
	LPFORM	_lpForm = (LPFORM)GetWindowLong(hwnd_, 0);

	//calculate form rect
	GetClientRect( hwnd_, lprc_ );
	lprc_->left += _lpForm->WidthRowTitle;
	lprc_->top += _lpForm->HeightColTitle;

	if(  (lprc_->left >= lprc_->right)
	   ||(lprc_->top >= lprc_->bottom) )
	{
		SetRectEmpty( lprc_ );
		return FALSE;
	}
	else
		return TRUE;
}

//...................................................................
//Description:
//Return:
//	if the form rectangle is empty, returns FALSE; otherwise
//	returns TRUE;
//...................................................................
BOOL _GetCellRect( HWND hwnd_, int row_, int col_, LPRECT lprc_ )
{
	LPFORM	_lpForm = (LPFORM)GetWindowLong(hwnd_, 0);
	RECT	_rc;

	if( row_ == FORMTITLE )
	{
		_rc.top = 0;
		_rc.bottom = _lpForm->HeightColTitle;
	}
	else
	{
		_rc.top = (row_ - _lpForm->FirstRow) * sFormLineHeight + _lpForm->HeightColTitle;
		_rc.bottom = _rc.top + sFormLineHeight;
	}

	if( col_ == FORMTITLE )
	{
		_rc.left = 0;
		_rc.right = _lpForm->WidthRowTitle;
	}
	else
	{
		_rc.left = _GetLeftColBorder( hwnd_, col_ );
		_rc.right = _GetLeftColBorder( hwnd_, col_ + 1);
	}

	CopyRect( lprc_, &_rc );
	return TRUE;
}

//...................................................................
//Description:
//...................................................................
static void _InvalidateLeftGap( HWND hwnd_ )
{
	LPFORM	_lpForm = (LPFORM)GetWindowLong(hwnd_, 0);
	RECT	_rc;

	GetClientRect( hwnd_, &_rc );
	_rc.left = _lpForm->WidthRowTitle;
	_rc.right = _rc.left + 1;
	InvalidateRect( hwnd_, &_rc, TRUE );
	return;
}

//...................................................................
//Description:
//...................................................................
static void _InvalidateTopGap( HWND hwnd_ )
{
	LPFORM	_lpForm = (LPFORM)GetWindowLong(hwnd_, 0);
	RECT	_rc;

	GetClientRect( hwnd_, &_rc );
	_rc.top = _lpForm->HeightColTitle;
	_rc.bottom = _rc.top + 1;
	InvalidateRect( hwnd_, &_rc, TRUE );
	return;
}

//...................................................................
//Description:
//...................................................................
static void _EditFormCell( HWND hwnd_ )
{
	extern HWND		ghwndMDIClient;
	UINT			_iEditFlag;
	char			_szBuffer[256];
	UINT			_EditRow, _EditCol;

	LPFORM			_lpForm = (LPFORM)GetWindowLong(hwnd_, 0);
	RECT			_rc;
	MSG				_msg;
	HWND			_hwndFocusOld;

	CELLINFO	_cell;		
	
	_cell.Size = sizeof(_cell);
	_cell.hwnd = hwnd_;
	_cell.Event = FORMEV_CANEDIT;
	_cell.Row = _lpForm->FocusRow;
	_cell.Col = _lpForm->FocusCol;
	_cell.lData = _lpForm->lData;
	_cell.lpText = NULL;
	_cell.TextSize = NULL;
	_cell.clrText = 0;
	_cell.clrBack = 0;

	if( !_IsFocusVisible(hwnd_) )
		return;

	if(!(_lpForm->pfnFormData)( &_cell ) )
	{
		//Notify parent
		PostCommandMessage( GetParent(hwnd_), 
							GetDlgCtrlID(hwnd_), 
							SHN_DBLCLK, 
							(LPARAM)hwnd_ );
		return;
	}

	_cell.Size = sizeof(_cell);
	_cell.Event = FORMEV_GETEDITTEXT;
	_cell.Row = _lpForm->FocusRow;
	_cell.Col = _lpForm->FocusCol;
	_cell.lData = _lpForm->lData;
	_cell.lpText = (LPSTR)_szBuffer;
	_cell.TextSize = (DWORD)sizeof(_szBuffer);
	_cell.clrText = 0;
	_cell.clrBack = 0;

	if( !(_lpForm->pfnFormData)( &_cell ) )
		return;	//if get data error, return here

	_rc.left = _GetLeftColBorder(hwnd_, _lpForm->FocusCol );
	_rc.top = (_lpForm->FocusRow - _lpForm->FirstRow) * sFormLineHeight + _lpForm->HeightColTitle;
	_rc.right = _GetLeftColBorder(hwnd_, _lpForm->FocusCol + 1);
	_rc.bottom = _rc.top + sFormLineHeight;

	MoveWindow( _lpForm->hwndEditor, _rc.left, _rc.top, _rc.right-_rc.left, _rc.bottom - _rc.top, FALSE );
	SendMessage( _lpForm->hwndEditor, WM_SETTEXT, 0, (LPARAM)_szBuffer );
	ShowWindow( _lpForm->hwndEditor, SW_SHOW );
	_hwndFocusOld = GetFocus();
	SetFocus( _lpForm->hwndEditor );

	//before entering the loop, backup the focus row and col first
	//because the focus position maybe change after the editing
	//action ends;
	_EditRow = _lpForm->FocusRow;
	_EditCol = _lpForm->FocusCol;

	//
	//create local message-loop to simulate model dialogbox
	_iEditFlag = 0;
	while( TRUE )
	{
		if( _lpForm->bEditLoseFocus )	//this value set by form window precedure
		{
			_iEditFlag = IDOK;
			_hwndFocusOld = 0;	//the old focus window is invalid
			break;
		}

		if( !PeekMessage(&_msg, NULL, 0, 0, PM_REMOVE) )
			continue;

		//when receive a WM_QUIT, end msg. loop and return
		if(_msg.message == WM_QUIT)
		{
			_iEditFlag = IDCANCEL;

			//break this local message loop and sent the WM_QUIT to
			//the main message loop
			PostQuitMessage( _msg.wParam );
			break;              //break local msg. loop
		}

		//test the TBEdit special keys
		if(  _msg.hwnd == _lpForm->hwndEditor )
		{
			if( _msg.message == WM_SETFOCUS )
			{
				break;
			}

			if( _msg.message == WM_KILLFOCUS )
			{
				_iEditFlag = IDOK;
				_hwndFocusOld = 0;	//the old focus window is invalid
				break;
			}

			if( _msg.message == WM_KEYDOWN )
				switch( _msg.wParam )
				{
					case VK_ESCAPE:
						_iEditFlag = IDCANCEL;
						goto _END_LOOP;

					case VK_RETURN:
					case VK_TAB:
						_iEditFlag = IDOK;
						goto _END_LOOP;

					case VK_UP:
					case VK_DOWN:
					//case VK_LEFT:
					//case VK_RIGHT:
						_iEditFlag = IDOK;
						DispatchMessage( &_msg );
						goto _END_LOOP;
					//for other key press, do nothing
				}
		}

		/*
		because this code is shared by tw.exe and corebase.dll, and
		ghwndMDIClient can't be linked, so we ignore the MDI accel-key translateion
		*/
		/*
		if(! TranslateMDISysAccel(ghwndMDIClient, &_msg))
		{
			TranslateMessage(&_msg);
			DispatchMessage(&_msg);
		}
		*/
		TranslateMessage(&_msg);
		DispatchMessage(&_msg);

	}	//end loop

_END_LOOP:
	ShowWindow( _lpForm->hwndEditor, SW_HIDE );
	if( _hwndFocusOld )	//the focus has been changed
		SetFocus( _hwndFocusOld );
	_lpForm->bEditLoseFocus = FALSE;

	if( _iEditFlag == IDCANCEL )
		return;
	else if (_iEditFlag == IDOK )
	{
		BOOL	_ret;
		if( !SendMessage( _lpForm->hwndEditor, EM_GETMODIFY, 0, 0 ) )
			return;	//no change

		GetWindowText( _lpForm->hwndEditor, _szBuffer, sizeof(_szBuffer) );

		_cell.Size = sizeof(_cell);
		_cell.Event = FORMEV_SETTEXT;
		_cell.Row = _EditRow;
		_cell.Col = _EditCol;
		_cell.lData = _lpForm->lData;
		_cell.lpText = (LPSTR)_szBuffer;
		_cell.TextSize = (DWORD)sizeof(_szBuffer);
		_cell.clrText = 0;
		_cell.clrBack = 0;

		_ret = (_lpForm->pfnFormData)( &_cell );

		if( !_ret )		//illegal data input
		{
			MessageBox(NULL, "Illegal data input", "Error", MB_ERROR );
		}
		else
		{
			_InvalidateCell( hwnd_, _EditRow, _EditCol );
			UpdateWindow( hwnd_ );
		}
		return;
	}

	return;
}

//...................................................................
//Description:
//...................................................................
BOOL _GetColTitle( LPFORM lpForm_, LPSTR lpText_, DWORD size_, UINT col_ )
{
	CELLINFO	_cell;		
	
	_cell.Size = sizeof(_cell);
	_cell.hwnd = lpForm_->hwnd;
	_cell.Event = FORMEV_GETTEXT;
	_cell.Row = COLTITLE;
	_cell.Col = col_;
	_cell.lData = lpForm_->lData;
	_cell.lpText = lpText_;
	_cell.TextSize = size_;
	_cell.clrText = 0;
	_cell.clrBack = 0;

	return (lpForm_->pfnFormData)( &_cell );
}

//...................................................................
//Description:
//...................................................................
BOOL _GetRowTitle( LPFORM lpForm_, LPSTR lpText_, DWORD size_, UINT row_ )
{
	CELLINFO	_cell;
	
	_cell.Size = sizeof(_cell);
	_cell.hwnd = lpForm_->hwnd;
	_cell.Event = FORMEV_GETTEXT;
	_cell.Row = row_;
	_cell.Col = ROWTITLE;
	_cell.lData = lpForm_->lData;
	_cell.lpText = lpText_;
	_cell.TextSize = size_;
	_cell.clrText = 0;
	_cell.clrBack = 0;

	return (lpForm_->pfnFormData)( &_cell );
}

int _GetLeftVisibleColomn(HWND hwnd_, int col_)
{
	LPFORM			_lpForm = (LPFORM)GetWindowLong(hwnd_, 0);
	/*
	for( --col_; col_>=0; --col_)
		if( m_pColInfo[col_].m_bShown )
			return col_;
	*/

	return --col_;
}

int _GetRightVisibleColomn(HWND hwnd_, int col_)
{
	LPFORM			_lpForm = (LPFORM)GetWindowLong(hwnd_, 0);
	/*
	for( ++col_; col_<m_Cols; ++col_)
		if( m_pColInfo[col_].m_bShown )
			return col_;
	*/

	++col_;

	if( col_ < (int)_lpForm->Cols )
		return col_;
	else
		return -1;
}

//...................................................................
//Entry:
//	WORD	colomn_
//			if colomn_ = (-1), invalidate the whole line.
//...................................................................
void _InvalidateCell(HWND hwnd_, int row_, int colomn_)
{
	RECT		_rcClient, _rcCell;

#ifdef _NEW_STYLE
	_GetCellRect( FORMTITLE, colomn_, &_rcCell );
	InvalidateRect(hwnd_, &_rcCell, TRUE);

	_GetCellRect( row_, FORMTITLE, &_rcCell );
	InvalidateRect(hwnd_, &_rcCell, TRUE);
#endif

	if( colomn_ != -1 )
		_GetCellRect( hwnd_, row_, colomn_, &_rcCell );
	else
	{
		GetClientRect( hwnd_, &_rcClient );

		_GetCellRect( hwnd_, row_, 0, &_rcCell );
		_rcCell.left = _rcClient.left;
		_rcCell.right = _rcClient.right;
	}

	InvalidateRect(hwnd_, &_rcCell, TRUE);
	return;
}

//...................................................................
//Entry:
//...................................................................
void _HideEdit( HWND hwnd_ )
{
	LPFORM	_lpForm = (LPFORM)GetWindowLong(hwnd_, 0);

	ShowWindow( _lpForm->hwndEditor, SW_HIDE );
	return;
}

//...................................................................
//Description:
//...................................................................
_DLLEXP  VOID FAR* _DLLFUNC AllocGlobalPtr( DWORD dwSize_ )
{
	return malloc( (size_t)dwSize_ );
}

//...................................................................
//Description:
//...................................................................
_DLLEXP VOID FAR* _DLLFUNC ReallocGlobalPtr( LPVOID lpOld_, DWORD dwSize_ )
{
	return realloc( lpOld_, (size_t)dwSize_ );
}

//...................................................................
//Description:
//...................................................................
_DLLEXP VOID _DLLFUNC FreeGlobalPtr( void far* lp_ )
{
	free( lp_ );
}

