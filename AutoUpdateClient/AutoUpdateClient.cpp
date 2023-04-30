// AutoUpdateClient.cpp : ����Ӧ�ó��������Ϊ��
//

#include "stdafx.h"
#include "AutoUpdateClient.h"
#include "AutoUpdateClientDlg.h"
#include ".\autoupdateclient.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAutoUpdateClientApp

BEGIN_MESSAGE_MAP(CAutoUpdateClientApp, CWinApp)
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

BOOL StringContainOtherString(CString szParentString, CString szChildString)
{
    int nParLength = szParentString.GetLength();
    int nChiLength = szChildString.GetLength();
    
    if(nParLength < nChiLength)
    {
        return FALSE;
    }
    else if(nParLength == nChiLength)
    {
        if(szParentString == szChildString)
        {
            return TRUE;
        }
        else
        {
            return FALSE;
        }
    }
    else if(nParLength > nChiLength)
    {
        if(nChiLength == 1)
        {
            int nFind = szParentString.Find(szChildString);
            if(nFind != -1)
            {
                return TRUE;
            }
            else
            {
                return FALSE;
            }
        }
        else if(nChiLength > 1)
        {
            CString szFirstWord = szChildString.Left(1);
            int nFind = szParentString.Find(szFirstWord);
            if(nFind == -1)
            {
                return FALSE;
            }
            else if(nParLength - nFind < nChiLength)
            {
                return FALSE;
            }
            else
            {
                for(int i = 0; i < nParLength + 1 - nFind - nChiLength; i++)
                {
                    CString szCompareString = szParentString.Mid(nFind + i, nChiLength);
                    if(szCompareString == szChildString)
                    {
                        return TRUE;
                    }
                }
            }
        }
    }
	return FALSE;
}

void SplitCString(const CString& _cstr, const CString& _flag, CStringArray& _resultArray)
{
	//CString str = _T("one+two+three+four");

	int nTokenPos = 0;
	CString strToken = _cstr.Tokenize(_flag, nTokenPos);

	while (!strToken.IsEmpty())
	{
		AfxMessageBox(strToken);

		_resultArray.Add(strToken);
		strToken = _cstr.Tokenize(_flag, nTokenPos);
	}
}

// CAutoUpdateClientApp ����

CAutoUpdateClientApp::CAutoUpdateClientApp()
{

}

CAutoUpdateClientApp theApp;

BOOL CAutoUpdateClientApp::InitInstance()
{
	InitCommonControls();

	::CoInitialize(NULL);

	CWinApp::InitInstance();

	if (!AfxSocketInit())
	{
		AfxMessageBox(IDP_SOCKETS_INIT_FAILED);
		return FALSE;
	}

	AfxEnableControlContainer();

	SetRegistryKey(_T("Droiyan Online AutoUpdate Client V1.0"));

#ifdef TEST_DOWNLOAD
	m_lpCmdLine = _T("-d");
#endif
	CString sCommond = m_lpCmdLine;
	CString strUrl = _T("");
	if (!sCommond.IsEmpty()) 
	{
		strUrl = sCommond;
	}
	else
	{
		PostQuitMessage(0);
		return false;
	}

	//AfxMessageBox(_T("AutoUpdate - ����ǰ��updateurl: ") + sCommond); //xiaowujz.com

	if(StringContainOtherString(sCommond,_T("-MANUAL"))) { 
		//AfxMessageBox(_T("��⵽MANNUL, ���°汾")); //xiaowujz.com

		int nTokenPos = 0;
		CString strToken = sCommond.Tokenize(_T(" "), nTokenPos);
		strUrl = strToken;

		//AfxMessageBox(_T("AutoUpdate - ������updateurl: ") + strUrl); //xiaowujz.com

	} else {
		AfxMessageBox(_T("�������þɰ汾�Ŀͻ��ˣ��������°��½��")); //xiaowujz.com
		return false;
	}

	CAutoUpdateClientDlg dlg;
	dlg.SetUpdateUrl(strUrl);
	dlg.m_InstanceChecker.ActivateChecker();
	if (dlg.m_InstanceChecker.PreviousInstanceRunning())
	{
		//AfxMessageBox(_T("��ս��¼���Ѵ�,��鿴�����������½�ͼ��."), MB_OK);
		dlg.m_InstanceChecker.ActivatePreviousInstance();
		m_pMainWnd = NULL;
		return FALSE;
	}
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{

	}
	else if (nResponse == IDCANCEL)
	{

	}

	return FALSE;
}


int CAutoUpdateClientApp::ExitInstance()
{
	::CoUninitialize();

	return CWinApp::ExitInstance();
}
