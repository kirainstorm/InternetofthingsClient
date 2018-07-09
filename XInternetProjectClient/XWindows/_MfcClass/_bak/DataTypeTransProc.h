#ifndef __DATATYPE_TRANSPROC_H_23KLHWQ3873245__
#define __DATATYPE_TRANSPROC_H_23KLHWQ3873245__



//-----------------------------------------------------------------------------
//----------���ͷ�ļ���XTransmisServer��XTransmisProc.exeͨѶʹ��
//
//�����ܹ���
//XTransmisServer��ȡ�����ļ���������ͬ��XTransmisProc.exe������֮����
//XTransmisProc.exeʹ�ò�ͬ�Ŀ����Ӳ�ͬ���豸�������͵���Ӧ�ķ�������
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
	_TRANSMIS_PROC_OF_DEVICE_CONNNETTING = 0,	//֪ͨXTransmisServer�������豸����
	_TRANSMIS_PROC_OF_DEVICE_CONNNETED,			//֪ͨXTransmisServer���豸���ӳɹ�
	_TRANSMIS_PROC_OF_DEVICE_DISCONNNETED,		//֪ͨXTransmisServer���豸�Ͽ�

	_TRANSMIS_PROC_OF_SERVER_CONNNETTING,		//֪ͨXTransmisServer���������������
	_TRANSMIS_PROC_OF_SERVER_CONNNETED,			//֪ͨXTransmisServer����������ӳɹ�
	_TRANSMIS_PROC_OF_SERVER_DISCONNNETED,		//֪ͨXTransmisServer��������Ͽ�

	_TRANSMIS_PROC_HEARBEAT, //����
};
//��Ϣͷ
struct TRANSMIN_PROC_HEAD__T
{
	int cmd;
	int result;	//0
	int session;
	int k;//������0xada2
};




//-----------------------------------------------------------------------------
//�豸���ͣ������ļҵ�sdkȡ��
enum emTRANSMIN_PROC_DEVICE_TYPE
{
	TRANSMIN_PROC_DEVICE_ME_MEDIA_TO_MEDIA = 0,		//�Լ��ķ�����sdk����������������������ĳ̨�豸�������͵�����������

	TRANSMIN_PROC_DEVICE_ME_MEDIA_TO_RTMP_HLS,		//�Լ��ķ�����sdk������������������ĳ̨�豸�������͵�RTMP/HLS������

	TRANSMIN_PROC_DEVICE_ONVIF_TO_MEDIA,			//ONVIF��������������û���õ�sdk��ʹ��onvifЭ�����͵�������

	TRANSMIN_PROC_DEVICE_YYJ_TO_MEDIA,

	TRANSMIN_PROC_DEVICE_YYJP2P_TO_MEDIA,

	TRANSMIN_PROC_DEVICE_SRAJW_TO_MEDIA,

	TRANSMIN_PROC_DEVICE_SRAXIANG_TO_MEDIA,

};


//���͵����ַ�������
enum emTRANSMIN_PROC_SERVER_TYPE
{
	TRANSMIN_PROC_SERVER_NORMAL = 0,

	TRANSMIN_PROC_SERVER_RTMP,			//nginx������
};


class XTransmisDeviceInterface
{
public:
	virtual void OpenTransmisDevice(char * p1 = NULL, char * p2 = NULL, char * p3 = NULL, char * p4 = NULL, int val1 = 0, int val2 = 0, int val3 = 0, int val4 = 0) = 0;
	virtual void CloseTransmisDevice() = 0;

	virtual BOOL GetError() = 0;//����д������˳��ó��򣨱����������ȣ���ȡ�ɵģ�����������������򵥡�
};


#endif