#include "stdafx.h"

#include "ourRTSPClient.h"

ourRTSPClient* ourRTSPClient::createNew(UsageEnvironment& env, char const* rtspURL,
					int verbosityLevel, char const* applicationName, portNumBits tunnelOverHTTPPortNum) 
{
  	return new ourRTSPClient(env, rtspURL, verbosityLevel, applicationName, tunnelOverHTTPPortNum);
}

void ourRTSPClient::setAVCallBack(AVCallback pAVFun)
{
	m_pAVFun = pAVFun;
}

ourRTSPClient::ourRTSPClient(UsageEnvironment& env, char const* rtspURL,
			     int verbosityLevel, char const* applicationName, portNumBits tunnelOverHTTPPortNum)
  : RTSPClient(env,rtspURL, verbosityLevel, applicationName, tunnelOverHTTPPortNum, -1) 
{
	m_pAVFun = NULL;
}

ourRTSPClient::~ourRTSPClient() 
{
	
}
