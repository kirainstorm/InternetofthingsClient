//
//  PlayViewController.m
//  monitor
//
//  Created by xjm on 16/1/18.
//  Copyright © 2016年 warningsys. All rights reserved.
//

#import "PlatformPlayViewController.h"

#import "AppDelegate.h"
//#import "Device.h"
//#import "Pic.h"
#import "ActionSheetStringPicker.h"
#import "IOSPlatformDevice.h"
#import "IOSDevInterface.h"

@interface PlatformPlayViewController ()<UIScrollViewDelegate>
@end

@implementation PlatformPlayViewController

@synthesize rowid;

@synthesize scrollViewxib;
@synthesize MainPlayView;
@synthesize toolbar;
//
@synthesize toolbar_cap;
@synthesize toolbar_rec;
@synthesize toolbar_sound;
@synthesize toolbar_talk;

IOSDevInterface * pIOSDevInterface;
IOSPlatformDevice *_dev;


- (void)viewDidLoad {
    [super viewDidLoad];
    
    

    //------------------------------------------------------------
    _dev=((AppDelegate *)[UIApplication sharedApplication].delegate).DeviceArray[rowid];
    pIOSDevInterface = [[IOSDevInterface alloc] init];
    [pIOSDevInterface CreateDevice:MainPlayView];
    [pIOSDevInterface OpenRealtime:const_cast<char *>([_dev->dev_media_ip UTF8String]) svr_port:[_dev->dev_media_port intValue] user:const_cast<char *>([((AppDelegate *)[UIApplication sharedApplication].delegate).UserName UTF8String]) pwd:const_cast<char *>([((AppDelegate *)[UIApplication sharedApplication].delegate).UserPwd UTF8String]) dev_id:[_dev->dev_id intValue] ];
    //------------------------------------------------------------
    AppDelegate* appdelegate = (AppDelegate *)[UIApplication sharedApplication].delegate;
    appdelegate.allowRoation = 1;
    [self setNewOrientation:YES];//调用转屏代码
    //------------------------------------------------------------
    //self.title=@"播放";
    //
    //UIBarButtonItem *left = [[UIBarButtonItem alloc]initWithTitle:NSLocalizedString(@"g_cancel", nil) style:UIBarButtonItemStylePlain target:self action:@selector(goback:)];
    //self.navigationItem.leftBarButtonItem = left;
    //------------------------------------------------------------

    toolbar.hidden = NO;

    //
    [toolbar_cap setBackgroundImage:[UIImage imageNamed:@"zz_capture.png" ] forState:UIControlStateNormal];
    [toolbar_cap setBackgroundImage:[UIImage imageNamed:@"zz_capture_sel.png" ] forState:UIControlStateHighlighted];//<<<
    [toolbar_rec setBackgroundImage:[UIImage imageNamed:@"zz_record.png" ] forState:UIControlStateNormal];
    [toolbar_rec setBackgroundImage:[UIImage imageNamed:@"zz_record_sel.png" ] forState:UIControlStateSelected];
    [toolbar_sound setBackgroundImage:[UIImage imageNamed:@"zz_sound.png" ] forState:UIControlStateNormal];
    [toolbar_sound setBackgroundImage:[UIImage imageNamed:@"zz_sound_sel.png" ] forState:UIControlStateSelected];
    //[toolbar_talk setBackgroundImage:[UIImage imageNamed:@"zz_talkback.png" ] forState:UIControlStateNormal];
    //[toolbar_talk setBackgroundImage:[UIImage imageNamed:@"zz_talkback_sel.png" ] forState:UIControlStateSelected];
    
    [toolbar_cap addTarget:self action:@selector(capbtnclick:) forControlEvents:UIControlEventTouchUpInside];
    [toolbar_rec addTarget:self action:@selector(recbtnclick:) forControlEvents:UIControlEventTouchUpInside];
    [toolbar_sound addTarget:self action:@selector(soundbtnclick:) forControlEvents:UIControlEventTouchUpInside];
    //[toolbar_talk addTarget:self action:@selector(talkbtnclick:) forControlEvents:UIControlEventTouchUpInside];
    
    scrollViewxib.backgroundColor = [UIColor clearColor];
    scrollViewxib.multipleTouchEnabled = YES;
    scrollViewxib.minimumZoomScale = 1.0;
    scrollViewxib.maximumZoomScale = 3.0;
    scrollViewxib.delegate = self;
}


-(void)viewDidDisappear:(BOOL)animated
{
//    //------------------------------------
//    [pIOSDevInterface Close];
//    //------------------------------------
//    AppDelegate* appdelegate = (AppDelegate *)[UIApplication sharedApplication].delegate;
//    appdelegate.allowRoation = 0;
//    [self dismissViewControllerAnimated:YES completion:nil];
}
#pragma mark - UITextFieldDelegate

- (BOOL)textFieldShouldBeginEditing:(UITextField *)textField {
    //[textField resignFirstResponder];
    return NO;
}



- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

-(void)viewWillLayoutSubviews
{
    UIDeviceOrientation o = (UIDeviceOrientation)[UIApplication sharedApplication].statusBarOrientation;
    if (o == UIDeviceOrientationPortrait || o == UIDeviceOrientationPortraitUpsideDown) {
       
        //竖屏
    }
    else
    {
        //横屏
    }
}
- (void)setNewOrientation:(BOOL)fullscreen
{
    if (fullscreen) {
        NSNumber *resetOrientationTarget = [NSNumber numberWithInt:UIInterfaceOrientationUnknown];
        [[UIDevice currentDevice] setValue:resetOrientationTarget forKey:@"orientation"];
              
        NSNumber *orientationTarget = [NSNumber numberWithInt:UIInterfaceOrientationLandscapeLeft];
        [[UIDevice currentDevice] setValue:orientationTarget forKey:@"orientation"];
    }else{
        NSNumber *resetOrientationTarget = [NSNumber numberWithInt:UIInterfaceOrientationUnknown];
        [[UIDevice currentDevice] setValue:resetOrientationTarget forKey:@"orientation"];
        
        NSNumber *orientationTarget = [NSNumber numberWithInt:UIInterfaceOrientationPortrait];
        [[UIDevice currentDevice] setValue:orientationTarget forKey:@"orientation"];
    }
}
//-(BOOL)shouldAutorotate
//{
//    return YES;
//}
//-(UIInterfaceOrientationMask) supportedInterfaceOrientations
//{
//    return UIInterfaceOrientationMaskLandscapeLeft;
//}
//-(UIInterfaceOrientation)preferredInterfaceOrientationForPresentation
//{
//    return UIInterfaceOrientationLandscapeLeft;
//}

/*
#pragma mark - Navigation

// In a storyboard-based application, you will often want to do a little preparation before navigation
- (void)prepareForSegue:(UIStoryboardSegue *)segue sender:(id)sender {
    // Get the new view controller using [segue destinationViewController].
    // Pass the selected object to the new view controller.
}
*/
-(IBAction)goback:(id)sender
{
    NSString * docDir = [((AppDelegate *)[UIApplication sharedApplication].delegate) GetMyappDocumentsDirectory];
    NSString *filepath = [NSString stringWithFormat:@"%@/pic/%@/logo/", docDir,_dev->dev_id];
    NSString *filename = @"logo.jpg";
    [pIOSDevInterface CapturePictures:const_cast<char *>([filepath UTF8String]) szFileName:const_cast<char *>([filename UTF8String])];
    //------------------------------------
    [pIOSDevInterface OpenRecord:FALSE szFilePath:"" szDevID:""];
    [pIOSDevInterface OpenSound:FALSE];
    [pIOSDevInterface Close];
    [pIOSDevInterface DestroyDevice];
    pIOSDevInterface = nil;
    
    //------------------------------------
    AppDelegate* appdelegate = (AppDelegate *)[UIApplication sharedApplication].delegate;
    appdelegate.allowRoation = 0;
    [self dismissViewControllerAnimated:YES completion:nil];
}

#pragma mark
-(IBAction)capbtnclick:(UIButton *)btn
{
    NSString * docDir = [((AppDelegate *)[UIApplication sharedApplication].delegate) GetMyappDocumentsDirectory];
    
    //
    NSString *filepath = [NSString stringWithFormat:@"%@/pic/cap/%@/", docDir,_dev->dev_id]; //[docDir stringByAppendingString:@"/pic/"];// + @"\\pic\\";
    //
    NSDate* date = [NSDate date];
    NSDateFormatter* formatter = [[NSDateFormatter alloc] init];
    [formatter setDateFormat:@"yyyy-MM-dd_HH:mm:ss"];
    NSString* strDateTime = [formatter stringFromDate:date];
    NSString *filename = [NSString stringWithFormat:@"%@.jpg", strDateTime];

    [pIOSDevInterface CapturePictures:const_cast<char *>([filepath UTF8String]) szFileName:const_cast<char *>([filename UTF8String])];
    
    
    
//    Pic *_pic=[[Pic alloc]initWithEntity:[NSEntityDescription entityForName:@"Pic" inManagedObjectContext:((AppDelegate *)[UIApplication sharedApplication].delegate).dbContext ] insertIntoManagedObjectContext:((AppDelegate *)[UIApplication sharedApplication].delegate).dbContext];
//    _pic.picname =filename;
//    _pic.deviceuuid =_dev.deviceuuid;
//    _pic.picdate =date;
//    
//    NSError *error=nil;
//    [((AppDelegate *)[UIApplication sharedApplication].delegate).dbContext save:&error];
 
    
    
}

BOOL bRecord = FALSE;
-(IBAction)recbtnclick:(UIButton *)btn
{
//    toolbar_rec.selected = !toolbar_rec.selected;
//    first_rec.selected = !first_rec.selected;
//    
//    //
//    NSArray *paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
//    NSString *docDir = [paths objectAtIndex:0];
//    //
//    [pIOSDevInterface OpenRecord:toolbar_rec.selected szFilePath:const_cast<char *>([docDir UTF8String]) szDevUUID:const_cast<char *>([_dev.deviceuuid UTF8String])];
    
    bRecord = !bRecord;
    NSString * docDir = [((AppDelegate *)[UIApplication sharedApplication].delegate) GetMyappDocumentsDirectory];
    NSString *filepath = [NSString stringWithFormat:@"%@/rec/", docDir];
    [pIOSDevInterface OpenRecord:bRecord  szFilePath:const_cast<char *>([filepath UTF8String]) szDevID:const_cast<char *>([[NSString stringWithFormat:@"%@",_dev->dev_id] UTF8String])];
    
    
}
BOOL bSoundOpen = FALSE;
-(IBAction)soundbtnclick:(UIButton *)btn
{
    //toolbar_sound.selected = !toolbar_sound.selected;
    
    bSoundOpen = !bSoundOpen;
    [pIOSDevInterface OpenSound:bSoundOpen];
}


-(IBAction)talkbtnclick:(UIButton *)btn
{
    toolbar_talk.selected = !toolbar_talk.selected;
//    first_talk.selected = !first_talk.selected;
    
}


#pragma mark - 电子放大
//监控界面缩放
-(UIView*)viewForZoomingInScrollView:(UIScrollView *)scrollView{
    
    return self.MainPlayView;
}

-(void)scrollViewDidEndDecelerating:(UIScrollView *)scrollView{
    
    
}

//监控界面缩放
-(void)scrollViewDidZoom:(UIScrollView *)scrollView{
    //[UIScreen mainScreen].bounds;
    
    CGFloat offsetX = (scrollViewxib.bounds.size.width > scrollViewxib.contentSize.width)?(scrollViewxib.bounds.size.width - scrollViewxib.contentSize.width)/2 : 0.0;
    CGFloat offsetY = (scrollViewxib.bounds.size.height > scrollViewxib.contentSize.height)?(scrollViewxib.bounds.size.height - scrollViewxib.contentSize.height)/2 : 0.0;
    MainPlayView.center = CGPointMake(scrollViewxib.contentSize.width/2 + offsetX,scrollViewxib.contentSize.height/2 + offsetY);
}

//监控界面缩放
-(void)scrollViewDidEndZooming:(UIScrollView *)scrollView withView:(UIView *)view atScale:(CGFloat)scale{
    NSLog(@"scrollViewDidEndZooming %f", scale);
    
}



@end

