//
//  H5PreviewViewController.m
//  PlatformMonitor
//
//  Created by rs on 2017/9/14.
//  Copyright © 2017年 warningsys. All rights reserved.
//

#import "H5PreviewViewController.h"
#import <WebKit/WebKit.h>
#import "MBProgressHUD.h"
#import "Header.h"

@interface H5PreviewViewController ()<WKNavigationDelegate>
@property (strong , nonatomic) WKWebView *webView;
@end

@implementation H5PreviewViewController

//@synthesize webView;
@synthesize H5PREVIEWID;


- (void)viewDidLoad {
    [super viewDidLoad];
    // Do any additional setup after loading the view from its nib.
    
    WKWebViewConfiguration *configuration = [[WKWebViewConfiguration alloc] init];
    configuration.userContentController = [WKUserContentController new];
    WKPreferences *preferences = [WKPreferences new];
    preferences.javaScriptCanOpenWindowsAutomatically = YES;
    preferences.minimumFontSize = 30.0;
    configuration.preferences = preferences;
    //configuration.allowsInlineMediaPlayback = YES;
    
    
    //
    NSString *mainBundlePath = [[NSBundle mainBundle] bundlePath];
    
    //html base path
    NSString * basePath = [NSString stringWithFormat:@"%@/html5", mainBundlePath];
    NSURL *baseURL = [NSURL fileURLWithPath:basePath isDirectory:YES];
    
    //index.html path
    NSString *htmlpath = [NSString stringWithFormat:@"%@/preview.html?id=%@", basePath,H5PREVIEWID];
    //NSString *htmlpath = [NSString stringWithFormat:@"%@/preview.html", basePath];
    NSString *htmlstring = [NSString stringWithContentsOfFile:htmlpath encoding:NSUTF8StringEncoding error:nil];
    
    //
    //[webView loadHTMLString:htmlstring baseURL:baseURL];
    
//    NSURL *wwurl = [NSURL URLWithString:@"http://www.baidu.com"];
//    NSURLRequest *request = [NSURLRequest requestWithURL:wwurl];
//    [self.webView loadRequest:request];
    
    NSURLRequest *request = [[NSURLRequest alloc] initWithURL:[NSURL URLWithString:[NSString stringWithFormat:@"file://%@",[htmlpath stringByAddingPercentEscapesUsingEncoding:NSUTF8StringEncoding]]]];
    
    
    //NSURLRequest *request = [[NSURLRequest alloc] initWithURL:[NSURL URLWithString:[NSString stringWithFormat:@"file: //%@",[fileHtmlstringByAddingPercentEscapesUsingEncoding:NSUTF8StringEncoding]]]];
    
    //webView.navigationDelegate = self;
    //[self.webView loadRequest:request];
    //[request rel];
    
    self.webView = [[WKWebView alloc]initWithFrame:CGRectMake(0, 64, KscreenWidth, KscreenHight - 64) configuration:configuration];
    self.webView.navigationDelegate = self;
    [self.view addSubview:self.webView];
    [self.webView loadRequest:request];//[self.webView loadHTMLString:htmlstring baseURL:baseURL];

    
    
}

- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

/*
#pragma mark - Navigation

// In a storyboard-based application, you will often want to do a little preparation before navigation
- (void)prepareForSegue:(UIStoryboardSegue *)segue sender:(id)sender {
    // Get the new view controller using [segue destinationViewController].
    // Pass the selected object to the new view controller.
}
*/

-(void)webView:(WKWebView *)webView didStartProvisionalNavigation:(WKNavigation *)navigation
{
    MBProgressHUD * hud = [MBProgressHUD showHUDAddedTo:self.webView animated:YES];
    hud.backgroundColor = [UIColor colorWithWhite:0.f alpha:.2f];//遮盖层颜色
}

-(void)webView:(WKWebView *)webView didFinishNavigation:(WKNavigation *)navigation
{
    [MBProgressHUD hideHUDForView:self.webView animated:YES];
}

-(void)webView:(WKWebView *)webView didFailNavigation:(WKNavigation *)navigation withError:(NSError *)error
{
    [MBProgressHUD hideHUDForView:self.webView animated:YES];
}
-(IBAction)goback:(id)sender
{
    
    //AppDelegate* appdelegate = (AppDelegate *)[UIApplication sharedApplication].delegate;
    //appdelegate.allowRoation = 0;
    [self dismissViewControllerAnimated:YES completion:nil];
}
@end

