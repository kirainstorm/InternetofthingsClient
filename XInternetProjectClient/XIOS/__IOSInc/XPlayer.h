// XPlayer.h : main header file for the XPlayer DLL
//


#ifndef XPLAYER_COM_4876645989
#define XPLAYER_COM_4876645989

#include "PlatformDefine.h"

#ifdef _PLATFORM_ANDROID
#define WINAPI
#endif

#ifdef _PLATFORM_IOS
#define WINAPI
#endif


enum FFmpeg_DecodeType
{
	RSPLAYER_CODEC_VIDEO_TYPE_NONE = 0,
	RSPLAYER_CODEC_VIDEO_TYPE_H264,//ffmpeg
	RSPLAYER_CODEC_VIDEO_TYPE_MJPEG,//ffmpeg
	RSPLAYER_CODEC_VIDEO_TYPE_H265,//ffmpeg
	RSPLAYER_CODEC_VIDEO_TYPE_H265_HISILICON,//hisilicon
};

enum Audio_Type
{
	RSPLAYER_CODEC_AUDIO_TYPE_NONE = 0,
	RSPLAYER_CODEC_AUDIO_TYPE_AAC,//YYJ-FFMPEG //audioChannels = 1, audioSamplesPerSec = 8000, audioBitsPerSample=16 bit_rate=16000 bufsize2048
	RSPLAYER_CODEC_AUDIO_TYPE_PCM,
	RSPLAYER_CODEC_AUDIO_TYPE_G711A,//YYJ-FFMPEG
	RSPLAYER_CODEC_AUDIO_TYPE_G711U,//YYJ-FFMPEG
	RSPLAYER_CODEC_AUDIO_TYPE_G711U_ZYX,//ZYX
	RSPLAYER_CODEC_AUDIO_TYPE_ADPCM,
};

class DecodeVideoCallBack//解码后数据回调接口
{
public:
	virtual int OnDecodeVideoCallBack(const char* pDecData, int nLen, int nWidth, int nHeight, int nTime, int nTypeFrame = 0, int nOutFormat = 0, int nFrameRate = 0) = 0;
};


//decode
void * WINAPI CreateFFmpegInstance(int VideoType, DecodeVideoCallBack*	pDec);
int WINAPI DestoryFFmpegInstance(void * pFFmpeg);
int WINAPI FFmpegStart(void * pFFmpeg);
int WINAPI FFmpegStop(void * pFFmpeg);
int WINAPI FFmpegInputData(void * pFFmpeg, char * buf, int len,int videoframerate/* = 25*/);
int WINAPI FFmpegCapPic(void * pFFmpeg, char * filePath, char * fileName);//filePath根目录+Pictures目录+日期目录+设备名称目录
//
//talk
void * WINAPI CreateAudioEncInstance(int AudioType);
int WINAPI DestoryAudioEncInstance(void * pEnc);
int WINAPI AudioEncData(void * pEnc, const char* pData, int lDataLen, char* pOutData, int * lOutDataLen);
//
//voice
void * WINAPI CreateAudioDecInstance(int AudioType);
int WINAPI DestoryAudioDecInstance(void * pDec);
int WINAPI AudioDecData(void * pDec, const char* pData, int lDataLen, char* pOutData, int * lOutDataLen);
//
int WINAPI WriteJpegFile(char * filepath, char * filename, unsigned char* yuvData, int image_width, int image_height);









//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
// #ifdef _PLATFORM_ANDROID
// void * WINAPI CreateOpenglEsInstance();
// int WINAPI DestoryOpenglEsInstance(void * opengl);
// int WINAPI OpenglEsInit(void * opengl);
// int WINAPI OpenglEsStep(void * opengl);
// int WINAPI OpenglEsResize(void * opengl,int width, int height);
// int WINAPI OpenglEsStop(void * opengl);
// int WINAPI OpenglEsClear(void * opengl);//OpenglEsResume
// int WINAPI OpenglEsPause(void * opengl);//OpenglEsResume
// int WINAPI OpenglEsResume(void * opengl);
// 
// int WINAPI FFmpegSetOpenglEs(void * pFFmpeg, void *opengl);//for android//--------------------------------------
// #endif
// 
// #ifdef _PLATFORM_IOS
// int WINAPI FFmpegSetYUVNotify4IOS(void * pFFmpeg, CXDecVideoData* pDec);//for ios//--------------------------------------
// #endif
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////



#endif//XPLAYER_COM_4876645989

