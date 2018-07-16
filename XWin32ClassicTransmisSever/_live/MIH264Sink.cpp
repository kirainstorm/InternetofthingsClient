#include "stdafx.h"

#include "MIH264Sink.h"

//judge I frame
bool IFIFrame(u_int8_t* pIn)
{
	bool bRet = false;
	unsigned char type = (pIn[0] & 0x1f);
	switch (type)
	{
	case 2:
	case 3:
	case 4:
	case 5:
	case 7:
	case 8:
		bRet = true;
		break;

	default:
		bRet = false;
		break;
	}

	return bRet;
}

#define MIH264_SINK_RECEIVE_BUFFER_SIZE (1*1024*1024)//300000

MIH264Sink* MIH264Sink::createNew(UsageEnvironment& env, MediaSubsession& subsession, char const* streamId)
{
	return new MIH264Sink(env, subsession, streamId);
}

MIH264Sink::MIH264Sink(UsageEnvironment& env, MediaSubsession& subsession, char const* streamId)
	: MediaSink(env),
	fSubsession(subsession),
	bHaveWrittenFirstFrame(false),
	pAVCallback(NULL)
{
	//Log("MIH264Sink::MIH264Sink  1....... \n");
	fStreamId = strDup(streamId);
	fReceiveBuffer = new u_int8_t[MIH264_SINK_RECEIVE_BUFFER_SIZE];
	fReceiveBuffer[0] = 0x00;
	fReceiveBuffer[1] = 0x00;
	fReceiveBuffer[2] = 0x00;
	fReceiveBuffer[3] = 0x01;

	memset(&m_spspps, 0, sizeof(m_spspps));//must
	//Log("MIH264Sink::MIH264Sink  2....... \n");
}

MIH264Sink::~MIH264Sink()
{
	delete[] fReceiveBuffer;
	delete[] fStreamId;
}

void MIH264Sink::setAVCallback(AVCallback pAVFun)
{
	pAVCallback = pAVFun;
}

void MIH264Sink::afterGettingFrame(void* clientData, unsigned frameSize, unsigned numTruncatedBytes,
struct timeval presentationTime, unsigned durationInMicroseconds)
{
	//Log("MIH264Sink::afterGettingFrame  ....... \n");

	MIH264Sink* sink = (MIH264Sink*)clientData;
	sink->afterGettingFrame(frameSize, numTruncatedBytes, presentationTime, durationInMicroseconds);
}

// If you don't want to see debugging output for each received frame, then comment out the following line:
#define DEBUG_PRINT_EACH_RECEIVED_FRAME 1

unsigned char nalu_buffer[MIH264_SINK_RECEIVE_BUFFER_SIZE];
 unsigned char* p_nalu_tail = nalu_buffer;









void MIH264Sink::afterGettingFrame(unsigned frameSize, unsigned numTruncatedBytes,
struct timeval presentationTime, unsigned /*durationInMicroseconds*/)
{
	// We've just received a frame of data.  (Optionally) print out information about it:
	/*
  #ifdef DEBUG_PRINT_EACH_RECEIVED_FRAME
  if (fStreamId != NULL) envir() << "Stream \"" << fStreamId << "\"; ";
  envir() << fSubsession.mediumName() << "/" << fSubsession.codecName() << ":\tReceived " << frameSize << " bytes";
  if (numTruncatedBytes > 0) envir() << " (with " << numTruncatedBytes << " bytes truncated)";
  char uSecsStr[6+1]; // used to output the 'microseconds' part of the presentation time
  sprintf(uSecsStr, "%06u", (unsigned)presentationTime.tv_usec);
  envir() << ".\tPresentation time: " << (int)presentationTime.tv_sec << "." << uSecsStr;
  if (fSubsession.rtpSource() != NULL && !fSubsession.rtpSource()->hasBeenSynchronizedUsingRTCP()) {
  envir() << "!"; // mark the debugging output to indicate that this presentation time is not RTCP-synchronized
  }
  #ifdef DEBUG_PRINT_NPT
  envir() << "\tNPT: " << fSubsession.getNormalPlayTime(presentationTime);
  #endif
  envir() << "\n";
  #endif
  */


	do 
	{
		if (0 == strncmp(fSubsession.codecName(), "H264", 16))
		{
			unsigned char type = (fReceiveBuffer[4] & 0x1f);

			char  sztmp[512] = { 0 };
			CROSS_SPRINTF(sztmp, "fReceiveBuffer[4] = %d     len = %d %d %d \n", type, frameSize, presentationTime.tv_sec, presentationTime.tv_usec); 
			//::OutputDebugStringA(sztmp);


			//���豸A 
			//if (fReceiveBuffer[4] == 0x27) sps
			//if (fReceiveBuffer[4] == 0x28) pps
			//if (fReceiveBuffer[4] == 0x25) i
			//if (fReceiveBuffer[4] == 0x21) p
			//����
			//if (fReceiveBuffer[4] == 0x67) sps
			//if (fReceiveBuffer[4] == 0x68) pps
			//if (fReceiveBuffer[4] == 0x06) sei
			//if (fReceiveBuffer[4] == 0x65) i
			//if (fReceiveBuffer[4] == 0x61) p

			/*
			֡�����У�
			NAL_SLICE = 1 �ǹؼ�֡
			NAL_SLICE_DPA = 2
			NAL_SLICE_DPB = 3
			NAL_SLICE_DPC =4
			NAL_SLICE_IDR =5 �ؼ�֡
			NAL_SEI = 6
			NAL_SPS = 7 SPS֡
			NAL_PPS = 8 PPS֡
			NAL_AUD = 9
			NAL_FILLER = 12
			*/

			switch (type)
			{
			case 2://NAL_SLICE_DPA
			case 3://NAL_SLICE_DPB
			case 4://NAL_SLICE_DPC
			case 6://NAL_SEI
			case 7://NAL_SPS
			case 8://NAL_PPS
			case 9://NAL_AUD
			case 12://NAL_FILLER
				{
					memcpy(m_spspps.spspps_buffer + m_spspps.spspps_len, fReceiveBuffer, frameSize + 4);
					m_spspps.spspps_len += (frameSize + 4);
				}
				break;
			case 5://NAL_SLICE_IDR
				{
					memcpy(nalu_buffer, m_spspps.spspps_buffer, m_spspps.spspps_len);
					memcpy(nalu_buffer + m_spspps.spspps_len, fReceiveBuffer, frameSize + 4);
					if (pAVCallback != NULL)
					{
						pAVCallback(nalu_buffer, m_spspps.spspps_len + 4 + frameSize, VIDEO_FRAME, 1, presentationTime);
					}
					memset(&m_spspps, 0, sizeof(m_spspps));//must
				}
				break;
			case 1://NAL_SLICE
				{
					if (pAVCallback != NULL)
					{
						pAVCallback(fReceiveBuffer, 4 + frameSize, VIDEO_FRAME, 0, presentationTime);
					}
				}
				break;
			default:
				break;
			}
		}


		if (0 == strcmp(fSubsession.mediumName(), "audio"))
		{

			if (pAVCallback != NULL)
			{
				pAVCallback(fReceiveBuffer+4, frameSize, AUDIO_FRAME, 0, presentationTime);
			}
		}

	} while (0);

	continuePlaying();
	return;

}

Boolean MIH264Sink::continuePlaying()
{
	//Log("MIH264Sink::continuePlaying  .......1 \n");
	if (fSource == NULL) return False; // sanity check (should not happen)

//Log("MIH264Sink::continuePlaying  .......2 \n");

	// Request the next frame of data from our input source.  "afterGettingFrame()" will get called later, when it arrives:
	fSource->getNextFrame(fReceiveBuffer+4, MIH264_SINK_RECEIVE_BUFFER_SIZE-4,
		afterGettingFrame, this,
		onSourceClosure, this);
	return True;
}

