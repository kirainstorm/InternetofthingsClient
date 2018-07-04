//
//  SwitchTableViewCell.h
//  monitor
//
//  Created by xjm on 16/2/19.
//  Copyright © 2016年 warningsys. All rights reserved.
//

#import <UIKit/UIKit.h>

@interface SwitchTableViewCell : UITableViewCell
{
    UIImageView *imgView;
    UILabel * text;
    UISwitch * btnSwitch;
}

@property(nonatomic,strong) IBOutlet UIImageView *imgView;
@property(nonatomic,strong) IBOutlet UILabel * text;
@property(nonatomic,strong) IBOutlet UISwitch * btnSwitch;


//-(IBAction)actionSwitch:(id)sender;

@end
