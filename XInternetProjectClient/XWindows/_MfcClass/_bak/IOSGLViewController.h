//
//  MyGLViewController.h
//  monitor
//
//  Created by xjm on 16/1/12.
//  Copyright © 2016年 warningsys. All rights reserved.
//

#import <UIKit/UIKit.h>
#import <GLKit/GLKit.h>


@interface IOSGLViewController : GLKViewController{
    int m_nWidth;
    int m_nHeight;
    
    Byte* m_pYUVData;
    NSCondition *m_YUVDataLock;
    
    GLuint _testTxture[3];
    
    //BOOL m_bNeedSleep;
}

-(void)initGLView: (GLKView *) glview;
-(void)deinitGLView;

- (void) WriteYUVFrame: (Byte*) pYUV Len: (int) length width: (int)width height: (int) height;

@end
