#pragma once

#ifndef FFmpeg_Decoder_ERHJDERTYUIJKJHGF
#define FFmpeg_Decoder_ERHJDERTYUIJKJHGF


#include "PlatformDefine.h"

#include "XPlayerCapturePictures.h"

//////////////////////////////////////////////////////////////////////////
#ifdef _PLATFORM_ANDROID
/*****************************************************************************************/
extern JavaVM* g_JavaVM;
//extern jobject g_VideoObj;
//#include "openglRenderer.h"
#endif

//////////////////////////////////////////////////////////////////////////
#ifdef _PLATFORM_WINDOW
//#include "VideoRender.h"
//#include "D3DSurfaceRender.h"

#pragma comment(lib, "turbojpeg-static.lib")

//#define ddrawrender 0
//#define d3drender (ddrawrender>0?0:1)
//#define d3drender 1 //使用d3d贴图



//ffmpeg
#ifdef __cplusplus
extern "C" {
#endif
	/*Include ffmpeg header file*/
#include <libavutil/avutil.h>
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#include <libswscale/swscale.h>
#ifdef __cplusplus
}
#endif

#pragma comment(lib, "avutil.lib")
#pragma comment(lib, "avformat.lib")
#pragma comment(lib, "avcodec.lib")
#pragma comment(lib, "swscale.lib")


#define DEC_OF_HISILICON_H265 0
#if DEC_OF_HISILICON_H265
#include "IHWVideo_Typedef.h"
#include "IHW265Dec_Api.h"
#pragma comment(lib, "HW_H265dec_Win32.lib")
#endif


#endif
//////////////////////////////////////////////////////////////////////////
#ifdef _PLATFORM_ANDROID
extern "C"
{
#include "libavformat/avformat.h"
#include "libavcodec/avcodec.h"
#include "libswscale/swscale.h"
}
#endif
//////////////////////////////////////////////////////////////////////////
#ifdef _PLATFORM_IOS
#include "xplayer.h"
extern "C"
{
#include "libavformat/avformat.h"
#include "libavcodec/avcodec.h"
#include "libswscale/swscale.h"
}
#endif


#define MAX_FRAME_SIZE (8*1024*1024)
class CInputFrame
{
public:
	CInputFrame(int len){
		m_bufferlen = len;
		int n = (int)(ceil(len / 1024.0) * 1024);
		m_buffer = new char[n];
		//memset(m_buffer, 0, len + 1);
	};
	~CInputFrame(){
		delete[] m_buffer;
	};
public:
	char * m_buffer;//未解码数据
	int m_bufferlen;
	int m_nTimeTick;//接收到的数据与上一帧的时间间隔
public:
	//char * m_frame;//解码后数据
	//int m_framelen;
	int m_width, m_height;
	//int key_frame;
};


#ifdef _WINDOWS_
static int log_level = AV_LOG_INFO;
extern "C" void av_log_set_callback(void(*callback)(void*, int, const char*, va_list));

// void av_log_callback(void* ptr, int level, const char* fmt, va_list vl)
// {
// 	char line[1024];
// 	if (level > log_level)
// 		return;
// 
// 	_vsnprintf(line, sizeof(line), fmt, vl);
// 
// 	OutputDebugStringA(line);
// }
#endif

#if DEC_OF_HISILICON_H265
void *HW265D_Malloc(UINT32 channel_id, UINT32 size)
{
	return (void *)malloc(size);
}
void HW265D_Free(UINT32 channel_id, void * ptr)
{
	free(ptr);
}
void HW265D_Log(UINT32 channel_id, IHWVIDEO_ALG_LOG_LEVEL eLevel, INT8
	*p_msg, ...)
{
	printf("%s.\n", p_msg);
}
INT32 H265DecLoadAU(UINT8* pStream, UINT32 iStreamLen, UINT32* pFrameLen)
{
	UINT32 i;
	UINT32 state = 0xffffffff;
	BOOL32 bFrameStartFound = 0;
	*pFrameLen = 0;
	if (NULL == pStream || iStreamLen <= 4)
	{
		return -1;
	}
	for (i = 0; i < iStreamLen; i++)
	{
		if ((state & 0xFFFFFF7E) >= 0x100 &&
			(state & 0xFFFFFF7E) <= 0x13E)
		{
			if (1 == bFrameStartFound)
			{
				if ((pStream[i + 1] >> 7) == 1)
				{
					*pFrameLen = i - 4;
					return 0;
				}
			}
			else
			{
				bFrameStartFound = 1;
			}
		}
		/*find a vps, sps, pps*/
		if ((state & 0xFFFFFF7E) == 0x140 ||
			(state & 0xFFFFFF7E) == 0x142 ||
			(state & 0xFFFFFF7E) == 0x144)
		{
			if (1 == bFrameStartFound)
			{
				*pFrameLen = i - 4;
				return 0;
			}
			else
			{
				bFrameStartFound = 1;
			}
		}
		state = (state << 8) | pStream[i];
	}
	*pFrameLen = i;
	return -1;
}
#endif



#include "XPlayer.hpp"
//////////////////////////////////////////////////////////////////////////
class CFFmpegDecoder
{
public:
	CFFmpegDecoder(int codec_type, DecodeVideoCallBack*	pDec)
	{
		m_videoframerate = 25;
		m_pDecVideoObjRealtime = pDec;
		//
		m_frame = new char[MAX_FRAME_SIZE];
		m_frame4cap = new char[MAX_FRAME_SIZE];
		m_width4cap = 0;
		m_height4cap = 0;
		//
		pCodecCtx = NULL;
		pCodec = NULL;
		pFrame = NULL;
		//


#ifdef _PLATFORM_WINDOW
		m_ThreadHandle = NULL;
#else
		m_ThreadID = CROSS_THREAD_NULL;
#endif

		//
		switch (codec_type)
		{
		case  RSPLAYER_CODEC_VIDEO_TYPE_MJPEG:
			m_id = AV_CODEC_ID_MJPEG;
			break;

		case  RSPLAYER_CODEC_VIDEO_TYPE_H264:
			m_id = AV_CODEC_ID_H264;
			break;
		case RSPLAYER_CODEC_VIDEO_TYPE_H265:
			m_id = AV_CODEC_ID_H265;
			break;
#if DEC_OF_HISILICON_H265
		case RSPLAYER_CODEC_VIDEO_TYPE_HISILICON:
			m_id = AV_CODEC_ID_H265;
			break;
#endif
		default:
			break;
		}

#if DEC_OF_HISILICON_H265
		if (m_id == AV_CODEC_ID_H265)
		{
			hDecoder = NULL;
			memset(&stInitParam, 0, sizeof(stInitParam));
			memset(&stOutArgs, 0, sizeof(stOutArgs));
		}
#endif

		//m_frameratebyinput = 30;
		//m_dwFirstInput = CrossGetTickCount64();
		//m_framecountbyinpt = 1;


	};
	~CFFmpegDecoder()
	{
		Stop();
		delete[] m_frame4cap;
		delete[] m_frame;

	};

#ifdef _WINDOWS_
	static void av_log_callback(void* ptr, int level, const char* fmt, va_list vl)
	{
		char line[1024];
		if (level > log_level)
			return;

		_vsnprintf(line, sizeof(line), fmt, vl);

		OutputDebugStringA(line);
	}
#endif

public:
	void Start()
	{
		av_register_all();
		//avcodec_register_all();

#ifdef _WINDOWS_
		av_log_set_callback(av_log_callback);//log?
#endif

		pCodec = avcodec_find_decoder(m_id);
		if (pCodec == NULL)
		{
			printf("pCodec == NULL\n");
			return;
		}

		pCodecCtx = avcodec_alloc_context3(pCodec);
		if (pCodecCtx == NULL)
		{

			printf("if(pCodecCtx == NULL)\n");
			return;
		}

		// 	pCodecCtx->codec_id = CODEC_ID_MJPEG;
		// 	pCodecCtx->codec_type = AVMEDIA_TYPE_VIDEO;
		// 	pCodecCtx->width = 1920;
		// 	pCodecCtx->height = 1080;


		if (avcodec_open2(pCodecCtx, pCodec, NULL) >= 0)
		{
			pFrame = av_frame_alloc();
		}

		if (pFrame == NULL)
		{
			printf("if(pFrame == NULL)\n");
			return;
		}
		// 	pCodecCtx->time_base.num = 15;
		// 	pCodecCtx->time_base.den = 1;
		// 	pCodecCtx->frame_number = 1;


#if DEC_OF_HISILICON_H265
		if (m_id == AV_CODEC_ID_H265)
		{
			/* create decode handle */
			stInitParam.uiChannelID = 0;
			stInitParam.iMaxWidth = 1920;
			stInitParam.iMaxHeight = 1088;
			stInitParam.iMaxRefNum = 2;
			stInitParam.eThreadType = IH265D_SINGLE_THREAD; // or
			IH265D_MULTI_THREAD;
			stInitParam.eOutputOrder = IH265D_DECODE_ORDER; // or
			IH265D_DISPLAY_ORDER;
			stInitParam.MallocFxn = HW265D_Malloc;
			stInitParam.FreeFxn = HW265D_Free;
			stInitParam.LogFxn = HW265D_Log;
			INT32 iRet = IHW265D_Create(&hDecoder, &stInitParam);
			if (IHW265D_OK != iRet)
			{
				return;
			}
		}
#endif

		//---------------------------------------------------
		m_bExit = FALSE;
#ifdef _PLATFORM_WINDOW
		// 1、THREAD_PRIORITY_TIME_CRITICAL：Time - critical，关键时间（最高）
		// 2、THREAD_PRIORITY_HIGHEST：Highest，最高（其实是“次高”）
		// 3、THREAD_PRIORITY_ABOVE_NORMAL：Above normal，高于标准
		// 4、THREAD_PRIORITY_NORMAL：Normal，标准
		// 5、THREAD_PRIORITY_BELOW_NORMAL：Below normal，低于标准
		// 6、THREAD_PRIORITY_LOWEST：Lowest，最低（其实是“次低”）
		// 7、THREAD_PRIORITY_IDLE：Idle，空闲（最低）

		//最小化到任务栏的时候，好像win10对线程优先级降低了
		m_ThreadHandle = ::CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)WorkerThread, this, CREATE_SUSPENDED, NULL);
		::SetThreadPriority(m_ThreadHandle, THREAD_PRIORITY_TIME_CRITICAL);
		::ResumeThread(m_ThreadHandle);

#else
		m_ThreadID = CrossCreateThread(WorkerThread, this);
#endif


		//---------------------------------------------------
	};
	void Stop()
	{
		m_bExit = TRUE;
		//exit cmd thread

#ifdef _PLATFORM_WINDOW
		if (NULL != m_ThreadHandle)
		{
			WaitForSingleObject(m_ThreadHandle, INFINITE);
			CloseHandle(m_ThreadHandle);
			m_ThreadHandle = NULL;
		}
#else
		if (m_ThreadID != CROSS_THREAD_NULL) {
			CrossWaitThread(m_ThreadID);
			m_ThreadID = CROSS_THREAD_NULL;
		}
#endif
		if (pFrame)
		{
			av_free(pFrame);
			pFrame = NULL;
		}

		if (pCodecCtx)
		{
			avcodec_close(pCodecCtx);
			av_free(pCodecCtx);
			pCodecCtx = NULL;
		}

		while (m_frameList.size() > 0)
		{
			m_frameList.pop_front();
		}


#if DEC_OF_HISILICON_H265
		if (m_id == AV_CODEC_ID_H265)
		{
			if (hDecoder != NULL)
			{
				IHW265D_Delete(hDecoder);
				hDecoder = NULL;
			}
		}
#endif

		m_pDecVideoObjRealtime = NULL;
	};
	void InputData(char * buf, int len, int videoframerate)
	{
		m_videoframerate = (videoframerate <= 0) ? 25 : videoframerate;

		m_cs.Lock();
#if 1
		if (m_frameList.size() > 30)
		{
			CROSS_TRACE(">>>>>>> ###### FFmpegDecoder ######  XXXXXXXXXX_buffer_SIZE > %d \n", m_frameList.size());

			do
			{
				m_frameList.pop_front();
			} while (m_frameList.size() > 0);
		}
#endif

		boost::shared_ptr<CInputFrame> sFrame(new CInputFrame(len));
		CInputFrame *p = sFrame.get();
		memcpy(p->m_buffer, buf, len);
		m_frameList.push_back(sFrame);
		m_cs.Unlock();
	};
	void CapPic(char * filePath, char * fileName)
	{
		if ((0 != m_width4cap) && (0 != m_height4cap))
		{
			WRITE_JPEG_FILE(filePath, fileName, (unsigned char *)m_frame4cap, m_width4cap, m_height4cap);
		}
	};

private:
	//key_frame:1 -> keyframe, 0-> not
	int FFmpegDecoder(const char * inBuf, int inSize, char *outBuf, int& outSize, int &outWidth, int &outHeight, int &key_frame)
	{
#if DEC_OF_HISILICON_H265
		// 	typedef enum tagHW265D_DECODESTATUS
		// 	{
		// 		IH265D_SLEEP = -1,//  [8/20/2015 win8]  XJMADD VS2013
		// 		IH265D_GETDISPLAY = 0,
		// 		IH265D_NEED_MORE_BITS,
		// 		IH265D_NO_PICTURE,
		// 		IH265D_ERR_HANDLE
		// 	} HW265D_DECODESTATUS;

		if (m_id == AV_CODEC_ID_H265)
		{
			stInArgs.pStream = (UINT8*)inBuf;
			stInArgs.uiStreamLen = inSize;
			if (IHW265D_OK == IHW265D_DecodeAU(hDecoder, &stInArgs, &stOutArgs))
			{
				int i = 0, nDataLen = 0;
				for (i = 0, nDataLen = 0; i < 3; i++)
				{
					int nShift = (i == 0) ? 0 : 1;
					BYTE * pYUVData = (BYTE *)stOutArgs.pucOutYUV[i];
					for (int j = 0; j < (stOutArgs.uiDecHeight >> nShift); j++)
					{
						memcpy(outBuf + nDataLen, pYUVData, (stOutArgs.uiDecWidth >> nShift));
						pYUVData += ((i == 0) ? stOutArgs.uiYStride : stOutArgs.uiUVStride);
						nDataLen += (stOutArgs.uiDecWidth >> nShift);
					}
				}
				outSize = nDataLen;
				outWidth = stOutArgs.uiDecWidth;
				outHeight = stOutArgs.uiDecHeight;
				//key_frame = 
				return 0;
			}
		}
#endif

		//
		AVPacket avpkt;
		av_init_packet(&avpkt);
		avpkt.data = (uint8_t *)inBuf;
		avpkt.size = inSize;
		//
		CROSS_DWORD64 dwdw = CrossGetTickCount64();
		int consumed_bytes = 0;
		int iResult = avcodec_decode_video2(pCodecCtx, pFrame, &consumed_bytes, &avpkt);
		//
		if (consumed_bytes > 0)
		{
			key_frame = pFrame->key_frame;
			outHeight = pCodecCtx->height;
			outWidth = pCodecCtx->width;
			outSize = outWidth*outHeight * 3 / 2;

#if 0
			uint8_t *p;

			int i, j, k;
			for (i = 0; i < pCodecCtx->height; i++)
			{
				p = (uint8_t*)outBuf + i *  pCodecCtx->width;
				memcpy(p, pFrame->data[0] + pFrame->linesize[0] * i, pCodecCtx->width);
			}

			for (j = 0; j < pCodecCtx->height / 2; j++)
			{
				p = (uint8_t*)outBuf + j *  pCodecCtx->width / 2 + pCodecCtx->height*pCodecCtx->width;
				memcpy(p, pFrame->data[1] + pFrame->linesize[1] * j, pCodecCtx->width / 2);
			}

			for (k = 0; k < pCodecCtx->height / 2; k++)
			{
				p = (uint8_t*)outBuf + k *  pCodecCtx->width / 2 + pCodecCtx->height*pCodecCtx->width + pCodecCtx->height / 2 * pCodecCtx->width / 2;
				memcpy(p, pFrame->data[2] + pFrame->linesize[2] * k, pCodecCtx->width / 2);
			}
#endif

#if 1
			//http://blog.csdn.net/xingyu19871124/article/details/6296912

			//pack yvu data 
			for (int i = 0, nDataLen = 0; i < 3; i++)
			{
				int nShift = (i == 0) ? 0 : 1;
				BYTE * pYUVData = (BYTE *)pFrame->data[i];
				for (int j = 0; j < (pCodecCtx->height >> nShift); j++)
				{
					memcpy(&outBuf[nDataLen], pYUVData, (pCodecCtx->width >> nShift));
					pYUVData += pFrame->linesize[i];
					nDataLen += (pCodecCtx->width >> nShift);
				}
			}
#endif

			return 0;
		}

		return -1;
	};
	//
	DecodeVideoCallBack*	m_pDecVideoObjRealtime;		// 解码后回调


#ifdef _PLATFORM_WINDOW
	HANDLE m_hSleepHandle;
#endif


	//CROSS_DWORD64 m_dwFirstInput;
	//int m_frameratebyinput;
	//int m_framecountbyinpt;
private:
	//解码后数据
	char * m_frame;
	int m_framelen;
	int m_width, m_height;
	int key_frame;
	//解码后要抓拍的数据
	char * m_frame4cap;
	int m_width4cap, m_height4cap;


	int m_videoframerate;
private:
	AVCodecID m_id;
	AVCodecContext   *pCodecCtx;
	AVCodec       *pCodec;
	AVFrame       *pFrame;

private:
	BOOL m_bExit;
#ifdef _PLATFORM_WINDOW
	HANDLE m_ThreadHandle;
#else
	CROSS_THREAD_HANDLE m_ThreadID;
#endif
	
	static int WorkerThread(void* param)
	{
		CFFmpegDecoder * p = (CFFmpegDecoder *)param;
		p->Worker();
		return 0;
	};
	void Worker()
	{
		//-----------------------------------------------------------------------------------------
		CROSS_DWORD64 get_and_dec_and_draw_oneframe_used_time_begin = 0;//记录准备解码显示睡眠的当前时间
		int get_and_dec_and_draw_oneframe_used_time = 0;
		//-----------------------------------------------------------------------------------------
		int nNeedSleepTime = 0;//根据帧率计算的需要休眠的时间


		while (!m_bExit)
		{
			CROSS_DWORD64 dwtime1 = CrossGetTickCount64();//<<<<<<<<<<
			boost::shared_ptr<CInputFrame> sFrame;
			CInputFrame *p = NULL;
			//------------------------------------------
			m_cs.Lock();
			//
			if (m_frameList.size() > 0)
			{
				sFrame = m_frameList.front();
				p = sFrame.get();
				m_frameList.pop_front();
			}
			else
			{
				m_cs.Unlock();//-----here
				CrossSleep(1);
				continue;
			}
			//
			m_cs.Unlock();
			//------------------------------------------

#ifdef _PLATFORM_WINDOW
			nNeedSleepTime = ((1000 / m_videoframerate) / 13) * 13;
#else
			nNeedSleepTime = ((1000 / m_videoframerate) / 5) * 5;
#endif

			//////////////////////////////////////////////////////////////////////////
			{
				//打印耗时，可以粗略估算帧率
				get_and_dec_and_draw_oneframe_used_time = (int)(CrossGetTickCount64() - get_and_dec_and_draw_oneframe_used_time_begin);//第一次解码成功后到获取下一帧的时间
				//CROSS_TRACE("---------------------------------------------------->>>>>>> %d ###### %d <<<<<<<<---- need %d", m_frameList.size(), get_and_dec_and_draw_oneframe_used_time, nNeedSleepTime);
				get_and_dec_and_draw_oneframe_used_time_begin = CrossGetTickCount64();
			}
			//////////////////////////////////////////////////////////////////////////






			//请注意：感觉android 的x264的param->rc.i_rc_method = X264_RC_ABR;为了平均码率，在检测到运动物体时会降低帧率，但是解码时间正常，缓存链表无数据
			//现象：静止画面很多帧数据，可能MAX，晃动的话一直卡，没有帧过来。
			//也有可能是android采集帧率设置有问题，很大可能是android无法设置视频采集帧率，帧率是变化的。
			int size = m_frameList.size();//当前缓冲的数据数量

			if (size > 5)
			{
				nNeedSleepTime -= 5;
			}

			if (size > 10)
			{
				nNeedSleepTime -= 10;
			}

			if (size > 15)
			{
				nNeedSleepTime -= 15;
			}

			if (size > 20)
			{
				nNeedSleepTime -= 20;
			}

			if (nNeedSleepTime < 0)
			{
				nNeedSleepTime = 0;
			}





			CROSS_DWORD64 dwDecodeStartTick = CrossGetTickCount64();
			int ret = FFmpegDecoder(p->m_buffer, p->m_bufferlen, m_frame, m_framelen, m_width, m_height, key_frame);
			//CROSS_TRACE("---------------------------------------------------->>>>>>> %d ###### %d <<<<<<<<111", m_frameList.size(), CrossGetTickCount64() - dwDecodeStartTick);
			do
			{
				//
				if (0 != ret)
				{
					break;
				}
				//
				if (m_pDecVideoObjRealtime)
				{
					m_pDecVideoObjRealtime->OnDecodeVideoCallBack(m_frame, m_framelen, m_width, m_height, 0);
				}
				//
				memcpy(m_frame4cap, m_frame, m_framelen);
				m_width4cap = m_width;
				m_height4cap = m_height;
				//
			} while (0);
			//
			sFrame.reset();

			//CROSS_TRACE("---------------------------------------------------->>>>>>> ###### FFmpegDecoder ###### m_frameList.size():%d  UsedTime: %d  Framerate=%d once=%d ENCtime=%d",
			//	m_frameList.size(), getanddecusedtime, 1000 / getanddecusedtime, nNeedSleepTime, (int)(CrossGetTickCount64() - dwDecodeStartTick));


			//CROSS_TRACE("---------------------------------------------------->>>>>>> %d ###### %d <<<<<<<<222", m_frameList.size(), CrossGetTickCount64() - dwDecodeStartTick);

			nNeedSleepTime = nNeedSleepTime - (int)(CrossGetTickCount64() - dwDecodeStartTick);//减掉解码耗时
			if (nNeedSleepTime < 0)
			{
				nNeedSleepTime = 0;
			}


			CROSS_DWORD64 mmm = CrossGetTickCount64();
			if (nNeedSleepTime > 4)
			{
				do
				{
					CrossSleep(1);//根据帧率休眠一段时间才播放下一帧
				} while (((CrossGetTickCount64() - mmm)) < (nNeedSleepTime));
			}

			//CROSS_TRACE("---------------------------------------------------->>>>>>> %d ######  %d", nNeedSleepTime, (int)(CrossGetTickCount64() - mmm));

		}



	};

private:
	CrossCriticalSection m_cs;
	list< boost::shared_ptr<CInputFrame> > m_frameList;

private:
#if DEC_OF_HISILICON_H265
	IH265DEC_HANDLE hDecoder;
	IHW265D_INIT_PARAM stInitParam;
	IH265DEC_INARGS stInArgs;
	IH265DEC_OUTARGS stOutArgs;
#endif

};



#endif












//int m_framelen4cap;
// public:
// #ifdef _PLATFORM_WINDOW
// 	void SetHwnd(HWND hwd, CXDecVideoData*	pDec){
// 		m_hwnd = hwd;
// 		m_pDecVideoObjRealtime = pDec;
// 	};
// #endif
// #ifdef _PLATFORM_ANDROID
// 	void SetOpenglEs(void * r)
// 	{
// 		render = (COpenglRenderer *)r;
// 	};
// #endif
//     
// 
// #ifdef _PLATFORM_IOS
// 	void FFmpegSetYuvNotify4Mobile(CXDecVideoData*	pDec)
//     {
//         m_YUVDelegate = pDec;
//     };
//     
//#endif







// private:
// #ifdef _PLATFORM_WINDOW
// //#if ddrawrender
// //	CVideoRender m_display;
// //#endif
// 
// #if d3drender
// 	CD3DSurfaceRender m_display;
// #endif
// 	
// 
// 	//HWND m_hwnd;
// 	
// #endif
// 
// #ifdef _PLATFORM_ANDROID
// 	COpenglRenderer * render;
// #endif
//     
// #ifdef _PLATFORM_IOS
// 	CXDecVideoData* m_YUVDelegate;
// #endif



// 	void AddFrame(boost::shared_ptr<DecodeFrame> &p);
// 	boost::shared_ptr<CStreameFrame> GetFrame();
// 	void ClearBuffer();
// 	BOOL IsEmpty();




#if 0
int Ccodec::Codec_SetEncodcParam(int w, int h, int bitrate, char Format, char framerate)
{
	if (m_pCodecCtx == NULL) return -1;
	/* put sample parameters */
	m_pCodecCtx->bit_rate = bitrate;
	/* resolution must be a multiple of two */
	m_pCodecCtx->width = w;
	m_pCodecCtx->height = h;
	/* frames per second */
	m_pCodecCtx->time_base.den = framerate;
	m_pCodecCtx->time_base.num = 1;
	m_pCodecCtx->gop_size = 10; /* emit one intra frame every ten frames */
	m_pCodecCtx->max_b_frames = 0;
	m_pCodecCtx->pix_fmt = (enum PixelFormat)Format;
	m_pCodecCtx->qmin = 2;
	m_pCodecCtx->qmax = 4;
	return 0;
}
//param src:    input av stream data buffer ptr;
//param srclen: inp data length;
//param dst:    output buffer ptr;
//param dstlen: output buffer length;
//return :      out data length;
int Ccodec::Codec_Encode(unsigned char *src, int srclen, unsigned char *dst, int dstlen)
{
	int result;
	if (!(m_nflag & CODEC_ENCODE_OPEN))  return -1;

	//Thr_LockMutex(hMutexCodec);;
	avpicture_fill((AVPicture *)m_pAVFrame, src, m_pCodecCtx->pix_fmt,
		m_pCodecCtx->width, m_pCodecCtx->height);

	result = avcodec_encode_video(m_pCodecCtx, dst, dstlen, m_pAVFrame);

	// 	ZDebug(this, "bit_rate: %d\n",m_pCodecCtx->bit_rate);
	// 	ZDebug(this, "bit_rate_tolerance: %d\n",m_pCodecCtx->bit_rate_tolerance);
	// 	ZDebug(this, "i_quant_factor: %d\n",m_pCodecCtx->i_quant_factor);
	// 	ZDebug(this, "mpeg_quant: %d\n",m_pCodecCtx->mpeg_quant);
	// 	ZDebug(this, "qmin: %d\n",m_pCodecCtx->qmin);
	// 	ZDebug(this, "qmax: %d\n",m_pCodecCtx->qmax);
	// 	ZDebug(this, "qmax: %f\n",m_pCodecCtx->time_base.num / (double)m_pCodecCtx->time_base.den);
	//Thr_UnLockMutex(hMutexCodec);
	return result;
}
#endif
