#pragma once


#include "BroadcastSearchStruct.h"
#include "PlatformDefine.h"
#include "XCross.h"


#define MAX_BIND_TIME 10
#define BROADCAST_RECV_PORT			5280

#ifdef _PLATFORM_IOS
#import "IOSDevInterfaceProtocol.h"
#endif


class CBroadcastSearch
{
public:
	CBroadcastSearch();
	~CBroadcastSearch();
#ifdef _PLATFORM_IOS
    id<SearchCameraResultProtocol> searchResultDelegate;
#endif
    
    
public:
	static CBroadcastSearch& Instance();

public:
#ifdef _PLATFORM_WINDOW
	BOOL Initialization(xxxxsearch_callback pcallback, void *pUser);
#else
	BOOL Initialization();
#endif
	BOOL unInitialization();
	BOOL Search();

private:
	CROSS_SOCKET	m_nSocket;
	char m_szBuffer[1024];
	//int m_Sessionid;

private:
	BOOL			m_bIsStoped;
	CROSS_THREAD_HANDLE	m_hThread;
	static int ThreadWorker(void * param);
	void Worker();

private:
#ifdef _PLATFORM_WINDOW
	xxxxsearch_callback	*m_pfncbProcMessage;
	void			*m_pUser;
#endif
};

