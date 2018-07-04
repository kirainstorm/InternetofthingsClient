//
//  PhotoViewController.m
//  monitor
//
//  Created by xjm on 16/5/24.
//  Copyright © 2016年 warningsys. All rights reserved.
//

#import "PhotoViewController.h"
#import "AppDelegate.h"
#import <CoreData/CoreData.h>
#import "Reachability.h"
#import "IOSPlatformDevice.h"
#import "PhotoCollViewCell.h"
#import "XLPhotoBrowser.h"
#import "CNPPopupController.h"

@interface PhotoViewController ()<UICollectionViewDataSource,UICollectionViewDelegate,XLPhotoBrowserDelegate,CNPPopupControllerDelegate>
@property (nonatomic, strong) CNPPopupController *popupController;
@end

@implementation PhotoViewController


@synthesize colView;
@synthesize rowid;
@synthesize photoArray;
//@synthesize imageView;


static NSString * const photoIdentifier = @"PhotoCollViewCell";
IOSPlatformDevice *_dev4photo;
NSMutableArray *URLArray;// = [NSMutableArray array];


XLPhotoBrowser *browser ;

- (void)viewDidLoad {
    [super viewDidLoad];
    // Do any additional setup after loading the view from its nib.
    
//    //
    _dev4photo=((AppDelegate *)[UIApplication sharedApplication].delegate).DeviceArray[rowid];
    photoArray =[NSMutableArray array];
    URLArray = [NSMutableArray array];

    self.colView.backgroundColor=[UIColor whiteColor];
    //[self.view addSubview:self.colView];
    //delegate,dataSource
    self.colView.delegate = self;
    self.colView.dataSource = self;
    [self.colView registerClass:[PhotoCollViewCell class] forCellWithReuseIdentifier:photoIdentifier];
    
    
    
    NSString * docDir = [((AppDelegate *)[UIApplication sharedApplication].delegate) GetMyappDocumentsDirectory];
    NSString * string = [docDir stringByAppendingFormat:@"/pic/cap/%@",_dev4photo->dev_id];
    [self FindFiles:string];
    [self.colView reloadData];
    
    //创建长按手势监听
    UILongPressGestureRecognizer *longPress = [[UILongPressGestureRecognizer alloc]
                                               initWithTarget:self
                                               action:@selector(myHandleTableviewCellLongPressed:)];
    longPress.minimumPressDuration = 0.5;
    [self.colView addGestureRecognizer:longPress];//将长按手势添加到需要实现长按操作的视图里
   
}

- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

/*
#pragma mark - Navigation

// In a storyboard-based application, you will often want to do a little preparation before navigation
- (void)prepareForSegue:(UIStoryboardSegue *)segue sender:(id)sender {
    // Get the new view controller using [segue destinationViewController].
    // Pass the selected object to the new view controller.
}
*/

-(IBAction)goback:(id)sender
{
    
    AppDelegate* appdelegate = (AppDelegate *)[UIApplication sharedApplication].delegate;
    appdelegate.allowRoation = 0;
    [self dismissViewControllerAnimated:YES completion:nil];
}



#pragma mark - UICollectionViewDataSource
-(NSInteger)collectionView:(UICollectionView *)collectionView numberOfItemsInSection:(NSInteger)section
{
    return photoArray.count;
}
-(NSInteger)numberOfSectionsInCollectionView:(UICollectionView *)collectionView
{
    return 1;
}
//定义并返回每个cell
-(UICollectionViewCell *)collectionView:(UICollectionView *)collectionView cellForItemAtIndexPath:(NSIndexPath *)indexPath
{
    
    PhotoCollViewCell *cell = [collectionView dequeueReusableCellWithReuseIdentifier:photoIdentifier forIndexPath:indexPath];
    [cell sizeToFit];
    if (!cell) {
        NSLog(@"wu fa chuangjian cell photoIdentifier");
    }

//    long l = indexPath.row;
//    Pic * pic =photoArray[l];
//    
//    
//    
//    NSArray *paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
//    NSString *docDir = [paths objectAtIndex:0];
//    //
//    NSString *filepathname = [NSString stringWithFormat:@"%@/pic/%@/%@", docDir,pic.deviceuuid,pic.picname];
//    
//    //bNSLog(@"%@",pic.picfullpathname);
//    NSLog(@"%@",filepathname);
    
    NSData *imgdata = [NSData dataWithContentsOfFile:photoArray[indexPath.row]];
    cell.imageView.image = [UIImage imageWithData:imgdata];
    cell.indexPath =indexPath;
    
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
    
    
    
    //[photoArray removeAllObjects];
    //NSString * docDir = [((AppDelegate *)[UIApplication sharedApplication].delegate) GetMyappDocumentsDirectory];
    //[self FindFiles:docDir];
    
    [URLArray removeAllObjects];
    for (NSString *urlString in self.photoArray) {
        NSURL *URL = [NSURL URLWithString:urlString relativeToURL:[NSURL fileURLWithPath: urlString]];//relativeToURL
        [URLArray addObject:URL];
    }
    //[XLPhotoBrowser showPhotoBrowserWithImages:URLArray currentImageIndex:0];
    
    
    
    browser = [XLPhotoBrowser showPhotoBrowserWithImages:URLArray currentImageIndex:indexPath.row];
    browser.browserStyle = XLPhotoBrowserStyleIndexLabel; // 微博样式
    
    // 设置长按手势弹出的地步ActionSheet数据,不实现此方法则没有长按手势
    [browser setActionSheetWithTitle:@"" delegate:self cancelButtonTitle:nil deleteButtonTitle:@"删除"
                   otherButtonTitles:@"保存图片",nil];
    
    
    //UICollectionViewCell * cell = (UICollectionViewCell *)[collectionView cellForItemAtIndexPath:indexPath];
    //cell.backgroundColor = [UIColor whiteColor];
    
//    dispatch_async(dispatch_get_main_queue(), ^{
//        PersonalPlayViewController * playview = ([[PersonalPlayViewController alloc] initWithNibName:@"PersonalPlayViewController" bundle:nil]);
//        playview.rowid=indexPath.row;
//        playview.view.backgroundColor=[UIColor whiteColor];
//        [self presentViewController:playview animated:YES completion:nil ];//glkview need:presentViewController
//    
//    });
}
//can be select
-(BOOL) collectionView:(UICollectionView *)collectionView shouldSelectItemAtIndexPath:(nonnull NSIndexPath *)indexPath
{
    
    return YES;
}

//允许Menu菜单
//- (BOOL)collectionView:(UICollectionView *)collectionView shouldShowMenuForItemAtIndexPath:(NSIndexPath *)indexPath
//{
//    //NSLog(@"shouldShowMenuForItemAtIndexPath : %ld %ld ",(long)indexPath.section,(long)indexPath.row);
//    //selectItemAtIndexPath
//    //[self.colView selectItemAtIndexPath:indexPath animated:YES scrollPosition:UICollectionViewScrollPositionCenteredHorizontally];
//    
//    //除了用菜单外，还可以用弹出框
//    NSMutableParagraphStyle *paragraphStyle = NSMutableParagraphStyle.new;
//    paragraphStyle.lineBreakMode = NSLineBreakByWordWrapping;
//    paragraphStyle.alignment = NSTextAlignmentCenter;
//    
//    
//    
//    NSAttributedString *title = [[NSAttributedString alloc] initWithString:@"It's A Popup!" attributes:@{NSFontAttributeName : [UIFont boldSystemFontOfSize:24], NSParagraphStyleAttributeName : paragraphStyle}];
//    
//    NSAttributedString *lineOne = [[NSAttributedString alloc] initWithString:@"You can add text and images" attributes:@{NSFontAttributeName : [UIFont systemFontOfSize:18], NSParagraphStyleAttributeName : paragraphStyle}];
//    
//    NSAttributedString *lineTwo = [[NSAttributedString alloc] initWithString:@"With style, using NSAttributedString" attributes:@{NSFontAttributeName : [UIFont systemFontOfSize:18], NSForegroundColorAttributeName : [UIColor colorWithRed:0.46 green:0.8 blue:1.0 alpha:1.0], NSParagraphStyleAttributeName : paragraphStyle}];
//    
//    
//    
//    CNPPopupButton *button = [[CNPPopupButton alloc] initWithFrame:CGRectMake(0, 0, 200, 60)];
//    
//    [button setTitleColor:[UIColor whiteColor] forState:UIControlStateNormal];
//    
//    button.titleLabel.font = [UIFont boldSystemFontOfSize:18];
//    
//    [button setTitle:@"Close Me" forState:UIControlStateNormal];
//    
//    button.backgroundColor = [UIColor colorWithRed:0.46 green:0.8 blue:1.0 alpha:1.0];
//    
//    button.layer.cornerRadius = 4;
//    
//    button.selectionHandler = ^(CNPPopupButton *button){
//        
//        [self.popupController dismissPopupControllerAnimated:YES];
//        
//        NSLog(@"Block for button: %@", button.titleLabel.text);
//        
//    };
//    
//    
//    
//    UILabel *titleLabel = [[UILabel alloc] init];
//    
//    titleLabel.numberOfLines = 0;
//    
//    titleLabel.attributedText = title;
//    
//    
//    
//    UILabel *lineOneLabel = [[UILabel alloc] init];
//    
//    lineOneLabel.numberOfLines = 0;
//    
//    lineOneLabel.attributedText = lineOne;
//    
//    
//    
//    UIImageView *imageView = [[UIImageView alloc] initWithImage:[UIImage imageNamed:@"icon"]];
//    
//    
//    
//    UILabel *lineTwoLabel = [[UILabel alloc] init];
//    
//    lineTwoLabel.numberOfLines = 0;
//    
//    lineTwoLabel.attributedText = lineTwo;
//    
//    
//    
//    
//    
//    UIView *customView = [[UIView alloc] initWithFrame:CGRectMake(0, 0, 250, 55)];
//    
//    customView.backgroundColor = [UIColor lightGrayColor];
//    
//    
//    
//    UITextField *textFied = [[UITextField alloc] initWithFrame:CGRectMake(10, 10, 230, 35)];
//    
//    textFied.borderStyle = UITextBorderStyleRoundedRect;
//    
//    textFied.placeholder = @"Custom view!";
//    
//    [customView addSubview:textFied];
//    
//    
//    
//    self.popupController = [[CNPPopupController alloc] initWithContents:@[titleLabel, lineOneLabel, imageView, lineTwoLabel, customView, button]];
//    
//    self.popupController.theme = [CNPPopupTheme defaultTheme];
//    
//    self.popupController.theme.popupStyle = CNPPopupStyleActionSheet;
//    
//    self.popupController.delegate = self;
//    
//    [self.popupController presentPopupControllerAnimated:YES];
//    
//    return YES;
//}
//- (BOOL)collectionView:(UICollectionView *)collectionView canPerformAction:(SEL)action forItemAtIndexPath:(NSIndexPath *)indexPath withSender:(id)sender
//{
//    //NSLog(@"canPerformAction : %ld %ld ",(long)indexPath.section,(long)indexPath.row);
//    //in cell return yes
//    return  YES;
//}
//
//- (void)collectionView:(UICollectionView *)collectionView performAction:(SEL)action forItemAtIndexPath:(NSIndexPath *)indexPath withSender:(id)sender
//{
//    //NSLog(@"performAction : %ld %ld ",(long)indexPath.section,(long)indexPath.row);
//}



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
    CGFloat w = rx.size.width/2 - 8;
    CGFloat h = (w*9)/16;
    return CGSizeMake(w, h);
}




-(UIEdgeInsets) collectionView:(UICollectionView *)collectionView layout:(UICollectionViewLayout *)collectionViewLayout insetForSectionAtIndex:(NSInteger)section
{
    return UIEdgeInsetsMake(2, 2, 2, 2);
    //return UIEdgeInsetsMake(cellSpace, cellSpace, cellSpace, cellSpace);
}
-(CGFloat)collectionView:(UICollectionView *)collectionView layout:(UICollectionViewLayout *)collectionViewLayout minimumInteritemSpacingForSectionAtIndex:(NSInteger)section
{
    return 1;
}


- (void) myHandleTableviewCellLongPressed:(UILongPressGestureRecognizer *)gestureRecognizer {
    
    
    CGPoint pointTouch = [gestureRecognizer locationInView:self.colView];
    
    if (gestureRecognizer.state == UIGestureRecognizerStateBegan) {
        NSLog(@"UIGestureRecognizerStateBegan");
        
        NSIndexPath *indexPath = [self.colView indexPathForItemAtPoint:pointTouch];
        if (indexPath == nil) {
            NSLog(@"空");
        }else{
            
            NSLog(@"Section = %ld,Row = %ld",(long)indexPath.section,(long)indexPath.row);
            //[self showPopupWithStyle:CNPPopupStyleActionSheet];
        }
    }
    if (gestureRecognizer.state == UIGestureRecognizerStateChanged) {
        NSLog(@"UIGestureRecognizerStateChanged");
    }
    
    if (gestureRecognizer.state == UIGestureRecognizerStateEnded) {
        NSLog(@"UIGestureRecognizerStateEnded");
    }
}
////每一个cell的大小
//- (CGSize)collectionView:(UICollectionView *)collectionView layout:(UICollectionViewLayout*)collectionViewLayout sizeForItemAtIndexPath:(NSIndexPath *)indexPath
//{
//
//}
//
////设置每组的cell的边界, 具体看下图
//- (UIEdgeInsets)collectionView:(UICollectionView *)collectionView layout:(UICollectionViewLayout*)collectionViewLayout insetForSectionAtIndex:(NSInteger)section;
//
//cell的最小行间距
- (CGFloat)collectionView:(UICollectionView *)collectionView layout:(UICollectionViewLayout*)collectionViewLayout minimumLineSpacingForSectionAtIndex:(NSInteger)section
{
    return 2;
}


-(void) deleteClick:(id)sender
{
}
- (void)PhoneCellSendValue:(NSIndexPath *)val msgtype:(NSInteger)type;
{
    NSFileManager *fileManager = [NSFileManager defaultManager];
    [fileManager removeItemAtPath:photoArray[val.row] error:nil];
    [photoArray  removeObjectAtIndex:val.row];
    [self.colView reloadData];
}

- (void)photoBrowser:(XLPhotoBrowser *)browser clickActionSheetIndex:(NSInteger)actionSheetindex currentImageIndex:(NSInteger)currentImageIndex
{
    NSLog(@"点击了actionSheet索引是:%zd , 当前展示的图片索引是:%zd",actionSheetindex,currentImageIndex);
    
    // do something yourself
    switch (actionSheetindex) {
        case 0: // 保存
        {
            [browser saveCurrentShowImage];
        }
            break;
        default:
        {
            int a = browser.currentImageIndex;
            
            NSFileManager *fileManager = [NSFileManager defaultManager];
            [fileManager removeItemAtPath:photoArray[a] error:nil];
            [photoArray removeObjectAtIndex:a];
            [self.colView reloadData];
            
            [browser deleteCurrentShowImage];
            
            
        }
            break;
    }
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

#pragma mark - CNPPopupController Delegate

- (void)showPopupWithStyle:(CNPPopupStyle)popupStyle {
    
    
    
    NSMutableParagraphStyle *paragraphStyle = NSMutableParagraphStyle.new;
    paragraphStyle.lineBreakMode = NSLineBreakByWordWrapping;
    paragraphStyle.alignment = NSTextAlignmentCenter;
    
    
    
    //NSAttributedString *title = [[NSAttributedString alloc] initWithString:@"It's A Popup!" attributes:@{NSFontAttributeName : [UIFont boldSystemFontOfSize:24], NSParagraphStyleAttributeName : paragraphStyle}];
    
    //NSAttributedString *lineOne = [[NSAttributedString alloc] initWithString:@"You can add text and images" attributes:@{NSFontAttributeName : [UIFont systemFontOfSize:18], NSParagraphStyleAttributeName : paragraphStyle}];
    
    //NSAttributedString *lineTwo = [[NSAttributedString alloc] initWithString:@"With style, using NSAttributedString" attributes:@{NSFontAttributeName : [UIFont systemFontOfSize:18], NSForegroundColorAttributeName : [UIColor colorWithRed:0.46 green:0.8 blue:1.0 alpha:1.0], NSParagraphStyleAttributeName : paragraphStyle}];
    
    
    
    CNPPopupButton *button = [[CNPPopupButton alloc] initWithFrame:CGRectMake(0, 0, [UIScreen mainScreen].bounds.size.width - 40, 48)];
    //[button setTitleColor:[UIColor blackColor] forState:UIControlStateNormal];
    button.titleLabel.font = [UIFont boldSystemFontOfSize:18];
    [button setTitle:@"删除" forState:UIControlStateNormal];
    button.backgroundColor = [UIColor colorWithRed:0.46 green:0.8 blue:1.0 alpha:1.0];
    button.layer.cornerRadius = 4;
    button.selectionHandler = ^(CNPPopupButton *button){
        
        [self.popupController dismissPopupControllerAnimated:YES];
        
        NSLog(@"Block for button: %@", button.titleLabel.text);
        
    };
    
    
    


    
    self.popupController = [[CNPPopupController alloc] initWithContents:@[button]];
    
    self.popupController.theme = [CNPPopupTheme defaultTheme];
    self.popupController.theme.popupStyle = popupStyle;
    self.popupController.delegate = self;
    [self.popupController presentPopupControllerAnimated:YES];
    
}


- (void)popupController:(CNPPopupController *)controller didDismissWithButtonTitle:(NSString *)title {
    
    NSLog(@"Dismissed with button title: %@", title);
    
}



- (void)popupControllerDidPresent:(CNPPopupController *)controller {
    
    NSLog(@"Popup controller presented.");
    
}


@end
