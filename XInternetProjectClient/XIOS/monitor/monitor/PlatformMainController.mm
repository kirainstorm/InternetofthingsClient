//
//  FirstViewController.m
//  monitor
//
//  Created by xjm on 16/1/18.
//  Copyright © 2016年 warningsys. All rights reserved.
//

#import "PlatformMainController.h"
#import "PlatformMainControllerCellEx.h"
#import "PlatformPlayViewController.h"

//相册的控制器
//#import "PhotoViewController.h"
#import "AppDelegate.h"
#import "IOSPlatformDevice.h"
#import "PhotoViewController.h"
#import "XLPhotoBrowser.h"
#import "CNPPopupController.h"
#import "PlatformRecordViewController.h"

//添加设备详情的控制器
//#import "AddDeviceController.h"

//#import "DeviceViewController.h"
#import "Header.h"

//
#import "MJRefresh.h"



@interface PlatformMainController () <PersonalMainControllerCellDelegate,UICollectionViewDataSource,UICollectionViewDelegate,CNPPopupControllerDelegate>

@property (nonatomic, strong) CNPPopupController *popupController;
@end

@implementation PlatformMainController

@synthesize photoArray;
@synthesize colView;

static NSString *cellIdentifier = @"PlatformMainControllerCell";
static CGFloat cellSpace = 1;
NSIndexPath *indexPath4Main;



-(void)viewWillAppear:(BOOL)animated{
    [self loadDataFromLocal];
    [self.colView reloadData];
    [self viewWillLayoutSubviews];
}

- (void)viewDidLoad {
    [super viewDidLoad];
    
    // Do any additional setup after loading the view, typically from a nib.
    
    photoArray =[NSMutableArray array];

    //menu
    UIMenuItem *modify_ = [[UIMenuItem alloc] initWithTitle:NSLocalizedString(@"g_modify", nil) action:@selector(modifyClick:)];
    UIMenuItem *delete_ = [[UIMenuItem alloc] initWithTitle:NSLocalizedString(@"g_delete", nil) action:@selector(deleteClick:)];
    UIMenuItem *pic_ = [[UIMenuItem alloc] initWithTitle:NSLocalizedString(@"g_pic", nil) action:@selector(picClick:)];
    UIMenuItem *record_ = [[UIMenuItem alloc] initWithTitle:NSLocalizedString(@"g_record", nil) action:@selector(recordClick:)];
    UIMenuItem *log_ = [[UIMenuItem alloc] initWithTitle:NSLocalizedString(@"g_log", nil) action:@selector(logClick:)];
    UIMenuController *menu = [UIMenuController sharedMenuController];
    [menu setMenuItems:[NSArray arrayWithObjects:modify_,delete_,pic_,record_,log_, nil]];
    //[menu setTargetRect:cell.c_post_content.frame inView:cell.c_post_content.superview];
    [menu setTargetRect:self.colView.bounds inView:self.colView];
    [menu setMenuVisible:YES animated:YES];
    [[UIMenuController sharedMenuController] update];
    //UICollectionView的尺寸
    //UICollectionViewFlowLayout *flayout=[[UICollectionViewFlowLayout alloc]init];
    //self.colView=[[UICollectionView alloc]initWithFrame:CGRectMake(0,0, [UIScreen mainScreen].bounds.size.width, [UIScreen mainScreen].bounds.size.height) collectionViewLayout:flayout];
    //self.navigationController.navigationBar.translucent=NO;
    self.colView.backgroundColor=[UIColor whiteColor];
    //[self.view addSubview:self.colView];
    //delegate,dataSource
    self.colView.delegate = self;
    self.colView.dataSource = self;
    //[self.colView registerClass:[PlatformMainControllerCellEx class] forCellWithReuseIdentifier:cellIdentifier];
    [self.colView registerNib:[UINib nibWithNibName:@"PlatformMainControllerCellEx" bundle:nil] forCellWithReuseIdentifier:cellIdentifier];
    [self loadDataFromLocal];
    
    
    
    


    //默认【下拉刷新】
    MJRefreshNormalHeader *header = [MJRefreshNormalHeader headerWithRefreshingTarget:self refreshingAction:@selector(refresh)];
    header.lastUpdatedTimeLabel.hidden = YES;
    self.colView.mj_header =header;
    //默认【上拉加载】
    //self.colView.mj_footer = [MJRefreshAutoNormalFooter footerWithRefreshingTarget:self refreshingAction:@selector(loadMore)];
    
    
    //创建长按手势监听
    UILongPressGestureRecognizer *longPress = [[UILongPressGestureRecognizer alloc]
                                               initWithTarget:self
                                               action:@selector(myHandleTableviewCellLongPressed:)];
    longPress.minimumPressDuration = 0.5;
    [self.colView addGestureRecognizer:longPress];//将长按手势添加到需要实现长按操作的视图里
}
-(void)refresh
{
    [(AppDelegate *)([UIApplication sharedApplication].delegate) RefreshDevices];
    [self.colView.mj_header endRefreshing];
    [self.colView reloadData];
    //[self.colView performBatchUpdates:^{
    //} completion:^(BOOL finished) {
    //}];
}
-(void)loadMore
{
    //[self getNetworkData:NO];
}


- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
    if ([self isViewLoaded] && self.view.window == nil) {
        self.view = nil;
    }
}
//解析数据
-(void)loadDataFromLocal{
   // [self.colView reloadData];
}

#pragma mark - UICollectionViewDataSource
//每一组有多少个cell
-(NSInteger)collectionView:(UICollectionView *)collectionView numberOfItemsInSection:(NSInteger)section
{
    printf("------ %d",((AppDelegate *)[UIApplication sharedApplication].delegate).DeviceArray.count);
    
    return ((AppDelegate *)[UIApplication sharedApplication].delegate).DeviceArray.count;//返回每组的cell的总数，因为使用流式布局，每一行可以通过cell大小自动计算显示几个item
}
//collectionView里有多少个组
-(NSInteger)numberOfSectionsInCollectionView:(UICollectionView *)collectionView
{
    return 1;//这里一直返回1，只有一个组
}
//定义并返回每个cell
-(UICollectionViewCell *)collectionView:(UICollectionView *)collectionView cellForItemAtIndexPath:(NSIndexPath *)indexPath
{
    
    
    PlatformMainControllerCellEx *cell = [collectionView dequeueReusableCellWithReuseIdentifier:cellIdentifier forIndexPath:indexPath];
    //[cell sizeToFit];
    if (!cell) {
        NSLog(@"wu fa chuangjian cell deviceviewcontroller");
    }
    


    
    
    IOSPlatformDevice *_dev=((AppDelegate *)[UIApplication sharedApplication].delegate).DeviceArray[indexPath.row];
    
    
    
    
    NSString * docDir = [((AppDelegate *)[UIApplication sharedApplication].delegate) GetMyappDocumentsDirectory];
    NSString *filepathname = [NSString stringWithFormat:@"%@/pic/%@/logo/logo.jpg", docDir,_dev->dev_id];
    NSFileManager * fileManger = [NSFileManager defaultManager];

    if (![fileManger fileExistsAtPath:filepathname]) {
        cell.imgView.image = [UIImage imageNamed:@"maincell"];
    }else{
        NSData *imgdata = [NSData dataWithContentsOfFile:filepathname];
        cell.imgView.image = [UIImage imageWithData:imgdata];;
    }
    
    cell.indexPath =indexPath;
    NSString *str=[NSString stringWithFormat: @"    %@",_dev->dev_name];
    cell.text.text=str;
    NSString *str1=[NSString stringWithFormat:@"      设备ID: %@",_dev->dev_id];
    cell.text_a.text=str1;
    
    if ( [_dev->dev_online intValue] > 0 )
    {
        [cell.text_b setTextColor:[UIColor greenColor]];
        cell.text_b.text=@"      在线";
        cell.imgViewlLive.image = [UIImage imageNamed:@"zz_online"];
    }
    else
    {
        [cell.text_b setTextColor:[UIColor redColor]];
        cell.text_b.text=@"      不在线";
        cell.imgViewlLive.image = [UIImage imageNamed:@"zz_offline"];
    }
    


    

    [cell setDelegate:self];//-----here-----------important
    
    return cell;
}
//定义并返回每个headerView或footerView
-(UICollectionReusableView *)collectionView:(UICollectionView *)collectionView viewForSupplementaryElementOfKind:(NSString *)kind atIndexPath:(NSIndexPath *)indexPath
{
    if (kind == UICollectionElementKindSectionHeader) {
        
        
    }
    
    if (kind == UICollectionElementKindSectionFooter) {
        
        
    }
    
    return NULL;
    
    //    UICollectionReusableView *headview = [collectionView dequeueReusableSupplementaryViewOfKind:UICollectionElementKindSectionHeader withReuseIdentifier:@"ReusableView" forIndexPath:indexPath];
    //
    //    [headview addSubview: ]
    //    return headview;
}
#pragma mark - UICollectionViewDelegate


-(void)collectionView:(UICollectionView *)collectionView didSelectItemAtIndexPath:(nonnull NSIndexPath *)indexPath
{
    NSLog(@"device colletion view : %ld %ld ",(long)indexPath.section,(long)indexPath.row);
//    在跳转前把标识传递过去
    
    //UICollectionViewCell * cell = (UICollectionViewCell *)[collectionView cellForItemAtIndexPath:indexPath];
    
    
    //cell.backgroundColor = [UIColor whiteColor];
    
    dispatch_async(dispatch_get_main_queue(), ^{
        PlatformPlayViewController * playview = ([[PlatformPlayViewController alloc] initWithNibName:@"PlatformPlayViewController" bundle:nil]);
        playview.rowid=indexPath.row;
        playview.view.backgroundColor=[UIColor whiteColor];
        [playview setModalTransitionStyle:UIModalTransitionStyleCrossDissolve];
        [self presentViewController:playview animated:YES completion:nil ];//glkview need:presentViewController
        
    });
}
//can be select
-(BOOL) collectionView:(UICollectionView *)collectionView shouldSelectItemAtIndexPath:(nonnull NSIndexPath *)indexPath
{
    
    return YES;
}
//#pragma mark - UICollectionViewDelegateFlowLayout
//- (CGSize)collectionView:(UICollectionView *)collectionView layout:(UICollectionViewLayout *)collectionViewLayout sizeForItemAtIndexPath:(NSIndexPath *)indexPath
//{
//    if(indexPath.section==0 && indexPath.row==1)
//    {
//        return CGSizeMake(50, 50);
//    }
//    else
//    {
//        return CGSizeMake(75, 30);
//    }
//}

- (CGSize)collectionView:(UICollectionView *)collectionView layout:(nonnull UICollectionViewLayout *)collectionViewLayout sizeForItemAtIndexPath:(nonnull NSIndexPath *)indexPath
{
    CGRect rx =  [[UIScreen mainScreen] bounds];
    CGFloat w = rx.size.width - 4;// - cellSpace - cellSpace ;//8==cellSpace
    CGFloat h = 190;
//    //
//    CGFloat w = rx.size.width - cellSpace - cellSpace ;//8==cellSpace
//    CGFloat h = 144 + 64;
//    
    return CGSizeMake(w, h);
}




//-(UIEdgeInsets) collectionView:(UICollectionView *)collectionView layout:(UICollectionViewLayout *)collectionViewLayout insetForSectionAtIndex:(NSInteger)section
//{
//    //return UIEdgeInsetsMake(0, 0, 0, 0);
//    return UIEdgeInsetsMake(cellSpace, cellSpace, cellSpace, cellSpace);
//}
-(CGFloat)collectionView:(UICollectionView *)collectionView layout:(UICollectionViewLayout *)collectionViewLayout minimumInteritemSpacingForSectionAtIndex:(NSInteger)section
{
    return 1;
}

//每一个cell的大小
////设置每组的cell的边界, 具体看下图
//- (UIEdgeInsets)collectionView:(UICollectionView *)collectionView layout:(UICollectionViewLayout*)collectionViewLayout insetForSectionAtIndex:(NSInteger)section;
//
//cell的最小行间距
- (CGFloat)collectionView:(UICollectionView *)collectionView layout:(UICollectionViewLayout*)collectionViewLayout minimumLineSpacingForSectionAtIndex:(NSInteger)section
{
    return 1;
}
//
////cell的最小列间距
//- (CGFloat)collectionView:(UICollectionView *)collectionView layout:(UICollectionViewLayout*)collectionViewLayout minimumInteritemSpacingForSectionAtIndex:(NSInteger)section
//{
//    return 3;
//}

//允许Menu菜单
//- (BOOL)collectionView:(UICollectionView *)collectionView shouldShowMenuForItemAtIndexPath:(NSIndexPath *)indexPath
//{
//    //NSLog(@"shouldShowMenuForItemAtIndexPath : %ld %ld ",(long)indexPath.section,(long)indexPath.row);
//    //selectItemAtIndexPath
//    [self.colView selectItemAtIndexPath:indexPath animated:YES scrollPosition:UICollectionViewScrollPositionCenteredHorizontally];
//    
//    //除了用菜单外，还可以用弹出框
//    //[self showPopupWithStyle:CNPPopupStyleCentered];
//    return YES;
//}
//- (BOOL)collectionView:(UICollectionView *)collectionView canPerformAction:(SEL)action forItemAtIndexPath:(NSIndexPath *)indexPath withSender:(id)sender
//{
//    //NSLog(@"canPerformAction : %ld %ld ",(long)indexPath.section,(long)indexPath.row);
//    //in cell return yes
//    return  NO;
//}
//
//- (void)collectionView:(UICollectionView *)collectionView performAction:(SEL)action forItemAtIndexPath:(NSIndexPath *)indexPath withSender:(id)sender
//{
//    //NSLog(@"performAction : %ld %ld ",(long)indexPath.section,(long)indexPath.row);
//}
-(void) modifyClick:(id)sender
{
}
-(void) deleteClick:(id)sender
{
}
-(void) picClick:(id)sender
{

}
-(void) recordClick:(id)sender

{


}
-(void) logClick:(id)sender
{
}


#pragma mark    -   XLPhotoBrowserDelegate

//点击不同的menu跳转到不痛的详情界面
- (void)CellSendValue:(NSIndexPath *)val msgtype:(NSInteger)type;
{
     //区分不同的按钮（type）来实现不同的方法
    if (type == 0) {
////        点击了修改的按钮
//        //跳转到添加的详情的界面
//        //跳转前将行的参数传给下一个界面
//        DeviceViewController *addvc=[[DeviceViewController alloc]init];
//        addvc.view.backgroundColor=[UIColor whiteColor];
//        addvc.row=val.row;
//        UINavigationController *nagvigationVC  = [[UINavigationController alloc]initWithRootViewController:addvc];
//        //nagvigationVC.modalTransitionStyle = 2;
//        [self presentViewController:nagvigationVC animated:YES completion:^{
//            //推出视图之后要做一些操作,可以写在这里;
//        }];
    }
      else if (type == 1) {
//           //从数据源中取出数据
//           Device *logg = ((AppDelegate *)[UIApplication sharedApplication].delegate).DeviceArray[val.row];
//           [((AppDelegate *)[UIApplication sharedApplication].delegate).dbContext deleteObject:logg];
//           
//           NSError *error = nil;
//           [((AppDelegate *)[UIApplication sharedApplication].delegate).dbContext save:&error];
//           if (!error) {
//               NSLog(@"保存成功");
//               [((AppDelegate *)[UIApplication sharedApplication].delegate).DeviceArray removeObject:logg];
//               [self.colView reloadData];
//           }
//        NSLog(@"点击了删除的按钮");
    }else if (type==2) {
//        NSLog(@";;;;");
//    //模态推出新的视图
//        PhotoViewController *addVC = [[PhotoViewController alloc]init];
//        NSString *tit=[NSString stringWithFormat:@"第%ld个摄像头的相册",(long)val.row+1];
//        addVC.title=tit;
//        addVC.view.backgroundColor=[UIColor whiteColor];
//    //给模态出来的视图控制添加导航控制器
//        UINavigationController *nagvigationVC  = [[UINavigationController alloc]initWithRootViewController:addVC];
//        
//    //配置模态出视图的样式
//        nagvigationVC.modalTransitionStyle = 2;
//    //模态不依赖于导航控制器,所以不需要找到导航控制器
//        [self presentViewController:nagvigationVC animated:YES completion:^{
//            //推出视图之后要做一些操作,可以写在这里;
//        }];
        
        

    
        
        
        
        PhotoViewController * photoview = ([[PhotoViewController alloc] initWithNibName:@"PhotoViewController" bundle:nil]);
        photoview.rowid=val.row;
        photoview.view.backgroundColor=[UIColor whiteColor];
        [self presentViewController:photoview animated:YES completion:nil ];
        
        
    }else if (type == 3){
//        //在这个按钮下 界面跳转到录像的播放器播放的界面
//        PersonalHisplayViewController *hisview=[[PersonalHisplayViewController alloc]init];
//        
//        hisview.rowid=val.row;
//        hisview.view.backgroundColor=[UIColor whiteColor];
//        [self presentViewController:hisview animated:YES completion:nil ];//glkview need:presentViewController

        
        
//        videovc.view.backgroundColor=[UIColor whiteColor];
//        videovc.title=@"录像";
//       // videovc.navigationController.navigationBarHidden=YES;
//        UINavigationController *nagvigationVC  = [[UINavigationController alloc]initWithRootViewController:videovc];
//        //配置模态出视图的样式
//        nagvigationVC.modalTransitionStyle = 2;
//        //模态不依赖于导航控制器,所以不需要找到导航控制器
//        [self presentViewController:nagvigationVC animated:YES completion:^{
//            //推出视图之后要做一些操作,可以写在这里;
//        }];
        

    }else if (type == 4){
        NSLog(@"点击了日志的按钮");
    }
    
    
    
    
    
//    UIAlertController * alertCtr = [UIAlertController alertControllerWithTitle:@"" message:@"删除联系人？" preferredStyle:UIAlertControllerStyleAlert];
//    UIAlertAction *firstAction = [UIAlertAction actionWithTitle:@"确定" style:UIAlertActionStyleDefault handler:^(UIAlertAction *action) {
//        //do something
//        
//    }];
//    UIAlertAction *secondAction = [UIAlertAction actionWithTitle:@"取消" style:UIAlertActionStyleCancel handler:^(UIAlertAction *action) {
//        //do something
//        
//    }];
//    [alertCtr addAction:firstAction];
//    [alertCtr addAction:secondAction];
//    [self presentViewController:alertCtr animated:YES completion:^{
//    }];
}


#pragma mark - CNPPopupController Delegate

- (void)popupController:(CNPPopupController *)controller didDismissWithButtonTitle:(NSString *)title {
    NSLog(@"Dismissed with button title: %@", title);
}

- (void)popupControllerDidPresent:(CNPPopupController *)controller {
    NSLog(@"Popup controller presented.");
}


- (IBAction)onAddDevices:(UIBarButtonItem *)sender {
//    DeviceViewController *addvc=[[DeviceViewController alloc]init];
//    addvc.row = -1;
//    UINavigationController *nagvigationVC  = [[UINavigationController alloc]initWithRootViewController:addvc];
//    //nagvigationVC.modalTransitionStyle = 2;
//    [self presentViewController:nagvigationVC animated:YES completion:^{
//        //推出视图之后要做一些操作,可以写在这里;
//    }];

}


- (void)FindFiles:(NSString *)path
{
    // 1.判断文件还是目录
    NSFileManager * fileManger = [NSFileManager defaultManager];
    BOOL isDir = NO;
    BOOL isExist = [fileManger fileExistsAtPath:path isDirectory:&isDir];
    if (isExist) {
        // 2. 判断是不是目录
        if (isDir) {
            NSArray * dirArray = [fileManger contentsOfDirectoryAtPath:path error:nil];
            NSString * subPath = nil;
            for (NSString * str in dirArray) {
                subPath = [path stringByAppendingPathComponent:str];
                BOOL issubDir = NO;
                [fileManger fileExistsAtPath:subPath isDirectory:&issubDir];
                [self FindFiles:subPath];
            }
            
            
        }else{
            NSLog(@"%@",path);
            
            [photoArray addObject:path];
        }
    }else{
        NSLog(@"你打印的是目录或者不存在");
    }
}

//-(IBAction)onLogin:(id)sender
//{
//
//}


- (void) myHandleTableviewCellLongPressed:(UILongPressGestureRecognizer *)gestureRecognizer {
    
    
    CGPoint pointTouch = [gestureRecognizer locationInView:self.colView];
    
    if (gestureRecognizer.state == UIGestureRecognizerStateBegan) {
        NSLog(@"UIGestureRecognizerStateBegan");
        
        indexPath4Main = [self.colView indexPathForItemAtPoint:pointTouch];
        if (indexPath4Main == nil) {
            NSLog(@"空");
        }else{
            
            NSLog(@"Section = %ld,Row = %ld",(long)indexPath4Main.section,(long)indexPath4Main.row);
            [self showPopupWithStyle4Main:CNPPopupStyleActionSheet];
        }
    }
    if (gestureRecognizer.state == UIGestureRecognizerStateChanged) {
        NSLog(@"UIGestureRecognizerStateChanged");
    }
    
    if (gestureRecognizer.state == UIGestureRecognizerStateEnded) {
        NSLog(@"UIGestureRecognizerStateEnded");
    }
}

#pragma mark - CNPPopupController Delegate

- (void)showPopupWithStyle4Main:(CNPPopupStyle)popupStyle {
    
    
    
    //NSMutableParagraphStyle *paragraphStyle = NSMutableParagraphStyle.defaultParagraphStyle;
    //paragraphStyle.lineBreakMode = NSLineBreakByWordWrapping;
    //paragraphStyle.alignment = NSTextAlignmentCenter;
    
    
    
    //NSAttributedString *title = [[NSAttributedString alloc] initWithString:@"It's A Popup!" attributes:@{NSFontAttributeName : [UIFont boldSystemFontOfSize:24], NSParagraphStyleAttributeName : paragraphStyle}];
    
    //NSAttributedString *lineOne = [[NSAttributedString alloc] initWithString:@"You can add text and images" attributes:@{NSFontAttributeName : [UIFont systemFontOfSize:18], NSParagraphStyleAttributeName : paragraphStyle}];
    
    //NSAttributedString *lineTwo = [[NSAttributedString alloc] initWithString:@"With style, using NSAttributedString" attributes:@{NSFontAttributeName : [UIFont systemFontOfSize:18], NSForegroundColorAttributeName : [UIColor colorWithRed:0.46 green:0.8 blue:1.0 alpha:1.0], NSParagraphStyleAttributeName : paragraphStyle}];
    
    
    
    CNPPopupButton *button_cap = [[CNPPopupButton alloc] initWithFrame:CGRectMake(0, 0, [UIScreen mainScreen].bounds.size.width - 40, 48)];
    [button_cap setTitleColor:[UIColor blackColor] forState:UIControlStateNormal];
    button_cap.titleLabel.font = [UIFont boldSystemFontOfSize:18];
    [button_cap setTitle:@"图片浏览" forState:UIControlStateNormal];
    //button.backgroundColor = [UIColor colorWithRed:0.46 green:0.8 blue:1.0 alpha:1.0];
    button_cap.layer.cornerRadius = 4;
    button_cap.selectionHandler = ^(CNPPopupButton *button){
        
        [self.popupController dismissPopupControllerAnimated:YES];
        
        PhotoViewController * photoview = ([[PhotoViewController alloc] initWithNibName:@"PhotoViewController" bundle:nil]);
        photoview.rowid=indexPath4Main.row;
        photoview.view.backgroundColor=[UIColor whiteColor];
        [self presentViewController:photoview animated:YES completion:nil ];
        
        NSLog(@"Block for button: %@", button.titleLabel.text);
        
    };
    
    CNPPopupButton *button_rec = [[CNPPopupButton alloc] initWithFrame:CGRectMake(0, 0, [UIScreen mainScreen].bounds.size.width - 40, 48)];
    [button_rec setTitleColor:[UIColor blackColor] forState:UIControlStateNormal];
    button_rec.titleLabel.font = [UIFont boldSystemFontOfSize:18];
    [button_rec setTitle:@"本地录像浏览" forState:UIControlStateNormal];
    //button.backgroundColor = [UIColor colorWithRed:0.46 green:0.8 blue:1.0 alpha:1.0];
    button_rec.layer.cornerRadius = 4;
    button_rec.selectionHandler = ^(CNPPopupButton *button){
        
        [self.popupController dismissPopupControllerAnimated:YES];
        
        PlatformRecordViewController * recordview = ([[PlatformRecordViewController alloc] initWithNibName:@"PlatformRecordViewController" bundle:nil]);
        recordview.rowid=indexPath4Main.row;
        recordview.view.backgroundColor=[UIColor whiteColor];
        [self presentViewController:recordview animated:YES completion:nil ];
        
        NSLog(@"Block for button: %@", button.titleLabel.text);
        
    };
    
    
    
    
    self.popupController = [[CNPPopupController alloc] initWithContents:@[button_cap,button_rec]];
    
    self.popupController.theme = [CNPPopupTheme defaultTheme];
    self.popupController.theme.popupStyle = popupStyle;
    self.popupController.delegate = self;
    [self.popupController presentPopupControllerAnimated:YES];
    
}


//- (void)popupController:(CNPPopupController *)controller didDismissWithButtonTitle:(NSString *)title {
//    
//    NSLog(@"Dismissed with button title: %@", title);
//    
//}
//
//
//
//- (void)popupControllerDidPresent:(CNPPopupController *)controller {
//    
//    NSLog(@"Popup controller presented.");
//    
//}



@end
