//
//  LogonViewController.h
//  monitor
//
//  Created by xjm
//  Copyright © 2016 warningsys. All rights reserved.
//

#import <UIKit/UIKit.h>



//
//拖动 storyboard entry point 指向不同的view作为初始页面
//可以灵活的启用／关闭登录页面


@interface LogonViewController : UIViewController


@property(nonatomic,strong) IBOutlet UITextField * uiTextFieldName;
@property(nonatomic,strong) IBOutlet UITextField * uiTextFieldPwd;
@property(nonatomic,strong) IBOutlet UITextField * uiTextFieldSverIP;

-(IBAction)logon:(id)sender;


@end
