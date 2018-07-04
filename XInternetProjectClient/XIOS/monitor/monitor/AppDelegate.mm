//
//  AppDelegate.m
//  monitor
//
//  Created by xjm on 16/1/18.
//  Copyright © 2016年 warningsys. All rights reserved.
//

#import "AppDelegate.h"
#import <CoreData/CoreData.h>
#import "Reachability.h"
#import "mytoast.h"

#import "IOSDevInterface.h"

@interface AppDelegate ()

@end
static AppDelegate *coredataManager;
@implementation AppDelegate

//
//@synthesize managedObjectContext;
//@synthesize managedObjectModel;
//@synthesize persistentStoreCoordinator;
//
@synthesize UserName;
@synthesize UserPwd;
@synthesize ServerUrl;
@synthesize DeviceArray;

//@synthesize dbContext;

#define UIColorFromRGB(rgbValue) [UIColor colorWithRed:((float)((rgbValue & 0xFF0000) >> 16))/255.0 green:((float)((rgbValue & 0xFF00) >> 8))/255.0 blue:((float)(rgbValue & 0xFF))/255.0 alpha:1.0]


IOSDevInterface * pIOSDevInterface4Application = nil;

- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions {
    // Override point for customization after application launch.
    
    XNetStartStreamManager(4);
    
    
    //self.tabBarController.tabBar.translucent = NO;
    _allowRoation = 0;
    
    
    [[UITabBar appearance] setTranslucent:NO];
    
    
    [[UINavigationBar appearance] setBarTintColor:UIColorFromRGB(0xfefefe)];
    

    //设定通知接受夜间模式
    [[NSNotificationCenter defaultCenter]addObserver:self selector:@selector(night) name:@"night" object:nil];
    //设定通知接受白天模式
    [[NSNotificationCenter defaultCenter]addObserver:self selector:@selector(day) name:@"day" object:nil];

    
    
    
    //dbContext=[((AppDelegate *)[UIApplication sharedApplication].delegate) managedObjectContext];
    //dbContext=[((AppDelegate *)[UIApplication sharedApplication].delegate) managedObjectContext];
    DeviceArray =[NSMutableArray array];
    pIOSDevInterface4Application = [[IOSDevInterface alloc] init];
    
    
    
    
    
    [NSThread sleepForTimeInterval:1.0];//设置启动页面时间
    
    
    
    
    
    
//    [((AppDelegate *)[UIApplication sharedApplication].delegate).dataArray removeAllObjects];
//    //查询
//    NSFetchRequest *request=[[NSFetchRequest alloc]init];
//    //创建实体
//    NSEntityDescription *womenED=[NSEntityDescription entityForName:@"Device" inManagedObjectContext:((AppDelegate *)[UIApplication sharedApplication].delegate).dbContext];
//    //给查询类设置实体描述
//    [request setEntity:womenED];
//    //排序类
//    NSSortDescriptor *sortDescriptor=[NSSortDescriptor sortDescriptorWithKey:@"devicedate" ascending:YES];
//    //设置排序方式
//    [request setSortDescriptors:@[sortDescriptor]];
//    NSError *error=nil;
//    //根据指定的条件查询数据
//    NSArray *resultarray=[((AppDelegate *)[UIApplication sharedApplication].delegate).dbContext executeFetchRequest:request error:&error];
//    if (resultarray.count!=0) {
//        [((AppDelegate *)[UIApplication sharedApplication].delegate).dataArray setArray:resultarray];
//    }
    
    
    return YES;
}


- (void)applicationWillResignActive:(UIApplication *)application {
    // Sent when the application is about to move from active to inactive state. This can occur for certain types of temporary interruptions (such as an incoming phone call or SMS message) or when the user quits the application and it begins the transition to the background state.
    // Use this method to pause ongoing tasks, disable timers, and throttle down OpenGL ES frame rates. Games should use this method to pause the game.
}

- (void)applicationDidEnterBackground:(UIApplication *)application {
    // Use this method to release shared resources, save user data, invalidate timers, and store enough application state information to restore your application to its current state in case it is terminated later.
    // If your application supports background execution, this method is called instead of applicationWillTerminate: when the user quits.
}

- (void)applicationWillEnterForeground:(UIApplication *)application {
    // Called as part of the transition from the background to the inactive state; here you can undo many of the changes made on entering the background.
}

- (void)applicationDidBecomeActive:(UIApplication *)application {
    // Restart any tasks that were paused (or not yet started) while the application was inactive. If the application was previously in the background, optionally refresh the user interface.
}

- (void)applicationWillTerminate:(UIApplication *)application {
    // Called when the application is about to terminate. Save data if appropriate. See also applicationDidEnterBackground:.
}
////
//注意注意注意！！！(UIInterfaceOrientationMaskAllButUpsideDown)
//UIInterfaceOrientationMaskAllButUpsideDown(自动横竖屏)请关闭你的iPhone的锁屏，不然你怎么旋转都不会起作用的
//从iPhone底部上滑，拉出菜单，关闭锁屏
-(UIInterfaceOrientationMask)application:(UIApplication *)application supportedInterfaceOrientationsForWindow:(UIWindow *)window
{
    if(_allowRoation == 1)
    {
        return UIInterfaceOrientationMaskLandscape;//强制横屏
        //return UIInterfaceOrientationMaskAllButUpsideDown;
    }
    else
    {
       return UIInterfaceOrientationMaskPortrait;
    }
}

//夜间模式
- (void)night{
    self.window.alpha = 0.39;
    
}
//白天模式
- (void)day{
    self.window.alpha = 1.0;
}



+ (instancetype) sharedCoreDataManager
{
    static dispatch_once_t onceToken;
    dispatch_once(&onceToken, ^{
        coredataManager = [[self alloc] init];
    });
    return coredataManager;
}
//返回document文件夹路径
- (NSURL *)applicationDocumentsDirectory {
    // The directory the application uses to store the Core Data store file. This code uses a directory named "com-lanou3g-li.LessonCoreData" in the application's documents directory.
    return [[[NSFileManager defaultManager] URLsForDirectory:NSDocumentDirectory inDomains:NSUserDomainMask] lastObject];
}

- (NSString*)GetMyappDocumentsDirectory
{
    
    /*
     Documents：应用中用户数据可以放在这里，iTunes备份和恢复的时候会包括此目录
     tmp：存放临时文件，iTunes不会备份和恢复此目录，此目录下文件可能会在应用退出后删除
     Library/Caches：存放缓存文件，iTunes不会备份此目录，此目录下文件不会在应用退出删除
     */
    //
    NSArray *paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
    NSString *docDir = [paths objectAtIndex:0];
    
    return docDir;
}




//懒加载创建被管理对象模型
//- (NSManagedObjectModel *)managedObjectModel {
//    // The managed object model for the application. It is a fatal error for the application not to be able to find and load its model.
//    if (managedObjectModel != nil) {
//        return managedObjectModel;
//    }
//    //    momd文件是由xcdatamodeld文件编译而来的
//    NSURL *modelURL = [[NSBundle mainBundle] URLForResource:@"database" withExtension:@"momd"];
//    managedObjectModel = [[NSManagedObjectModel alloc] initWithContentsOfURL:modelURL];
//    return managedObjectModel;
//}
//
////通过懒加载方法创建数据链接器
//- (NSPersistentStoreCoordinator *)persistentStoreCoordinator {
//    // The persistent store coordinator for the application. This implementation creates and return a coordinator, having added the store for the application to it.
//    if (persistentStoreCoordinator != nil) {
//        return persistentStoreCoordinator;
//    }
//    
//    // Create the coordinator and store
//    
//    persistentStoreCoordinator = [[NSPersistentStoreCoordinator alloc] initWithManagedObjectModel:[self managedObjectModel]];
//    NSURL *storeURL = [[self applicationDocumentsDirectory] URLByAppendingPathComponent:@"database.sqlite"];
//    NSError *error = nil;
//    NSString *failureReason = @"There was an error creating or loading the application's saved data.";
//    //    coredata支持的4种存储类型
//    //   NSSQLiteStoreType,NSXMLStoreType,NSBinaryStoreType,NSInMemoryStoreType
//    //    如果支持版本迁移  需啊哟在option参数里面写一个字典
//    //    @{NSInferMappingModelAutomaticallyOption:@YES,NSMigratePersistentStoresAutomaticallyOption:@YES}
//    if (![persistentStoreCoordinator addPersistentStoreWithType:NSSQLiteStoreType configuration:nil URL:storeURL options:@{NSInferMappingModelAutomaticallyOption:@YES,NSMigratePersistentStoresAutomaticallyOption:@YES} error:&error]) {
//        // Report any error we got.
//        NSMutableDictionary *dict = [NSMutableDictionary dictionary];
//        dict[NSLocalizedDescriptionKey] = @"Failed to initialize the application's saved data";
//        dict[NSLocalizedFailureReasonErrorKey] = failureReason;
//        dict[NSUnderlyingErrorKey] = error;
//        error = [NSError errorWithDomain:@"YOUR_ERROR_DOMAIN" code:9999 userInfo:dict];
//        // Replace this with code to handle the error appropriately.
//        // abort() causes the application to generate a crash log and terminate. You should not use this function in a shipping application, although it may be useful during development.
//        
//        abort();
//    }
//    
//    return persistentStoreCoordinator;
//}
////数据管理器  被管理对象上下文  通过懒加载方法创建
//
//- (NSManagedObjectContext *)managedObjectContext {
//    // Returns the managed object context for the application (which is already bound to the persistent store coordinator for the application.)
//    if (managedObjectContext != nil) {
//        return managedObjectContext;
//    }
//    
//    NSPersistentStoreCoordinator *coordinator = [self persistentStoreCoordinator];
//    if (!coordinator) {
//        return nil;
//    }
//    //    创建数据管理器
//    managedObjectContext = [[NSManagedObjectContext alloc] initWithConcurrencyType:NSPrivateQueueConcurrencyType];
//    //    设置数据链接器(设置助理)
//    [managedObjectContext setPersistentStoreCoordinator:coordinator];
//    //
//    return managedObjectContext;
//}

#pragma mark - Core Data Saving support

//- (void)saveContext {
//    NSManagedObjectContext *managedObjectContext = self.managedObjectContext;
//    if (managedObjectContext != nil) {
//        NSError *error = nil;
//        if ([managedObjectContext hasChanges] && ![managedObjectContext save:&error]) {
//            // Replace this implementation with code to handle the error appropriately.
//            // abort() causes the application to generate a crash log and terminate. You should not use this function in a shipping application, although it may be useful during development.
//            
//            abort();
//        }
//    }
//}

- (void)SetUserInfo:(NSString *)user pwd:(NSString *)pwd  svr:(NSString *)svr
{
    UserName = user;////[NSString stringWithUTF8String:user];
    UserPwd = pwd;//[NSString stringWithUTF8String:pwd];
    ServerUrl = svr;
}

- (BOOL)RefreshDevices
{
    //检测网络，没有网络不允许登陆
    //检测wifi状态
    Reachability *wifi = [Reachability reachabilityForLocalWiFi];
    //检测手机是否能上网(WIFI\3G\4G)
    Reachability *conn = [Reachability reachabilityForInternetConnection];
    
    //判断网络状态
    if ([wifi currentReachabilityStatus] != NotReachable)
    {
        NSLog(@"wifi");
    }
    else if ([conn currentReachabilityStatus] != NotReachable)
    {
        NSLog(@"2G/3G/4G");
    }
    else
    {
        NSLog(@"没有网络");
        //
        [mytoast showWithText:@"无法连接到网络，请检查!"];
        return FALSE;
    }
    
    
    int _pprt = 7600;
//    BOOL bOK =[pIOSDevInterface4Application GetUserDeviceList:("xplat.srikam.com") center_port:_pprt user:const_cast<char *>([UserName UTF8String]) pwd:const_cast<char *>([UserPwd UTF8String]) devices:DeviceArray];
//    
    BOOL bOK =[pIOSDevInterface4Application GetUserDeviceList:const_cast<char *>([ServerUrl UTF8String]) center_port:_pprt user:const_cast<char *>([UserName UTF8String]) pwd:const_cast<char *>([UserPwd UTF8String]) devices:DeviceArray];
    
    if(!bOK)
    {
        [mytoast showWithText:@"获取失败，请检查!"];
        return FALSE;
    }
    
    
    return TRUE;
}














@end
