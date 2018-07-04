#pragma once

#include "PlatformDefine.h"

//--------------------------------------------------------------
// https://github.com/kirainstorm/ffmpegmp4
// author: kirainstorm
// ������޸���ĳЩ���룬ϣ��������������֪���������ǹ�ͬ���Ƹô���
//--------------------------------------------------------------


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











//AVFormatContext �������AVStream����avformat_new_stream���ɣ���_fmt_ctx->streams[xx]��Ӧ
#define WRITE_AUDIO_VIDEO 1
#define WRITE_AUDIO_AAC 1

//blog.csdn.net/leixiaohua1020/article/details/39802913/
/*
FIX: H.264 in some container format (FLV, MP4, MKV etc.) need
"h264_mp4toannexb" bitstream filter (BSF)
  *Add SPS,PPS in front of IDR frame
  *Add start code ("0,0,0,1") in front of NALU
H.264 in some container (MPEG2TS) don't need this BSF.
*/
//'1': Use H.264 Bitstream Filter   
#define USE_H264BSF WRITE_AUDIO_VIDEO

/*
FIX:AAC in some container format (FLV, MP4, MKV etc.) need
"aac_adtstoasc" bitstream filter (BSF)
*/
//'1': Use AAC Bitstream Filter   
#define USE_AACBSF WRITE_AUDIO_AAC


//AAC��������
//16bit��������Ϊ0�����Ǿ���
enum WriteDataType
{
	WRITE_TYPE_H264 = 0,
	WRITE_TYPE_H265,
};

#define MAX_FILE_SIZE (128*1024*1024)


class CMp4Writer
{
public:
	CMp4Writer(enum WriteDataType Type,
		const char * filepath,
		int videoWidth, int videoHeight, int videoFrameRate, int videoGopSize,
		int audioChannels, int audioSamplesPerSec, int audioBitsPerSample, int audioBitRate, int audio_frame_size)
	{

		CROSS_TRACE("### %d ### %d ### %d ### %d ### %d ### %d ### %d ### %d ###%d",
			videoWidth, videoHeight, videoFrameRate, videoGopSize, audioChannels, audioSamplesPerSec, audioBitsPerSample, audioBitRate, m_audioFrameSize);

		opts = NULL;
		optsAudio = NULL;
		//
		m_VideoCodecType = Type;
		//
		memset(m_filepath, 0, sizeof(m_filepath));
		memcpy(m_filepath, filepath, strlen(filepath));
		//
		m_width = videoWidth;
		m_height = videoHeight;
		m_framerate = videoFrameRate;
		m_gopsize = videoGopSize;
		//
		m_audioChannels = audioChannels;
		m_audioSamplesPerSec = audioSamplesPerSec;
		m_audioBitsPerSample = audioBitsPerSample;
		m_audioBitRate = audioBitRate;
		m_audioFrameSize = audio_frame_size;
		//
		o_fmt_ctx = NULL;
#if WRITE_AUDIO_VIDEO
		o_video_stream = NULL;
#endif

#if WRITE_AUDIO_AAC
		o_audio_stream = NULL;
		memset(m_szMuteAudio, 0, sizeof(m_szMuteAudio));
#endif
		//FIX  
#if USE_H264BSF
		h264bsfc = av_bitstream_filter_init("h264_mp4toannexb");
#endif
#if USE_AACBSF
		aacbsfc = av_bitstream_filter_init("aac_adtstoasc");
#endif

		//
		avcodec_register_all();
		//
		av_register_all();
		//Network  
		avformat_network_init();
	};
	~CMp4Writer()
	{
		WriteStop();
#if USE_H264BSF
		av_bitstream_filter_close(h264bsfc);
#endif
#if USE_AACBSF
		av_bitstream_filter_close(aacbsfc);
#endif

	};
private:
	//////////////////////////////////////////////////////////////////////////
	AVDictionary *opts;
	AVDictionary *optsAudio;
	//////////////////////////////////////////////////////////////////////////


public:
	int WriteData(BOOL isVideo, BOOL isVideoI, const char * data, int nLen)
	{
		int ret = -1;
		m_cs.Lock();


		do
		{
			//CROSS_TRACE("CMp4Writer::WriteData 111 \n");
			//--------------------------------------------------------
			if (NULL == o_fmt_ctx)
			{
				WriteStop();
				if (0 != WriteStart())
				{
					break;;
				}
			}
			//CROSS_TRACE("CMp4Writer::WriteData 222 \n");
			//--------------------------------------------------------
			if (m_nFileLen > MAX_FILE_SIZE)
			{
				WriteStop();
				if (0 != WriteStart())
				{
					break;;
				}
			}

			if (m_bNeadIframe)
			{
				if (!isVideoI)
				{
					break;
				}
				m_bNeadIframe = FALSE;
			}

			//������������Ƶ��ͬ������Ϊ���˺ܶ���Ƶ
			// 		if (m_bFirstVideoCount < m_framerate)//��¼1�봿��Ƶ������ʱ�ͻ�������ͼ�����������Ҫ��1�����ͼ��
			// 		{
			// 			if (!isVideo)
			// 			{
			// 				m_bFirstVideoCount++;
			// 				break;
			// 			}
			// 		}

			//CROSS_TRACE("CMp4Writer::WriteData --- rec --- isVideo=%d    isVideoI=%d \n", isVideo, isVideoI);

			m_nFileLen += nLen;

			//CROSS_TRACE("CMp4Writer::WriteData 333 \n");
			//--------------------------------------------------------
			AVPacket pkt;
			av_init_packet(&pkt);
			pkt.data = (uint8_t *)data;
			pkt.size = nLen;
			//FIX:Bitstream Filter  






#if WRITE_AUDIO_AAC
#if WRITE_AUDIO_VIDEO
			pkt.stream_index = isVideo ? o_video_stream_index : o_audio_stream_index;
#else
			pkt.stream_index = o_audio_stream_index;
#endif

#else
			pkt.stream_index = o_video_stream_index;
#endif
			last_pts += 1024;
			last_dts += 1024;
			//
			if (isVideo)
			{
#if WRITE_AUDIO_VIDEO
#if USE_H264BSF
				//av_bitstream_filter_filter(h264bsfc, o_video_stream->codec, NULL, &pkt.data, &pkt.size, pkt.data, pkt.size, 0);
#endif  
				pkt.flags |= isVideoI ? AV_PKT_FLAG_KEY : 0;
				pkt.pts = (o_video_stream->time_base.den / o_video_stream->time_base.num) * o_video_stream_count / (o_video_stream->r_frame_rate.den / o_video_stream->r_frame_rate.num);
				pkt.dts = (o_video_stream->time_base.den / o_video_stream->time_base.num) * o_video_stream_count / (o_video_stream->r_frame_rate.den / o_video_stream->r_frame_rate.num);
				o_video_stream_count++;
				if (0 != av_interleaved_write_frame(o_fmt_ctx, &pkt))
				{
					CROSS_TRACE("CMp4Writer::WriteData  ------ error \n");
				}
#endif
			}
#if WRITE_AUDIO_AAC
			else
			{
#if USE_AACBSF  
				av_bitstream_filter_filter(aacbsfc, o_audio_stream->codec, NULL, &pkt.data, &pkt.size, pkt.data, pkt.size, 0);
#endif
				pkt.flags |= AV_PKT_FLAG_KEY;
				pkt.pts = (o_video_stream->time_base.den / o_video_stream->time_base.num) * o_audio_stream_count / (o_video_stream->r_frame_rate.den / o_video_stream->r_frame_rate.num);
				pkt.dts = (o_video_stream->time_base.den / o_video_stream->time_base.num) * o_audio_stream_count / (o_video_stream->r_frame_rate.den / o_video_stream->r_frame_rate.num);
				o_audio_stream_count++;
				if (0 != av_interleaved_write_frame(o_fmt_ctx, &pkt))
				{
					CROSS_TRACE("CMp4Writer::WriteData  ------ error \n");
				}
			}
#endif
			//CROSS_TRACE("CMp4Writer::WriteData 444 \n");

		} while (0);

		m_cs.Unlock();
		return 0;

	};

private:
	int WriteStart()
	{

		m_bNeadIframe = TRUE;
		m_bFirstVideoCount = 0;

		int ret = 0;
		//char szPath[MAX_PATH] = { 0 };
		char szName[MAX_PATH] = { 0 };
		AVCodec *codec = NULL;
		m_nFileLen = 0;
		last_pts = 0;
		last_dts = 0;
		//framecnt = 0;
#if WRITE_AUDIO_VIDEO
		o_video_stream_count = 0;
#endif
#if WRITE_AUDIO_AAC
		o_audio_stream_count = 0;
#endif


		do
		{

			if ((m_width <= 0) || (m_height <= 0) || (m_framerate <= 0) || (m_gopsize <= 0)
				|| (m_audioChannels <= 0) || (m_audioSamplesPerSec <= 0) || (m_audioBitsPerSample <= 0)
				)
			{
				ret = -1;
				CROSS_TRACE("CMp4Writer::WriteStart  ------ 111 \n");
				break;
			}


#ifdef _PLATFORM_WINDOW
			SYSTEMTIME   sysTime;
			GetLocalTime(&sysTime);
			//
			//sprintf_s(szPath, "%s\\%04d%02d%02d", m_filepath, sysTime.wYear, sysTime.wMonth, sysTime.wDay);
			//
			sprintf_s(szName, "%s\\%04d%02d%02d_%02d%02d%02d_%03d.mp4", m_filepath, sysTime.wYear, sysTime.wMonth, sysTime.wDay, sysTime.wHour, sysTime.wMinute, sysTime.wSecond, sysTime.wMilliseconds);
			//
			if (0 != CreatDir(m_filepath))
			{
				ret = -2;
				break;
			}
#endif

#if (defined _PLATFORM_ANDROID) || (defined _PLATFORM_IOS)
			struct tm *p;
			time_t t;
			struct timeval _start;
			time(&t);
			p = localtime(&t);
			gettimeofday(&_start, NULL);
			//
			//sprintf(szPath, "%s/%04d%02d%02d", m_filepath, 1900 + p->tm_year, 1 + p->tm_mon, p->tm_mday);
			//
			sprintf(szName, "%s/%04d%02d%02d%02d%02d%02d%03d.mp4", m_filepath, 1900 + p->tm_year, 1 + p->tm_mon, p->tm_mday, p->tm_hour, p->tm_min, p->tm_sec, (int)(_start.tv_usec / 1000));
			//
			if (0 != CreatDir(m_filepath))
			{
				CROSS_TRACE("CMp4Writer::WriteStart  ------ 222 \n");
				ret = -2;
				break;
			}
#endif


			//
			avformat_alloc_output_context2(&o_fmt_ctx, NULL, "mp4", szName);
			if (!o_fmt_ctx)
			{
				CROSS_TRACE("CMp4Writer::WriteStart  ------ 333 \n");
				ret = -3;
				break;
			}

			//
			if (m_VideoCodecType == WRITE_TYPE_H264)
			{
				o_fmt_ctx->oformat->video_codec = AV_CODEC_ID_H264;
			}
			if (m_VideoCodecType == WRITE_TYPE_H265)
			{
				o_fmt_ctx->oformat->video_codec = AV_CODEC_ID_H265;
			}
			o_fmt_ctx->oformat->audio_codec = AV_CODEC_ID_AAC;

#if WRITE_AUDIO_VIDEO
			////
			codec = avcodec_find_encoder(o_fmt_ctx->oformat->video_codec);
			CROSS_TRACE("CMp4Writer::WriteStart  ------#####======########----------find video lib = %s\n", codec->name);
			if (!codec)
			{
				CROSS_TRACE("CMp4Writer::WriteStart  ------ 444 --- %d\n", o_fmt_ctx->oformat->video_codec);
				ret = -4;
				break;
			}
			////
			o_video_stream = avformat_new_stream(o_fmt_ctx, codec);
			if (!o_video_stream)
			{
				CROSS_TRACE("CMp4Writer::WriteStart  ------ 555 \n");
				ret = -5;
				break;
			}

#endif



#if WRITE_AUDIO_VIDEO
			//----------------------------------------------------------------------------------------------
			//
			o_video_stream_index = o_video_stream->index;//<<<<<<<<<<<<<<<avformat_new_stream���Զ�����index
			avcodec_get_context_defaults3(o_video_stream->codec, codec);
			//
			if (m_VideoCodecType == WRITE_TYPE_H264){
				o_video_stream->codec->codec_id = AV_CODEC_ID_H264;
			}
			if (m_VideoCodecType == WRITE_TYPE_H265){
				o_video_stream->codec->codec_id = AV_CODEC_ID_H265;
			}
			o_video_stream->codec->bit_rate = /*400000*/1000000;
			o_video_stream->codec->codec_type = AVMEDIA_TYPE_VIDEO;
			//
			o_video_stream->codec->width = m_width;
			o_video_stream->codec->height = m_height;
			o_video_stream->codec->time_base.den = m_framerate;
			o_video_stream->codec->time_base.num = 1;
			o_video_stream->codec->gop_size = m_gopsize; /* emit one intra frame every twelve frames at most */
			o_video_stream->codec->pix_fmt = AV_PIX_FMT_YUV420P;//AV_PIX_FMT_YUVJ420P;
			//
			o_video_stream->codec->profile = FF_PROFILE_H264_BASELINE;
			//
			o_video_stream->time_base.den = m_framerate;//Ѹ����90000��Ȼ����....
			o_video_stream->time_base.num = 1;
			//
			o_video_stream->r_frame_rate.den = m_framerate;
			o_video_stream->r_frame_rate.num = 1;


			// 		o_video_stream->codec->bit_rate = 400000;
			// 		o_video_stream->codec->me_range = i_video_stream->codec->me_range;
			// 		o_video_stream->codec->max_qdiff = i_video_stream->codec->max_qdiff;
			// 		o_video_stream->codec->qmin = i_video_stream->codec->qmin;
			// 		o_video_stream->codec->qmax = i_video_stream->codec->qmax;
			// 		o_video_stream->codec->qcompress = i_video_stream->codec->qcompress;

			//o_video_stream->codec->codec_tag = 0;
			//
			if (o_fmt_ctx->oformat->flags & AVFMT_GLOBALHEADER)
				o_video_stream->codec->flags |= CODEC_FLAG_GLOBAL_HEADER;


			//ֻ����baseline��ϵͳ����������ʶ�𲥷š�������
			av_dict_set(&opts, "profile", "baseline", 0);
			if (avcodec_open2(o_video_stream->codec, codec, &opts) < 0)
			{
				CROSS_TRACE("CMp4Writer::WriteStart  ------ xxxxx\n");
				ret = -888;
				break;
			}


			// 		// Set H264 preset and tune  
			// 		AVDictionary *param = 0;
			// 		av_dict_set(&param, "preset", "ultrafast", 0);
			// 		av_dict_set(&param, "tune", "zerolatency", 0);

#endif
			//----------------------------------------------------------------------------------------------
			//
			//
			//
			//----------------------------------------------------------------------------------------------
			//
#if WRITE_AUDIO_AAC
			//
			//��ע��ffmpeg 3.0���Ƴ���libvo_aacenc,����windows������ms�Ķ���
			//����дMP4��android������ʹ��2.8.7�汾����дmp4�Ŀ�����.so����������ʹ��
			//android 2.8.7�汾����ʱ������--enable-libvo-aacenc  
			//
			//��Ϊ��ͬƽ̨�µ�Ĭ��MP4��Ƶ����ⲻ��������������д�롣

			//codec = avcodec_find_encoder(o_fmt_ctx->oformat->audio_codec);
#ifdef _PLATFORM_IOS
			//ios use ffmpeg-3.1.1
			//system player can play it with sound
			codec = avcodec_find_encoder_by_name("libfdk_aac");

#else
			//todu:upgrade "pc/android" other day
			codec = avcodec_find_encoder_by_name("libvo_aacenc");//��ע������//androd ����ʱ������--enable-libvo-aacenc
#endif

			CROSS_TRACE("CMp4Writer::WriteStart  ------#####======########----------find audio lib = %s\n", codec->name);
			if (!codec)
			{
				CROSS_TRACE("CMp4Writer::WriteStart  ------ 666 \n");
				ret = -6;
				break;
			}
			////
			o_audio_stream = avformat_new_stream(o_fmt_ctx, codec);
			if (!o_audio_stream)
			{
				CROSS_TRACE("CMp4Writer::WriteStart  ------ 777 \n");
				ret = -7;
				break;
			}
#endif
#if WRITE_AUDIO_AAC
			o_audio_stream_index = o_audio_stream->index;//<<<<<<<<<<<<<<<avformat_new_stream���Զ�����index
			avcodec_get_context_defaults3(o_audio_stream->codec, codec);
			//
			o_audio_stream->codec->codec_id = AV_CODEC_ID_AAC;
			o_audio_stream->codec->codec_type = AVMEDIA_TYPE_AUDIO;
			//
			if (16 == m_audioBitsPerSample)
			{
				o_audio_stream->codec->sample_fmt = AV_SAMPLE_FMT_S16;
			}

			o_audio_stream->codec->sample_rate = m_audioSamplesPerSec;//8000
			o_audio_stream->codec->channels = m_audioChannels;//1
			o_audio_stream->codec->bit_rate = m_audioBitRate;
			o_audio_stream->codec->frame_size = m_audioFrameSize;
			//
			//o_audio_stream->codec->time_base.den = m_audioSamplesPerSec;
			//o_audio_stream->codec->time_base.num = 1;

			//o_audio_stream->r_frame_rate.den = m_framerate;
			//o_audio_stream->r_frame_rate.num = 1;

			o_audio_stream->codec->channel_layout = AV_CH_LAYOUT_MONO;


			o_audio_stream->codec->codec_tag = 0;
			//
			if (o_fmt_ctx->oformat->flags & AVFMT_GLOBALHEADER)/* Some formats want stream headers to be separate. */
				o_audio_stream->codec->flags |= CODEC_FLAG_GLOBAL_HEADER;
			//----------------------------------------------------------------------------------------------
			/*
			����������AAC�⣺

			- FAAC, FAAD/FAAD2    ---����ֻ��AAC LC
			- neroAACenc ---����ҵ����ʹ�ã��ṩ���빤�ߣ�
			- FFmpeg's native AAC encoder(part of libavcodec), experimental by the developers as of December 2010   ---ֻ��AAC LC������������
			- libvo_aacenc, the Android VisualOn AAC encoder   ---ֻ��AAC LC�� opencore-amr-vo-aacenc��ֻ�б���
			- libfdk-aac, the Fraunhofer FDK AAC libray  ---������HE-AACv2�������ܽϺ�   opencore-amr-fdk-aac
			- libaacplus, 3GPP released reference implementations 3GPP High Efficiency Advanced Audio Codec (HE-AAC) Codec (3GPP TS 26.410 V 8.0.0).  --ֻ�б��룬��Enhanced aacPlus general audio codec; Floating-point ANSI-C code��
			*/
			//windows ffmpeg Ĭ��ѡȡ����libvo_aacenc��������ֻ��AAC LC��������windowsϵͳ�Դ��������ܲ��ų�����
			//x264�������Ƶ��ʹ��libvo_aacencѹ��aac lcд��MP4�ֻ���Ȼ���Բ�������
			//
			//�ֻ���ffmpegдMP4���ӵ�aac������libvo_aacenc��������--enable-libvo-aacenc��libvo_aacenc���Դ��ġ���
			//
			//
			o_audio_stream->codec->profile = FF_PROFILE_AAC_LOW; //aac_lc

			//ֻ����baseline��ϵͳ����������ʶ�𲥷š�������
			//av_dict_set(&optsAudio, "profile", "aac_lc", 0);

			if (avcodec_open2(o_audio_stream->codec, codec, NULL) < 0)
			{
				CROSS_TRACE("CMp4Writer::WriteStart  ------ xxxxx audio\n");
				ret = -999;
				break;
			}
#endif


			//if (!(o_fmt_ctx->oformat->flags & AVFMT_NOFILE)) /* open the output file, if needed */
			//{
			if (avio_open(&o_fmt_ctx->pb, szName, AVIO_FLAG_WRITE) < 0)
			{
				CROSS_TRACE("CMp4Writer::WriteStart  ------ 888 %s\n", szName);
				ret = -8;
				break;
			}
			//}



			/* Write the stream header, if any. */
			int rrr = 0;
			if ((rrr = avformat_write_header(o_fmt_ctx, NULL)) < 0)
			{
				CROSS_TRACE("CMp4Writer::WriteStart  ------ 999 - %d - %s\n", rrr, szName);
				ret = -9;
				break;
			}


			ret = 0;
		} while (0);

		if (0 != ret)
		{
			if (o_fmt_ctx)
			{
				avio_close(o_fmt_ctx->pb);
				av_free(o_fmt_ctx);
				o_fmt_ctx = NULL;
			}


			WriteStop();
		}

		return ret;
	}
	;
	int WriteStop()
	{
		//
		int n = 0;
		if (o_fmt_ctx)
		{
			n = av_write_trailer(o_fmt_ctx);
		}
#if WRITE_AUDIO_VIDEO
		//
		if (o_video_stream)
		{
			avcodec_close(o_video_stream->codec);
			av_freep(&o_video_stream->codec);
			av_freep(&o_video_stream);
			o_video_stream = NULL;
		}
#endif

#if WRITE_AUDIO_AAC
		//
		if (o_audio_stream)
		{
			avcodec_close(o_audio_stream->codec);
			av_freep(&o_audio_stream->codec);
			av_freep(&o_audio_stream);
			o_audio_stream = NULL;
		}
#endif
		//
		if (o_fmt_ctx)
		{
			avio_close(o_fmt_ctx->pb);
			av_free(o_fmt_ctx);
			o_fmt_ctx = NULL;
		}
		return 0;
	};

private:
	//264/265
	WriteDataType m_VideoCodecType;
	//�ļ�·��
	char m_filepath[256];
	//video
	int m_width, m_height, m_framerate, m_gopsize;//��--��--֡��--i֡���
	//audio
	int m_audioChannels, m_audioSamplesPerSec, m_audioBitsPerSample, m_audioBitRate,m_audioFrameSize;//ͨ��(������Ϊ 1, ������Ϊ 2)--ÿ���������--������С

private:
	//ctx
	AVFormatContext *o_fmt_ctx;
#if WRITE_AUDIO_VIDEO
	//video
	AVStream  *o_video_stream;
	int o_video_stream_index;
	int o_video_stream_count;
#if USE_H264BSF
	AVBitStreamFilterContext* h264bsfc;
#endif
#endif


#if WRITE_AUDIO_AAC
	//
	AVStream  *o_audio_stream; //���ܴ�g711��pcm�������Դ�aac
	int o_audio_stream_index;
	int o_audio_stream_count;
	char m_szMuteAudio[4096];
	AVBitStreamFilterContext* aacbsfc;// = av_bitstream_filter_init("aac_adtstoasc");
#endif

	//
	//int64_t framecnt;
	int64_t last_pts;
	int64_t last_dts;

	BOOL m_bNeadIframe;
	int m_bFirstVideoCount;//��д1��֡�ʵ���Ƶ����д��Ƶ ������������Ƶ��ͬ��
private:
	CrossCriticalSection m_cs;
	int32_t m_nFileLen;//��ǰд���ļ��ĳ���,,����MAX_FILE_SIZEҪ�л��ļ�
	//int32_t m_nFileTime;//��ǰд���ļ���ʱ�䣬���� ����Ҫ�л��ļ�
};

