#include "stdafx.h"

#include "CRTSPSession.h"
//#include <pthread.h>
int CRTSPSession::rtsp_thread_fun(void *param)
{
	 CRTSPSession *pThis = (CRTSPSession*)param;
	 pThis->rtsp_fun ();
	 return 0;
}

CRTSPSession::CRTSPSession()
{
	 m_rtspClient = NULL;
	 m_running = false;
	 eventLoopWatchVariable = 0;
	 m_nStatus = 0;
	 m_pAVFun = NULL;
	  tid = CROSS_THREAD_NULL;

}

CRTSPSession::~CRTSPSession()
{

}

int CRTSPSession::startRTSPClient(char const* progName, char const* rtspURL, int debugLevel)
{
	printf("startRTSPClient called.\n");
	 m_progName = progName;
	 m_rtspUrl = rtspURL;
	 m_debugLevel = debugLevel;
	 eventLoopWatchVariable = 0;

	 if(m_nStatus==3)
	 	m_nStatus = 0;
	 else  if(m_nStatus==1||m_nStatus==2)//already start, must wait stop first!!
	 {
	 	printf("startRTSPClient error, already start, must wait stop first!!\n");
	 	return -1;
	 }
	 
 tid = CrossCreateThread(rtsp_thread_fun, this);

	 return 0;
}

int CRTSPSession::stopRTSPClient()
{

 	eventLoopWatchVariable = 1;


	if (tid != CROSS_THREAD_NULL)
	{
		CrossWaitThread(tid);
		CrossCloseThread(tid);
		tid = CROSS_THREAD_NULL;
	}




 	return 0;
}


void CRTSPSession::rtsp_fun()
{
	//Log("CRTSPSession::rtsp_fun  1....... \n");

 	TaskScheduler* scheduler = BasicTaskScheduler::createNew();
 	UsageEnvironment* env = BasicUsageEnvironment::createNew(*scheduler);
	m_nStatus = 1;
	//Log("CRTSPSession::rtsp_fun  2....... \n");
	 if (openURL(*env, m_progName.c_str(), m_rtspUrl.c_str(), m_debugLevel) == 0)
	 {
		 //Log("CRTSPSession::rtsp_fun  3....... \n");
		 m_nStatus = 2;
		 env->taskScheduler().doEventLoop(&eventLoopWatchVariable);
		 //Log("CRTSPSession::rtsp_fun  4....... \n");
		 m_running = false;
		 eventLoopWatchVariable = 0;
#if 0	 
		 if (m_rtspClient)
		 {
		 	shutdownStream(m_rtspClient,0);
		 }
	 	m_rtspClient = NULL;
#endif		 
	 }

	 _close_get_param_thread();
//Log("CRTSPSession::rtsp_fun  5....... \n");
	 env->reclaim(); 
	 env = NULL;
	 delete scheduler; 
	 scheduler = NULL;
	 m_nStatus = 3;
}


int CRTSPSession::openURL(UsageEnvironment& env, char const* progName, char const* rtspURL, int debugLevel)
{
 	m_rtspClient = ourRTSPClient::createNew(env, rtspURL, debugLevel, progName);
	
	if (m_rtspClient == NULL) 
	{
		env << "Failed to create a RTSP client for URL \"" << rtspURL << "\": " << env.getResultMsg() << "\n";
	 	return -1;
	}
	
	((ourRTSPClient*)m_rtspClient)->setAVCallBack(m_pAVFun);
	//((ourRTSPClient*)m_rtspClient)->m_nID = m_nID;
	m_rtspClient->sendDescribeCommand(continueAfterDESCRIBE); 
	return 0;
}

void CRTSPSession::setAVHandle(AVCallback pAVFun)
{
	m_pAVFun = pAVFun;
	if(m_rtspClient != NULL)
	{
		((ourRTSPClient*)m_rtspClient)->setAVCallBack(m_pAVFun);
	}
}
