#ifndef _DEVICE_INTERFACE_H_KJ2342789LKS3K23
#define _DEVICE_INTERFACE_H_KJ2342789LKS3K23


/*
//enum是不可靠的，不同的平台的长度可能不同，所以>>>>>>> 不 可 以 <<<<<<直接用于结构体中的变量，切记
该问题就是sizeof( EType1 )等于多少的问题，是不是每一个用户自定义的枚举类型都具有相同的尺寸呢？
在大多数的32位编译器下（如：VC++、gcc等）一个枚举类型的尺寸其实就是一个sizeof( int )的大小，难道枚举类型的尺寸真的就应该是int类型的尺寸吗？
其实不是这样的，在C++标准文档（ISO14882）中并没有这样来定义，
标准中是这样说明的：“枚举类型的尺寸是以能够容纳最大枚举子的值的整数的尺寸”，
同时标准中也说名了：“枚举类型中的枚举子的值必须要能够用一个int类型表述”，
*/

#include "PlatformDefine.h"
#include "XCross.h"

//与SYSTEMTIME相同的结构体
struct XSERVERTIME {
	uint16_t wYear;
	uint16_t wMonth;
	uint16_t wDayOfWeek;
	uint16_t wDay;
	uint16_t wHour;
	uint16_t wMinute;
	uint16_t wSecond;
	uint16_t wMilliseconds;
} ;

class CXMessageCallback;
class CXStreamData;
class CXDecVideoData;
class CXDeviceInterface;
//
class CXPreviewInterface;//实时预览接口
//
class CXTalkInterface;//对讲接口
//
class CXLocalPlaybackInterface;//本地预览接口
class CXLocalPlaybackProgressCallback;//本地播放进度回调
//
//class CXOnlinePlaybackInterface;//在线回放接口
//class CXOnlinePlaybackProgressCallback;//在线回放播放进度回调
//
class CXDownloadHisFileInterface;//下载文件接口
class CXDownloadHisFileCallback;
//
class CXTranTransmissionInterface;//透明传输接口



#include "DevInterfaceStruct.h"
#include "PlatformDefine.h"
#include "XPlayer.h"


//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------
class CXMessageCallback//消息回调接口
{
public:
	virtual int OnMessageCallback(int nDevID, int nDevChannel, int nMessageType, int nMessageVal, void * pUser) = 0;
};
class CXStreamData//原始流数据回调接口
{
public:
	virtual int OnStreamData(const char* pStreamData, int nLen, int nFrameType /*emFREAM_TYPE_DEFINE*/, int nTime, int nFramerate) = 0;
};
// class CXDecVideoData//解码后数据回调接口
// {
// public:
// 	virtual int OnDecVideoData(const char* pDecData, int nLen, int nWidth, int nHeight, int nTime, int nTypeFrame = 0, int nOutFormat = 0, int nFrameRate = 0) = 0;
// };
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------




//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------
class CXDeviceInterface
{
public:
	virtual int XDeviceDelete() = 0;

public:
	//[当为ipc/nvr设备时]	--- 设备IP地址-设备端口-设备用户-设备密码-P2PID
	//[当从流媒体取流时]	--- 流媒体IP地址-流媒体端口-中心用户-中心密码-设备ID
	virtual int XDeviceSetInfo(const char* szIP, int nPort, const char* szUser, const char* szPwd, const char* szUuid, int nVal1, int nVal2, emX_DEVICE_TYPE nType, CXMessageCallback *cb = NULL, void * pUser = NULL) = 0;
	//open
	virtual int XDeviceOpen() = 0;
	//close
	virtual int XDeviceClose() = 0;
	//云台
	virtual int XDevicePTZControl(int nChannel, int nCtrlType, int nVal1, int nVal2 = 0, int nVal3 = 0) = 0;
	//获取通道参数//特别说明：每家的CXXXDevInterface和ui显示部分，使用自己的参数结构体强转szChanParam
	virtual int XDeviceGetChannelParam(int nChannel, char * szChanParam, int& nlen, int nParamType/*X_PARAM_TYPE_XXX*/, const  char * pUserData = NULL) = 0;
	//设置通道参数
	virtual int XDeviceSetChannelParam(int nChannel, const  char *  szChanParam, int nlen, int nParamType/*X_PARAM_TYPE_XXX*/) = 0;

public:
	//实时预览
	virtual CXPreviewInterface* CreatePreviewInstance() = 0;
	//对讲
	virtual CXTalkInterface* CreateTalkInstance() = 0;
	//本地预览
	virtual CXLocalPlaybackInterface* CreateLocalPlaybackInstance() = 0;
	//在线回放
	//virtual CXOnlinePlaybackInterface* CreateOnlinePlaybackInstance() = 0;
	//下载文件
	virtual CXDownloadHisFileInterface* CreateDownloadHisFileInstance() = 0;
	//透明传输
	virtual CXTranTransmissionInterface* CreateTranTransmissionInstanceForMediaSever() = 0;//提供给第三方智能设备使用，比如：智能家居，报警器等等
};
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------



//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------
//每家的sdk请自己做好自动重连
class CXPreviewInterface
{
public:
	virtual int XPreviewDelete() = 0;

public:
#ifdef _PLATFORM_WINDOW
	//hWnd可以为NULL
	virtual int XPreviewOpenForWindows(int nChannel, HWND hWnd, int nLinkMode/* 0:MAIN CHANNEL ,1:SUB CHANNEL */, CXStreamData* pDataObj = NULL, DecodeVideoCallBack* pDecDataObj = NULL, void *pUser = NULL) = 0;
#endif
#if (defined _PLATFORM_ANDROID) || (defined _PLATFORM_IOS) || (defined _PLATFORM_WINDOW_WIN10_UWP)
	virtual int XPreviewOpenForApp(DecodeVideoCallBack* pDecDataObj = NULL, CXStreamData* pVoiceObj = NULL, void *pUser = NULL) = 0;
#endif
	//
	virtual int XPreviewClose() = 0;
	//播放声音
	virtual int XPreviewSound(int nOpenSound) = 0;
	//通道拍照，在流中截取图片//建议:szFilePath由以下组成：用户选择的根目录+后额外加上Pictures目录+日期目录+设备名称目录//方便查找//请确保:szFilePath,szFileName不包含特殊字符
	virtual int XPreviewCapturePictures(const char* szFilePath, const char* szFileName) = 0;
	//通道录像
	//virtual int XPreviewRecord(const char* szFilePath, const char *szDevName, int nSec, int nSpace, int nFreeSpace, int nAutoRemain, BOOL bStart) = 0;
	virtual int XPreviewRecord(const char* szFilePath, const char *szDevName, int val, BOOL bStart) = 0;
};
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------



//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------
class CXLocalPlaybackInterface
{
public:
	virtual int XLocalPlaybackDelete() = 0;

public:
#ifdef _PLATFORM_WINDOW
	virtual int XLocalPlaybackOpenForWindows(HWND hWnd, const char* szFile, XSERVERTIME &nStartTime/*[out]*/, XSERVERTIME &nEndTime/*[out]*/, CXLocalPlaybackProgressCallback *pProgressObj = NULL, void *pUser = NULL) = 0;
#endif
#if (defined _PLATFORM_ANDROID) || (defined _PLATFORM_IOS) || (defined _PLATFORM_WINDOW_WIN10_UWP)
	virtual int XLocalPlaybackOpenForApp(const char* szFile, XSERVERTIME &nStartTime/*[out]*/, XSERVERTIME &nEndTime/*[out]*/, CXDecVideoData* pDecDataObj, CXLocalPlaybackProgressCallback *pProgressObj = NULL, void *pUser = NULL) = 0;
#endif
	virtual int XLocalPlaybackClose() = 0;
	//
	virtual int XLocalPlaybackPlayCtrl(int nType /*emLOCAL_PLAYBACK_TYPE*/, int val) = 0;
};
class CXLocalPlaybackProgressCallback
{
public:
	//回调CXLocalPlaybackInterface指针给你
	virtual int OnLocalPlaybackProgressCallback(XSERVERTIME stTime, const CXLocalPlaybackInterface *pInstance, const void * pUser) = 0;
};
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------



//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------
//only for ipc sd'card
// class CXOnlinePlaybackInterface
// {
// 	
// public:
// 	virtual int XOnlinePlaybackDelete() = 0;
// 
// public:
// #ifdef _PLATFORM_WINDOW
// 	virtual int XXOnlinePlaybackForWindows(HWND hWnd, const char* szFile, int val1, int val2, int val3, CXOnlinePlaybackProgressCallback *pProgressObj = NULL, void *pUser = NULL) = 0;
// #endif
// #if (defined _PLATFORM_ANDROID) || (defined _PLATFORM_IOS) || (defined _PLATFORM_WINDOW_WIN10_UWP)
// 	virtual int XOnlinePlaybackForApp(const char* szFile, int val1, int val2, int val3, CXOnlinePlaybackProgressCallback *pProgressObj = NULL, void *pUser = NULL) = 0;
// #endif
// 	virtual int XOnlinePlaybackClose() = 0;
// 	//
// 	virtual int XOnlinePlaybackPlayCtrl(int nType /*emLOCAL_PLAYBACK_TYPE*/, int val) = 0;
// };
// class CXOnlinePlaybackProgressCallback
// {
// public:
// 	virtual int OnOnlinePlaybackProgressCallback(int val1, int val2, int val3, const void * pUser) = 0;
// };
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------



//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------
//请注意:该接口尽可能快的发数据给客户端//强烈建议只使用该接口下载到本地
// class CXDownloadHisFileInterface
// {
// public:
// 	virtual int XDownloadHisFileDelete() = 0;
// 
// public:
// 
// 	//------------
// 	//短链接,阻塞的
// 	virtual int XDownloadHislogListFromMedia(int nDevID, int lChannel, XSERVERTIME szBeginTime, XSERVERTIME szEndTime, vector<HIS_LOG_T> &v) = 0;
// 	//nHislogid不为-1.则服务器忽略后面的szBeginTime，szEndTime，，否则按时间段下载录像
// 	virtual int XDownloadHislogFromMedia(int nDevID, int lChannel, int nHislogid, XSERVERTIME szBeginTime, XSERVERTIME szEndTime, CXDownloadHisFileCallback* pDownloadObj = NULL, void *pUser = NULL) = 0;
// 	//------------
// 
// 
// 
// 	//------------
// 	//
// 	virtual int XDownloadGetHislogListFromDevice(int lChannel, XSERVERTIME szBeginTime, XSERVERTIME szEndTime, vector<HIS_LOG_T> &v) = 0;
// 	virtual int XDownloadHisFileFromDevice(const char* szFile, int val1, int val2, int val3, CXDownloadHisFileCallback *pProgressObj = NULL, void *pUser = NULL) = 0;
// 	//
// 	//------------
// 	virtual int XDownloadHisFileClose() = 0;
// };
// class CXDownloadHisFileCallback
// {
// 	//消息回调接口
// public:
// 	//回调CXDownloadHisFileInterface指针给你
// 	//len: -1表示结束
// 	virtual int OnDownloadHisFileCallback(const char *data, int len, const void * pUser) = 0;
// };
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------




//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------
class CXTalkInterface
{
public:
	virtual int XTalkDelete() = 0;
public:
	virtual int XTalkOpen() = 0;
	virtual int XTalkClose() = 0;
	virtual int XTalkInputData(const char * szBuffer ,int nLen) = 0;
};
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------






//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------
//非常重要：
//对于平台：：暂时只支持主动注册平台的自己的私有协议的设备，包括ipc，中控等
class CXTranTransmissionInterface
{
public:
	virtual void DeleteTranTransmission() = 0;
public:
	//打开
	virtual int OpenTranTransmission(int nChannel) = 0;
	//关闭
	virtual int CloseTranTransmission() = 0;
	//透明传输
	//提供给第三方智能设备使用，比如：智能家居，报警器等等。。。
	virtual int TransparentTransmission(const  char *  szChanParam, int nlen) = 0;
};



// 
// 
// class CXPlayInterface
// {
// 	//派生类请记住自己的通道号码
// 	//nChannel 当连接流媒体服务器时：该参数表示数据库的设备ID，  当连接某台设备（NVR / IPC...）:该参数表示通道号码(默认从0开始)
// 
// public:
// 	// 	描述:	上层应用调用该接口销毁设备对象
// 	virtual int Delete() = 0;
// 
// 
// public:
// 	//以下功能只能有一个起作用:OpenRealtime,DownloadHisFromMedia,OpenLocal
// 
// 
// 
// #ifdef _WINDOWS_
// 	//描述:	播放实时视频
// 	//hWnd可以为NULL
// 	//CXDecVideoData与CXStreamData只回调一个
// 	//CXStreamData优先调用
// 	//当CXStreamData不为NULL时，即使CXDecVideoData不为NULL也不调用
// 	virtual int OpenRealtime(int nChannel, HWND hWnd, int nLinkMode/* 0:MAIN CHANNEL ,1:SUB CHANNEL */, CXStreamData* pDataObj = NULL, CXDecVideoData* pDecDataObj = NULL, void *pUser = NULL) = 0;
// 	//
// 	virtual int OpenLocal(HWND hWnd, const char* szFile, XSERVERTIME &nStartTime/*[out]*/, XSERVERTIME &nEndTime/*[out]*/, CXStreamData* pDataObj = NULL, CXDecVideoData* pDecDataObj = NULL, CXProgressCallback *pProgressObj = NULL, void *pUser = NULL) = 0;
// #endif
// 
// #if (defined _PLATFORM_ANDROID) || (defined _PLATFORM_IOS) || (defined _PLATFORM_WINDOW_WIN10_UWP)
// 	//CXDecVideoData与CXStreamData只回调一个
// 	//CXStreamData优先调用
// 	//当CXStreamData不为NULL时，即使CXDecVideoData不为NULL也不调用
// 	virtual int OpenRealtimeEx(int nChannel, int nLinkMode, CXDecVideoData* pDecDataObj, CXStreamData* pDataObj = NULL) = 0;
// 	virtual int OpenLocalEx(const char* szFile, XSERVERTIME &nStartTime/*[out]*/, XSERVERTIME &nEndTime/*[out]*/,CXDecVideoData* pDecDataObj, CXStreamData* pDataObj = NULL,  CXProgressCallback *pProgressObj = NULL, void *pUser = NULL) = 0;
// #endif
// 
// 
// 
// 	virtual int PlayCtrl(int nType /*PLAY_CTRL_TYPE*/, int val) = 0;
// 
// 
// 	// 	描述:	关闭播放(ALL)
// 	virtual int Close() = 0;
// 
// 	//--------------------------------------------------------------------
// 	// 	描述:	通道拍照，在流中截取图片
// 	//	建议:szFilePath由以下组成：用户选择的根目录+后额外加上Pictures目录+日期目录+设备名称目录//方便查找
// 	//	请确保:szFilePath,szFileName不包含特殊字符
// 	//--------------------------------------------------------------------
// 	virtual int CapturePictures(const char* szFilePath, const char* szFileName) = 0;
// 
// 	//--------------------------------------------------------------------
// 	// 	描述:	通道录像
// 	//	szFilePath=用户选择的根目录
// 	//	录像路径会在szFilePath后自动加上RecordFiles目录+日期目录%04d%02d%02d+szDevName设备名称目录//方便查找
// 	//	如果szFilePath改变，改变前，请立即停止所有录像，然后重新启动
// 	//	请确保szFilePath，szDevName正确，不包含特殊字符
// 	//	由于绝大多数情况下帧率是不准的所以使用绝对时间录像
// 	//	当然如果你录像的时候修改了系统时间，肯定有问题,所以不允许用户大范围调整时间
// 	//	也可以使用相对时间录像，当当前时间与上一帧的时间差<0或者>1s的时候(系统时间被改了)，写个固定值例如40ms，以后改
// 	//--------------------------------------------------------------------
// 	virtual int ChannelRecord(const char* szFilePath, const char *szDevName, int nSec, int nSpace, int nFreeSpace, int nAutoRemain, BOOL bStart) = 0;
// 
// 
// };












// #ifdef _WINDOWS_
// //创建设备对象输出接口函数
// typedef CXDeviceInterface* (WINAPI *XCreateDeviceInterfaceFunction)();
// 
// typedef int (WINAPI *XDeviceInterfaceInitSDKFun)();
// typedef int (WINAPI *XDeviceInterfaceUinitSDK)();
// 
// 
// //---------------------------------------------------------------------------------------------------------------
// //SRAJW
// typedef void (*SRAJWvP2PReceiveAlarmMesg)(DWORD dwSourceID, DWORD dwAlarmType, DWORD dwAlarmGroup, DWORD dwAlarmChannel);
// typedef void (*SRAJWvRecvMessage)(DWORD dwSrcID, void *pMesg, DWORD dwMesgSize);
// typedef int (WINAPI *XDeviceInterfaceInitSDKFun4Srajw)(char *, char *, SRAJWvP2PReceiveAlarmMesg, SRAJWvRecvMessage);
// //---------------------------------------------------------------------------------------------------------------
// 
// // 创建设备对象声明
// CXDeviceInterface* WINAPI CreateDevice();
// 
// // 获取底层设备接口版本号
// char* WINAPI GetDevInterfaceVersion();
//#endif





#endif
