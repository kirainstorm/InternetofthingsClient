//
//  PlatformRecordTableViewController.h
//  PlatformMonitor
//
//  Created by rs on 2017/5/23.
//  Copyright © 2017年 warningsys. All rights reserved.
//

#import <UIKit/UIKit.h>

@interface PlatformRecordViewController : UIViewController<UITableViewDataSource,UITableViewDelegate>


@property(nonatomic,retain) IBOutlet UITableView * tabView;


@property(nonatomic,assign) long rowid;
@property(nonatomic,strong)NSMutableArray *recordArray;

-(IBAction)goback:(id)sender;
@end
