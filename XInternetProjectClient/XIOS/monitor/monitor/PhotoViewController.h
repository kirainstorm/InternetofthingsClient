//
//  PhotoViewController.h
//  monitor
//
//  Created by xjm on 16/5/24.
//  Copyright © 2016年 warningsys. All rights reserved.
//

#import <UIKit/UIKit.h>

@interface PhotoViewController : UIViewController
@property(nonatomic,retain) IBOutlet UICollectionView * colView;


@property(nonatomic,assign) long rowid;
@property(nonatomic,strong)NSMutableArray *photoArray;

//@property (nonatomic, retain) IBOutlet UIImageView *imageView;

-(IBAction)goback:(id)sender;
@end
