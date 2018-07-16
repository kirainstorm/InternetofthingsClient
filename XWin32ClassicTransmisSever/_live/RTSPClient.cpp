#include "stdafx.h"

#include "CRTSPSession.h"
#include "RTSPClient.h"
//#include <pthread.h>

//C api
CRTSPSession* gpRtsp = NULL;

extern "C"{

void RTSPClientDelete()
{
	if(gpRtsp==NULL)
		return;
	gpRtsp->stopRTSPClient();
	//sleep(2);
	delete gpRtsp;
	gpRtsp = NULL;
}

int RTSPClientInit(AVCallback pAVFun )
{
	if(gpRtsp)
		delete gpRtsp;
	
	gpRtsp = new CRTSPSession;
	gpRtsp->setAVHandle(pAVFun);
	
	return 1;
}

int RTSPClientStart(char* rtspUrl )
{
	printf("RTSPClientStart called.\n");
	return gpRtsp->startRTSPClient("test", rtspUrl, 1);
}
int RTSPClientStop( )
{
	gpRtsp->stopRTSPClient();
	return 1;
}
#if 0
int RTSPClientStart(char* rtspUrl, AVCallback pAVFun)
{
	int ret  = -1;
	if(gpRtsp)
		delete gpRtsp;
	gpRtsp = new CRTSPSession;
	gpRtsp->setAVHandle(pAVFun);
	ret = gpRtsp->startRTSPClient("test", rtspUrl, 0);
	
}
int RTSPClientStop( )
{
	if(gpRtsp==NULL)
		return 1;
	gpRtsp->stopRTSPClient();
	sleep(1);
	delete gpRtsp;
	gpRtsp = NULL;
	return 1;
}
#endif


}//end extern "C"
