#ifndef __DATATYPE_SERVER_H_23KLHWQ38D2373245__
#define __DATATYPE_SERVER_H_23KLHWQ38D2373245__

#include <stdint.h>

//---------------------------------------------------------------------------------------------------------------------------
//
//----------���ͷ�ļ���<<���ֿͻ���/��ý��������� ƽ̨���ķ�����>> <<MediaInterface����ý�������>>ͨѶʹ��
//
//----------��ע��ṹ�岻Ҫʹ��std::tm��android��windows���ǲ�ͬ��
//
//---------------------------------------------------------------------------------------------------------------------------


//��Ϣ����
enum emCmdOfServer
{
	//---------------------------------------------------------------------------------------------------------------
	//�û���½����
	//1:�����ȷ���NEED_SESSIONID
	//2:LOGIN
	Z_CMD_OF_SERVER_LOGIN_NEED_SESSIONID = 0,//��ת����������ͻ��˷���
	Z_CMD_OF_SERVER_LOGIN,
	Z_CMD_OF_SERVER_LOGOUT,

	Z_CMD_OF_SERVER_HEARBEAT,//��������

	Z_CMD_OF_SERVER_OPEN_STREAM,			//����ʵʱ�� //�����������ػ�Ӧ
	Z_CMD_OF_SERVER_CLOSE_STREAM,			//����ʵʱ�� //�����������ػ�Ӧ

	Z_CMD_OF_SERVER_GET_STORE_HISTORY_LOG,	//��ȡ¼���б�,��ʹ�ö����ӵķ�ʽ��ȡ  client->media
	Z_CMD_OF_SERVER_OPEN_HIS_STREAM,		//����	//�����������ظ�cmd�����ص���Z_CMD_OF_SERVER_AV_STREAM����ע���ж�result����_CMD_OF_SERVER_RESULT_DOWNLOAD_XXXX
	Z_CMD_OF_SERVER_CLOSE_HIS_STREAM,		//�ر�����

	Z_CMD_OF_SERVER_GET_DEVICE_LIST,		//�����ķ�������ȡ�豸�б�,��ʹ�ö����ӵķ�ʽ��ȡ(����������) (ͬʱ����������������Ƶ) c->S

	//---------------------------------------------------------------------------------------------------------------
	Z_CMD_OF_SERVER_AV_STREAM,				//��ʾ��ǰ�����������Ƶ����
	Z_CMD_OF_SERVER_TRANS_TRANSMISSION,		//͸������ָ��


	//---------------------------------------------------------------------------------------------------------------
	//smartlife
	Z_CMD_OF_SERVER_SMARTLIFE_ACCOUNT_REGISTER,			//��ƽ̨����ע�ᣨ�����ӵķ�ʽ��
	Z_CMD_OF_SERVER_SMARTLIFE_ACCOUNT_CHANGE_PWD,		//�޸����루�����ӵķ�ʽ��
	Z_CMD_OF_SERVER_SMARTLIFE_ACCOUNT_ADD_DEVICE,		//��ƽ̨�е�ĳ��ID��Ϊ��ǰ�˺ſ��ã������ӵķ�ʽ��
	Z_CMD_OF_SERVER_SMARTLIFE_ACCOUNT_DELETE_DEVICE,	//��ƽ̨�е�ĳ��ID�ӵ�ǰ�˺��Ƴ��������ӵķ�ʽ��
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
	_CMD_OF_SERVER_RESULT_SMARTLIFE_REGISTER_ERROR,//smartlife  �û����Ѿ�����

	_CMD_OF_SERVER_RESULT_DOWNLOAD_START, //���ؿ�ʼ��־
	_CMD_OF_SERVER_RESULT_DOWNLOAD_END,//���ؽ�����־
	_CMD_OF_SERVER_RESULT_DOWNLOAD_DATA,//���ݱ�־
	_CMD_OF_SERVER_RESULT_DOWNLOAD_ERROR,//�����־
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
//��Ϣͷ
struct ST_SERVER_HEAD
{
	//------
	uint16_t cmd;		//emCmdOfServer
	uint16_t result;	//emCmdResultOfServer
	uint16_t seq;		//����
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
	uint16_t	nAVFrameType;			//֡�������� //emXSERVER_FRAME_TYPE_DEFINE
	//video
	uint16_t	nVideoFrameRate;	//֡��
	uint16_t	nVideoWidth;
	uint16_t	nVideoHeight;
	//audio
	//nAudioBitRate��nAudioBitsPerSample�ڲ�ͬ�ĵ��������п���ֻ��һ�֣������ǿ��Լ������һ����
	uint16_t	nAudioChannels;		//ͨ����: ������Ϊ 1, ������Ϊ 2
	uint16_t	nAudioSamplesRate;	// nAudioSamplesPerSec //ÿ��Ĳ�����������������//8000,44100��
	uint16_t	nAudioBitRate;		//���ݴ����ƽ������(ÿ����ֽ���)  �Զ����ֵ
	uint16_t	nAudioBitsPerSample;//������С(�ֽ�)����16 
	//------
	uint32_t session;
	uint32_t datalen;
};

struct ST_SREVER_LOGIN_INFO
{
	int	dev_id;			//�豸�����ݿ��ID
	int dev_channel;	//һ����0����ʹ��NVR��ÿ��ͨ������һ������ID
	char user[32];
	char pwd[256];		// ʹ��des����
};

//--------- �ò�����ƽ̨ͨѶʹ��
struct ST_SREVER_DEVICE_INFO_BASE
{
	int dev_is_publish;			//0:˽���豸  1��publish device
	//
	int	dev_id;					//�豸�����ݿ��id
	int	dev_areaid;				//����id
	int dev_parent_areaid;		//����id�ĸ�id
	//
	char dev_name[64];			//�豸����
	char dev_area_name[64];		//��������
	char dev_media_ip[16];		//���ķ�����ip
	char dev_publish_ip[16];	//�ַ�������ip
	//
	int	dev_media_port;			//���ķ�����Ԥ���˿�
	int dev_playback_port;		//���ķ������طŶ˿�
	int dev_publish_port;		//�ַ�������Ԥ���˿�
	//
	int dev_browse_num;			//����û�����
	int dev_online;				//�Ƿ�����

	int reserve[3];
};

//����ʷ��Ƶ
struct ST_SREVER_OPEN_HIS_STREAM
{
	_XSERVER_TIME	tmBeginTime;
	_XSERVER_TIME	tmEndTime;
};

//��ȡ��ĳһʱ��ε���ʷ��Ƶ
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
	ST_SREVER_HIS_LOG t[128];//��෵��128���ļ�
};



struct ST_SREVER_SMARTLIFE_CHANGEPWD
{
	char newpwd[256];		// ʹ��des����,����ʹ�ö����ӣ��ڵ�½ʱ�Ѿ���֤��ԭʼ����
};
struct ST_SREVER_SMARTLIFE_DEVICE
{
	char devuuid[32];
};






#endif