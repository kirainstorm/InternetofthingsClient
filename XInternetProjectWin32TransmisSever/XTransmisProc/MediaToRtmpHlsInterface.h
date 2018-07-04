#pragma once


#include "librtmpapp.h"
#include "MediaDevInterface.h"

class CMediaToRtmpHlsInterface :public XTransmisDeviceInterface, public CXMediaStreamData// ,public CXNetStreamData
{
public:
	CMediaToRtmpHlsInterface();
	~CMediaToRtmpHlsInterface();

// public:
// 	void OnPacketCompleteNetStreamData(int32_t bytesTransferred, emXStreamTansferKey transferKey);
// private:
// 	CXNetStream * m_pTimer;

public:
	virtual void OpenTransmisDevice(char * p1 = NULL, char * p2 = NULL, char * p3 = NULL, char * p4 = NULL, int val1 = 0, int val2 = 0, int val3 = 0, int val4 = 0);
	virtual void CloseTransmisDevice();
	virtual BOOL GetError();
public:
	virtual int OnStreamData(ST_SERVER_HEAD head, const char* pStreamData, int nLen);

private:
	volatile BOOL m_bUserCloseThis;
	CMediaDevInterface *m_pDevObject;
	CrossCriticalSection m_cs;

	//char szTmpURL_rtmp[256];
	//CLibRTMPApp * m_pLibRTMP_rtmp;

	char szTmpURL_hls[256];
	CLibRTMPApp * m_pLibRTMP_hls;
	BOOL m_bRTMPError;

	time_t m_dwTick;

	DWORD64 m_dTickHearbeat;
};

