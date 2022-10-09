// AutoUpdateFileDlg.cpp : implementation file
//

#include "stdafx.h"
#include "AutoUpdateConfig.h"
#include "AutoUpdateFileDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAutoUpdateFileDlg dialog


CAutoUpdateFileDlg::CAutoUpdateFileDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAutoUpdateFileDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAutoUpdateFileDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_lpUpdateFile = NULL;
}


void CAutoUpdateFileDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAutoUpdateFileDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAutoUpdateFileDlg, CDialog)
	//{{AFX_MSG_MAP(CAutoUpdateFileDlg)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON2, &CAutoUpdateFileDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON1, &CAutoUpdateFileDlg::OnBnClickedButton1)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAutoUpdateFileDlg message handlers

LRESULT CAutoUpdateFileDlg::DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CDialog::DefWindowProc(message, wParam, lParam);
}

BOOL CAutoUpdateFileDlg::DestroyWindow() 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CDialog::DestroyWindow();
}

BOOL CAutoUpdateFileDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here

	if(m_lpUpdateFile == NULL)
		EndDialog(IDCANCEL);
	else
	{
		SetDlgItemText(IDC_EDIT_FILENAME, m_lpUpdateFile->m_sFileName);
		SetDlgItemText(IDC_EDIT_CLIENT_PATH, m_lpUpdateFile->m_sLocalPath);
		SetDlgItemText(IDC_EDIT_DOWNLOAD_URL, m_lpUpdateFile->m_sURL);
		SetDlgItemText(IDC_EDIT_MD5, m_lpUpdateFile->m_sMD5);

	}
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CAutoUpdateFileDlg::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
	EndDialog(IDCANCEL);
}

void CAutoUpdateFileDlg::OnBnClickedButton1()
{
	//GetDlgItemText(IDC_EDIT_FILENAME, m_lpUpdateFile->m_sFileName);
	GetDlgItemText(IDC_EDIT_CLIENT_PATH, m_lpUpdateFile->m_sLocalPath);
	GetDlgItemText(IDC_EDIT_DOWNLOAD_URL, m_lpUpdateFile->m_sURL);
	//GetDlgItemText(IDC_EDIT_MD5, m_lpUpdateFile->m_sMD5);

	EndDialog(IDOK);
}
