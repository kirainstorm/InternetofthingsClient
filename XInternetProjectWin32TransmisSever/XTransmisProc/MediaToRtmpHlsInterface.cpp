#include "stdafx.h"
#include "MediaToRtmpHlsInterface.h"


CMediaToRtmpHlsInterface::CMediaToRtmpHlsInterface()
{

	WORD version;
	WSADATA wsaData;
	version = MAKEWORD(2, 2);
	WSAStartup(version, &wsaData);

	m_dwTick = 0;
	m_bRTMPError = FALSE;
//	m_pLibRTMP_rtmp = NULL;
	m_pLibRTMP_hls = NULL;
	m_bUserCloseThis = FALSE;
}


CMediaToRtmpHlsInterface::~CMediaToRtmpHlsInterface()
{
	WSACleanup();
}


void CMediaToRtmpHlsInterface::OpenTransmisDevice(char * p1, char * p2, char * p3, char * p4, int val1, int val2, int val3, int val4)
{
	m_dTickHearbeat = GetTickCount64();
	//
	DeviceInfo_T t;
	memset(&t, 0, sizeof(DeviceInfo_T));
	strcpy_s(t.core_svr_ip, p1);
	strcpy_s(t.user_name, p2);
	strcpy_s(t.user_pwd, p3);
	t.core_svr_port = val1;
	t.dev_id = val2;
	t.dev_channel = 0;
	//

	

#if 0
	memset(szTmpURL_rtmp, 0, sizeof(szTmpURL_rtmp));
	sprintf_s(szTmpURL_rtmp, "rtmp://%s:%d/myapp/%d", p4, val3, val4);
	m_pLibRTMP_rtmp = new CLibRTMPApp();
#endif



#if 1
	memset(szTmpURL_hls, 0, sizeof(szTmpURL_hls));
	sprintf_s(szTmpURL_hls, "rtmp://%s/hls/%d", p4, val4);
	m_pLibRTMP_hls = new CLibRTMPApp();
	m_pLibRTMP_hls->RTMP264_Connect(szTmpURL_hls);
#endif


	m_pDevObject = new CMediaDevInterface();// CreateMyMediaDeviceInterface();// pDevFun();
	m_pDevObject->XDeviceSetInfo(t);
	m_pDevObject->XDeviceLogin();
	m_pDevObject->XPreviewOpenForWindows(NULL, this);
}
void CMediaToRtmpHlsInterface::CloseTransmisDevice()
{
	//
	m_bUserCloseThis = TRUE;

	//
// 	if (m_pTimer)
// 	{
// 		m_pTimer->Release();
// 		m_pTimer = NULL;
// 	}

	if (m_pDevObject)
	{
		m_pDevObject->XPreviewClose();
		m_pDevObject->XDeviceLogout();
		m_pDevObject->XDeviceDelete();
		m_pDevObject = NULL;
	}

// 	if (m_pLibRTMP_rtmp)
// 	{
// 		m_pLibRTMP_rtmp->RTMP264_RTMP264_Close();
// 		delete m_pLibRTMP_rtmp;
// 		m_pLibRTMP_rtmp = NULL;
// 	}
	if (m_pLibRTMP_hls)
	{
		m_pLibRTMP_hls->RTMP264_RTMP264_Close();
		delete m_pLibRTMP_hls;
		m_pLibRTMP_hls = NULL;
	}


	delete this;
}
BOOL CMediaToRtmpHlsInterface::GetError()
{

	if ((CrossGetTickCount64() - m_dTickHearbeat) > 5*1000)
	{
		return TRUE;
	}

	return m_bRTMPError;
}
// 
// void CMediaToRtmpHlsInterface::OnPacketCompleteNetStreamData(int32_t bytesTransferred, emXStreamTansferKey transferKey)
// {
// 	if (m_bUserCloseThis)
// 	{
// 		return;
// 	}
// 
// 
// 	if (ENUM_XTSTREAM_TRANSKEY_TIMER == transferKey)
// 	{
// 		m_cs.Lock();
// 
// 		if (m_bRTMPError)
// 		{
// 
// 			if (m_pLibRTMP_hls)
// 			{
// 				m_pLibRTMP_hls->RTMP264_RTMP264_Close();
// 
// 				if (0 == m_pLibRTMP_hls->RTMP264_Connect(szTmpURL_hls))
// 				{
// 					m_dwTick = 0;
// 					m_bRTMPError = FALSE;
// 				}
// 			}
// 
// 
// 
// 
// #if 0
// 			if (m_pLibRTMP_rtmp)
// 			{
// 				if (0 != m_pLibRTMP_rtmp->RTMP264_Connect(szTmpURL_rtmp))
// 				{
// 					break;
// 				}
// 			}
// #endif
// 
// 
// 			// 	// 		if (0 != m_pLibRTMP_rtmp->RTMP264_Connect(szTmpURL_rtmp))
// 			// 	// 		{
// 			// 	// 			break;
// 			// 	// 		}
// 
// 
// 
// 		}
// 
// 		m_cs.Unlock();
// 
// 		m_pTimer->PostDelayTimer(10);
// 	}
// }


int CMediaToRtmpHlsInterface::OnStreamData(ST_SERVER_HEAD head, const char* pStreamData, int nLen)
{
#if 0
	m_dTickHearbeat = CrossGetTickCount64();//
	return 0;
#endif

	m_cs.Lock();

	//----------------------------------------
	if (m_bRTMPError)
	{
		m_cs.Unlock();
		return 0;
	}
	//---------------------------------------

	struct tm gm = { head.wSecond, head.wMinute, head.wHour, head.wDay, head.wMonth - 1, head.wYear - 1900, 0, 0, 0 };
	if (0 == m_dwTick)
	{
		m_dwTick = (mktime(&gm) * 1000 + head.wMilliseconds);
	}
 
	uint32_t nTimestamp = (uint32_t)((mktime(&gm) * 1000 + head.wMilliseconds) - m_dwTick);

	if (nTimestamp > 400000000)
	{
		m_dwTick = (mktime(&gm) * 1000 + head.wMilliseconds);
		nTimestamp = (uint32_t)((mktime(&gm) * 1000 + head.wMilliseconds) - m_dwTick);
	}

	char szPeee[256] = { 0 };
	//sprintf_s(szPeee, "#####*********************************####### --- %d\n", nTimestamp);
	OutputDebugStringA(szPeee);

// 	if ((m_pLibRTMP_rtmp) && (head.nAVFrameType < 6))
// 	{
// 		int err = m_pLibRTMP_rtmp->RTMP264_SendH264Packet((unsigned char *)pStreamData, nLen, (head.nAVFrameType == XSERVER_FREAM_TYPE_H264_IFRAME)? 1:0, nTimestamp);
// 		if (0 == err)
// 		{
// 			//m_dTickHearbeat = CrossGetTickCount64();//
// 		}
// 		else
// 		{
// 			m_bRTMPError = TRUE;
// 		}
// 	}
	if ((m_pLibRTMP_hls) && (head.nAVFrameType < 6))
	{
		int err = m_pLibRTMP_hls->RTMP264_SendH264Packet((unsigned char *)pStreamData, nLen, (head.nAVFrameType == XSERVER_FREAM_TYPE_H264_IFRAME) ? 1 : 0, nTimestamp);
		if (0 == err)
		{
			m_dTickHearbeat = CrossGetTickCount64();//
		}
		else
		{
			m_bRTMPError = TRUE;
		}
	}

// 	if ((m_pLibRTMP_rtmp) && (head.nAVFrameType == XSERVER_FREAM_TYPE_AAC))
// 	{
// 		int err = m_pLibRTMP_rtmp->RTMP264_SendAACPacket((unsigned char *)pStreamData, nLen, 8000, nTimestamp);
// 		if (0 == err)
// 		{
// 			//m_dTickHearbeat = CrossGetTickCount64();//
// 		}
// 		else
// 		{
// 			m_bRTMPError = TRUE;
// 		}
// 	}
	if ((m_pLibRTMP_hls) && (head.nAVFrameType == XSERVER_FREAM_TYPE_AAC))
	{
		int err = m_pLibRTMP_hls->RTMP264_SendAACPacket((unsigned char *)pStreamData, nLen, 8000, nTimestamp);
		if (0 == err)
		{
			m_dTickHearbeat = CrossGetTickCount64();//
		}
		else
		{
			m_bRTMPError = TRUE;
		}
	}

	m_cs.Unlock();


	return 0;


}
