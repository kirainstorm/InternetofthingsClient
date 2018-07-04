//
//  SecondViewController.m
//  monitor
//
//  Created by xjm on 16/1/18.
//  Copyright © 2016年 warningsys. All rights reserved.
//

#import "PersonalAboutController.h"
//#import "PersonalPlayViewController.h"
#import "SwitchTableViewCell.h"
#import "AboutusViewController.h"
#import "CNPPopupController.h"
#import "HelpViewController.h"

@interface PersonalAboutController () <UITableViewDataSource,UITableViewDelegate>

@end

@implementation PersonalAboutController

@synthesize tabView;
- (void)viewDidLoad {
    [super viewDidLoad];
    
    
    // Do any additional setup after loading the view, typically from a nib.
    
    
    
    
    
    //yun xun xuan zhong?
    self.tabView.allowsSelection = NO;
    //fen ge xian
    self.tabView.separatorStyle = UITableViewCellSeparatorStyleNone;
    //self.tabView.separatorStyle = UITableViewCellSeparatorStyleSingleLine;
    //self.tabView.separatorStyle = UITableViewCellSeparatorStyleSingleLineEtched;
    self.tabView.dataSource = self;
    self.tabView.delegate = self;
    self.tabView.scrollEnabled =NO;//不能滚动
    //[self.tabView registerNib:[UINib nibWithNibName:@"DayAndNightCell" bundle:nil] forCellReuseIdentifier:@"day"];
    self.tabView.backgroundColor = [UIColor clearColor];//背景透明
}

- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
    if ([self isViewLoaded] && self.view.window == nil) {
    self.view = nil;
    }
}

-(void)viewWillAppear:(BOOL)animated
{
    //pop返回table时，cell自动取消选中状态
    [self.tabView deselectRowAtIndexPath:[self.tabView indexPathForSelectedRow] animated:NO];
}



#pragma mark - tableview
- (NSInteger)numberOfSectionsInTableView:(UITableView *)tableView
{
    return 1;
}
- (NSString *)tableView:(UITableView *)tableView titleForHeaderInSection:(NSInteger)section
{

    return @"";
}
-(NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section
{
//    if (0 == section) {
//        return 2;
//    }
//    if (1 == section) {
//        return 2;
//    }
    return 3;
}
-(UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath
{
    CGFloat FontOfSize_ = 14.0;

    static NSString *simpleIdentifier = @"SimpleTableCell";
    
    UITableViewCell *cell = [tabView dequeueReusableCellWithIdentifier:simpleIdentifier];
    if (cell == nil) {
        cell = [[UITableViewCell alloc] initWithStyle:UITableViewCellStyleDefault reuseIdentifier:simpleIdentifier];
    }

    switch (indexPath.row) {
        case 0:
            cell.textLabel.text = NSLocalizedString(@"acomp", nil);
            //cell.imageView.image = [UIImage imageNamed:@"w3.png"];
            break;
        case 1:
            cell.textLabel.text = NSLocalizedString(@"ahelp", nil);
            //cell.imageView.image = [UIImage imageNamed:@"w3.png"];
            break;
        case 2:
            cell.textLabel.text = NSLocalizedString(@"aabout", nil);
            //cell.imageView.image = [UIImage imageNamed:@"w1.png"];
            break;
        default:
            break;
    }
    cell.accessoryType = UITableViewCellAccessoryNone;//UITableViewCellAccessoryDisclosureIndicator;
    cell.selectionStyle = UITableViewCellSelectionStyleBlue;
    [cell.textLabel setFont:[UIFont systemFontOfSize:FontOfSize_]];
    cell.backgroundColor = [UIColor clearColor];
    
    return cell;
    
    
    
    
    
    
    
    //label
    //[cell.textLabel setFont:[UIFont systemFontOfSize:14]];
    //
    //cell.accessoryType = UITableViewCellAccessoryNone;
    //cell.accessoryType = UITableViewCellAccessoryDisclosureIndicator;//xiao jian tou >
    //cell.accessoryType = UITableViewCellAccessoryDetailDisclosureButton;//lan se yuan quan + xiao jian tou >
    //cell.accessoryType = UITableViewCellAccessoryCheckmark;//dui hao
    //cell.accessoryType = UITableViewCellAccessoryDetailButton;////lan se yuan quan
    //
    //cell.selectionStyle = UITableViewCellSelectionStyleBlue;
    //

    
    return nil;
}

//yun xu shan chu
//-(BOOL)tableView:(UITableView *)tableView canEditRowAtIndexPath:(NSIndexPath *)indexPath
//{
//    return YES;
//}
//-(void)tableView:(UITableView *)tableView commitEditingStyle:(UITableViewCellEditingStyle)editingStyle forRowAtIndexPath:(NSIndexPath *)indexPath
//{
//    if (editingStyle == UITableViewCellEditingStyleDelete) {
//        //[self.tabView deleteRowsAtIndexPaths:[NSArray arrayWithObject:indexPath] withRowAnimation:UITableViewRowAnimationFade];
//    }
//}
//gao du
//夜间模式的实现
-(void)dayandnightchange:(UISwitch *)sender{
    switch ((int)sender.on) {
        case NO:
            //发出通知,让appdelegate里面执行day方法
            [[NSNotificationCenter defaultCenter]postNotificationName:@"day" object:nil userInfo:nil];
            break;
        case YES:
            //发出通知,让appdelegate里面执行night方法
            [[NSNotificationCenter defaultCenter]postNotificationName:@"night" object:nil userInfo:nil];
            break;
        default:
            break;
    }
}
-(void)wifichange:(UISwitch *)sender{
    switch ((int)sender.on) {
        case NO:
            NSLog(@"任何状态都可以播放");
            break;
        case YES:
           
             NSLog(@"仅Wi-Fi可以播放视频");
            break;
        default:
            break;
    }
}
-(CGFloat)tableView:(UITableView *)tableView heightForRowAtIndexPath:(NSIndexPath *)indexPath
{
    return 44;
}
//xuan zhong mou hang

-(void) tableView:(UITableView *)tableView didSelectRowAtIndexPath:(nonnull NSIndexPath *)indexPath
{
//    //-----------------------------------------------------------------
//    if (0 == indexPath.section) {
//        switch (indexPath.row) {
//            case 0:
//            {
//             //dispatch_async(dispatch_get_main_queue(), ^{
//             //      [self showAdboutPopupWithStyle:CNPPopupStyleFullscreen];
//             //  });
//            }
//                break;
//            default:
//                break;
//        }
//    }
//    if (1 == indexPath.section) {
//        switch (indexPath.row) {
//            case 0:
//            {
//                HelpViewController *aa=[[HelpViewController alloc]init];
//                [self.navigationController pushViewController:aa animated:YES];
//            }
//                break;
//            case 1:
//            {
//                AboutusViewController *aa=[[AboutusViewController alloc]init];
//                [self.navigationController pushViewController:aa animated:YES];
//            }
//                break;
//           
//          
//            default:
//                break;
//        }
//    }
    //-----------------------------------------------------------------
    switch (indexPath.row) {
        case 0:
        {
            HelpViewController *aa=[[HelpViewController alloc]init];
            [self.navigationController pushViewController:aa animated:YES];
            //[self presentViewController:aa animated:YES completion:nil ];
            
        }
            break;
        case 1:
        {
            AboutusViewController *aa=[[AboutusViewController alloc]init];
            [self.navigationController pushViewController:aa animated:YES];
            //[self presentViewController:aa animated:YES completion:nil ];
            
        }
            break;
            
            
        default:
            break;
    }
}
-(void)tableView:(UITableView *)tableView accessoryButtonTappedForRowWithIndexPath:(NSIndexPath *)indexPath
{

}

@end
