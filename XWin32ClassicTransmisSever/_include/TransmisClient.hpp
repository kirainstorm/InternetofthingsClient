#pragma once

#include "PlatformDefine.h"
#include "TransmisDefine.h"

class CTransmisClient
{
private:
	CTransmisClient(void) 
		:socket_(io_service_, boost::asio::ip::udp::endpoint(boost::asio::ip::udp::v4(), 0))
	{
		m_bError = FALSE;
		m_hWorkerThread = CROSS_THREAD_NULL;
		m_dwTick = GetTickCount64();
	};
	~CTransmisClient(void) {
	};

public:
	static CTransmisClient& Instance() {
		static CTransmisClient ins;
		return ins;
	};
public:
	int Start(string _devuuid)
	{
		memset(m_szDeviceUUID, 0, sizeof(m_szDeviceUUID));
		strcpy_s(m_szDeviceUUID, _devuuid.c_str());
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
		if ((GetTickCount64() - m_dwTick) > 10000)
		{
			m_bError = TRUE;
		}
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
		//
		//
		TRANSMIN_PROC_DATA stSend, stRecv;
		memset(&stSend, 0, sizeof(TRANSMIN_PROC_DATA));
		memset(&stRecv, 0, sizeof(TRANSMIN_PROC_DATA));
		stSend.cmd = 0x3344;
		stSend.k = 0xada2;
		strcpy_s(stSend.uuid, m_szDeviceUUID);
		//
		//
		//
		try
		{
			boost::asio::ip::udp::resolver resolver(io_service_);
			boost::asio::ip::udp::resolver::query query(boost::asio::ip::udp::v4(), "127.0.0.1", "11100");
			boost::asio::ip::udp::resolver::iterator iterator = resolver.resolve(query);
			boost::asio::ip::udp::endpoint sender_endpoint;
			//
			//
			while (!m_hEvtWorkerStop)
			{


				try
				{
					//这里仅仅使用心跳信息告诉父程序自己还活着，或者父程序允许自己活着
					socket_.send_to(boost::asio::buffer(&stSend, sizeof(TRANSMIN_PROC_DATA)), *iterator);
					//
					size_t reply_length = socket_.receive_from(boost::asio::buffer(&stRecv, sizeof(TRANSMIN_PROC_DATA)), sender_endpoint);
					m_dwTick = GetTickCount64();
				}
				catch (boost::system::error_code& e) {
					m_bError = TRUE;
				}
				catch (boost::exception &e) {
					m_bError = TRUE;
				}
				catch (std::exception &e) {
					m_bError = TRUE;
				}
				catch (...) {
					m_bError = TRUE;
				}
				//
				if (m_bError)
				{
					break;
				}
				//
				for (int i = 0; i < 100; i++)
				{
					Sleep(40);
					if (m_hEvtWorkerStop)
					{
						break;
					}
				}
			}
			//
		}
		catch (boost::system::error_code& e) {
			return;
		}
		catch (boost::exception &e) {
			return;
		}
		catch (std::exception &e) {
			return;
		}
		catch (...) {
			return;
		}
	}

private:
	//
	boost::asio::io_service io_service_;
	boost::asio::ip::udp::socket socket_;

	//
	char m_szDeviceUUID[1024];
	BOOL m_bError;
	CROSS_THREAD_HANDLE	m_hWorkerThread;
	BOOL m_hEvtWorkerStop;
	//
	DWORD64 m_dwTick;
};
