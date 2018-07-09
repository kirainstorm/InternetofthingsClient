#ifndef _DEVICE_INTERFACE_STRUCT_H_KJ2342789LKS3K23
#define _DEVICE_INTERFACE_STRUCT_H_KJ2342789LKS3K23


//enum是不可靠的，不同的平台的长度可能不同，所以不可以直接用于结构体中的变量，切记
//尽量使用固定精度的数据类型，与32位兼容，不使用long之类的
//约定：所有int的函数返回0表示成功，其他失败

//#include "FrameData.h"

#ifdef _PLATFORM_WINDOW
#include <windows.h>
#include <tchar.h>
#include <atlstr.h>
#endif
//
#include <string>
#include <vector>
#include <list>
#include <map>
using namespace std;



// ---------------------- 流数据接口---------------------------------------------------------------------------
enum emFREAM_TYPE_DEFINE
{
	//H264
	FREAM_TYPE_H264_IFRAME = 0,
	FREAM_TYPE_H264_PFRAME,
	FREAM_TYPE_H264_OTHERS,
	//H265
	FREAM_TYPE_H265_IFRAME,
	FREAM_TYPE_H265_PFRAME,
	FREAM_TYPE_H265_OTHERS,
	//MPEG4
	FREAM_TYPE_MPEG4,
	//MJPEG
	FREAM_TYPE_MJPEG,
	//Audio
	FREAM_TYPE_ADPCM,
	FREAM_TYPE_G711A,
	FREAM_TYPE_G711U,
	FREAM_TYPE_G711UZYX,
	FREAM_TYPE_AAC,
};


// ---------------------------------------------------------------------------------------------------------------------------
enum emX_DEVICE_TYPE
{
	//用来定义厂商及其sdk
	X_DEVICE_TYPE_NONE = 0,
	X_DEVICE_TYPE_MEDIA,//流媒体
	X_DEVICE_TYPE_ONVIF,
	X_DEVICE_TYPE_GB28181,
	X_DEVICE_TYPE_YYJ_LAN, //yyj lan
	X_DEVICE_TYPE_YYJ_P2P,
	X_DEVICE_TYPE_SRIJW,
	X_DEVICE_TYPE_SRIXIANG,
};


// ---------------------------------------------------------------------------------------------------------------------------
//
enum emCallbackMessageType
{
	//-------------------------------------------------------------------------------------------
	XMSG_NET_STATUS____CONNECT_SUCCESS = 0,		//连接成功
	XMSG_NET_STATUS____CONNECT_FAIlED,			//INCLUDE : NOT CONNCTED or TIME_OUT
	XMSG_NET_STATUS____CONNECTING,				//正在连接
	XMSG_NET_STATUS____CONNECT_INVALID_PARAM,	//错误的id，用户名，密码
	XMSG_NET_STATUS____DEVICE_NOT_ON_LINE,		//设备不在线
	XMSG_NET_STATUS____DISCONNECTED,			//断线
	//-------------------------------------------------------------------------------------------
	XMSG_NET_STATUS_PREVIEW____CONNECT_SUCCESS,		//连接成功
	XMSG_NET_STATUS_PREVIEW____CONNECT_FAIlED,		//INCLUDE : NOT CONNCTED or TIME_OUT
	XMSG_NET_STATUS_PREVIEW____CONNECTING,			//正在连接
	XMSG_NET_STATUS_PREVIEW____DISCONNECTED,		//断线
	//-------------------------------------------------------------------------------------------
	XMSG_PLAT_USER_OPERATION_STATUS____LOGIN = 1000,				//
	XMSG_PLAT_USER_OPERATION_STATUS____LOGOUT,						//
	//
	XMSG_PLAT_USER_OPERATION_STATUS____MONEY,						//
	//
	XMSG_PLAT_USER_OPERATION_DISK_STATUS____ADD,					//
	XMSG_PLAT_USER_OPERATION_DISK_STATUS____REPLACE,				//
	XMSG_PLAT_USER_OPERATION_DISK_STATUS____DEL,					//
	//
	XMSG_PLAT_USER_OPERATION_DATATRAFFIC_STATUS____ADD,				//
	XMSG_PLAT_USER_OPERATION_DATATRAFFIC_STATUS____MAXDATA,			//设备流量足够，服务器可以转发
	XMSG_PLAT_USER_OPERATION_DATATRAFFIC_STATUS____MINDATA,			//设备流量耗尽，请服务器马上停止转发
	//
	XMSG_PLAT_USER_OPERATION_DEVICE_STATUS____DEVICE_ADD,			//设备增加	//每个<<<客户端>>><<<服务器>>>需要更新自己的设备列表
	XMSG_PLAT_USER_OPERATION_DEVICE_STATUS____DEVICE_UPDATA,		//设备修改	//每个<<<客户端>>><<<服务器>>>需要更新自己的设备列表
	XMSG_PLAT_USER_OPERATION_DEVICE_STATUS____DEVICE_DELETE,		//设备删除	//每个<<<客户端>>><<<服务器>>>需要更新自己的设备列表
	//
	XMSG_PLAT_USER_OPERATION_SERVER_STATUS____SERVER_ADD,			//增加
	XMSG_PLAT_USER_OPERATION_SERVER_STATUS____SERVER_UPDATA,		//修改
	XMSG_PLAT_USER_OPERATION_SERVER_STATUS____SERVER_DELETE,		//删除	//每个<<<客户端>>><<<服务器>>>需要更新自己的设备列表
	//
	XMSG_PLAT_USER_OPERATION_DEVICE_STATUS____DEVICESTATUS_ONLINE,	//设备在线	//每个<<<客户端>>><<<>>>需要更新自己的设备列表
	XMSG_PLAT_USER_OPERATION_DEVICE_STATUS____DEVICESTATUS_OFFLINE,	//设备离线	//每个<<<客户端>>><<<>>>需要更新自己的设备列表




	//-------------------------------------------------------------------------------------------
	XMSG_ALARM_TYPE____IO = 20000,
	XMSG_ALARM_TYPE____MOTION,
	//不允许在此后面添加除报警类型的其他的消息类型
};

// ---------------------------------------------------------------------------------------------------------------------------
//PTZMove Direction
#define				PTZ_MD_STOP							0									// 停止运动
#define				PTZ_MD_LEFT							1									// 向左
#define				PTZ_MD_RIGHT						2									// 向右
#define				PTZ_MD_UP							3									// 向上
#define				PTZ_MD_DOWN							4									// 向下
#define				PTZ_MD_LEFT_UP						5									// 左上
#define				PTZ_MD_LEFT_DOWN					6									// 左下
#define				PTZ_MD_RIGHT_UP						7									// 右上
#define				PTZ_MD_RIGHT_DOWN					8									// 右下
#define				PTZ_MD_RESET						9									// 复位
// PTZAction
#define				PTZ_ACTION_ZOOM_FAR					21									// 缩放，放大 
#define				PTZ_ACTION_ZOOM_NEAR				22									// 缩放，缩小
#define				PTZ_ACTION_ZOOM_STOP				23									// 停止缩放
#define				PTZ_ACTION_FOCUS_FAR				24									// 聚焦，往远处聚焦
#define				PTZ_ACTION_FOCUS_NEAR				25									// 聚焦，往近处聚焦
#define				PTZ_ACTION_FOCUS_STOP				26									// 停止聚焦
#define				PTZ_ACTION_AUTO_FOCUS				27									// 自动聚焦
#define				PTZ_ACTION_AUTO_FOCUS_STOP			28									// 停止自动聚焦
#define				PTZ_ACTION_AUTO_SCAN				29									// 自动扫描
#define				PTZ_ACTION_AUTO_SCAN_STOP			30									// 停止自动扫描
#define				PTZ_ACTION_CAMERA					31									// 打开||关闭摄像机
#define				PTZ_ACTION_IRIS						32									// 打开||关闭光圈
#define				PTZ_ACTION_IRIS_STOP				33									// 
//ptzpreset
#define				PTZ_PRESET_SET_PRESET				40									//SET云台预置位
#define				PTZ_PRESET_GO_PRESET				41									//GOTO云台预置位
#define				PTZ_PRESET_CL_PRESET				42									//CLEAR云台预置位


// ---------------------------------------------------------------------------------------------------------------------------
//基本信息
#define X_PARAM_TYPE_INFO 0
// ---------------------------------------------------------------------------------------------------------------------------
//图像参数
#define X_PARAM_TYPE_IMAGE 2
#define X_PARAM_TYPE_IMAGE_SET_DEFAULT 3	
#define X_PARAM_TYPE_IMAGE_SATURATION 4
#define X_PARAM_TYPE_IMAGE_BRIGHTNESS 5
#define X_PARAM_TYPE_IMAGE_CONTRAST 6

// ---------------------------------------------------------------------------------------------------------------------------
//编码参数
#define X_PARAM_TYPE_ENCODE 10
#define X_PARAM_TYPE_ENCODE_MAIN 11 //主码流
#define X_PARAM_TYPE_ENCODE_SUB 12	//子码流
#define X_PARAM_TYPE_ENCODE_FILP 13

#define X_PARAM_TYPE_ENCODE_MAIN_BPS 14

#define X_PARAM_TYPE_ENCODE_SUB_BPS 15
#define X_PARAM_TYPE_ENCODE_SUB_SIZE 16



//音频编码参数
#define X_PARAM_TYPE_AUDIO 19
// ---------------------------------------------------------------------------------------------------------------------------
//时间设置
#define X_PARAM_TYPE_GET_TIME 20
#define X_PARAM_TYPE_SET_TIME 21
#define X_PARAM_TYPE_SET_SNTP 22

#define X_PARAM_TYPE_TIME_TIMEZONE 23
#define X_PARAM_TYPE_TIME_NTPSVR	24

// ---------------------------------------------------------------------------------------------------------------------------
//网络参数
#define X_PARAM_TYPE_NET 30
#define X_PARAM_TYPE_NET_V2 31

// ---------------------------------------------------------------------------------------------------------------------------
//wifi参数
#define X_PARAM_TYPE_WIFI 40
#define X_PARAM_TYPE_WIFI_V2 41
// ---------------------------------------------------------------------------------------------------------------------------
//osd参数
#define X_PARAM_TYPE_OSD 50
#define X_PARAM_TYPE_OSD_NAME 51

// ---------------------------------------------------------------------------------------------------------------------------
//告警参数
#define X_PARAM_TYPE_ALARM 60
// ---------------------------------------------------------------------------------------------------------------------------
//用户参数
#define X_PARAM_TYPE_USER 70
// ---------------------------------------------------------------------------------------------------------------------------
//系统参数
#define X_PARAM_TYPE_SYS 80
// ---------------------------------------------------------------------------------------------------------------------------
//ftp参数
#define X_PARAM_TYPE_FTP 90
// ---------------------------------------------------------------------------------------------------------------------------
//email参数
#define X_PARAM_TYPE_EMAIL 100
// ---------------------------------------------------------------------------------------------------------------------------
//ptz参数
#define X_PARAM_TYPE_PTZ 110
// ---------------------------------------------------------------------------------------------------------------------------
//sd卡设置
#define X_PARAM_TYPE_SDCARD 120
#define X_PARAM_TYPE_SDCARD_FORMAT 121
//串口
#define X_PARAM_TYPE_SERIAL 130
//录像文件
#define X_PARAM_TYPE_RECORDFILES 140
//PLAT

#define X_PARAM_TYPE_PLAT_ME 150

// ---------------------------------------------------------------------------------------------------------------------------

#define X_PARAM_TYPE_GET_DEVICE_ALL_PARAM 20000

// ---------------------------------------------------------------------------------------------------------------------------

#define X_PARAM_TYPE_GET_DEVICE_ONLINE_STATUS 3000 //获取设备在线状态，给本地客户端用的
#define X_PARAM_TYPE_GET_DEVICE_PREVIEW_STATUS 3001//获取打开流的状态  0：没有流过来  1：流存在

//#define X_PARAM_TYPE_GET_DEVICE_HISLOG_FILE 3002//获取录像文件列表

//for sra
#define X_PARAM_TYPE_SRAJW_DEVICE_STATUS  200000









// ---------------------------------------------------------------------------------------------------------------------------
struct stX_DEVICE_ONLINE_STATUS
{
	int status; //0:no  1:ok
};




// struct HIS_LOG_T
// {
// 	int	log_id;
// 	int reserve;
// 	char store_file[128];
// 	XSERVERTIME	store_begin_time_tm;
// 	XSERVERTIME	store_end_time_tm;
// };

// enum PLAY_CTRL_TYPE
// {
// 	PLAY_CTRL_PAUSE = 0 ,
// 	PLAY_CTRL_SET_POS ,
// 	PLAY_CTRL_SET_SPEED ,
// 	PLAY_CTRL_SET_SOUND ,
// };

enum emLOCAL_PLAYBACK_TYPE
{
	LOCAL_PLAYBACK_CTRL_PAUSE = 0,
	LOCAL_PLAYBACK_CTRL_SET_POS,
	LOCAL_PLAYBACK_CTRL_SET_SOUND,
	LOCAL_PLAYBACK_CTRL_SET_SPEED,
};



#endif
