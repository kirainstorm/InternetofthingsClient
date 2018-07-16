#ifndef OUR_RTSP_CLIENT_H
#define OUR_RTSP_CLIENT_H
#include "liveMedia.hh"
#include "BasicUsageEnvironment.hh"
#include "StreamClientState.h"
#include "MIH264Sink.h"

class ourRTSPClient: public RTSPClient {
public:
  static ourRTSPClient* createNew(UsageEnvironment& env, char const* rtspURL,
				  int verbosityLevel = 0,
				  char const* applicationName = NULL,
				  portNumBits tunnelOverHTTPPortNum = 0);
  void setAVCallBack(AVCallback pAVFun);

protected:
  ourRTSPClient(UsageEnvironment& env, char const* rtspURL,
		int verbosityLevel, char const* applicationName, portNumBits tunnelOverHTTPPortNum);
    // called only by createNew();
  virtual ~ourRTSPClient();

public:
  StreamClientState scs;
  AVCallback  m_pAVFun;
};
#endif
