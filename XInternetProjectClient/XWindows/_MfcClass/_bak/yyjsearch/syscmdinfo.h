#ifndef _SYS_CMD_INFO_H_
#define _SYS_CMD_INFO_H_




#pragma pack(1)


// enum emFREAM_TYPE_DEFINE_111111
// {
// 	//H264
// 	FREAM_TYPE_H264_IFRAME = 0,
// 	FREAM_TYPE_H264_PFRAME,
// 	FREAM_TYPE_H264_OTHERS,
// 	//H265
// 	FREAM_TYPE_H265_IFRAME,
// 	FREAM_TYPE_H265_PFRAME,
// 	FREAM_TYPE_H265_OTHERS,
// 	//MPEG4
// 	FREAM_TYPE_MPEG4,
// 	//MJPEG
// 	FREAM_TYPE_MJPEG,
// 	//Audio
// 	FREAM_TYPE_ADPCM,
// 	FREAM_TYPE_G711A,
// 	FREAM_TYPE_G711U,
// 	FREAM_TYPE_G711UZYX,
// };



#define MAGIC_NUM 0XAA55
#define BROADCASTSEARCHPORT	9795

enum _X_CMD_DEFINE
{
	_X_CMD_LOGIN = 0,					//��½
	_X_CMD_LOGIN_KEY,					//key s -> c
	_X_CMD_HEARBEAT,					//����
	_X_CMD_LOGOUT,						//ע��
	_X_CMD_OPEN_MAIN_STREAM,	//��ʵʱ��
	_X_CMD_CLOSE_MAIN_STREAM,	//�ر�ʵʱ��
	_X_CMD_OPEN_SUB_STREAM,		//��ʵʱ��(������)
	_X_CMD_CLOSE_SUB_STREAM,	//�ر�ʵʱ��(������)
	_X_CMD_STREAM,
	

	_X_CMD_OPEN_TALKBACK,			//�򿪶Խ�
	_X_CMD_DATA_TALKBACK,
	_X_CMD_CLOSE_TALKBACK,		//�رնԽ�
	_X_CMD_ALARM_UPLOAD,			//�澯�ϱ�
	_X_CMD_SNAP_UPLOAD,				//ͼƬ�ϴ�
	
	_X_CMD_GET_USERPASSWORD,	//�û������� authparam_t
	_X_CMD_SET_USERPASSWORD,
	_X_CMD_GET_ENCODEPARAM,		//������� encodeparam_t
	_X_CMD_SET_ENCODEPARAM,
	_X_CMD_GET_IMAGEPARAM,		//ͼ����� imageparam_t
	_X_CMD_SET_IMAGEPARAM,
	_X_CMD_GET_WIREDNETWORK,	//���߲��� wirednetwork_t
	_X_CMD_SET_WIREDNETWORK,
	_X_CMD_GET_SYSTIMEPARAM,	//ϵͳʱ����� systime_t
	_X_CMD_SET_SYSTIMEPARAM,
	_X_CMD_GET_OSDPARAM,			//OSD���� osdparam_t
	_X_CMD_SET_OSDPARAM,
	_X_CMD_GET_DDNSPARAM,			//DDNS���� ddnsparam_t
	_X_CMD_SET_DDNSPARAM,
	_X_CMD_GET_AUDIOPARAM,		//��Ƶ���� audioparam_t
	_X_CMD_SET_AUDIOPARAM,
	_X_CMD_GET_MOTIONDETECT,	//�ƶ������� motiondetect_t
	_X_CMD_SET_MOTIONDETECT,
	_X_CMD_GET_ALARMINPARAM,	//�澯������� alarmin_t
	_X_CMD_SET_ALARMINPARAM,
	_X_CMD_GET_ALARMOUTPARAM,	//�澯������� alarmout_t
	_X_CMD_SET_ALARMOUTPARAM,
	_X_CMD_GET_ALARMPARAM,	//�澯���� alarm_all
	_X_CMD_SET_ALARMPARAM,
	_X_CMD_GET_WIFIPARAM,			//WIFI���� wifiparam_t
	_X_CMD_SET_WIFIPARAM,
	_X_CMD_GET_SERIALPARAM,		//���ڲ��� serialparam_t
	_X_CMD_SET_SERIALPARAM,
	_X_CMD_GET_PTZCONTROL,		//PTZ���� ptzcontrol_t
	_X_CMD_SET_PTZCONTROL,
	_X_CMD_GET_VERSIONINFO,		//ϵͳ�汾��Ϣ versioninfo_t
	_X_CMD_SET_VERSIONINFO,
	_X_CMD_GET_WANPLATFORM,
	_X_CMD_SET_WANPLATFORM,
	_X_CMD_TRANS = 48,//͸������

	//20170503 -start
	_X_CMD_SEACH_TOOL,
	_X_CMD_SET_FACTORY_PARAM,
	_X_CMD_UPDATE_START,
	_X_CMD_UPDATE_DATA,
	_X_CMD_UPDATE_END,
	//20170503 - end



	//v2.0 start -----------
	_X_CMD_GET_ALLPARAM_V2 = 1000,			//��ȡv2�汾���в���	��������==============������get������
	_X_CMD_SET_ADMIN_PASSWORD_V2,			//���ù���Ա���룬�豸ҪУ��
	_X_CMD_SET_ENCODE_MAIN_BPS_V2,			//��������������С���豸Ĭ��512kbps
	_X_CMD_SET_ENCODE_FILP_V2,		//��ת��0~3
	_X_CMD_SET_IMAGE_SATURATION_V2,		//���Ͷ� 0-64
	_X_CMD_SET_IMAGE_BRIGHTNESS_V2,		//����0-64
	_X_CMD_SET_IMAGE_CONTRAST_V2,			//�Աȶ�0-64
	_X_CMD_SET_NETWORK_V2,					//�Ƿ�����DHCP+ip��ַ��v2�汾ֻ֧���豸����������֧�ֶ�������ip�����豸���Լ�����������������أ�dnsĬ��8.8.8.8����
	_X_CMD_SET_WIFI_V2,					//WIFI
	_X_CMD_SET_SYSTIME_TIMEZONE_V2,		//ʱ��
	_X_CMD_SET_SYSTIME_NTPSERVER_V2,		//ntp��������ַ������������豸���ַ���Ϊ�գ����豸�Զ���_sys_time_t->ntp_enabled ����Ϊ0
	_X_CMD_SET_OSD_DEVICENAME_V2,			//�豸���ƣ�_osd_param_t->titleenableĬ���ұ���Ϊ1�����ַ��豸����ʾ
	_X_CMD_SET_ALARM_ALL_V2,				//������alarm_all�����е�enable������ ����/���� ��Ĭ�ϣ����û����ߵ�ʱ���Լ�ע��
	_X_CMD_SET_X_PLATFORM_V2,				//ƽ̨����

	_X_CMD_SET_ENCODE_SUB_BPS_V2,			//��������������С
	_X_CMD_SET_ENCODE_SUB_SIZE_V2,			//0��d1 1��cif
	//v2.0 end -----------

	_X_CMD_SUPPORT_END
};

enum _X_COMPANY_DEFINE
{
	_X_COMPANY_MYCOM = 0,
	_X_COMPANY_HIK,
	_X_COMPANY_DH,
	_X_COMPANY_ZYX,
	_X_COMPANY_TST,
	_X_COMPANY_JYLP2P,
};

enum _X_CMD_RESULT
{
	_X_CMD_RESULT_OK = 0,
	_X_CMD_RESULT_ERROR_PWD,
	_X_CMD_RESULT_ERROR
};

typedef struct _head_t
{
	unsigned int magicnum;
	unsigned int cmd;		// _X_CMD_SNAP_UPLOAD ֮������� (_X_CMD_TRANS) ���һ���ٸ�һ��_head_t_sub����
	unsigned int session;
	unsigned int seq;

	long long tick;//�豸��Ҫʹ��Ӳ��ʱ�䱣֤����ֵһֱ�ǵ�����

	unsigned int result;	//_X_CMD_RESULT_OK ��������Ӧ������ͻ�����Ҫ���
	unsigned int datalen;
	unsigned int dataextern;
	unsigned int reserve[9];
}rqst_head;//request head
struct _head_t_sub
{
	unsigned int cmd;		//_X_CMD_DEFINE
	unsigned int datalen;
};


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


struct _login_t
{
	char user[64];
	char pwd[128]; // ʹ��des����
};

typedef struct _authority_t
{
	unsigned char admin_user[32];		//username
	unsigned char admin_password[16];	//password 

	unsigned char guest_user[32];		//username
	unsigned char guest_password[16];	//password 

}authparam_t;


//-------------------------------------------------
typedef struct _sys_encode_param
{
	//unsigned int streamid;		//0:main stream; 1:sub stream	
	unsigned int encode_type; //0: none, 1: H.264, 2: MJPEG, 3: H265
	unsigned int encode_fps;	//0-30
	unsigned int brcmode;		//brc mode, 0: CBR; 1: VBR; 2: CBR keep quality; 3: VBR keep quality //
	unsigned int cbravgbps;		//cbr avg bps
	unsigned int encode_size;	//0��1080p 1��960P 2��720P
	unsigned int flip_rotate;	//0~3

	unsigned int sub_encode_type; //0: none, 1: H.264, 2: MJPEG, 3: H265
	unsigned int sub_encode_fps;	//0-30
	unsigned int sub_brcmode;		//brc mode, 0: CBR; 1: VBR; 2: CBR keep quality; 3: VBR keep quality
	unsigned int sub_cbravgbps;		//cbr avg bps
	unsigned int sub_encode_size;	//0:D1;1:CIF
	unsigned int sub_flip_rotate;	//0~3

	unsigned int reserve[8];
}encodeparam_t;
//-------------------------------------------------
typedef struct _sys_image_param
{
	unsigned int awb_mode;				//0~12
	unsigned int saturation;			//0~100(25)////-------------------------------------------------
	unsigned int brightness;			//0~100(50)//-------------------------------------------------
	unsigned int hue;							//0~30(15)//-------------------------------------------------
	unsigned int contrast;				//0~100(25)//-------------------------------------------------
	unsigned int sharpness;				//0~100(50)//-------------------------------------------------
	unsigned int antiflickermode;	//anti flicker mode,0=50HZ 1=60HZ //-------------------------------------------------
	unsigned int shuttertime;			//shutter time max,512000000 /{1,2,3,4,5,10,15,20,25,30,50,60,100,120,200,240,480,500,960,1000,2000,5000,10000,15000,20000};
	unsigned int daynightmode;			//day night mode, 0: diable, 1: enable 2: auto
	unsigned int backlightcompenable;	//back light comp,0-disable, 1-enable
	unsigned int slowshutterenable;		//slow shutter enable,1=enabled 0=disabled
	unsigned int reserve[8];
}imageparam_t;

typedef struct _wired_network_t
{
	unsigned int dhcp_enabled;	//0: diable, 1: enable
	unsigned char ipaddr[16];
	unsigned char netmask[16];
	unsigned char gateway[16];
	unsigned char dns[16];
	unsigned int http_port;
	unsigned int multicast_enable; //0: diable, 1: enable
	unsigned int onvif_enabled;
	unsigned int reserve[8];
}wirednetwork_t;

typedef struct _sys_time_t
{
	unsigned int ntp_enabled;				// 0: diable, 1: enable//-------------------------------------------------
	unsigned char ntp_server[64];// -------------------------------------------------
  	unsigned int ntp_timezone;// -------------------------------------------------39600 === -39600
	unsigned int daylight_time;// ����ʱ

	unsigned int timenow;
	unsigned int reserve[8];
}systime_t;

typedef struct{
	// text area box parameters
	unsigned int start_x;					// text area offset x, 0~100, 100 means 100% of encode width
	unsigned int start_y;					// text area offset y, 0~100, 100 means 100% of encode height
} osdpos_t;
//-------------------------------------------------
typedef struct _osd_param_t
{
	unsigned int timeenable;		//0: diable, 1: enable
	unsigned int timetype;			//time type, 0~2
	osdpos_t	 time;
	unsigned int titleenable;		//0: diable, 1: enable
	unsigned char titlestr[64];
	osdpos_t	 title;
	unsigned int reserve[8];
}osdparam_t;

typedef struct _ddns_param_t
{
	unsigned int ddnsenable;				//( 0:disable 1:enable }
	unsigned char service_type[16]; //service type
	unsigned char server_ip[16];
	unsigned int server_port;
	unsigned char domainName[64];
	unsigned char user[32];
	unsigned char pwd[16];
	unsigned int reserve[8];
}ddnsparam_t;

typedef struct _audio_param_t
{
  unsigned int audioenable;			//( 0:disable 1:enable }//-------------------------------------------------
	unsigned int samplingrate;		//8000;16000;24000;32000;48000
	unsigned int encodetype;			//0:G711A 1:G711U 2:AAC
	unsigned int audioinvolume; 
	unsigned int audiooutvolume; 
	unsigned int reserve[8];
}audioparam_t;
//-------------------------------------------------
typedef struct _motion_detect_t
{
	unsigned int mdenable;/*0-�رգ�1-��*/
	unsigned int mdsensitivity; /*4���ȼ�:0-�ͣ�1-�У�2-�ϸߣ�3:��*/
	unsigned char mdregion[64];/*(ֻ��40���ֽ���Ч)�������ã�20 * 16 ��������40���ֽڱ�ʾ��ÿλ��ʾһ������*/
	unsigned int reserve[8];
}motiondetect_t;
//-------------------------------------------------
typedef struct _alarm_in_t
{
	unsigned int inenable;	/*0-�رգ�1-��*/
	unsigned int instate; 	/*0-���գ�1-����*/
	unsigned int reserve[8];
}alarmin_t;
//-------------------------------------------------
typedef struct _alarm_out_t
{
	unsigned int outenable;	/*0-�رգ�1-��*/
	unsigned int outstate; 	/*0-���գ�1-����*/
	unsigned int reserve[8];
}alarmout_t;

struct alarm_all
{
	motiondetect_t motion;
	alarmin_t alarmin;
	alarmout_t alarmout;
};
//-------------------------------------------------
typedef struct _wifi_param_t
{
	unsigned int  wifi_enabled;
	unsigned char ssid[32];	
	unsigned char password[32]; 	
	unsigned int reserve[8];
}wifiparam_t;

typedef struct _alarm_upload_t
{
	unsigned int alarmtype;	
	unsigned int reserve;
}alarmupload_t;

typedef struct _snap_upload_t
{
	unsigned char data[0];
}snapupload_t;

typedef struct _serial_param_t
{
    unsigned int baudrate;/*�����ʣ�0-1200��1-2400��2-4800��3-9600*/
    unsigned int databit; /*����λ��0-8��1-7��2-6��3-5*/
    unsigned int stopbit;/*ֹͣλ��0-1��1-2*/
    unsigned int check;/*У�飬0-None��1-Odd��2-Even��3-Mark��4-Space*/
    unsigned int reserve[8];
}serialparam_t;

//-------------------------------------------------
enum _PTZ_CMD_DEFINE
{
	PTZ_CMD_START=0,
	PTZ_CMD_UP,
	PTZ_CMD_DOWN,
	PTZ_CMD_LEFT,
	PTZ_CMD_RIGHT,
	PTZ_CMD_LEFTUP,
	PTZ_CMD_LEFTDOWN,
	PTZ_CMD_RIGHTUP,
	PTZ_CMD_RIGHTDOWN,
	PTZ_CMD_STOP,
	PTZ_CMD_SETPRESET,
	PTZ_CMD_CLEARPRESET,
	PTZ_CMD_CALLPRESET,
	PTZ_CMD_PRESETSCAN,
	PTZ_CMD_LEVELAUTOSCAN,
	PTZ_CMD_VERTICALAUTOSCAN,
	PTZ_CMD_END,
};

typedef struct _ptz_control_t
{
  unsigned int cmd;
  unsigned int data; 
  unsigned int reserve[8];
}ptzcontrol_t;
//-------------------------------------------------
typedef struct _version_info_t
{
	unsigned char ubootver[32];
	unsigned char kernelver[32];
	unsigned char rootfsver[32];
	unsigned char devicetype[32];
	unsigned char webver[32];	// wangye banben
	unsigned char deviceid[32]; // xuliehao
	unsigned char reserve[128]; // baoliu
}versioninfo_t;



/*
typedef struct _audio_output_t
{
    unsigned int audiooutvolume;	
	unsigned int samplingrate;		//8000;16000;24000;32000;48000
	unsigned int encodetype;		//0:G711A 1:G711U
}audiooutput_t;
*/


//=============================================================

//uuid���벻һ��
//uuid���һ��������ʱֻ��ʾһ������һ��ʱ���������豸���mac�Ƿ����ظ�
//des���ܵ�
typedef struct _uuid_t
{
	char uuid[128];
	char mac_wired[128];
}uuidparam_t;

//=============================================================
//v2�汾�ṹ�忪ʼ --- 20170505

//_X_CMD_SET_ADMIN_PASSWORD_v2 = 1000,	//���ù���Ա���룬�豸ҪУ��
struct _admin_pwd_v2
{
	
	unsigned char oldpwd[16];	//�û������������ֵ����Ϊ�п���������͵���Ѿ���½���豸
	unsigned char newpwd[16];
};

//_X_CMD_SET_ENCODE_MAIN_BPS_v2,			//��������������С���豸Ĭ��768kbps
struct _enc_main_bps_v2
{
	
	unsigned int bps;
};

//_X_CMD_SET_ENCODE_MAIN_FILP_v2,		//��������ת��0~3
struct _enc_filp_v2
{
	
	unsigned int filp;
};

struct _enc_sub_bps_v2
{

	unsigned int bps;
};
struct _enc_sub_size_v2
{

	unsigned int size;
};



//_X_CMD_SET_IMAGE_SATURATION_v2,		//���Ͷ� 0-64
struct _image_staturtion_v2
{
	
	unsigned int staturtion;
};

//_X_CMD_SET_IMAGE_BRIGHTNESS_v2,		//����0-64
struct _image_brightness_v2
{
	
	unsigned int brightness;
};

//_X_CMD_SET_IMAGE_CONTRAST_v2,			//�Աȶ�0-64
struct _image_contrast_v2
{
	
	unsigned int contrast;
};

//_X_CMD_SET_NETWORK_v2
struct _network_v2
{
	
	unsigned int dhcp;
	unsigned char ipaddr[32];
};

//_X_CMD_SET_WIFI_v2,					//WIFI
struct _wifi_v2
{
	
	unsigned char ssid[32];
	unsigned char pwd[32];
};

//_X_CMD_SET_SYSTIME_TIMEZONE_v2,		//ʱ��
struct _systime_timezone_v2
{
	
	int timezone; //������int
};

//_X_CMD_SET_SYSTIME_NTPSERVER_v2,		//ntp��������ַ������������豸���ַ���Ϊ�գ����豸�Զ���_sys_time_t->ntp_enabled ����Ϊ0
struct _systime_ntpserver_v2
{
	
	unsigned char ntpserver[128];//
};

//_X_CMD_SET_OSD_DEVICENAME_v2,			//�豸���ƣ�_osd_param_t->titleenableĬ���ұ���Ϊ1�����ַ��豸����ʾ
struct _osd_devicename_v2
{
	
	unsigned char devicename[64];
};

//_X_CMD_SET_X_PLATFORM_v2,				//ƽ̨����
struct _platform_v2
{
	
	unsigned char ip[64];//pv4��ipv6������
	unsigned int port;
	unsigned int id;
};

//_X_CMD_SET_ALARM_ALL_v2,				//������alarm_all�����е�enable������ ����/���� ��Ĭ�ϣ����û����ߵ�ʱ���Լ�ע��
struct _alarm_all_v2
{
	
	unsigned int enable;
};



struct _allparam_v2
{
	_enc_main_bps_v2		enc_bps;
	_enc_filp_v2		enc_filp;


	_image_staturtion_v2	img_sta;
	_image_brightness_v2	img_bri;
	_image_contrast_v2		img_con;

	_network_v2			net_wired;

	_wifi_v2				wifi;

	_systime_timezone_v2	time_zone;
	_systime_ntpserver_v2	time_svr;

	_osd_devicename_v2		osd_name;


	_platform_v2			plat;



	_enc_sub_bps_v2			enc_sub_bps;
	_enc_sub_size_v2		enc_sub_size;
};

//v2�汾�ṹ�����
//=============================================================






#pragma pack()










#endif

