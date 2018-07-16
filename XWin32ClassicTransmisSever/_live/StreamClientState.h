#ifndef __SESSION_STATE_H
#define __SESSION_STATE_H
#include "liveMedia.hh"
#include "BasicUsageEnvironment.hh"

class StreamClientState
{
  public:
	StreamClientState();
	virtual ~StreamClientState();

  public:
	MediaSubsessionIterator *iter;
	MediaSession *session;
	MediaSubsession *subsession;
	TaskToken streamTimerTask;
	double duration;
};
#endif
