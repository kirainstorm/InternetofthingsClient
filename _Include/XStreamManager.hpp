#pragma once

//----------------------------------------
// kirainstorm
// https://github.com/kirainstorm
//----------------------------------------

#include "PlatformDefine.h"
#include "XStreamInterface.h"

class CXStreamManager
{
public:
	CXStreamManager(void) :work(io_service_)
	{
		m_bStart = FALSE;
#ifdef _PLATFORM_WINDOW
		WSADATA wsa_data;
		WSAStartup(0x0201, &wsa_data);
#endif
	};
	~CXStreamManager(void)
	{
		CROSS_TRACE("#delete# CXStreamManager::~CXStreamManager(void) \n");
#ifdef _PLATFORM_WINDOW
		WSACleanup();
#endif
	};
public:
	static CXStreamManager& Instance()
	{
		static CXStreamManager ins;
		return ins;
	};
public:
	long Start()
	{
		if (!m_bStart)
		{
			m_bStart = TRUE;

#ifdef _PLATFORM_WINDOW
			SYSTEM_INFO SystemInfo;
			GetSystemInfo(&SystemInfo);
			thread_pool_size_ = SystemInfo.dwNumberOfProcessors * 2 + 2;
#else
			thread_pool_size_ = 4;
#endif
			// Create a pool of threads to run all of the io_services.
			for (std::size_t i = 0; i < thread_pool_size_; ++i)
			{
				boost::shared_ptr<boost::thread> thread(new boost::thread(boost::bind(&boost::asio::io_service::run, &io_service_)));
				threads.push_back(thread);
			}
		}
		return 0;
	};
	void Stop()
	{
		if (m_bStart)
		{
			m_bStart = FALSE;
			io_service_.stop();
			// Wait for all threads in the pool to exit.
			for (std::size_t i = 0; i < threads.size(); ++i)
				threads[i]->join();
		}
	};
public:
	boost::asio::io_service io_service_;/// The io_service used to perform asynchronous operations.
private:
	volatile BOOL m_bStart;
	std::size_t thread_pool_size_;/// The number of threads that will call io_service::run().
	std::vector<boost::shared_ptr<boost::thread> > threads;
	boost::asio::io_service::work work;
};




// class CXSSLStreamManager //��ʱ������ wss ʹ��
// {
// public:
// 	CXSSLStreamManager(void) :work(io_service_), ssl_context(boost::asio::ssl::context::sslv23)
// 	{
// 		m_bStart = FALSE;
// #ifdef _PLATFORM_WINDOW
// 		WSADATA wsa_data;
// 		WSAStartup(0x0201, &wsa_data);
// #endif
// 		load_server_certificate(ssl_context);
// 	};
// 	~CXSSLStreamManager(void)
// 	{
// 		CROSS_TRACE("#delete# CXSSLStreamManager::~CXSSLStreamManager(void) \n");
// #ifdef _PLATFORM_WINDOW
// 		WSACleanup();
// #endif
// 	};
// public:
// 	static CXSSLStreamManager& Instance()
// 	{
// 		static CXSSLStreamManager ins;
// 		return ins;
// 	};

// public:
// 	long Start()
// 	{
// 		if (!m_bStart)
// 		{
// 			m_bStart = TRUE;
// #ifdef _PLATFORM_WINDOW
// 			SYSTEM_INFO SystemInfo;
// 			GetSystemInfo(&SystemInfo);
// 			thread_pool_size_ = SystemInfo.dwNumberOfProcessors * 2 + 2; //iocp�߳���������̫�ߣ����� n core * 2 + 2�����ܻἱ���½�
// #else
// 			thread_pool_size_ = 4;
// #endif
// 			// Create a pool of threads to run all of the io_services.
// 			for (std::size_t i = 0; i < thread_pool_size_; ++i)
// 			{
// 				boost::shared_ptr<boost::thread> thread(new boost::thread(boost::bind(&boost::asio::io_service::run, &io_service_)));
// 				threads.push_back(thread);
// 			}
// 		}
// 		return 0;
// 	};
// 	void Stop()
// 	{
// 		if (m_bStart)
// 		{
// 			m_bStart = FALSE;
// 			io_service_.stop();
// 			// Wait for all threads in the pool to exit.
// 			for (std::size_t i = 0; i < threads.size(); ++i)
// 				threads[i]->join();
// 			//
// 		}
// 	};

// public:
// 	boost::asio::io_service io_service_;/// The io_service used to perform asynchronous operations.
// 	boost::asio::ssl::context ssl_context;// The SSL context is required, and holds certificates// { boost::asio::ssl::context::sslv23 };
// private:
// 	volatile BOOL m_bStart;
// 	std::size_t thread_pool_size_;/// The number of threads that will call io_service::run().
// 	std::vector<boost::shared_ptr<boost::thread> > threads;
// 	boost::asio::io_service::work work;



// private:
// 	std::string get_password() const
// 	{
// 		return "214536426970268";
// 	}

// 	void load_server_certificate(boost::asio::ssl::context& ctx)
// 	{

// 		//ʹ�õ��ǰ�������������ca
// 		//dh���������ɵ�
// 		//ʹ�õ�ֱ�Ӽ���file�ķ�ʽ��ʹ��dh����"-----BEGIN CERTIFICATE-----\n"��ʽ���׳������ִ���std::exception::what: use_rsa_private_key: key values mismatch

// 		std::string const certfile = "c:\\214536426970268.pem";
// 		std::string const keyfile = "c:\\214536426970268.key";
// 		//��openssl����õ��ļ����ڴ�openssl.exeִ��dhparam -out dh.pem 2048
// 		std::string const dh =
// 			"-----BEGIN DH PARAMETERS-----\n"
// 			"MIIBCAKCAQEA5MNEfyUwhK/gAYQDzUiKMXv9bEs/Rn2t2Vf9d1yk4kqcQbX/Aqyf\n"
// 			"jbBXLFH6VLsj81WEf8o0TucFBefg1IrwEmpplsMcdaoN1JAxgIwIij2Ex8B648ty\n"
// 			"IMbAeaEkiKADBzwZud6QMtf+PT337QwMtHCa2848mMVW1kYtTUkejNpbn+a0nUqr\n"
// 			"nAcveYU70G4AhVoX4ycBht0jyiAGToWvFjyZ6VAEyuRooPb3nKXa0mSvX2qRkudG\n"
// 			"a+XYJXYp3JHPkZbJaUuCmKGrwU4qktZ7Otbgd4vJiIwh8Iae8GilMXx9QgEhuiwO\n"
// 			"wK8kPBEkJJbHdSEtxoXx4mE5NR9k15JpEwIBAg==\n"
// 			"-----END DH PARAMETERS-----\n";

// 		//
// 		//
// 		ctx.set_password_callback(boost::bind(&CXSSLStreamManager::get_password, this));
// 		//
// 		ctx.set_options(
// 			boost::asio::ssl::context::default_workarounds |
// 			boost::asio::ssl::context::no_sslv2 | //sslv2������ȫ
// 			boost::asio::ssl::context::single_dh_use);
// 		//
// 		ctx.use_certificate_chain_file(certfile);
// 		//
// 		ctx.use_rsa_private_key_file(keyfile, boost::asio::ssl::context::file_format::pem);
// 		//
// 		ctx.use_tmp_dh(boost::asio::buffer(dh.data(), dh.size()));
// 	}
// };