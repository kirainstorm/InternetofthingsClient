#pragma once

#include <windows.h>
#include <atlstr.h>
#include "XStream.h"

class CTransmisService :public CXNetStreamData
{
private:
	CTransmisService(void);
	~CTransmisService(void);

public:
	static CTransmisService& Instance();
public:
	void OnPacketCompleteNetStreamData(int32_t bytesTransferred, emXStreamTansferKey transferKey);

public:
	int Start(int session_id);
	int Stop();
	BOOL GetError(){ return m_bError; };

private:
	int m_nSessionID;
	CXNetStream * m_TcpStream;
	CXNetStream * m_pTimer;
	BOOL m_bError;
};
