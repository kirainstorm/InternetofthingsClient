#include "stdafx.h"
#include "librtmpapp.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#ifdef WIN32 
#include "rtmp.h"   
#include "rtmp_sys.h"   
#include "amf.h"  
#else
#include "librtmp/rtmp.h"   
#include "librtmp/rtmp_sys.h"   
#include "librtmp/amf.h" 	
#endif 



// #define WRITE_FILE_TEST 1
// #if WRITE_FILE_TEST
// FILE * file = NULL;
// #endif



/**********************************************/
// #define AUDIO_TYPE_G711A	7
// #define AUDIO_TYPE_G711U	8
// #define AUDIO_TYPE_AAC		10
// 
// 
// #define AUDIO_SAMPLERATE_5_5K		0
// #define AUDIO_SAMPLERATE_11K		1
// #define AUDIO_SAMPLERATE_22K		2
// #define AUDIO_SAMPLERATE_44K		44
// 
// #define AUDIO_SAMPLE_8BIT		0
// #define AUDIO_SAMPLE_16BIT		1
// 
// #define AUDIO_SAMPLE_MONO		0
// #define AUDIO_SAMPLE_STEREO		1
// 
// #define AUDIO_AAC_SEQUENCE_HEADER	0
// #define AUDIO_AAC_RAW				1




/**********************************************/
//定义包头长度，RTMP_MAX_HEADER_SIZE=18
#define RTMP_HEAD_SIZE   (sizeof(RTMPPacket)+RTMP_MAX_HEADER_SIZE)

//H264
#define NALU_TYPE_SLICE 1
#define NALU_TYPE_DPA 2
#define NALU_TYPE_DPB 3
#define NALU_TYPE_DPC 4
#define NALU_TYPE_IDR 5
#define NALU_TYPE_SEI 6
#define NALU_TYPE_SPS 7
#define NALU_TYPE_PPS 8
#define NALU_TYPE_AUD 9	//访问分隔符
#define NALU_TYPE_EOSEQ 10
#define NALU_TYPE_EOSTREAM 11
#define NALU_TYPE_FILL 12







CLibRTMPApp::CLibRTMPApp()
{
#ifdef WIN32
	WORD version;
	WSADATA wsaData;
	version = MAKEWORD(2, 1);
	WSAStartup(version, &wsaData);
#endif

	rtmpapp = NULL;

// #if WRITE_FILE_TEST
// 	fopen_s(&file, "d:\\test.rtmp", "wb");
// #endif


}

CLibRTMPApp::~CLibRTMPApp()
{
#ifdef WIN32
	WSACleanup();
#endif
// #if WRITE_FILE_TEST
// 	fclose(file);
// #endif
}


int CLibRTMPApp::RTMP264_Connect(const char* url)
{

	int ret = -1;
	do 
	{
		//
		if (NULL != rtmpapp)
		{
			ret = 0;
			break;
		}
		//
		rtmpapp = new RtmpApp_t();
		if (NULL == rtmpapp)
		{
			break;
		}
		//
		memset(rtmpapp, 0, sizeof(RtmpApp_t));
		//
		rtmpapp->Rtmp = RTMP_Alloc();
		if (NULL == rtmpapp->Rtmp)
		{
			break;
		}
		//
		RTMP_Init(rtmpapp->Rtmp);

		//rtmpapp->Rtmp->Link.lFlags |= RTMP_LF_LIVE;//=======@@@@
		//
		if (RTMP_SetupURL(rtmpapp->Rtmp, (char*)url) == FALSE)/*设置URL*/
		{
			break;
		}
		//
		RTMP_EnableWrite(rtmpapp->Rtmp);/*设置可写,即发布流,这个函数必须在连接前使用,否则无效*/
		//
		if (RTMP_Connect(rtmpapp->Rtmp, NULL) == FALSE)/*连接服务器*/
		{
			break;
		}

// 		if (SetChunkSize(rtmpapp->Rtmp, 1440) == FALSE)//修改数据包大小的接口 
// 		{
// 			break;
// 		}

		//
		if (RTMP_ConnectStream(rtmpapp->Rtmp, 0) == FALSE)/*连接流*/
		{
			break;
		}

		//

		//
		ret = 0;

	} while (0);

	//
	if (0 != ret)
	{
		RTMP264_RTMP264_Close();
	}

	return ret;
}
int CLibRTMPApp::RTMP264_RTMP264_Close()
{
	if (rtmpapp)
	{
		//
		if (rtmpapp->Rtmp)
		{
			RTMP_Close(rtmpapp->Rtmp);
			RTMP_Free(rtmpapp->Rtmp);
			rtmpapp->Rtmp = NULL;
		}

		//
		if (rtmpapp->MetaTags.Pps)
		{
			free(rtmpapp->MetaTags.Pps);
			rtmpapp->MetaTags.Pps = NULL;
		}
		//
		if (rtmpapp->MetaTags.Sps)
		{
			free(rtmpapp->MetaTags.Sps);
			rtmpapp->MetaTags.Sps = NULL;
		}

		//

		//
		delete rtmpapp;
		rtmpapp = NULL;
	}

	return 0;
}
int CLibRTMPApp::RTMP264_SendH264Packet(unsigned char *data111, unsigned int length, int bIsKeyFrame, unsigned int nTimeStamp)
{

	if ((NULL == rtmpapp) || (NULL == rtmpapp->Rtmp))
	{
		return -1;
	}

	if (data111 == NULL && length < 11){
		return -2;
	}

	//if (rtmpapp->MetaTags.nSpsLen == 0)
	NaluUnit_t nalu;
	memset(&nalu, 0, sizeof(NaluUnit_t));
	nalu.data = (unsigned char*)data111;
	nalu.size = length;
	ParseH264NaluData(&nalu);
	//TRACE("--------------- %d -- %d\n", length, nalu.targetsize);

	unsigned char * pdata = nalu.targetdata;
	int size = nalu.targetsize;


	if ((bIsKeyFrame) && rtmpapp->MetaTags.Sps)// rtmpapp->MetaTags.Pps&&rtmpapp->MetaTags.Sps)
	{
		//printf("Send Video Sps Pps.\n");
		//SendVideoSpsPps(rtmpapp->MetaTags.Pps, rtmpapp->MetaTags.nPpsLen, rtmpapp->MetaTags.Sps, rtmpapp->MetaTags.nSpsLen);

		SendVideoSpsPps(rtmpapp->MetaTags.Sps, rtmpapp->MetaTags.nSpsLen, rtmpapp->MetaTags.Pps, rtmpapp->MetaTags.nPpsLen);




// 		for (int i = 0; i < rtmpapp->MetaTags.nSpsLen; i++)
// 		{
// 			char s[16] = { 0 };
// 			sprintf_s(s, "%02x ", rtmpapp->MetaTags.Sps[i]);
// 			OutputDebugStringA(s);
// 		}
// 		OutputDebugStringA("\n");


// 		for (int i = 0; i < rtmpapp->MetaTags.nPpsLen; i++)
// 		{
// 			char s[16] = { 0 };
// 			sprintf_s(s, "%02x ", rtmpapp->MetaTags.Pps[i]);
// 			OutputDebugStringA(s);
// 		}
// 		OutputDebugStringA("\n");
	}

	//RTMP* m_pRtmp = (RTMP*)rtmpapp->Rtmp;


	RTMPPacket* packet = (RTMPPacket *)malloc(RTMP_HEAD_SIZE + size + 9);/*分配包内存和初始化,len为包体长度*/
	memset(packet, 0, RTMP_HEAD_SIZE);

//	if (!packet)
// 	{
// 		return -3;
// 
// 	}


	packet->m_body = (char *)packet + RTMP_HEAD_SIZE;



	unsigned char *body = (unsigned char *)packet->m_body;//(unsigned char*)malloc(size+9);


	//memset(body,0,size+9);
	memset(body, 0, 9);

	//int i = 0;
	if (bIsKeyFrame)
	{
		body[0] = 0x17;// 1:Iframe  7:AVC   
	}
	else
	{
		body[0] = 0x27;// 2:Pframe  7:AVC   
	}

// 	body[i++] = 0x01;// AVC NALU   
// 	body[i++] = 0x00;
// 	body[i++] = 0x00;
// 	body[i++] = 0x00;
// 
// 
// 	// NALU size   
// 	body[i++] = size >> 24 & 0xff;
// 	body[i++] = size >> 16 & 0xff;
// 	body[i++] = size >> 8 & 0xff;
// 	body[i++] = size & 0xff;


	body[1] = 0x01;   /*nal unit*/
	body[2] = 0x00;
	body[3] = 0x00;
	body[4] = 0x00;

	body[5] = (size >> 24) & 0xff;
	body[6] = (size >> 16) & 0xff;
	body[7] = (size >> 8) & 0xff;
	body[8] = (size)& 0xff;


	// NALU data   
	memcpy(&body[9], pdata, size);



	packet->m_nBodySize = size + 9;
	packet->m_hasAbsTimestamp = 0;
	packet->m_packetType = RTMP_PACKET_TYPE_VIDEO; /*此处为类型有两种一种是音频,一种是视频*/
	packet->m_nInfoField2 = rtmpapp->Rtmp->m_stream_id;
	packet->m_nChannel = 0x04;

	packet->m_headerType = RTMP_PACKET_SIZE_LARGE;
	packet->m_nTimeStamp = nTimeStamp;

	int nRet = 0;
	if (RTMP_IsConnected(rtmpapp->Rtmp))
	{
		//nRet = RTMP_SendPacket(rtmpapp->Rtmp, packet, TRUE); /*TRUE为放进发送队列,FALSE是不放进发送队列,直接发送*/
		nRet = RTMP_SendPacket(rtmpapp->Rtmp, packet, FALSE); /*TRUE为放进发送队列,FALSE是不放进发送队列,直接发送*/
	}
	free(packet);
	return (nRet>0)?0:-1;


	//return TRUE;
}

int CLibRTMPApp::RTMP264_SendAACPacket(unsigned char *data, unsigned int size, unsigned int nSampleRate, unsigned int nTimeStamp)
{
	int bRet = -1;
	static int q = 0;

	if ((!rtmpapp) || (!rtmpapp->Rtmp))
	{
		return bRet;
	}

	if (data == NULL && size < 11)
	{
		return bRet;
	}
#if 1
	//if(nTimeStamp == 0)
	if (q == 0)
	{
		nTimeStamp = 0;
		q = 1;
		bRet = SendAACASCPacket(data, 0, nSampleRate, nTimeStamp);
		if (bRet == false)
			return bRet;
	}

	char *aacraw = (char *)&data[7];
	unsigned int aacrawsize = size - 7;
#else
	char *aacraw = data;
	unsigned int aacrawsize = size;
#endif
	RTMP* m_pRtmp = (RTMP*)rtmpapp->Rtmp;
	RTMPPacket* packet;
	/*分配包内存和初始化,len为包体长度*/
	packet = (RTMPPacket *)malloc(RTMP_HEAD_SIZE + aacrawsize + 2);
	if (!packet)
	{
		return false;
	}
	packet->m_body = (char *)packet + RTMP_HEAD_SIZE;
	unsigned char *body = (unsigned char*)packet->m_body;//malloc(aacrawsize+2);  
	//memset(body,0,aacrawsize+2);
	memset(body, 0, 2);

	int i = 0;

	char tmp = (char)0xA3;
	if (nSampleRate == 5512)
	{
		tmp |= 0x00;
	}
	else if (nSampleRate == 11025)
	{
		tmp |= 0x04;
	}
	else if (nSampleRate == 22050)
	{
		tmp |= 0x08;
	}
	else
	{
		tmp |= 0x0C;
	}

	body[i++] = tmp; //0xAF;
	body[i++] = 0x01;

	memcpy(&body[i], aacraw, aacrawsize);



	packet->m_nBodySize = aacrawsize + i;
	packet->m_hasAbsTimestamp = 0;
	packet->m_packetType = RTMP_PACKET_TYPE_AUDIO; /*此处为类型有两种一种是音频,一种是视频*/
	packet->m_nInfoField2 = m_pRtmp->m_stream_id;
	packet->m_nChannel = 0x04;

	packet->m_headerType = RTMP_PACKET_SIZE_MEDIUM;
	packet->m_nTimeStamp = nTimeStamp;

	int nRet = 0;
	if (RTMP_IsConnected(m_pRtmp))
	{
		//nRet = RTMP_SendPacket(m_pRtmp,packet,TRUE); /*TRUE为放进发送队列,FALSE是不放进发送队列,直接发送*/
		nRet = RTMP_SendPacket(m_pRtmp, packet, FALSE); /*TRUE为放进发送队列,FALSE是不放进发送队列,直接发送*/

		//TRACE("send aac ---- ret =%d\n", nRet);
	}
	free(packet);
	return (nRet>0) ? 0 : -1;
	return TRUE;
}




//int SendPacket(RtmpApp_t *rtmpapp,unsigned int nPacketType,unsigned char *data,unsigned int size,unsigned int nTimestamp)  
// int CLibRTMPApp::SendPacket(unsigned int nPacketType, RTMPPacket *packet, unsigned int size, unsigned int nTimestamp)
// {  
// 	if((!rtmpapp)||(!rtmpapp->Rtmp)||(!packet))
// 	{
// 		return false;
// 	}
// 
// 	RTMP* m_pRtmp = (RTMP*)rtmpapp->Rtmp;
// 	//RTMPPacket* packet;
// 	/*分配包内存和初始化,len为包体长度*/
// 	//packet = (RTMPPacket *)malloc(RTMP_HEAD_SIZE+size);
// 	memset(packet,0,RTMP_HEAD_SIZE);
// 	/*包体内存*/
// 	//packet->m_body = (char *)packet + RTMP_HEAD_SIZE;
// 	packet->m_nBodySize = size;
// 	//memcpy(packet->m_body,data,size);
// 	packet->m_hasAbsTimestamp = 0;
// 	packet->m_packetType = nPacketType; /*此处为类型有两种一种是音频,一种是视频*/
// 	packet->m_nInfoField2 = m_pRtmp->m_stream_id;
// 	packet->m_nChannel = 0x04;
// 
// 	packet->m_headerType = RTMP_PACKET_SIZE_LARGE;
// 	if (RTMP_PACKET_TYPE_AUDIO ==nPacketType && size !=4)
// 	{
// 		packet->m_headerType = RTMP_PACKET_SIZE_MEDIUM;
// 	}
// 	packet->m_nTimeStamp = nTimestamp;
// 	printf("send data: %p.\n", packet);
// 	/*发送*/
// 	int nRet =0;
// 	if (RTMP_IsConnected(m_pRtmp))
// 	{
// 		//nRet = RTMP_SendPacket(m_pRtmp,packet,TRUE); /*TRUE为放进发送队列,FALSE是不放进发送队列,直接发送*/
// 		nRet = RTMP_SendPacket(m_pRtmp,packet,FALSE); /*TRUE为放进发送队列,FALSE是不放进发送队列,直接发送*/
// 	}
// 	printf("send done.\n");
// 	/*释放内存*/
// 	//free(packet);
// 	return nRet;  
// }  

int CLibRTMPApp::SendVideoSpsPps(unsigned char * sps, int sps_len, unsigned char *pps, int pps_len)
{
	if((!rtmpapp)||(!rtmpapp->Rtmp))
	{
		return false;
	}

	
	//RTMP* m_pRtmp = (RTMP*)rtmpapp->Rtmp;
	RTMPPacket * packet=NULL;//rtmp包结构
	unsigned char * body=NULL;
	int i;


	packet = (RTMPPacket *)malloc(RTMP_HEAD_SIZE+1024);
	//RTMPPacket_Reset(packet);//重置packet状态
	memset(packet,0,RTMP_HEAD_SIZE+1024);
	packet->m_body = (char *)packet + RTMP_HEAD_SIZE;
	body = (unsigned char *)packet->m_body;


	i = 0;
	body[i++] = 0x17;
	body[i++] = 0x00;

	body[i++] = 0x00;
	body[i++] = 0x00;
	body[i++] = 0x00;

	/*AVCDecoderConfigurationRecord*/
	body[i++] = 0x01;
	body[i++] = sps[1];
	body[i++] = sps[2];
	body[i++] = sps[3];
	body[i++] = 0xff;

	/*sps*/
	body[i++]   = 0xe1;
	body[i++] = (sps_len >> 8) & 0xff;
	body[i++] = sps_len & 0xff;
	memcpy(&body[i],sps,sps_len);
	i +=  sps_len;

	/*pps*/
	body[i++]   = 0x01;
	body[i++] = (pps_len >> 8) & 0xff;
	body[i++] = (pps_len) & 0xff;
	memcpy(&body[i],pps,pps_len);
	i +=  pps_len;

	packet->m_packetType = RTMP_PACKET_TYPE_VIDEO;
	packet->m_nBodySize = i;
	packet->m_nChannel = 0x04;
	packet->m_nTimeStamp = 0;
	packet->m_hasAbsTimestamp = 0;
	packet->m_headerType = RTMP_PACKET_SIZE_MEDIUM;
	packet->m_nInfoField2 = rtmpapp->Rtmp->m_stream_id;

	/*调用发送接口*/
	//int nRet = RTMP_SendPacket(rtmpapp->Rtmp, packet, TRUE);
	int nRet = RTMP_SendPacket(rtmpapp->Rtmp, packet, FALSE);
	free(packet);    //释放内存
	return nRet;
}

// int SendH264Packet(RtmpApp_t *rtmpapp,unsigned char *data,unsigned int size,int bIsKeyFrame,unsigned int nTimeStamp)  
// {  
// 
// } 

int CLibRTMPApp::ParseH264NaluData(NaluUnit_t *nalu)
{
	int ret = false;
	
	if((!nalu)||(!(nalu->data)))
	{
		printf("if((!nalu)||(!(nalu->data)))\n");
		return false;
	}

	
	if((!rtmpapp)||(!(rtmpapp->Rtmp)))
	{
		printf("if((!rtmpapp)||(!(rtmpapp->Rtmp)))\n");
		return false;
	}

	do{
		
		if((nalu->data[nalu->parsesize] == 0)&&
			(nalu->data[nalu->parsesize + 1] == 0)&&
			(nalu->data[nalu->parsesize + 2] == 0)&&
			(nalu->data[nalu->parsesize + 3] == 1))
		{
			nalu->parsesize += 4;
			nalu->type = nalu->data[nalu->parsesize] &0x1f;
			//printf("nalu type: %02x.\n", nalu->type);
			if(nalu->type == NALU_TYPE_SPS)
			{
				int parsesize = nalu->parsesize;
				do{
					nalu->parsesize ++;
				}while(!((nalu->data[nalu->parsesize] == 0)&&
						(nalu->data[nalu->parsesize + 1] == 0)&&
						(nalu->data[nalu->parsesize + 2] == 0)&&
						(nalu->data[nalu->parsesize + 3] == 1)));
				
				rtmpapp->MetaTags.nSpsLen = nalu->parsesize - parsesize;
				if (rtmpapp->MetaTags.Sps)
				{
					free(rtmpapp->MetaTags.Sps);
					rtmpapp->MetaTags.Sps = NULL;
				}
				//Free(rtmpapp->MetaTags.Sps);
				rtmpapp->MetaTags.Sps = (unsigned char *)malloc(rtmpapp->MetaTags.nSpsLen);
				memcpy(rtmpapp->MetaTags.Sps, &nalu->data[parsesize], rtmpapp->MetaTags.nSpsLen);
			}
			else if(nalu->type == NALU_TYPE_PPS)
			{
				int parsesize = nalu->parsesize;
				do{
					nalu->parsesize ++;
				}while(!((nalu->data[nalu->parsesize] == 0)&&
						(nalu->data[nalu->parsesize + 1] == 0)&&
						(nalu->data[nalu->parsesize + 2] == 0)&&
						(nalu->data[nalu->parsesize + 3] == 1)));
				
				rtmpapp->MetaTags.nPpsLen = nalu->parsesize - parsesize;
				//Free(rtmpapp->MetaTags.Pps);

				if (rtmpapp->MetaTags.Pps)
				{
					free(rtmpapp->MetaTags.Pps);
					rtmpapp->MetaTags.Pps = NULL;
				}

				rtmpapp->MetaTags.Pps = (unsigned char *)malloc(rtmpapp->MetaTags.nPpsLen);
				memcpy(rtmpapp->MetaTags.Pps, &nalu->data[parsesize], rtmpapp->MetaTags.nPpsLen);
			}
			else if((nalu->type == NALU_TYPE_IDR)||(nalu->type == NALU_TYPE_SLICE))
			{
				nalu->targetdata = &nalu->data[nalu->parsesize];
				nalu->targetsize = nalu->size - nalu->parsesize;
				nalu->parsesize = nalu->size;
				ret = true;
			}	
		}
		else
		{
			nalu->parsesize++;		
		}
		
	}while(nalu->parsesize < nalu->size);
	
	return ret; 
}

/************************************************************************************************/
enum AudioObjectType {
    AOT_NULL,
                               // Support?                Name
    AOT_AAC_MAIN,              ///< Y                       Main
    AOT_AAC_LC,                ///< Y                       Low Complexity
    AOT_AAC_SSR,               ///< N (code in SoC repo)    Scalable Sample Rate
    AOT_AAC_LTP,               ///< Y                       Long Term Prediction
    AOT_SBR,                   ///< Y                       Spectral Band Replication
    AOT_AAC_SCALABLE,          ///< N                       Scalable
    AOT_TWINVQ,                ///< N                       Twin Vector Quantizer
    AOT_CELP,                  ///< N                       Code Excited Linear Prediction
    AOT_HVXC,                  ///< N                       Harmonic Vector eXcitation Coding
    AOT_TTSI             = 12, ///< N                       Text-To-Speech Interface
    AOT_MAINSYNTH,             ///< N                       Main Synthesis
    AOT_WAVESYNTH,             ///< N                       Wavetable Synthesis
    AOT_MIDI,                  ///< N                       General MIDI
    AOT_SAFX,                  ///< N                       Algorithmic Synthesis and Audio Effects
    AOT_ER_AAC_LC,             ///< N                       Error Resilient Low Complexity
    AOT_ER_AAC_LTP       = 19, ///< N                       Error Resilient Long Term Prediction
    AOT_ER_AAC_SCALABLE,       ///< N                       Error Resilient Scalable
    AOT_ER_TWINVQ,             ///< N                       Error Resilient Twin Vector Quantizer
    AOT_ER_BSAC,               ///< N                       Error Resilient Bit-Sliced Arithmetic Coding
    AOT_ER_AAC_LD,             ///< N                       Error Resilient Low Delay
    AOT_ER_CELP,               ///< N                       Error Resilient Code Excited Linear Prediction
    AOT_ER_HVXC,               ///< N                       Error Resilient Harmonic Vector eXcitation Coding
    AOT_ER_HILN,               ///< N                       Error Resilient Harmonic and Individual Lines plus Noise
    AOT_ER_PARAM,              ///< N                       Error Resilient Parametric
    AOT_SSC,                   ///< N                       SinuSoidal Coding
    AOT_PS,                    ///< N                       Parametric Stereo
    AOT_SURROUND,              ///< N                       MPEG Surround
    AOT_ESCAPE,                ///< Y                       Escape Value
    AOT_L1,                    ///< Y                       Layer 1
    AOT_L2,                    ///< Y                       Layer 2
    AOT_L3,                    ///< Y                       Layer 3
    AOT_DST,                   ///< N                       Direct Stream Transfer
    AOT_ALS,                   ///< Y                       Audio LosslesS
    AOT_SLS,                   ///< N                       Scalable LosslesS
    AOT_SLS_NON_CORE,          ///< N                       Scalable LosslesS (non core)
    AOT_ER_AAC_ELD,            ///< N                       Error Resilient Enhanced Low Delay
    AOT_SMR_SIMPLE,            ///< N                       Symbolic Music Representation Simple
    AOT_SMR_MAIN,              ///< N                       Symbolic Music Representation Main
    AOT_USAC_NOSBR,            ///< N                       Unified Speech and Audio Coding (no SBR)
    AOT_SAOC,                  ///< N                       Spatial Audio Object Coding
    AOT_LD_SURROUND,           ///< N                       Low Delay MPEG Surround
    AOT_USAC,                  ///< N                       Unified Speech and Audio Coding
};

// const int avpriv_mpeg4audio_sample_rates[16] = {
//     96000, 88200, 64000, 48000, 44100, 32000,
//     24000, 22050, 16000, 12000, 11025, 8000, 7350
// };

/*
0: Defined in AOT Specifc Config 
1: 1 channel: front-center 
2: 2 channels: front-left, front-right 
3: 3 channels: front-center, front-left, front-right 
4: 4 channels: front-center, front-left, front-right, back-center 
5: 5 channels: front-center, front-left, front-right, back-left, back-right 
6: 6 channels: front-center, front-left, front-right, back-left, back-right, LFE-channel 
7: 8 channels: front-center, front-left, front-right, side-left, side-right, back-left, back-right, LFE-channel 
8-15: Reserved

*/
const uint8_t ff_mpeg4audio_channels[8] = {
    0, 1, 2, 3, 4, 5, 6, 8
};



int CLibRTMPApp::SendAACASCPacket(unsigned char *data,unsigned int size,unsigned int nSampleRate,unsigned int nTimeStamp)
{
	if((!rtmpapp)||(!rtmpapp->Rtmp))
	{
		return false;
	}
	
	if(data == NULL)
	{  
		return false;  
	}  
	RTMP* m_pRtmp = (RTMP*)rtmpapp->Rtmp;
	RTMPPacket* packet;
	/*分配包内存和初始化,len为包体长度*/
	packet = (RTMPPacket *)malloc(RTMP_HEAD_SIZE+size+4);
	if(!packet)
	{  
		return false;  
	}  
	packet->m_body = (char *)packet + RTMP_HEAD_SIZE;
	unsigned char *body = (unsigned char*)packet->m_body;//malloc(size+4);  
	memset(body,0,size+4);

	char object_type = AOT_AAC_LC;//5bit
	char sample_rate = 11;//4bit 48000 //11 
	//char sample_rate = 4;//4bit 44100
	char channels = 1;//4bit
	
	int i = 0; 
	
	char tmp = (char)0xA3;
	if(nSampleRate == 5512)
	{
		tmp |= 0x00;
	}
	else if(nSampleRate == 11025)
	{
		tmp |= 0x04;
	}
	else if(nSampleRate == 22050)
	{
		tmp |= 0x08;
	}
	else
	{
		tmp |= 0x0C;
	}

	body[i++] = tmp; 
	body[i++] = 0x00;  
	//编码机构:5bit; 采样率:4bit; 输出通道:4bit
 	body[i++] = (object_type<<3)|(sample_rate>>1); 
	body[i++] = (sample_rate<<7)|(channels<<3); //低三位全为0

	printf("%02x %02x %02x %02x\n", body[0], body[1], body[2], body[3]);
	
	//af 00 15 88 //20170524 --- 1ch 8000hz 16bit



	//memcpy(&body[i],data,size); 

	packet->m_nBodySize = size+i;
	packet->m_hasAbsTimestamp = 0;
	packet->m_packetType = RTMP_PACKET_TYPE_AUDIO; /*此处为类型有两种一种是音频,一种是视频*/
	packet->m_nInfoField2 = m_pRtmp->m_stream_id;
	packet->m_nChannel = 0x04;

	packet->m_headerType = RTMP_PACKET_SIZE_LARGE;
	packet->m_nTimeStamp = nTimeStamp;
	
	int nRet =0;
	if (RTMP_IsConnected(m_pRtmp))
	{
		//nRet = RTMP_SendPacket(m_pRtmp,packet,TRUE); /*TRUE为放进发送队列,FALSE是不放进发送队列,直接发送*/
		nRet = RTMP_SendPacket(m_pRtmp,packet,FALSE); /*TRUE为放进发送队列,FALSE是不放进发送队列,直接发送*/
	}
	free(packet);  
	return nRet;  
}