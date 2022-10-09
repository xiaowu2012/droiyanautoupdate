#include "StdAfx.h"
#include ".\updatefiles.h"
#include ".\md5.h"
#include "..\zlib\zconf.h"
#include "..\zlib\zlib.h"


bool ZipFile(LPTSTR ufilepath, LPTSTR uzippath)
{
	char filepath[260],zippath[260];
	memset(filepath, 0, sizeof(filepath));
	memset(zippath, 0, sizeof(zippath));

	WideCharToMultiByte(CP_ACP, NULL, ufilepath, _tcslen(ufilepath) + 1, filepath, sizeof(filepath), NULL, NULL);
	WideCharToMultiByte(CP_ACP, NULL, uzippath, _tcslen(uzippath) + 1, zippath, sizeof(zippath), NULL, NULL);

	bool result = false;

	FILE* pf = fopen(filepath, "rb");
	if(!pf) return false;

	fseek(pf, 0, SEEK_END);
	int filesize = ftell(pf);
	fseek(pf, 0, SEEK_SET);

	char* sourcebuf = new char[filesize + 1];
	fread(sourcebuf, filesize, 1, pf);
	sourcebuf[filesize] = '\0';

	fclose(pf);

	int targetsize = filesize * 2;
	char* targetbuf = new char[targetsize];
	memset(targetbuf, 0, targetsize);

	int ret = compress((Bytef*)targetbuf, (uLongf*)&targetsize, (Bytef*)sourcebuf, (uLong)filesize);
	if(ret != Z_OK) 
	{
		// 压缩失败？
		MessageBox(NULL, TEXT("竟然有压缩失败的"), TEXT("出错"), MB_OK);
		goto go_result;
	}
	if(targetsize <= 2) goto go_result;

	//strcpy(zippath, filepath);
	//strcat(zippath, ".zip");

	pf = fopen(zippath, "w+b");
	if(!pf)
	{
		goto go_result;
	}

	int first , second;
	first = (BYTE)targetbuf[0];
	second = (BYTE)targetbuf[1];

	if(first == 0x78) targetbuf[0] = 0xAA;
	if(second == 0x9C) targetbuf[1] = 0x55;
	
	fwrite(&targetsize, sizeof(int), 1, pf);
	fwrite(&filesize,  sizeof(int), 1, pf);

	fwrite(targetbuf, targetsize, 1, pf);
	fclose(pf);

	result = true;

go_result:

	if(sourcebuf)
		delete [] sourcebuf;
	sourcebuf = NULL;
	if(targetbuf)
		delete [] targetbuf;
	targetbuf = NULL;

	return result;

}

bool UnZipFile(LPTSTR ufilepath, LPTSTR uzippath)
{
	char filepath[260],zippath[260];
	memset(filepath, 0, sizeof(filepath));
	memset(zippath, 0, sizeof(zippath));

	WideCharToMultiByte(CP_ACP, NULL, ufilepath, _tcslen(ufilepath) + 1, filepath, sizeof(filepath), NULL, NULL);
	WideCharToMultiByte(CP_ACP, NULL, uzippath, _tcslen(uzippath) + 1, zippath, sizeof(zippath), NULL, NULL);

	bool result = false;

	FILE* pf = fopen(zippath, "r+b");
	if(!pf) return false;

	fseek(pf, 0, SEEK_END);
	int filesize = ftell(pf);
	fseek(pf, 0, SEEK_SET);

	if(filesize <= 8) 
	{
		fclose(pf);
		return false;
	}

	int sourcesize = 0, targetsize = 0;
	fread(&sourcesize, sizeof(int), 1, pf);
	fread(&targetsize, sizeof(int), 1, pf);

	if(sourcesize <= 2)
	{
		fclose(pf);
		return false;
	}

	char* sourcebuf = new char[sourcesize + 1];
	fread(sourcebuf, sourcesize, 1, pf);
	sourcebuf[sourcesize] = '\0';

	fclose(pf);

	int first , second;
	first = (BYTE)sourcebuf[0];
	second = (BYTE)sourcebuf[1];

	if(first == 0xAA) sourcebuf[0] = 0x78;
	if(second == 0x55) sourcebuf[1] = 0x9C;

	char* targetbuf = new char[targetsize + 1];
	memset(targetbuf, 0, targetsize + 1);

	int ret = uncompress((Bytef*)targetbuf, (uLongf*)&targetsize, (Bytef*)sourcebuf, (uLong)sourcesize);
	if(ret != Z_OK) goto go_result;
	if(targetsize <= 2) goto go_result;

	//strcpy(zippath, filepath);
	//strcat(zippath, ".zip");

	pf = fopen(filepath, "w+b");
	if(!pf)
	{
		goto go_result;
	}

	fwrite(targetbuf, targetsize, 1, pf);
	fclose(pf);


	DeleteFile(uzippath);

	result = true;

go_result:

	if(sourcebuf)
		delete [] sourcebuf;
	sourcebuf = NULL;
	if(targetbuf)
		delete [] targetbuf;
	targetbuf = NULL;

	return result;

}

CUpdateFiles::CUpdateFiles(void)
{
	::GetModuleFileName(NULL, m_szMainDir, sizeof(m_szMainDir));
	TCHAR *p = _tcsrchr(m_szMainDir, _T('\\'));	if (p) 		*p = 0;
	::GetWindowsDirectory(m_szWindowsDir, sizeof(m_szWindowsDir));
	::GetSystemDirectory(m_szSystemDir, sizeof(m_szSystemDir));
}

CUpdateFiles::~CUpdateFiles(void)
{
	ClearAllList();
}

void CUpdateFiles::ClearAllList(void)
{
	CKevinAutoLock  lock(&m_lock);

	while (m_listFile.size() > 0) {
		list<CUpdateFile*>::iterator it = m_listFile.begin();
		CUpdateFile* pUpdateFile = *it;
		m_listFile.erase(it);
		SAFE_DELETE(pUpdateFile);
	}
}

bool CUpdateFiles::ReadFromXMLFile(TCHAR* pszFileName,  BOOL bFromZipFile)
{
	ClearAllList();

	CKevinAutoLock  lock(&m_lock);

	bool bRet = false;

	TCHAR szXmlPath[MAX_PATH]; 
	ZeroMemory(szXmlPath, sizeof(szXmlPath));
	_tcscpy(szXmlPath, pszFileName);

	if(bFromZipFile == TRUE)
	{
		_tcscat(szXmlPath, _T(".xml"));
		if(!UnZipFile(szXmlPath, pszFileName)) return false;
	}

	IXMLDOMDocument* pDOMDocument = NULL;
	IXMLDOMElement*  pDOMElement = NULL;
	IXMLDOMNodeList* pDOMNodeList = NULL;
	BSTR bstrBaseName = ::SysAllocString(_T("File"));
	CComVariant vFile(szXmlPath);	VARIANT_BOOL vBool;
	
	if(SUCCEEDED(CoCreateInstance(CLSID_DOMDocument, NULL, CLSCTX_INPROC_SERVER, IID_IXMLDOMDocument, (void**)&pDOMDocument) )) 
	{	
		if(SUCCEEDED(pDOMDocument->put_async(VARIANT_FALSE)) && SUCCEEDED(pDOMDocument->load(vFile, &vBool))) 
		{
			if(VARIANT_TRUE == vBool && SUCCEEDED(pDOMDocument->get_documentElement(&pDOMElement))) 
			{
				if (SUCCEEDED(pDOMElement->getElementsByTagName(bstrBaseName, &pDOMNodeList))) 
				{
					return GetFileList(pDOMNodeList);
				}
			}
		}
	}

	::SysFreeString(bstrBaseName);
	SAFE_RELEASE(pDOMDocument);
	SAFE_RELEASE(pDOMElement);
	SAFE_RELEASE(pDOMNodeList);

	return bRet;
}

//获取所有的更新文件
bool CUpdateFiles::GetFileList(IXMLDOMNodeList* pDOMNodeList)
{
	long lLen = 0;
	if (SUCCEEDED(pDOMNodeList->get_length(&lLen))) 
	{
		/*CString strTemp ;
		strTemp.Format(_T("共有%d个文件"),lLen);
		AfxMessageBox((LPCTSTR)strTemp);*/
		for (int i = 0; i < lLen; i ++) 
		{	
			IXMLDOMNode* pNode = NULL;
			if (SUCCEEDED(pDOMNodeList->get_item(i, &pNode))) 
			{
				CUpdateFile* pUpdateFile = new CUpdateFile(_T(""));
				if (!pUpdateFile) return false;

				IXMLDOMNode* pNext=NULL;
				IXMLDOMNode* pChild;
				pNode->get_firstChild(&pChild);

				USES_CONVERSION;
				while(pChild)
				{					
					TCHAR szPathName[_MAX_PATH];
					memset(szPathName, 0, sizeof(szPathName));
					BSTR bstrNodeName, bstrText;
					pChild->get_nodeName(&bstrNodeName);
					pChild->get_text(&bstrText);

					if (0 == _tcsicmp(bstrNodeName, _T("FileName"))) 
					{
						pUpdateFile->m_sFileName = bstrText;
					}
					else if (0 == _tcsicmp(bstrNodeName, _T("LocalPath")))
					{
						pUpdateFile->m_sLocalPath = bstrText;
					}
					else if (0 == _tcsicmp(bstrNodeName, _T("URL")))
					{
						pUpdateFile->m_sURL = bstrText;
					}
					else if (0 == _tcsicmp(bstrNodeName, _T("MD5")))
					{
						pUpdateFile->m_sMD5 = bstrText;
					}
					/*else if (0 == _tcsicmp(bstrNodeName, _T("FileVersion"))){
						pUpdateFile->m_sFileVersion = bstrText;
					}
					else if (0 == _tcsicmp(bstrNodeName, _T("ProductVersion"))){
						pUpdateFile->m_sProductVersion = bstrText;
					}
					else if (0 == _tcsicmp(bstrNodeName, _T("Length"))){
						pUpdateFile->m_ullSize = atoi(T2A(bstrText));
					}
					else if (0 == _tcsicmp(bstrNodeName, _T("FileDateLow"))){
						pUpdateFile->m_fileTime.dwLowDateTime = atoi(T2A(bstrText));
					}
					else if (0 == _tcsicmp(bstrNodeName, _T("FileDateHigh"))){
						pUpdateFile->m_fileTime.dwHighDateTime = atoi(T2A(bstrText));
					}
					else if (0 == _tcsicmp(bstrNodeName, _T("InstallDir")))
					{
						pUpdateFile->m_sInstallDir = bstrText;
					}
					else if (0 == _tcsicmp(bstrNodeName, _T("Register"))){
						pUpdateFile->m_bRegister = (0 == _tcsicmp(bstrText, _T("YES"))) ? true : false;
					}
					else if (0 == _tcsicmp(bstrNodeName, _T("ZipFile"))){
						pUpdateFile->m_bZipFile = (0 == _tcsicmp(bstrText, _T("YES"))) ? true : false;
					}
					else if (0 == _tcsicmp(bstrNodeName, _T("ForceOverwrite"))){
						pUpdateFile->m_bForceOverwrite = (0 == _tcsicmp(bstrText, _T("YES"))) ? true : false;
					}
					else if (0 == _tcsicmp(bstrNodeName, _T("Execute")))
					{
						pUpdateFile->m_bExecute = (0 == _tcsicmp(bstrText, _T("YES"))) ? true : false;
					}
					else if (0 == _tcsicmp(bstrNodeName, _T("ExecutePar")))
					{
						pUpdateFile->m_sExecutePar = bstrText;
					}
					else if (0 == _tcsicmp(bstrNodeName, _T("UserName"))){
						pUpdateFile->m_sUserName = bstrText;
					}
					else if (0 == _tcsicmp(bstrNodeName, _T("Password"))){
						pUpdateFile->m_sPassword = bstrText;
					}*/
					::SysFreeString(bstrNodeName);
					::SysFreeString(bstrText);

					pChild->get_nextSibling(&pNext);
					pChild->Release();
					pChild = pNext;
				}
				
				if (pUpdateFile->m_sLocalPath.IsEmpty()) 
				{
					/*if (0 == pUpdateFile->m_sInstallDir.CollateNoCase(_T("WINDOWSDIR"))) 
					{
						pUpdateFile->m_sFilePath.Format(_T("%s\\%s"),m_szWindowsDir, pUpdateFile->m_sFileName);
					} 
					else if (0 == pUpdateFile->m_sInstallDir.CollateNoCase(_T("SYSTEMDIR"))) 
					{
						pUpdateFile->m_sFilePath.Format(_T("%s\\%s"),m_szSystemDir, pUpdateFile->m_sFileName);
					} 
					else 
					{*/
						pUpdateFile->m_sFilePath.Format(_T("%s\\%s"),m_szMainDir, pUpdateFile->m_sFileName);
					//}
				}
				else 
				{
					/*if (0 == pUpdateFile->m_sInstallDir.CollateNoCase(_T("WINDOWSDIR"))) 
					{
						pUpdateFile->m_sFilePath.Format(_T("%s\\%s\\%s"),m_szWindowsDir, pUpdateFile->m_sLocalPath, pUpdateFile->m_sFileName);
					} 
					else if (0 == pUpdateFile->m_sInstallDir.CollateNoCase(_T("SYSTEMDIR"))) 
					{
						pUpdateFile->m_sFilePath.Format(_T("%s\\%s\\%s"),m_szSystemDir, pUpdateFile->m_sLocalPath, pUpdateFile->m_sFileName);
					} 
					else 
					{*/
						//在这里如何去判断这个目录是否存在


						pUpdateFile->m_sFilePath.Format(_T("%s\\%s\\%s"),m_szMainDir, pUpdateFile->m_sLocalPath, pUpdateFile->m_sFileName);
					//}
				}

				/*
				判断目录是否存在并创建目录：
　　// Test Whether the dir exist

　　CString m_dir;

　　if (m_dir.Right(1) == “\\”)

　　m_dir = m_dir.Left(m_dir.GetLength()-1);

　　if (GetFileAttributes(m_dir) == FILE_ATTRIBUTE_DIRECTORY)

　　return TURE;

　　else

　　CreateAllDirectory(m_dir);

　　// Function CreateAllDirectory

　　// recursive function

　　void CreateAllDirectory(CString Dir)

　　{

　　if (Dir.Right(1) == “\\”)

　　Dir = Dir.Left(Dir.GetLength()-1);

　　if (GetFileAttributes == FILE_ATTRIBUTE_DIRECTORY)

　　return;

　　else if (GetFileAttributes(Dir) != -1)

　　{

　　if (DeleteFile(Dir)) // delete the file with the same name

　　if (CreateDirectory(Dir, NULL))

　　return;

　　MessageBox(_T(“Can not create directory for captured pictures”), NULL, MB_OK);

　　}

　　int n = Dir.ReverseFind(‘\\’);

　　CreateAllDirectory(Dir.Left(n));

　　if (!CreateDirectory(Dir, NULL))

　　MessageBox(_T(“Can not create directory for captured pictures”), NULL, MB_OK);

　　}

　　考试大提示判断Dir中某个文件是否存在,还可以通过FileFind来实现:

　　BOOL FileExist(CString strFileName)

　　{

　　CFileFind fFind;

　　return fFind.FindFile(strFileName);

　　}

				*/
				
				m_listFile.push_back(pUpdateFile);
				TRACE("新文件对象添加：%s, 对象总数：%u\n", T2A(pUpdateFile->m_sFileName), m_listFile.size());
				SAFE_RELEASE(pNode);
			}
		}
		return true;
	}

	return false;
}


// 保存更新文件的同时，把文件COPY到相对应的目录
bool CUpdateFiles::SaveToXMLFile(TCHAR* pszFileName)
{
	CKevinAutoLock  lock(&m_lock);

	bool bRet = false;

	TCHAR szTargetZipPath[MAX_PATH];

	TCHAR fullpath[MAX_PATH]; _tcscpy(fullpath, pszFileName);
	TCHAR fulldir[_MAX_DIR];
	TCHAR filedrive[MAX_PATH];
	TCHAR filename[MAX_PATH];
	TCHAR fileext[MAX_PATH];
	_tsplitpath(fullpath, filedrive, fulldir, NULL, NULL); 

	CFile f;
	if (f.Open(pszFileName, CFile::modeCreate | CFile::modeWrite)) 
	{
		f.Write(XML_HEADER, (UINT)strlen(XML_HEADER));

		TCHAR filezip[MAX_PATH]; memset(filezip, 0x0, sizeof(filezip));
		char szText[512], szDate[256];
		CTime t = CTime::GetCurrentTime();
		sprintf(szDate, "%.4u-%.2u-%.2u %.2u:%.2u:%.2u", t.GetYear(), t.GetMonth(), t.GetDay(),
			t.GetHour(), t.GetMinute(), t.GetSecond());
		sprintf(szText, XML_PRODUCT_HEADER, "小屋决战更新程序", szDate, m_listFile.size());
		f.Write(szText, (UINT)strlen(szText));

		char szXMLStr[1024];
		m_listFile.sort();
		list<CUpdateFile*>::iterator it = m_listFile.begin();
		while (it != m_listFile.end()) 
		{
			CUpdateFile* pUpdateFile = *it;
			if (BuildXMLStr(pUpdateFile, szXMLStr)) 
			{
				f.Write(szXMLStr, (UINT)strlen(szXMLStr));


				_tcscpy(fullpath, (LPTSTR)(LPCTSTR)pUpdateFile->m_sFilePath);
				_tsplitpath(fullpath, filedrive, fulldir, filename, fileext);
				_tcscpy(filezip, filedrive);
				_tcscat(filezip, fulldir);
				_tcscat(filezip, _T("\\"));
				_tcscat(filezip, filename);
				_tcscat(filezip, fileext);
				_tcscat(filezip, _T(".zip"));

				ZipFile(fullpath, filezip);

				//CopyFile(pUpdateFile->m_sFilePath, filezip, FALSE);

				it++;
			}
			else
			{
				// 有失败的文件
			}
		}

		bRet = true;
		f.Write(XML_PRODUCT_TAIL, (UINT)strlen(XML_PRODUCT_TAIL));
		f.Write(XML_TAIL, (UINT)strlen(XML_TAIL));
		f.Close();
	}

	if(bRet == true)
	{
		_tcscpy(szTargetZipPath, pszFileName);
		_tcscat(szTargetZipPath, _T(".zip"));
		ZipFile(pszFileName, szTargetZipPath);
	}

	return bRet;
}

bool CUpdateFiles::SaveToLstFile(TCHAR* pszFileName)
{
	CKevinAutoLock  lock(&m_lock);

	bool bRet = false;

	TCHAR fullpath[MAX_PATH]; _tcscpy(fullpath, pszFileName);
	TCHAR fulldir[_MAX_DIR];
	TCHAR filedrive[MAX_PATH];
	TCHAR filename[MAX_PATH];
	TCHAR fileext[MAX_PATH];
	_tsplitpath(fullpath, filedrive, fulldir, NULL, NULL); 

	CFile f;
	if (f.Open(pszFileName, CFile::modeCreate | CFile::modeWrite)) 
	{
		TCHAR filezip[MAX_PATH]; memset(filezip, 0x0, sizeof(filezip));
		char szText[1024], szDate[256];
		CTime t = CTime::GetCurrentTime();
		sprintf(szDate, "%.4u-%.2u-%.2u %.2u:%.2u:%.2u", t.GetYear(), t.GetMonth(), t.GetDay(),
			t.GetHour(), t.GetMinute(), t.GetSecond());
		sprintf(szText, ";文件数量：%d，更新时间：%s.\r\n", m_listFile.size(), szDate);
		f.Write(szText, (UINT)strlen(szText));

		sprintf(szText, "#:%d \r\n", m_listFile.size());
		f.Write(szText, (UINT)strlen(szText));

		m_listFile.sort();
		list<CUpdateFile*>::iterator it = m_listFile.begin();
		while (it != m_listFile.end()) 
		{
			CUpdateFile* pUpdateFile = *it;

			if (BuildXMLStr(pUpdateFile, szText)) 
			{
				memset(szText, 0x00, sizeof(szText));
									// 文件名|URL|目标目录|MD5

				USES_CONVERSION;
				sprintf(szText, "%s|%s|%s|%s\r\n", 
					T2A(pUpdateFile->m_sFileName), 
					T2A(pUpdateFile->m_sURL),
					T2A(pUpdateFile->m_sLocalPath),
					T2A(pUpdateFile->m_sMD5));
				f.Write(szText, (UINT)strlen(szText));

				_tcscpy(fullpath, (LPTSTR)(LPCTSTR)pUpdateFile->m_sFilePath);
				_tsplitpath(fullpath, filedrive, fulldir, filename, fileext);
				_tcscpy(filezip, filedrive);
				_tcscat(filezip, fulldir);
				_tcscat(filezip, _T("\\"));
				_tcscat(filezip, filename);
				_tcscat(filezip, fileext);
				_tcscat(filezip, _T(".zip"));

				ZipFile(fullpath, filezip);

				//CopyFile(pUpdateFile->m_sFilePath, filezip, FALSE);

				it++;
			}
		}

		bRet = true;
		f.Close();
	}

	return bRet;
}

bool CUpdateFiles::AddUpdateFile(TCHAR* pszFileName)
{
	CKevinAutoLock  lock(&m_lock);

	TCHAR fullpath[MAX_PATH]; _tcscpy(fullpath, pszFileName);
	TCHAR fulldir[_MAX_DIR];
	TCHAR filedrive[MAX_PATH];
	TCHAR filename[MAX_PATH];
	TCHAR fileext[MAX_PATH];
	_tsplitpath(pszFileName, filedrive, fulldir, filename, fileext); 

	if(_tcsicmp(fileext, TEXT(".zip")) == 0) return TRUE;
	if(_tcsicmp(fileext, TEXT(".ini")) == 0) return TRUE;
	if(_tcsicmp(filename, TEXT("autoupdate")) == 0) return TRUE;
	if(_tcsicmp(filename, TEXT("autoupdate")) == 0) return TRUE;
	if(m_sModulePath.CompareNoCase(filename) == 0) return TRUE; // 排除和自己文件名字一样的文件

	//删除重复
	list<CUpdateFile*>::iterator it = m_listFile.begin();
	while (it != m_listFile.end()) 
	{
		CUpdateFile* pUpdateFile = *it;
		if (pUpdateFile) 
		{
			if (pUpdateFile->m_sFilePath.CompareNoCase(pszFileName) == 0) 
			{
				it = m_listFile.erase(it);
				SAFE_DELETE(pUpdateFile)
				continue;
			}
		}
		it ++;
	}

	CUpdateFile* pUpdateFile = new CUpdateFile(pszFileName);
	if (pUpdateFile) 
	{
		if (pUpdateFile->InitUpdateFile()) 
		{
			m_listFile.push_back(pUpdateFile);
			return true;
		}
		else SAFE_DELETE(pUpdateFile);
	}

	return false;
}


bool CUpdateFiles::BuildXMLStr(CUpdateFile* pUpdateFile, char szXMLStr[1024])
{
	bool bRet = false;

	if (pUpdateFile) 
	{

		/*CString strFileName = pUpdateFile->m_sFileName;
		CString strExt = strFileName.Right(3);
		CString strPre = strFileName.Left(3);

		if(strExt.CompareNoCase(_T("say")) == 0)
		{
			pUpdateFile->m_sLocalPath = _T("nRes/800/");
		}
		else if(strExt.CompareNoCase(_T("cmp")) == 0)
		{
			pUpdateFile->m_sLocalPath = _T("DLG");
		}
		else if(strExt.CompareNoCase(_T("cm8")) == 0)
		{
			pUpdateFile->m_sLocalPath = _T("DLG");
		}
		else if(strExt.CompareNoCase(_T("ifb")) == 0)
		{
			pUpdateFile->m_sLocalPath = _T("nRes");
		}
		else if(strExt.CompareNoCase(_T("atm")) == 0 || strExt.CompareNoCase(_T("imf")) == 0 || strExt.CompareNoCase(_T("smo")) == 0 || strExt.CompareNoCase(_T("stm")) == 0 || strExt.CompareNoCase(_T("tsd")) == 0 )
		{
			pUpdateFile->m_sLocalPath = _T("Map");
		}

		if(strPre.CompareNoCase(_T("emm")) == 0 || strPre.CompareNoCase(_T("eww")) == 0 || strPre.CompareNoCase(_T("mmm")) == 0 || strPre.CompareNoCase(_T("www")) == 0)
		{
			pUpdateFile->m_sLocalPath = _T("Status");
		}
		else
		{
			if(strExt.CompareNoCase(_T("spl")) == 0)
			{
				if(strFileName.GetLength() == 12)
				{
					pUpdateFile->m_sLocalPath = _T("Char");
				}
			}
		}
		
		if(strFileName.CompareNoCase(_T("item2.spl")) == 0 || strFileName.CompareNoCase(_T("item3.spl")) == 0 || strFileName.CompareNoCase(_T("item4.spl")) == 0 || strFileName.CompareNoCase(_T("npc.spl")) == 0 )
		{
			pUpdateFile->m_sLocalPath = _T("nRes");
		}

		if(pUpdateFile->m_sLocalPath.GetLength() == 0)
		{
			if(m_sRootPath.GetLength() > 0 && m_sRootPath.GetLength() < pUpdateFile->m_sFilePath.GetLength())
			{
				pUpdateFile->m_sLocalPath = pUpdateFile->m_sFilePath;
				pUpdateFile->m_sLocalPath.Delete(0, m_sRootPath.GetLength());

				pUpdateFile->m_sLocalPath.Delete(pUpdateFile->m_sLocalPath.ReverseFind('\\') + 1, pUpdateFile->m_sFileName.GetLength());
			}
			else
			{
				pUpdateFile->m_sLocalPath = TEXT("/");
			}

			if(pUpdateFile->m_sLocalPath.CompareNoCase( TEXT("\\")) == 0)
			{
				pUpdateFile->m_sLocalPath = TEXT("/");
			}

			pUpdateFile->m_sLocalPath.Replace(TEXT("\\"), TEXT("/"));
			pUpdateFile->m_sLocalPath.Replace(TEXT("//"), TEXT("/"));

		}*/

		pUpdateFile->m_sLocalPath = pUpdateFile->m_sFilePath;
		if(pUpdateFile->m_sLocalPath.ReverseFind(_T('\\')) != -1)
		{
			int iPos = pUpdateFile->m_sLocalPath.ReverseFind(_T('\\'));
			pUpdateFile->m_sLocalPath.Delete(iPos, pUpdateFile->m_sLocalPath.GetLength() - iPos);
		}
		pUpdateFile->m_sLocalPath.Delete(0, m_sModuleDir.GetLength() + 1);
		pUpdateFile->m_sLocalPath.Replace(_T("\\\\"),_T("/"));
		pUpdateFile->m_sLocalPath.Replace(_T("//"),_T("/"));

		/*if(pUpdateFile->m_sLocalPath.GetLength() == 0)
		{
			pUpdateFile->m_sLocalPath = _T("/");
		}*/

		pUpdateFile->m_sURL = _T("");
		if(m_sCommonURL.GetLength() > 0)
		{
			pUpdateFile->m_sURL.AppendFormat(_T("/%s"), m_sCommonURL);
		}
		if(pUpdateFile->m_sLocalPath.GetLength() > 0)
		{
			pUpdateFile->m_sURL.AppendFormat(_T("/%s"), pUpdateFile->m_sLocalPath);
		}
		pUpdateFile->m_sURL.AppendFormat(_T("/%s.zip"), pUpdateFile->m_sFileName);

		if(pUpdateFile->m_sLocalPath.GetLength() == 0)
		{
			pUpdateFile->m_sLocalPath = _T("/");
		}
		
		USES_CONVERSION;
		sprintf(
			szXMLStr, 

			"\t\t<File>\r\n"\
			"\t\t\t<FileName>%s</FileName>\r\n"\
		/*	"\t\t\t<FileVersion>%s</FileVersion>\r\n"\
			"\t\t\t<ProductVersion>%s</ProductVersion>\r\n"\
			"\t\t\t<Length>%u</Length>\r\n"\
			"\t\t\t<FileDateLow>%u</FileDateLow>\r\n"\
			"\t\t\t<FileDateHigh>%u</FileDateHigh>\r\n"\
			"\t\t\t<InstallDir>%s</InstallDir>\r\n"\
			"\t\t\t<Register>%s</Register>\r\n"\
			"\t\t\t<ZipFile>%s</ZipFile>\r\n"\
			"\t\t\t<ForceOverwrite>%s</ForceOverwrite>\r\n"\
			"\t\t\t<Execute>%s</Execute>\r\n"\
			"\t\t\t<ExecutePar>%s</ExecutePar>\r\n"\ */
			"\t\t\t<LocalPath>%s</LocalPath>\r\n"\
			"\t\t\t<URL>%s</URL>\r\n"\
	/*		"\t\t\t<UserName>%s</UserName>\r\n"\
			"\t\t\t<Password>%s</Password>\r\n"\*/
			"\t\t\t<MD5>%s</MD5>\r\n"\
			"\t\t</File>\r\n",

			T2A(pUpdateFile->m_sFileName), 
			//T2A(pUpdateFile->m_sFileVersion),
			//T2A(pUpdateFile->m_sProductVersion),  
			//(DWORD)pUpdateFile->m_ullSize, 
			//pUpdateFile->m_fileTime.dwLowDateTime,
			//pUpdateFile->m_fileTime.dwHighDateTime,
			//T2A(pUpdateFile->m_sInstallDir),
			//pUpdateFile->m_bRegister ? "REGISTER" : "NO",
			//pUpdateFile->m_bZipFile ? "ZIPFILE" : "NO",
			//pUpdateFile->m_bForceOverwrite ? "FORCEOVERWRITE" : "NO",
			//pUpdateFile->m_bExecute ? "EXECUTE" : "NO",
			//T2A(pUpdateFile->m_sExecutePar),
			T2A(pUpdateFile->m_sLocalPath),
			T2A(pUpdateFile->m_sURL),
			//T2A(pUpdateFile->m_sUserName),
			//T2A(pUpdateFile->m_sPassword),
			T2A(pUpdateFile->m_sMD5)
			);
		bRet = true;
	}

	return bRet;
}

void CUpdateFiles::FillListCtrl(CListCtrl& m_lstCtrl)
{
	CKevinAutoLock  lock(&m_lock);

	list<CUpdateFile*>::iterator it = m_listFile.begin();
	int iCount = 0; 
	TCHAR szText[16];
	while (it != m_listFile.end()) 
	{
		CUpdateFile* pUpdateFile = *it;
		if (pUpdateFile) 
		{
			iCount ++;
			_stprintf(szText, _T("%u"), iCount);
			int iItem = m_lstCtrl.InsertItem(
				LVIF_TEXT|LVIF_STATE, 0, szText, 
				LVIS_SELECTED, LVIS_SELECTED,
				0, 0);
			if (iItem != -1) 
			{
				//m_lstCtrl.SetItemText(iItem, 1, pUpdateFile->m_sFileVersion);
				m_lstCtrl.SetItemText(iItem, 1, _T("无版本"));
				_stprintf(szText, _T("%u"), (DWORD)pUpdateFile->m_ullSize);
				m_lstCtrl.SetItemText(iItem, 2, szText);
				m_lstCtrl.SetItemText(iItem, 3, pUpdateFile->m_sFileName);
				m_lstCtrl.SetItemText(iItem, 4, pUpdateFile->m_sFilePath);
				m_lstCtrl.SetItemData(iItem, (DWORD_PTR)pUpdateFile);
			}
		}
		it ++;
	}
}

void CUpdateFiles::DeleteItem(CUpdateFile * pUpdateFile)
{
	CKevinAutoLock  lock(&m_lock);
	m_listFile.remove(pUpdateFile);
	SAFE_DELETE(pUpdateFile);
}

CUpdateFile* CUpdateFiles::PopUpdateFile(void)
{
	CKevinAutoLock  lock(&m_lock);
	CUpdateFile* pUpdateFile = NULL;
	if (m_listFile.size() > 0) {
		pUpdateFile = m_listFile.front();
		m_listFile.pop_front();
	}
	return pUpdateFile;
}

void CUpdateFiles::PushUpdateFile(CUpdateFile* pUpdateFile)
{
	CKevinAutoLock  lock(&m_lock);
	m_listFile.push_back(pUpdateFile);
}

DWORD CUpdateFiles::CheckNeedUpdate(void)
{
	CKevinAutoLock  lock(&m_lock);
	DWORD dwUpdateCount = 0;
	list<CUpdateFile*>::iterator it = m_listFile.begin();
	while (it != m_listFile.end()) 
	{
		CUpdateFile* pUpdateFile = *it;
		if (pUpdateFile) 
		{
			if (pUpdateFile->CheckNeedUpdate())
				dwUpdateCount ++;
			else 
			{		// delete it
				it = m_listFile.erase(it);
				SAFE_DELETE(pUpdateFile);
				continue;
			}
		}
		it ++;
	}

	return dwUpdateCount;
}

//	获取得有文件的总大小
// 调用此函数前请先调用 CheckNeedUpdate() 以初始化 pUpdateFile->m_bLocalNeedUpdate
DWORD CUpdateFiles::GetNeedUpdateTotalLength(void)
{
	CKevinAutoLock  lock(&m_lock);
	DWORD dwSize = 0;
	list<CUpdateFile*>::iterator it = m_listFile.begin();
	while (it != m_listFile.end()) 
	{
		CUpdateFile* pUpdateFile = *it;
		if (pUpdateFile && pUpdateFile->m_bLocalNeedUpdate) {
			dwSize += (DWORD)pUpdateFile->m_ullSize;
		}
		it ++;
	}

	return dwSize;
}

DWORD CUpdateFiles::Count(void)
{
	CKevinAutoLock  lock(&m_lock);

	return (DWORD)m_listFile.size();
}