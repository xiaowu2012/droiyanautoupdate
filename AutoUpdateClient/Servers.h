#pragma once

#include ".\server.h"

#include "kevinautolock.h"
#include <list>
using namespace std;

class CServers
{
public:
	CServers(void);
	virtual ~CServers(void);

private:
	list<CServer *> m_listServer;
	CKevinCritSec m_lock;
	bool AddServer(CServer* pServer);
public:
	void ClearAllList(void);
	bool ReadFromXMLFile(TCHAR* pszFileName);
	bool SaveToXMLFile(TCHAR* pszFileName);
	bool AddServer(CString sServerName, CString sUpdateFileXmlURL);
	void FillCmbCtrl(CComboBox& m_cmbCtrl);
	void DeleteItem(CServer* pServer);
	DWORD Count(void);
};
