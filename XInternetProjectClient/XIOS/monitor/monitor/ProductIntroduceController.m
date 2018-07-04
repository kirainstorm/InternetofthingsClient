//
//  ProductIntroduceController.m
//  monitor
//
//  Created by xjm on 16/2/23.
//  Copyright © 2016年 warningsys. All rights reserved.
//

#import "ProductIntroduceController.h"
#import "DCPicScrollView.h"
#import "LiveAddsDeytailViewController.h"
#import "NameCollectionViewCell.h"
//#import "Namecell.h"
#import "Header.h"




@interface ProductIntroduceController ()<UICollectionViewDataSource,UICollectionViewDelegate,UICollectionViewDelegateFlowLayout>
//@property(nonatomic,retain)UICollectionView *collectionview;
@property(nonatomic,retain)NSMutableArray *dataarray;
@end
//CGFloat h = 200;
@implementation ProductIntroduceController



@synthesize MainView;
@synthesize scrollView;
@synthesize colView;

- (void)viewDidLoad {
    [super viewDidLoad];
    
    
    
    self.view.backgroundColor=[UIColor whiteColor];
    self.dataarray=[[NSMutableArray alloc]init];
    
    
    CGSize s;
    s.width = 320;
    s.height = 170 *4;
    colView.frame = CGRectMake(0,180, [UIScreen mainScreen].bounds.size.width,s.height);
    s.height += 130;
    MainView.contentSize = s;
    
    colView.scrollEnabled = NO;
    
    //UICollectionViewFlowLayout *flayout=[[UICollectionViewFlowLayout alloc]init];
    //self.collectionview=[[UICollectionView alloc]initWithFrame:CGRectMake(0,180, [UIScreen mainScreen].bounds.size.width, [UIScreen mainScreen].bounds.size.height-180-110) collectionViewLayout:flayout];
    //self.navigationController.navigationBar.translucent=NO;
    self.colView.delegate=self;
    self.colView.dataSource=self;
    self.colView.backgroundColor=[UIColor whiteColor];
    //[self.view addSubview:self.collectionview];
    //调用载入图片的方法
    [self LoadScrollPic];
    //注册cell
    [self.colView registerNib:[UINib nibWithNibName:@"NameCollectionViewCell" bundle:nil] forCellWithReuseIdentifier:@"one"];
    //注册cell
    //[self.collectionview registerNib:[UINib nibWithNibName:@"Namecell" bundle:nil] forCellWithReuseIdentifier:@"two"];
    // Do any additional setup after loading the view.

    
    
    //
}
//布局
-(void)layout{
    
    
    
    
}
//返回CollectionView中分区的个数
- (NSInteger)numberOfSectionsInCollectionView:(UICollectionView *)collectionView{
    
    return 1;
}
////返回item之间的最小列间距
- (CGFloat)collectionView:(UICollectionView *)collectionView layout:(UICollectionViewLayout*)collectionViewLayout minimumInteritemSpacingForSectionAtIndex:(NSInteger)section{
    
    return 0;
}
//返回每一行item之间的最小间距
- (CGFloat)collectionView:(UICollectionView *)collectionView layout:(UICollectionViewLayout*)collectionViewLayout minimumLineSpacingForSectionAtIndex:(NSInteger)section{
    return 0;
}
//返回每个分区item的个数
- (NSInteger)collectionView:(UICollectionView *)collectionView numberOfItemsInSection:(NSInteger)section{
    //    if (section%2==0) {
    //        return 1;
    //    }else{
    return 8;
    // }
    
}
//返回每个item的大小
- (CGSize)collectionView:(UICollectionView *)collectionView layout:(UICollectionViewLayout*)collectionViewLayout sizeForItemAtIndexPath:(NSIndexPath *)indexPath{
    
    //    if (indexPath.section%2==0) {
    //        return CGSizeMake(KscreenWidth, 60);
    //    }else{
    return CGSizeMake(KscreenWidth/2, 160);
    //    }
    
    
}
- (UICollectionViewCell *)collectionView:(UICollectionView *)collectionView cellForItemAtIndexPath:(NSIndexPath *)indexPath{
    //    if (indexPath.section%2==1) {
    NameCollectionViewCell *cell = [collectionView dequeueReusableCellWithReuseIdentifier:@"one" forIndexPath:indexPath];
    //        TrivalModel *model=self.dataarray[(indexPath.section-1)/2];
    cell.productpicture.image=[UIImage imageNamed:@"productpicture"];
    cell.namepicture.text=@"球状摄像头";
    //        cell.productpicture.layer.cornerRadius=15;
    //        cell.productpicture.layer.masksToBounds=YES;
    cell.namepicture.textColor=[UIColor redColor];
    //        cell.picture.layer.cornerRadius=8;
    //        cell.picture.layer.masksToBounds=YES;
    return cell;
    //   }else{
    //       Namecell *cell = [collectionView dequeueReusableCellWithReuseIdentifier:@"two" forIndexPath:indexPath];
    //       cell.productname.text=@"球状系列产品展示";
    //       cell.productname.textColor=[UIColor grayColor];
    ////        TrivalModel *model=self.dataarray[indexPath.section/2];
    ////        cell.namelabel.text=[NSString stringWithFormat:@"%@地区",model.region];
    //        return cell;
    //   }
}
//点击item的时候
-(void)collectionView:(UICollectionView *)collectionView didSelectItemAtIndexPath:(NSIndexPath *)indexPath{
    
    //模态推出新的视图
    LiveAddsDeytailViewController *addVC = [[LiveAddsDeytailViewController alloc]init];
    addVC.view.backgroundColor=[UIColor whiteColor];
    //给模态出来的视图控制添加导航控制器
    UINavigationController *nagvigationVC  = [[UINavigationController alloc]initWithRootViewController:addVC];
    //配置模态出视图的样式
    nagvigationVC.modalTransitionStyle = 2;
    //模态不依赖于导航控制器,所以不需要找到导航控制器
    [self presentViewController:nagvigationVC animated:YES completion:^{
        //推出视图之后要做一些操作,可以写在这里;
    }];
    
    // 达到点击item跳转到一个新的界面  无论是产品详情还是产品类别分类
    //        TrivalSecondController *detail=[[TrivalSecondController alloc]init];
    //        TrivalModel *model=self.dataarray[(indexPath.section-1)/2];
    //        detail.title=model.province[indexPath.item][@"name"];
    //        detail.view.backgroundColor=[UIColor whiteColor];
    //        [self.navigationController pushViewController:detail animated:YES];
}






- (void)LoadScrollPic {
    //还可以通过网络加载
    /*
     NSArray *arr = @[@"http ://p1.qou.com/pic/UploadPic/2013-3/19/2013031923222781617.jpg",
     @"http ://cdn.duang.com/uploads/item/201409/27/20140927192649_NxVKT.thumb.700_0.png",
     @"http ://img4.dang.com/uploads/item/201409/27/20140927192458_GcRxV.jpeg",
     @"http ://cdn.dung.com/uploads/item/201304/20/20130420192413_TeRRP.thumb.700_0.jpeg"];
     */
    NSArray *ImageName = @[@"rolling.png",
                           @"rolling.png",
                           @"rolling.png",
                           @"rolling.png"];
    
    
    NSArray *titleData = [@"xxxxx.科技进步.时代先锋.科学进步." componentsSeparatedByString:@"."];
    
    DCPicScrollView  *picView = [[DCPicScrollView alloc] initWithFrame:CGRectMake(0,0, self.view.frame.size.width, 180) WithImageNames:ImageName];
    
    picView.titleData = titleData;
    picView.placeImage = [UIImage imageNamed:@"rolling.png"];//占位图片
    picView.AutoScrollDelay = 3.0f;
    [picView setImageViewDidTapAtIndex:^(NSInteger index) {
        //内部写点击的方法
        //模态推出新的视图
        LiveAddsDeytailViewController *addVC = [[LiveAddsDeytailViewController alloc]init];
        addVC.view.backgroundColor=[UIColor whiteColor];
        //给模态出来的视图控制添加导航控制器
        UINavigationController *nagvigationVC  = [[UINavigationController alloc]initWithRootViewController:addVC];
        //配置模态出视图的样式
        nagvigationVC.modalTransitionStyle = 2;
        //模态不依赖于导航控制器,所以不需要找到导航控制器
        [self presentViewController:nagvigationVC animated:YES completion:^{
            //推出视图之后要做一些操作,可以写在这里;
        }];
        printf("你点到我了index:%zd\n",index);
    }];
    [self.scrollView addSubview:picView];
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

@end

