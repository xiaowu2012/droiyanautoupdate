#if !defined(AFX_AUTOUPDATEFILEDLG_H__11D106E0_7EA0_4A4A_AA01_952FFD02E076__INCLUDED_)
#define AFX_AUTOUPDATEFILEDLG_H__11D106E0_7EA0_4A4A_AA01_952FFD02E076__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CAutoUpdateFileDlg.h : header file
//

#include "..\\AutoUpdateClient\\UpdateFile.h"

/////////////////////////////////////////////////////////////////////////////
// AutoUpdateFileDlg dialog

class CAutoUpdateFileDlg : public CDialog
{
// Construction
public:
	CAutoUpdateFileDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CAutoUpdateFileDlg)
	enum { IDD = IDD_AUTOUPDATEFILE_DIALOG };
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAutoUpdateFileDlg)
	public:
	virtual BOOL DestroyWindow();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual LRESULT DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CAutoUpdateFileDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton2();
	inline void	SetUpdateFile(CUpdateFile* lpFile)
	{
		m_lpUpdateFile = lpFile;
	};

protected:
	CUpdateFile*	m_lpUpdateFile;
public:
	afx_msg void OnBnClickedButton1();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_AUTOUPDATEFILEDLG_H__11D106E0_7EA0_4A4A_AA01_952FFD02E076__INCLUDED_)
