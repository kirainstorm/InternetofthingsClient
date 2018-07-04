//
//  PlatformMainControllerCellEx.h
//  PlatformMonitor
//
//  Created by rs on 2017/9/15.
//  Copyright © 2017年 warningsys. All rights reserved.
//

#import <UIKit/UIKit.h>


//------------------------------------------------
// 新建一个协议，协议的名字一般是由“类名+Delegate”
@protocol PersonalMainControllerCellDelegate <NSObject>

// 代理传值方法
- (void)CellSendValue:(NSIndexPath *)val  msgtype:(NSInteger)type;

@end
//------------------------------------------------


@interface PlatformMainControllerCellEx : UICollectionViewCell
    {
        NSInteger a;
    }
    @property(nonatomic,strong) NSIndexPath *indexPath;
    
@property(nonatomic,retain) IBOutlet UIImageView *imgView;//要使用相对布局，并添加约束
@property(nonatomic,retain) IBOutlet UIImageView *imgViewlLive;
    //@property(nonatomic,retain) PersonalMainController *peron;
    
@property(nonatomic,retain) IBOutlet UILabel * text;
    @property(nonatomic,strong) UILabel * text_a;
    @property(nonatomic,strong) UILabel * text_b;
    //@property(nonatomic,strong) UILabel * text_c;
    
    // 委托代理人，代理一般需使用弱引用(weak)
    @property (weak, nonatomic) id<PersonalMainControllerCellDelegate> delegate;

@end
