
#pragma once

#import "ios/CrossBridge.h"

@interface Application : NSObject <UIApplicationDelegate>
{
    CrossBridge *bridge;
    UIWindow *window;
}

@end
