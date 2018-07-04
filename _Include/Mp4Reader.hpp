#pragma once


#ifdef __cplusplus
extern "C" {
#endif

#include "libswscale/swscale.h"
#include "libavcodec/avcodec.h"
#include "libavformat/avformat.h"
#include <libavutil/mathematics.h>
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavdevice/avdevice.h>
#ifdef __cplusplus
}
#endif

#ifdef _PLATFORM_WINDOW
#pragma comment(lib, "avutil.lib")
#pragma comment(lib, "avformat.lib")
#pragma comment(lib, "avcodec.lib")
#pragma comment(lib, "swscale.lib")
#pragma comment(lib, "avdevice.lib")
#endif

class CMp4Reader
{
public:
	CMp4Reader(const char * filename)
	{
		memset(m_filename, 0, sizeof(m_filename));
		memcpy(m_filename, filename, strlen(filename));

		av_register_all();
		avcodec_register_all();
		pFormatCtx = NULL;
		pFrame = NULL;
		pFrameYUV = NULL;
		//packet = NULL;
	};
	~CMp4Reader();

public:
	int ReadStart()
	{
		//
		pFormatCtx = avformat_alloc_context();
		if (!pFormatCtx)
		{
			return -1;
		}
		//
		if (0 != avformat_open_input(&pFormatCtx, m_filename, NULL, NULL))
		{
			ReadStop();
			return -2;
		}
		//
		if (avformat_find_stream_info(pFormatCtx, NULL) < 0)
		{
			//printf("Couldn't find stream information.\n");
			ReadStop();
			return -3;
		}
		videoindex = -1;
		for (unsigned int i = 0; i < pFormatCtx->nb_streams; i++)
		{
			if (pFormatCtx->streams[i]->codec->codec_type == AVMEDIA_TYPE_VIDEO)
			{
				videoindex = i;
				break;
			}
		}
		if (videoindex == -1)
		{
			//printf("Couldn't find a video stream.\n");
			ReadStop();
			return -4;
		}
#if 0
		//
		pCodecCtx = pFormatCtx->streams[videoindex]->codec;
		pCodec = avcodec_find_decoder(pCodecCtx->codec_id);
		if (pCodec == NULL)
		{
			//printf("Codec not found.\n");
			ReadStop();
			return -5;
		}
		//
		if (avcodec_open2(pCodecCtx, pCodec, NULL) < 0)
		{
			//printf("Could not open codec.\n");
			ReadStop();
			return -6;
		}
#endif
		//
		pFrame = av_frame_alloc();
		pFrameYUV = av_frame_alloc();

		return 0;
	};
	int ReadData(bool *isVideo, char * data, int *nLen)
	{

#if 0
		AVPacket *packet = (AVPacket *)av_malloc(sizeof(AVPacket));

		if (av_read_frame(pFormatCtx, packet) >= 0)//即读取音频若干帧或者视频一帧，返回上层调用
		{
			if (packet->stream_index == videoindex)
			{
				*isVideo = true;
				*nLen = packet->size;
				memcpy(data, packet->data, packet->size);
			}
		}
		av_free(packet);
#endif

#if 1
		AVPacket packet;
		//av_init_packet(&packet);
		if (av_read_frame(pFormatCtx, &packet) >= 0)//即读取音频若干帧或者视频一帧，返回上层调用
		{
			if (packet.stream_index == videoindex)
			{
				*isVideo = true;
				*nLen = packet.size;
				memcpy(data, packet.data, packet.size);
			}
			else
			{
				*isVideo = false;
				*nLen = packet.size;
				memcpy(data, packet.data, packet.size);
			}

			return 0;
		}
#endif
		return -1;
	};//读一帧
	int ReadStop()
	{
		if (pFrame)
		{
			av_free(pFrame);
			pFrame = NULL;
		}

		if (pFrameYUV)
		{
			av_free(pFrameYUV);
			pFrameYUV = NULL;
		}

		if (pFormatCtx)
		{
			//avcodec_close(pCodecCtx);
			avformat_close_input(&pFormatCtx);
			pFormatCtx = NULL;
		}

		return 0;

	}
	;

private:
	char m_filename[256];

private:
	AVFormatContext *pFormatCtx;
	int				videoindex;
	//AVCodecContext  *pCodecCtx;//= pFormatCtx->streams[videoindex]->codec;
	//AVCodec         *pCodec; // = avcodec_find_decoder(pCodecCtx->codec_id)


	AVFrame *pFrame, *pFrameYUV;
	//AVPacket *packet;
};

