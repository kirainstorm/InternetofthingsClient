#ifndef CRTSP_SESSION_H
#define CRTSP_SESSION_H
#include "RTSPUlitily.h"
#include "PlatformDefine.h"
#include <string>
using namespace std;

class CRTSPSession 
{
public:
	 CRTSPSession();
	 virtual ~CRTSPSession();
	 int startRTSPClient(char const* progName, char const* rtspURL, int debugLevel);
	 int stopRTSPClient();
	 void setAVHandle(AVCallback pAVFun);
private:
	 int openURL(UsageEnvironment& env, char const* progName, char const* rtspURL, int debugLevel);
	 void rtsp_fun();
	 static int rtsp_thread_fun (void *param);
	 
private:
	 RTSPClient* m_rtspClient;

	 char eventLoopWatchVariable;

	 CROSS_THREAD_HANDLE tid;




	 bool m_running;

	 string m_rtspUrl;

	 string m_progName;

	 int m_debugLevel;

	 int m_nStatus;

	 AVCallback m_pAVFun;
};
#endif
