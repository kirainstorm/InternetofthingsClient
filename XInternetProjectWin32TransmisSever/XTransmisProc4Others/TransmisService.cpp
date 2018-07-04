#include "StdAfx.h"
#include "TransmisService.h"

CTransmisService::CTransmisService(void)
{
	m_TcpStream = NULL;
	m_nSessionID = 0;
	m_bThreadExit = FALSE;
	m_hThread = NULL;
}
CTransmisService::~CTransmisService(void)
{
}
CTransmisService& CTransmisService::Instance()
{
	static CTransmisService ins;
	return ins;
}
int CTransmisService::Start(int session_id)
{
	m_nSessionID = session_id;
	m_bThreadExit = FALSE;
	m_hThread = CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)WorkerThread, this, NULL, NULL);
	return 0;
}
int CTransmisService::Stop()
{
	if (m_hThread)
	{
		m_bThreadExit = TRUE;
		WaitForSingleObject(m_hThread,INFINITE);
		CloseHandle(m_hThread);
		m_bThreadExit = NULL;
	}
	return 0;
}
int	CTransmisService::WorkerThread(void* param)
{
	CTransmisService *p = (CTransmisService *)param;
	p->Worker();
	return 0;
}
void CTransmisService::Worker()
{
	if (m_TcpStream)
	{
		m_TcpStream->Shutdown();
		m_TcpStream->Release();
	}

	do
	{
		//
/*		try{*/
			m_TcpStream = XNetCreateStream4Connect("127.0.0.1", 7891, 3);
// 		}
// 		catch (...)
// 		{
// 			printf("why !!!!!!!!!!!!!!!!!!!!!!!  XTransmisProc \r\n");
// 			break;
// 		}


		//
		if (0 != XNetConnectStream(m_TcpStream))
		{
			printf("链接 127.0.0.1：7891 错误");
			break;
		}
		//
		while (!m_bThreadExit)
		{
			//
			TRANSMIN_PROC_HEAD__T stSend, stRecv;
			memset(&stSend, 0, sizeof(TRANSMIN_PROC_HEAD__T));
			memset(&stRecv, 0, sizeof(TRANSMIN_PROC_HEAD__T));
			stSend.cmd = _TRANSMIS_PROC_HEARBEAT;
			stSend.session = m_nSessionID;
			stSend.k = 0xada2;
			//这里仅仅使用心跳信息告诉父程序自己还活着，或者父程序允许自己活着。使用传递过来的session id标识
			if (0 != m_TcpStream->SyncWriteAndRead(&stSend, sizeof(TRANSMIN_PROC_HEAD__T), &stRecv, sizeof(TRANSMIN_PROC_HEAD__T)))
			{
				printf("与 127.0.0.1：7891 进行SyncWriteAndRead 错误");
				break;
			}

			g_dwLastTick = GetTickCount64();//-<<<<<<<<<<<<<<<<---======
			Sleep(2000);
		}
	} while (0);

	if (m_TcpStream)
	{
		m_TcpStream->Shutdown();
		m_TcpStream->Release();
		m_TcpStream = NULL;
	}
}