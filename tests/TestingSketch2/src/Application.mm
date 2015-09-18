
#import "Application.h"

@implementation Application

- (BOOL) application:(UIApplication*)application didFinishLaunchingWithOptions:(NSDictionary*)launchOptions
{
    window = [[UIWindow alloc] initWithFrame:UIScreen.mainScreen.bounds];
    window.backgroundColor = [UIColor redColor];
    [window makeKeyAndVisible];
    
    return YES;
}

- (void) dealloc
{
    [window release];
    [super dealloc];
}

@end
