#pragma once

#include "XStream.h"
#include "xcross.h"
#include "DataTypeServer.h"

#ifdef _PLATFORM_ANDROID
#include "XDes.h"
#endif


#ifndef MAX_XML_BUFFER_SIZE
#define MAX_XML_BUFFER_SIZE		(2*1024*1024)
#endif

// start [7/11/2016 sun]
// Short connection
// end   [7/11/2016 sun]

class CNetToServer
{
public:
	CNetToServer();
	~CNetToServer();
public:
	static CNetToServer& Instance();

public:
	int GetUserDevices(const char *ip, int port, const char* user, const char * pwd, vector<ST_SREVER_DEVICE_INFO_BASE> &rs);

private:
	CrossCriticalSection m_cs;
	 
};


CNetToServer::CNetToServer()
{
}


CNetToServer::~CNetToServer()
{
}
CNetToServer& CNetToServer::Instance()
{
	static CNetToServer inc;
	return inc;
}
int CNetToServer::GetUserDevices(const char *ip, int port, const char* user, const char * pwd, vector<ST_SREVER_DEVICE_INFO_BASE> &rs)
{

	int m_Session;

	int ret = -1;
	ST_SERVER_HEAD stHeadRecv;

	m_cs.Lock();
	CXNetStream* m_pTcpStream = NULL;

	do
	{
		CROSS_TRACE(">>>Login server<<< cc");
		m_pTcpStream = XNetCreateStream4Connect(ip, port, 3);
		if (NULL == m_pTcpStream)
		{
			break;
		}
		CROSS_TRACE(">>>Login server<<< cc1");
		if (0 != XNetConnectStream(m_pTcpStream))
		{
			break;
		}
		//recv key
		ST_SERVER_HEAD t;
		memset(&t, 0, sizeof(t));
		t.cmd = Z_CMD_OF_SERVER_LOGIN;
		t.datalen = 0;
		//send "need session"
		if (0 != m_pTcpStream->SyncWriteAndRead(&t, sizeof(ST_SERVER_HEAD), &stHeadRecv, sizeof(ST_SERVER_HEAD)))
		{
			CROSS_TRACE(">>>Login server<<< 111");
			break;
		}
		if (stHeadRecv.cmd != Z_CMD_OF_SERVER_LOGIN_NEED_SESSIONID)
		{
			CROSS_TRACE(">>>Login server<<< 222");
			break;
		}

		if (stHeadRecv.datalen != 0)
		{
			CROSS_TRACE(">>>Login server<<< 333");
			break;
		}


		//Encode "key"
		char szTempSend[1024] = { 0 };
		ST_SREVER_LOGIN_INFO _login;
		memset(&_login, 0, sizeof(_login));

		CROSS_STRCPY(_login.user, user);

		char dest[256] = { 0 };
		XDESEncode(pwd, stHeadRecv.session, dest);
		m_Session = stHeadRecv.session;

		CROSS_STRCPY(_login.pwd, dest);//<<-------------------------

		//char mkdest[256] = { 0 };
		//unsigned int mkmk = 0;
		//XDESDecode(_login.pwd, mkdest, mkmk);


		t.cmd = Z_CMD_OF_SERVER_LOGIN;
		t.datalen = sizeof(ST_SREVER_LOGIN_INFO);
		t.session = m_Session;

		memcpy(szTempSend, &t, sizeof(ST_SERVER_HEAD));
		memcpy(szTempSend + sizeof(ST_SERVER_HEAD), &_login, sizeof(ST_SREVER_LOGIN_INFO));
		if (0 != m_pTcpStream->SyncWriteAndRead(szTempSend, sizeof(ST_SERVER_HEAD) + sizeof(ST_SREVER_LOGIN_INFO), &stHeadRecv, sizeof(ST_SERVER_HEAD)))
		{
			CROSS_TRACE(">>>Login server<<< 444");
			break;
		}
		if (stHeadRecv.cmd != Z_CMD_OF_SERVER_LOGIN)
		{
			CROSS_TRACE(">>>Login server<<< 555");
			break;
		}
		if (stHeadRecv.result != _CMD_OF_SERVER_RESULT_OK)
		{
			CROSS_TRACE(">>>Login server<<< 666");
			break;
		}


		t.cmd = Z_CMD_OF_SERVER_GET_DEVICE_LIST;
		t.session = m_Session;
		t.datalen = 0;
		if (0 != m_pTcpStream->SyncWriteAndRead(&t, sizeof(ST_SERVER_HEAD), &stHeadRecv, sizeof(ST_SERVER_HEAD)))
		{
			CROSS_TRACE(">>>Login server<<< 777");
			break;
		}
		if (stHeadRecv.cmd != Z_CMD_OF_SERVER_GET_DEVICE_LIST)
		{
			CROSS_TRACE(">>>Login server<<< 888");
			break;
		}
		if (stHeadRecv.result != _CMD_OF_SERVER_RESULT_OK)
		{
			CROSS_TRACE(">>>Login server<<< 999");
			break;
		}


		if (stHeadRecv.datalen > 0)
		{
			char *pBuffer = new char[stHeadRecv.datalen + 1];
			pBuffer[stHeadRecv.datalen] = '\0';

			if (0 != m_pTcpStream->SyncWriteAndRead(NULL, 0, pBuffer, stHeadRecv.datalen))
			{
				CROSS_TRACE(">>>Login server<<< 101010");
				break;
			}

			for (size_t i = 0; i < (stHeadRecv.datalen / sizeof(ST_SREVER_DEVICE_INFO_BASE)); i++)
			{
				ST_SREVER_DEVICE_INFO_BASE * _dev = (ST_SREVER_DEVICE_INFO_BASE *)(pBuffer + i*sizeof(ST_SREVER_DEVICE_INFO_BASE));

				ST_SREVER_DEVICE_INFO_BASE st;
				memcpy(&st, _dev, sizeof(ST_SREVER_DEVICE_INFO_BASE));

				rs.push_back(st);
			}

			delete[] pBuffer;
		}
		ret = 0;
		CROSS_TRACE(">>>Login server<<< okokok");
	} while (FALSE);

	if (m_pTcpStream)
	{
		m_pTcpStream->Release();
		m_pTcpStream = NULL;
	}

	m_cs.Unlock();
	return ret;


	return 0;

}
