#ifndef __x_device_inatance_header_sdghk6543dfg6j5432kh
#define __x_device_inatance_header_sdghk6543dfg6j5432kh
#include "PlatformDefine.h"
#include "XDeviceSignaling.h"
#include "XDeviceStream.h"

#if XINTERNET_TEST_STATUS
class CDeviceMessageCb :public XDeviceMessageCallback
{
public:
	CDeviceMessageCb() {};
	~CDeviceMessageCb() {};
	virtual void OnDeviceMessageCallback(int type, char *buffer, int len)
	{

	};
private:

};
#endif

class CXDeviceInstance :public XDeviceInterface
{
public:
	CXDeviceInstance();
	~CXDeviceInstance();
public:
	virtual void XDelete();
	//
	virtual int XConnect(XDeviceMessageCallback *callback, char * s_ip, char * uuid);
	virtual int XDisconnect();
	//
	virtual int XSendMeaasge();
	virtual int XSendStream();
private:
	CXDeviceStream *m_pStream;
	CXDeviceSignaling *m_pSignaling;
	XDeviceMessageCallback * m_pMessageCallback;
private:
	char m_szsip[64];
	int s_nsport;
	char m_szuuid[64];
private:
	volatile BOOL m_bTheadExit;
	CROSS_THREAD_HANDLE m_hThread;
	static int ThreadWorker(void * param);
	void Woker();
	//
	char m_sziotip[64];
	char m_szusername[64];
	void GetIotServerInfo();
};

#endif