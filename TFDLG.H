#if !defined(AFX_TFDLG_H__253AACA2_C28B_11D1_899F_000021E0CB4A__INCLUDED_)
#define AFX_TFDLG_H__253AACA2_C28B_11D1_899F_000021E0CB4A__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// tfdlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// tfdlg dialog

class tfdlg : public CDialog
{
// Construction
public:
	tfdlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(tfdlg)
	enum { IDD = IDD_DLGMATRIX };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(tfdlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(tfdlg)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TFDLG_H__253AACA2_C28B_11D1_899F_000021E0CB4A__INCLUDED_)
