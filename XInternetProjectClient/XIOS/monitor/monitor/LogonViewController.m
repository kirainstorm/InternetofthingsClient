//
//  LogonViewController.m
//  monitor
//
//  Created by xjm on 16/2/17.
//  Copyright © 2016年 warningsys. All rights reserved.
//

#import "LogonViewController.h"
#import <CoreData/CoreData.h>
#import "Reachability.h"
#import "mytoast.h"
#import "AppDelegate.h"
#import "PlatformMainTabBarController.h"
#import "MBProgressHUD.h"


@interface LogonViewController ()

@end

@implementation LogonViewController
@synthesize uiTextFieldPwd;
@synthesize uiTextFieldName;
@synthesize uiTextFieldSverIP;

- (void)viewDidLoad {
    [super viewDidLoad];
    // Do any additional setup after loading the view.
    [self readFromFileSeverip];
    [self readFromFileUser];
    [self readFromFilePwd];
}

- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
    if ([self isViewLoaded] && self.view.window == nil) {
        self.view = nil;
    }
}
- (void)touchesBegan:(NSSet<UITouch *> *)touches withEvent:(UIEvent *)event {
    [self.view endEditing:YES];
}
/*
#pragma mark - Navigation

// In a storyboard-based application, you will often want to do a little preparation before navigation
- (void)prepareForSegue:(UIStoryboardSegue *)segue sender:(id)sender {
    // Get the new view controller using [segue destinationViewController].
    // Pass the selected object to the new view controller.
}
*/

-(void) writeToFileSeverip
{
    NSArray *paths = NSSearchPathForDirectoriesInDomains(NSCachesDirectory, NSUserDomainMask, YES);
    NSString *cachesDir = [paths objectAtIndex:0];
    NSString *regionListFile = [cachesDir stringByAppendingPathComponent:@"svrip.txt"];

    //
    NSFileManager *fileManager = [NSFileManager defaultManager];
    BOOL isDelete=[fileManager removeItemAtPath:regionListFile error:nil];
    
    BOOL res=[uiTextFieldSverIP.text writeToFile:regionListFile atomically:YES encoding:NSUTF8StringEncoding error:nil];
    //NSString *content=[NSString stringWithContentsOfFile:regionListFile encoding:NSUTF8StringEncoding error:nil];
}
-(void) writeToFileUser
{
    NSArray *paths = NSSearchPathForDirectoriesInDomains(NSCachesDirectory, NSUserDomainMask, YES);
    NSString *cachesDir = [paths objectAtIndex:0];
    NSString *regionListFile = [cachesDir stringByAppendingPathComponent:@"user.txt"];
    
    //
    NSFileManager *fileManager = [NSFileManager defaultManager];
    BOOL isDelete=[fileManager removeItemAtPath:regionListFile error:nil];
    
    BOOL res=[uiTextFieldName.text writeToFile:regionListFile atomically:YES encoding:NSUTF8StringEncoding error:nil];
    //NSString *content=[NSString stringWithContentsOfFile:regionListFile encoding:NSUTF8StringEncoding error:nil];
}
-(void) writeToFilePwd
{
    NSArray *paths = NSSearchPathForDirectoriesInDomains(NSCachesDirectory, NSUserDomainMask, YES);
    NSString *cachesDir = [paths objectAtIndex:0];
    NSString *regionListFile = [cachesDir stringByAppendingPathComponent:@"pwd.txt"];
    
    //
    NSFileManager *fileManager = [NSFileManager defaultManager];
    BOOL isDelete=[fileManager removeItemAtPath:regionListFile error:nil];
    
    BOOL res=[uiTextFieldPwd.text writeToFile:regionListFile atomically:YES encoding:NSUTF8StringEncoding error:nil];
    //NSString *content=[NSString stringWithContentsOfFile:regionListFile encoding:NSUTF8StringEncoding error:nil];
}
-(void) readFromFileSeverip
{
    NSArray *paths = NSSearchPathForDirectoriesInDomains(NSCachesDirectory, NSUserDomainMask, YES);
    NSString *cachesDir = [paths objectAtIndex:0];
    NSString *regionListFile = [cachesDir stringByAppendingPathComponent:@"svrip.txt"];
    
    //
    uiTextFieldSverIP.text=[NSString stringWithContentsOfFile:regionListFile encoding:NSUTF8StringEncoding error:nil];
}
-(void) readFromFileUser
{
    NSArray *paths = NSSearchPathForDirectoriesInDomains(NSCachesDirectory, NSUserDomainMask, YES);
    NSString *cachesDir = [paths objectAtIndex:0];
    NSString *regionListFile = [cachesDir stringByAppendingPathComponent:@"user.txt"];
    
    //
    uiTextFieldName.text=[NSString stringWithContentsOfFile:regionListFile encoding:NSUTF8StringEncoding error:nil];
}
-(void) readFromFilePwd
{
    NSArray *paths = NSSearchPathForDirectoriesInDomains(NSCachesDirectory, NSUserDomainMask, YES);
    NSString *cachesDir = [paths objectAtIndex:0];
    NSString *regionListFile = [cachesDir stringByAppendingPathComponent:@"pwd.txt"];
    
    //
    uiTextFieldPwd.text=[NSString stringWithContentsOfFile:regionListFile encoding:NSUTF8StringEncoding error:nil];
}

//MBProgressHUD *HUD;
-(IBAction)logon:(id)sender
{
    [self.view endEditing:YES];
    
    [self writeToFileSeverip];
    
    //save to app
    [(AppDelegate *)([UIApplication sharedApplication].delegate) SetUserInfo:uiTextFieldName.text pwd:uiTextFieldPwd.text svr:uiTextFieldSverIP.text];
    
    
    //
    MBProgressHUD * hud = [MBProgressHUD showHUDAddedTo:self.view animated:YES];
    hud.backgroundColor = [UIColor colorWithWhite:0.f alpha:.2f];//遮盖层颜色
    //
    dispatch_after(dispatch_time(DISPATCH_TIME_NOW, 0.001 * NSEC_PER_SEC), dispatch_get_main_queue(), ^(void){
        //
        [NSThread sleepForTimeInterval:0.7];//随便加的
        //执行任务
        if (TRUE == [(AppDelegate *)([UIApplication sharedApplication].delegate) RefreshDevices])
        {
            [self writeToFileUser];
            [self writeToFilePwd];
            //
            UIStoryboard *story = [UIStoryboard storyboardWithName:@"Main" bundle:[NSBundle mainBundle]];
            PlatformMainTabBarController *view = [story instantiateViewControllerWithIdentifier:@"maintabbar"];
            [self presentViewController:view animated:YES completion:nil];
            //self.view = nil;
        }
        //关闭
        [MBProgressHUD hideHUDForView:self.view animated:YES];
    });

}


@end
