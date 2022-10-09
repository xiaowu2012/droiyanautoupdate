// AutoUpdateConfigDlg.h : 头文件
//

#pragma once
#include "..\\AutoUpdateClient\\updatefiles.h"
#include "afxcmn.h"


// CAutoUpdateConfigDlg 对话框
class CAutoUpdateConfigDlg : public CDialog
{
// 构造
public:
	CAutoUpdateConfigDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_AUTOUPDATECONFIG_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
private:
	CUpdateFiles m_UpdateFiles;

public:
	afx_msg void OnBnClickedBtnTest();
	CListCtrl m_lstFiles;
	afx_msg void OnBnClickedBtnAddfile();
	afx_msg void OnBnClickedBtnAddfolder();
	afx_msg void OnBnClickedBtnSaveXml();
	afx_msg void OnBnClickedBtnSave();
	void RefreshCtrl(void);
	afx_msg void OnNMRclickLstFiles(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedBtnDelsel();
	afx_msg void OnBnClickedUpdateFile();
	CString m_sCommonURL;
	CString m_sCommonXML;
	CString m_sCommonLst;
	void ReadCommonSet(void);
	void SaveCommonSet(void);
	afx_msg void OnBnClickedOk();
	afx_msg void OnDropFiles(HDROP hDropInfo);
	
	virtual BOOL DestroyWindow();
	afx_msg void OnBnClickedButton2();

	void InsertFolderFiles(TCHAR* dirpath);
	afx_msg void OnClose();
};
