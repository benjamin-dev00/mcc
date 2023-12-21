//===================================================================
//
//Zhang GuanFan, Zheng zhikai, 	97-6-2
//===================================================================

#include "tfalgo.h"
#include "tfcmdmc.H"
#include "corebase.rh"
#include "twdlg.rh"
#include "form.h"

BOOL _MatrixSheetGetData( LONG lData_, LPSTR lpvalstr_, WORD size_, WORD row_, WORD col_ );
BOOL _MatrixSheetSetData( LONG lData_, LPSTR lpvalstr_, WORD row_, WORD col_ );
BOOL _MatrixSheetGetRowTitle( LONG lData_, LPSTR szBuffer_, WORD wSize_, WORD wRow_);
BOOL _MatrixSheetGetColTitle( LONG lData_, LPSTR szBuffer_, WORD wSize_, WORD wCol_);
int _DlgMatrix_InitDialog( HWND hwnd_, LPCOBJ FAR* );

BOOL _DLLFUNC DlgMatrixProc(HWND hwnd_, UINT msg_, WPARAM wParam_, LPARAM lParam_);

extern HINSTANCE	ghInst;

//...................................................................
//...................................................................
#pragma argsused
DWORD _DLLFUNC ObjectMatrix(LPPU lpPU_, WORD wMode_, HWND hwnd_)
{
	SimplePU	_spu;
	LPCOBJ		_lpCobj, _lpCobjEdit;
	BOOL		_bOk;
	int			_ret;
	//HWND		_hwndDesktop;

	memset( &_spu, 0, sizeof(_spu) );
	_spu.prefix = PF_IMM|IMM_LPCOBJ;
	_spu.du.zzk__LPCOBJ = 0;
	if( !PURead(&_spu, lpPU_) )
		return FALSE;

	_lpCobj = _spu.du.zzk__LPCOBJ;
	if( !_lpCobj )
	{
		CORE_ASSERT( FALSE );
		return FALSE;
	}

	_lpCobjEdit = TFCobjCopy( _lpCobj, &_bOk );
	//_hwndDesktop = GetDesktopWindow();
	if( !_bOk )
	{
		MessageBox( NULL, "Core object copy error!", "Error", MB_OK );
		return FALSE;
	}

	_ret = DialogBoxParam( ghInst,
						   MAKEINTRESOURCE(DLG_MATRIX),
						   hwnd_,	// ?? _hwndDesktop, if set this to _hwndDesktop, the mouse will be error
						   (DLGPROC)DlgMatrixProc,
						   (LPARAM)(LPCOBJ FAR*)(&_lpCobjEdit) );
	if( _ret == IDCANCEL )
		CobjFree( _lpCobjEdit );
	else if( _ret == IDOK )
	{
		memset( &_spu, 0, sizeof(_spu) );
		_spu.prefix = PF_IMM|IMM_LPCOBJ;
		_spu.du.zzk__LPCOBJ = _lpCobjEdit;
		if( !PUWrite( lpPU_, &_spu ) )
		{
			CobjFree( _lpCobjEdit );
			return FALSE;
		}
	}
	else
	{
		CORE_ASSERT( FALSE );
		return FALSE;
	}

	return TRUE;
}

//...................................................................
//...................................................................
void __declspec( dllexport ) MatrixFree( LPMATRIX lpm )
{
	CobjFree( MATRIX_2_COBJ(lpm) );
	return ;
}

//...................................................................
//...................................................................
#pragma argsused
BOOL _DLLFUNC DlgMatrixProc(HWND hwnd_, UINT msg_, WPARAM wParam_, LPARAM lParam_)
{
	switch(msg_)
	{
		case WM_INITDIALOG:
			return _DlgMatrix_InitDialog( hwnd_, (LPCOBJ FAR*)lParam_ );

		case WM_COMMAND:
			switch(wParam_)
			{
				case IDOK:
					if( GetFocus() == GetDlgItem( hwnd_, IDC_MATRIX_SHEET ) )
					{
						SendDlgItemMessage( hwnd_, IDC_MATRIX_SHEET, WM_KEYDOWN, VK_RETURN, 0 );
						SendDlgItemMessage( hwnd_, IDC_MATRIX_SHEET, WM_KEYUP, VK_RETURN, 0 );
					}
					else
						EndDialog(hwnd_, IDOK);
					return TRUE;

				case IDCANCEL:
					EndDialog(hwnd_, IDCANCEL);
					return TRUE;

				default:
					return FALSE;
			}

		default:
			return FALSE;
	}
}

int _DlgMatrix_InitDialog( HWND hwnd_, LPCOBJ FAR* lplp_ )
{
	char		_szBuffer[128];
	//SimplePU	_spu;
	//LPCOBJ		_lpCobj;
	LPMATRIX	_lpMatrix;
	RECT		_rcDefault;
	FORM		_sheet;
	HWND		_hwndSheet;
	POINT		_ptUpperLeft, _ptLowerRight;

	_lpMatrix = COBJ_2_MATRIX( (*lplp_) );

	memset( &_sheet, 0, sizeof(_sheet) );
	_sheet.Rows = _lpMatrix->row;
	_sheet.Cols = _lpMatrix->col;
	_sheet.WidthRowTitle = 60;
	_sheet.HeightColTitle = 20;
	_sheet.bEqualWidthCol = TRUE;
	_sheet.pColWidth = NULL;
	_sheet.ColWidth = 70;
	/*_sheet.lpGetData = _MatrixSheetGetData;
	_sheet.lpSetData = (SSETDATA)_MatrixSheetSetData;
	_sheet.lpGetRowTitle = _MatrixSheetGetRowTitle;
	_sheet.lpGetColTitle = _MatrixSheetGetColTitle;*/
	_sheet.lData = (LONG)_lpMatrix;

	GetWindowRect( GetDlgItem(hwnd_, IDC_MATRIX_DEFAULT), &_rcDefault );
	_ptUpperLeft.x = _rcDefault.left;
	_ptUpperLeft.y = _rcDefault.top;
	_ptLowerRight.x = _rcDefault.right;
	_ptLowerRight.y = _rcDefault.bottom;
	ScreenToClient( hwnd_, &_ptUpperLeft );
	ScreenToClient( hwnd_, &_ptLowerRight );

	//GetClassInfo( ghInst, ZZKSHEETCLASSNAME, "" )

	_hwndSheet = CreateWindow( ZZKFORMCLASSNAME, "",
				  WS_BORDER|WS_CHILD|WS_VISIBLE|WS_TABSTOP,
				  _ptUpperLeft.x,
				  _ptUpperLeft.y,
				  _ptLowerRight.x - _ptUpperLeft.x,
				  _ptLowerRight.y  - _ptUpperLeft.y,
				  hwnd_,
				  (HMENU)IDC_MATRIX_SHEET,
				  ghInst,
				  (LPVOID)&_sheet );

	if( !_hwndSheet )
		goto _BAD;
	SetFocus( _hwndSheet ); //set focus by myself
	sprintf( _szBuffer, "Row:%d", _lpMatrix->row );
	SendDlgItemMessage( hwnd_, IDC_MATRIX_ROW, WM_SETTEXT, 0, (LPARAM)_szBuffer );
	sprintf( _szBuffer, "Col:%d", _lpMatrix->col );
	SendDlgItemMessage( hwnd_, IDC_MATRIX_COL, WM_SETTEXT, 0, (LPARAM)_szBuffer );
	return FALSE;

_BAD:
	EndDialog( hwnd_, IDCANCEL );
	return TRUE;
}

//...................................................................
//...................................................................
#pragma argsused
BOOL _MatrixSheetGetData( LONG lData_, LPSTR lpvalstr_, WORD size_, WORD row_, WORD col_ )
{
	LPMATRIX	_lpMatrix = (LPMATRIX)lData_;
	sprintf( lpvalstr_, "%lg", MGET(_lpMatrix, row_, col_) );

	return ACRET_TRUE;
}

//...................................................................
//...................................................................
#pragma argsused
BOOL _MatrixSheetSetData( LONG lData_, LPSTR lpvalstr_, WORD row_, WORD col_ )
{
	LPMATRIX	_lpMatrix = (LPMATRIX)lData_;
	double		_v;
	if( 1 != sscanf( lpvalstr_, "%lg", &_v ) )
		return FALSE;
	else
	{
		MGET(_lpMatrix, row_, col_) = _v;
		return TRUE;
	}
}

//...................................................................
//...................................................................
#pragma argsused
BOOL _MatrixSheetGetRowTitle( LONG lData_, LPSTR szBuffer_, WORD wSize_, WORD wRow_)
{
//	itoa( wRow_ + 1, szBuffer_, 10 );
	sprintf( szBuffer_, "%i", wRow_ + 1);
	return TRUE;
}

//...................................................................
//...................................................................
#pragma argsused
BOOL _MatrixSheetGetColTitle( LONG lData_, LPSTR szBuffer_, WORD wSize_, WORD wCol_)
{
//    itoa( wCol_ + 1, szBuffer_, 10 );
	sprintf( szBuffer_, "%i", wCol_ + 1);
	return TRUE;
}

//...................................................................
//...................................................................
LPMATRIX	__declspec( dllexport ) MatrixAlloc( int row, int col )
{
	LPMATRIX	lpMatrix;
	LPCOBJ		_lpCobj;
	int			_i, _j;

	if( row <= 0 || col <= 0 )
	{
		MessageBox( NULL, "Dimension of Matrix should be greater than 0", "ERRO", MB_OK );
		return NULL;
	}
	_lpCobj = CobjAlloc( IDCOBJ_MATRIX, (long)row * (long)col * (long)sizeof(double) * 1L + sizeof(Matrix) + sizeof(COBJ) );
	lpMatrix = COBJ_2_MATRIX( _lpCobj );
	lpMatrix->row = row;
	lpMatrix->col = col;
	for(_i = 0; _i < lpMatrix->row; _i++ )
		for(_j = 0; _j < lpMatrix->col; _j++ )
			MGET( lpMatrix, _i, _j) = 0.0;

	return lpMatrix;
}

//...................................................................
//...................................................................
int madd(LPMATRIX lpmResult, LPMATRIX lpma, LPMATRIX lpmb )
{
	int	_row, _col;

	//check the dim, if illegal
	if((lpma->row<=0)|(lpma->col<=0))
	{
		MessageBox(NULL, "err", "", MB_OK);
		return 0;
	}

	if((lpma->row)!=(lpmb->row))
	{
		MessageBox(NULL, "err", "", MB_OK);
		return 0;
	}

	if((lpma->col)!=(lpmb->col))
	{
		MessageBox(NULL, "err", "", MB_OK);
		return 0;
	}

	//then calcul.
	lpmResult->row = lpma->row;
	lpmResult->col = lpma->col;

	for(_row = 0; _row < lpma->row; _row++ )
		for(_col = 0; _col < lpma->col; _col++ )
			MGET(lpmResult, _row, _col) = MGET(lpma, _row, _col) + MGET(lpmb, _row, _col);
	return 1;
}

