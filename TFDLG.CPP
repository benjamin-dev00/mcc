// tfdlg.cpp : implementation file
//

#include "stdafx.h"
#include "platform.h"
#include "tfdlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// tfdlg dialog


tfdlg::tfdlg(CWnd* pParent /*=NULL*/)
	: CDialog(tfdlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(tfdlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void tfdlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(tfdlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(tfdlg, CDialog)
	//{{AFX_MSG_MAP(tfdlg)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// tfdlg message handlers
