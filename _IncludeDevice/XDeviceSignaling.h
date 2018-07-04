#ifndef __x_device_signaling_header_kl34fd43sd56f7sfd6d
#define __x_device_signaling_header_kl34fd43sd56f7sfd6d
#include "PlatformDefine.h"
#include "XDeviceStructDefine.h"
#include "XStream.hpp"
#include "Pool.hpp"
//--------------------------------------------------------------------------------------------------------------------
#define ST_DEVICE_SIGNALING_BUFFER_LEN_LITTLE (LITTLE_BUFFER_SIZE_KB - sizeof(signaling_channel_head_t))
struct ST_DEVICE_SIGNALING_SEND_BUFFER
{
	signaling_channel_head_t head;
	uint8_t msg[ST_DEVICE_SIGNALING_BUFFER_LEN_LITTLE];
};
class CXDeviceSignalingBufferPool
{
public:
	CXDeviceSignalingBufferPool() {};
	~CXDeviceSignalingBufferPool() {};
	static CXDeviceSignalingBufferPool& Instance(){
		static CXDeviceSignalingBufferPool inc;
		return inc;
	};
	ST_DEVICE_SIGNALING_SEND_BUFFER * malloc(){
		ST_DEVICE_SIGNALING_SEND_BUFFER * pBuffer = (ST_DEVICE_SIGNALING_SEND_BUFFER *)CLittleBufferPool::Instance().malloc();
		return pBuffer;
	}
	void free(ST_DEVICE_SIGNALING_SEND_BUFFER * p){
		CLittleBufferPool::Instance().free((char *)p);
	}
};
//--------------------------------------------------------------------------------------------------------------------
class CXDeviceSignaling :public  CXNetStreamData
{
public:
	CXDeviceSignaling(XDeviceMessageCallback *cb);
	~CXDeviceSignaling();

public:
	virtual void OnPacketCompleteNetStreamData(int32_t bytesTransferred, emXStreamTansferKey transferKey);

public:
	void Connect(char * s_ip, char * uuid, char * user);
	void Disconnect();
	BOOL IsError();//当前连接是否出错
	void AddSendMessage(int type,  char *buffer, int len);

private:
	volatile BOOL m_bIsError;//初始化为TRUE
	CXNetStream* m_pTcpStream;
	//
	XDeviceMessageCallback *m_pCallBack;
	unsigned int m_nSessionID;
	//
	signaling_channel_head_t m_head;
	char *m_pBuffer;
	emNetRecieveStep m_nRecvStep;
	void DoMsg();
	CROSS_DWORD64 m_dwLastTick;
	//
	CrossCriticalSection m_csSendList;//list多线程不安全
	list<ST_DEVICE_SIGNALING_SEND_BUFFER *> m_msgSendList;
	void ClearSendBufferList();
	//
#if XINTERNET_TEST_STATUS
	volatile BOOL m_bCanReleaseSuccessCounter;
#endif
};

#endif