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
	_X_CMD_LOGIN = 0,					//登陆
	_X_CMD_LOGIN_KEY,					//key s -> c
	_X_CMD_HEARBEAT,					//心跳
	_X_CMD_LOGOUT,						//注销
	_X_CMD_OPEN_MAIN_STREAM,	//打开实时流
	_X_CMD_CLOSE_MAIN_STREAM,	//关闭实时流
	_X_CMD_OPEN_SUB_STREAM,		//打开实时流(子码流)
	_X_CMD_CLOSE_SUB_STREAM,	//关闭实时流(子码流)
	_X_CMD_STREAM,
	

	_X_CMD_OPEN_TALKBACK,			//打开对讲
	_X_CMD_DATA_TALKBACK,
	_X_CMD_CLOSE_TALKBACK,		//关闭对讲
	_X_CMD_ALARM_UPLOAD,			//告警上报
	_X_CMD_SNAP_UPLOAD,				//图片上传
	
	_X_CMD_GET_USERPASSWORD,	//用户名密码 authparam_t
	_X_CMD_SET_USERPASSWORD,
	_X_CMD_GET_ENCODEPARAM,		//编码参数 encodeparam_t
	_X_CMD_SET_ENCODEPARAM,
	_X_CMD_GET_IMAGEPARAM,		//图像参数 imageparam_t
	_X_CMD_SET_IMAGEPARAM,
	_X_CMD_GET_WIREDNETWORK,	//有线参数 wirednetwork_t
	_X_CMD_SET_WIREDNETWORK,
	_X_CMD_GET_SYSTIMEPARAM,	//系统时间参数 systime_t
	_X_CMD_SET_SYSTIMEPARAM,
	_X_CMD_GET_OSDPARAM,			//OSD参数 osdparam_t
	_X_CMD_SET_OSDPARAM,
	_X_CMD_GET_DDNSPARAM,			//DDNS参数 ddnsparam_t
	_X_CMD_SET_DDNSPARAM,
	_X_CMD_GET_AUDIOPARAM,		//音频参数 audioparam_t
	_X_CMD_SET_AUDIOPARAM,
	_X_CMD_GET_MOTIONDETECT,	//移动侦测参数 motiondetect_t
	_X_CMD_SET_MOTIONDETECT,
	_X_CMD_GET_ALARMINPARAM,	//告警输入参数 alarmin_t
	_X_CMD_SET_ALARMINPARAM,
	_X_CMD_GET_ALARMOUTPARAM,	//告警输出参数 alarmout_t
	_X_CMD_SET_ALARMOUTPARAM,
	_X_CMD_GET_ALARMPARAM,	//告警参数 alarm_all
	_X_CMD_SET_ALARMPARAM,
	_X_CMD_GET_WIFIPARAM,			//WIFI参数 wifiparam_t
	_X_CMD_SET_WIFIPARAM,
	_X_CMD_GET_SERIALPARAM,		//串口参数 serialparam_t
	_X_CMD_SET_SERIALPARAM,
	_X_CMD_GET_PTZCONTROL,		//PTZ控制 ptzcontrol_t
	_X_CMD_SET_PTZCONTROL,
	_X_CMD_GET_VERSIONINFO,		//系统版本信息 versioninfo_t
	_X_CMD_SET_VERSIONINFO,
	_X_CMD_GET_WANPLATFORM,
	_X_CMD_SET_WANPLATFORM,
	_X_CMD_TRANS = 48,//透明传输

	//20170503 -start
	_X_CMD_SEACH_TOOL,
	_X_CMD_SET_FACTORY_PARAM,
	_X_CMD_UPDATE_START,
	_X_CMD_UPDATE_DATA,
	_X_CMD_UPDATE_END,
	//20170503 - end



	//v2.0 start -----------
	_X_CMD_GET_ALLPARAM_V2 = 1000,			//获取v2版本所有参数	《《《《==============！！！get！！！
	_X_CMD_SET_ADMIN_PASSWORD_V2,			//重置管理员密码，设备要校验
	_X_CMD_SET_ENCODE_MAIN_BPS_V2,			//主码流的码流大小，设备默认512kbps
	_X_CMD_SET_ENCODE_FILP_V2,		//翻转，0~3
	_X_CMD_SET_IMAGE_SATURATION_V2,		//饱和度 0-64
	_X_CMD_SET_IMAGE_BRIGHTNESS_V2,		//亮度0-64
	_X_CMD_SET_IMAGE_CONTRAST_V2,			//对比度0-64
	_X_CMD_SET_NETWORK_V2,					//是否启用DHCP+ip地址，v2版本只支持设备再内网，不支持独立外网ip，，设备请自己计算子网掩码和网关，dns默认8.8.8.8即可
	_X_CMD_SET_WIFI_V2,					//WIFI
	_X_CMD_SET_SYSTIME_TIMEZONE_V2,		//时区
	_X_CMD_SET_SYSTIME_NTPSERVER_V2,		//ntp服务器地址，，如果传到设备的字符串为空，请设备自动将_sys_time_t->ntp_enabled 设置为0
	_X_CMD_SET_OSD_DEVICENAME_V2,			//设备名称，_osd_param_t->titleenable默认且必须为1，有字符设备就显示
	_X_CMD_SET_ALARM_ALL_V2,				//代表了alarm_all的所有的enable。对于 常闭/常开 请默认，让用户接线的时候自己注意
	_X_CMD_SET_X_PLATFORM_V2,				//平台参数

	_X_CMD_SET_ENCODE_SUB_BPS_V2,			//子码流的码流大小
	_X_CMD_SET_ENCODE_SUB_SIZE_V2,			//0：d1 1：cif
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
	unsigned int cmd;		// _X_CMD_SNAP_UPLOAD 之后必须用 (_X_CMD_TRANS) 其后一定再跟一个_head_t_sub！！
	unsigned int session;
	unsigned int seq;

	long long tick;//设备需要使用硬件时间保证该数值一直是递增的

	unsigned int result;	//_X_CMD_RESULT_OK 服务器响应结果，客户不需要填充
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
	unsigned int	nVideoFrameRate;	//帧率
	unsigned int	nVideoWidth;
	unsigned int	nVideoHeight;
	unsigned int	nVideoReserve;


	//audio
	//nAudioBitRate，nAudioBitsPerSample在不同的第三方库中可能只用一种，但是是可以计算出另一个的
	unsigned int	nAudioChannels;		//通道数: 单声道为 1, 立体声为 2
	unsigned int	nAudioSamplesRate;	// nAudioSamplesPerSec //每秒的采样样本数，采样率//8000,44100等
	unsigned int	nAudioBitRate;		//数据传输的平均速率(每秒的字节数)  自定义的值
	unsigned int	nAudioBitsPerSample;//采样大小(字节)比如16 
	//
	unsigned int reserve[2];
}streaminfo_t;


struct _login_t
{
	char user[64];
	char pwd[128]; // 使用des加密
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
	unsigned int encode_size;	//0：1080p 1：960P 2：720P
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
	unsigned int daylight_time;// 夏令时

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
	unsigned int mdenable;/*0-关闭，1-打开*/
	unsigned int mdsensitivity; /*4个等级:0-低，1-中，2-较高，3:高*/
	unsigned char mdregion[64];/*(只有40个字节有效)区域设置，20 * 16 个区域，用40个字节表示，每位表示一个区域*/
	unsigned int reserve[8];
}motiondetect_t;
//-------------------------------------------------
typedef struct _alarm_in_t
{
	unsigned int inenable;	/*0-关闭，1-打开*/
	unsigned int instate; 	/*0-常闭，1-常开*/
	unsigned int reserve[8];
}alarmin_t;
//-------------------------------------------------
typedef struct _alarm_out_t
{
	unsigned int outenable;	/*0-关闭，1-打开*/
	unsigned int outstate; 	/*0-常闭，1-常开*/
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
    unsigned int baudrate;/*波特率，0-1200，1-2400，2-4800，3-9600*/
    unsigned int databit; /*数据位，0-8，1-7，2-6，3-5*/
    unsigned int stopbit;/*停止位，0-1，1-2*/
    unsigned int check;/*校验，0-None，1-Odd，2-Even，3-Mark，4-Space*/
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

//uuid必须不一致
//uuid如果一致则搜索时只显示一个，不一致时遍历已有设备检查mac是否有重复
//des加密的
typedef struct _uuid_t
{
	char uuid[128];
	char mac_wired[128];
}uuidparam_t;

//=============================================================
//v2版本结构体开始 --- 20170505

//_X_CMD_SET_ADMIN_PASSWORD_v2 = 1000,	//重置管理员密码，设备要校验
struct _admin_pwd_v2
{
	
	unsigned char oldpwd[16];	//用户必须输入这个值，因为有可能其他人偷改已经登陆的设备
	unsigned char newpwd[16];
};

//_X_CMD_SET_ENCODE_MAIN_BPS_v2,			//主码流的码流大小，设备默认768kbps
struct _enc_main_bps_v2
{
	
	unsigned int bps;
};

//_X_CMD_SET_ENCODE_MAIN_FILP_v2,		//主码流翻转，0~3
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



//_X_CMD_SET_IMAGE_SATURATION_v2,		//饱和度 0-64
struct _image_staturtion_v2
{
	
	unsigned int staturtion;
};

//_X_CMD_SET_IMAGE_BRIGHTNESS_v2,		//亮度0-64
struct _image_brightness_v2
{
	
	unsigned int brightness;
};

//_X_CMD_SET_IMAGE_CONTRAST_v2,			//对比度0-64
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

//_X_CMD_SET_SYSTIME_TIMEZONE_v2,		//时区
struct _systime_timezone_v2
{
	
	int timezone; //这里是int
};

//_X_CMD_SET_SYSTIME_NTPSERVER_v2,		//ntp服务器地址，，如果传到设备的字符串为空，请设备自动将_sys_time_t->ntp_enabled 设置为0
struct _systime_ntpserver_v2
{
	
	unsigned char ntpserver[128];//
};

//_X_CMD_SET_OSD_DEVICENAME_v2,			//设备名称，_osd_param_t->titleenable默认且必须为1，有字符设备就显示
struct _osd_devicename_v2
{
	
	unsigned char devicename[64];
};

//_X_CMD_SET_X_PLATFORM_v2,				//平台参数
struct _platform_v2
{
	
	unsigned char ip[64];//pv4，ipv6，域名
	unsigned int port;
	unsigned int id;
};

//_X_CMD_SET_ALARM_ALL_v2,				//代表了alarm_all的所有的enable。对于 常闭/常开 请默认，让用户接线的时候自己注意
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

//v2版本结构体结束
//=============================================================






#pragma pack()










#endif

