//
//  PlayViewController.h
//  monitor
//
//  Created by xjm on 16/1/18.
//  Copyright © 2016年 warningsys. All rights reserved.
//

#import <UIKit/UIKit.h>
#import <GLKit/GLKit.h>

//实时流播放
//在该控制器中写入抓拍的功能，抓拍的照片根据不同的探头存入不同的相册，可以把dev_id作为唯一标识
@interface PlatformPlayViewController : UIViewController

//
@property(nonatomic,assign) long rowid;//=========
//
@property(nonatomic,retain) IBOutlet UIScrollView *scrollViewxib;
@property(nonatomic,retain) IBOutlet GLKView *MainPlayView;
@property (weak, nonatomic) IBOutlet UIToolbar *toolbar;
//
@property(weak, nonatomic) IBOutlet UIButton *toolbar_cap;
@property(weak, nonatomic) IBOutlet UIButton *toolbar_rec;
@property(weak, nonatomic) IBOutlet UIButton *toolbar_sound;
@property(weak, nonatomic) IBOutlet UIButton *toolbar_talk;


-(IBAction)goback:(id)sender;
-(IBAction)capbtnclick:(UIButton *)btn;
-(IBAction)soundbtnclick:(UIButton *)btn;
-(IBAction)recbtnclick:(UIButton *)btn;




@end
