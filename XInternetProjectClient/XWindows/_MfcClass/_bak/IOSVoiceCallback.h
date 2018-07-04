//
//  IOSVoiceCallback.h
//  IOSDevInterface
//
//  Created by xjm on 16/5/25.
//  Copyright © 2016年 warningsys. All rights reserved.
//

#ifndef IOSVoiceCallback_h
#define IOSVoiceCallback_h
#include <stdio.h>
#include "PlatformDefine.h"

#include "XPlayer.h"
#include "mediainterfacedefine.h"


#import <Foundation/Foundation.h>

#import <AudioToolbox/AudioToolbox.h>
#import <AudioToolbox/AudioFile.h>
#include <AudioToolbox/AudioQueue.h>
#include <stdio.h>
#include <errno.h>
#include <sys/stat.h>

#define NUM_BUFFERS 5

struct st_ios_voice
{
    int len;
    char buffer[3*1024 - sizeof(int)];
};


class CIOSVoiceCallback:public CXMediaStreamData
{
public:
    CIOSVoiceCallback();
    virtual ~CIOSVoiceCallback();
    
public:
    virtual int OnStreamData(ST_SERVER_HEAD head, const char* pStreamData, int nLen);
    
public:
    void OpenSound();
    void CloseSound();
private:
    void * m_pDecVoice;
    
    
    
private:
    static void PlaybackAudioBuffer_Callback(void *inUserData, AudioQueueRef inAQ, AudioQueueBufferRef outBufferRef);
    int readPacketsIntoBuffer(AudioQueueBufferRef buffer);
    
private:
    AudioStreamBasicDescription m_dataFormat;
    AudioQueueRef m_queue;
    AudioStreamPacketDescription *m_packetDescs;
    AudioQueueBufferRef m_buffers[NUM_BUFFERS];
    
    
    list<st_ios_voice *> m_listVoice;
    CrossCriticalSection m_cs;
};



#endif /* IOSVoiceCallback_h */
