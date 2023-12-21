//===================================================================
//
//Zheng zhikai	97-1-9
//===================================================================

#ifndef __SHEET_H
#define __SHEET_H

//the window class name
#define ZZKSHEETCLASSNAME	"ZzkSheet"

//get active cell in sheet
//wParam = n/a
//lParam = n/a
//return: row in LOWORD, colomn in HIWORD
#define SM_GET_ACTIVECELL					(WM_USER + 0xFF)

//invalidate and update the area in sheet window
//wParam = the row;
//lParam = the col;		if ==(WORD)(-1) invalidate the whole row
#define SM_INVALIDATE			(WM_USER + 0x100)

//get the row of the sheet
//wParam: 	n/a(=0);
//lParam:	n/a(=0);
//return:	the current number of row;
#define SM_GET_NUMBER_OF_ROW	(WM_USER + 0x101)

//set the row of the sheet;
//if the sheet is equal width, this message sets all
//colomn width to new value;
//wParam: 	n/a(=0);
//lParam:	new number of row
//return:	TRUE/FALSE
#define SM_SET_NUMBER_OF_ROW	(WM_USER + 0x102)

//get the number of col of the sheet
//wParam: 	n/a(=0);
//lParam:	n/a(=0);
//return:	the current number of colomn;
#define SM_GET_NUMBER_OF_COL	(WM_USER + 0x103)
//set the number of col of the sheet
//wParam: 	n/a(=0);
//lParam:	new number of row
//return:	TRUE/FALSE
#define SM_SET_NUMBER_OF_COL	(WM_USER + 0x104)

//get the width of a certain colomn
//wParam: 	the index of colomn which is a zero based integer;
//lParam:	n/a(=0);
//return:	the width of the colomn given by wParam if successful
//			otherwise, returns NULL;
#define SM_GET_WIDTH_OF_COL		(WM_USER + 0x105)

//set the width of a certain colomn
//wParam: 	the index of colomn which is a zero based integer;
//lParam:	the width of the colomn given by wParam
//return:	TRUE/FALSE
#define SM_SET_WIDTH_OF_COL		(WM_USER + 0x106)

#define SHN_SETFOCUS			( 0x100 )
#define SHN_KILLFOCUS			( 0x101 )
#define SHN_DBLCLK				( 0x102 )
#ifndef _WIN32
 typedef BOOL _ZZKDECL (*SGETDATA)( LONG lData_, LPSTR lpvalstr_, WORD size_, WORD row_, WORD col_ );
 typedef BOOL _ZZKDECL (*SSETDATA)( LONG lData_, LPCSTR lpvalstr_, WORD row_, WORD col_ );
 typedef BOOL _ZZKDECL (*SGETROWTITLE)( LONG lData_, LPSTR szBuffer_, WORD wSize_, WORD wRow_);
 typedef BOOL _ZZKDECL (*SGETCOLTITLE)( LONG lData_, LPSTR szBuffer_, WORD wSize_, WORD wCol_);
 typedef BOOL _ZZKDECL (*SCANEDIT)( LONG lData_, WORD row_, WORD col_ );
#else
 typedef BOOL  (*SGETDATA)( LONG lData_, LPSTR lpvalstr_, WORD size_, WORD row_, WORD col_ );
 typedef BOOL  (*SSETDATA)( LONG lData_, LPCSTR lpvalstr_, WORD row_, WORD col_ );
 typedef BOOL  (*SGETROWTITLE)( LONG lData_, LPSTR szBuffer_, WORD wSize_, WORD wRow_);
 typedef BOOL  (*SGETCOLTITLE)( LONG lData_, LPSTR szBuffer_, WORD wSize_, WORD wCol_);
 typedef BOOL  (*SCANEDIT)( LONG lData_, WORD row_, WORD col_ );
#endif


LRESULT CALLBACK _export ZzkSheetWndProc(HWND hwnd_, UINT msg_, WPARAM wParam_, LPARAM lParam_);

typedef struct _tag_SHEET
{
	WORD			wRows;			//the total rows in data sheet
	WORD    		wCols;			//the total colomns every row
	int				wWidthRowTitle;
	int				wHeightColTitle;

	BOOL			bEqualWidthCol;	//if False, pwColWidth is valid
									//otherwise, wColWidth is valid;
	int				wColWidth;
	int _FAR*		pwColWidth;		//the array storing each colomn width;
	LONG			lData;			//user defined data

	SGETDATA		lpGetData;
	SSETDATA		lpSetData;
	SGETROWTITLE	lpGetRowTitle;
	SGETCOLTITLE	lpGetColTitle;
	SCANEDIT		lpCanEdit;

	//inner element
	HWND			hwnd;			//the window associated this struction
	int				left;			//the left of the first colomn
	WORD			wFirstRow;		//the first row visible in window
	WORD			wFocusRow;		//the row that the focus is on
	WORD			wFocusCol;		//the colomn that the focus is on
	BOOL			bFocus;			//whether the window own focus
	HWND			hwndEditor;		//HWND of the pop editor window
	BOOL			bEditLoseFocus;
}SHEET, _FAR* LPSHEET;

_DLLEXP VOID FAR* _ZZKDECL AllocGlobalPtr( DWORD dwSize_ );
_DLLEXP VOID _ZZKDECL FreeGlobalPtr( void far* lp_ );
_DLLEXP VOID FAR* _ZZKDECL ReallocGlobalPtr( LPVOID lpOld_, DWORD dwSize_ );

#endif	//__CMETER_H
