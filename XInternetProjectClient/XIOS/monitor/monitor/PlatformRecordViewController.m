//
//  PlatformRecordTableViewController.m
//  PlatformMonitor
//
//  Created by rs on 2017/5/23.
//  Copyright © 2017年 warningsys. All rights reserved.
//

#import "PlatformRecordViewController.h"
#import "AppDelegate.h"
#import <CoreData/CoreData.h>
#import "IOSPlatformDevice.h"
@import AVKit;
@import AVFoundation;
@interface PlatformRecordViewController ()<UITableViewDataSource,UITableViewDelegate>

@end

@implementation PlatformRecordViewController

@synthesize rowid;
@synthesize recordArray;
@synthesize tabView;
IOSPlatformDevice *_dev4record;


- (void)viewDidLoad {
    [super viewDidLoad];

    
    _dev4record=((AppDelegate *)[UIApplication sharedApplication].delegate).DeviceArray[rowid];
    recordArray =[NSMutableArray array];
    
    
    //yun xun xuan zhong?
    //self.tabView.allowsSelection = YES;
    //fen ge xian
    //self.tabView.separatorStyle = UITableViewCellSeparatorStyleNone;
    //self.tabView.separatorStyle = UITableViewCellSeparatorStyleSingleLine;
    //self.tabView.separatorStyle = UITableViewCellSeparatorStyleSingleLineEtched;
    self.tabView.dataSource = self;
    self.tabView.delegate = self;
    
    
    
    NSString * docDir = [((AppDelegate *)[UIApplication sharedApplication].delegate) GetMyappDocumentsDirectory];
    NSString * string = [docDir stringByAppendingFormat:@"/rec/Mp4Files/%@",_dev4record->dev_id];
    [self FindFiles:string];
    [self.tabView reloadData];
    
    

}

- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

-(IBAction)goback:(id)sender
{
    
    //AppDelegate* appdelegate = (AppDelegate *)[UIApplication sharedApplication].delegate;
    //appdelegate.allowRoation = 0;
    [self dismissViewControllerAnimated:YES completion:nil];
}


#pragma mark - Table view data source

- (NSInteger)numberOfSectionsInTableView:(UITableView *)tableView
{
    return 1;
}

-(NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section
{
    return recordArray.count;
}

-(UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath
{
    CGFloat FontOfSize_ = 14.0;

    static NSString *simpleIdentifier = @"SimpleTableCell";
    
    UITableViewCell *cell = [tabView dequeueReusableCellWithIdentifier:simpleIdentifier];
    if (cell == nil) {
        cell = [[UITableViewCell alloc] initWithStyle:UITableViewCellStyleDefault reuseIdentifier:simpleIdentifier];
    }
    

    NSArray *array = [recordArray[indexPath.row] componentsSeparatedByString:@"/"];
    cell.textLabel.text = array[array.count -1];
    cell.imageView.image = [UIImage imageNamed:@"w3.png"];
 

    cell.accessoryType = UITableViewCellAccessoryDisclosureIndicator;
    cell.selectionStyle = UITableViewCellSelectionStyleBlue;
    [cell.textLabel setFont:[UIFont systemFontOfSize:FontOfSize_]];
    
    return cell;
}


/*
// Override to support conditional editing of the table view.
- (BOOL)tableView:(UITableView *)tableView canEditRowAtIndexPath:(NSIndexPath *)indexPath {
    // Return NO if you do not want the specified item to be editable.
    return YES;
}
*/

/*
// Override to support editing the table view.
- (void)tableView:(UITableView *)tableView commitEditingStyle:(UITableViewCellEditingStyle)editingStyle forRowAtIndexPath:(NSIndexPath *)indexPath {
    if (editingStyle == UITableViewCellEditingStyleDelete) {
        // Delete the row from the data source
        [tableView deleteRowsAtIndexPaths:@[indexPath] withRowAnimation:UITableViewRowAnimationFade];
    } else if (editingStyle == UITableViewCellEditingStyleInsert) {
        // Create a new instance of the appropriate class, insert it into the array, and add a new row to the table view
    }   
}
*/

/*
// Override to support rearranging the table view.
- (void)tableView:(UITableView *)tableView moveRowAtIndexPath:(NSIndexPath *)fromIndexPath toIndexPath:(NSIndexPath *)toIndexPath {
}
*/

/*
// Override to support conditional rearranging of the table view.
- (BOOL)tableView:(UITableView *)tableView canMoveRowAtIndexPath:(NSIndexPath *)indexPath {
    // Return NO if you do not want the item to be re-orderable.
    return YES;
}
*/


#pragma mark - Table view delegate

-(void) tableView:(UITableView *)tableView didSelectRowAtIndexPath:(nonnull NSIndexPath *)indexPath
{

    //NSString *videoPath = [[NSBundle mainBundle] pathForResource:@"instruction" ofType:@"mp4"];
    NSURL *url = [NSURL fileURLWithPath:recordArray[indexPath.row]];
    AVPlayer *player = [AVPlayer playerWithURL:url];
    AVPlayerViewController *playerViewController = [AVPlayerViewController new];
    playerViewController.player = player;
    [self presentViewController:playerViewController animated:YES completion:nil];
    [playerViewController.player play];
    
}


/*
#pragma mark - Navigation

// In a storyboard-based application, you will often want to do a little preparation before navigation
- (void)prepareForSegue:(UIStoryboardSegue *)segue sender:(id)sender {
    // Get the new view controller using [segue destinationViewController].
    // Pass the selected object to the new view controller.
}
*/

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
            
            [recordArray addObject:path];
        }
    }else{
        NSLog(@"你打印的是目录或者不存在");
    }
}

@end
