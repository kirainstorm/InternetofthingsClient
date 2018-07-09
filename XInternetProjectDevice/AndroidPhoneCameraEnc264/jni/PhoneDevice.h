#ifndef PHONE_DEVICE_X_456W34324
#define PHONE_DEVICE_X_456W34324
#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <x264.h>

#include "VoiceHeader.h"


#define TEST_4_PLAT_LIVE 1//////////////////////////////////////////////////////////////////////////


#if TEST_4_PLAT_LIVE
#include "PlatformDefine.h"
#include "XCross.h"

#include <boost/shared_ptr.hpp>
#include <boost/shared_array.hpp>
#include <boost/pool/pool.hpp>

#pragma pack(1)

typedef struct _stream_t
{
	unsigned int avType;//emFREAM_TYPE_DEFINE
	unsigned int datalen;
	long long seq;
	long long tick;//�豸��Ҫʹ��Ӳ��ʱ�䱣֤����ֵһֱ�ǵ�����
	//video
	unsigned int	nVideoFrameRate;	//֡��
	unsigned int	nVideoWidth;
	unsigned int	nVideoHeight;
	unsigned int	nVideoReserve;


	//audio
	//nAudioBitRate��nAudioBitsPerSample�ڲ�ͬ�ĵ��������п���ֻ��һ�֣������ǿ��Լ������һ����
	unsigned int	nAudioChannels;		//ͨ����: ������Ϊ 1, ������Ϊ 2
	unsigned int	nAudioSamplesRate;	// nAudioSamplesPerSec //ÿ��Ĳ�����������������//8000,44100��
	unsigned int	nAudioBitRate;		//���ݴ����ƽ������(ÿ����ֽ���) nAudioChannels*nAudioSamplesRate*nAudioBitsPerSample/8
	unsigned int	nAudioBitsPerSample;//������С(�ֽ�)����16

	unsigned int reserve[4];
}streaminfo_t;

#define MAGIC_NUM 0XAA55

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
	_X_CMD_STREAM,//��ʾ����������
	_X_CMD_ALARM_UPLOAD = 12,
	_X_CMD_SET_PTZCONTROL = 43,
	_X_CMD_TRANS = 48,//͸������


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
	_X_CMD_RESULT_ERROR
};

typedef struct _head_t
{
	unsigned int magicnum;
	unsigned int cmd;		//_X_CMD_DEFINE
	unsigned int session;
	unsigned int seq;
	unsigned int result;	//_X_CMD_RESULT_OK ��������Ӧ������ͻ�����Ҫ���
	unsigned int datalen;
	unsigned int dataextern; //��������cmdΪ_X_CMD_GET_AREA_LIST_RESPONSE��_X_CMD_GET_DEVICE_LIST_RESPONSE��_X_CMD_GET_STORE_STRATEGY_RESPONSE
	unsigned int reserve[9];
}rqst_head;//request head

struct _login_t
{
	char user[64];
	char pwd[128]; // ʹ��des����
};

//-------------------------------------------------
enum _PTZ_CMD_DEFINE
{
	PTZ_CMD_START = 0,
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

typedef enum {
	ALARM_MOTION_DETECT, ///< Motion detection alarm.
	ALARM_ALARMIN_TRIG, ///< External trigger alarm.
	ALARM_MAX ///<  Alarm manager command number.
} _X_ALARM_TYPE;
typedef struct _alarm_upload_t
{
	unsigned int alarmtype;
	unsigned int reserve;
}alarmupload_t;
//-------------------------------------------------

#pragma pack()




#define MAX_MSG_4_PHONE_DEVICE_SESSION (2*1024*1024)
struct MyStruct4PhoneDeviceSession
{
	rqst_head head;
	char buffer[MAX_MSG_4_PHONE_DEVICE_SESSION - sizeof(rqst_head) - sizeof(int) - sizeof(int)];
	int nSendLen;
	int nType;//emEnc264Type
};
enum emPhoneDeviceNetSendStep
{
	NET_SEND_STEP_SLEEP = 0,
	NET_SEND_STEP_SEND
};

#endif

#if TEST_4_PLAT_LIVE
class CPhoneDevice :public CXNetStreamData
#else
class CPhoneDevice
#endif
{
public:
	CPhoneDevice(int a_chanel, int a_sample_rate, int a_bit_rate);
	~CPhoneDevice(void);


	//////////////////////////////////////////////////////////////////////////
public:
	void Start(int width ,int height);
	void Stop();
	void InputData(int enc264type, char * buffer, int len);

private:
	int m_a_chanel, m_a_sample_rate, m_a_bit_rate;
	CXVoiceInterface *m_pAac;

	CXVoiceInterface *m_pAacDecode;

private:
	x264_param_t * m_param;
	x264_t * m_handle;
	x264_picture_t * m_pic_in;
	x264_nal_t  *m_nal;

	x264_picture_t *m_pic_out;



	long long i_pts;


	int m_width, m_height;



	char *m_pBuffer;
	int m_len;




#if TEST_4_PLAT_LIVE
	//////////////////////////////////////////////////////////////////////////
public:
	virtual void DeleteStreamData();
	virtual void OnPacketCompleteNetStreamData(int32_t dwCompleteBytes, int32_t bRecieve);
private:
	int LoginPlat();
	int LogoutPlat();
	void DoMsg();
	//
	//long long m_videotick;
	BOOL m_bLogin;
	BOOL m_bStreamIsError;
	int m_nSession;
	//
	boost::pool<> m_plMsg;//�ڴ�أ���Ϣ����
	//
	emNetRecieveStep m_nRcvStep;
	rqst_head m_head;
	int m_dataLen;
	unsigned char * m_szBuffer;
	//
	CXNetStream* m_pTcpStream;
	CrossCriticalSection m_csTcp;
	//list<boost::shared_ptr<CStreameFrame>> m_frameList;
	list<MyStruct4PhoneDeviceSession *> m_msgList;
	CrossCriticalSection m_csMsg;
	//
	list<MyStruct4PhoneDeviceSession *> m_msgListYUV;
	CrossCriticalSection m_csMsgYUV;
	emPhoneDeviceNetSendStep m_nSendStep;
	static int WorkerThread(void*);
	void Worker();
	CROSS_THREAD_HANDLE	m_hWorkerThread;
	BOOL		m_hEvtWorkerStop;

	CROSS_DWORD64 m_dTickHearbeat;

private:



	

#endif




};



// /*
// x264_param_default������ZZ��
// 
// void    x264_param_default( x264_param_t *param )
// 
// 
// 
// 
// {
// 
// /* �����ڴ�ռ�*/
// 
// memset(param, 0, sizeof(x264_param_t));
// 
// 
// 
// /* CPU�Զ���� */
// 
// param->cpu = x264_cpu_detect();
// 
// param->i_threads = X264_THREADS_AUTO;  /* ���б����߳�Ϊ0 */
// 
// param->b_deterministic = 1; /*�����ȷ����ʱ�߳��Ż�*/
// 
// param->i_sync_lookahead = X264_SYNC_LOOKAHEAD_AUTO; /* �Զ�ѡ���̳߳�ǰ�����С-1 */
// 
// 
// 
// /* ��Ƶ���� */
// 
// param->i_csp = X264_CSP_I420;  /*�����������Ƶ�����ĸ�ʽ0x0001yuv 4:2:0 planar*/
// 
// param->i_width = 0; /* ���*/
// 
// param->i_height = 0;  /* �߶�*/
// 
// param->vui.i_sar_width = 0;
// 
// param->vui.i_sar_height = 0;  /* ���ó���� */
// 
// param->vui.i_overscan = 0;   /* ��ɨ���ߣ�Ĭ��undef(������)����ѡ��show(�ۿ�)crop(ȥ��) */
// 
// param->vui.i_vidformat = 5;  /* undef��Ƶ��ʽ */
// 
// param->vui.b_fullrange = 0;  /* off */
// 
// param->vui.i_colorprim = 2;   /* undefԭʼɫ�ȸ�ʽ*/
// 
// param->vui.i_transfer = 2;  /* undef ת����ʽ*/
// 
// param->vui.i_colmatrix = 2;   /* undef ɫ�Ⱦ�������*/
// 
// param->vui.i_chroma_loc = 0;   /* left center ɫ������ָ������Χ0~5��Ĭ��0*/
// 
// param->i_fps_num = 25; /*֡��*/
// 
// param->i_fps_den = 1;  /*���������͵����ı�ֵ������ʾ֡��*/
// 
// param->i_level_idc = -1; /* levelֵ������*/
// 
// param->i_slice_max_size = 0;  /* ÿƬ�ֽڵ������������Ԥ�Ƶ�NAL����. */
// 
// param->i_slice_max_mbs = 0;  /* ÿƬ�������������д i_slice_count */
// 
// param->i_slice_count = 0;  /* ÿ֡��������Ŀ: ���þ�������. */
// 
// 
// 
// /* ������� */
// 
// param->i_frame_reference = 3;  /*�ο�֡�����֡����*/
// 
// param->i_keyint_max = 250;  /* �ڴ˼������IDR�ؼ�֡ */
// 
// param->i_keyint_min = 25;  /* �����л����ڴ�ֵ����λI, ������ IDR. */
// 
// param->i_bframe = 3; /*�����ο�֮֡���B֡��Ŀ*/
// 
// param->i_scenecut_threshold = 40;  /*��λ����ز�������I֡ */
// 
// param->i_bframe_adaptive = X264_B_ADAPT_FAST; /*����ӦB֡�ж�1*/
// 
// param->i_bframe_bias = 0; /*���Ʋ���B֡�ж�����Χ-100~+100��Խ��Խ���ײ���B֡*/
// 
// param->b_bframe_pyramid = 0;  /*������BΪ�ο�֡ */
// 
// 
// 
// param->b_deblocking_filter = 1;  /*ȥ��ЧӦ���*/
// 
// param->i_deblocking_filter_alphac0 = 0; /* [-6, 6] -6 �����˲���, 6 ǿ */
// 
// param->i_deblocking_filter_beta = 0; /* [-6, 6]  ͬ�� */
// 
// 
// 
// param->b_cabac = 1; /*cabac�Ŀ���*/
// 
// param->i_cabac_init_idc = 0;
// 
// 
// 
// /*���ʿ���*/
// 
// param->rc.i_rc_method = X264_RC_CRF;;/*�㶨����*/
// 
// param->rc.i_bitrate = 0;/*����ƽ�����ʴ�С*/
// 
// param->rc.f_rate_tolerance = 1.0;
// 
// param->rc.i_vbv_max_bitrate = 0;  /*ƽ������ģʽ�£����˲ʱ���ʣ�Ĭ��0(��-B������ͬ) */
// 
// param->rc.i_vbv_buffer_size = 0;  /*���ʿ��ƻ������Ĵ�С����λkbit��Ĭ��0 */
// 
// param->rc.f_vbv_buffer_init = 0.9; /* <=1: fraction of buffer_size. >1: kbit���ʿ��ƻ��������ݱ���������������뻺������С֮�ȣ���Χ0~1.0��Ĭ��0.9*/
// 
// param->rc.i_qp_constant = 23;;/*��Сqpֵ*/
// 
// param->rc.f_rf_constant = 23;
// 
// param->rc.i_qp_min = 10; /*�������С����ֵ */
// 
// param->rc.i_qp_max = 51; /*������������ֵ*/
// 
// param->rc.i_qp_step = 4;  /*֡������������� */
// 
// param->rc.f_ip_factor = 1.4;
// 
// param->rc.f_pb_factor = 1.3;
// 
// param->rc.i_aq_mode = X264_AQ_VARIANCE; /* psy adaptive QP. (X264_AQ_*) */
// 
// param->rc.f_aq_strength = 1.0;
// 
// param->rc.i_lookahead = 40;
// 
// 
// 
// param->rc.b_stat_write = 0;  /* Enable stat writing in psz_stat_out */
// 
// param->rc.psz_stat_out = "x264_2pass.log";
// 
// param->rc.b_stat_read = 0;
// 
// param->rc.psz_stat_in = "x264_2pass.log";
// 
// param->rc.f_qcompress = 0.6; /* 0.0 => cbr, 1.0 => constant qp */
// 
// param->rc.f_qblur = 0.5;   /*ʱ����ģ������ */
// 
// param->rc.f_complexity_blur = 20;  /* ʱ����ģ�������� */
// 
// param->rc.i_zones = 0;  /* number of zone_t's */
// 
// param->rc.b_mb_tree = 1; /* Macroblock-tree ratecontrol. */
// 
// 
// 
// /* ��־ */
// 
// param->pf_log = x264_log_default;
// 
// param->p_log_private = NULL;
// 
// param->i_log_level = X264_LOG_INFO;/*Ĭ��Ϊ��Info��*/
// 
// 
// 
// /*���� */
// 
// param->analyse.intra = X264_ANALYSE_I4x4 | X264_ANALYSE_I8x8;
// 
// param->analyse.inter = X264_ANALYSE_I4x4 | X264_ANALYSE_I8x8
// 
// | X264_ANALYSE_PSUB16x16 | X264_ANALYSE_BSUB16x16;
// 
// param->analyse.i_direct_mv_pred = X264_DIRECT_PRED_SPATIAL;/*�ռ�Ԥ��ģʽ*/
// 
// param->analyse.i_me_method = X264_ME_HEX;/*�˶������㷨HEX*/
// 
// param->analyse.f_psy_rd = 1.0;
// 
// param->analyse.b_psy = 1;
// 
// param->analyse.f_psy_trellis = 0;
// 
// param->analyse.i_me_range = 16;/*�˶����Ʒ�Χ*/
// 
// param->analyse.i_subpel_refine = 7; /* �������˶��������� */
// 
// param->analyse.b_mixed_references = 1;  /*����ÿ�����ķ�����P֡�����Լ��Ĳο���*/
// 
// param->analyse.b_chroma_me = 1;  /* ������ɫ���˶����ƺ�P֡��ģʽѡ�� */
// 
// param->analyse.i_mv_range_thread = -1; /* �߳�֮�����С�ռ�. -1 = auto, based on number of threads. */
// 
// param->analyse.i_mv_range = -1; /*�˶�ʸ����󳤶�set from level_idc*/
// 
// param->analyse.i_chroma_qp_offset = 0; /*ɫ����������ƫ���� */
// 
// param->analyse.b_fast_pskip = 1;  /*����P֡�������*/
// 
// param->analyse.b_weighted_bipred = 1; /*Ϊb֡��ʽ��Ȩ */
// 
// param->analyse.b_dct_decimate = 1; /* ��P-framesת�������� */
// 
// param->analyse.b_transform_8x8 = 1; /* ֡�����*/
// 
// param->analyse.i_trellis = 1;  /* Trellis��������ÿ��8x8�Ŀ�Ѱ�Һ��ʵ�����ֵ����ҪCABAC��Ĭ��0 0���ر�1��ֻ��������ʱʹ��2��һֱʹ��*/
// 
// param->analyse.i_luma_deadzone[0] = 21; /*֡������������ʹ�õ���Ч����С*/
// 
// param->analyse.i_luma_deadzone[1] = 11;  /*֡������������ʹ�õ���Ч����С*/
// 
// param->analyse.b_psnr = 0;/*�Ƿ���ʾPSNR*/
// 
// param->analyse.b_ssim = 0;/*�Ƿ���ʾSSIM*/
// 
// 
// 
// /*����*/
// 
// param->i_cqm_preset = X264_CQM_FLAT;  /*�Զ�����������(CQM),��ʼ������ģʽΪflat 0*/
// 
// memset(param->cqm_4iy, 16, 16);
// 
// memset(param->cqm_4ic, 16, 16);
// 
// memset(param->cqm_4py, 16, 16);
// 
// memset(param->cqm_4pc, 16, 16);
// 
// memset(param->cqm_8iy, 16, 64);
// 
// memset(param->cqm_8py, 16, 64);/*���ٿռ�*/
// 
// /*muxing*/
// 
// param->b_repeat_headers = 1;  /* ��ÿ���ؼ�֡ǰ����SPS/PPS*/
// 
// param->b_aud = 0; /*���ɷ��ʵ�Ԫ�ָ���*/
// 
// }
// 











//
//
//
//x264���ʿ��� (2011-09-13 15:36:54)
//ת�ب�
//
//
//��ǩ�� x264 ���ʿ��� ��̸
//
//
//���ʿ���
//
//qp
//
//Ĭ�ϣ���
// ˵�������ֿ�ѡ�����ʿ��Ʒ���֮һ������x264ʹ�ù̶���������ģʽ������������������ΪP֡������������I֡��B֡�����������ɨCipratio and �Cpbratio������һ�������QPģʽ���ù̶�����������������ζ�����յ��ļ���С�ǲ���֪�ģ�����ͨ��һЩ��������Ԥ�⣩������Ϊ0������������������ͬ�Ӿ�����ʱ��QPģʽ�������ļ���crfģʽ��QPģʽ���ر�����Ӧ����������Ϊ���ǹ̶�QP�ġ�
// ���ѡ��� �Cbitrate�ͨCcrf�ǻ���ģ�����ֻ��ѡһ�������Ǳ����ԭ����Կ���ƪ����http://blog.yikuyiku.com/index.php/archives/1901��
// һ�����crf���ܴ���QPģʽ������QP��Ϊ��ȫ����ҪԤ�������������еظ���Щ��
// �μ����Cbitrate, �Ccrf, �Cipratio, �Cpbratio
//
//bitrate
//
//Ĭ�ϣ���
// ˵�������ֿ�ѡ�����ʿ��Ʒ���֮��������x264ʹ�ù̶�Ŀ�������ģʽ���̶�Ŀ���������ζ�������ļ��Ĵ�С�ǿ�֪�ģ�����Ŀ��������ǲ���֪�ġ� x264����ͼ�������ļ������������������������ȡ�����������Ϊkilobits/sec��8bit = 1byte����
// ͨ�����ѡ��ͨCpassѡ����Ͻ���2�˱��롣
// ���ѡ��� �Cqp�ͨCcrf�ǻ���ģ�����ֻ��ѡһ�������Ǳ����ԭ����Կ���ƪ����http://blog.yikuyiku.com/index.php/archives/1901��
// �μ����Cqp, �Ccrf, �Cratetol, �Cpass, �Cstats
//
//crf
//
//Ĭ�ϣ�23.0
// ˵�������ֿ�ѡ�����ʿ��Ʒ���֮�����̶�ratefactor��QP�ǹ̶���������bitrate�ǹ̶��ļ���С��crf���ǹ̶�����������crf�����ṩ��QPһ�����Ӿ��������������ļ���С��crf�ĵ�λ��ratefactor��
// crf��ͨ��������Щ������ô��Ҫ����֡������������һ�еġ�������ô��Ҫ����˼�ǹ��ںķ����������������۲����֡�����縴�ӻ��߳��������еĳ�����ʡ���������ʻ�������������Ч��֡�
// crf�����2�˱���죬��Ϊ���൱��ʡ���˵�1�˱��롣����crf����������Ҳ�ǲ���Ԥ��ġ���Ӧ�ø���Ӧ�ó�����ѡ�����ʿ��Ʒ�ʽ��
// ���ѡ��� �Cqp�ͨCcrf�ǻ���ģ�����ֻ��ѡһ�������Ǳ����ԭ����Կ���ƪ����http://blog.yikuyiku.com/index.php/archives/1901��
// �μ����Cqp, �Cbitrate

// */



#endif //PHONE_DEVICE_X_456W34324
