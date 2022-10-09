#include "StdAfx.h"
#include ".\servers.h"

CServers::CServers(void)
{
}

CServers::~CServers(void)
{
	ClearAllList();
}

void CServers::ClearAllList(void)
{
	CKevinAutoLock  lock(&m_lock);

	while (m_listServer.size() > 0) {
		list<CServer*>::iterator it = m_listServer.begin();
		CServer* pServer = *it;
		m_listServer.erase(it);
		SAFE_DELETE(pServer);
	}
};

bool CServers::ReadFromXMLFile(TCHAR* pszFileName)
{
	CKevinAutoLock  lock(&m_lock);

	return false;
};

bool CServers::SaveToXMLFile(TCHAR* pszFileName)
{
	CKevinAutoLock  lock(&m_lock);

	return false;
};

bool CServers::AddServer(CServer* pServer)
{
	CKevinAutoLock  lock(&m_lock);

	if (pServer) {
		m_listServer.push_back(pServer);
		return true;
	}

	return false;
};

bool CServers::AddServer(CString sServerName, CString sUpdateFileXmlURL)
{
	CServer* pServer = new CServer();
	if (pServer) { 
		pServer->m_sServerName = sServerName;
		pServer->m_sUpdateFileXmlURL = sUpdateFileXmlURL;
		return AddServer(pServer);
	}

	return false;
}

void CServers::FillCmbCtrl(CComboBox& m_cmbCtrl)
{
	CKevinAutoLock  lock(&m_lock);

	int iCount = 0; 
	list<CServer*>::iterator it = m_listServer.begin();
	while (it != m_listServer.end()) {
		CServer* pServer = *it;
		if (pServer) {
			iCount ++;
			int nItem = m_cmbCtrl.AddString(pServer->m_sServerName);
			m_cmbCtrl.SetItemData(nItem, (DWORD_PTR)pServer);
			m_cmbCtrl.SetCurSel(nItem);
		}
		it ++;
	}
};

void CServers::DeleteItem(CServer* pServer)
{
	CKevinAutoLock  lock(&m_lock);
	m_listServer.remove(pServer);
	SAFE_DELETE(pServer);
};

DWORD CServers::Count(void)
{
	CKevinAutoLock  lock(&m_lock);

	return (DWORD)m_listServer.size();
}