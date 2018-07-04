//
//  H5AdWebViewController.m
//  PlatformMonitor
//
//  Created by rs on 2017/9/14.
//  Copyright © 2017年 warningsys. All rights reserved.
//

#import "H5AdWebViewController.h"
#import <WebKit/WebKit.h>
#import "MBProgressHUD.h"
#import "Header.h"
@interface H5AdWebViewController ()<WKNavigationDelegate>
@property (strong , nonatomic) WKWebView *webView;

@end

@implementation H5AdWebViewController



//@synthesize webView;
@synthesize url;

- (void)viewDidLoad {
    [super viewDidLoad];
    // Do any additional setup after loading the view from its nib.
    WKWebViewConfiguration *configuration = [[WKWebViewConfiguration alloc] init];
    configuration.userContentController = [WKUserContentController new];
    WKPreferences *preferences = [WKPreferences new];
    preferences.javaScriptCanOpenWindowsAutomatically = YES;
    preferences.minimumFontSize = 30.0;
    configuration.preferences = preferences;
    
    NSURL *wwurl = [NSURL URLWithString:url];
    NSURLRequest *request = [NSURLRequest requestWithURL:wwurl];
    //[self.webView loadRequest:request];
    
    
    self.webView = [[WKWebView alloc]initWithFrame:CGRectMake(0, 64, KscreenWidth, KscreenHight - 64) configuration:configuration];
    self.webView.navigationDelegate = self;
    [self.view addSubview:self.webView];
    [self.webView loadRequest:request];
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
