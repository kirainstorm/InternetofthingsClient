#pragma once

#include "PlatformDefine.h"
#include "XStream.hpp"
#include "XMsgDefForTransmisProcess.h"

class CTransmisClient
{
private:
	CTransmisClient(void) {
		m_bError = FALSE;
		m_TcpStream = NULL;
		m_hWorkerThread = CROSS_THREAD_NULL;
	};
	~CTransmisClient(void) {
	};

public:
	static CTransmisClient& Instance() {
		static CTransmisClient ins;
		return ins;
	};
// public:
// 	void OnPacketCompleteNetStreamData(int32_t bytesTransferred, emXStreamTansferKey transferKey) {
// 		if (m_bError)
// 		{
// 			return;
// 		}
// 
// 
// 		if (ENUM_XTSTREAM_TRANSKEY_TIMER == transferKey)
// 		{
// 			TRANSMIN_PROC_HEAD__T stSend, stRecv;
// 			memset(&stSend, 0, sizeof(TRANSMIN_PROC_HEAD__T));
// 			memset(&stRecv, 0, sizeof(TRANSMIN_PROC_HEAD__T));
// 			stSend.cmd = _TRANSMIS_PROC_HEARBEAT;
// 			stSend.session = m_nSessionID;
// 			stSend.k = 0xada2;
// 			//这里仅仅使用心跳信息告诉父程序自己还活着，或者父程序允许自己活着。使用传递过来的session id标识
// 			if (0 != m_TcpStream->SyncWriteAndRead(&stSend, sizeof(TRANSMIN_PROC_HEAD__T), &stRecv, sizeof(TRANSMIN_PROC_HEAD__T)))
// 			{
// 				OutputDebugStringA("CTransmisClient::OnPacketCompleteNetStreamData error\n");
// 				m_bError = TRUE;
// 			}
// 
// 
// 			m_pTimer->PostDelayTimer(1500);
// 		}
// 	};

public:
	int Start(string _devuuid) {
		
		memset(m_szDeviceUUID, 0, sizeof(m_szDeviceUUID));
		strcpy_s(m_szDeviceUUID, _devuuid.c_str());
		if (CROSS_THREAD_NULL == m_hWorkerThread)
		{
			m_hEvtWorkerStop = FALSE;
			m_hWorkerThread = CrossCreateThread(WorkerThread, this);
		}

// 		if (m_TcpStream)
// 		{
// 			m_TcpStream->Release();
// 			m_TcpStream = NULL;
// 		}

		return 0;
	};
	int Stop() {
// 		if (m_pTimer)
// 		{
// 			m_pTimer->Release();
// 			m_pTimer = NULL;
// 		}


		if (CROSS_THREAD_NULL != m_hWorkerThread)
		{
			m_hEvtWorkerStop = TRUE;
			CrossWaitThread(m_hWorkerThread);
			CrossCloseThread(m_hWorkerThread);
			m_hWorkerThread = CROSS_THREAD_NULL;
		}

		if (m_TcpStream)
		{
			m_TcpStream->Release();
			m_TcpStream = NULL;
		}

		return 0;
	};
	BOOL GetError()
	{ 
		return m_bError; 
	};

private:
	static int	WorkerThread(void* param)
	{
		CTransmisClient *pService = (CTransmisClient*)param;
		pService->Worker();
		return 0;
	};
	void Worker()
	{

		m_TcpStream = XNetCreateStream4Connect("127.0.0.1", 15432, 4);
		if (0 != XNetConnectStream(m_TcpStream))
		{
			m_bError = TRUE;
			return;
		};

		while (!m_hEvtWorkerStop)
		{

			TRANSMIN_PROC_DATA stSend, stRecv;
			memset(&stSend, 0, sizeof(TRANSMIN_PROC_DATA));
			memset(&stRecv, 0, sizeof(TRANSMIN_PROC_DATA));
			stSend.cmd = 0x3344;
			stSend.k = 0xada2;
			strcpy_s(stSend.uuid, m_szDeviceUUID);
			//这里仅仅使用心跳信息告诉父程序自己还活着，或者父程序允许自己活着
			if (0 != m_TcpStream->SyncWriteAndRead(&stSend, sizeof(TRANSMIN_PROC_DATA), &stRecv, sizeof(TRANSMIN_PROC_DATA)))
			{
				OutputDebugStringA("CTransmisClient::OnPacketCompleteNetStreamData error\n");
				m_bError = TRUE;
			}

			//
			if (m_bError)
			{
				break;
			}
			//
			for (int i = 0 ; i < 100 ; i++)
			{
				Sleep(40);
				if (m_hEvtWorkerStop)
				{
					break;
				}
			}
		}
	}

private:
	char m_szDeviceUUID[32];
	CXNetStream * m_TcpStream;
	BOOL m_bError;
	CROSS_THREAD_HANDLE	m_hWorkerThread;
	BOOL m_hEvtWorkerStop;
};
