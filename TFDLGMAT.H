#if !defined(AFX_TFDLGMAT_H__2891E8C2_C02C_11D1_899F_000021E0CB4A__INCLUDED_)
#define AFX_TFDLGMAT_H__2891E8C2_C02C_11D1_899F_000021E0CB4A__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// TFDLGMAT.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// TFDLGMAT dialog

class TFDLGMAT : public CDialog
{
// Construction
public:
	TFDLGMAT(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(TFDLGMAT)
	enum { IDD = IDD_DLGMATRIX };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(TFDLGMAT)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(TFDLGMAT)
	afx_msg void OnCtInsertparam3();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TFDLGMAT_H__2891E8C2_C02C_11D1_899F_000021E0CB4A__INCLUDED_)
