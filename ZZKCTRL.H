#ifndef __ZZKCTRL_H
#define __ZZKCTRL_H

//#define PROP_SELECTED			"hwndSelected"

//post to the window procedure of the PageButton which is selected
//wParam / lParam				no used
#define PM_SELECTED				(WM_USER+21)
//post to the window procedure of the PageButton which is unselected
//wParam / lParam				no used
#define PM_UNSELECTED			(WM_USER+22)
//whenever the active title page changed, the messge is sent to frame window of the dialog
//wParam / lParam				no used
#define PM_SELECTCHANGED		(WM_USER+23)
//when a dialog page is visible again, the messgae is posted to page procedure
//if the dialog page is visible first time, this is not posted.
//wParam / lParam				no used
#define PDM_PAGEVISIBLE			(WM_USER+24)

//PAGEBTN_EXTRA for:
//	EXTRA_PB_HWNDPAGE
//		HWND of associated dialog Page.
//	EXTRA_PB_CAPTURE
//		FLAG of mouse capture. when the LBtton is down, the mouse is captured
//		until LButton is up.
#define PAGEBTN_EXTRA			(sizeof(HWND) + sizeof(WORD))
#define EXTRA_PB_HWNDPAGE		0
#define EXTRA_PB_CAPTURE		sizeof(HWND)
#define	PAGEBTNCLASS			"PAGEBTN"

//EXTRA_PB_xxxx for:
//	PROP_PDF_HWNDPAGEBTN
//		HWND of the active PageBtn. (At the any time, there is
//		ONE and ONLY ONE active PgBtn)
//	PROP_PDF_LPENTRYHI
//	PROP_PDF_LPENTRYLO
//		Point to a data structue PAGEDLGENTRY
#define PROP_PDF_HWNDPAGEBTN	"HWNDPAGEBTN"	//MAKEINTRESOURCE(111)
#define PROP_PDF_LPENTRYHI		"LPENTRYHI"		//MAKEINTRESOURCE(222)
#define PROP_PDF_LPENTRYLO		"LPENTRYLO"		//MAKEINTRESOURCE(333)

/*
#define	DLGPAGECLASS			"DLGPAGE"
#define DLGPAGE_EXTRA			( DLGWINDOWEXTRA + (sizeof(HWND)) )
#define EXTRA_DP_HWNDPAGEBTN	DLGWINDOWEXTRA
*/

typedef struct tagPDPAGE
{
	WORD	wID;
	DLGPROC	lpfnPageProc;
	LPSTR	lpszDlgTemp;
	LPVOID	lpInitData;
}PDPAGE;

typedef struct tagPAGEDLGENTRY
{
	HINSTANCE	hInstance;
	WORD		wPageNum;
	WORD		wPageCX;	//page width in dialog base unit
	WORD		wPageCY;	//page height in dialog base unit
	DLGPROC		lpfnFrameProc;	//call back func for the frame window of PageDialog
	PDPAGE		Page[1];
}PAGEDLGENTRY, far* LPPAGEDLGENTRY;

LRESULT CALLBACK _export PageBtnWndProc(HWND hwnd_, UINT msg_, WPARAM wParam_, LPARAM lParam_);
BOOL CALLBACK _export PageBtnDlgProc(HWND hwnd_, UINT msg_, WPARAM wParam_, LPARAM lParam_);

//LRESULT CALLBACK _export PDPageWndProc(HWND hwnd_, UINT msg_, WPARAM wParam_, LPARAM lParam_);
BOOL CALLBACK _export PDFrameDlgProc(HWND hwnd_, UINT msg_, WPARAM wParam_, LPARAM lParam_);

_DLLEXP LPPAGEDLGENTRY _ZZKDECL GetPageDialogFrameEntry(HINSTANCE hInst_,
												HWND hwndParent_);
_DLLEXP BOOL _ZZKDECL SetPageDialogFrameEntry(HINSTANCE hInst_,
									  HWND hwndParent_,
									  LPPAGEDLGENTRY lpEntry_);

_DLLEXP void _ZZKDECL PDInitializePage(HWND hwnd_);
_DLLEXP void _ZZKDECL PDEndDialog(HWND hwnd_, int iResult_);
_DLLEXP HWND _ZZKDECL PDGetPage(HWND hwndFrame_, int idPageTitle_);

_DLLEXP int _ZZKDECL PDCreateDialogParam( HINSTANCE			hInst_,
								  LPCSTR			lpszDlgTemp_,
								  HWND				hwndParent_,
								  LPPAGEDLGENTRY	lpEntry_);

//inner function
BOOL RegisterClasses(void);
void UnregisterClasses(void);

//inputbox function
_DLLEXP int _ZZKDECL InputBox(HWND hwndParent_,
					 LPSTR szTitle_,
					 LPSTR szPrompt_,
					 LPSTR szValue_,
					 WORD wValueSize_);

#endif	// __ZZKCTRL_H
