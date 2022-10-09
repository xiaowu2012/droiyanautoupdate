#pragma once

#include "updatefile.h"

class CServer
{
public:
	CServer(void);
	virtual ~CServer(void);
	CString m_sServerName;
	CString m_sUpdateFileXmlURL;
};
