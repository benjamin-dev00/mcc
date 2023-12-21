//===================================================================
//
//	Helper window for DMC algorithm
//===================================================================
#include "tfalgo.h"
#include "tfmatrix.h"
#include "corebase.rh"
#include "tfmcv.h"
#include "sheet.h"

char	_TagValue[2][20] =
				{
					"TAG",
					"Value",
				};


BOOL _DLLFUNC _TagValueSheetCanEdit( LONG lData_, WORD row_, WORD col_ );
BOOL _DLLFUNC _TagValueSheetGetColTitle( LONG lData_, LPSTR szBuffer_, WORD wSize_, WORD wCol_);
BOOL _DLLFUNC _TagValueSheetGetRowTitle( LONG lData_, LPSTR szBuffer_, WORD wSize_, WORD wRow_);
BOOL _DLLFUNC _TagValueSheetGetData( LONG lData_, LPSTR lpvalstr_, WORD size_, WORD row_, WORD col_ );
BOOL _DLLFUNC _TagValueSheetSetData( LONG lData_, LPSTR lpvalstr_, WORD row_, WORD col_ );

HWND _DLLFUNC TagValueWndCreate( HWND hwndParent_,
							 DWORD	dwStyle_,
							 HINSTANCE	hInst_,
							 int x_,
							 int y_,
							 int cx_,
							 int cy_,
							 WORD	id_,
							 LPCOBJ lplp_ )
{
	LPMATRIX	_lpLinkMatrix;
	SHEET		_sheet;
	HWND		_hwndSheet;

	_lpLinkMatrix = COBJ_2_MATRIX( (lplp_) );

	memset( &_sheet, 0, sizeof(_sheet) );
	_sheet.wRows = _lpLinkMatrix->col;
	_sheet.wCols = 2;
	_sheet.wWidthRowTitle = 20;
	_sheet.wHeightColTitle = 20;
	_sheet.bEqualWidthCol = TRUE;
	_sheet.pwColWidth = NULL;
	_sheet.wColWidth = 80;
	_sheet.lpGetData = (BOOL)_TagValueSheetGetData;
	_sheet.lpSetData = (BOOL)_TagValueSheetSetData;
	_sheet.lpGetRowTitle = (BOOL)_TagValueSheetGetRowTitle;
	_sheet.lpGetColTitle = (BOOL)_TagValueSheetGetColTitle;
	_sheet.lpCanEdit = (BOOL)_TagValueSheetCanEdit;
	_sheet.lData = (LONG)_lpLinkMatrix;

	_hwndSheet = CreateWindow( ZZKSHEETCLASSNAME, "",
				  dwStyle_,
				  x_, y_, cx_, cy_,
				  hwndParent_,
				  (HMENU)id_,
				  hInst_,
				  (LPVOID)&_sheet );

	if( !_hwndSheet )
		goto _BAD;
	return _hwndSheet;
_BAD:
	return NULL;
}

//...................................................................
//...................................................................
#pragma argsused
BOOL _DLLFUNC _TagValueSheetCanEdit( LONG lData_, WORD row_, WORD col_ )
{
	return FALSE;
}

//...................................................................
//...................................................................
#pragma argsused
BOOL _DLLFUNC _TagValueSheetGetRowTitle( LONG lData_, LPSTR szBuffer_, WORD wSize_, WORD wRow_)
{
	sprintf( szBuffer_, "%i", wRow_ + 1 );
	return TRUE;
}

//...................................................................
//...................................................................
#pragma argsused
BOOL _DLLFUNC _TagValueSheetGetColTitle( LONG lData_, LPSTR szBuffer_, WORD wSize_, WORD wCol_)
{
	sprintf( szBuffer_, "%s", _TagValue[wCol_] );
	return TRUE;
}

//...................................................................
//...................................................................
BOOL _DLLFUNC _TagValueSheetGetData( LONG lData_, LPSTR lpvalstr_, WORD size_, WORD row_, WORD col_ )
{
	LPLINKMATRIX	_lpLinkMatrix = (LPLINKMATRIX)lData_;
	LPPU			_lptempPU;
	GETVALUESTRING	_gvs;
	DWORD			_ret_gvs;

	_gvs.lpszBuffer = lpvalstr_;
	_gvs.wSize = size_;

	if(  row_ >= _lpLinkMatrix->row )
	{
		CORE_ASSERT( FALSE );
		return FALSE;
	}

	if( col_ == 0 )
	{
		DECODE _decode;

		col_ += MV_VALUE;
		_lptempPU = DLM_GET_LPPU( _lpLinkMatrix, col_, row_ );

		_decode.wAccess = 0;
		if( PUDecode( _lptempPU, &_decode) )
			CombineDecode( lpvalstr_, &_decode );
		else
		{
			strcpy(lpvalstr_, "#ERR");
		}
	}
	else
	{
		col_ += MV_VALUE-1;
		//_lptempPU = DLM_GET_LPPU( _lpLinkMatrix, row_, col_ );
		_lptempPU = DLM_GET_LPPU( _lpLinkMatrix, col_, row_ );

		_ret_gvs = PUGetValueString( _lptempPU, &_gvs, VSOF_REAL );
	}
	if( 0 == _ret_gvs )
		strcpy(lpvalstr_, "#ERR");	//invalid value

	return TRUE;
}

//...................................................................
//...................................................................
#pragma argsused
BOOL _DLLFUNC _TagValueSheetSetData( LONG lData_, LPSTR lpvalstr_, WORD row_, WORD col_ )
{
	return TRUE;
}

