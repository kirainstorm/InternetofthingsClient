#pragma once

#include "PlatformDefine.h"
#include "TransmisDefine.h"
#include "DatabaseForClassicTransmis.h"




class CMsgTransmisRtmpManagement
{
public:
	CMsgTransmisRtmpManagement()
	{
		m_hThread = NULL;
		m_hEvtWorkerStop = FALSE;
	};
	~CMsgTransmisRtmpManagement(){};
public:
	static CMsgTransmisRtmpManagement& Instance()
	{
		static CMsgTransmisRtmpManagement ins;
		return ins;
	};

public:
	void UpdataProcess(vector<st_database_Transmis_stream_rtmp *> v)
	{
		if (v.empty() || v.size() <= 0)
		{
			return;
		}


		m_cs.Lock();

		//add
		for (vector<st_database_Transmis_stream_rtmp *>::iterator it_add = v.begin(); it_add != v.end(); it_add++)
		{
			BOOL bFind = FALSE;
			string url = (*it_add)->url;
			for (map<string, ST_PROCESS_INFO*>::iterator it_map_add = m_mapRtspProcess.begin(); it_map_add != m_mapRtspProcess.end(); it_map_add++)
			{
				if (0 == strcmp(it_map_add->first.c_str(), (*it_add)->url))
				{
					bFind = TRUE;
					break;
				}
			}

			if (!bFind)
			{
				ST_PROCESS_INFO * pInfo = malloc_transmis_process_info();
				CROSS_STRCPY(pInfo->strParam, (*it_add)->url);
				m_mapRtspProcess[url] = pInfo;
			}
		}





		//remove 
		for (map<string, ST_PROCESS_INFO*>::iterator it_map_remove = m_mapRtspProcess.begin(); it_map_remove != m_mapRtspProcess.end(); it_map_remove++)
		{
			BOOL bFind = FALSE;
			string url = it_map_remove->first.c_str();
			for (vector<st_database_Transmis_stream_rtmp *>::iterator it_remove = v.begin(); it_remove != v.end(); it_remove++)
			{
				if (0 == strcmp(it_map_remove->first.c_str(), (*it_remove)->url))
				{
					bFind = TRUE;
					break;
				}
			}

			if (!bFind)
			{
				map<string, ST_PROCESS_INFO*>::iterator _remove = m_mapRtspProcess.find(url);
				CloseTransmisProcProcessRtmp(*_remove->second);
				free_transmis_process_info(_remove->second);
				m_mapRtspProcess.erase(_remove);
			}
		}
		//

		m_cs.Unlock();
	}



	
	void UpdataTick(char * devuuid)
	{
		m_cs.Lock();
		string strUuid(devuuid);
		map<string, ST_PROCESS_INFO*>::iterator it = m_mapRtspProcess.find(strUuid);
		if (it != m_mapRtspProcess.end())
		{
			it->second->dwTick = CrossGetTickCount64();
		}
		m_cs.Unlock();
	};;//告诉CTransmisRtmpManagement当前uuid正在推流
	//
	void Start()
	{
		m_hEvtWorkerStop = FALSE;
		m_hThread = ::CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)WorkerThread, this, 0, NULL);
		assert(m_hThread);
	};
	void Stop()
	{
		m_hEvtWorkerStop = TRUE;
		WaitForSingleObject(m_hThread, INFINITE);
		CloseHandle(m_hThread);
		m_hThread = NULL;
		//
		for (map<string, ST_PROCESS_INFO*>::iterator it = m_mapRtspProcess.begin(); it != m_mapRtspProcess.end(); it++)
		{
			free_transmis_process_info(it->second);
		}
		m_mapRtspProcess.clear();
	};

private:
	static void* WINAPI WorkerThread(void* param)
	{
		CMsgTransmisRtmpManagement *pService = (CMsgTransmisRtmpManagement*)param;
		pService->Worker();
		return NULL;
	};;
	void Worker()
	{
		while (!m_hEvtWorkerStop)
		{
			m_cs.Lock();
			for (map<string, ST_PROCESS_INFO*>::iterator it = m_mapRtspProcess.begin(); it != m_mapRtspProcess.end(); it++)
			{
				if(NULL == it->second->pi.hProcess)
				{
					if (0 == OpenTransmisProcProcessRtmp((*it->second), L"XWin32RtspTransmisProcess.exe"))//打开进程
					{
						it->second->dwTick = CrossGetTickCount64();
						//printf("OpenTransmisProcProcess %s\n", it->second->strParam);
					}
					else
					{
						it->second->pi.hProcess = NULL;
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
// 	void AddRtmp(const char * user, const char * pwd, vector<string> devuuid_list_new, vector<string> &rtmpuuid_list)
// 	{
// 
// 		m_cs.Lock();
// 		for (vector<string>::iterator it = devuuid_list_new.begin(); it != devuuid_list_new.end(); it++)
// 		{
// 			map<string, ST_PROCESS_INFO*>::iterator it_process = m_mapRtspProcess.find(*it);
// 			if (it_process != m_mapRtspProcess.end())
// 			{
// 				if (0 > it_process->second->openCount)
// 				{
// 					assert(0);//不可能到这里，到这里就错了
// 				}
// 				else if ((0 == it_process->second->openCount) && (NULL == it_process->second->pi.hProcess))
// 				{
// 					it_process->second->openCount++;
// 					//
// 					memset(it_process->second->rtmpUuid, 0, sizeof(it_process->second->rtmpUuid));
// 					_newUuid(it_process->second->rtmpUuid);
// 					rtmpuuid_list.push_back(it_process->second->rtmpUuid);
// 
// 					//memcpy(rtmpuuid, it->second->rtmpUuid, sizeof(it->second->rtmpUuid));
// 					memset(it_process->second->strParam, 0, sizeof(it_process->second->strParam));
// 					sprintf_s(it_process->second->strParam, "%s#%s#%s#%s", (*it).c_str(), it_process->second->rtmpUuid, user, pwd);
// 				}
// 				else
// 				{
// 					it_process->second->openCount++;
// 					rtmpuuid_list.push_back(it_process->second->rtmpUuid);
// 				}
// 
// 			}
// 			else
// 			{
// 				ST_PROCESS_INFO* p = malloc_transmis_process_info();
// 				assert(p);
// 				memset(p, 0, sizeof(ST_PROCESS_INFO));
// 				//
// 				p->openCount = 1;
// 				_newUuid(p->rtmpUuid);
// 				rtmpuuid_list.push_back(p->rtmpUuid);
// 				//
// 				sprintf_s(p->strParam, "%s#%s#%s#%s", (*it).c_str(), p->rtmpUuid, user, pwd);
// 				//
// 				m_mapRtspProcess[*it] = p;
// 			}
// 		}
// 
// 		m_cs.Unlock();
// 
// 
// 	};
// 	void RemoveRtmp(vector<string> devuuid_list)
// 	{
// 		m_cs.Lock();
// 		for (vector<string>::iterator it = devuuid_list.begin(); it != devuuid_list.end(); it++)
// 		{
// 			map<string, ST_PROCESS_INFO*>::iterator it_process = m_mapRtspProcess.find(*it);
// 			if (it_process != m_mapRtspProcess.end())
// 			{
// 				it_process->second->openCount--;
// 				if (0 == it_process->second->openCount)
// 				{
// 					it_process->second->openCountZeroTick = CrossGetTickCount64();
// 				}
// 			}
// 			else
// 			{
// 				//不可能到这里，到这里就错了
// 				assert(0);
// 			}
// 		}
// 		m_cs.Unlock();
// 	};
private:
	HANDLE m_hThread;
	volatile BOOL m_hEvtWorkerStop;
	//
	CrossCriticalSection m_cs;
	map<string/*rtsp url*/, ST_PROCESS_INFO*> m_mapRtspProcess;
};





class CTransmisServer
{
public:
	CTransmisServer(int port)
		:socket_(io_service_, boost::asio::ip::udp::endpoint(boost::asio::ip::udp::v4(), port))
	{
		m_bError = FALSE;
		m_hWorkerThread = CROSS_THREAD_NULL;
	};
	~CTransmisServer(void)
	{
	};
public:
	int Start()
	{
		if (CROSS_THREAD_NULL == m_hWorkerThread)
		{
			m_hEvtWorkerStop = FALSE;
			m_hWorkerThread = CrossCreateThread(WorkerThread, this);
		}
		return 0;
	};
	int Stop()
	{
		if (CROSS_THREAD_NULL != m_hWorkerThread)
		{
			try
			{
				if (socket_.is_open())
				{
					socket_.cancel();
					boost::system::error_code ignored_ec;
					socket_.shutdown(boost::asio::ip::tcp::socket::shutdown_both, ignored_ec);
					socket_.close(ignored_ec);
				}
			}
			catch (boost::system::error_code& e) {
				//trace_catch_error_code(__FILE__, __LINE__, __FUNCTION__, e.message().c_str());
			}
			catch (boost::exception &e) {
				//trace_catch_error_code(__FILE__, __LINE__, __FUNCTION__, boost::diagnostic_information(e).c_str());
				//trace_catch_error_code(__FILE__, __LINE__, __FUNCTION__, boost::current_exception_diagnostic_information().c_str());
			}
			catch (std::exception &e) {
				//trace_catch_error_code(__FILE__, __LINE__, __FUNCTION__, e.what());
			}
			catch (...) {
				//trace_catch_error_code(__FILE__, __LINE__, __FUNCTION__, "(catch (...))");
			}
			m_hEvtWorkerStop = TRUE;
			CrossWaitThread(m_hWorkerThread);
			CrossCloseThread(m_hWorkerThread);
			m_hWorkerThread = CROSS_THREAD_NULL;
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
		CTransmisServer *pService = (CTransmisServer*)param;
		pService->Worker();
		return 0;
	};
	void Worker()
	{
		while (!m_hEvtWorkerStop)
		{
			char datarecv[max_asio_udp_length_rs] = { 0 };
			//char datasend[max_asio_udp_length_rs] = { 0 };
			boost::asio::ip::udp::endpoint sender_endpoint;
			//
			//

			try
			{
				size_t length = socket_.receive_from(boost::asio::buffer(datarecv, max_asio_udp_length_rs), sender_endpoint);
				//---------------------------------------------------------------------------------------------------------
				TRANSMIN_PROC_DATA * p = (TRANSMIN_PROC_DATA *)datarecv;
				if ((p->cmd == 0x3344) && (p->k == 0xada2))
				{
					CMsgTransmisRtmpManagement::Instance().UpdataTick(p->uuid);
					socket_.send_to(boost::asio::buffer(datarecv, length), sender_endpoint);
				}
				//---------------------------------------------------------------------------------------------------------
			}
			catch (boost::system::error_code& e) {
				assert(0);
			}
			catch (boost::exception &e) {
				assert(0);
			}
			catch (std::exception &e) {
				assert(0);
			}
			catch (...) {
				assert(0);
			}
		}
	}

private:
	//
	BOOL m_bError;
	int m_nPort;
	boost::asio::io_service io_service_;
	boost::asio::ip::udp::socket socket_;
	//
	CROSS_THREAD_HANDLE	m_hWorkerThread;
	BOOL m_hEvtWorkerStop;
};