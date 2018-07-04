//
//  PlatformLiveViewController.m
//  PlatformMonitor
//
//  Created by rs on 2017/5/17.
//  Copyright © 2017年 warningsys. All rights reserved.
//
#import <WebKit/WebKit.h>
#import "PlatformLiveViewController.h"
#import "WebViewJavascriptBridge.h"
#import "WKWebViewJavascriptBridge.h"
#import "H5PreviewViewController.h"
#import "H5AdWebViewController.h"
#import "Header.h"
#import "MBProgressHUD.h"

@interface PlatformLiveViewController ()<WKNavigationDelegate>
//@property WebViewJavascriptBridge* bridge;
@property WKWebViewJavascriptBridge* bridge;
@property (strong , nonatomic) WKWebView *webView;
@end

@implementation PlatformLiveViewController


@synthesize webView;


- (void)viewDidLoad {
    [super viewDidLoad];
    // Do any additional setup after loading the view.
    WKWebViewConfiguration *configuration = [[WKWebViewConfiguration alloc] init];
    configuration.userContentController = [WKUserContentController new];
    WKPreferences *preferences = [WKPreferences new];
    preferences.javaScriptCanOpenWindowsAutomatically = YES;
    preferences.minimumFontSize = 30.0;
    configuration.preferences = preferences;
    //self.webView = [[WKWebView alloc] initWithFrame:self.view.frame configuration:configuration];
    
    //
    NSString *mainBundlePath = [[NSBundle mainBundle] bundlePath];
    
    //html base path
    NSString * basePath = [NSString stringWithFormat:@"%@/html5", mainBundlePath];
    NSURL *baseURL = [NSURL fileURLWithPath:basePath isDirectory:YES];
    
    //index.html path
    NSString *htmlpath = [NSString stringWithFormat:@"%@/index.html", basePath];
    NSString *htmlstring = [NSString stringWithContentsOfFile:htmlpath encoding:NSUTF8StringEncoding error:nil];
    
    //
    //webView.delegate = self;
    //webView.scrollView.decelerationRate = UIScrollViewDecelerationRateNormal;
    //[webView loadHTMLString:htmlstring baseURL:baseURL];
    
    
    
    self.webView = [[WKWebView alloc]initWithFrame:CGRectMake(0, 64, KscreenWidth, KscreenHight - 64 - 40) configuration:configuration];
    //self.webView.navigationDelegate = self;
    [self.webView loadHTMLString:htmlstring baseURL:baseURL];
    [self.view addSubview:self.webView];
    
    
    //self.webView.UIDelegate = self;
    
    
    //www.cnblogs.com/markstray/p/5757244.html
    //self.bridge = [WebViewJavascriptBridge bridgeForWebView:webView];
    self.bridge = [WKWebViewJavascriptBridge bridgeForWebView:self.webView];
    [self.bridge setWebViewDelegate:self];
    
    [self.bridge registerHandler:@"showWebPreview" handler:^(id url, WVJBResponseCallback responseCallback) {
        NSString *info = [NSString stringWithFormat:@"%@",url];
        //[self showMsg:info];
        
        
        H5PreviewViewController * h5view = ([[H5PreviewViewController alloc] initWithNibName:@"H5PreviewViewController" bundle:nil]);
        h5view.H5PREVIEWID=info;
        [self presentViewController:h5view animated:YES completion:nil ];
        
    }];
    [self.bridge registerHandler:@"showWebAD" handler:^(id url, WVJBResponseCallback responseCallback) {
        NSString *info = [NSString stringWithFormat:@"%@",url];
        //[self showMsg:info];
        H5AdWebViewController * h5view = ([[H5AdWebViewController alloc] initWithNibName:@"H5AdWebViewController" bundle:nil]);
        h5view.url=info;
        [self presentViewController:h5view animated:YES completion:nil ];
    }];


}

- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
    if ([self isViewLoaded] && self.view.window == nil) {
        self.view = nil;
    }
}

-(void) viewDidAppear:(BOOL)animated
{
}

-(void)webView:(WKWebView *)webView didStartProvisionalNavigation:(WKNavigation *)navigation
{
    MBProgressHUD * hud = [MBProgressHUD showHUDAddedTo:self.view animated:YES];
    hud.backgroundColor = [UIColor colorWithWhite:0.f alpha:.2f];//遮盖层颜色
}

-(void)webView:(WKWebView *)webView didFinishNavigation:(WKNavigation *)navigation
{
    [MBProgressHUD hideHUDForView:self.view animated:YES];
}

-(void)webView:(WKWebView *)webView didFailNavigation:(WKNavigation *)navigation withError:(NSError *)error
{
    [MBProgressHUD hideHUDForView:self.view animated:YES];
}

/*
#pragma mark - Navigation

// In a storyboard-based application, you will often want to do a little preparation before navigation
- (void)prepareForSegue:(UIStoryboardSegue *)segue sender:(id)sender {
    // Get the new view controller using [segue destinationViewController].
    // Pass the selected object to the new view controller.
}
*/

@end
