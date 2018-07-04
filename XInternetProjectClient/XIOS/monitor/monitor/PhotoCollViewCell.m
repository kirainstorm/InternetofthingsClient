//
//  PhotoCollViewCell.m
//  monitor
//
//  Created by xjm on 16/5/24.
//  Copyright © 2016年 warningsys. All rights reserved.
//

#import "PhotoCollViewCell.h"

@implementation PhotoCollViewCell

@synthesize imageView;


-(id)initWithFrame:(CGRect)frame
{
    self = [super initWithFrame:frame];
    if (self) {
        //
        self.backgroundColor = [UIColor whiteColor];
        //
        CGFloat w = CGRectGetWidth(self.frame);
        //
        self.imageView = [[UIImageView alloc]initWithFrame:CGRectMake(1, 1, w - 2,CGRectGetHeight(self.frame)- 2)];
        self.imageView.backgroundColor = [UIColor whiteColor];
        [self addSubview:self.imageView];
                self.contentView.backgroundColor = [UIColor whiteColor];
    }
    
    //_indexPath = [NSIndexPath indexPathForRow:0 inSection:0];
    
    return self;
}

@end
