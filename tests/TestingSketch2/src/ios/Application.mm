
#import "Application.h"

@implementation Application

- (BOOL) application:(UIApplication*)application didFinishLaunchingWithOptions:(NSDictionary*)launchOptions
{
    bridge = [[CrossBridge alloc] init];

    bridge.viewControllerProperties =
    @{
        GLViewControllerRenderingAPIKey: @(kEAGLRenderingAPIOpenGLES2),
        GLViewControllerInterfaceOrientationMaskKey: @(UIInterfaceOrientationMaskLandscape),
        GLViewControllerDepthFormatKey: @(GLKViewDrawableDepthFormatNone),
    };

    window = [[UIWindow alloc] initWithFrame:UIScreen.mainScreen.bounds];
    window.backgroundColor = [UIColor blackColor];

    [window setRootViewController:bridge.viewController];
    [window makeKeyAndVisible];

    return YES;
}

- (void) dealloc
{
    [window release];
    [bridge release];

    [super dealloc];
}

@end
