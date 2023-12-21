//===================================================================
//
//Zheng zhikai 	, 97-6-10
//Note:
//	Because the value string for DataLinkMatrix has two
//	meanings: value and data-link, but the FORM window
//	can only show ONE kind of value string, how to decide
//	which value string should be return in functions
//		_LinkMatrixformGetData
//		_LinkMatrixformSetData
//	At this version, I use a TEMPERORY TIP to solve this
//	problem:
//		if ( LINKMATRIX->Nomeaning )
//			show data value of the data-link matrix
//		else
//			show data link of the data-link matrix
//	and the LINKMATRIX->Nomeaning is set and reset by the
//	state of two radio check boxes in dialog box;
//===================================================================
#include "tfalgo.H"
#include "Mccprehead.h"
#include "Mcccoreall.h"

#include "tfmat.h"
#include "corebase.rh"
#include "tfmcv.h"
#include "Mcczassert.h"
//#include "twdlg.rh"
#include "FORM.h"
#define IDC_MATRIX_form	0x200

BOOL _LinkMatrixformGetData( LONG lData_, LPSTR lpvalstr_, WORD size_, WORD row_, WORD col_ );
BOOL _LinkMatrixformSetData( LONG lData_, LPSTR lpvalstr_, WORD row_, WORD col_ );
BOOL _LinkMatrixformGetRowTitle( LONG lData_, LPSTR szBuffer_, WORD wSize_, WORD wRow_);
BOOL _LinkMatrixformGetColTitle( LONG lData_, LPSTR szBuffer_, WORD wSize_, WORD wCol_);
static int _DlgMatrix_InitDialog( HWND hwnd_, LPCOBJ FAR* );

BOOL _DLLFUNC DlgLinkMatrixProc(HWND hwnd_, UINT msg_, WPARAM wParam_, LPARAM lParam_);

extern HINSTANCE	ghInst;

//...................................................................
//...................................................................
#pragma argsused
DWORD ObjectLinkMatrix(LPPU lpPU_, WORD wMode_, HWND hwnd_)
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
						   MAKEINTRESOURCE(DLG_LINKMATRIX),
						   hwnd_, 
						   (DLGPROC)DlgLinkMatrixProc,
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
LPLINKMATRIX LinkMatrixAlloc( int row, int col )
{
	LPLINKMATRIX	_lpLinkMatrix;
	LPCOBJ			_lpCobj;
	int				_i, _j;

	_lpCobj = CobjAlloc( IDCOBJ_LINKMATRIX, (long)row * (long)col * (long)sizeof(PU) * 1L + sizeof(LINKMATRIX) + sizeof(COBJ) );

	_lpLinkMatrix = COBJ_2_LINKMATRIX( _lpCobj );
	_lpLinkMatrix->row = row;
	_lpLinkMatrix->col = col;
	for(_i = 0; _i < _lpLinkMatrix->row; _i++ )
		for(_j = 0; _j < _lpLinkMatrix->col; _j++ )
		{
			DLM_GET_LPPU( _lpLinkMatrix, _i, _j )->prefix = PF_IMM|IMM_REAL;
			DLM_GET_LPPU( _lpLinkMatrix, _i, _j )->du.zzk__REAL = 0;
		}

	return _lpLinkMatrix;
}

//...................................................................
//...................................................................
void LinkMatrixFree( LPLINKMATRIX lpm_ )
{
	CobjFree( LINKMATRIX_2_COBJ(lpm_) );
	return ;
}

//...................................................................
//...................................................................
#pragma argsused
BOOL _DLLFUNC DlgLinkMatrixProc(HWND hwnd_, UINT msg_, WPARAM wParam_, LPARAM lParam_)
{
	switch(msg_)
	{
		case WM_INITDIALOG:
			return _DlgMatrix_InitDialog( hwnd_, (LPCOBJ FAR*)lParam_ );

		case WM_COMMAND:
			switch(wParam_)
			{
				case IDC_DATALINK:
				case IDC_VALUE:
				{
					BOOL			_bChecked;
					HWND			_hwndform;
					LPLINKMATRIX	_lpLinkMatrix;
					LPFORM			_lpform;

					if( HIWORD(lParam_ ) != BN_CLICKED )
						break;

					if( IsDlgButtonChecked( hwnd_, IDC_VALUE ) )
						_bChecked = TRUE;
					else
						_bChecked = FALSE;

					_hwndform = GetDlgItem( hwnd_, IDC_MATRIX_form );
					_lpform = (LPFORM)GetWindowLong( _hwndform, 0 );
					_lpLinkMatrix = (LPLINKMATRIX)_lpform->lData;
					if( !_lpLinkMatrix )
						break;

					if( (BOOL)(_lpLinkMatrix->Nomeaning[0]) != _bChecked )
					{
						_lpLinkMatrix->Nomeaning[0] = _bChecked;
						InvalidateRect( _hwndform, NULL, TRUE );	//redraw the FORM window
					}
					break;
				}

				case IDOK:
					if( GetFocus() == GetDlgItem( hwnd_, IDC_MATRIX_form ) )
					{
						SendDlgItemMessage( hwnd_, IDC_MATRIX_form, WM_KEYDOWN, VK_RETURN, 0 );
						SendDlgItemMessage( hwnd_, IDC_MATRIX_form, WM_KEYUP, VK_RETURN, 0 );
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

static int _DlgMatrix_InitDialog( HWND hwnd_, LPCOBJ FAR* lplp_ )
{
	char		_szBuffer[128];
	//SimplePU	_spu;
	//LPCOBJ		_lpCobj;
	LPLINKMATRIX	_lpLinkMatrix;
	RECT		_rcDefault;
	FORM		_form;
	HWND		_hwndform;
	POINT		_ptUpperLeft, _ptLowerRight;

	_lpLinkMatrix = COBJ_2_LINKMATRIX( (*lplp_) );
	if( !_lpLinkMatrix )
		goto _BAD;

	SendDlgItemMessage(hwnd_, IDC_DATALINK, BM_SETCHECK, 1, 0);
	_lpLinkMatrix->Nomeaning[0] = 0;

	memset( &_form, 0, sizeof(_form) );
	_form.Rows = _lpLinkMatrix->row;
	_form.Cols = _lpLinkMatrix->col;
	_form.WidthRowTitle = 60;
	_form.HeightColTitle = 20;
	_form.bEqualWidthCol = TRUE;
	_form.pColWidth = NULL;
	_form.ColWidth = 70;
/*	_form.lpGetData = _LinkMatrixformGetData;
	_form.lpSetData = (BOOL)_LinkMatrixformSetData;
	_form.lpGetRowTitle = _LinkMatrixformGetRowTitle;
	_form.lpGetColTitle = _LinkMatrixformGetColTitle;
*/	_form.lData = (LONG)_lpLinkMatrix;

	GetWindowRect( GetDlgItem(hwnd_, IDC_MATRIX_DEFAULT), &_rcDefault );
	_ptUpperLeft.x = _rcDefault.left;
	_ptUpperLeft.y = _rcDefault.top;
	_ptLowerRight.x = _rcDefault.right;
	_ptLowerRight.y = _rcDefault.bottom;
	ScreenToClient( hwnd_, &_ptUpperLeft );
	ScreenToClient( hwnd_, &_ptLowerRight );

	//GetClassInfo( ghInst, ZZKformCLASSNAME, "" )
	_hwndform = CreateWindow( ZZKFORMCLASSNAME, "",
				  WS_BORDER|WS_CHILD|WS_VISIBLE|WS_TABSTOP,
				  _ptUpperLeft.x,
				  _ptUpperLeft.y,
				  _ptLowerRight.x - _ptUpperLeft.x,
				  _ptLowerRight.y  - _ptUpperLeft.y,
				  hwnd_,
				  (HMENU)IDC_MATRIX_form,
				  ghInst,
				  (LPVOID)&_form );

	if( !_hwndform )
		goto _BAD;
	SetFocus( _hwndform ); //set focus by myself

	sprintf( _szBuffer, "Row:%d", _lpLinkMatrix->row );
	SendDlgItemMessage( hwnd_, IDC_MATRIX_ROW, WM_SETTEXT, 0, (LPARAM)_szBuffer );
	sprintf( _szBuffer, "Col:%d", _lpLinkMatrix->col );
	SendDlgItemMessage( hwnd_, IDC_MATRIX_COL, WM_SETTEXT, 0, (LPARAM)_szBuffer );
	return FALSE;

_BAD:
	EndDialog( hwnd_, IDCANCEL );
	return TRUE;
}

//...................................................................
//...................................................................
#pragma argsused
BOOL _LinkMatrixformGetData( LONG lData_, LPSTR lpvalstr_, WORD size_, WORD row_, WORD col_ )
{
	LPLINKMATRIX	_lpLinkMatrix = (LPLINKMATRIX)lData_;
	LPPU			_lpPU;


	if(  ( row_ >= _lpLinkMatrix->row )
	   ||( col_ >= _lpLinkMatrix->col ) )
	{
		CORE_ASSERT( FALSE );
		return FALSE;
	}

	//_lpPU = DLM_GET_LPPU( _lpLinkMatrix, row_, col_ );
	_lpPU = DLM_GET_LPPU( _lpLinkMatrix, row_, col_ );

	if( _lpLinkMatrix->Nomeaning[0] )		//show value
	{
		GETVALUESTRING	_gvs;
		DWORD			_ret_gvs;

		_gvs.lpszBuffer = lpvalstr_;
		_gvs.wSize = size_;
		_ret_gvs = PUGetValueString( _lpPU, &_gvs, VSOF_REAL );
		if( 0 == _ret_gvs )
			strcpy(lpvalstr_, "#ERR");	//invalid value
	}
	else	//show data link
	{
		DECODE	_decode;
		
		_decode.wAccess = 0;

		if( PUDecode( _lpPU, &_decode ) )
			CombineDecode(lpvalstr_, &_decode);
		else
			strcpy( lpvalstr_, "#ERR" );	//invalid data-link
	}

	return TRUE;
}

//...................................................................
//...................................................................
#pragma argsused
BOOL _LinkMatrixformSetData( LONG lData_, LPSTR lpvalstr_, WORD row_, WORD col_ )
{
	LPLINKMATRIX	_lpLinkMatrix = (LPLINKMATRIX)lData_;
	LPPU			_lpPU;


	if(  ( row_ >= _lpLinkMatrix->row )
	   ||( col_ >= _lpLinkMatrix->col ) )
	{
		CORE_ASSERT( FALSE );
		return FALSE;
	}

	_lpPU = DLM_GET_LPPU( _lpLinkMatrix, row_, col_ );

	if( _lpLinkMatrix->Nomeaning[0] )		//set value
		return (BOOL)PUSetValueString( _lpPU, lpvalstr_, VSOF_REAL );
	else	//set data link
	{
		ENCODE	_encode;
		_encode.lpszBuffer = lpvalstr_;
		_encode.wAccess = 0;
		_encode.wError = 0;

		return PUEncode( _lpPU, &_encode );
	}
}

//...................................................................
//...................................................................
#pragma argsused
BOOL _LinkMatrixformGetRowTitle( LONG lData_, LPSTR szBuffer_, WORD wSize_, WORD wRow_)
{
	sprintf( szBuffer_, "%i", wRow_ + 1 );
	return TRUE;
}

//...................................................................
//...................................................................
#pragma argsused
BOOL _LinkMatrixformGetColTitle( LONG lData_, LPSTR szBuffer_, WORD wSize_, WORD wCol_)
{
	sprintf( szBuffer_, "%i", wCol_ + 1);
	return TRUE;
}

