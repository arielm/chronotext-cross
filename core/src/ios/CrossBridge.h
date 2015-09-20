/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012-2015, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE SIMPLIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

/*
 * "TOUCH MAPPING" BASED ON CINDER:
 * https://github.com/cinder/Cinder/blob/v0.8.5/include/cinder/app/CinderViewCocoaTouch.h
 */

#import "cocoa/NSString+JSON.h"
#import "ios/BridgeListener.h"
#import "ios/GLViewController.h"

enum
{
    SKETCH_WILL_INIT,
    SKETCH_DID_INIT,
    SKETCH_WILL_UNINIT,
    SKETCH_DID_UNINIT,
    
    APP_DID_RESUME,
    APP_WILL_PAUSE,
    VIEW_WILL_APPEAR,
    VIEW_WILL_DISAPPEAR,
    
    SKETCH_WILL_SETUP,
    SKETCH_DID_SETUP,
    SKETCH_WILL_SHUTDOWN,
    SKETCH_DID_SHUTDOWN
};

@interface CrossBridge : NSObject <UIAccelerometerDelegate, BridgeListener>
{
    NSObject<BridgeListener> *listener;
    GLViewController *viewController;
    NSDictionary *viewControllerProperties;
}

@property (nonatomic, retain) NSObject<BridgeListener> *listener;
@property (nonatomic, readonly) GLViewController *viewController;
@property (nonatomic, retain) NSDictionary *viewControllerProperties;

- (id) initWithListener:(NSObject<BridgeListener>*)listener;

- (void) performSetup;
- (void) performResize;
- (void) performUpdate;
- (void) performDraw;

- (void) startWithReason:(int)reason;
- (void) stopWithReason:(int)reason;

//- (void) touchesBegan:(NSSet*)touches withEvent:(UIEvent*)event;
//- (void) touchesMoved:(NSSet*)touches withEvent:(UIEvent*)event;
//- (void) touchesEnded:(NSSet*)touches withEvent:(UIEvent*)event;
//- (void) touchesCancelled:(NSSet*)touches withEvent:(UIEvent*)event;

- (void) sendMessageToSketch:(int)what;
- (void) sendMessageToSketch:(int)what json:(id)json;
- (void) sendMessageToSketch:(int)what body:(NSString*)body;

- (void) dispatchMessage:(int)what body:(NSString*)body;
- (void) dispatchEvent:(int)eventId;

@end

namespace chr
{
    namespace system
    {
        extern CrossBridge *bridge;
    }
}
