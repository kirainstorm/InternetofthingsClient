//
//  H5AdWebViewController.h
//  PlatformMonitor
//
//  Created by rs on 2017/9/14.
//  Copyright © 2017年 warningsys. All rights reserved.
//

#import <UIKit/UIKit.h>

@interface H5AdWebViewController : UIViewController

//@property(nonatomic,retain) IBOutlet UIWebView * webView;
@property(nonatomic,strong)NSString* url;


-(IBAction)goback:(id)sender;
@end
