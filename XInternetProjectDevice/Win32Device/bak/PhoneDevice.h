#ifndef PHONE_DEVICE_X_456W34324
#define PHONE_DEVICE_X_456W34324
#include <stdio.h>
#include <stdlib.h>
#include "PlatformDefine.h"
#include "XStream.hpp"
#include <boost/shared_ptr.hpp>
#include <boost/shared_array.hpp>
#include <boost/pool/pool.hpp>
//#include "XDefineMediaChannelStruct.h"

#pragma pack(1)

typedef struct _stream_t
{
	unsigned int avType;//emFREAM_TYPE_DEFINE
	unsigned int datalen;
	//
	unsigned int	nVideoFrameRate;	//帧率
	unsigned int	nVideoWidth;
	unsigned int	nVideoHeight;
	unsigned int	nVideoReserve;


	//audio
	//nAudioBitRate，nAudioBitsPerSample在不同的第三方库中可能只用一种，但是是可以计算出另一个的
	unsigned int	nAudioChannels;		//通道数: 单声道为 1, 立体声为 2
	unsigned int	nAudioSamplesRate;	// nAudioSamplesPerSec //每秒的采样样本数，采样率//8000,44100等
	unsigned int	nAudioBitRate;		//数据传输的平均速率(每秒的字节数)  自定义的值
	unsigned int	nAudioBitsPerSample;//采样大小(字节)比如16 
	//
	unsigned int reserve[2];
}streaminfo_t;

#define MAGIC_NUM 0XAA55

enum _X_CMD_DEFINE
{
	_X_CMD_LOGIN = 0,					//登陆
	_X_CMD_LOGIN_NEED_SESSION,		//key s -> c
	_X_CMD_HEARBEAT,					//心跳
	_X_CMD_LOGOUT,						//注销

	_X_CMD_OPEN_MAIN_STREAM,	//打开实时流
	_X_CMD_CLOSE_MAIN_STREAM,	//关闭实时流
	_X_CMD_OPEN_SUB_STREAM,		//打开实时流(子码流)
	_X_CMD_CLOSE_SUB_STREAM,	//关闭实时流(子码流)

	_X_CMD_STREAM,//表示这是流数据
	_X_CMD_TRANS = 48,//透明传输,表示这事消息数据
};

enum _X_CMD_RESULT
{
	_X_CMD_RESULT_OK = 0,
	_X_CMD_RESULT_ERROR
};

typedef struct _head_t
{
	unsigned int magicnum;
	unsigned int cmd;		//_X_CMD_DEFINE
	unsigned int session;
	unsigned int seq;

	//long long seq;
	long long tick;//设备需要使用硬件时间保证该数值一直是递增的


	unsigned int result;	//_X_CMD_RESULT_OK 服务器响应结果，客户不需要填充
	unsigned int datalen;
	unsigned int dataextern;
	unsigned int reserve[9];
}rqst_head;//request head

struct _login_t
{
	char user[64];
	char pwd[128]; // 使用des加密 //设备在平台的序列号+获取的sessionid一起加密
};
struct _gps_t
{
	double x;
	double y;
};

//-------------------------------------------------
#define MAX_MSG_4_PHONE_DEVICE_SESSION (1*1024*1024)
struct MyStruct4PhoneDeviceSession
{
	_head_t head;
	char buffer[MAX_MSG_4_PHONE_DEVICE_SESSION - sizeof(_head_t)];
};
#pragma pack()



enum emPhoneDeviceNetSendStep
{
	NET_SEND_STEP_SLEEP = 0,
	NET_SEND_STEP_SEND
};
// enum emNetRecieveStep
// {
// NET_RECIEVE_STEP_HEAD = 0,
// NET_RECIEVE_STEP_BODY
// };
class CPhoneDevice :public CXNetStreamData
{
public:
	CPhoneDevice();
	~CPhoneDevice();
	//////////////////////////////////////////////////////////////////////////
public:
	void Start();
	void Stop();
	void InputData(int avtype, int isKey, char * buffer, int len,int width,int heigt,int rate);


public:
	virtual void OnPacketCompleteNetStreamData(int32_t bytesTransferred, emXStreamTansferKey transferKey);
private:
	int LoginPlat();
	void LogoutPlat();
	void DoMsg();
	//
	BOOL m_bLogin;
	BOOL m_bStreamIsError;
	uint32_t m_nSession;
	//
	boost::pool<> m_plMsg;
	//
	emNetRecieveStep m_nRcvStep;
	_head_t m_head;
	int m_dataLen;
	unsigned char * m_szBuffer;
	//
	CXNetStream* m_pTcpStream;
	CrossCriticalSection m_csTcp;
	list<MyStruct4PhoneDeviceSession *> m_msgList;
	CrossCriticalSection m_csMsg;
	//
	//emPhoneDeviceNetSendStep m_nSendStep;
	static int WorkerThread(void*);
	void Worker();
	CROSS_THREAD_HANDLE	m_hWorkerThread;
	BOOL		m_hEvtWorkerStop;
	CROSS_DWORD64 m_dTickHearbeat;


};


#endif //PHONE_DEVICE_X_456W34324
