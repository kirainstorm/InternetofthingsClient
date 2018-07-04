#include "stdafx.h"
#include "SrajwToMediaInterface.h"
#include "TransmisStreamToPlatform.h"

#if USE_SRA_JIWEI



#include "../../../../Direct-connected/_DevInterface/SRAInterfaceA/oem/Interface/SDK_Interface_AVCtrl.h"
#include "../../../../Direct-connected/_DevInterface/SRAInterfaceA/oem/Interface/EncryptGW.h"
#pragma comment( lib, "P2PCLib.lib" )
#pragma comment( lib, "EncryptGW.lib" )

CSrajwToMediaInterface * g_srajwinterface;
int g_srajwvideoframerate = 25;
int g_srajwvideo_width = 0;
int g_srajwvideo_height = 0;

void  vP2PReceiveAlarmMesg(DWORD dwSourceID, DWORD dwAlarmType, DWORD dwAlarmGroup, DWORD dwAlarmChannel)
{
	//收到报警消息
	char cString[128];
	sprintf_s(cString, "来自%d的告警：类型=%d，防区%d-通道%d\n", dwSourceID, dwAlarmType, dwAlarmGroup, dwAlarmChannel);
	::OutputDebugStringA(cString);
}
void vRecvMessage(DWORD dwSrcID, void *pMesg, DWORD dwMesgSize)
{
	BYTE *pCmd = (BYTE*)pMesg;
	char cString[128];
	sprintf_s(cString, "来自[%d]的消息：类型=%d \n", dwSrcID, pCmd[0]);
	::OutputDebugStringA(cString);
}
void vSendMessageAck(DWORD dwDesID, DWORD dwMesgID, DWORD dwError)
{
	CString text = L"FFF";
	switch (dwError)
	{
	case MESG_ERROR_NONE:
		ATLTRACE(_T(" 3cid = %d --- %s success\n"), dwDesID, text);		//消息传达成功
		break;

	case MESG_ERROR_PASSWORD_FAIL:
		ATLTRACE(_T(" 3cid = %d --- %s failed, password error\n"), dwDesID, text);		//消息传达失败，密码错误
		break;
	case MESG_ERROR_TIMEOUT:
		ATLTRACE(_T(" 3cid = %d --- %s failed, network timeout, msgid=%d\n"), dwDesID, text, dwMesgID);	//消息传达失败，网络超时
		break;

	case MESG_IOS_PUSH_ERROR:
		ATLTRACE(_T(" 3cid = %d --- %s failed,  ios\n"), dwDesID, text);	//消息传达失败，对方是IOS系统，推送也失败了
		break;

	case MESG_ERROR_NO_RIGHT:
		ATLTRACE(_T(" 3cid = %d --- %s failed, right error\n"), dwDesID, text);		//消息传达失败，权限不足
		break;

	default:
		ATLTRACE(_T(" 3cid = %d --- %s failed error = %d \n"), dwDesID, text, dwError);
		break;
	}
}
void vDevicesStatusUpdate(sDevicesStatusType * pDevicesStatus, DWORD dwDevicesCount)
{

}
void vGetFileCmdACK(DWORD dwDesID, char *pFilename, DWORD dwErrorCode)
{

}
void vUpdateMesgFlag(UINT64 u64MesgFlag)
{

}
void LoginInAnotherOne(DWORD LoginStatus)
{

}

void SRA_vP2PReceiveAVGroup(unsigned long  dwP2PChannelID, unsigned char *pGopData, unsigned long  dwSize)
{

	//TRACE("-------SRA_vP2PReceiveAVGroup======\n");



	//前四个字节标识合法性
	if (pGopData[0] != AV_HEADER_FLAG ||
		pGopData[1] != AV_HEADER_FLAG ||
		pGopData[2] != AV_HEADER_FLAG ||
		pGopData[3] != AV_HEADER_FLAG_X)
	{
		return;
	}

	g_srajwinterface->m_dTickHearbeat = CrossGetTickCount64();//<<<=================================


	//第六个字节指明数据类型
	char cType = pGopData[5];
	if (cType == PACKET_TYPE_HEADER_ONLY)
	{
		sAVBlockHeader2Type* pHeader = (sAVBlockHeader2Type*)pGopData;
		g_srajwvideoframerate = pHeader->sAvInfo.videoFrameRate;
		g_srajwvideo_width = pHeader->sAvInfo.VideoWidth;
		g_srajwvideo_height = pHeader->sAvInfo.VideoHeight;

	}
	else if (cType == PACKET_TYPE_AV_DATA)
	{
		sAVBlockHeaderType* pHeader = (sAVBlockHeaderType*)pGopData;
		BYTE* pDataBuffer = pGopData + sizeof(sAVBlockHeaderType);
		unsigned char *pos = pGopData;
		int len = sizeof(sAVBlockHeaderType);
		pos += len;
		short audiolen[MAX_AUDIO_PACK_NS];
		for (int i = 0; i < pHeader->wAudioPackCnt; i++)
		{
			audiolen[i] = *((short*)pos);
			pDataBuffer += sizeof(short);
		}
		for (int i = 0; i < pHeader->wAudioPackCnt; i++)
		{
			BYTE* pAmrBuffer = pDataBuffer;		//pAmrBuffer是32字节的amr音频数据
			//自行处理pAmrBuffer
			//memcpy(pVoice->m_buffer, pAmrBuffer, audiolen[i]);
			pDataBuffer += audiolen[i];
		}


		BYTE* pBufferH264 = pDataBuffer;		//pHeader->dwVideoDataLen个字节的h264数据
		//自行处理pBufferH264
		//FFmpegInputData(p->m_dec, (char *)pBufferH264, pHeader->dwVideoDataLen, 27);

		if (0 != g_srajwvideo_width)
		{
			//TRACE("-------SRA_vP2PReceiveAVGroup %d  \n", pHeader->dwVideoDataLen);



// 			for (int i = 0; i < 30; i++)
// 			{
// 				char s[16] = { 0 };
// 				sprintf_s(s, "%02x ", pBufferH264[i]);
// 				OutputDebugStringA(s);
// 			}
// 			OutputDebugStringA("\n");


			if ((0x00 == pBufferH264[21]) && (0x00 == pBufferH264[22]) && (0x00 == pBufferH264[23]) && (0x01 == pBufferH264[24]) && (0x65 == pBufferH264[25]))
			{
				CTransmisStreamToPlatform::Instance().SendMessgeToPlatform(FREAM_TYPE_H264_IFRAME, (char *)pBufferH264, pHeader->dwVideoDataLen,
					g_srajwvideoframerate, g_srajwvideo_width, g_srajwvideo_height, pHeader->u64VPTS);
			}
			else
			{
				CTransmisStreamToPlatform::Instance().SendMessgeToPlatform(FREAM_TYPE_H264_PFRAME, (char *)pBufferH264, pHeader->dwVideoDataLen,
					g_srajwvideoframerate, g_srajwvideo_width, g_srajwvideo_height, pHeader->u64VPTS);
			}



// 			if ( 0x65 == pBufferH264[6])
// 			{
// 				CTransmisStreamToPlatform::Instance().SendMessgeToPlatform(FREAM_TYPE_H264_IFRAME, (char *)pBufferH264, pHeader->dwVideoDataLen,
// 					g_srajwvideoframerate, g_srajwvideo_width, g_srajwvideo_height, pHeader->u64VPTS);
// 			}
// 
// 			if (0x41 == pBufferH264[6])
// 			{
// 				CTransmisStreamToPlatform::Instance().SendMessgeToPlatform(FREAM_TYPE_H264_PFRAME, (char *)pBufferH264, pHeader->dwVideoDataLen,
// 					g_srajwvideoframerate, g_srajwvideo_width, g_srajwvideo_height, pHeader->u64VPTS);
// 			}



		}



	}
}


#endif


CSrajwToMediaInterface::CSrajwToMediaInterface()
{
	m_dTickHearbeat = CrossGetTickCount64();
}


CSrajwToMediaInterface::~CSrajwToMediaInterface()
{
}
void CSrajwToMediaInterface::OpenTransmisDevice(char * p1, char * p2, char * p3, char * p4, int val1, int val2, int val3, int val4)
{
#if USE_SRA_JIWEI
	g_srajwinterface = this;


	sP2PSystemInitPrmType  sPrm;
	memset(&sPrm, 0, sizeof(sPrm));
	//---------------------------
	//匿名账户信息
	sPrm.dwID = 517400;// 匿名
	sPrm.dwR1 = 0;
	sPrm.dwR2 = 0;
	//---------------------------
	sPrm.pHostName = "|p2p1.videoipcamera.cn|p2p2.videoipcamera.com|p2p3.videoipcamera.cn|p2p4.videoipcamera.com|p2p5.videoipcamera.cn|p2p6.videoipcamera.com";
	sPrm.dwCustomerID[0] = 0;
	sPrm.dwCustomerID[1] = 0;
	sPrm.dwCustomerID[2] = 0;
	sPrm.dwCustomerID[3] = 0;
	sPrm.dwCustomerID[4] = 0;
	sPrm.dwCustomerID[5] = 0;
	sPrm.dwCustomerID[6] = 0;
	sPrm.dwCustomerID[7] = 0;
	sPrm.dwCustomerID[8] = 0;
	sPrm.dwCustomerID[9] = 0;
	sPrm.vP2PReceiveAlarmMesg = vP2PReceiveAlarmMesg;
	sPrm.vRecvMessage = vRecvMessage;
	sPrm.vSendMessageAck = vSendMessageAck;
	sPrm.vDevicesStatusUpdate = vDevicesStatusUpdate;
	sPrm.vGetFileCmdACK = vGetFileCmdACK;
	sPrm.vUpdateMesgFlag = vUpdateMesgFlag;
	sPrm.LoginInAnotherOne = LoginInAnotherOne;
	if (!fgP2PSystemInit_C(&sPrm))
	{
		OutputDebugString(L"P2P系统初始化失败\n");
		return;//
	}


	sP2PChannelPrmType srajwpPrm;
	srajwpPrm.dwP2PChannelID = 235;
	srajwpPrm.fgDecodeAudio = NULL;
	srajwpPrm.fgDecodeVideo = NULL;
	srajwpPrm.vInitDecoder = NULL;
	srajwpPrm.vDestoryDecoder = NULL;
	srajwpPrm.fgEncodeAudio = NULL;
	srajwpPrm.fgEncodeVideo = NULL;
	srajwpPrm.vInitEncoder = NULL;
	srajwpPrm.vDestoryEncoder = NULL;
	srajwpPrm.vP2PReceiveAVGroup = SRA_vP2PReceiveAVGroup;// vP2PReceiveAVGroup;
	int iRet = iP2PCreateChannel_C(&srajwpPrm);
	
	if (1 != iRet)
	{
		vP2PSystemExit_C();
		return;
	}

	int a = atoi(p1);
	int b = GetPassword((char *)p2);
	iP2PConnectTo_C(235, a, b, VIDEO_QUALITY_720P);

	m_dTickHearbeat = CrossGetTickCount64();

#endif
}
void CSrajwToMediaInterface::CloseTransmisDevice()
{
#if USE_SRA_JIWEI
	vP2PDisconnect_C(235);
	vP2PDestoryChannel_C(235);
	vP2PSystemExit_C();

#endif
}
BOOL CSrajwToMediaInterface::GetError()
{

	TRACE("CrossGetTickCount64() - m_dTickHearbeat = %d", CrossGetTickCount64() - m_dTickHearbeat);


	if ((CrossGetTickCount64() - m_dTickHearbeat) > (10 * 1000))
	{
		return TRUE;//超时出错
	}

	return CTransmisStreamToPlatform::Instance().GetError();//发送到服务器线程出错
}



