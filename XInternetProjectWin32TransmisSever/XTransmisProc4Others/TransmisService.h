#pragma once

#include <windows.h>
#include <atlstr.h>
#include "XStream.h"

class CTransmisService
{
private:
	CTransmisService(void);
	~CTransmisService(void);

public:
	static CTransmisService& Instance();


public:
	int Start(int session_id);
	int Stop();


private:
	int m_nSessionID;
	CXNetStream * m_TcpStream;

	BOOL m_bThreadExit;//线程是否退出
	HANDLE m_hThread;
	static int	WorkerThread(void*);
	void Worker();
};
