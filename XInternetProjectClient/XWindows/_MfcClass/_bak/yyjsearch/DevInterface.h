#ifndef _DEVICE_INTERFACE_H_KJ2342789LKS3K23
#define _DEVICE_INTERFACE_H_KJ2342789LKS3K23


/*
//enum�ǲ��ɿ��ģ���ͬ��ƽ̨�ĳ��ȿ��ܲ�ͬ������>>>>>>> �� �� �� <<<<<<ֱ�����ڽṹ���еı������м�
���������sizeof( EType1 )���ڶ��ٵ����⣬�ǲ���ÿһ���û��Զ����ö�����Ͷ�������ͬ�ĳߴ��أ�
�ڴ������32λ�������£��磺VC++��gcc�ȣ�һ��ö�����͵ĳߴ���ʵ����һ��sizeof( int )�Ĵ�С���ѵ�ö�����͵ĳߴ���ľ�Ӧ����int���͵ĳߴ���
��ʵ���������ģ���C++��׼�ĵ���ISO14882���в�û�����������壬
��׼��������˵���ģ���ö�����͵ĳߴ������ܹ��������ö���ӵ�ֵ�������ĳߴ硱��
ͬʱ��׼��Ҳ˵���ˣ���ö�������е�ö���ӵ�ֵ����Ҫ�ܹ���һ��int���ͱ�������
*/

#include "PlatformDefine.h"
#include "XCross.h"

//��SYSTEMTIME��ͬ�Ľṹ��
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
class CXPreviewInterface;//ʵʱԤ���ӿ�
//
class CXTalkInterface;//�Խ��ӿ�
//
class CXLocalPlaybackInterface;//����Ԥ���ӿ�
class CXLocalPlaybackProgressCallback;//���ز��Ž��Ȼص�
//
//class CXOnlinePlaybackInterface;//���߻طŽӿ�
//class CXOnlinePlaybackProgressCallback;//���߻طŲ��Ž��Ȼص�
//
class CXDownloadHisFileInterface;//�����ļ��ӿ�
class CXDownloadHisFileCallback;
//
class CXTranTransmissionInterface;//͸������ӿ�



#include "DevInterfaceStruct.h"
#include "PlatformDefine.h"
#include "XPlayer.h"


//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------
class CXMessageCallback//��Ϣ�ص��ӿ�
{
public:
	virtual int OnMessageCallback(int nDevID, int nDevChannel, int nMessageType, int nMessageVal, void * pUser) = 0;
};
class CXStreamData//ԭʼ�����ݻص��ӿ�
{
public:
	virtual int OnStreamData(const char* pStreamData, int nLen, int nFrameType /*emFREAM_TYPE_DEFINE*/, int nTime, int nFramerate) = 0;
};
// class CXDecVideoData//��������ݻص��ӿ�
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
	//[��Ϊipc/nvr�豸ʱ]	--- �豸IP��ַ-�豸�˿�-�豸�û�-�豸����-P2PID
	//[������ý��ȡ��ʱ]	--- ��ý��IP��ַ-��ý��˿�-�����û�-��������-�豸ID
	virtual int XDeviceSetInfo(const char* szIP, int nPort, const char* szUser, const char* szPwd, const char* szUuid, int nVal1, int nVal2, emX_DEVICE_TYPE nType, CXMessageCallback *cb = NULL, void * pUser = NULL) = 0;
	//open
	virtual int XDeviceOpen() = 0;
	//close
	virtual int XDeviceClose() = 0;
	//��̨
	virtual int XDevicePTZControl(int nChannel, int nCtrlType, int nVal1, int nVal2 = 0, int nVal3 = 0) = 0;
	//��ȡͨ������//�ر�˵����ÿ�ҵ�CXXXDevInterface��ui��ʾ���֣�ʹ���Լ��Ĳ����ṹ��ǿתszChanParam
	virtual int XDeviceGetChannelParam(int nChannel, char * szChanParam, int& nlen, int nParamType/*X_PARAM_TYPE_XXX*/, const  char * pUserData = NULL) = 0;
	//����ͨ������
	virtual int XDeviceSetChannelParam(int nChannel, const  char *  szChanParam, int nlen, int nParamType/*X_PARAM_TYPE_XXX*/) = 0;

public:
	//ʵʱԤ��
	virtual CXPreviewInterface* CreatePreviewInstance() = 0;
	//�Խ�
	virtual CXTalkInterface* CreateTalkInstance() = 0;
	//����Ԥ��
	virtual CXLocalPlaybackInterface* CreateLocalPlaybackInstance() = 0;
	//���߻ط�
	//virtual CXOnlinePlaybackInterface* CreateOnlinePlaybackInstance() = 0;
	//�����ļ�
	virtual CXDownloadHisFileInterface* CreateDownloadHisFileInstance() = 0;
	//͸������
	virtual CXTranTransmissionInterface* CreateTranTransmissionInstanceForMediaSever() = 0;//�ṩ�������������豸ʹ�ã����磺���ܼҾӣ��������ȵ�
};
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------



//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------
//ÿ�ҵ�sdk���Լ������Զ�����
class CXPreviewInterface
{
public:
	virtual int XPreviewDelete() = 0;

public:
#ifdef _PLATFORM_WINDOW
	//hWnd����ΪNULL
	virtual int XPreviewOpenForWindows(int nChannel, HWND hWnd, int nLinkMode/* 0:MAIN CHANNEL ,1:SUB CHANNEL */, CXStreamData* pDataObj = NULL, DecodeVideoCallBack* pDecDataObj = NULL, void *pUser = NULL) = 0;
#endif
#if (defined _PLATFORM_ANDROID) || (defined _PLATFORM_IOS) || (defined _PLATFORM_WINDOW_WIN10_UWP)
	virtual int XPreviewOpenForApp(DecodeVideoCallBack* pDecDataObj = NULL, CXStreamData* pVoiceObj = NULL, void *pUser = NULL) = 0;
#endif
	//
	virtual int XPreviewClose() = 0;
	//��������
	virtual int XPreviewSound(int nOpenSound) = 0;
	//ͨ�����գ������н�ȡͼƬ//����:szFilePath��������ɣ��û�ѡ��ĸ�Ŀ¼+��������PicturesĿ¼+����Ŀ¼+�豸����Ŀ¼//�������//��ȷ��:szFilePath,szFileName�����������ַ�
	virtual int XPreviewCapturePictures(const char* szFilePath, const char* szFileName) = 0;
	//ͨ��¼��
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
	//�ص�CXLocalPlaybackInterfaceָ�����
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
//��ע��:�ýӿھ����ܿ�ķ����ݸ��ͻ���//ǿ�ҽ���ֻʹ�øýӿ����ص�����
// class CXDownloadHisFileInterface
// {
// public:
// 	virtual int XDownloadHisFileDelete() = 0;
// 
// public:
// 
// 	//------------
// 	//������,������
// 	virtual int XDownloadHislogListFromMedia(int nDevID, int lChannel, XSERVERTIME szBeginTime, XSERVERTIME szEndTime, vector<HIS_LOG_T> &v) = 0;
// 	//nHislogid��Ϊ-1.����������Ժ����szBeginTime��szEndTime��������ʱ�������¼��
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
// 	//��Ϣ�ص��ӿ�
// public:
// 	//�ص�CXDownloadHisFileInterfaceָ�����
// 	//len: -1��ʾ����
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
//�ǳ���Ҫ��
//����ƽ̨������ʱֻ֧������ע��ƽ̨���Լ���˽��Э����豸������ipc���пص�
class CXTranTransmissionInterface
{
public:
	virtual void DeleteTranTransmission() = 0;
public:
	//��
	virtual int OpenTranTransmission(int nChannel) = 0;
	//�ر�
	virtual int CloseTranTransmission() = 0;
	//͸������
	//�ṩ�������������豸ʹ�ã����磺���ܼҾӣ��������ȵȡ�����
	virtual int TransparentTransmission(const  char *  szChanParam, int nlen) = 0;
};



// 
// 
// class CXPlayInterface
// {
// 	//���������ס�Լ���ͨ������
// 	//nChannel ��������ý�������ʱ���ò�����ʾ���ݿ���豸ID��  ������ĳ̨�豸��NVR / IPC...��:�ò�����ʾͨ������(Ĭ�ϴ�0��ʼ)
// 
// public:
// 	// 	����:	�ϲ�Ӧ�õ��øýӿ������豸����
// 	virtual int Delete() = 0;
// 
// 
// public:
// 	//���¹���ֻ����һ��������:OpenRealtime,DownloadHisFromMedia,OpenLocal
// 
// 
// 
// #ifdef _WINDOWS_
// 	//����:	����ʵʱ��Ƶ
// 	//hWnd����ΪNULL
// 	//CXDecVideoData��CXStreamDataֻ�ص�һ��
// 	//CXStreamData���ȵ���
// 	//��CXStreamData��ΪNULLʱ����ʹCXDecVideoData��ΪNULLҲ������
// 	virtual int OpenRealtime(int nChannel, HWND hWnd, int nLinkMode/* 0:MAIN CHANNEL ,1:SUB CHANNEL */, CXStreamData* pDataObj = NULL, CXDecVideoData* pDecDataObj = NULL, void *pUser = NULL) = 0;
// 	//
// 	virtual int OpenLocal(HWND hWnd, const char* szFile, XSERVERTIME &nStartTime/*[out]*/, XSERVERTIME &nEndTime/*[out]*/, CXStreamData* pDataObj = NULL, CXDecVideoData* pDecDataObj = NULL, CXProgressCallback *pProgressObj = NULL, void *pUser = NULL) = 0;
// #endif
// 
// #if (defined _PLATFORM_ANDROID) || (defined _PLATFORM_IOS) || (defined _PLATFORM_WINDOW_WIN10_UWP)
// 	//CXDecVideoData��CXStreamDataֻ�ص�һ��
// 	//CXStreamData���ȵ���
// 	//��CXStreamData��ΪNULLʱ����ʹCXDecVideoData��ΪNULLҲ������
// 	virtual int OpenRealtimeEx(int nChannel, int nLinkMode, CXDecVideoData* pDecDataObj, CXStreamData* pDataObj = NULL) = 0;
// 	virtual int OpenLocalEx(const char* szFile, XSERVERTIME &nStartTime/*[out]*/, XSERVERTIME &nEndTime/*[out]*/,CXDecVideoData* pDecDataObj, CXStreamData* pDataObj = NULL,  CXProgressCallback *pProgressObj = NULL, void *pUser = NULL) = 0;
// #endif
// 
// 
// 
// 	virtual int PlayCtrl(int nType /*PLAY_CTRL_TYPE*/, int val) = 0;
// 
// 
// 	// 	����:	�رղ���(ALL)
// 	virtual int Close() = 0;
// 
// 	//--------------------------------------------------------------------
// 	// 	����:	ͨ�����գ������н�ȡͼƬ
// 	//	����:szFilePath��������ɣ��û�ѡ��ĸ�Ŀ¼+��������PicturesĿ¼+����Ŀ¼+�豸����Ŀ¼//�������
// 	//	��ȷ��:szFilePath,szFileName�����������ַ�
// 	//--------------------------------------------------------------------
// 	virtual int CapturePictures(const char* szFilePath, const char* szFileName) = 0;
// 
// 	//--------------------------------------------------------------------
// 	// 	����:	ͨ��¼��
// 	//	szFilePath=�û�ѡ��ĸ�Ŀ¼
// 	//	¼��·������szFilePath���Զ�����RecordFilesĿ¼+����Ŀ¼%04d%02d%02d+szDevName�豸����Ŀ¼//�������
// 	//	���szFilePath�ı䣬�ı�ǰ��������ֹͣ����¼��Ȼ����������
// 	//	��ȷ��szFilePath��szDevName��ȷ�������������ַ�
// 	//	���ھ�����������֡���ǲ�׼������ʹ�þ���ʱ��¼��
// 	//	��Ȼ�����¼���ʱ���޸���ϵͳʱ�䣬�϶�������,���Բ������û���Χ����ʱ��
// 	//	Ҳ����ʹ�����ʱ��¼�񣬵���ǰʱ������һ֡��ʱ���<0����>1s��ʱ��(ϵͳʱ�䱻����)��д���̶�ֵ����40ms���Ժ��
// 	//--------------------------------------------------------------------
// 	virtual int ChannelRecord(const char* szFilePath, const char *szDevName, int nSec, int nSpace, int nFreeSpace, int nAutoRemain, BOOL bStart) = 0;
// 
// 
// };












// #ifdef _WINDOWS_
// //�����豸��������ӿں���
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
// // �����豸��������
// CXDeviceInterface* WINAPI CreateDevice();
// 
// // ��ȡ�ײ��豸�ӿڰ汾��
// char* WINAPI GetDevInterfaceVersion();
//#endif





#endif
