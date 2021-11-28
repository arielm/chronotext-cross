
#import "Application.h"

@implementation Application

- (BOOL) application:(UIApplication*)application didFinishLaunchingWithOptions:(NSDictionary*)launchOptions
{
    GLViewController *viewController = [[[GLViewController alloc] init] autorelease];
    
    viewController.properties =
    @{
        GLViewControllerRenderingAPIKey: @(kEAGLRenderingAPIOpenGLES2),
        GLViewControllerInterfaceOrientationMaskKey: @(UIInterfaceOrientationMaskLandscape),
        GLViewControllerDepthFormatKey: @(GLKViewDrawableDepthFormatNone),
    };

    bridge = [[CrossBridge alloc] init];
    [bridge bind:viewController];

    window = [[UIWindow alloc] initWithFrame:UIScreen.mainScreen.bounds];
    window.backgroundColor = [UIColor blackColor];

    [window setRootViewController:viewController];
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
