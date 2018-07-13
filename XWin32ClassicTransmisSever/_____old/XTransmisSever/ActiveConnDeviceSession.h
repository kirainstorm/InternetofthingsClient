#pragma once



#include "DataTypeTransProc.h"
#include "RecycleService.h"

class CActiveConnDeviceSession : public CXNetStreamData, public CXRecycleSession
{
public:
	CActiveConnDeviceSession(CXNetStream * pAcceptStream);
	~CActiveConnDeviceSession(void);

public:
	virtual void SessionDelete();

public:
	virtual void OnPacketCompleteNetStreamData(int32_t bytesTransferred, emXStreamTansferKey transferKey);

private:
	void DoMsg();
	CXNetStream * m_pTcpStream;
private:
	BOOL m_bCheckError;
	BOOL m_bNetIsError;
	CROSS_DWORD64 m_dwTick;

private:
	emNetRecieveStep m_nRcvStep;
	TRANSMIN_PROC_HEAD__T m_headRecv;
	TRANSMIN_PROC_HEAD__T m_headSend;
	int m_nSession;
};

