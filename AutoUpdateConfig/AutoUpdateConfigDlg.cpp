// AutoUpdateConfigDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "AutoUpdateConfig.h"
#include "AutoUpdateConfigDlg.h"
#include "AutoUpdateFileDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CAutoUpdateConfigDlg 对话框



CAutoUpdateConfigDlg::CAutoUpdateConfigDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAutoUpdateConfigDlg::IDD, pParent)
	, m_sCommonURL(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CAutoUpdateConfigDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LST_FILES, m_lstFiles);
	DDX_Text(pDX, IDC_EDT_URL, m_sCommonURL);
}

BEGIN_MESSAGE_MAP(CAutoUpdateConfigDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(ID_BTN_TEST, OnBnClickedBtnTest)
	ON_BN_CLICKED(IDC_BTN_ADDFILE, OnBnClickedBtnAddfile)
	ON_BN_CLICKED(IDC_BTN_ADDFOLDER, OnBnClickedBtnAddfolder)
	ON_BN_CLICKED(IDC_BTN_SAVE, OnBnClickedBtnSave)
	ON_NOTIFY(NM_RCLICK, IDC_LST_FILES, OnNMRclickLstFiles)
	ON_BN_CLICKED(IDC_BTN_DELSEL, OnBnClickedBtnDelsel)
	ON_BN_CLICKED(IDOK, &CAutoUpdateConfigDlg::OnBnClickedOk)
	ON_WM_DROPFILES()
	ON_BN_CLICKED(IDC_BUTTON1, &CAutoUpdateConfigDlg::OnBnClickedUpdateFile)
	ON_BN_CLICKED(IDC_BUTTON2, &CAutoUpdateConfigDlg::OnBnClickedButton2)
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CAutoUpdateConfigDlg 消息处理程序

BOOL CAutoUpdateConfigDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 将\“关于...\”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	SetWindowText(_T("自动更新配置文件自动生成XML || SL"));

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	DWORD dwExStyle = m_lstFiles.GetExtendedStyle();
	dwExStyle |= LVS_EX_GRIDLINES;
	dwExStyle |= LVS_EX_FULLROWSELECT ;
	//dwStyle |= LVS_EX_CHECKBOXES;
	m_lstFiles.SetExtendedStyle(dwExStyle);

	m_lstFiles.InsertColumn(0, _T("序号"), LVCFMT_LEFT, 40, 0);
	m_lstFiles.InsertColumn(1, _T("文件版本"), LVCFMT_LEFT, 80, 0);
	m_lstFiles.InsertColumn(2, _T("长度"), LVCFMT_LEFT, 80, 0);
	m_lstFiles.InsertColumn(3, _T("文件名"), LVCFMT_LEFT, 200, 0);
	m_lstFiles.InsertColumn(4, _T("文件路径"), LVCFMT_LEFT, 800, 0);

	ReadCommonSet();

	if(m_UpdateFiles.ReadFromXMLFile((LPTSTR)(LPCTSTR)m_sCommonXML, FALSE))
		RefreshCtrl();
	
	return TRUE;  // 除非设置了控件的焦点，否则返回 TRUE
}


BOOL CAutoUpdateConfigDlg::DestroyWindow()
{
	// TODO: 在此添加专用代码和/或调用基类

	SaveCommonSet();

	return CDialog::DestroyWindow();
}


void CAutoUpdateConfigDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CAutoUpdateConfigDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标显示。
HCURSOR CAutoUpdateConfigDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CAutoUpdateConfigDlg::OnBnClickedBtnTest()
{
#ifdef _DEBUG
	TCHAR szFilename[MAX_PATH];
	::GetModuleFileName(NULL, szFilename, sizeof(szFilename));
	m_UpdateFiles.AddUpdateFile(szFilename);
	RefreshCtrl();

	TCHAR* pszFileName = _T("C:\\publish.xml");
	m_UpdateFiles.SaveToXMLFile(pszFileName);
	::ShellExecute(m_hWnd, NULL, pszFileName, NULL, NULL, SW_SHOW);
#endif
}

#define NAMEBUF 1024

void CAutoUpdateConfigDlg::OnBnClickedBtnAddfile()
{
	TCHAR szFilters[] = _T("All Files (*.*)|*.*||");

	CFileDialog fileDlg (true, _T(""), _T("*.*"),
		OFN_FILEMUSTEXIST | OFN_ALLOWMULTISELECT, szFilters, this);

	
	fileDlg.m_ofn.lpstrFile=new TCHAR[NAMEBUF];
	memset(fileDlg.m_ofn.lpstrFile, 0, sizeof(TCHAR[NAMEBUF]));  
	fileDlg.m_ofn.nMaxFile = NAMEBUF;

	CString sPathName;
	if(fileDlg.DoModal ()==IDOK)
	{ 
		SaveCommonSet();
		POSITION mPos=fileDlg.GetStartPosition();
		while(mPos!=NULL) 
		{ 
			sPathName=fileDlg.GetNextPathName(mPos);
			m_UpdateFiles.AddUpdateFile((TCHAR *)(LPCTSTR)sPathName);
			USES_CONVERSION;
			TRACE("%s\n",T2A(sPathName));
		} 
		RefreshCtrl();
	}

	delete [] fileDlg.m_ofn.lpstrFile;
}

void CAutoUpdateConfigDlg::OnBnClickedBtnAddfolder()
{
	BROWSEINFO bi = { 0 };
    bi.lpszTitle = _T("Pick a Directory");
    LPITEMIDLIST pidl = SHBrowseForFolder ( &bi );
    if ( pidl != 0 )
    {
		SaveCommonSet();
        TCHAR szPath[MAX_PATH];
        if ( SHGetPathFromIDList ( pidl, szPath ) )
        {
			CFileFind fFind;
			_tcscat(szPath, _T("\\*.*"));
			USES_CONVERSION;
			int fCount = fFind.FindFile(szPath) ;
			while(fCount) 
			{
				fCount = fFind.FindNextFile(); 
				if (!fFind.IsDirectory() && !fFind.IsDots()) 
				{
					m_UpdateFiles.AddUpdateFile((LPTSTR)(LPCTSTR)fFind.GetFilePath());
					TRACE("%s\n", T2A(fFind.GetFilePath()));
				}
				else 
				{
					//这边增加其它目录
				}
			}

			RefreshCtrl();
		}
	}
}

void CAutoUpdateConfigDlg::OnBnClickedBtnSaveXml()
{
	TCHAR szFilters[] = _T("XML File (*.xml)|*.xml||");

	CFileDialog fileDlg (false, _T("xml"), _T("*.xml"),
		OFN_FILEMUSTEXIST, szFilters, this);

	if( fileDlg.DoModal() == IDOK )
	{
		UpdateData(TRUE);
		m_UpdateFiles.m_sCommonURL = m_sCommonURL;

		m_UpdateFiles.SaveToXMLFile((LPTSTR)(LPCTSTR)fileDlg.GetPathName());
		m_sCommonXML = fileDlg.GetPathName();
		::ShellExecute(m_hWnd, NULL, fileDlg.GetPathName(), NULL, NULL, SW_SHOW);
	}

	SaveCommonSet();
}

void CAutoUpdateConfigDlg::OnBnClickedBtnSave()
{
	// 保存 LST
	UpdateData(TRUE);
	m_UpdateFiles.m_sCommonURL = m_sCommonURL;

	TCHAR szModulePath[MAX_PATH];
	TCHAR szTargetPath[MAX_PATH];
	
	TCHAR fulldir[_MAX_DIR];
	TCHAR filedrive[MAX_PATH];
	TCHAR filename[MAX_PATH];
	TCHAR fileext[MAX_PATH];

	GetModuleFileName(NULL, szModulePath, MAX_PATH);

	_tsplitpath(szModulePath, filedrive, fulldir, filename, fileext);
	
	m_UpdateFiles.m_sRootPath = TEXT("");
	m_UpdateFiles.m_sRootPath.Format(TEXT("%s%s"), filedrive, fulldir);

	if(true)
	{
		_tcscpy(szTargetPath, filedrive);
		_tcscat(szTargetPath, fulldir);
		_tcscat(szTargetPath, _T("\\autoupdate.xml"));

		m_UpdateFiles.SaveToXMLFile(szTargetPath);

		MessageBox(TEXT("列表文件保存成功，请见 autoupdate.xml"));
	}
	else
	{
		_tcscpy(szTargetPath, filedrive);
		_tcscat(szTargetPath, fulldir);
		_tcscat(szTargetPath, _T("\\autoupdate.lst"));

		m_UpdateFiles.SaveToLstFile(szTargetPath);

		MessageBox(TEXT("列表文件保存成功，请见 autoupdate.lst"));
	}

	m_sCommonLst = szTargetPath;

	SaveCommonSet();

	::ShellExecute(m_hWnd, NULL, szTargetPath, NULL, NULL, SW_SHOW);

	return;

	TCHAR szFilters[] = _T("XML File (*.xml)|*.xml||");

	CFileDialog fileDlg (false, _T("xml"), _T("*.xml"),
		OFN_FILEMUSTEXIST, szFilters, this);

	if( fileDlg.DoModal() == IDOK )
	{
		UpdateData(TRUE);
		m_UpdateFiles.m_sCommonURL = m_sCommonURL;

		m_UpdateFiles.SaveToXMLFile((LPTSTR)(LPCTSTR)fileDlg.GetPathName());
		m_sCommonXML = fileDlg.GetPathName();
		::ShellExecute(m_hWnd, NULL, fileDlg.GetPathName(), NULL, NULL, SW_SHOW);
	}

	SaveCommonSet();
}
void CAutoUpdateConfigDlg::RefreshCtrl(void)
{
	m_lstFiles.DeleteAllItems();
	m_UpdateFiles.FillListCtrl(m_lstFiles);
}

void CAutoUpdateConfigDlg::OnNMRclickLstFiles(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
}

void CAutoUpdateConfigDlg::OnBnClickedBtnDelsel()
{
	POSITION pos = m_lstFiles.GetFirstSelectedItemPosition();
	if (pos == NULL)
		TRACE0("No items were selected!\n");
	else
	{
		TCHAR szText[256];
		_stprintf(szText, _T("确实要删除这 %u 项吗？"), m_lstFiles.GetSelectedCount());
		if (MessageBox(szText, _T("系统提示"), MB_YESNOCANCEL) == IDYES) {
			while (pos)
			{
				int nItem = m_lstFiles.GetNextSelectedItem(pos);
				TRACE1("Item %d was selected!\n", nItem);
				CUpdateFile *pUpdateFile = (CUpdateFile *)m_lstFiles.GetItemData(nItem);
				m_UpdateFiles.DeleteItem(pUpdateFile);
			}
			RefreshCtrl();
		}
	}
}

void CAutoUpdateConfigDlg::OnBnClickedUpdateFile()
{
	POSITION pos = m_lstFiles.GetFirstSelectedItemPosition();
	if (pos == NULL)
		TRACE0("No items were selected!\n");
	else
	{
		int nItem = m_lstFiles.GetNextSelectedItem(pos);

		CUpdateFile *pUpdateFile = (CUpdateFile *)m_lstFiles.GetItemData(nItem);
		if(pUpdateFile != NULL)
		{
			CAutoUpdateFileDlg dlg;
			dlg.SetUpdateFile(pUpdateFile);
			if(dlg.DoModal() == IDOK)
			{
				
				RefreshCtrl();
			}
		}
	}
}


void CAutoUpdateConfigDlg::ReadCommonSet(void)
{
	TCHAR szIniPath[_MAX_PATH], szCommonURL[_MAX_PATH], szCommonXML[MAX_PATH];
	::GetModuleFileName(NULL, szIniPath, sizeof(szIniPath));
	_tcscat(szIniPath, _T(".ini"));
	::GetPrivateProfileString(_T("COMMON"), _T("COMMONURL"), _T("default"), szCommonURL, sizeof(szCommonURL), szIniPath);
	::GetPrivateProfileString(_T("COMMON"), _T("COMMONXML"), _T("autoupdate.xml"), szCommonXML, sizeof(szCommonXML), szIniPath);
	m_sCommonURL = szCommonURL;
	m_sCommonXML = szCommonXML;
	UpdateData(false);
}

void CAutoUpdateConfigDlg::SaveCommonSet(void)
{
	UpdateData();
	TCHAR szIniPath[_MAX_PATH];
	::GetModuleFileName(NULL, szIniPath, sizeof(szIniPath));
	_tcscat(szIniPath, _T(".ini"));
	::WritePrivateProfileString(_T("COMMON"), _T("COMMONURL") , m_sCommonURL, szIniPath);
	::WritePrivateProfileString(_T("COMMON"), _T("COMMONXML") , m_sCommonXML, szIniPath);
	m_UpdateFiles.m_sCommonURL = m_sCommonURL;
	m_UpdateFiles.m_sCommonXML = m_sCommonXML;
}


//void CMainFrame::OnFileOpen() 
//{
//	// TODO: Add your command handler code here
//	CFileDialog mFileDlg(TRUE,NULL,NULL,
//		OFN_ALLOWMULTISELECT,
//		_T("Text Files (*.txt)|*.txt|All Files (*.*)|*.*||"),
//		AfxGetMainWnd());
//
//	mFileDlg.m_ofn.lpstrFile=new TCHAR[NAMEBUF];   // 重新定义 lpstrFile 缓冲大小
//	memset(mFileDlg.m_ofn.lpstrFile,0,NAMEBUF);  // 初始化定义的缓冲 
//	mFileDlg.m_ofn.nMaxFile = NAMEBUF;           // 重定义 nMaxFile 
//	
//	TRACE("nMaxFile :%d\n",mFileDlg.m_ofn.nMaxFile);
//	
//	CString pathName;
//	if(mFileDlg.DoModal ()==IDOK)
//	{ 
//		POSITION mPos=mFileDlg.GetStartPosition();
//		while(mPos!=NULL) 
//		{ 
//			pathName=mFileDlg.GetNextPathName(mPos);
//			TRACE("%s\n",pathName);
//		} 
//	}
//	else
//		TRACE("IDCANCLE\n");
//
//	delete [] mFileDlg.m_ofn.lpstrFile;             // 切记使用完后释放资源
//}
void CAutoUpdateConfigDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	OnOK();
}

void CAutoUpdateConfigDlg::OnDropFiles(HDROP hDropInfo)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	TCHAR strFilePath[MAX_PATH]; memset(strFilePath, 0x0, sizeof(strFilePath));
	int nFileCount = DragQueryFile(hDropInfo, 0xFFFFFFFF, NULL,  0);

	if(nFileCount > 0)
	{
		SaveCommonSet();

		for(int i = 0 ; i < nFileCount ; i++)
		{
			// 取索引0处文件
			int nFileLen = DragQueryFile(hDropInfo, i, strFilePath,  MAX_PATH);
			if(nFileLen > 0)
			{
				m_UpdateFiles.AddUpdateFile((TCHAR *)(LPCTSTR)strFilePath);
			}
		}
		RefreshCtrl();
	}

	DragFinish(hDropInfo);

	CDialog::OnDropFiles(hDropInfo);
}

void CAutoUpdateConfigDlg::OnBnClickedButton2()
{
	m_UpdateFiles.ClearAllList();

	TCHAR dirpath[_MAX_DIR];
	TCHAR drivename[MAX_PATH];
	TCHAR filename[MAX_PATH];

	TCHAR fullpath[MAX_PATH];
	GetModuleFileName(NULL, fullpath, MAX_PATH );

	::_tsplitpath(fullpath, drivename, dirpath, filename, NULL); 

	memset(fullpath, 0x00, sizeof(fullpath));
	_tcscpy(fullpath, drivename);
	_tcscat(fullpath, TEXT("\\"));
	_tcscat(fullpath, dirpath);

	m_UpdateFiles.m_sModulePath = filename;
	m_UpdateFiles.m_sModuleDir = fullpath;

	InsertFolderFiles(fullpath);

}

void CAutoUpdateConfigDlg::InsertFolderFiles(TCHAR* dirpath)
{
	TCHAR fullpath[MAX_PATH];
	_tcscpy(fullpath, dirpath);
	_tcscat(fullpath, TEXT("\\*"));

	WIN32_FIND_DATA hFindData;
	HANDLE hFindHandle = FindFirstFile(fullpath, &hFindData);

	if(hFindHandle != INVALID_HANDLE_VALUE)
	{
		if(_tcscmp(hFindData.cFileName, TEXT(".")) != 0 && _tcscmp(hFindData.cFileName, TEXT("..")) != 0)
		{
			if((hFindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY ) == FILE_ATTRIBUTE_DIRECTORY)
			{
				TCHAR findpath[_MAX_DIR];
				_tcscpy(findpath, dirpath);
				_tcscat(findpath, TEXT("\\"));
				_tcscat(findpath, hFindData.cFileName);
				_tcscat(findpath, TEXT("\\"));
				InsertFolderFiles(findpath);
			}
			else
			{
				TCHAR cFileName[MAX_PATH];
				_tcscpy(cFileName, dirpath);
				_tcscat(cFileName, hFindData.cFileName);
				m_UpdateFiles.AddUpdateFile((TCHAR *)(LPCTSTR)cFileName);
			}
		}

		while(FindNextFile(hFindHandle, &hFindData))
		{
			if(_tcscmp(hFindData.cFileName, TEXT(".")) != 0 && _tcscmp(hFindData.cFileName, TEXT("..")) != 0)
			{
				if((hFindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY ) == FILE_ATTRIBUTE_DIRECTORY)
				{
					TCHAR findpath[_MAX_DIR];
					_tcscpy(findpath, dirpath);
					_tcscat(findpath, TEXT("\\"));
					_tcscat(findpath, hFindData.cFileName);
					_tcscat(findpath, TEXT("\\"));
					InsertFolderFiles(findpath);
				}
				else
				{
					TCHAR cFileName[MAX_PATH];
					_tcscpy(cFileName, dirpath);
					_tcscat(cFileName, hFindData.cFileName);
					m_UpdateFiles.AddUpdateFile((TCHAR *)(LPCTSTR)cFileName);
				}
			}
		}
		
		//CloseHandle(hFindHandle);
	}

	RefreshCtrl();
}
void CAutoUpdateConfigDlg::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	CDialog::OnClose();
}
