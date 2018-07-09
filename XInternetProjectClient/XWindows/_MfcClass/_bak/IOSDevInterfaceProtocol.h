#ifndef _DEV_INTERFACE_PROTOCOL_h
#define _DEV_INTERFACE_PROTOCOL_h


#include "PlatformDefine.h"


#ifdef _PLATFORM_IOS
#define STD_CALL
#endif


#import <Foundation/Foundation.h>

//////
@protocol YUVNotifyProtocol <NSObject>
-(void)YUVNotify: (char*)yuv length : (int)length width : (int)width height : (int)height timestamp : (unsigned int)timestamp;
@end


//
@protocol SearchCameraResultProtocol
- (void) SearchCameraResult:(NSString *)mac Name:(NSString *)name Addr:(NSString *)addr Port:(NSString *)port DID:(NSString*)did;
@end


#endif
