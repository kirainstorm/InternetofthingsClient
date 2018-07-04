//
//  LiveAddsDeytailViewController.m
//  monitor
//
//  Created by xjm on 16/2/22.
//  Copyright © 2016年 warningsys. All rights reserved.
//

#import "LiveAddsDeytailViewController.h"

@interface LiveAddsDeytailViewController ()<UIWebViewDelegate>
@property (nonatomic,retain)UIWebView *webView;

@end

@implementation LiveAddsDeytailViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    self.title = @"产品";
    self.webView = [[UIWebView alloc]initWithFrame:[UIScreen mainScreen].bounds];
    self.webView.scrollView.contentInset = UIEdgeInsetsMake(-48, 0, -200, 0);
    self.webView.scrollView.bounces = NO;
    self.webView.delegate = self;
    self.webView.scalesPageToFit = YES;
    NSURL *url=[NSURL URLWithString:@"http://weather1.sina.cn/?&vt=4"];
    [self.webView loadRequest:[NSURLRequest requestWithURL:url]];
    [self.view addSubview:self.webView];
    self.navigationController.navigationBar.tintColor = [UIColor whiteColor];
    self.navigationController.navigationBar.titleTextAttributes = @{NSForegroundColorAttributeName:[UIColor whiteColor]};
    self.navigationController.navigationBar.barTintColor = [UIColor colorWithRed:52/255.0 green:156/255.0 blue:251/255.0 alpha:1];
    
    UIBarButtonItem *leftItem = [[UIBarButtonItem alloc]initWithTitle:@"返回" style:UIBarButtonItemStylePlain target:self action:@selector(handlBack:)];
    self.navigationItem.leftBarButtonItem = leftItem;
    // Do any additional setup after loading the view.
}
- (void)handlBack:(UIBarButtonItem *)item{
    //返回上一级界面
    [self dismissViewControllerAnimated:YES completion:nil];
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
