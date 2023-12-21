#include "form.h"

#define TABLE_FORM_STYLE 	MDIS_ALLCHILDSTYLES		//(WS_MAXIMIZE|WS_MINIMIZE|WS_HSCROLL|WS_VSCROLL)

#define HEIGHT_TOOLBAR		32
#define HEIGHT_STATUSBAR	24

#define	HEIGHT_TABLE_TITLE	24
#define WIDTH_RECORD		48

#define TB_X_EDIT			60
#define TB_Y_EDIT			5
#define TB_CX_EDIT			300
#define TB_CY_EDIT          24

#define TB_ID_EDIT			1
#define	TB_ID_EDIT_OK		2
#define	TB_ID_EDIT_CANCEL	3

#define	WINDOWMODE_FORM	1
#define	WINDOWMODE_LATTICE  2

#define DLG_NOTPROCESSED	0
#define DLG_PROCESSED		1

#define LOADICON(ID)				\
	LoadIcon( ghInst, MAKEINTRESOURCE(ID) )
#define LOADSTRING(ID, szBuffer)	\
	LoadString( ghInst, ID, szBuffer, sizeof(szBuffer) )
#define LOADBITMAP(ID)				\
	LoadBitmap( ghInst, MAKEINTRESOURCE(ID) )

typedef struct _tagNAMESET
{
	WORD	wOrder;
	char	szName[16];
}NAMESET, FAR* LPNAMESET;

typedef struct _tagFORMCOLOMN{
	WORD	wWidth;
	WORD	wOrder;
}FORMCOLOMN, FAR* LPFORMCOLOMN;

typedef LPSTR (FAR *LPGETDATA)(LPSTR lpsz_, int size_, WRD wXTU_, WORD element_id_);
typedef int (FAR *LPSETDATA)(LPCSTR lpsz_, WRD wrd_, WORD order_);

typedef struct tagFORM_WINDOW_DESCRIPTION
{
	LPNAMESET		lpNameSet;		//point to an name-order sets
	LPGETDATA		lpGetData;
	LPSETDATA		lpSetData;
	LPFORMCOLOMN	lpSC;
	WORD			wColomns;		//the number of colomn
	int				left;			//the left of the first colomn

	WRD				wRecords;		//the number of record

	//the following two elements has valid range from 0 to (wRecords-1)
	WRD  			wFirstRecord;	//the record which shown as the first line
	WRD 			wFocusRecord;	//the record which owned the focus

	//the valid range is from 0 to (wColomns - 1)
	int				iFocusColomn;	//the colomn of which record owned the focus

	HWND			hwnd;
}SWD, FAR* LPSWD;

typedef struct tagLATTICE_WINDOW_DESCRIPTION FAR* LPLWD;

typedef void (FAR* LPUPDATEMASTER)(LPLWD);
typedef void (FAR* LPUPDATEDETAIL)(LPLWD);
typedef void (FAR* LPNEW)( HWND hwnd_ );
typedef void (FAR* LPDELETE)( HWND, WRD );

///////////////////////////////////////////////////////////
//LWD	Lattice Window Descripor
typedef struct tagLATTICE_WINDOW_DESCRIPTION
{
	HWND			hwnd;
	HWND			hwndMaster;		//HWND of Master listbox
	HWND			hwndDetail;		//HWND of Detail listbox

	HWND			hwndFocus;		//HWND of window which own focus or should own focus
									//is equal to hwndMaster or hwndDetail

	WORD			wMasterName;	//a certain element value of xTU shown in master list box
	WORD FAR*		lpwShown;		//point to an array for element to be shown
	LPNAMESET		lpNameSet;		//point to an name-order sets

	LPUPDATEMASTER  lpFillMaster;
	LPUPDATEDETAIL	lpUpdateDetail;
	LPGETDATA		lpGetData;
	LPSETDATA		lpSetData;
	LPNEW			lpNew;			//create a new xTU
	LPDELETE		lpDelete;		//delete a xTU
}LWD, FAR* LPLWD ;

typedef struct _tagMODULATTICE{
	HWND			hwnd;
	HWND			hwndTree;
	HWND			hwndDetail;
	HWND			hwndParam;
	HWND			hwndFocus;
	WORD FAR*		lpwShown;		//point to an array for element to be shown
	LPNAMESET		lpNameSet;		//point to an name-order sets
}MFLATTICE, FAR* LPMFLATTICE;


LRESULT CALLBACK MyMDIClientWndProc(HWND hwnd_, UINT msg_, WPARAM wParam_, LPARAM lParam_);
LRESULT CALLBACK FormWndProc(HWND hwnd_, UINT msg_, WPARAM wParam_, LPARAM lParam);
LRESULT CALLBACK LatticeWndProc(HWND hwnd_, UINT msg_, WPARAM wParam_, LPARAM lParam);
LRESULT CALLBACK StatusBarWndProc(HWND hwnd_, UINT msg_, WPARAM wParam_, LPARAM lParam_);
LRESULT CALLBACK ToolBarWndProc(HWND hwnd_, UINT msg_, WPARAM wParam_, LPARAM lParam_);

//void PaintDecorateTools(HWND hwnd_);

LRESULT FrameWMCommand(HWND hwnd_, WPARAM wParam_, LPARAM lParam_);
LRESULT LatticeCommandHandler(HWND hwnd_, WPARAM wParam_, LPARAM lParam_);

BOOL QueryCloseChild(HWND hwnd_);

//BOOL FillWndReal(HWND hwnd_, HDC hdc_, int iPosit_, WRD record_);

void TBButtonDown(HWND hwnd_, WPARAM wParam_, LPARAM lParam_);
void TBButtonUp(HWND hwnd_, WPARAM wParam_, LPARAM lParam_);

//void InvalidateTable(HWND hwnd_, WRD line_, int colomn_);
void InvalidateFormZone(HWND hwnd_, WRD record_, int colomn_);

//void Testing(void);
void TestAllocReal(void);
void TestFrag(void);
void TBEditCheck(void);

BOOL CALLBACK DlgRealUnitProc(HWND hwnd_, UINT msg_, WPARAM wParam_, LPARAM lParam_);
BOOL CALLBACK DlgTintUnitProc(HWND hwnd_, UINT msg_, WPARAM wParam_, LPARAM lParam_);
BOOL CALLBACK DlgBoolUnitProc(HWND hwnd_, UINT msg_, WPARAM wParam_, LPARAM lParam_);
BOOL CALLBACK DlgInputBoxProc(HWND hwnd_, UINT msg_, WPARAM wParam_, LPARAM lParam_);
BOOL CALLBACK DlgPrefProc(HWND hwnd_, UINT msg_, WPARAM wParam_, LPARAM lParam_);

int GetOrderName(LPSTR szBuffer_, WORD wSize_, NAMESET FAR* lpNameSet_, WORD wOrder_);
//void GetPDUTypeName(PDU FAR* lpPDU_, LPSTR szName_);

void CreateLattice(LPLWD lpLwd_, LPSTR szTitle_);
void CreateForm(LPSWD lpSwd_, LPSTR szTitle_);

void CMRealAutomatic(HWND hwnd_);
void CMRealAllocate(HWND hwnd_);
void CMRealFree(HWND hwnd_, WRD wrd_ );

void CMTintAutomatic(HWND hwnd_);
void CMTintAllocate(HWND hwnd_);
void CMTintFree(HWND hwnd_, WRD wrd_ );

void CMBoolAutomatic(HWND hwnd_);
void CMBoolAllocate(HWND hwnd_);
void CMBoolFree(HWND hwnd_, WRD wrd_ );

void CMModuAllocate(HWND hwnd_);
void CMModuInspect(HWND hwnd_);
void CMModuDelete(HWND hwnd_, WRD wrd_ );

void CMFaceAllocate(HWND hwnd_);
void CMFaceInspect(HWND hwnd_);
void CMFaceDelete(HWND hwnd_, WRD wrd_ );

void CMTestFrag(HWND hwnd_);

#define UTW_UNITADD			1
#define UTW_UNITDELETE		2
#define UTW_UNITUPDATE		3
void UpdateTableWindow(WORD table_, WORD trans_, WRD record_, WORD order_);

typedef struct _col_info{
	WORD	element;
	BOOL	visible;
	WORD	width;
}COLINFO;

//Face Tree window functions
void UpdateFaceTree(HWND hwnd_);

///////////////////////////////////////////////////////////
//TREEWND:
//	Data struct for FaceTree tree and ModuTree tree windows.
//This struct records all information about the tree window
//which associated with it. Other windows, such as ListBoxes
//in ModuLattice window and FaceLattice window, READ the
//content of the structure too and use GetWindowLong() to
//access the struct associate with the tree window.
//Note:
//	Only the tree window can write data to its associated
//TREEWND struct, others ONLY READ it.
typedef struct tagTREE_WINDOW
{
	HWND			hwnd;	//the window which associates with it

	//child tree list data
	COLORREF		clrTextSelected;
	COLORREF		clrBackSelected;
	HBITMAP			hbmpNormal;
	HBITMAP			hbmpLapped;
	HBITMAP			hbmpExpanded;

//private data elements
	WRD				wLinesCount;
	WORD			wWidth;		//max width in X coor.

	BOOL			bFocusOn;

	//the NO. of the Face in face tree listed at the top of window
	WORD			wFirstLine;

	//the NO. of the Face selected in face tree
	WORD			wSelLine;
}TREEWND, FAR* LPTREEWND;

//user defined message for Modu/Face lattice windows
#define LM_FLUSH		(WM_USER + 10)

//user defined notifing code for Modu/Face lattice windows
//send by ModuTree / FaceTree window
#define TM_SELCHANGED	(1000)
typedef struct _tagITERATOR FAR * LPITERATOR;
typedef BOOL (near * LPTEST)( LPITERATOR );

///////////////////////////////////////////////////////////
//ITERATOR
//	It is entry parameter data structure for iterator functions
//used for tree windows, i.e. ModuTree and FaceTree. Those
//function is used to paint FTUs/MTUs, to find a certain FTU
//or MTU, and so on.
//	This structure is maintained both by iterator function
//and its caller.
typedef struct _tagITERATOR{
	LPTEST 	lpfnTest; 		//test function
	WORD 	wMaxCount;		//after test, if wCounter >= wMaxCount, iterator stop
	WORD	wCounter;		//counter, init. value = 0
	WRD 	wBegin;			//WRD of the first face/modu to iterate
	WRD		wCurrent;		//WRD of current face/modu
	WORD	wLevel;			//current child level, init. value set by caller
	LPARAM 	lReturn;		//filled by test function
}ITERATOR, FAR* LPITERATOR;
void FaceTreeIterator(LPITERATOR lpIterator_);
void TreeSearchFaceByLine(LPITERATOR lpit_, WORD line_);

void ModuTreeIterator(LPITERATOR lpIterator_);
void TreeSearchModuByLine(LPITERATOR lpit_, WORD line_);

LRESULT CALLBACK ModuTreeWndProc(HWND hwnd_, UINT msg_, WPARAM wParam_, LPARAM lParam_);
WRD GetSelectModuWithPrompt( HWND hwnd_, LPSTR lpszPrompt_ );
WRD GetSelectFaceWithPrompt( HWND hwnd_, LPSTR lpszPrompt_ );

int BarEdit(HWND hwnd_, LPSTR lpsz_, WORD wBufferSize_);

//create tables form windows
void CMCreateRealTableForm( HWND hwnd_ );
void CMCreateTintTableForm( HWND hwnd_ );
void CMCreateBoolTableForm( HWND hwnd_ );
void CMCreateModuTableForm( HWND hwnd_ );
void CMCreateFaceTableForm( HWND hwnd_ );
void CMCreateAlgoTableForm( HWND hwnd_ );
void CMCreateStampTableForm( HWND hwnd_ );

