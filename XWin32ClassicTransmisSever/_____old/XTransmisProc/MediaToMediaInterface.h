#pragma once


#define USE_THREAD_NOT_TIMER 1

#include "MediaDevInterface.h"

class CMediaToMediaInterface :public XTransmisDeviceInterface,public CXMediaStreamData
{
public:
	CMediaToMediaInterface();
	~CMediaToMediaInterface();
//public:
	//void OnPacketCompleteNetStreamData(int32_t bytesTransferred, emXStreamTansferKey transferKey);
//private:
	//CXNetStream * m_pTimer;此处不能用timer，因为用的同一个StreamManager，嵌套调用了

public:
	virtual void OpenTransmisDevice(char * p1 = NULL, char * p2 = NULL, char * p3 = NULL, char * p4 = NULL, int val1 = 0, int val2 = 0, int val3 = 0, int val4 = 0);
	virtual void CloseTransmisDevice();
	virtual BOOL GetError();

public:
	virtual int OnStreamData(ST_SERVER_HEAD head, const char* pStreamData, int nLen);

private:
	DWORD64 m_dwLastTick;
	volatile BOOL m_bUserCloseThis;
	CMediaDevInterface *m_pDevObject;
	CrossCriticalSection m_cs;
	DeviceInfo_T t;
private:
	static void* WINAPI WorkerThread(void*);
	void Worker();
	HANDLE m_hThread;
	volatile BOOL m_hEvtWorkerStop;
};

