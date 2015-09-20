
#pragma once

#import "CrossBridge.h"

@interface Application : NSObject <UIApplicationDelegate>
{
    CrossBridge *bridge;
    UIWindow *window;
}

@end
