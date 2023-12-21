//===================================================================
//
//Zheng zhikai  97-1-9
//===================================================================

#ifndef __FORM_H
#define __FORM_H

////////////////////////////////////////
//style of form window:
//form grid with vertical line
#define SHS_VERLINE			0x0001L
//form grid with horizontal line
#define SHS_HORLINE			0x0002L
//the select area is a whole line of the form, otherwise, is only a cell 
#define SHS_ROWSELECT		0x0004L
#define SHS_MULTIROWSELECT (0x0010L|SHS_ROWSELECT)

////////////////////////////////////////
//the window class name
#define ZZKFORMCLASSNAME    ("ZzkForm")

//get user defined data FORM.lData
//wParam = n/a
//lParam = n/a
//return: FORM.lData
#define SM_GET_DATA				(WM_USER + 0xF0)

//get/set active cell in form
//wParam = n/a
//lParam = n/a
//return: row in LOWORD, colomn in HIWORD
#define SM_GET_ACTIVECELL		(WM_USER + 0xF1)
#define SM_SET_ACTIVECELL		(WM_USER + 0xFF)

//invalidate and update the area in form window
//wParam = the row;
//lParam = the col;     if ==(WORD)(-1) invalidate the whole row
#define SM_INVALIDATE           (WM_USER + 0x100)

//get the row of the form
//wParam:   n/a(=0);
//lParam:   n/a(=0);
//return:   the current number of row;
#define SM_GET_NUMBER_OF_ROW    (WM_USER + 0x101)

//set the row of the form;
//if the form is equal width, this message sets all
//colomn width to new value;
//wParam:   n/a(=0);
//lParam:   new number of row
//return:   TRUE/FALSE
#define SM_SET_NUMBER_OF_ROW    (WM_USER + 0x102)

//get the number of col of the form
//wParam:   n/a(=0);
//lParam:   n/a(=0);
//return:   the current number of colomn;
#define SM_GET_NUMBER_OF_COL    (WM_USER + 0x103)
//set the number of col of the form
//wParam:   n/a(=0);
//lParam:   new number of row
//return:   TRUE/FALSE
#define SM_SET_NUMBER_OF_COL    (WM_USER + 0x104)

//get the width of a certain colomn
//wParam:   the index of colomn which is a zero based integer;
//lParam:   n/a(=0);
//return:   the width of the colomn given by wParam if successful
//          otherwise, returns NULL;
#define SM_GET_WIDTH_OF_COL     (WM_USER + 0x105)

//set the width of a certain colomn
//wParam:   the index of colomn which is a zero based integer;
//lParam:   the width of the colomn given by wParam
//return:   TRUE/FALSE
#define SM_SET_WIDTH_OF_COL     (WM_USER + 0x106)

//Single Select Form:
//	as same as SM_SET_ACTIVECELL,
//Multi-Row-Select form:
//	set select row, the active cell isn't changed
//get/set active cell in form
//lParam = n/a
//return: row in LOWORD, colomn in HIWORD
#define SM_SET_SEL				(WM_USER + 0x107)

//Single Select Form:
//	as same as SM_GET_ACTIVECELL,
//Multi-Row-Select form:
//	as same as SM_GET_ACTIVECELL,
//	set select row, the active cell isn't changed
//get/set active cell in form
//lParam = n/a
//return: row in LOWORD, colomn in HIWORD
#define SM_GET_SEL				(WM_USER + 0x108)

#define SHN_SETFOCUS            ( 0x100 )
#define SHN_KILLFOCUS           ( 0x101 )
#define SHN_DBLCLK              ( 0x102 )
#define SHN_SELCHANGED			( 0x103 )

//the row and colomn is a zero based number of the following value
#define		ROWTITLE	(-1)	//for CELLINFO to specify row title
#define		COLTITLE	(-1)	//for CELLINFO to specify colomn title
#define		FORMTITLE	(-1)

#define		NO_SEL		((UINT)(-1))

//default color in clrText and clrBack
#define		FORM_DEFAULT_CLR	(-1)

//definition for Event
#define	FORMEV_GETTEXT			0x00001L		//get cell text, can combine with FORMEV_GETCOLOR
#define FORMEV_GETCOLOR			0x00002L		//get cell color, can combine with FORMEV_TEXT
#define FORMEV_SETTEXT			0x00004L		//notify user the changed cell text
#define FORMEV_CANEDIT			0x00008L		//notify user whether cell can edit
#define	FORMEV_GETEDITTEXT		0x00010L		//get cell edit text, 

typedef struct _tag_CELLINFO{
	DWORD	Size;		//size of this struct
	HWND	hwnd;		//hwnd of this window
	DWORD	Event;		//the event code
	UINT	Row;		//the row, zero-based number or ROWTITLE
	UINT	Col;		//the colomn, zero-based number or COLTITLE
	LONG	lData;		//user defined data in the form struct
	LPSTR	lpText;		//pointer to text buffer
	DWORD	TextSize;	//the length of text buffer
	DWORD	clrText;	//the text color of cell
	DWORD	clrBack;	//the back color of cell
} CELLINFO, FAR* LPCELLINFO; 


////////////////////////////////////////////////////
//at the following Set/Get data functions:
//the parameter row and col is from zero
typedef BOOL (*LPFORMDATA)( LPCELLINFO lpCellInfo_ );

typedef struct _tag_FORM
{
	UINT			Size;			//size of the struct
	UINT			Rows;			//the total rows in data form
	UINT			Cols;			//the total colomns every row
	int				WidthRowTitle;
	int				HeightColTitle;

	BOOL            bEqualWidthCol; //if False, pwColWidth is valid
									//otherwise, wColWidth is valid;
	int				ColWidth;
	int _FAR*		pColWidth;		//the array storing each colomn width;
	LONG            lData;          //user defined data

	LPFORMDATA		pfnFormData;

	//inner element
	HWND            hwnd;           //the window associated this struction
	int				left;           //the left of the first colomn
	int				FirstRow;		//the first row visible in window
	int				FocusRow;		//the row that the focus is on
	int				FocusCol;		//the colomn that the focus is on
	UINT _FAR*		pSelected;		//for multi-selected, this is an array end with SELECT_END
									//storing selected row index;
	UINT			ArrayItems;		//size of array specified by lpSelected;
	BOOL            bFocus;         //whether the window own focus

	HWND            hwndEditor;     //HWND of the pop editor window
	BOOL            bEditLoseFocus;

	BOOL			bInHorScroll;
	BOOL			bInVerScroll;
}FORM, _FAR* LPFORM;

_DLLEXP VOID FAR* _ZZKDECL AllocGlobalPtr( DWORD dwSize_ );
_DLLEXP VOID _ZZKDECL FreeGlobalPtr( void far* lp_ );
_DLLEXP VOID FAR* _ZZKDECL ReallocGlobalPtr( LPVOID lpOld_, DWORD dwSize_ );

#endif  //__CMODU_H
