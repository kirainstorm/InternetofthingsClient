//
//  PlatformDevice.h
//  PlatformMonitor
//
//  Created by rs on 2017/5/16.
//  Copyright © 2017年 warningsys. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface IOSPlatformDevice : NSObject
{
@public NSNumber *dev_is_publish;
@public NSNumber *dev_id;
@public NSNumber *dev_areaid;
@public NSNumber *dev_parent_areaid;
@public NSString *dev_name;
@public NSString *dev_area_name;
@public NSString *dev_media_ip;
@public NSString *dev_publish_ip;
@public NSNumber *dev_media_port;
@public NSNumber *dev_playback_port;
@public NSNumber *dev_publish_port;
@public NSNumber *dev_browse_num;
@public NSNumber *dev_online;
}


@end
