//
//  NameCollectionViewCell.m
//  monitor
//
//  Created by xjm on 16/2/23.
//  Copyright © 2016年 warningsys. All rights reserved.
//

#import "NameCollectionViewCell.h"

@implementation NameCollectionViewCell

- (void)awakeFromNib {
    // Initialization code
}


- (void)setSelected:(BOOL)selected
{
    [super setSelected:selected];
    
    if (selected) {
        //        [UIView animateWithDuration:1.0 delay:0 options:UIViewAnimationOptionAllowUserInteraction animations:^{
        //            self.contentView.backgroundColor = [UIColor redColor];
        //        } completion:^(BOOL finished) {
        //            self.contentView.backgroundColor = [UIColor greenColor];
        //        }];
        self.contentView.backgroundColor = [UIColor redColor];
    }
    else
    {
        self.contentView.backgroundColor = [UIColor whiteColor];
    }
    
    
}


@end
