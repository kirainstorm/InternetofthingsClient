//
//  H5PreviewViewController.h
//  PlatformMonitor
//
//  Created by rs on 2017/9/14.
//  Copyright © 2017年 warningsys. All rights reserved.
//

#import <UIKit/UIKit.h>
#import <WebKit/WebKit.h>

@interface H5PreviewViewController : UIViewController


//@property(nonatomic,retain) IBOutlet UIWebView * webView;
@property(nonatomic,strong)NSString* H5PREVIEWID;

-(IBAction)goback:(id)sender;
@end
