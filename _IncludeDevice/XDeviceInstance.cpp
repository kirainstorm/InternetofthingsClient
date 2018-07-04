#include "stdafx.h"
#include "XDeviceInstance.h"
#if XINTERNET_TEST_STATUS
extern HANDLE g_hStartDoing; //手动重置,初始无信号
#endif
CXDeviceInstance::CXDeviceInstance()
{
	m_bTheadExit = FALSE;
	m_hThread = CROSS_THREAD_NULL;
	m_pStream = NULL;
	m_pSignaling = NULL;
}

CXDeviceInstance::~CXDeviceInstance()
{
}

void CXDeviceInstance::XDelete()
{
	if (m_pSignaling)
	{
		m_pSignaling->Disconnect();
		delete m_pSignaling;
		m_pSignaling = NULL;
	}

	if (m_pStream)
	{
		m_pStream->Disconnect();
		delete m_pStream;
		m_pStream = NULL;
	}
}
int CXDeviceInstance::XConnect(XDeviceMessageCallback *callback, char * s_ip, char * uuid)
{
	m_pMessageCallback = callback;

	if (CROSS_THREAD_NULL == m_hThread)
	{
		memset(m_szsip, 0, sizeof(m_szsip));
		memset(m_szuuid, 0, sizeof(m_szuuid));
		memset(m_szusername, 0, sizeof(m_szusername));
		//
		memcpy(m_szsip, s_ip, strlen(s_ip));
		memcpy(m_szuuid, uuid, strlen(uuid));
		//memcpy(m_szusername, username, strlen(username));
		s_nsport = 6599;
		//
		m_pSignaling = new CXDeviceSignaling(m_pMessageCallback);
		assert(m_pSignaling);
		m_pStream = new CXDeviceStream();
		assert(m_pStream);
		m_bTheadExit = FALSE;
		m_hThread = CrossCreateThread(ThreadWorker, this);
	}
	return 0;
}
int CXDeviceInstance::XDisconnect()
{

	if (CROSS_THREAD_NULL != m_hThread)
	{
		m_bTheadExit = TRUE;
		CrossWaitThread(m_hThread);
		CrossCloseThread(m_hThread);
		m_hThread = CROSS_THREAD_NULL;
		//
		m_pSignaling->Disconnect();
		delete m_pSignaling;
		m_pStream->Disconnect();
		delete m_pStream;
	}

	return 0;
}
int CXDeviceInstance::XSendMeaasge()
{
	return 0;
}
int CXDeviceInstance::XSendStream()
{
	return 0;
}


int CXDeviceInstance::ThreadWorker(void * param)
{
	CXDeviceInstance * p = (CXDeviceInstance *)param;
	p->Woker();
	return 0;
}
void CXDeviceInstance::Woker()
{
#if XINTERNET_TEST_STATUS
	WaitForSingleObject(g_hStartDoing, INFINITE);
#endif

	while (FALSE == m_bTheadExit)
	{
		if (m_pSignaling->IsError() && m_pStream->IsError())
		{
			//
			string s1 = m_szuuid;
			if (____XDeviceInterfaceIsClassicID(s1))
			{
				memset(m_sziotip, 0, sizeof(m_sziotip));
				CROSS_STRCPY(m_sziotip, m_szsip);
			}
			else
			{
				GetIotServerInfo();
			}

		}

		if (m_pSignaling->IsError())
		{

			//connect
			m_pSignaling->Disconnect();
			m_pSignaling->Connect(m_sziotip, m_szuuid, m_szusername);
		}
		else
		{
			//心跳
			m_pSignaling->AddSendMessage(SINGNALING_CHANNEL_CMD_HEARBEAT, nullptr, 0);
			char szttmp[64] = { 0 };
			sprintf(szttmp, "%s send: hello world (alarm)!", m_szuuid);
			for (int i = 0; i < 3; i++)
			{
				m_pSignaling->AddSendMessage(SINGNALING_CHANNEL_CMD_ALARM, szttmp, strlen(szttmp));
			}
		}


// 
// 
// 		if (m_pStream->IsError())
// 		{
// 			//connect
// 		}
// 		else
// 		{
// 			//心跳
// 		}

		for (int i = 0; i< 100; i ++)
		{
			CrossSleep(50);
			if (m_bTheadExit)
			{
				break;
			}
		}
	}
}


void CXDeviceInstance::GetIotServerInfo()
{

#if XINTERNET_TEST_STATUS
	memset(m_sziotip, 0, sizeof(m_sziotip));
	memset(m_szusername, 0, sizeof(m_szusername));
	//CROSS_STRCPY(m_sziotip, "127.0.0.1");
	CROSS_STRCPY(m_sziotip, "47.96.249.142");
	//CROSS_STRCPY(m_sziotip, "52.175.25.247");
	CROSS_STRCPY(m_szusername, "11@qq.com");
	return;
#endif
	//

	ST_DEVICE_SIGNALING_SEND_BUFFER *pSendBuffer = CXDeviceSignalingBufferPool::Instance().malloc();
	login_redirection_t rt;
	signaling_channel_head_t msg_recv;
	char szBuffer[256] = {};
	CXNetStream* pTcpStream = NULL;

	//m_cs.Lock();
	do
	{
		pTcpStream = XNetCreateStream4Connect(m_szsip, s_nsport, 3);
		if (NULL == pTcpStream)
		{
			CROSS_TRACE(">>>GetIOTServerIP Error<<< 1");
			break;
		}
		if (0 != XNetConnectStream(pTcpStream))
		{
			CROSS_TRACE(">>>GetIOTServerIP Error<<< 2");
			break;
		}
		//
		memset(pSendBuffer, 0, sizeof(ST_DEVICE_SIGNALING_SEND_BUFFER));
		memset(&rt, 0, sizeof(rt));
		pSendBuffer->head.cmd = SINGNALING_CHANNEL_CMD_REDIRECTION_DEVICE;
		pSendBuffer->head.datalen = sizeof(rt);
		CROSS_STRCPY(rt.id, m_szuuid);
		memcpy(pSendBuffer->msg, &rt, sizeof(rt));
		//
		if (0 != pTcpStream->SyncWriteAndRead(pSendBuffer, sizeof(signaling_channel_head_t) + pSendBuffer->head.datalen, &msg_recv, sizeof(signaling_channel_head_t)))
		{
			CROSS_TRACE(">>>GetIOTServerIP Error<<< 3");
			break;
		}
		if (msg_recv.cmd != SINGNALING_CHANNEL_CMD_REDIRECTION_DEVICE)
		{
			CROSS_TRACE(">>>GetIOTServerIP Error<<< 4");
			break;
		}
		if (msg_recv.datalen == 0)
		{
			CROSS_TRACE(">>>GetIOTServerIP Error<<< 5");
			break;
		}
		if (0 != pTcpStream->SyncWriteAndRead(NULL, 0, szBuffer, msg_recv.datalen))
		{
			CROSS_TRACE(">>>GetIOTServerIP Error<<< 10");
			break;
		}
		//
		string s = szBuffer;
		vector<string> v;
		_SplitString(s, v, "|");
		//
		CROSS_STRCPY(m_sziotip, v[0].c_str());
		CROSS_STRCPY(m_szusername, v[1].c_str());
		//
		CROSS_TRACE(">>>GetIOTServerIP<<< okokok");
	} while (FALSE);
	//
	if (pTcpStream)
	{
		pTcpStream->Release();
		pTcpStream = NULL;
	}
	//
	//m_cs.Unlock();
	//return (0 == ret) ? m_szIOTServerIp : nullptr;

}