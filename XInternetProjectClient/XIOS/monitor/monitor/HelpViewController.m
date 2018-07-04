//
//  HelpViewController.m
//  monitor
//
//  Created by xjm on 16/4/15.
//  Copyright © 2016年 warningsys. All rights reserved.
//

#import "HelpViewController.h"

@interface HelpViewController ()

@end

@implementation HelpViewController

- (void)viewDidLoad {
    [super viewDidLoad];
     UIWebView *webview=[[UIWebView alloc]initWithFrame:[UIScreen mainScreen].bounds];
    NSURL *fileURL = [[NSBundle mainBundle] URLForResource:@"helpus.html" withExtension:nil];
    
    NSURLRequest *request = [NSURLRequest requestWithURL:fileURL];
    
    [webview loadRequest:request];
    [self.view addSubview:webview];
    // Do any additional setup after loading the view.
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

@end
