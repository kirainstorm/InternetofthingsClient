//
//  SwitchTableViewCell.m
//  monitor
//
//  Created by xjm on 16/2/19.
//  Copyright © 2016年 warningsys. All rights reserved.
//

#import "SwitchTableViewCell.h"

@implementation SwitchTableViewCell

@synthesize imgView;
@synthesize text;
@synthesize btnSwitch;

- (void)awakeFromNib {
    // Initialization code
}

- (void)setSelected:(BOOL)selected animated:(BOOL)animated {
    [super setSelected:selected animated:animated];

    // Configure the view for the selected state
}

//-(IBAction)actionSwitch:(id)sender
//{
//    if (self.btnSwitch.selected) {
//        [self.btnSwitch setSelected:NO];
//    }else{
//        [self.btnSwitch setSelected:YES];
//    }
//}


@end
