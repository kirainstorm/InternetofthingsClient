请注意：拖动文件夹到工程中，，
选中
Destination Copy items into destination group's folder (if needed)
Folders Create groups for any added folders




MJRefresh
https://github.com/CoderMJLee/MJRefresh

Manual import： 

Import the main file：#import "MJRefresh.h"

self.tableView.mj_header = [MJRefreshNormalHeader headerWithRefreshingTarget:self refreshingAction:@selector(refresh)];
self.tableView.mj_footer = [MJRefreshAutoNormalFooter footerWithRefreshingTarget:self refreshingAction:@selector(loadMore)];


    MJRefreshNormalHeader *header = [MJRefreshNormalHeader headerWithRefreshingTarget:self refreshingAction:@selector(refresh)];
    header.lastUpdatedTimeLabel.hidden = YES;
    self.colView.mj_header =header;


[self.tableView.mj_header endRefreshing];
[self.tableView.mj_footer endRefreshing];




MBProgressHUD
https://github.com/matej/MBProgressHUD

    MBProgressHUD * hud = [MBProgressHUD showHUDAddedTo:self.view animated:YES];
    hud.backgroundColor = [UIColor colorWithWhite:0.f alpha:.2f];//遮盖层颜色
    //
    dispatch_after(dispatch_time(DISPATCH_TIME_NOW, 0.001 * NSEC_PER_SEC), dispatch_get_main_queue(), ^(void){
        //
        [NSThread sleepForTimeInterval:0.7];//随便加的
        //执行任务
        if (TRUE == [(AppDelegate *)([UIApplication sharedApplication].delegate) RefreshDevices])
        {
            //
            UIStoryboard *story = [UIStoryboard storyboardWithName:@"Main" bundle:[NSBundle mainBundle]];
            PlatformMainTabBarController *view = [story instantiateViewControllerWithIdentifier:@"maintabbar"];
            [self presentViewController:view animated:YES completion:nil];
        }
        //关闭
        [MBProgressHUD hideHUDForView:self.view animated:YES];
    });





XLPhotoBrowser
https://github.com/Shannoon/XLPhotoBrowser
基本用法
        [photoArray removeAllObjects];
        NSString * docDir = [((AppDelegate *)[UIApplication sharedApplication].delegate) GetMyappDocumentsDirectory];
        [self FindFiles:docDir];

        NSMutableArray *URLArray = [NSMutableArray array];
        for (NSString *urlString in self.photoArray) {
            NSURL *URL = [NSURL URLWithString:urlString relativeToURL:[NSURL fileURLWithPath: urlString]];//relativeToURL:
            [URLArray addObject:URL];
        }
        [XLPhotoBrowser showPhotoBrowserWithImages:URLArray currentImageIndex:0];

