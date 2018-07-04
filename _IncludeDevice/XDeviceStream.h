#ifndef __x_device_stream_header_kj34j45j234k45h54362kh
#define __x_device_stream_header_kj34j45j234k45h54362kh
#include "PlatformDefine.h"
#include "XDeviceStructDefine.h"
#include "XStream.hpp"
#include "Pool.hpp"
class CXDeviceStream :public  CXNetStreamData
{
public:
	CXDeviceStream();
	~CXDeviceStream();

public:
	virtual void OnPacketCompleteNetStreamData(int32_t bytesTransferred, emXStreamTansferKey transferKey);

public:
	void Connect(char * s_ip ,int s_port, char * uuid ,char * user);
	void Disconnect();
	BOOL IsError();//当前连接是否出错
	void AddSendStream(char *data , int len);

private:
	volatile BOOL m_bIsError;//初始化为TRUE
	CXNetStream* m_pTcpStream;
	//
	ST_XMEDIA_HEAD m_head;
	char *m_pBuffer;
	emNetRecieveStep m_nRecvStep;
	void DoMsg();
	CROSS_DWORD64 m_dwLastTick;
	//
};


#endif