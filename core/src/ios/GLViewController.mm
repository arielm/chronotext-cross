/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012-2015, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE SIMPLIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#import "ios/GLViewController.h"
#import "ios/CrossBridge.h"
#import "cocoa/Utils.h"

NSString* GLViewControllerRenderingAPIKey = @"GLViewControllerRenderingAPIKey";
NSString* GLViewControllerPreferredFramesPerSecondKey = @"GLViewControllerPreferredFramesPerSecondKey";
NSString* GLViewControllerMultipleTouchEnabledKey = @"GLViewControllerMultipleTouchEnabledKey";
NSString* GLViewControllerInterfaceOrientationMaskKey = @"GLViewControllerInterfaceOrientationMaskKey";
NSString* GLViewControllerColorFormatKey = @"GLViewControllerColorFormatKey";
NSString* GLViewControllerDepthFormatKey = @"GLViewControllerDepthFormatKey";
NSString* GLViewControllerStencilFormatKey = @"GLViewControllerStencilFormatKey";
NSString* GLViewControllerMultisampleKey = @"GLViewControllerMultisampleKey";

@interface GLViewController ()
{
    NSMutableDictionary *properties;
    NSUInteger interfaceOrientationMask;

    BOOL started;
    BOOL appeared;

    BOOL resizeRequest;
    int viewportWidth;
    int viewportHeight;

    BOOL startRequest;
    int startReason;
    
    int updateCount;
    int drawCount;
}

- (void) startWithReason:(int)reason;
- (void) stopWithReason:(int)reason;

@end

@implementation GLViewController

@synthesize crossBridge;
@synthesize glView;
@synthesize appeared;

- (id) initWithBridge:(CrossBridge*)bridge properties:(NSDictionary*)_properties
{
    if (self = [super init])
    {
        crossBridge = bridge;
        [crossBridge retain];

        // ---
        
        NSDictionary *defaults =
        @{
          GLViewControllerRenderingAPIKey: @(kEAGLRenderingAPIOpenGLES2),
          GLViewControllerPreferredFramesPerSecondKey: @60,
          GLViewControllerMultipleTouchEnabledKey: @YES,
          GLViewControllerInterfaceOrientationMaskKey: @(UIInterfaceOrientationMaskPortrait),
          GLViewControllerColorFormatKey: @(GLKViewDrawableColorFormatRGBA8888),
          GLViewControllerDepthFormatKey: @(GLKViewDrawableDepthFormat24),
          GLViewControllerStencilFormatKey: @(GLKViewDrawableStencilFormatNone),
          GLViewControllerMultisampleKey: @(GLKViewDrawableMultisampleNone),
          };
        
        if (_properties)
        {
            properties = [[NSMutableDictionary alloc] initWithDictionary:_properties];
        }
        else
        {
            properties = [[NSMutableDictionary alloc] init];
        }
        
        for (id key in defaults)
        {
            if (![properties objectForKey:key])
            {
                [properties setObject:[defaults objectForKey:key] forKey:key];
            }
        }

        interfaceOrientationMask = [properties[GLViewControllerInterfaceOrientationMaskKey] intValue];
    }
    
    return self;
}

- (void) dealloc
{
    DLOG(@"GLViewController.dealloc");
    
    [crossBridge release];
    [properties release];
    
    [super dealloc];
}

- (void) loadView
{
    DLOG(@"GLViewController:loadView");
    
    [super loadView];

    glView = (GLKView*)self.view;
    glView.context = [[[EAGLContext alloc] initWithAPI:(EAGLRenderingAPI)[properties[GLViewControllerRenderingAPIKey] intValue]] autorelease];

    self.preferredFramesPerSecond = [properties[GLViewControllerPreferredFramesPerSecondKey] intValue];
    self.view.multipleTouchEnabled = [properties[GLViewControllerMultipleTouchEnabledKey] boolValue];

    glView.drawableColorFormat = (GLKViewDrawableColorFormat)[properties[GLViewControllerColorFormatKey] intValue];
    glView.drawableDepthFormat = (GLKViewDrawableDepthFormat)[properties[GLViewControllerDepthFormatKey] intValue];
    glView.drawableStencilFormat = (GLKViewDrawableStencilFormat)[properties[GLViewControllerStencilFormatKey] intValue];
    glView.drawableMultisample = (GLKViewDrawableMultisample)[properties[GLViewControllerMultisampleKey] intValue];
    
    if ([UIScreen.mainScreen respondsToSelector:@selector(nativeScale)]) // I.E. IOS 8+
    {
        /*
         * REFERENCE:
         * - https://github.com/brackeen/glfm/blob/c3d7a72872d82eac903285b6f108ea83ac79e66c/src/glfm_platform_ios.m#L366-371
         *
         * REAL IOS 8+ DEVICES TESTED:
         * - IPAD AIR 2
         * - IPHONE 6
         *
         * REAL IOS 8+ DEVICES TO TEST:
         * - IPHONE 6+
         */
        
        self.view.contentScaleFactor = UIScreen.mainScreen.nativeScale;
    }

    // ---
    
    [EAGLContext setCurrentContext:glView.context]; // MUST TAKE PLACE BEFORE "SETUP"
    
    [crossBridge performSetup];
    resizeRequest = YES;
}

- (void) viewWillAppear:(BOOL)animated
{
    [super viewWillAppear:animated];
    
    if (self.view && !appeared)
    {
        appeared = YES;
        
        DLOG(@"GLViewController:viewWillAppear | beingPresented: %d | movingToParentViewController: %d", [self isBeingPresented], [self isMovingToParentViewController]);
        [self startWithReason:REASON_VIEW_WILL_APPEAR];
        
        NSNotificationCenter *center = [NSNotificationCenter defaultCenter];
        [center addObserver:self selector:@selector(applicationDidBecomeActive) name:UIApplicationDidBecomeActiveNotification object:nil];
        [center addObserver:self selector:@selector(applicationWillResignActive) name:UIApplicationWillResignActiveNotification object:nil];
    }
}

- (void) viewWillDisappear:(BOOL)animated
{
    [super viewWillDisappear:animated];
    
    if (self.view && appeared)
    {
        appeared = NO;
        
        DLOG(@"GLViewController:viewWillDisappear | beingDismissed: %d | movingFromParentViewController: %d", [self isBeingDismissed], [self isMovingFromParentViewController]);
        [self stopWithReason:REASON_VIEW_WILL_DISAPPEAR];
        
        NSNotificationCenter *center = [NSNotificationCenter defaultCenter];
        [center removeObserver:self name:UIApplicationDidBecomeActiveNotification object:nil];
        [center removeObserver:self name:UIApplicationWillResignActiveNotification object:nil];
    }
}

- (void) startWithReason:(int)reason
{
    if (!started)
    {
        [EAGLContext setCurrentContext:glView.context]; // MUST TAKE PLACE BEFORE "START" AND "DRAW"
        
        started = YES;
        startRequest = YES;
        startReason = reason;
        
        drawCount = 0;
        updateCount = 0;
    }
}

- (void) stopWithReason:(int)reason
{
    if (started)
    {
        [crossBridge stopWithReason:reason];
        started = NO;
    }
}

- (void) update
{
    if (started)
    {
        if (resizeRequest)
        {
            [crossBridge performResize];
            resizeRequest = NO;
        }
        
        if (startRequest)
        {
            [crossBridge startWithReason:startReason];
        }
        
        [crossBridge performUpdate];
    }
    
    startRequest = NO;
    updateCount++;
}

- (void) glkView:(GLKView*)view drawInRect:(CGRect)rect
{
    if (started)
    {
        if (updateCount == 0)
        {
            [self update];
        }
        
        if (drawCount++ == 0)
        {
            DLOG(@"GLViewController:drawInRect");
        }
        
        [crossBridge performDraw];
    }
}

/*
 * FOR iOS 7+
 */
- (BOOL) prefersStatusBarHidden
{
    return YES;
}

#pragma mark ---------------------------------------- ORIENTATION ----------------------------------------

- (void) willRotateToInterfaceOrientation:(UIInterfaceOrientation)toInterfaceOrientation duration:(NSTimeInterval)duration
{
    /*
     * WHEN ROTATING FROM PORTRAIT TO LANDSCAPE (OR VICE-VERSA):
     *
     * - THIS IS NECESSARY, OTHERWISE SCREEN WILL BE DEFORMED
     *
     * - CURRENTLY, THERE IS STILL A SHORT "STRETCH ARTEFACT" DURING ROTATION:
     *   A MINOR ISSUE WHICH WON'T AFFECT MOST OF THE APPS, WHICH ARE
     *   SUPPOSED TO BE EITHER LANDSCAPE RIGHT/LEFT, OR PORTAIT BOTTOM/UP
     */
    resizeRequest = YES;
}

/*
 * IMPORTANT:
 *
 * - SUPPORTED ORIENTATIONS SHOULD ALSO BE LISTED IN Info.plist
 * - THE APP WILL START USING THE 1ST ORIENTATION IN THE LIST (Item 0)
 */

/*
 * FOR iOS 6+
 */

- (BOOL) shouldAutorotate
{
    return YES;
}

- (NSUInteger) supportedInterfaceOrientations
{
    return interfaceOrientationMask;
}

/*
 * FOR iOS 5.x
 */

- (BOOL) shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)toInterfaceOrientation
{
    return (1 << toInterfaceOrientation) & interfaceOrientationMask;
}

#pragma mark ---------------------------------------- TOUCH ----------------------------------------

- (void) touchesBegan:(NSSet*)touches withEvent:(UIEvent*)event
{
//    [crossBridge touchesBegan:touches withEvent:event];
}

- (void) touchesMoved:(NSSet*)touches withEvent:(UIEvent*)event
{
//    [crossBridge touchesMoved:touches withEvent:event];
}

- (void) touchesEnded:(NSSet*)touches withEvent:(UIEvent*)event
{
//    [crossBridge touchesEnded:touches withEvent:event];
}

- (void) touchesCancelled:(NSSet*)touches withEvent:(UIEvent*)event
{
//    [crossBridge touchesCancelled:touches withEvent:event];
}

#pragma mark ---------------------------------------- NOTIFICATIONS ----------------------------------------

- (void) applicationDidBecomeActive
{
    if (self.view)
    {
        DLOG(@"GLViewController:applicationDidBecomeActive");
        [self startWithReason:REASON_APP_DID_BECOME_ACTIVE];
    }
}

- (void) applicationWillResignActive
{
    if (self.view)
    {
        DLOG(@"GLViewController:applicationWillResignActive");
        [self stopWithReason:REASON_APP_WILL_RESIGN_ACTIVE];
    }
}

@end
