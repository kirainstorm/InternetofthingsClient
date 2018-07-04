

#ifndef FDKAAC_ASLKD23KMB2HJDKWEU23IENDMSNMXXMCNVWIO
#define FDKAAC_ASLKD23KMB2HJDKWEU23IENDMSNMXXMCNVWIO

#include "PlatformDefine.h"

#include "genericStds.h"
#include "machine_type.h"
#include "FDK_audio.h"
#include "aacenc_lib.h"
#include "aacdecoder_lib.h"
#ifdef _PLATFORM_WINDOW
#pragma comment(lib, "rsaac.dll.a")
#endif


extern "C"
{
#include "libavformat/avformat.h"
#include "libavcodec/avcodec.h"
#include "libswscale/swscale.h"
}



//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
#define PROFILE_AAC_MAIN 0
#define PROFILE_AAC_LOW  1
#define PROFILE_AAC_SSR  2
#define PROFILE_AAC_LTP  3
#define PROFILE_AAC_HE   4
#define PROFILE_AAC_HE_V2 28
#define PROFILE_AAC_LD   22
#define PROFILE_AAC_ELD  38
#define PROFILE_MPEG2_AAC_LOW 128
#define PROFILE_MPEG2_AAC_HE  131



static const char *aac_get_error(AACENC_ERROR err)
{
	switch (err) {
	case AACENC_OK:
		return "No error";
	case AACENC_INVALID_HANDLE:
		return "Invalid handle";
	case AACENC_MEMORY_ERROR:
		return "Memory allocation error";
	case AACENC_UNSUPPORTED_PARAMETER:
		return "Unsupported parameter";
	case AACENC_INVALID_CONFIG:
		return "Invalid config";
	case AACENC_INIT_ERROR:
		return "Initialization error";
	case AACENC_INIT_AAC_ERROR:
		return "AAC library initialization error";
	case AACENC_INIT_SBR_ERROR:
		return "SBR library initialization error";
	case AACENC_INIT_TP_ERROR:
		return "Transport library initialization error";
	case AACENC_INIT_META_ERROR:
		return "Metadata library initialization error";
	case AACENC_ENCODE_ERROR:
		return "Encoding error";
	case AACENC_ENCODE_EOF:
		return "End of file";
	default:
		return "Unknown error";
	}
}
typedef struct {
	int sample_rate; ///< samples per second
	int channels;    ///< number of audio channels
	/**
	* Audio cutoff bandwidth (0 means "automatic")
	* - encoding: Set by user.
	* - decoding: unused
	*/
	int cutoff;
	/**
	* CODEC_FLAG_*.
	* - encoding: Set by user.
	* - decoding: Set by user.
	*/
	int flags;
	/**
	* profile
	* - encoding: Set by user.
	* - decoding: Set by libavcodec.
	*/
	int profile;
	/**
	* the average bitrate
	* - encoding: Set by user; unused for constant quantizer encoding.
	* - decoding: Set by libavcodec. 0 or some bitrate if this info is available in the stream.
	*/
	int bit_rate;
	/**
	* Global quality for codecs which cannot change it per frame.
	* This should be proportional to MPEG-1/2/4 qscale.
	* - encoding: Set by user.
	* - decoding: unused
	*/
	int global_quality;
	//AACContext
	int afterburner;
	int eld_sbr;
	int signaling;
	int latm;
	int header_period;
	int vbr;
} AACEncoder_t;
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
enum ConcealMethod {
	CONCEAL_METHOD_DEFAULT = -1,
	CONCEAL_METHOD_SPECTRAL_MUTING = 0,
	CONCEAL_METHOD_NOISE_SUBSTITUTION = 1,
	CONCEAL_METHOD_ENERGY_INTERPOLATION = 2,
	CONCEAL_METHOD_NB,
};

typedef struct {
	char *extradata;
	int extradata_size;
	ConcealMethod conceal_method;
} AACDecoder_t;
//////////////////////////////////////////////////////////////////////////
typedef struct FDKAACDecContext {
	HANDLE_AACDECODER handle;
	int initialized;
	enum ConcealMethod conceal_method;
} FDKAACDecContext;




typedef struct AACContext {
	HANDLE_AACENCODER handle;
	int afterburner;
	int eld_sbr;
	int signaling;
	int latm;
	int header_period;
	int vbr;
} AACContext;

class CFdkaac :public CXVoiceInterface
{
public:
	CFdkaac()
	{
		//////////////////////////////////////////////////////////////////////////
		//计算得出wBitsPerSample为16
		memset(&m_AACEncoder, 0, sizeof(m_AACEncoder));
		m_AACEncoder.sample_rate = 8000;
		m_AACEncoder.channels = 1;
		m_AACEncoder.cutoff = 0;
		m_AACEncoder.flags = 0;
		m_AACEncoder.profile = PROFILE_AAC_LOW;
		m_AACEncoder.bit_rate = m_AACEncoder.sample_rate << 1;//16000

		//////////////////////////////////////////////////////////////////////////
		memset(&m_AACDecoder, 0, sizeof(m_AACDecoder));
		m_AACDecoder.conceal_method = CONCEAL_METHOD_NOISE_SUBSTITUTION;


		//////////////////////////////////////////////////////////////////////////
		avctx = NULL;
	};
	~CFdkaac()
	{
	};


public:
	virtual void Delete()
	{
		delete this;
	}
	;

	virtual int InitVoice(BOOL bEncode)
	{
		m_bEncode = bEncode;

		if (m_bEncode)
		{
			if (0 != __aac_encode_init())
			{
				return -1;
			}
		}
		else
		{
			if (0 != __aac_decode_init())
			{
				return -1;
			}
		}

		return 0;
	};
	virtual int DeinitVoice()
	{
		if (NULL == avctx)
			return 0;

		if (m_bEncode)
		{
			AACContext *aac = (AACContext *)avctx->priv_data;
			if (aac->handle)
			{
				aacEncClose(&aac->handle);
				aac->handle = NULL;
			}

			if (avctx->extradata)
			{
				free(avctx->extradata);
				avctx->extradata = NULL;
			}

			if (avctx->priv_data)
			{
				free(avctx->priv_data);
				avctx->priv_data = NULL;
			}

			if (avctx)
			{
				free(avctx);
				avctx = NULL;
			}
		}
		else
		{
			FDKAACDecContext *s = (FDKAACDecContext *)avctx->priv_data;

			if (s->handle)
			{
				aacDecoder_Close(s->handle);
				s->handle = NULL;
			}

			if (avctx->extradata)
			{
				free(avctx->extradata);
				avctx->extradata = NULL;
			}

			if (avctx->priv_data)
			{
				free(avctx->priv_data);
				avctx->priv_data = NULL;
			}

			if (avctx)
			{
				free(avctx);
				avctx = NULL;
			}
		}


		return 0;
	};


	virtual int EncodeVoice(const void* pVoiceData, int lVoiceDataLen, void* pEncDataBuf, int* plEncDataLen)
	{

		if ((!pVoiceData) || (!pEncDataBuf) || (!lVoiceDataLen))
			return -1;

		int ret = -1;

		AACENC_BufDesc in_buf = { 0 }, out_buf = { 0 };
		AACENC_InArgs  in_args = { 0 };
		AACENC_OutArgs out_args = { 0 };
		int in_buffer_identifier = IN_AUDIO_DATA;
		int in_buffer_size, in_buffer_element_size;
		int out_buffer_identifier = OUT_BITSTREAM_DATA;
		int out_buffer_size, out_buffer_element_size;
		void *in_ptr, *out_ptr;
		AACENC_ERROR err;


		do
		{
			//
			in_ptr = (void *)pVoiceData;
			in_buffer_size = 0;
			in_buffer_element_size = 2;
			in_args.numInSamples = avctx->channels * avctx->frame_size;
			in_buf.numBufs = 1;
			in_buf.bufs = &in_ptr;
			in_buf.bufferIdentifiers = &in_buffer_identifier;
			in_buf.bufSizes = &in_buffer_size;
			in_buf.bufElSizes = &in_buffer_element_size;
			//
			out_ptr = pEncDataBuf;
			out_buffer_size = 2 * avctx->channels * avctx->frame_size;// *plEncDataLen;
			out_buffer_element_size = 1;
			out_buf.numBufs = 1;
			out_buf.bufs = &out_ptr;
			out_buf.bufferIdentifiers = &out_buffer_identifier;
			out_buf.bufSizes = &out_buffer_size;
			out_buf.bufElSizes = &out_buffer_element_size;
			//
			if ((err = aacEncEncode(((AACContext *)avctx->priv_data)->handle, &in_buf, &out_buf, &in_args, &out_args)) != AACENC_OK)
			{
				CROSS_TRACE("Unable to encode frame: %s\n", aac_get_error(err));
				break;
			}

			if (!out_args.numOutBytes)
				break;

			*plEncDataLen = out_args.numOutBytes;

			CROSS_TRACE("Fdkaac::EncodeVoice:   len = %d<<<<\n", out_args.numOutBytes);

			ret = 0;
		} while (0);

		return ret;
	};
	virtual int DecodeVoice(const void* pEncDataBuf, int nEncDataLen, void* pVoiceData, int* pnVoiceDataLen)
	{
		int ret = -1;
		AAC_DECODER_ERROR err;
		char *buf;
		int buf_size;
		UINT pkt_size = nEncDataLen;
		UINT valid_size = nEncDataLen;

		do
		{
			FDKAACDecContext *s = (FDKAACDecContext *)avctx->priv_data;
			HANDLE_AACDECODER *phaacdec = (HANDLE_AACDECODER *)s->handle;

			err = aacDecoder_Fill(s->handle, (unsigned char **)&pEncDataBuf, &pkt_size, &valid_size);
			if (err != AAC_DEC_OK) {
				CROSS_TRACE("aacDecoder_Fill() failed: %x\n", err);
				break;
			}

			buf = (char *)pVoiceData;
			buf_size = nEncDataLen;


			err = aacDecoder_DecodeFrame(s->handle, (INT_PCM *)buf, buf_size, 0);
			if (err == AAC_DEC_NOT_ENOUGH_BITS) {
				CROSS_TRACE("aacDecoder_DecodeFrame() failed: AAC_DEC_NOT_ENOUGH_BITS\n");
				break;
			}
			if (err != AAC_DEC_OK) {
				CROSS_TRACE("aacDecoder_DecodeFrame() failed: %x\n", err);
				break;
			}


			if (!s->initialized) {
				if ((ret = __aac_decode_get_stream_info()) < 0)
				{
					CROSS_TRACE("get_stream_info() failed\n");
					break;
				}
				s->initialized = 1;
			}

			*pnVoiceDataLen = avctx->channels * avctx->frame_size *av_get_bytes_per_sample(avctx->sample_fmt);
			//CROSS_TRACE("Fdkaac::DecodeVoice:   len = %d<<<<\n", *plVoiceDataLen);

			ret = 0;
		} while (0);

		return ret;
	};


private:
	BOOL m_bEncode;


private:
	AVCodecContext *avctx;
	//AVFrame *pFrame;


private:
	int __aac_encode_init()
	{
		int ret = -1;
		AACENC_InfoStruct info = { 0 };
		CHANNEL_MODE mode;
		AACENC_ERROR err;
		int aot = FF_PROFILE_AAC_LOW + 1;
		int sce = 0, cpe = 0;


		do
		{
			//---------------------------------------------------------------------------
			avctx = (AVCodecContext *)malloc(sizeof(AVCodecContext));
			if (!avctx)
			{
				CROSS_TRACE("CFdkaac::InitVoice  111");
				break;
			}
			memset(avctx, 0, sizeof(AVCodecContext));
			avctx->sample_rate = m_AACEncoder.sample_rate;
			avctx->channels = m_AACEncoder.channels;
			avctx->cutoff = m_AACEncoder.cutoff;
			avctx->flags = m_AACEncoder.flags;
			avctx->profile = m_AACEncoder.profile;
			avctx->bit_rate = m_AACEncoder.bit_rate;
			avctx->sample_fmt = AV_SAMPLE_FMT_S16;
			//---------------------------------------------------------------------------
			avctx->priv_data = malloc(sizeof(AACContext));
			if (!avctx->priv_data)
			{
				CROSS_TRACE("CFdkaac::InitVoice  222");
				break;
			}
			memset(avctx->priv_data, 0, sizeof(AACContext));
			AACContext *aac = (AACContext *)avctx->priv_data;

			aac->afterburner = m_AACEncoder.afterburner;
			aac->eld_sbr = m_AACEncoder.eld_sbr;
			aac->signaling = m_AACEncoder.signaling;
			aac->latm = m_AACEncoder.latm;
			aac->header_period = m_AACEncoder.header_period;
			aac->vbr = m_AACEncoder.vbr;
			//---------------------------------------------------------------------------
			//
			if ((err = aacEncOpen(&aac->handle, 0, avctx->channels)) != AACENC_OK)
			{
				CROSS_TRACE("Unable to open the encoder: %s\n", aac_get_error(err));
				break;
			}
			//
			if (avctx->profile != FF_PROFILE_UNKNOWN)
				aot = avctx->profile + 1;
			//
			if ((err = aacEncoder_SetParam(aac->handle, AACENC_AOT, aot)) != AACENC_OK)
			{
				CROSS_TRACE("Unable to set the AOT %d: %s\n", aot, aac_get_error(err));
				break;
			}
			//no
			if (aot == FF_PROFILE_AAC_ELD + 1 && aac->eld_sbr)
			{
				if ((err = aacEncoder_SetParam(aac->handle, AACENC_SBR_MODE, 1)) != AACENC_OK)
				{
					CROSS_TRACE("Unable to enable SBR for ELD: %s\n", aac_get_error(err));
					break;
				}
			}

			if ((err = aacEncoder_SetParam(aac->handle, AACENC_SAMPLERATE, avctx->sample_rate)) != AACENC_OK)
			{
				CROSS_TRACE("Unable to set the sample rate %d: %s\n", avctx->sample_rate, aac_get_error(err));
				break;
			}

			switch (avctx->channels)
			{
			case 1: mode = MODE_1;       sce = 1; cpe = 0; break;
			case 2: mode = MODE_2;       sce = 0; cpe = 1; break;
			case 3: mode = MODE_1_2;     sce = 1; cpe = 1; break;
			case 4: mode = MODE_1_2_1;   sce = 2; cpe = 1; break;
			case 5: mode = MODE_1_2_2;   sce = 1; cpe = 2; break;
			case 6: mode = MODE_1_2_2_1; sce = 2; cpe = 2; break;
			default:
				CROSS_TRACE("Unsupported number of channels %d\n", avctx->channels);
				break;
			}

			if ((err = aacEncoder_SetParam(aac->handle, AACENC_CHANNELMODE, mode)) != AACENC_OK)
			{
				CROSS_TRACE("Unable to set channel mode %d: %s\n", mode, aac_get_error(err));
				break;
			}

			if ((err = aacEncoder_SetParam(aac->handle, AACENC_CHANNELORDER, 1)) != AACENC_OK)
			{
				CROSS_TRACE("Unable to set wav channel order %d: %s\n", mode, aac_get_error(err));
				break;
			}

			if (avctx->flags & CODEC_FLAG_QSCALE || aac->vbr)
			{
				int mode = aac->vbr ? aac->vbr : avctx->global_quality;
				if (mode <  1 || mode > 5)
				{
					CROSS_TRACE("VBR quality %d out of range, should be 1-5\n", mode);
					mode = av_clip(mode, 1, 5);
				}
				CROSS_TRACE("Note, the VBR setting is unsupported and only works with ""some parameter combinations\n");
				if ((err = aacEncoder_SetParam(aac->handle, AACENC_BITRATEMODE, mode)) != AACENC_OK)
				{
					CROSS_TRACE("Unable to set the VBR bitrate mode %d: %s\n", mode, aac_get_error(err));
					break;
				}
			}
			else
			{
				if (avctx->bit_rate <= 0)
				{
					if (avctx->profile == FF_PROFILE_AAC_HE_V2)
					{
						sce = 1;
						cpe = 0;
					}
					avctx->bit_rate = (96 * sce + 128 * cpe) * avctx->sample_rate / 44;
					if (avctx->profile == FF_PROFILE_AAC_HE ||
						avctx->profile == FF_PROFILE_AAC_HE_V2 ||
						avctx->profile == FF_PROFILE_MPEG2_AAC_HE ||
						aac->eld_sbr)
						avctx->bit_rate /= 2;
				}

				if ((err = aacEncoder_SetParam(aac->handle, AACENC_BITRATE, (UINT)avctx->bit_rate)) != AACENC_OK)
				{
					CROSS_TRACE("Unable to set the bitrate %d: %s\n", avctx->bit_rate, aac_get_error(err));
					break;
				}
			}

			/* Choose bitstream format - if global header is requested, use
			* raw access units, otherwise use ADTS. */
			if ((err = aacEncoder_SetParam(aac->handle, AACENC_TRANSMUX,
				avctx->flags & CODEC_FLAG_GLOBAL_HEADER ? 0 : aac->latm ? 10 : 2)) != AACENC_OK)
			{
				CROSS_TRACE("Unable to set the transmux format: %s\n", aac_get_error(err));
				break;
			}

			if (aac->latm && aac->header_period)
			{
				if ((err = aacEncoder_SetParam(aac->handle, AACENC_HEADER_PERIOD, aac->header_period)) != AACENC_OK)
				{
					CROSS_TRACE("Unable to set header period: %s\n", aac_get_error(err));
					break;
				}
			}

			/* If no signaling mode is chosen, use explicit hierarchical signaling
			* if using mp4 mode (raw access units, with global header) and
			* implicit signaling if using ADTS. */
			if (aac->signaling < 0)
				aac->signaling = avctx->flags & CODEC_FLAG_GLOBAL_HEADER ? 2 : 0;

			if ((err = aacEncoder_SetParam(aac->handle, AACENC_SIGNALING_MODE, aac->signaling)) != AACENC_OK)
			{
				CROSS_TRACE("Unable to set signaling mode %d: %s\n", aac->signaling, aac_get_error(err));
				break;
			}

			if ((err = aacEncoder_SetParam(aac->handle, AACENC_AFTERBURNER, aac->afterburner)) != AACENC_OK)
			{
				CROSS_TRACE("Unable to set afterburner to %d: %s\n", aac->afterburner, aac_get_error(err));
				break;
			}

			if (avctx->cutoff > 0)
			{
				if (avctx->cutoff < (avctx->sample_rate + 255) >> 8 || avctx->cutoff > 20000)
				{
					CROSS_TRACE("cutoff valid range is %d-20000\n", (avctx->sample_rate + 255) >> 8);
					break;
				}
				if ((err = aacEncoder_SetParam(aac->handle, AACENC_BANDWIDTH, avctx->cutoff)) != AACENC_OK)
				{
					CROSS_TRACE("Unable to set the encoder bandwidth to %d: %s\n", avctx->cutoff, aac_get_error(err));
					break;
				}
			}

			if ((err = aacEncEncode(aac->handle, NULL, NULL, NULL, NULL)) != AACENC_OK)
			{
				CROSS_TRACE("Unable to initialize the encoder: %s\n", aac_get_error(err));
				break;
			}

			if ((err = aacEncInfo(aac->handle, &info)) != AACENC_OK)
			{
				CROSS_TRACE("Unable to get encoder info: %s\n", aac_get_error(err));
				break;
			}

			avctx->frame_size = info.frameLength;
			avctx->delay = info.encoderDelay;
#if 0//add yyj
			ff_af_queue_init(avctx, &aac->afq);
#endif
			if (avctx->flags & CODEC_FLAG_GLOBAL_HEADER)
			{
				avctx->extradata_size = info.confSize;
				//avctx->extradata      = av_mallocz(avctx->extradata_size + FF_INPUT_BUFFER_PADDING_SIZE);
				avctx->extradata = (uint8_t *)malloc(avctx->extradata_size + FF_INPUT_BUFFER_PADDING_SIZE);

				if (!avctx->extradata)
				{
					ret = -1;
					break;
				}
				memset(avctx->extradata, 0, avctx->extradata_size + FF_INPUT_BUFFER_PADDING_SIZE);
				memcpy(avctx->extradata, info.confBuf, info.confSize);
			}
			CROSS_TRACE("CFdkaac::InitVoice  okokokokokokokok");
			ret = 0;
	} while (0);

	if (0 != ret)
	{
		DeinitVoice();
	}

	return 0;
};
	AACEncoder_t m_AACEncoder;


#if 0
	avctx->sample_rate = 4800;
	avctx->channels = 1;

	avctx->cutoff = 8000;
	avctx->flags = 0;
	avctx->profile = FF_PROFILE_AAC_MAIN;
	avctx->bit_rate = 8000;

	//avctx->frame_size;
	//avctx->delay;
	//avctx->global_quality;
	//avctx->sample_fmt;
	//avctx->channel_layout;

	AACContext *aac = (AACContext *)avctx->priv_data;

	//aac->afterburner = 0;
	//aac->eld_sbr = 0;
	//aac->signaling = 0;
	//aac->latm = 0;
	//aac->header_period = 0;
	aac->vbr = 1;
#endif
private:
	int __aac_decode_init()
	{
		AAC_DECODER_ERROR err;
		int ret = -1;

		do
		{

			//-------------------------------------------------------
			avctx = (AVCodecContext *)malloc(sizeof(AVCodecContext));
			if (!avctx)
			{
				break;
			}
			memset(avctx, 0, sizeof(AVCodecContext));
			//-------------------------------------------------------
			avctx->priv_data = malloc(sizeof(FDKAACDecContext));
			if (!avctx->priv_data)
			{
				break;
			}
			memset(avctx->priv_data, 0, sizeof(FDKAACDecContext));
			//v
			//
			FDKAACDecContext *aac = (FDKAACDecContext *)avctx->priv_data;
			aac->conceal_method = m_AACDecoder.conceal_method;
			//aac->handle = aacDecoder_Open(avctx->extradata_size ? TT_MP4_RAW : TT_MP4_ADTS, 1);
			aac->handle = aacDecoder_Open(TT_MP4_ADTS, 1);
			if (!aac->handle) {
				CROSS_TRACE("Error opening decoder\n");
				break;
			}

			//
			m_AACDecoder.conceal_method = CONCEAL_METHOD_NOISE_SUBSTITUTION;

			if (aac->conceal_method != CONCEAL_METHOD_DEFAULT)
			{
				if ((err = aacDecoder_SetParam(aac->handle, AAC_CONCEAL_METHOD, aac->conceal_method)) != AAC_DEC_OK)
				{
					CROSS_TRACE("Unable to set error concealment method\n");
					break;
				}
			}

			CStreamInfo *aac_stream_info = NULL;
			aac_stream_info = aacDecoder_GetStreamInfo(aac->handle);
			// 		if (aac_stream_info != NULL) {
			// 			CROSS_TRACE("> stream info: channel = %d\tsample_rate = %d\tframe_size = %d\taot = %d\tbitrate = %d\n",
			// 				aac_stream_info->channelConfig, aac_stream_info->aacSampleRate,
			// 				aac_stream_info->aacSamplesPerFrame, aac_stream_info->aot, aac_stream_info->bitRate);
			// 		}

			avctx->sample_fmt = AV_SAMPLE_FMT_S16;


			ret = 0;
		} while (0);

		return 0;
	};
	int __aac_decode_get_stream_info()
	{
		FDKAACDecContext *s = (FDKAACDecContext *)avctx->priv_data;
		CStreamInfo *info = aacDecoder_GetStreamInfo(s->handle);
		int channel_counts[9] = { 0 };
		int i, ch_error = 0;
		uint64_t ch_layout = 0;

		if (!info) {
			CROSS_TRACE("Unable to get stream info\n");
			return -1;
		}

		if (info->sampleRate <= 0) {
			CROSS_TRACE("Stream info not initialized\n");
			return -1;
		}
		avctx->sample_rate = info->sampleRate;
		avctx->frame_size = info->frameSize;

		for (i = 0; i < info->numChannels; i++) {
			AUDIO_CHANNEL_TYPE ctype = info->pChannelType[i];
			if (ctype <= ACT_NONE || ctype > ACT_TOP) {
				CROSS_TRACE("unknown channel type\n");
				break;
			}
			channel_counts[ctype]++;
		}
#if 0
		DBG("%d channels - front:%d side:%d back:%d lfe:%d top:%d\n",
			info->numChannels,
			channel_counts[ACT_FRONT], channel_counts[ACT_SIDE],
			channel_counts[ACT_BACK], channel_counts[ACT_LFE],
			channel_counts[ACT_FRONT_TOP] + channel_counts[ACT_SIDE_TOP] +
			channel_counts[ACT_BACK_TOP] + channel_counts[ACT_TOP]);
#endif
		switch (channel_counts[ACT_FRONT]) {
		case 4:
			ch_layout |= AV_CH_LAYOUT_STEREO | AV_CH_FRONT_LEFT_OF_CENTER |
				AV_CH_FRONT_RIGHT_OF_CENTER;
			break;
		case 3:
			ch_layout |= AV_CH_LAYOUT_STEREO | AV_CH_FRONT_CENTER;
			break;
		case 2:
			ch_layout |= AV_CH_LAYOUT_STEREO;
			break;
		case 1:
			ch_layout |= AV_CH_FRONT_CENTER;
			break;
		default:
			CROSS_TRACE("unsupported number of front channels: %d\n",
				channel_counts[ACT_FRONT]);
			ch_error = 1;
			break;
		}
		if (channel_counts[ACT_SIDE] > 0) {
			if (channel_counts[ACT_SIDE] == 2) {
				ch_layout |= AV_CH_SIDE_LEFT | AV_CH_SIDE_RIGHT;
			}
			else {
				CROSS_TRACE("unsupported number of side channels: %d\n",
					channel_counts[ACT_SIDE]);
				ch_error = 1;
			}
		}
		if (channel_counts[ACT_BACK] > 0) {
			switch (channel_counts[ACT_BACK]) {
			case 3:
				ch_layout |= AV_CH_BACK_LEFT | AV_CH_BACK_RIGHT | AV_CH_BACK_CENTER;
				break;
			case 2:
				ch_layout |= AV_CH_BACK_LEFT | AV_CH_BACK_RIGHT;
				break;
			case 1:
				ch_layout |= AV_CH_BACK_CENTER;
				break;
			default:
				CROSS_TRACE("unsupported number of back channels: %d\n", channel_counts[ACT_BACK]);
				ch_error = 1;
				break;
			}
		}
		if (channel_counts[ACT_LFE] > 0) {
			if (channel_counts[ACT_LFE] == 1) {
				ch_layout |= AV_CH_LOW_FREQUENCY;
			}
			else {
				CROSS_TRACE("unsupported number of LFE channels: %d\n",
					channel_counts[ACT_LFE]);
				ch_error = 1;
			}
		}
		if (!ch_error && av_get_channel_layout_nb_channels(ch_layout) != info->numChannels)
		{
			CROSS_TRACE("unsupported channel configuration\n");
			ch_error = 1;
		}
		if (ch_error)
			avctx->channel_layout = 0;
		else
			avctx->channel_layout = ch_layout;

		avctx->channels = info->numChannels;


		CStreamInfo *aac_stream_info = NULL;
		aac_stream_info = aacDecoder_GetStreamInfo(s->handle);
		if (aac_stream_info != NULL) {
			CROSS_TRACE("> stream info: channel = %d\tsample_rate = %d\tframe_size = %d\taot = %d\tbitrate = %d\n",
				aac_stream_info->channelConfig, aac_stream_info->aacSampleRate,
				aac_stream_info->aacSamplesPerFrame, aac_stream_info->aot, aac_stream_info->bitRate);
		}




		return 0;
	};//
	AACDecoder_t m_AACDecoder;
};



#endif