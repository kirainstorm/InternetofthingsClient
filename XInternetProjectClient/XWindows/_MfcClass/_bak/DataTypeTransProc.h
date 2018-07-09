#ifndef __DATATYPE_TRANSPROC_H_23KLHWQ3873245__
#define __DATATYPE_TRANSPROC_H_23KLHWQ3873245__



//-----------------------------------------------------------------------------
//----------这个头文件是XTransmisServer与XTransmisProc.exe通讯使用
//
//基本架构：
//XTransmisServer读取配置文件，产生不同的XTransmisProc.exe，并于之心跳
//XTransmisProc.exe使用不同的库连接不同的设备，并推送到相应的服务器上
//-----------------------------------------------------------------------------

#include <atlbase.h>
#include <atlstr.h>
#include <string>
#include <vector>
#include <list>
#include <map>
using namespace std;

//-----------------------------------------------------------------------------
enum emCmdTransmisProcResult
{
	_TRANSMIS_PROC_RESULT_OK = 0,
	_TRANSMIS_PROC_RESULT_ERROR,
};
enum emCmdTransmisProc
{
	_TRANSMIS_PROC_OF_DEVICE_CONNNETTING = 0,	//通知XTransmisServer正在与设备连接
	_TRANSMIS_PROC_OF_DEVICE_CONNNETED,			//通知XTransmisServer与设备连接成功
	_TRANSMIS_PROC_OF_DEVICE_DISCONNNETED,		//通知XTransmisServer与设备断开

	_TRANSMIS_PROC_OF_SERVER_CONNNETTING,		//通知XTransmisServer正在与服务器连接
	_TRANSMIS_PROC_OF_SERVER_CONNNETED,			//通知XTransmisServer与服务器连接成功
	_TRANSMIS_PROC_OF_SERVER_DISCONNNETED,		//通知XTransmisServer与服务器断开

	_TRANSMIS_PROC_HEARBEAT, //心跳
};
//消息头
struct TRANSMIN_PROC_HEAD__T
{
	int cmd;
	int result;	//0
	int session;
	int k;//必须是0xada2
};




//-----------------------------------------------------------------------------
//设备类型，加载哪家的sdk取流
enum emTRANSMIN_PROC_DEVICE_TYPE
{
	TRANSMIN_PROC_DEVICE_ME_MEDIA_TO_MEDIA = 0,		//自己的服务器sdk，场景：将内网服务器的某台设备的流推送到公网服务器

	TRANSMIN_PROC_DEVICE_ME_MEDIA_TO_RTMP_HLS,		//自己的服务器sdk，场景：将服务器的某台设备的流推送到RTMP/HLS服务器

	TRANSMIN_PROC_DEVICE_ONVIF_TO_MEDIA,			//ONVIF，场景：其他家没有拿到sdk的使用onvif协议推送到服务器

	TRANSMIN_PROC_DEVICE_YYJ_TO_MEDIA,

	TRANSMIN_PROC_DEVICE_YYJP2P_TO_MEDIA,

	TRANSMIN_PROC_DEVICE_SRAJW_TO_MEDIA,

	TRANSMIN_PROC_DEVICE_SRAXIANG_TO_MEDIA,

};


//推送到哪种服务器上
enum emTRANSMIN_PROC_SERVER_TYPE
{
	TRANSMIN_PROC_SERVER_NORMAL = 0,

	TRANSMIN_PROC_SERVER_RTMP,			//nginx服务器
};


class XTransmisDeviceInterface
{
public:
	virtual void OpenTransmisDevice(char * p1 = NULL, char * p2 = NULL, char * p3 = NULL, char * p4 = NULL, int val1 = 0, int val2 = 0, int val3 = 0, int val4 = 0) = 0;
	virtual void CloseTransmisDevice() = 0;

	virtual BOOL GetError() = 0;//如果有错误则退出该程序（比如网络出错等），取巧的，不重连，这样做最简单。
};


#endif