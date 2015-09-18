
#pragma once

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>

#import "CrossBridge.h"

@interface Application : NSObject <UIApplicationDelegate>
{
    CrossBridge *bridge;
    UIWindow *window;
}

@end
