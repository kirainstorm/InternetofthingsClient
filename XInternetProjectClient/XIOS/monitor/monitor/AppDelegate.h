//
//  AppDelegate.h
//  monitor
//
//  Created by xjm on 16/1/18.
//  Copyright © 2016年 warningsys. All rights reserved.
//

#import <UIKit/UIKit.h>

@interface AppDelegate : UIResponder <UIApplicationDelegate>

@property(nonatomic,assign) NSInteger allowRoation;

@property (strong, nonatomic) UIWindow *window;
//@property (nonatomic, retain, readonly) NSManagedObjectContext *managedObjectContext;
//@property (nonatomic, retain, readonly) NSManagedObjectModel *managedObjectModel;
//@property (nonatomic, retain, readonly) NSPersistentStoreCoordinator *persistentStoreCoordinator;


//
@property(nonatomic,strong)NSString* UserName;
@property(nonatomic,strong)NSString* UserPwd;
@property(nonatomic,strong)NSString* ServerUrl;

@property(nonatomic,strong)NSMutableArray *DeviceArray;//IOSPlatformDevice
//@property(nonatomic,strong)NSManagedObjectContext *dbContext;


//- (void)saveContext;
//+ (instancetype) sharedCoreDataManager;
- (NSURL *)applicationDocumentsDirectory;

- (NSString*)GetMyappDocumentsDirectory;

- (void)SetUserInfo:(NSString *)user pwd:(NSString *)pwd svr:(NSString *)svr;
- (BOOL)RefreshDevices;

@end

