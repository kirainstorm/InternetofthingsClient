#include "stdafx.h"
#include "XMessageTransmisRtmp.h"


//启动进程“推送rtmp流”

extern volatile BOOL g_bServiceExit;
//extern void __TransmisUpdataTick(char * devuuid);


CMsgTransmisAccepter::CMsgTransmisAccepter(CXNetStream * pAcceptStream)
{
	CRecycleService::Instance().AddRef();
	m_pTcpStream = pAcceptStream;
	m_bNetIsError = FALSE;
	m_bCheckError = FALSE;
	m_nRcvStep = NET_RECIEVE_STEP_HEAD;
	m_dwTick = CrossGetTickCount64();
};
CMsgTransmisAccepter::~CMsgTransmisAccepter(void)
{
};
void CMsgTransmisAccepter::SessionDelete()
{
	if (m_pTcpStream)
	{
		m_pTcpStream->Release();
		m_pTcpStream = NULL;
	}
	delete this;
};


void CMsgTransmisAccepter::OnPacketCompleteNetStreamData(int32_t bytesTransferred, emXStreamTansferKey transferKey)
{
	if (m_bCheckError)
	{
		return;
	}
	if (ENUM_XTSTREAM_TRANSKEY_TIMER == transferKey)
	{
		if (g_bServiceExit || (CrossGetTickCount64() - m_dwTick) > 7 * 1000)//与用户的链接/超时
		{
			CRecycleService::Instance().AddToReleaseList(this);//移动自己到待删除链表
			m_bCheckError = TRUE;
		}
		else
		{
			m_pTcpStream->PostDelayTimer();
		}
		return;
	}
	if (m_bNetIsError)
	{
		return;
	}
	if (bytesTransferred < 0)
	{
		m_bNetIsError = TRUE;
		return;
	}
	if (ENUM_XTSTREAM_TRANSKEY_READ == transferKey)
	{
		if (0 == bytesTransferred)
		{
			m_nRcvStep = NET_RECIEVE_STEP_HEAD;
			m_pTcpStream->AsyncRead(&m_headRecv, sizeof(m_headRecv));
			return;
		}
		else
		{
			if (m_nRcvStep == NET_RECIEVE_STEP_HEAD)
			{
				DoMsg();
				m_nRcvStep = NET_RECIEVE_STEP_HEAD;
				m_pTcpStream->AsyncRead(&m_headRecv, sizeof(m_headRecv));
				return;
			}
		}
	}
}

void CMsgTransmisAccepter::DoMsg()
{
	if (m_headRecv.cmd == 0x3344)
	{
		m_dwTick = CrossGetTickCount64();
		if (m_headRecv.k != 0xada2)
		{
			m_bNetIsError = TRUE;
		}
		else
		{
			//__TransmisUpdataTick(m_headRecv.uuid);
			CMsgTransmisRtmpManagement::Instance().UpdataTick(m_headRecv.uuid);
			memcpy(&m_headSend, &m_headRecv, sizeof(TRANSMIN_PROC_DATA));
			m_pTcpStream->AsyncWrite(&m_headSend, sizeof(m_headSend));
		}
	}
};




CMsgTransmisRtmpManagement::CMsgTransmisRtmpManagement() {
	m_hServer = NULL;
	m_hThread = NULL;
	m_hEvtWorkerStop = FALSE;
};
CMsgTransmisRtmpManagement::~CMsgTransmisRtmpManagement() {
	Stop();
};

CMsgTransmisRtmpManagement& CMsgTransmisRtmpManagement::Instance()
{
	static CMsgTransmisRtmpManagement ins;
	return ins;
};


void CMsgTransmisRtmpManagement::OnAcceptComplete(CXNetStream * pAcceptStream)
{
	if (g_bServiceExit)
	{
		pAcceptStream->Release();
		return;
	}
	CMsgTransmisAccepter * pSession = new CMsgTransmisAccepter(pAcceptStream);
	pAcceptStream->SetStreamData(pSession);
};

void CMsgTransmisRtmpManagement::AddRtmp(const char * user, const char * pwd,vector<string> devuuid_list_new, vector<string> &rtmpuuid_list)
{
	m_cs.Lock();
	for (vector<string>::iterator it = devuuid_list_new.begin(); it != devuuid_list_new.end(); it++)
	{
		map<string, ST_PROCESS_INFO*>::iterator it_process = g_mapProcess.find(*it);
		if (it_process != g_mapProcess.end())
		{
			if (0 > it_process->second->openCount)
			{
				assert(0);//不可能到这里，到这里就错了
			}
			else if ((0 == it_process->second->openCount)&&(NULL == it_process->second->pi.hProcess))
			{
				it_process->second->openCount++;
				//
				memset(it_process->second->rtmpUuid, 0, sizeof(it_process->second->rtmpUuid));
				_newUuid(it_process->second->rtmpUuid);
				rtmpuuid_list.push_back(it_process->second->rtmpUuid);

				//memcpy(rtmpuuid, it->second->rtmpUuid, sizeof(it->second->rtmpUuid));
				memset(it_process->second->strParam, 0, sizeof(it_process->second->strParam));
				sprintf_s(it_process->second->strParam, "%s#%s#%s#%s", (*it).c_str(), it_process->second->rtmpUuid, user, pwd);
			}
			else
			{
				it_process->second->openCount++;
				rtmpuuid_list.push_back(it_process->second->rtmpUuid);
			}

		}
		else
		{
			ST_PROCESS_INFO* p = singleton_pool_malloc_transmis_process_info();
			assert(p);
			memset(p, 0, sizeof(ST_PROCESS_INFO));
			//
			p->openCount = 1;
			_newUuid(p->rtmpUuid);
			rtmpuuid_list.push_back(p->rtmpUuid);
			//
			sprintf_s(p->strParam, "%s#%s#%s#%s", (*it).c_str(), p->rtmpUuid, user, pwd);
			//
			g_mapProcess[*it] = p;
		}
	}

	m_cs.Unlock();

}


void CMsgTransmisRtmpManagement::RemoveRtmp(vector<string> devuuid_list)
{
	m_cs.Lock();
	for (vector<string>::iterator it = devuuid_list.begin(); it != devuuid_list.end(); it++)
	{
		map<string, ST_PROCESS_INFO*>::iterator it_process = g_mapProcess.find(*it);
		if (it_process != g_mapProcess.end())
		{
			it_process->second->openCount--;
			if (0 == it_process->second->openCount)
			{
				it_process->second->openCountZeroTick = CrossGetTickCount64();
			}
		}
		else
		{
			//不可能到这里，到这里就错了
			assert(0);
		}
	}
	m_cs.Unlock();
}

void CMsgTransmisRtmpManagement::UpdataTick(char * devuuid)//告诉CTransmisRtmpManagement当前uuid正在推流
{
	m_cs.Lock();
	string strUuid(devuuid);
	map<string, ST_PROCESS_INFO*>::iterator it = g_mapProcess.find(strUuid);
	if (it != g_mapProcess.end())
	{
		it->second->dwTick = CrossGetTickCount64();
	}
	m_cs.Unlock();
};

void CMsgTransmisRtmpManagement::Start(int port)
{
	if (m_hServer == NULL)
	{
		m_hServer = new CXTcpListener(this, port);
		assert(m_hServer);
		m_hServer->Start();
		//
		m_hEvtWorkerStop = FALSE;
		m_hThread = ::CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)WorkerThread, this, 0, NULL);
		assert(m_hThread);
	}
};
void CMsgTransmisRtmpManagement::Stop()
{
	if (m_hServer)
	{
		m_hEvtWorkerStop = TRUE;
		WaitForSingleObject(m_hThread, INFINITE);
		CloseHandle(m_hThread);
		m_hThread = NULL;
		//
		m_hServer->Stop();//自删除
		m_hServer = nullptr;
		//
		for (map<string, ST_PROCESS_INFO*>::iterator it = g_mapProcess.begin(); it != g_mapProcess.end(); it++)
		{
			singleton_pool_free_transmis_process_info(it->second);
		}
		g_mapProcess.clear();
	}
};


void* WINAPI CMsgTransmisRtmpManagement::WorkerThread(void* param)
{
	CMsgTransmisRtmpManagement *pService = (CMsgTransmisRtmpManagement*)param;
	pService->Worker();
	return NULL;
};
void CMsgTransmisRtmpManagement::Worker()
{
	while (!m_hEvtWorkerStop)
	{
		m_cs.Lock();
		for (map<string, ST_PROCESS_INFO*>::iterator it = g_mapProcess.begin(); it != g_mapProcess.end(); it++)
		{
			if ((NULL == it->second->pi.hProcess) && (it->second->openCount > 0))
			{
				if (0 == OpenTransmisProcProcessRtmp((*it->second), L"XRtmpTransmisProc.exe"))//打开进程
				{
					it->second->dwTick = CrossGetTickCount64();
					//printf("OpenTransmisProcProcess %s\n", it->second->strParam);
				}
				else
				{
					it->second->pi.hProcess = NULL;
				}
			}
			else if ((NULL != it->second->pi.hProcess) && (it->second->openCount == 0))
			{
				if ((CrossGetTickCount64() - it->second->openCountZeroTick) > (15 * 1000))
				{
					CloseTransmisProcProcessRtmp(*it->second);
				}
			}
			else
			{
				if ((CrossGetTickCount64() - it->second->dwTick) > (15 * 1000))
				{
					CloseTransmisProcProcessRtmp(*it->second);
				}
			}
		}
		m_cs.Unlock();
		//
		for (int i = 0; i < 100; i++)
		{
			Sleep(10);
			if (m_hEvtWorkerStop)
			{
				break;
			}
		}
	}
};

// 
// void __TransmisUpdataTick(char * devuuid)
// {
// 	CTransmisRtmpManagement::Instance().UpdataTick(devuuid);
// }
