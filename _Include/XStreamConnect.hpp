#pragma once
#include "PlatformDefine.h"
#include "XStreamInterface.h"
#include "XStreamManager.hpp"

//----------------------------------------
// kirainstorm
// https://github.com/kirainstorm
//----------------------------------------

//Ϊʲô����Ƶ������ʱ�������´���
//Microsoft C++ exception: boost::exception_detail::clone_impl<boost::exception_detail::error_info_injector<boost::system::system_error> > 
//ԭ����connet/read/write�ĺ�����û��ʹ��boost::system::error_code e;�����˾�û����

class CXNetStreamConnect : public CXNetStream
{
public:
	CXNetStreamConnect(string szServerAddress, string lPort, DWORD dwWaitSecond = 10)
		: strand_(CXStreamManager::Instance().io_service_),
		socket_(CXStreamManager::Instance().io_service_),
		resolver(CXStreamManager::Instance().io_service_),
		query(szServerAddress.c_str(), lPort.c_str()),
		timer_(CXStreamManager::Instance().io_service_),
		timer_sync_write_read(CXStreamManager::Instance().io_service_),
		timer_connect(CXStreamManager::Instance().io_service_),
		timer_ontimer(CXStreamManager::Instance().io_service_),
		m_lRef(1)
	{
		m_nAsyncWriteDatBufferLen = 4096;
		m_AsyncWriteData = new char[4096];
		m_pStreamData = nullptr;
		m_strServerAddress = szServerAddress;
		m_nPort = lPort;
		m_dwWaitSecond = dwWaitSecond;
	};
	~CXNetStreamConnect()
	{
		//Self-delete, don't deal with anything here
		//trace_catch_error_code(__FILE__, __LINE__, __FUNCTION__,"#delete# CConnectStream::~CConnectStream() \n");
	};

public:
	virtual void Release()
	{
		try
		{
			m_cs.Lock();
			//
			Shutdown();
			//
			m_pStreamData = NULL;
			//
			m_cs.Unlock();

		}
		catch (...)
		{
			trace_catch_error_code(__FILE__, __LINE__, __FUNCTION__,"CConnectStream::Release! error");
		}

		//
		ReleaseRef();

	};
	virtual void SetStreamData(CXNetStreamData* pStreamData)
	{
		m_cs.Lock();
		if (NULL == m_pStreamData)
		{
			m_pStreamData = pStreamData;
			m_pStreamData->OnPacketCompleteNetStreamData(0, ENUM_XTSTREAM_TRANSKEY_READ);//callback read
			m_pStreamData->OnPacketCompleteNetStreamData(0, ENUM_XTSTREAM_TRANSKEY_WRITE);//callback send
			m_pStreamData->OnPacketCompleteNetStreamData(0, ENUM_XTSTREAM_TRANSKEY_TIMER);
		}
		m_cs.Unlock();
	};
	virtual void AsyncRead(void *data, int32_t dataLen)
	{
		try
		{
			AddRef();
			boost::asio::async_read(socket_, boost::asio::buffer((char *)data, dataLen),
				strand_.wrap(
				boost::bind(&CXNetStreamConnect::handle_read, this,
				boost::asio::placeholders::error,
				boost::asio::placeholders::bytes_transferred)));
		}
		catch (boost::system::error_code& e) {
			trace_catch_error_code(__FILE__, __LINE__, __FUNCTION__,e.message().c_str());
		}
		catch (boost::exception &e) {
			trace_catch_error_code(__FILE__, __LINE__, __FUNCTION__,boost::diagnostic_information(e).c_str());
			trace_catch_error_code(__FILE__, __LINE__, __FUNCTION__,boost::current_exception_diagnostic_information().c_str());
		}
		catch (std::exception &e) {
			trace_catch_error_code(__FILE__, __LINE__, __FUNCTION__,e.what());
		}
		catch (...) {
			trace_catch_error_code(__FILE__, __LINE__, __FUNCTION__,"(catch (...))");
		}
	};
	virtual int32_t SyncWriteAndRead(const void *writeData, int32_t wantWriteLen, void * readData, int32_t wantReadLen, int32_t waitsec = 3)
	{
		int ret = -1;
		int m = 0, n = 0;
		do
		{
			try
			{
				AddRef();
				m_bsync_write_read_complete = FALSE;
				timer_sync_write_read.expires_from_now(boost::posix_time::milliseconds(waitsec * 1000));
				timer_sync_write_read.async_wait(strand_.wrap(boost::bind(&CXNetStreamConnect::handle_sync_write_read, this, boost::asio::placeholders::error)));

#if 0
				//timer_sync_write_read.cancel(); ���첽ִ�еģ�boost::asio::error::operation_aborted
				CrossSleep(10);//only for test //��û�г�ʱ��cancel��ʱ�򣬶��cancelִֻ��һ��handle_sync_write_read
				//CrossSleep(5000);//only for test //��handle_sync_write_read���ص���ִ��cancel��Ч
				timer_sync_write_read.cancel();
				//trace_catch_error_code(__FILE__, __LINE__, __FUNCTION__,"CConnectStream::SyncWriteAndRead --##### 2 - 1");
				timer_sync_write_read.cancel();
				//trace_catch_error_code(__FILE__, __LINE__, __FUNCTION__,"CConnectStream::SyncWriteAndRead --##### 2 - 2");
				timer_sync_write_read.cancel();
				//trace_catch_error_code(__FILE__, __LINE__, __FUNCTION__,"CConnectStream::SyncWriteAndRead --##### 2 - 3");
				timer_sync_write_read.cancel();
				//trace_catch_error_code(__FILE__, __LINE__, __FUNCTION__,"CConnectStream::SyncWriteAndRead --##### 2 - 4");
				timer_sync_write_read.cancel();
				//trace_catch_error_code(__FILE__, __LINE__, __FUNCTION__,"CConnectStream::SyncWriteAndRead --##### 2 - 5");
				CrossSleep(10000000);
#endif

				boost::system::error_code e;

				if (wantWriteLen > 0)
				{
					m = (int32_t)boost::asio::write(socket_, boost::asio::buffer((char *)writeData, wantWriteLen), e);
				}
				if (e)
				{
					trace_catch_error_code(__FILE__, __LINE__, __FUNCTION__,e.message().c_str());
					ret = -1;
					break;
				}

				if (wantReadLen > 0)
				{
					n = (int32_t)boost::asio::read(socket_, boost::asio::buffer((char *)readData, wantReadLen), e);
				}

				if (e)
				{
					trace_catch_error_code(__FILE__, __LINE__, __FUNCTION__,e.message().c_str());
					ret = -1;
					break;
				}

				//
				ret = 0;
			}
			catch (boost::system::error_code& e) {
				ret = -1;
				trace_catch_error_code(__FILE__, __LINE__, __FUNCTION__,e.message().c_str());
			}
			catch (boost::exception &e) {
				ret = -1;
				trace_catch_error_code(__FILE__, __LINE__, __FUNCTION__,boost::diagnostic_information(e).c_str());
				trace_catch_error_code(__FILE__, __LINE__, __FUNCTION__,boost::current_exception_diagnostic_information().c_str());
			}
			catch (std::exception &e) {
				ret = -1;
				trace_catch_error_code(__FILE__, __LINE__, __FUNCTION__,e.what());
			}
			catch (...) {
				ret = -1;
				trace_catch_error_code(__FILE__, __LINE__, __FUNCTION__,"(catch (...))");
			}


		} while (0);


		timer_sync_write_read.cancel();

		do
		{
			CrossSleep(10);
		} while (!m_bsync_write_read_complete);

		return ret;

	};
	virtual void AsyncWrite(const void *data, int32_t dataLen)
	{
		try
		{
			AddRef();
			//resize len
			if (dataLen > m_nAsyncWriteDatBufferLen)
			{
				m_nAsyncWriteDatBufferLen = 4096 * ((dataLen / 4096) + 1);
				delete[] m_AsyncWriteData;
				m_AsyncWriteData = new char[m_nAsyncWriteDatBufferLen];
			}
			//copy data
			m_nAsyncWriteRealLen = dataLen;
			memset(m_AsyncWriteData, 0, m_nAsyncWriteDatBufferLen);
			memcpy(m_AsyncWriteData, (char *)data, dataLen);
			//
			//boost::asio::async_write(socket_, boost::asio::buffer((char *)data, datlen),
			boost::asio::async_write(socket_, boost::asio::buffer((char *)m_AsyncWriteData, m_nAsyncWriteRealLen),
				strand_.wrap(
				boost::bind(&CXNetStreamConnect::handle_write, this,
				boost::asio::placeholders::error,
				boost::asio::placeholders::bytes_transferred)));
		}
		catch (boost::system::error_code& e) {
			trace_catch_error_code(__FILE__, __LINE__, __FUNCTION__,e.message().c_str());
		}
		catch (boost::exception &e) {
			trace_catch_error_code(__FILE__, __LINE__, __FUNCTION__,boost::diagnostic_information(e).c_str());
			trace_catch_error_code(__FILE__, __LINE__, __FUNCTION__,boost::current_exception_diagnostic_information().c_str());
		}
		catch (std::exception &e) {
			trace_catch_error_code(__FILE__, __LINE__, __FUNCTION__,e.what());
		}
		catch (...) {
			trace_catch_error_code(__FILE__, __LINE__, __FUNCTION__,"(catch (...))");
		}

	}
	;
	virtual void PostDelayWriteStatus(int milliseconds)
	{
		AddRef();
		timer_.expires_from_now(boost::posix_time::milliseconds(milliseconds));//��ֵ̫С�������ã�����д20�����31ms��ص�
		timer_.async_wait(boost::bind(&CXNetStreamConnect::handle_delay_write, this));
	};
	virtual void PostDelayTimer(int32_t milliseconds)
	{
		AddRef();//�˴������������ڸú�����OnPacketCompleteNetStreamData�б����ã���OnPacketCompleteNetStreamData�Ѿ���ס��
		timer_ontimer.expires_from_now(boost::posix_time::milliseconds(milliseconds));
		timer_ontimer.async_wait(boost::bind(&CXNetStreamConnect::handle_delay_timer, this));
	};
public:
	int ConnectServer()
	{


		int ret = -1;

		do
		{
			//
			try
			{
				AddRef();
				m_bsync_connect_timer_complete = FALSE;
				//m_bsync_connect_timer_user_cancel = FALSE;
				timer_connect.expires_from_now(boost::posix_time::milliseconds((m_dwWaitSecond * 1000)));
				timer_connect.async_wait(boost::bind(&CXNetStreamConnect::handle_sync_connect, this, boost::asio::placeholders::error));

				iterator = resolver.resolve(query);
				boost::asio::ip::tcp::endpoint endpoint = *iterator;
				/*
				socket_.connect
				* The socket is automatically opened if it is not already open. If the
				* connect fails, and the socket was automatically opened, the socket is
				* not returned to the closed state.
				*/
				boost::system::error_code e;
				socket_.connect(endpoint, e);
				if (e)
				{
					trace_catch_error_code(__FILE__, __LINE__, __FUNCTION__,e.message().c_str());
					break;
				}
				ret = 0;
			}
			catch (boost::system::error_code& e) {
				trace_catch_error_code(__FILE__, __LINE__, __FUNCTION__,e.message().c_str());
			}
			catch (boost::exception &e) {
				trace_catch_error_code(__FILE__, __LINE__, __FUNCTION__,boost::diagnostic_information(e).c_str());
				trace_catch_error_code(__FILE__, __LINE__, __FUNCTION__,boost::current_exception_diagnostic_information().c_str());
			}
			catch (std::exception &e) {
				trace_catch_error_code(__FILE__, __LINE__, __FUNCTION__,e.what());
			}
			catch (...) {
				trace_catch_error_code(__FILE__, __LINE__, __FUNCTION__,"(catch (...))");
			}

		} while (0);


		timer_connect.cancel();//handle_sync_connect�����ú��ٵ���cancel()������Ч

		do
		{
			CrossSleep(5);
		} while (!m_bsync_connect_timer_complete);


		return ret;
	};//�ɹ�����0

private:
	string m_strServerAddress;
	string m_nPort;
	DWORD m_dwWaitSecond;

private:
	CrossCriticalSection m_cs;//��֤����m_pStreamDataʱ�Ǽ�����
	CXNetStreamData* volatile m_pStreamData;

private:
	CrossCriticalSection m_csRef;
	volatile unsigned int m_lRef; //�����ü���
	void AddRef()
	{
		m_csRef.Lock();// ���̲߳���������
		m_lRef++;
		m_csRef.Unlock();
	};
	void ReleaseRef()//ֻ����void������android����
	{
		m_csRef.Lock();// ���̲߳���������
		m_lRef--;
		m_csRef.Unlock();

		if (0 == m_lRef)
		{
			timer_ontimer.cancel();
			timer_.cancel();
			timer_sync_write_read.cancel();
			timer_connect.cancel();

			if (m_AsyncWriteData)
			{
				delete[] m_AsyncWriteData;
				m_AsyncWriteData = nullptr;
			}
			delete this;
		}

	};
	void Shutdown()
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
			trace_catch_error_code(__FILE__, __LINE__, __FUNCTION__,e.message().c_str());
		}
		catch (boost::exception &e) {
			trace_catch_error_code(__FILE__, __LINE__, __FUNCTION__,boost::diagnostic_information(e).c_str());
			trace_catch_error_code(__FILE__, __LINE__, __FUNCTION__,boost::current_exception_diagnostic_information().c_str());
		}
		catch (std::exception &e) {
			trace_catch_error_code(__FILE__, __LINE__, __FUNCTION__,e.what());
		}
		catch (...) {
			trace_catch_error_code(__FILE__, __LINE__, __FUNCTION__,"(catch (...))");
		}

	};
private:
	//connect
	boost::asio::deadline_timer timer_connect;//	Ϊʲô���ڴ�й¶��[20170619 ���Է��� 1.56.0�汾й¶   ��1.64.0�汾��й¶]
	void handle_sync_connect(const boost::system::error_code& error)
	{
		//
		//m_cs_sync_connect.Lock();

		if (error != boost::asio::error::operation_aborted)
		{
			Shutdown();//��ʱ����,�����ˣ�ǿ��connect�˳�
		}
		else
		{
			;
		}
		m_bsync_connect_timer_complete = TRUE;
		//m_cs_sync_connect.Unlock();

		ReleaseRef();
	};
	//CrossCriticalSection m_cs_sync_connect;
	volatile BOOL m_bsync_connect_timer_complete;//handle_sync_connect �Ƿ�ִ�����ˣ�ǿ����һ�ε�����ִ����handle_sync_connect
	//BOOL m_bsync_connect_timer_user_cancel;//�û�������cancel���handle_sync_connect��Ҫshutdown��

	/// Handle completion of a read operation.
	void handle_read(const boost::system::error_code& e, std::size_t bytes_transferred)
	{
		m_cs.Lock();
		if (m_pStreamData)
			m_pStreamData->OnPacketCompleteNetStreamData((!e) ? (int32_t)bytes_transferred : EVENT_ON_PACKET_COMPLETE_READING, ENUM_XTSTREAM_TRANSKEY_READ);
		m_cs.Unlock();
		if (e)
		{
			string s = e.message();
			trace_catch_error_code(__FILE__, __LINE__, __FUNCTION__,s.c_str());
		}

		ReleaseRef();
	};
	/// Handle completion of a write operation.
	void handle_write(const boost::system::error_code& e, std::size_t bytes_transferred)
	{
		m_cs.Lock();
		if (m_pStreamData)
			m_pStreamData->OnPacketCompleteNetStreamData((!e) ? (int32_t)bytes_transferred : EVENT_ON_PACKET_COMPLETE_WRITING, ENUM_XTSTREAM_TRANSKEY_WRITE);
		m_cs.Unlock();
		if (e)
		{
			string s = e.message();
			trace_catch_error_code(__FILE__, __LINE__, __FUNCTION__,s.c_str());
		}

		ReleaseRef();
	};
	///
	void handle_delay_write()
	{
		m_cs.Lock();
		if (m_pStreamData)
			m_pStreamData->OnPacketCompleteNetStreamData(0, ENUM_XTSTREAM_TRANSKEY_WRITE);
		m_cs.Unlock();
		ReleaseRef();
	};
	/// Handle completion of a timer operation.
	void handle_delay_timer()
	{
		m_cs.Lock();//��ס�ص�����֤һ��ֻ��һ���ص�
		if ((m_pStreamData))
			m_pStreamData->OnPacketCompleteNetStreamData(0, ENUM_XTSTREAM_TRANSKEY_TIMER);
		m_cs.Unlock();
		//
		ReleaseRef();
	};
private:
	//boost::asio::io_service& io_service_;
	/// Strand to ensure the connection's handlers are not called concurrently.
	boost::asio::io_service::strand strand_;
	boost::asio::ip::tcp::socket socket_;
	boost::asio::ip::tcp::resolver resolver;
	boost::asio::ip::tcp::resolver::query query;
	boost::asio::ip::tcp::resolver::iterator iterator;

	/// timer
	boost::asio::deadline_timer timer_;//	Ϊʲô���ڴ�й¶��[20170619 ���Է��� 1.56.0�汾й¶   ��1.64.0�汾��й¶]
	boost::asio::deadline_timer timer_ontimer;
	//�����շ����ݳ�ʱ
	boost::asio::deadline_timer timer_sync_write_read;//	Ϊʲô���ڴ�й¶��[20170619 ���Է��� 1.56.0�汾й¶   ��1.64.0�汾��й¶]
	void handle_sync_write_read(const boost::system::error_code& error)
	{
		if (error != boost::asio::error::operation_aborted)   //   ������ ��cancel�����Ծ����ڵȴ���ʱ����û�����SyncWriteAndRead
		{
			Shutdown();//��ʱ��
		}
		else
		{
			;//��cancel()��
		}
		m_bsync_write_read_complete = TRUE;
		ReleaseRef();
	};
	volatile BOOL m_bsync_write_read_complete;
private:
	char * m_AsyncWriteData;
	int32_t m_nAsyncWriteRealLen;
	int32_t m_nAsyncWriteDatBufferLen;//������ڴ��С������ʵ����Ҫ��
};

