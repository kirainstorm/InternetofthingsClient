//
//  IOSYuvCallback.h
//  IOSDevInterface
//
//  Created by xjm
//  Copyright © 2017 warningsys. All rights reserved.
//

#ifndef IOSYuvCallback_h_kuasehjrfhasui1213khkj
#define IOSYuvCallback_h_kuasehjrfhasui1213khkj

#include <stdio.h>
#include "PlatformDefine.h"
#import "IOSGLViewController.h"
#include "XPlayer.h"

class CIOSYuvCallback:public DecodeVideoCallBack
{
public:
    CIOSYuvCallback(IOSGLViewController *view);
    virtual ~CIOSYuvCallback();
public:
    virtual int OnDecodeVideoCallBack(const char* pDecData, int nLen, int nWidth, int nHeight, int nTime, int nTypeFrame = 0, int nOutFormat = 0, int nFrameRate = 0);
private:
    IOSGLViewController *m_pView;
};


#endif /* IOSYuvCallback_h */
