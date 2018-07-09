#include "stdafx.h"
#include "ActiveConnService.h"


extern volatile BOOL g_bServiceExit;

CActiveConnService::CActiveConnService()
{
	m_hServer = NULL;
	m_hThread = NULL;
	m_hEvtWorkerStop = FALSE;
}

CActiveConnService::~CActiveConnService()
{
	Stop();
}

int CActiveConnService::Start()
{
	if (m_hServer == NULL)
	{
		m_hServer = XNetCreateListen(this, 7891);
		m_hEvtWorkerStop = FALSE;
		m_hThread = ::CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)WorkerThread, this, 0, NULL);
	}
	return 0;
};

int CActiveConnService::Stop()
{
	if (m_hServer)
	{
		//
		m_hEvtWorkerStop = TRUE;
		if (m_hThread)
		{
			WaitForSingleObject(m_hThread, INFINITE);
			CloseHandle(m_hThread);
			m_hThread = NULL;
		}
		//
		XNetDestroyListen(m_hServer);//停止监听，取消新的链接接入
		m_hServer = NULL;
	}
	return 0;
};

void CActiveConnService::OnAcceptComplete(CXNetStream * pAcceptStream)
{
	if (g_bServiceExit)
	{
		pAcceptStream->Release();
		return;
	}

	CActiveConnDeviceSession * pSession = new CActiveConnDeviceSession(pAcceptStream);
	pAcceptStream->SetStreamData(pSession);
};

void* WINAPI CActiveConnService::WorkerThread(void* param)
{
	CActiveConnService *pService = (CActiveConnService*)param;
	pService->Worker();
	return NULL;
}


void CActiveConnService::Worker()
{
	while (!m_hEvtWorkerStop)
	{
		g_csProcessSession.Lock();
		for (map<int, MyStruct4Process*>::iterator it = g_mapProcess.begin(); it != g_mapProcess.end(); it++)
		{
			//printf("--------- ");
			//打开进程
			if (FALSE == it->second->bOpenProc)
			{
				if (0 == OpenTransmisProcProcess(*it->second))
				{

					it->second->bOpenProc = TRUE;
					it->second->dw = CrossGetTickCount64();
					printf("OpenTransmisProcProcess %s\n", it->second->strParam);
				}
			}
			else
			{
				if ((CrossGetTickCount64() - it->second->dw) > (10 * 1000))
				{
					CloseTransmisProcProcess(*it->second);
					it->second->bOpenProc = FALSE;
				}
			}
		}
		g_csProcessSession.Unlock();


		Sleep(1000);
	}
}
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
int CActiveConnService::OpenTransmisProcProcess(MyStruct4Process &t)
{
	//获取执行路径
	CString strPath;
	::GetModuleFileName(NULL, strPath.GetBufferSetLength(MAX_PATH), MAX_PATH);
	strPath.ReleaseBuffer();
	int p = strPath.ReverseFind(_T('\\'));
	strPath = strPath.Left(p + 1);
	//启动设备访问进程
#if 0
	//ACL
	SECURITY_DESCRIPTOR 	sd;
	InitializeSecurityDescriptor(&sd, SECURITY_DESCRIPTOR_REVISION);
	SetSecurityDescriptorDacl(&sd, TRUE, (PACL)NULL, FALSE);

	SECURITY_ATTRIBUTES	sa;
	sa.nLength = sizeof(SECURITY_ATTRIBUTES);
	sa.bInheritHandle = FALSE;
	sa.lpSecurityDescriptor = &sd;
#endif

	WCHAR wszInfo[256] = { 0 };
	MultiByteToWideChar(CP_UTF8, 0, t.strParam, (int)strlen(t.strParam) + 1, wszInfo, (int)(sizeof(wszInfo) / sizeof(wszInfo[0])));
	CString strCmd;

#ifdef _WIN64
	strCmd.Format(_T("\"%s..\\XTransmisProc.exe\" %s"), strPath, wszInfo);
#else
	strCmd.Format(_T("\"%sXTransmisProc.exe\" %s"), strPath, wszInfo);
#endif


	STARTUPINFO si;

	ZeroMemory(&t.pi, sizeof(t.pi));
	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);
#if 0
	if (!CreateProcess(NULL, (LPWSTR)(LPCTSTR)strCmd, &sa, &sa, FALSE, 0, NULL, NULL, &si, &pi))
	{
		return -1;
	}
#endif

	if (!CreateProcess(NULL, (LPWSTR)(LPCTSTR)strCmd, NULL, NULL, FALSE, 0, NULL, NULL, &si, &t.pi))
	{

		printf("CreateProcess error");
		return -1;
	}
	//CloseHandle(pi.hThread);
	//CloseHandle(pi.hProcess);

	return 0;
}

int CActiveConnService::CloseTransmisProcProcess(MyStruct4Process &t)
{
	if (0 != t.pi.hProcess)
	{
		printf("CloseTransmisProcProcess %s   [%d]\n", t.strParam, t.pi.hThread);

		BOOL b;
		b = TerminateThread(t.pi.hThread, 0);
		b = TerminateProcess(t.pi.hProcess, 0);
		b = CloseHandle(t.pi.hThread);
		b = CloseHandle(t.pi.hProcess);
		ZeroMemory(&t.pi, sizeof(t.pi));
	}

	return 0;
}