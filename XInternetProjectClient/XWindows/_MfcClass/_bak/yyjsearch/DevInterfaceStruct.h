#ifndef _DEVICE_INTERFACE_STRUCT_H_KJ2342789LKS3K23
#define _DEVICE_INTERFACE_STRUCT_H_KJ2342789LKS3K23


//enum�ǲ��ɿ��ģ���ͬ��ƽ̨�ĳ��ȿ��ܲ�ͬ�����Բ�����ֱ�����ڽṹ���еı������м�
//����ʹ�ù̶����ȵ��������ͣ���32λ���ݣ���ʹ��long֮���
//Լ��������int�ĺ�������0��ʾ�ɹ�������ʧ��

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



// ---------------------- �����ݽӿ�---------------------------------------------------------------------------
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
	//�������峧�̼���sdk
	X_DEVICE_TYPE_NONE = 0,
	X_DEVICE_TYPE_MEDIA,//��ý��
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
	XMSG_NET_STATUS____CONNECT_SUCCESS = 0,		//���ӳɹ�
	XMSG_NET_STATUS____CONNECT_FAIlED,			//INCLUDE : NOT CONNCTED or TIME_OUT
	XMSG_NET_STATUS____CONNECTING,				//��������
	XMSG_NET_STATUS____CONNECT_INVALID_PARAM,	//�����id���û���������
	XMSG_NET_STATUS____DEVICE_NOT_ON_LINE,		//�豸������
	XMSG_NET_STATUS____DISCONNECTED,			//����
	//-------------------------------------------------------------------------------------------
	XMSG_NET_STATUS_PREVIEW____CONNECT_SUCCESS,		//���ӳɹ�
	XMSG_NET_STATUS_PREVIEW____CONNECT_FAIlED,		//INCLUDE : NOT CONNCTED or TIME_OUT
	XMSG_NET_STATUS_PREVIEW____CONNECTING,			//��������
	XMSG_NET_STATUS_PREVIEW____DISCONNECTED,		//����
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
	XMSG_PLAT_USER_OPERATION_DATATRAFFIC_STATUS____MAXDATA,			//�豸�����㹻������������ת��
	XMSG_PLAT_USER_OPERATION_DATATRAFFIC_STATUS____MINDATA,			//�豸�����ľ��������������ֹͣת��
	//
	XMSG_PLAT_USER_OPERATION_DEVICE_STATUS____DEVICE_ADD,			//�豸����	//ÿ��<<<�ͻ���>>><<<������>>>��Ҫ�����Լ����豸�б�
	XMSG_PLAT_USER_OPERATION_DEVICE_STATUS____DEVICE_UPDATA,		//�豸�޸�	//ÿ��<<<�ͻ���>>><<<������>>>��Ҫ�����Լ����豸�б�
	XMSG_PLAT_USER_OPERATION_DEVICE_STATUS____DEVICE_DELETE,		//�豸ɾ��	//ÿ��<<<�ͻ���>>><<<������>>>��Ҫ�����Լ����豸�б�
	//
	XMSG_PLAT_USER_OPERATION_SERVER_STATUS____SERVER_ADD,			//����
	XMSG_PLAT_USER_OPERATION_SERVER_STATUS____SERVER_UPDATA,		//�޸�
	XMSG_PLAT_USER_OPERATION_SERVER_STATUS____SERVER_DELETE,		//ɾ��	//ÿ��<<<�ͻ���>>><<<������>>>��Ҫ�����Լ����豸�б�
	//
	XMSG_PLAT_USER_OPERATION_DEVICE_STATUS____DEVICESTATUS_ONLINE,	//�豸����	//ÿ��<<<�ͻ���>>><<<>>>��Ҫ�����Լ����豸�б�
	XMSG_PLAT_USER_OPERATION_DEVICE_STATUS____DEVICESTATUS_OFFLINE,	//�豸����	//ÿ��<<<�ͻ���>>><<<>>>��Ҫ�����Լ����豸�б�




	//-------------------------------------------------------------------------------------------
	XMSG_ALARM_TYPE____IO = 20000,
	XMSG_ALARM_TYPE____MOTION,
	//�������ڴ˺�����ӳ��������͵���������Ϣ����
};

// ---------------------------------------------------------------------------------------------------------------------------
//PTZMove Direction
#define				PTZ_MD_STOP							0									// ֹͣ�˶�
#define				PTZ_MD_LEFT							1									// ����
#define				PTZ_MD_RIGHT						2									// ����
#define				PTZ_MD_UP							3									// ����
#define				PTZ_MD_DOWN							4									// ����
#define				PTZ_MD_LEFT_UP						5									// ����
#define				PTZ_MD_LEFT_DOWN					6									// ����
#define				PTZ_MD_RIGHT_UP						7									// ����
#define				PTZ_MD_RIGHT_DOWN					8									// ����
#define				PTZ_MD_RESET						9									// ��λ
// PTZAction
#define				PTZ_ACTION_ZOOM_FAR					21									// ���ţ��Ŵ� 
#define				PTZ_ACTION_ZOOM_NEAR				22									// ���ţ���С
#define				PTZ_ACTION_ZOOM_STOP				23									// ֹͣ����
#define				PTZ_ACTION_FOCUS_FAR				24									// �۽�����Զ���۽�
#define				PTZ_ACTION_FOCUS_NEAR				25									// �۽����������۽�
#define				PTZ_ACTION_FOCUS_STOP				26									// ֹͣ�۽�
#define				PTZ_ACTION_AUTO_FOCUS				27									// �Զ��۽�
#define				PTZ_ACTION_AUTO_FOCUS_STOP			28									// ֹͣ�Զ��۽�
#define				PTZ_ACTION_AUTO_SCAN				29									// �Զ�ɨ��
#define				PTZ_ACTION_AUTO_SCAN_STOP			30									// ֹͣ�Զ�ɨ��
#define				PTZ_ACTION_CAMERA					31									// ��||�ر������
#define				PTZ_ACTION_IRIS						32									// ��||�رչ�Ȧ
#define				PTZ_ACTION_IRIS_STOP				33									// 
//ptzpreset
#define				PTZ_PRESET_SET_PRESET				40									//SET��̨Ԥ��λ
#define				PTZ_PRESET_GO_PRESET				41									//GOTO��̨Ԥ��λ
#define				PTZ_PRESET_CL_PRESET				42									//CLEAR��̨Ԥ��λ


// ---------------------------------------------------------------------------------------------------------------------------
//������Ϣ
#define X_PARAM_TYPE_INFO 0
// ---------------------------------------------------------------------------------------------------------------------------
//ͼ�����
#define X_PARAM_TYPE_IMAGE 2
#define X_PARAM_TYPE_IMAGE_SET_DEFAULT 3	
#define X_PARAM_TYPE_IMAGE_SATURATION 4
#define X_PARAM_TYPE_IMAGE_BRIGHTNESS 5
#define X_PARAM_TYPE_IMAGE_CONTRAST 6

// ---------------------------------------------------------------------------------------------------------------------------
//�������
#define X_PARAM_TYPE_ENCODE 10
#define X_PARAM_TYPE_ENCODE_MAIN 11 //������
#define X_PARAM_TYPE_ENCODE_SUB 12	//������
#define X_PARAM_TYPE_ENCODE_FILP 13

#define X_PARAM_TYPE_ENCODE_MAIN_BPS 14

#define X_PARAM_TYPE_ENCODE_SUB_BPS 15
#define X_PARAM_TYPE_ENCODE_SUB_SIZE 16



//��Ƶ�������
#define X_PARAM_TYPE_AUDIO 19
// ---------------------------------------------------------------------------------------------------------------------------
//ʱ������
#define X_PARAM_TYPE_GET_TIME 20
#define X_PARAM_TYPE_SET_TIME 21
#define X_PARAM_TYPE_SET_SNTP 22

#define X_PARAM_TYPE_TIME_TIMEZONE 23
#define X_PARAM_TYPE_TIME_NTPSVR	24

// ---------------------------------------------------------------------------------------------------------------------------
//�������
#define X_PARAM_TYPE_NET 30
#define X_PARAM_TYPE_NET_V2 31

// ---------------------------------------------------------------------------------------------------------------------------
//wifi����
#define X_PARAM_TYPE_WIFI 40
#define X_PARAM_TYPE_WIFI_V2 41
// ---------------------------------------------------------------------------------------------------------------------------
//osd����
#define X_PARAM_TYPE_OSD 50
#define X_PARAM_TYPE_OSD_NAME 51

// ---------------------------------------------------------------------------------------------------------------------------
//�澯����
#define X_PARAM_TYPE_ALARM 60
// ---------------------------------------------------------------------------------------------------------------------------
//�û�����
#define X_PARAM_TYPE_USER 70
// ---------------------------------------------------------------------------------------------------------------------------
//ϵͳ����
#define X_PARAM_TYPE_SYS 80
// ---------------------------------------------------------------------------------------------------------------------------
//ftp����
#define X_PARAM_TYPE_FTP 90
// ---------------------------------------------------------------------------------------------------------------------------
//email����
#define X_PARAM_TYPE_EMAIL 100
// ---------------------------------------------------------------------------------------------------------------------------
//ptz����
#define X_PARAM_TYPE_PTZ 110
// ---------------------------------------------------------------------------------------------------------------------------
//sd������
#define X_PARAM_TYPE_SDCARD 120
#define X_PARAM_TYPE_SDCARD_FORMAT 121
//����
#define X_PARAM_TYPE_SERIAL 130
//¼���ļ�
#define X_PARAM_TYPE_RECORDFILES 140
//PLAT

#define X_PARAM_TYPE_PLAT_ME 150

// ---------------------------------------------------------------------------------------------------------------------------

#define X_PARAM_TYPE_GET_DEVICE_ALL_PARAM 20000

// ---------------------------------------------------------------------------------------------------------------------------

#define X_PARAM_TYPE_GET_DEVICE_ONLINE_STATUS 3000 //��ȡ�豸����״̬�������ؿͻ����õ�
#define X_PARAM_TYPE_GET_DEVICE_PREVIEW_STATUS 3001//��ȡ������״̬  0��û��������  1��������

//#define X_PARAM_TYPE_GET_DEVICE_HISLOG_FILE 3002//��ȡ¼���ļ��б�

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
