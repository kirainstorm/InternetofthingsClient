#ifndef __DATATYPE_DEVICE_H_KJ34UIWFWMDIU23MKASDJ5__
#define __DATATYPE_DEVICE_H_KJ34UIWFWMDIU23MKASDJ5__


#include <stdint.h>

//---------------------------------------------------------------------------------------------------------------------------
//
//		���ͷ�ļ����豸�������ͨѶ�����ݶ���
//
//      ��Ϣ��_head_t + data
//
//		͸����Ϣ��_head_t(_X_CMD_TRANS) + data
//
//		�����ݣ�_head_t(_X_CMD_STREAM) +_stream_t + data
//
//---------------------------------------------------------------------------------------------------------------------------


#pragma pack(1)

typedef struct _stream_t
{
	unsigned int avType;//emFREAM_TYPE_DEFINE
	unsigned int datalen;
	//
	unsigned int	nVideoFrameRate;	//֡��
	unsigned int	nVideoWidth;
	unsigned int	nVideoHeight;
	unsigned int	nVideoReserve;


	//audio
	//nAudioBitRate��nAudioBitsPerSample�ڲ�ͬ�ĵ��������п���ֻ��һ�֣������ǿ��Լ������һ����
	unsigned int	nAudioChannels;		//ͨ����: ������Ϊ 1, ������Ϊ 2
	unsigned int	nAudioSamplesRate;	// nAudioSamplesPerSec //ÿ��Ĳ�����������������//8000,44100��
	unsigned int	nAudioBitRate;		//���ݴ����ƽ������(ÿ����ֽ���)  �Զ����ֵ
	unsigned int	nAudioBitsPerSample;//������С(�ֽ�)����16 
	//
	unsigned int reserve[2];
}streaminfo_t;

#define MAGIC_NUM 0XAA55

enum _X_CMD_DEFINE
{
	_X_CMD_LOGIN = 0,					//��½
	_X_CMD_LOGIN_NEED_SESSION,		//key s -> c
	_X_CMD_HEARBEAT,					//����
	_X_CMD_LOGOUT,						//ע��

	_X_CMD_OPEN_MAIN_STREAM,	//��ʵʱ��
	_X_CMD_CLOSE_MAIN_STREAM,	//�ر�ʵʱ��
	_X_CMD_OPEN_SUB_STREAM,		//��ʵʱ��(������)
	_X_CMD_CLOSE_SUB_STREAM,	//�ر�ʵʱ��(������)

	_X_CMD_STREAM,//��ʾ����������
	_X_CMD_TRANS = 48,//͸������,��ʾ������Ϣ����
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
	long long tick;//�豸��Ҫʹ��Ӳ��ʱ�䱣֤����ֵһֱ�ǵ�����


	unsigned int result;	//_X_CMD_RESULT_OK ��������Ӧ������ͻ�����Ҫ���
	unsigned int datalen;
	unsigned int dataextern; 
	unsigned int reserve[9];
}rqst_head;//request head

struct _login_t
{
	char user[64];
	char pwd[128]; // ʹ��des���� //�豸��ƽ̨�����к�+��ȡ��sessionidһ�����
};

#pragma pack()



#endif