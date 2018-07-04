//
//  PlatformMainControllerCellEx.m
//  PlatformMonitor
//
//  Created by rs on 2017/9/15.
//  Copyright © 2017年 warningsys. All rights reserved.
//

#import "PlatformMainControllerCellEx.h"

@implementation PlatformMainControllerCellEx

@synthesize imgView;
@synthesize imgViewlLive;
@synthesize text;
@synthesize text_a;
@synthesize text_b;


- (void)awakeFromNib {
    [super awakeFromNib];
    // Initialization code
}
- (void)layoutSubviews{
    
    [super layoutSubviews];//一定不要忘了调用父类的方法，否则cell的布局会产生很奇怪的问题
    
}

-(void)setBounds:(CGRect)bounds{
    [super setBounds:bounds];
    self.contentView.frame=bounds;
}

- (void)setSelected:(BOOL)selected
{
    [super setSelected:selected];
    
    //    if (selected) {
    //        self.contentView.backgroundColor = [UIColor grayColor];
    //    }
    //    else
    //    {
    //        self.contentView.backgroundColor = [UIColor whiteColor];
    //    }
    
    
}
-(BOOL) canPerformAction:(SEL)action withSender:(id)sender
{
    //return NO;
    
    //return (action == @selector(modifyClick:)|| action == @selector(deleteClick:));
    return (action == @selector(modifyClick:)|| action == @selector(deleteClick:)|| action == @selector(picClick:) || action == @selector(recordClick:));//||action == @selector(logClick:));
} /// this methods will be called for the cell menu items

-(void) modifyClick:(id)sender
{
    //NSLog(@"modifyClick cell: %ld %ld ",(long)_indexPath.section,(long)_indexPath.row);
    [_delegate CellSendValue:_indexPath msgtype:0]; // 通知执行协议方法
}
-(void) deleteClick:(id)sender
{
    [_delegate CellSendValue:_indexPath msgtype:1]; // 通知执行协议方法
}
-(void) picClick:(id)sender
{
    [_delegate CellSendValue:_indexPath msgtype:2]; // 通知执行协议方法
}
-(void) recordClick:(id)sender
{
    [_delegate CellSendValue:_indexPath msgtype:3]; // 通知执行协议方法
}
-(void) logClick:(id)sender
{
    [_delegate CellSendValue:_indexPath msgtype:4]; // 通知执行协议方法
}

@end
