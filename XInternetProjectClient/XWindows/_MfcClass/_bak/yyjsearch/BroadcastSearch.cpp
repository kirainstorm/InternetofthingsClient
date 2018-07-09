#include "StdAfx.h"
#include "BroadcastSearch.h"

#ifdef _PLATFORM_WINDOW
//#include "packet.h"

#include <ws2tcpip.h>
#include <winsock2.h>
#endif


#include "BroadcastSearch.h"
#include "PlatformDefine.h"
#ifdef _PLATFORM_ANDROID
#include <jni.h>
#include "BroadcastSearch.h"


extern JavaVM* g_JavaVM;
extern jobject g_MsgObj;


#endif



#ifdef _PLATFORM_IOS
#include <pthread.h>
#include <memory.h>
#include <string.h>
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/tcp.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/unistd.h>
#include <sys/fcntl.h>
#include <sys/time.h>
#endif
CBroadcastSearch::CBroadcastSearch()
{
#ifdef _PLATFORM_WINDOW
	WORD	wVersionRequested;
	WSADATA wsaData;

	wVersionRequested = MAKEWORD(2, 2);
	WSAStartup(wVersionRequested, &wsaData);
#endif

	m_hThread = CROSS_THREAD_NULL;
	m_nSocket			= INVALID_SOCKET;
#ifdef _PLATFORM_WINDOW
	m_pfncbProcMessage	= NULL;
#endif
	srand((unsigned int)time(NULL));
	//m_Sessionid = (unsigned int)CrossGetTickCount64();
}


CBroadcastSearch::~CBroadcastSearch()
{	
#ifdef _PLATFORM_WINDOW
	WSACleanup();
#endif
}

CBroadcastSearch& CBroadcastSearch::Instance()
{
	static CBroadcastSearch inc;
	return inc;
}
#define SEARCH_USE_BROADCAST 1
//创建socket(多播/广播)
#ifdef _PLATFORM_WINDOW
BOOL CBroadcastSearch::Initialization(xxxxsearch_callback pcallback, void *pUser)
#else
BOOL CBroadcastSearch::Initialization()
#endif
//BOOL CBroadcastSearch::Initialization(search_callback pcallback, void *pUser)
{

#ifdef _PLATFORM_ANDROID
	Log("0-------0------1111111111");
#endif

	

	//
	int iRet;
#ifdef _PLATFORM_WINDOW
	m_pfncbProcMessage = pcallback;
	m_pUser = pUser;
#endif
	//

	//
	if (INVALID_SOCKET != m_nSocket){
		return TRUE;
	}
#ifdef _PLATFORM_ANDROID
	Log("0-------0------22222222222");
#endif
	do 
	{
		m_nSocket = socket(AF_INET, SOCK_DGRAM, 0);
		if (m_nSocket < 0){
			break;
		}
#ifdef _PLATFORM_ANDROID
		Log("0-------0------333333333");
#endif
#if SEARCH_USE_BROADCAST
		const int fBroadcast = 1;
		iRet = setsockopt(m_nSocket, SOL_SOCKET, SO_BROADCAST, (char *)&fBroadcast, sizeof(int));
		if (iRet < 0)
		{
			break;
		}
#endif

#ifdef _PLATFORM_ANDROID
		Log("0-------0------444444444");
#endif
#ifdef _PLATFORM_WINDOW
		SOCKADDR_IN _addrin;
#endif
#ifdef _PLATFORM_ANDROID
		struct sockaddr_in _addrin;
#endif
        
#ifdef _PLATFORM_IOS
        struct sockaddr_in _addrin;
#endif
        
        
        
		memset((char *)&_addrin, 0, sizeof(_addrin));

		_addrin.sin_family = AF_INET;
		_addrin.sin_addr.s_addr = INADDR_ANY;

#ifdef _PLATFORM_ANDROID
		Log("0-------0------5555555555");
#endif
		int i;
		for (i = 0; i < MAX_BIND_TIME; i++)
		{
			_addrin.sin_port = htons(BROADCAST_RECV_PORT + i);
			iRet = ::bind(m_nSocket, (sockaddr *)&_addrin, sizeof(_addrin));
			//Log("bind iRet: %d", iRet);
			if (iRet < 0)
			{
				continue;
			}

			break;
		}

		if (iRet < 0)
		{
			break;
		}

		//
		m_bIsStoped = FALSE;
		m_hThread = CrossCreateThread(ThreadWorker, this);
#ifdef _PLATFORM_ANDROID
		Log("0-------0------CBroadcastSearch ini ok");
#endif
		return TRUE;

	} while (FALSE);


#ifdef _PLATFORM_ANDROID
	Log("0-------0------666666666");
#endif

	shutdown(m_nSocket, 0);
	CROSS_ClOSESOCKET(m_nSocket);

	return FALSE;
}

//关闭socket(广播)
BOOL CBroadcastSearch::unInitialization()
{
	if (m_nSocket != INVALID_SOCKET)
	{
		shutdown(m_nSocket,0);
		CROSS_ClOSESOCKET(m_nSocket);
		m_nSocket = INVALID_SOCKET;
	}
	m_bIsStoped = TRUE;
	if (m_hThread != CROSS_THREAD_NULL)
	{
		CrossWaitThread(m_hThread);
		CrossCloseThread(m_hThread);
		m_hThread = CROSS_THREAD_NULL;
	}

	return TRUE;
}

//向dvs发送搜索命令
BOOL CBroadcastSearch::Search()
{
 	memset(m_szBuffer, 0, sizeof(m_szBuffer));
	//
#ifdef _PLATFORM_WINDOW
	SOCKADDR_IN _addrin;
#else
	struct sockaddr_in _addrin;
#endif
	//
	memset(&_addrin, 0, sizeof(_addrin));
	_addrin.sin_family = AF_INET;
	_addrin.sin_addr.s_addr = INADDR_BROADCAST;
	_addrin.sin_port = htons(BROADCASTSEARCHPORT);


	//
	XSEARCH_MESSAGE_HEAD pkt;
	memset(&pkt, 0, sizeof(XSEARCH_MESSAGE_HEAD));
	pkt.magicnum = MAGIC_NUM;
	pkt.cmd = _X_CMD_SEACH_TOOL;
	//
	int SendLen = sizeof(XSEARCH_MESSAGE_HEAD);
	memcpy(m_szBuffer, (unsigned char *)&pkt, sizeof(XSEARCH_MESSAGE_HEAD));
	//
	int n = sendto(m_nSocket, m_szBuffer, SendLen, 0, (sockaddr *)&_addrin, sizeof(_addrin));

	//CROSS_TRACE("0-------0------sendto=%d    SendLen=%d", n, SendLen);

	return (n > 0) ? TRUE : FALSE;
}



int CBroadcastSearch::ThreadWorker(void * param)
{
	CBroadcastSearch *p = (CBroadcastSearch *)param;
	if (p)
	{
		p->Worker();
	}
	return 0;
}

//接收ipc发送来的信息
void CBroadcastSearch::Worker()
{
#ifdef _PLATFORM_ANDROID
	JNIEnv *env;			////////////////////////////
	jclass jclsProcess;
	jmethodID jmidProcess;  ///////////////////////////
	bool isAttached = false;
	int status;
	status = g_JavaVM->GetEnv((void **)&env, JNI_VERSION_1_4);
	if (status < 0) {
		status = g_JavaVM->AttachCurrentThread(&env, NULL);
		if (status < 0) {
			Log("_SearchListenThread: AttachCurrentThread Failed!!");
			return;
		}
		isAttached = true;
	}

	jclsProcess = env->GetObjectClass(g_MsgObj);
	if (jclsProcess == NULL) {
		Log("_SearchListenThread: m_envLocalCmd->GetObjectClass(g_CmdObj) error\n");
		return;
	}

	jmidProcess = env->GetMethodID(jclsProcess, "SearchNotify", "([BI)V");
	if (jmidProcess == NULL) {
		Log("env->GetMethodID(clazz, \"SearchNotify\", error\n");
		return;
	}


	//pService->m_jmidMsgNotify = jmidProcess;
	//pService->m_envLocalMsg = env;
#endif


	struct sockaddr_in  cliAddr;
	int					bytes = 0;
	int					cliAddrLen = sizeof(struct sockaddr_in);
	char				szBuffer[1400];

	//select
	// 	struct timeval timeout;
	// 	fd_set r;
	// 
	// 	FD_ZERO(&r);
	// 	timeout.tv_sec = 1;
	// 	timeout.tv_usec = 0;
	// 
	// 	while (!m_bExit) {
	// 
	// 
	// 		FD_SET(sockfd, &r);
	// 		int ret = select(sockfd + 1, &r, 0, 0, &timeout);
	// 		if (ret <= 0) {
	// 			continue;
	// 		}
	// 		else if (ret == -1) {
	// 			usleep(100 * 1000);
	// 			continue;
	// 		}
	// 	}


	while (FALSE == m_bIsStoped)
	{
		memset(&cliAddr, 0, sizeof(struct sockaddr_in));
		memset(szBuffer, 0, sizeof(szBuffer));
		bytes = recvfrom(m_nSocket, szBuffer, sizeof(szBuffer), 0, (sockaddr*)&cliAddr, (socklen_t *)&cliAddrLen);
        //bytes = recvfrom(m_nSocket, szBuffer, 1024, 0, (sockaddr*)&cliAddr, (socklen_t *)&cliAddrLen);
        
        
		if (bytes == SOCKET_ERROR || bytes == 0)
		{
			//DWORD dwError = ::GetLastError();
			m_bIsStoped = TRUE;
		}
		else
		{
			char *pStr = inet_ntoa(cliAddr.sin_addr);

			//只接收自己的搜索回应
			XSEARCH_MESSAGE_HEAD *pHead = (XSEARCH_MESSAGE_HEAD *)szBuffer;
			if (
				(pHead->magicnum != MAGIC_NUM) ||
				(pHead->cmd != _X_CMD_SEACH_TOOL) ||
				(pHead->result != _X_CMD_RESULT_OK) ||
				(pHead->datalen != sizeof(XSEARCH_MESSAGE_T))
				)
			{
				continue;
			}


#ifdef _PLATFORM_WINDOW
			if (m_pfncbProcMessage)
			{
				m_pfncbProcMessage(pStr,(const XSEARCH_MESSAGE_T  *)(szBuffer + sizeof(XSEARCH_MESSAGE_HEAD)), m_pUser);//将设备信息回给用户
			}
#endif
            
            
#ifdef _PLATFORM_IOS
//             if (searchResultDelegate != nil)
//             {
//                 //NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
//                 
//                 ANNP_PACKET *pbuf = (ANNP_PACKET *)szBuffer;
//                 if (pbuf->magic_number != MAGICNM)
//                     continue;
//                 
//                 WORD type = htons(pbuf->type);
//                 switch (type) {
//                     case OFFER:
//                     {
//                         //////////////////////////////////////////////////////////////////////////
//                         //IPNCClass Ipnc;
//                         pbuf->messages[511] = 0;
//                         char *pMsg = (char *)pbuf->messages;
//                         char szIP[32] = { 0 };
//                         char szMacUboot[32] = { 0 };
//                         char szTmp[64] = { 0 };
//                         char szPort[16] = { 0 };
//                         sscanf(pMsg, "%[^;];%[^;];%[^;];%[^;];%[^;];%[^;];%[^;];%[^;];",
//                                szTmp, szTmp, szTmp, szIP, szTmp, szTmp, szPort, szMacUboot/*szTmp*/);
// 
//                         [searchResultDelegate SearchCameraResult:nil
//                                                             Name:[NSString stringWithUTF8String:szIP]
//                                                             Addr:[NSString stringWithUTF8String:szIP]
//                                                             Port:[NSString stringWithUTF8String:szPort]
//                                                              DID:nil];
//                         
//                         
//                     }
//                         break;
//                 }
//             }
#endif
            
            
            
            
#ifdef _PLATFORM_ANDROID

			XSEARCH_MESSAGE_T * ptmp = (XSEARCH_MESSAGE_T  *)(szBuffer + sizeof(XSEARCH_MESSAGE_HEAD));


			////ANNP_PACKET *pbuf = (ANNP_PACKET *)szBuffer;
			//if (pbuf->magic_number != MAGICNM)
			//	continue;

			//WORD type = htons(pbuf->type);
			//switch (type) {
			//case OFFER:
			{
				//////////////////////////////////////////////////////////////////////////
				//IPNCClass Ipnc;
// 				pbuf->messages[511] = 0;
// 				char *pMsg = (char *)pbuf->messages;
// 				char szIP[32] = { 0 };
// 				char szMacUboot[32] = { 0 };
// 				char szTmp[64] = { 0 };
// 				char szPort[16] = { 0 };
// 
// 				sscanf(pMsg, "%[^;];%[^;];%[^;];%[^;];%[^;];%[^;];%[^;];%[^;];",
// 					szTmp, szTmp, szTmp, szIP, szTmp, szTmp, szPort, szMacUboot/*szTmp*/);


				char szCallback[512] = {0};

				CROSS_SPRINTF(szCallback, "%s#%s#%s", pStr, ptmp->mac_wired, ptmp->uuid);





				int NLEN = strlen(szCallback);
				jbyteArray jBuff;
				jBuff = env->NewByteArray(NLEN);
				env->SetByteArrayRegion(jBuff, 0, NLEN, (jbyte*)szCallback);
				//回调java，通知
				env->CallVoidMethod(g_MsgObj, jmidProcess,jBuff,0);
				env->DeleteLocalRef(jBuff);






			}
// 			break;
// 			}


#endif





			//TRACE(".............>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\n");


// 			int mmm = sizeof(BCASTPARAM);
// 			for (int i = 0 ; i< bytes ; i++)
// 			{
// 				char sss[2] = {szBuffer[i]};
// 				TRACE(sss);
// 			}
			//OnMessageProc(szBuffer, bytes, pStr);

			/*
				PMESSAGE_HEAD pstMsgHead = (PMESSAGE_HEAD)pszBuffer;

				if (pstMsgHead->nStartCode != STARTCODE)
				{
				return FALSE;
				}
				// 	pstMsgHead->nStartCode	= STARTCODE;
				// 	pstMsgHead->nCmd	= CMD_GET_NET_PARAM_REQUEST;

				// 	CPacket			packet;
				// 	const char		*pszMessage;
				// 	unsigned short	nType, nMessageLen, nSessionID;
				//
				// 	pszMessage = packet.Unpacket(pszBuffer, iBufferSize, nType, nSessionID, nMessageLen);


				//TRACE(" ###>>> %04x   rev:%d  sizeof=%d\n",pstMsgHead->nCmd,iBufferSize,sizeof(BCASTPARAM));
				switch(pstMsgHead->nCmd)
				{
				case CMD_GET_NET_PARAM_RESPONSE:
				{
				if (m_pfncbProcMessage)
				{
				m_pfncbProcMessage(pstMsgHead->nCmd, iBufferSize-sizeof(MESSAGE_HEAD), pszBuffer+sizeof(MESSAGE_HEAD), m_pUser);//将设备信息回给用户
				}
				}
				break;
				case CMD_SET_NET_PARAM_RESPONSE:
				{
				int	nResult=0;
				memcpy(&nResult, pszBuffer+sizeof(MESSAGE_HEAD)+sizeof(BCASTPARAM), sizeof(int));
				if (nResult == STATUS_OK)
				{
				SetEvent(m_hSetParamEvent);
				TRACE(" ###>>> 设置设备参数成功!\r\n");
				}
				}
				break;
				}
			*/
		}
	}


#ifdef _PLATFORM_ANDROID
	if (isAttached)
		g_JavaVM->DetachCurrentThread();
#endif
}









































/*
void WriteFlashData(CString strFileName ,const char *pszFileData, unsigned int dwFileLength)
{
	unsigned int	iPos, length;
	FILE			*fp = fopen((const char *)strFileName.GetBuffer(strFileName.GetLength()), "wb");
	strFileName.ReleaseBuffer();
	if (fp)
	{
		iPos = 0;
		while (iPos < dwFileLength)
		{
			length = dwFileLength - iPos;
			length = length > 1024 ? 1024 : length;
			fwrite(pszFileData, 1, length, fp);
			iPos += length;
			pszFileData += length;
		}

		fclose(fp);
	}
}
//读flash
void GetFlashThread()
{

}
BOOL CBroadcastSearch::GetFlashData(CString strFileName , PBCASTPARAM pstParam)
{
	int	iRet;
	sGetFlash = socket(AF_INET, SOCK_DGRAM, 0);
	if (sGetFlash == INVALID_SOCKET)
	{
		return FALSE;
	}

	BOOL fBroadcast = TRUE;
	iRet = setsockopt(sGetFlash, SOL_SOCKET, SO_BROADCAST, (CHAR *)&fBroadcast, sizeof(BOOL));
	if (SOCKET_ERROR == iRet)
	{
		closesocket(sGetFlash);
		sGetFlash = INVALID_SOCKET;
		return FALSE;
	}


	SOCKADDR_IN saUdpCli;
	memset((char *)&saUdpCli, 0, sizeof(saUdpCli));  

	unsigned short	m_nBCPort;
	m_nBCPort = rand() % 65535;
	if (m_nBCPort < 1000)
	{
		m_nBCPort += 1000;
	}
	saUdpCli.sin_family = AF_INET;
	saUdpCli.sin_addr.s_addr = htonl(INADDR_ANY);
	saUdpCli.sin_port = htons(m_nBCPort); //BROADCAST_RECV_PORT);
	iRet = bind(sGetFlash, (SOCKADDR *)&saUdpCli, sizeof(SOCKADDR_IN));
	if (SOCKET_ERROR == iRet)
	{
		//::MessageBox(NULL, "bind failure", "SearchDVS", MB_OK);
		closesocket(sGetFlash);
		sGetFlash = INVALID_SOCKET;
		return FALSE;
	}

	char szBuffer[1400];
	char *pos			= szBuffer;
	PFLASHDATAHEAD	pstHead;
	int iSendBufferSize = sizeof(BCASTPARAM) + sizeof(FLASHDATAHEAD);

	memset(szBuffer, 0, sizeof(szBuffer));
	memcpy(pos, pstParam, sizeof(BCASTPARAM)); 
	pos	+= sizeof(BCASTPARAM);
	unsigned short m_nTotalCount	= 8 * 1024;
	unsigned short m_nCurIndex = 0;
	pstHead		= (PFLASHDATAHEAD)pos;

	struct sockaddr_in  cliAddr;
	int					cliAddrLen = sizeof(struct sockaddr_in);
	char *m_pszFlashData	= new char[8 * 1024 * 1024 + 1];

	while(!m_bIsStoped)
	{
		TRACE("==================>>>>>>      %d\n",m_nCurIndex);


		//pstHead		= (PFLASHDATAHEAD)pos;
		pstHead->nPackIndex = m_nCurIndex;
		pstHead->nPackCount	= m_nTotalCount;
		pstHead->nPackLen	= 0;

		CPacket		packet;
		int			iBufferSize;
		const char	*pszBuffer = packet.Packet(GET_FLASH_DATA_REQUEST, 0, szBuffer, iSendBufferSize, iBufferSize);
		int iLen = sendto(sGetFlash, pszBuffer, iBufferSize, 0, (sockaddr *)&servAddr, sizeof(SOCKADDR_IN));



		char szRecvBuffer[1400]={0};
		int bytes = recvfrom(sGetFlash, szRecvBuffer, sizeof(szRecvBuffer), 0, (LPSOCKADDR)&cliAddr, (int *)&cliAddrLen);
		if (bytes == SOCKET_ERROR || bytes == 0)
		{
			DWORD dwError = ::GetLastError();
			m_bIsStoped = TRUE;
			shutdown(sGetFlash,0);
			closesocket(sGetFlash);
			sGetFlash = INVALID_SOCKET;
			return FALSE;
		}


		//CPacket			packet;
		const char		*pszMessage;
		unsigned short	nType, nMessageLen, nSessionID;

		pszMessage = packet.Unpacket(szRecvBuffer, bytes, nType, nSessionID, nMessageLen);
		pszMessage+=sizeof(MESSAGE_HEAD);

// 		unsigned short	nResult=0;
// 		memcpy(&nResult, pszMessage+sizeof(MESSAGE_HEAD), sizeof(unsigned short));

// 		if (!nResult)
// 		{
// 			Sleep(10);
// 			continue;
// 		}


		PBCASTPARAM		pstParamRecv = (PBCASTPARAM)pszMessage;
		PFLASHDATAHEAD	pstHeadRecv = (PFLASHDATAHEAD)(pszMessage+sizeof(BCASTPARAM));
		const char		*posRecv = pszMessage+sizeof(BCASTPARAM)+sizeof(FLASHDATAHEAD);

		memcpy(&m_pszFlashData[pstHeadRecv->nPackIndex * 1024], posRecv, pstHeadRecv->nPackLen);

		if (m_nCurIndex == pstHeadRecv->nPackIndex)
		{
			m_nCurIndex++;
			//m_oProcessDialog.UpdatePosition(m_nCurIndex);
		}

		if (m_nCurIndex == pstHeadRecv->nPackCount)
		{
			unsigned short m_dwFileLength = 8 * 1024 * 1024;
			WriteFlashData(strFileName ,m_pszFlashData, m_dwFileLength);
			delete m_pszFlashData;
			m_dwFileLength = 0;

			shutdown(sGetFlash,0);
			closesocket(sGetFlash);
			sGetFlash = INVALID_SOCKET;
			return TRUE;
			//GetDlgItem(IDC_BUTTON_READ_FLASH)->EnableWindow(TRUE);
			//m_oProcessDialog.ShowWindow(SW_HIDE);
			//MessageBox("获取Flash数据完成!");
		}
		
	}

	delete m_pszFlashData;
	shutdown(sGetFlash,0);
	closesocket(sGetFlash);
	sGetFlash = INVALID_SOCKET;
	return TRUE;
}
*/