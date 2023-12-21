#include "tfalgo.h"
#include "tfcmdmc.h"
#include "form.h"
HWND _DLLFUNC _MVWindow_Create( HWND hwndParent_,
							 DWORD	dwStyle_,
							 HINSTANCE	hInst_,
							 int x_,
							 int y_,
							 int cx_,
							 int cy_,
							 HMENU	id_,
							 LPCOBJ FAR* lplp_ )
{
	LPMATRIX	_lpLinkMatrix;
	FORM		_FORM;
	HWND		_hwndFORM;

	_lpLinkMatrix = COBJ_2_MATRIX( (*lplp_) );

	memset( &_FORM, 0, sizeof(_FORM) );
	_FORM.Rows = _lpLinkMatrix->row;
	_FORM.Cols = _lpLinkMatrix->col;
	_FORM.WidthRowTitle = 60;
	_FORM.HeightColTitle = 20;
	_FORM.bEqualWidthCol = TRUE;
	_FORM.pColWidth = NULL;
	_FORM.ColWidth = 70;
/*	_FORM.lpGetData = (BOOL)_MVStructFORMGetData;
	_FORM.lpSetData = (BOOL)_MVStructFORMSetData;
	_FORM.lpGetRowTitle = (BOOL)_MVStructFORMGetRowTitle;
	_FORM.lpGetColTitle = (BOOL)_MVStructFORMGetColTitle;*/
	_FORM.pfnFormData = _MVStructFORMData;
	_FORM.lData = (LONG)_lpLinkMatrix;

	_hwndFORM = CreateWindow( ZZKFORMCLASSNAME, "",
				  dwStyle_,
				  x_, y_, cx_, cy_,
				  hwndParent_,
				  (HMENU)id_,
				  hInst_,
				  (LPVOID)&_FORM );

	if( !_hwndFORM )
		goto _BAD;
	return _hwndFORM;
_BAD:
	return NULL;
}

HWND _DLLFUNC _CVWindow_Create( HWND hwndParent_,
							 DWORD	dwStyle_,
							 HINSTANCE	hInst_,
							 int x_,
							 int y_,
							 int cx_,
							 int cy_,
							 HMENU	id_,
							 LPCOBJ FAR* lplp_ )
{
	LPMATRIX	_lpLinkMatrix;
	FORM		_FORM;
	HWND		_hwndFORM;

	_lpLinkMatrix = COBJ_2_MATRIX( (*lplp_) );

	memset( &_FORM, 0, sizeof(_FORM) );
	_FORM.Rows = _lpLinkMatrix->row;
	_FORM.Cols = _lpLinkMatrix->col;
	_FORM.WidthRowTitle = 60;
	_FORM.HeightColTitle = 20;
	_FORM.bEqualWidthCol = TRUE;
	_FORM.pColWidth = NULL;
	_FORM.ColWidth = 70;
	/*_FORM.lpGetData = (BOOL)_CVStructFORMGetData;
	_FORM.lpSetData = (BOOL)_CVStructFORMSetData;
	_FORM.lpGetRowTitle = (BOOL)_CVStructFORMGetRowTitle;
	_FORM.lpGetColTitle = (BOOL)_CVStructFORMGetColTitle;*/
	_FORM.pfnFormData = _CVStructFORMData;
	_FORM.lData = (LONG)_lpLinkMatrix;

	_hwndFORM = CreateWindow( ZZKFORMCLASSNAME, "",
				  dwStyle_,
				  x_, y_, cx_, cy_,
				  hwndParent_,
				  (HMENU)id_,
				  hInst_,
				  (LPVOID)&_FORM );

	if( !_hwndFORM )
		goto _BAD;
	return _hwndFORM;
_BAD:
	return NULL;
}

HWND _DLLFUNC _DVWindow_Create( HWND hwndParent_,
							 DWORD	dwStyle_,
							 HINSTANCE	hInst_,
							 int x_,
							 int y_,
							 int cx_,
							 int cy_,
							 HMENU	id_,
							 LPCOBJ FAR* lplp_ )
{
	LPMATRIX	_lpLinkMatrix;
	FORM		_FORM;
	HWND		_hwndFORM;

	_lpLinkMatrix = COBJ_2_MATRIX( (*lplp_) );

	memset( &_FORM, 0, sizeof(_FORM) );
	_FORM.Rows = _lpLinkMatrix->row;
	_FORM.Cols = _lpLinkMatrix->col;
	_FORM.WidthRowTitle = 60;
	_FORM.HeightColTitle = 20;
	_FORM.bEqualWidthCol = TRUE;
	_FORM.pColWidth = NULL;
	_FORM.ColWidth = 70;
/*	_FORM.lpGetData = _DVStructFORMGetData;
	_FORM.lpSetData = (BOOL)_DVStructFORMSetData;
	_FORM.lpGetRowTitle = _DVStructFORMGetRowTitle;
	_FORM.lpGetColTitle = _DVStructFORMGetColTitle;
*/	_FORM.pfnFormData = _DVStructFORMData;
	_FORM.lData = (LONG)_lpLinkMatrix;
						 
	_hwndFORM = CreateWindow( ZZKFORMCLASSNAME, "",
				  dwStyle_,
				  x_, y_, cx_, cy_,
				  hwndParent_,
				  (HMENU)id_,
				  hInst_,
				  (LPVOID)&_FORM );

	if( !_hwndFORM )
		goto _BAD;
	return _hwndFORM;
_BAD:
	return NULL;
}

