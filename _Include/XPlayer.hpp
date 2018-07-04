// XPlayer.h : main header file for the XPlayer DLL
//

//----------------------------------------
// kirainstorm
// https://github.com/kirainstorm
//----------------------------------------
#ifndef XPLAYER_COM_4876645989
#define XPLAYER_COM_4876645989

#include "PlatformDefine.h"
class DecodeVideoCallBack//解码后数据回调接口
{
public:
	virtual int OnDecodeVideoCallBack(const char* pDecData, int nLen, int nWidth, int nHeight, int nTime, int nTypeFrame = 0, int nOutFormat = 0, int nFrameRate = 0) = 0;
};


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
	RSPLAYER_CODEC_AUDIO_TYPE_G711U_zzzzzz,//zzzzzzz
	RSPLAYER_CODEC_AUDIO_TYPE_ADPCM,
};




// decode
// void * CreateFFmpegInstance(int VideoType, DecodeVideoCallBack*	pDec);
// int DestoryFFmpegInstance(void * pFFmpeg);
// int FFmpegStart(void * pFFmpeg);
// int FFmpegStop(void * pFFmpeg);
// int FFmpegInputData(void * pFFmpeg, char * buf, int len,int videoframerate/* = 25*/);
// int FFmpegCapPic(void * pFFmpeg, char * filePath, char * fileName);//filePath根目录+Pictures目录+日期目录+设备名称目录
// 
// talk
// void * CreateAudioEncInstance(int AudioType);
// int DestoryAudioEncInstance(void * pEnc);
// int AudioEncData(void * pEnc, const char* pData, int lDataLen, char* pOutData, int * lOutDataLen);
// 
// voice
// void * CreateAudioDecInstance(int AudioType);
// int DestoryAudioDecInstance(void * pDec);
// int AudioDecData(void * pDec, const char* pData, int lDataLen, char* pOutData, int * lOutDataLen);
// 
// int WriteJpegFile(char * filepath, char * filename, unsigned char* yuvData, int image_width, int image_height);
class CXVoiceInterface
{
public:

	virtual void Delete() = 0;

	virtual int InitVoice(BOOL bEncode) = 0;//只能使用编解码中的一个
	virtual int DeinitVoice() = 0;

	virtual int EncodeVoice(const void* pVoiceData, int nVoiceDataLen, void* pEncDataBuf, int* pnEncDataLen) = 0;
	virtual int DecodeVoice(const void* pEncDataBuf, int nEncDataLen, void* pVoiceData, int* pnVoiceDataLen) = 0;
};



#include "XPlayerFFmpegDecoder.hpp"
#include "XPlayerFdkaac.hpp"


inline void * CreateFFmpegInstance(int type, DecodeVideoCallBack* pDec)
{
	return new CFFmpegDecoder(type, pDec);
}
inline int DestoryFFmpegInstance(void * pFFmpeg)
{
	CFFmpegDecoder *p = (CFFmpegDecoder *)pFFmpeg;
	if (NULL == p)
	{
		return -1;
	}
	delete p;
	return 0;
}
inline int FFmpegStart(void * pFFmpeg)
{
	CFFmpegDecoder *p = (CFFmpegDecoder *)pFFmpeg;
	if (NULL == p)
	{
		return -1;
	}
	p->Start();
	return 0;
}
inline int FFmpegStop(void * pFFmpeg)
{
	CFFmpegDecoder *p = (CFFmpegDecoder *)pFFmpeg;
	if (NULL == p)
	{
		return -1;
	}
	p->Stop();
	return 0;
}
inline int FFmpegInputData(void * pFFmpeg, char * buf, int len, int videoframerate)
{
	CFFmpegDecoder *p = (CFFmpegDecoder *)pFFmpeg;
	if (NULL == p)
	{
		return -1;
	}
	p->InputData(buf, len, videoframerate);
	return 0;
}

inline int FFmpegCapPic(void * pFFmpeg, char * filePath, char * fileName)
{
	CFFmpegDecoder *p = (CFFmpegDecoder *)pFFmpeg;
	if (NULL == p)
	{
		return -1;
	}
	p->CapPic(filePath, fileName);
	return 0;
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
inline void * CreateAudioEncInstance(int AudioType = RSPLAYER_CODEC_AUDIO_TYPE_AAC)
{
	CXVoiceInterface *P = NULL;

	switch (AudioType)
	{
	case RSPLAYER_CODEC_AUDIO_TYPE_AAC:
		P = new CFdkaac();
		break;
	default:
		break;
	}
	if (P)
	{
		P->InitVoice(TRUE);
	}
	return P;
}
inline int DestoryAudioEncInstance(void * pTake)
{
	CXVoiceInterface *P = (CXVoiceInterface *)pTake;
	P->DeinitVoice();
	P->Delete();

	return 0;
}
inline int AudioEncData(void * pTake, const char* pData, int lDataLen, char* pOutData, int * lOutDataLen)
{
	CXVoiceInterface *P = (CXVoiceInterface *)pTake;
	P->EncodeVoice(pData, lDataLen, pOutData, lOutDataLen);

	return 0;
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
inline void * CreateAudioDecInstance(int AudioType = RSPLAYER_CODEC_AUDIO_TYPE_AAC)
{
	CXVoiceInterface *P = NULL;

	switch (AudioType)
	{
	case RSPLAYER_CODEC_AUDIO_TYPE_AAC:
		P = new CFdkaac();
		break;
	default:
		break;
	}

	if (P)
	{
		P->InitVoice(FALSE);
	}
	return P;
}
inline int DestoryAudioDecInstance(void * pVoice)
{
	CXVoiceInterface *P = (CXVoiceInterface *)pVoice;
	P->DeinitVoice();
	P->Delete();

	return 0;
}
inline int AudioDecData(void * pVoice, const char* pData, int lDataLen, char* pOutData, int * lOutDataLen)
{
	CXVoiceInterface *P = (CXVoiceInterface *)pVoice;
	P->DecodeVoice(pData, lDataLen, pOutData, lOutDataLen);
	return 0;
}


inline int WriteJpegFile(char * filepath, char * filename, unsigned char* yuvData, int image_width, int image_height)
{
	return WRITE_JPEG_FILE(filepath, filename, (unsigned char *)yuvData, image_width, image_height, 90);
}







//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
// #ifdef _PLATFORM_ANDROID
// void * CreateOpenglEsInstance();
// int DestoryOpenglEsInstance(void * opengl);
// int OpenglEsInit(void * opengl);
// int OpenglEsStep(void * opengl);
// int OpenglEsResize(void * opengl,int width, int height);
// int OpenglEsStop(void * opengl);
// int OpenglEsClear(void * opengl);//OpenglEsResume
// int OpenglEsPause(void * opengl);//OpenglEsResume
// int OpenglEsResume(void * opengl);
// 
// int FFmpegSetOpenglEs(void * pFFmpeg, void *opengl);//for android//--------------------------------------
// #endif
// 
// #ifdef _PLATFORM_IOS
// int FFmpegSetYUVNotify4IOS(void * pFFmpeg, CXDecVideoData* pDec);//for ios//--------------------------------------
// #endif
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////



#endif//XPLAYER_COM_4876645989

