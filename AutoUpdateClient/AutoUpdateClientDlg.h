// AutoUpdateClientDlg.h : 头文件
//

#pragma once
#include "afxwin.h"
#include "afxcmn.h"

#include ".\servers.h"
#include "updatefiles.h"
#include ".\inetfiles.h"
#include "MyButton.h"
#include "ImageProgress.h"
#include "AniStatic.h"
#include "SInstance.h"

class CAutoUpdateClientDlg : public CDialog
{
// 构造
public:
	CAutoUpdateClientDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_AUTOUPDATECLIENT_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg LRESULT OnDownLoadFile(WPARAM wParam, LPARAM lParam); 
	afx_msg LRESULT OnDownLoadFileEnd(WPARAM wParam, LPARAM lParam); 
	DECLARE_MESSAGE_MAP()

protected :
	BOOL	m_bNeedExecute;
	CString m_ExecuteFile;
	CString m_ExecuteParam;
	CString m_UpdateUrl;

	CFont	m_Font;
	CBitmap	m_AniBitmap;

private:


	DWORD m_dwCount;
	DWORD m_dwUpdatedCount;
	DWORD m_dwUpdateTotalBytes;

	CKevinCritSec m_lock;

	CServers m_Servers;
	CUpdateFiles m_RemoteFiles;

	CInetFiles m_InetFiles;			//等待下载的INTERNET网络文件

	UINT m_nMaxDownThread;
	UINT m_nCurThreadCount;
	//bool m_bTray;
	//NOTIFYICONDATA	m_pTray;

	void RenameAndExeFile(CInetFile* pInetFile);


	CPoint	m_ptBefore;
	BOOL	m_bMouceDown;

public:
	CInstanceChecker m_InstanceChecker;

	CKevinCritSec	m_AniLock;
	int				m_AniFrame;
	void			AniTick();

public:
	void SetUpdateUrl(CString strUrl);
	void StartUpload(void);
	afx_msg void OnTimer(UINT nIDEvent);
	void AddLog(LPCTSTR sLog, int nErrCode = 0);
	void AddLogContent(LPCTSTR sLog, int nErrCode = 0);
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	virtual BOOL DestroyWindow();
	void RunGame();
	CMyButton m_CancelButton;
	afx_msg void OnBnClickedCancelbutton();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg LRESULT OnNcHitTest(CPoint point);

	CAniStatic		TextInfo;
	CAniStatic		TextInfoContent;
	CImageProgress	m_progDownLoad;
	afx_msg void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
};
