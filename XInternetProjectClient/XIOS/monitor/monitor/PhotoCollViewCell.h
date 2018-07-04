//
//  PhotoCollViewCell.h
//  monitor
//
//  Created by xjm on 16/5/24.
//  Copyright © 2016年 warningsys. All rights reserved.
//

#import <UIKit/UIKit.h>

@interface PhotoCollViewCell : UICollectionViewCell
@property (nonatomic, retain) IBOutlet UIImageView *imageView;
@property(nonatomic,strong) NSIndexPath *indexPath;

@end
