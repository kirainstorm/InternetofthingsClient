//
//  IOSDevInterface.h
//  IOSDevInterface
//
//  Created by xjm on 16/1/14.
//  Copyright © 2016年 warningsys. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "IOSGLViewController.h"
#include "mediainterfacedefine.h"
#include "IOSYuvCallback.h"
#include "IOSVoiceCallback.h"

@interface IOSDevInterface : NSObject
{
    IOSGLViewController * viewContrller;
    CXMediaDeviceInterface * devInterface;
    CIOSYuvCallback *pCallback;
    CIOSVoiceCallback *pVoiceCallback;
}

-(BOOL)GetUserDeviceList:(char *)center_ip center_port:(int)center_port user:(char *)user pwd:(char *)pwd devices:(NSMutableArray *)devices;


-(void)CreateDevice: (GLKView *) glview;
-(void)DestroyDevice;

-(void)OpenRealtime:(char *)svr_ip svr_port:(int)svr_port user:(char *)user pwd:(char *)pwd  dev_id:(int)dev_id;
-(void)OpenHisplay:(char*)szFilePathName;
-(void)Close;

-(void)CapturePictures:(char*)szFilePath szFileName:(char*)szFileName;

-(void)OpenSound:(int)bOpen;

-(void)OpenRecord:(BOOL)bOpen szFilePath:(char*)szFilePath szDevID:(char*)szDevID;

//-(void)SearchDevice;


@end
