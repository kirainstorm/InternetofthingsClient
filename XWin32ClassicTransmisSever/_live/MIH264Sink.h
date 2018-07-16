#ifndef MI_H264_SINK_H
#define MI_H264_SINK_H
#include "liveMedia.hh"
#include "BasicUsageEnvironment.hh"


struct spspps_t
{

	unsigned char spspps_buffer[2048];
	unsigned int spspps_len;
};


//data:  audio or video data; len:data length; type: 0 is audio, 1 is video; iframe: 0 not key frame,  1 key frame
typedef void (* AVCallback) (unsigned char* data, int len, int type,  int iframe,  struct timeval timestamp);
enum
{
	AUDIO_FRAME,
	VIDEO_FRAME
};

class MIH264Sink: public MediaSink {
public:
  static MIH264Sink* createNew(UsageEnvironment& env,
			      MediaSubsession& subsession, // identifies the kind of data that's being received
			      char const* streamId = NULL); // identifies the stream itself (optional)

  void setAVCallback(AVCallback pAVFun);
private:
  MIH264Sink(UsageEnvironment& env, MediaSubsession& subsession, char const* streamId);
    // called only by "createNew()"
  virtual ~MIH264Sink();

  static void afterGettingFrame(void* clientData, unsigned frameSize,
                                unsigned numTruncatedBytes,
				struct timeval presentationTime,
                                unsigned durationInMicroseconds);
  void afterGettingFrame(unsigned frameSize, unsigned numTruncatedBytes,
			 struct timeval presentationTime, unsigned durationInMicroseconds);
private:
  // redefined virtual functions:
  virtual Boolean continuePlaying();

private:
  u_int8_t* fReceiveBuffer;
  MediaSubsession& fSubsession;
  char* fStreamId;
  bool bHaveWrittenFirstFrame;
  AVCallback pAVCallback;
  //

  spspps_t m_spspps;
};

//bool IFIFrame(u_int8_t* pIn);
#endif
