#ifndef __DATATYPE_DEVICE_H_KJ34UIWFWMDIU23MKASDJ5__
#define __DATATYPE_DEVICE_H_KJ34UIWFWMDIU23MKASDJ5__


#include <stdint.h>

//---------------------------------------------------------------------------------------------------------------------------
//
//		这个头文件是设备与服务器通讯的数据定义
//
//      消息：_head_t + data
//
//		透传消息：_head_t(_X_CMD_TRANS) + data
//
//		流数据：_head_t(_X_CMD_STREAM) +_stream_t + data
//
//---------------------------------------------------------------------------------------------------------------------------


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

#pragma pack()



#endif