#ifndef __DATATYPE_SERVER_H_23KLHWQ38D2373245__
#define __DATATYPE_SERVER_H_23KLHWQ38D2373245__

#include <stdint.h>

//---------------------------------------------------------------------------------------------------------------------------
//
//----------这个头文件是<<各种客户端/流媒体服务器与 平台中心服务器>> <<MediaInterface与流媒体服务器>>通讯使用
//
//----------请注意结构体不要使用std::tm在android和windows下是不同的
//
//---------------------------------------------------------------------------------------------------------------------------


//消息命令
enum emCmdOfServer
{
	//---------------------------------------------------------------------------------------------------------------
	//用户登陆步骤
	//1:必须先发送NEED_SESSIONID
	//2:LOGIN
	Z_CMD_OF_SERVER_LOGIN_NEED_SESSIONID = 0,//流转发服务器向客户端发送
	Z_CMD_OF_SERVER_LOGIN,
	Z_CMD_OF_SERVER_LOGOUT,

	Z_CMD_OF_SERVER_HEARBEAT,//心跳保活

	Z_CMD_OF_SERVER_OPEN_STREAM,			//请求实时流 //服务器不返回回应
	Z_CMD_OF_SERVER_CLOSE_STREAM,			//请求实时流 //服务器不返回回应

	Z_CMD_OF_SERVER_GET_STORE_HISTORY_LOG,	//获取录像列表,请使用短链接的方式获取  client->media
	Z_CMD_OF_SERVER_OPEN_HIS_STREAM,		//下载	//服务器不返回该cmd，返回的是Z_CMD_OF_SERVER_AV_STREAM，请注意判断result区分_CMD_OF_SERVER_RESULT_DOWNLOAD_XXXX
	Z_CMD_OF_SERVER_CLOSE_HIS_STREAM,		//关闭下载

	Z_CMD_OF_SERVER_GET_DEVICE_LIST,		//从中心服务器获取设备列表,请使用短链接的方式获取(包括了区域) (同时包括公共发布的视频) c->S

	//---------------------------------------------------------------------------------------------------------------
	Z_CMD_OF_SERVER_AV_STREAM,				//表示当前传输的是音视频数据
	Z_CMD_OF_SERVER_TRANS_TRANSMISSION,		//透明传输指令


	//---------------------------------------------------------------------------------------------------------------
	//smartlife
	Z_CMD_OF_SERVER_SMARTLIFE_ACCOUNT_REGISTER,			//在平台中心注册（短链接的方式）
	Z_CMD_OF_SERVER_SMARTLIFE_ACCOUNT_CHANGE_PWD,		//修改密码（短链接的方式）
	Z_CMD_OF_SERVER_SMARTLIFE_ACCOUNT_ADD_DEVICE,		//将平台中的某个ID设为当前账号可用（短链接的方式）
	Z_CMD_OF_SERVER_SMARTLIFE_ACCOUNT_DELETE_DEVICE,	//将平台中的某个ID从当前账号移除（短链接的方式）
	//---------------------------------------------------------------------------------------------------------------



};

enum emXSERVER_FRAME_TYPE_DEFINE
{
	//H264
	XSERVER_FREAM_TYPE_H264_IFRAME = 0,
	XSERVER_FREAM_TYPE_H264_PFRAME,
	XSERVER_FREAM_TYPE_H264_OTHERS,
	//H265
	XSERVER_FREAM_TYPE_H265_IFRAME,
	XSERVER_FREAM_TYPE_H265_PFRAME,
	XSERVER_FREAM_TYPE_H265_OTHERS,
	//MPEG4
	XSERVER_FREAM_TYPE_MPEG4,
	//MJPEG
	XSERVER_FREAM_TYPE_MJPEG,
	//Audio
	XSERVER_FREAM_TYPE_PCM,
	XSERVER_FREAM_TYPE_ADPCM,
	XSERVER_FREAM_TYPE_G711A,
	XSERVER_FREAM_TYPE_G711U,
	XSERVER_FREAM_TYPE_AAC,
};

enum emCmdResultOfServer
{
	_CMD_OF_SERVER_RESULT_OK = 0,
	_CMD_OF_SERVER_RESULT_ERROR,
	_CMD_OF_SERVER_RESULT_SMARTLIFE_REGISTER_ERROR,//smartlife  用户名已经存在

	_CMD_OF_SERVER_RESULT_DOWNLOAD_START, //下载开始标志
	_CMD_OF_SERVER_RESULT_DOWNLOAD_END,//下载结束标志
	_CMD_OF_SERVER_RESULT_DOWNLOAD_DATA,//数据标志
	_CMD_OF_SERVER_RESULT_DOWNLOAD_ERROR,//出错标志
};

struct _XSERVER_TIME {
	uint16_t wYear;
	uint16_t wMonth;
	uint16_t wDayOfWeek;
	uint16_t wDay;
	uint16_t wHour;
	uint16_t wMinute;
	uint16_t wSecond;
	uint16_t wMilliseconds;
};
//消息头
struct ST_SERVER_HEAD
{
	//------
	uint16_t cmd;		//emCmdOfServer
	uint16_t result;	//emCmdResultOfServer
	uint16_t seq;		//自增
	uint16_t reserve;
	//------
	uint16_t wYear;
	uint16_t wMonth;
	uint16_t wDayOfWeek;
	uint16_t wDay;
	uint16_t wHour;
	uint16_t wMinute;
	uint16_t wSecond;
	uint16_t wMilliseconds;
	//av?
	uint16_t	nAVFrameType;			//帧数据类型 //emXSERVER_FRAME_TYPE_DEFINE
	//video
	uint16_t	nVideoFrameRate;	//帧率
	uint16_t	nVideoWidth;
	uint16_t	nVideoHeight;
	//audio
	//nAudioBitRate，nAudioBitsPerSample在不同的第三方库中可能只用一种，但是是可以计算出另一个的
	uint16_t	nAudioChannels;		//通道数: 单声道为 1, 立体声为 2
	uint16_t	nAudioSamplesRate;	// nAudioSamplesPerSec //每秒的采样样本数，采样率//8000,44100等
	uint16_t	nAudioBitRate;		//数据传输的平均速率(每秒的字节数)  自定义的值
	uint16_t	nAudioBitsPerSample;//采样大小(字节)比如16 
	//------
	uint32_t session;
	uint32_t datalen;
};

struct ST_SREVER_LOGIN_INFO
{
	int	dev_id;			//设备在数据库的ID
	int dev_channel;	//一定是0，即使是NVR，每个通道都有一个独立ID
	char user[32];
	char pwd[256];		// 使用des加密
};

//--------- 该参数与平台通讯使用
struct ST_SREVER_DEVICE_INFO_BASE
{
	int dev_is_publish;			//0:私有设备  1：publish device
	//
	int	dev_id;					//设备在数据库的id
	int	dev_areaid;				//区域id
	int dev_parent_areaid;		//区域id的父id
	//
	char dev_name[64];			//设备名称
	char dev_area_name[64];		//区域名称
	char dev_media_ip[16];		//核心服务器ip
	char dev_publish_ip[16];	//分发服务器ip
	//
	int	dev_media_port;			//核心服务器预览端口
	int dev_playback_port;		//核心服务器回放端口
	int dev_publish_port;		//分发服务器预览端口
	//
	int dev_browse_num;			//浏览用户数量
	int dev_online;				//是否在线

	int reserve[3];
};

//打开历史视频
struct ST_SREVER_OPEN_HIS_STREAM
{
	_XSERVER_TIME	tmBeginTime;
	_XSERVER_TIME	tmEndTime;
};

//获取在某一时间段的历史视频
struct ST_SREVER_GET_HIS_LOG
{
	_XSERVER_TIME	tmBeginTime;
	_XSERVER_TIME	tmEndTime;
};
struct ST_SREVER_HIS_LOG
{
	int nHislogID;
	int reserve;
	char szFile[128];
	_XSERVER_TIME	begin_time_tm;
	_XSERVER_TIME	end_time_tm;
};
struct ST_SREVER_HIS_LOG_LIST
{
	ST_SREVER_HIS_LOG t[128];//最多返回128个文件
};



struct ST_SREVER_SMARTLIFE_CHANGEPWD
{
	char newpwd[256];		// 使用des加密,由于使用短链接，在登陆时已经验证了原始密码
};
struct ST_SREVER_SMARTLIFE_DEVICE
{
	char devuuid[32];
};






#endif