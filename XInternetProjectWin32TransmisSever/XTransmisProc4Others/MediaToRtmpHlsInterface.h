#pragma once

#include "mediainterfacedefine.h"
#include "librtmpapp.h"

class CMediaToRtmpHlsInterface :public XTransmisDeviceInterface, public CXMediaStreamData
{
public:
	CMediaToRtmpHlsInterface();
	~CMediaToRtmpHlsInterface();

public:
	virtual void OpenTransmisDevice(char * p1 = NULL, char * p2 = NULL, char * p3 = NULL, char * p4 = NULL, int val1 = 0, int val2 = 0, int val3 = 0, int val4 = 0);
	virtual void CloseTransmisDevice();
	virtual BOOL GetError();
public:
	virtual int OnStreamData(ST_SERVER_HEAD head, const char* pStreamData, int nLen);

private:
	CXMediaDeviceInterface *m_pDevObject;
	CROSS_DWORD64 m_dTickHearbeat;//

private:
	CLibRTMPApp * m_pLibRTMP_rtmp;
	CLibRTMPApp * m_pLibRTMP_hls;


	CROSS_DWORD64 m_dwTick;

};

