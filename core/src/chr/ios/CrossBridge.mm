#import "chr/ios/CrossBridge.h"
#import "chr/cocoa/Utils.h"

#include "chr/ios/CrossDelegate.h"
#include "chr/cross/TouchEvent.h"
#include "chr/cross/WindowInfo.h"

using namespace std;
using namespace chr;

namespace chr
{
  namespace system
  {
    CrossBridge *bridge = nil;
  }
}

@interface CrossBridge ()
{
  BOOL initialized;
  BOOL setup;

  CrossDelegate *crossDelegate;
  
  map<UITouch*, uint32_t> touchIdMap;
}

- (BOOL) performInit;
- (void) performUninit;

- (glm::vec2) windowSize;
- (int) aaSamples;
- (int) depthBits;

- (uint32_t) addTouchToMap:(UITouch*)touch;
- (void) removeTouchFromMap:(UITouch*)touch;
- (uint32_t) findTouchInMap:(UITouch*)touch;
- (void) updateActiveTouches;

@end

@implementation CrossBridge

@synthesize listener;

- (id) init
{
  return [self initWithListener:nil];
}

- (id) initWithListener:(NSObject<BridgeListener>*)_listener
{
  if (self = [super init])
  {
    self.listener = _listener;
    [self performInit];
  }
  
  return self;
}

- (void) dealloc
{
  DLOG(@"CrossBridge.dealloc");

  [listener release];
  [viewController release];
  [self performUninit];
  
  [super dealloc];
}

- (void) bind:(GLViewController*)controller
{
    viewController = controller;
    [viewController retain];

    [viewController bind:self];
}

- (BOOL) performInit
{
  if (!initialized)
  {
    DLOG(@"CrossBridge.performInit");

    system::bridge = self;
    crossDelegate = new CrossDelegate();
    
    NSNotificationCenter* center = [NSNotificationCenter defaultCenter];
    [center addObserver:self selector:@selector(applicationWillTerminate) name:UIApplicationWillTerminateNotification object:nil];
    [center addObserver:self selector:@selector(applicationWillResignActive) name:UIApplicationWillResignActiveNotification object:nil];
    [center addObserver:self selector:@selector(applicationDidBecomeActive) name:UIApplicationDidBecomeActiveNotification object:nil];
    [center addObserver:self selector:@selector(applicationDidReceiveMemoryWarning) name:UIApplicationDidReceiveMemoryWarningNotification object:nil];

    [self dispatchEvent:SKETCH_WILL_INIT];
    crossDelegate->performInit();
    [self dispatchEvent:SKETCH_DID_INIT];
    
    // ---
    
    initialized = YES;
  }
  
  return initialized;
}

- (void) performUninit
{
  if (initialized)
  {
    DLOG(@"CrossBridge:performUninit");
    
    if (setup)
    {
      [self dispatchEvent:SKETCH_WILL_SHUTDOWN];
      crossDelegate->performShutdown();
      [self dispatchEvent:SKETCH_DID_SHUTDOWN];
      
      setup = NO;
    }
    
    if (viewController)
    {
      [viewController release];
      viewController = nil;
    }
    
    // ---
    
    system::bridge = nil;
    
    [self dispatchEvent:SKETCH_WILL_UNINIT];
    crossDelegate->performUninit();
    [self dispatchEvent:SKETCH_DID_UNINIT];
    
    delete crossDelegate;
    crossDelegate = nullptr;
    
    [[NSNotificationCenter defaultCenter] removeObserver:self];
    
    // ---
    
    initialized = NO;
  }
}

- (void) performSetup
{
  if (!setup && initialized)
  {
    DLOG(@"CrossBridge.performSetup");

    [self dispatchEvent:SKETCH_WILL_SETUP];
    crossDelegate->performSetup(WindowInfo([self windowSize], [self aaSamples], [self depthBits]));
    [self dispatchEvent:SKETCH_DID_SETUP];
    
    setup = YES;
  }
}

- (void) performResize
{
  auto size = [self windowSize];
  DLOG(@"CrossBridge:performResize - %dx%d", size.x, size.y);
  
  crossDelegate->performResize(size);
}

- (void) performUpdate
{
  crossDelegate->performUpdate();
}

- (void) performDraw
{
  crossDelegate->performDraw();
}

- (GLViewController*) viewController
{
  return viewController;
}

- (void) startWithReason:(int)reason
{
  switch (reason)
  {
    case REASON_VIEW_WILL_APPEAR:
    {
      DLOG(@"CrossBridge:startWithReason - SHOWN");
      
      [self dispatchEvent:VIEW_WILL_APPEAR];
      crossDelegate->handleEvent(CrossSketch::EVENT_SHOWN);
      
      break;
    }
      
    case REASON_APP_DID_BECOME_ACTIVE:
    {
      DLOG(@"CrossBridge:startWithReason - RESUMED");
      
      [self dispatchEvent:APP_DID_RESUME];
      crossDelegate->handleEvent(CrossSketch::EVENT_RESUMED);
      break;
    }
  }
}

- (void) stopWithReason:(int)reason
{
  switch (reason)
  {
    case REASON_VIEW_WILL_DISAPPEAR:
    {
      DLOG(@"CrossBridge:stopWithReason - HIDDEN");
      
      [self dispatchEvent:VIEW_WILL_DISAPPEAR];
      crossDelegate->handleEvent(CrossSketch::EVENT_HIDDEN);
      
      break;
    }
      
    case REASON_APP_WILL_RESIGN_ACTIVE:
    {
      DLOG(@"CrossBridge:stopWithReason - PAUSED");
      
      [self dispatchEvent:APP_WILL_PAUSE];
      crossDelegate->handleEvent(CrossSketch::EVENT_PAUSED);
      
      break;
    }
  }
}

#pragma mark ---------------------------------------- BRIDGE CALLBACKS ----------------------------------------

- (void) dispatchMessage:(int)what body:(NSString*)body
{
  if (self.listener)
  {
    [listener handleMessage:what body:body];
  }
  else
  {
    [self handleMessage:what body:body];
  }
}

- (void) dispatchEvent:(int)eventId
{
  if (self.listener)
  {
    [listener handleEvent:eventId];
  }
  else
  {
    [self handleEvent:eventId];
  }
}

#pragma mark ---------------------------------------- WINDOW-INFO ----------------------------------------

- (glm::vec2) windowSize;
{
  glm::vec2 size;
  auto view = viewController.view;

  if ([UIScreen.mainScreen respondsToSelector:@selector(nativeScale)]) // I.E. IOS 8+
  {
    size.x = view.frame.size.width;
    size.y = view.frame.size.height;
  }
  else
  {
    switch (viewController.interfaceOrientation)
    {
      case UIDeviceOrientationUnknown:
      case UIInterfaceOrientationPortrait:
      case UIInterfaceOrientationPortraitUpsideDown:
        size.x = view.frame.size.width;
        size.y = view.frame.size.height;
        break;

      case UIInterfaceOrientationLandscapeLeft:
      case UIInterfaceOrientationLandscapeRight:
        size.x = view.frame.size.height;
        size.y = view.frame.size.width;
        break;
    }
  }

  return size * float(view.contentScaleFactor);
}

- (int) aaSamples
{
  switch (viewController.glView.drawableMultisample)
  {
    case GLKViewDrawableMultisampleNone:
      return 0;
      
    case GLKViewDrawableMultisample4X:
      return 4;
  }
}

- (int) depthBits
{
  switch (viewController.glView.drawableDepthFormat)
  {
    case GLKViewDrawableDepthFormatNone:
      return 0;
      
    case GLKViewDrawableDepthFormat16:
      return 16;
      
    case GLKViewDrawableDepthFormat24:
      return 24;
  }
}

#pragma mark ---------------------------------------- ACCELEROMETER ----------------------------------------

- (void) accelerometer:(UIAccelerometer*)accelerometer didAccelerate:(UIAcceleration*)acceleration
{
  float ax;
  float ay;
  
  switch (viewController.interfaceOrientation)
  {
    case UIDeviceOrientationUnknown:
    case UIInterfaceOrientationPortrait:
      ax = +acceleration.x;
      ay = +acceleration.y;
      break;
      
    case UIInterfaceOrientationPortraitUpsideDown:
      ax = -acceleration.x;
      ay = -acceleration.y;
      break;
      
    case UIInterfaceOrientationLandscapeLeft:
      ax = +acceleration.y;
      ay = -acceleration.x;
      break;
      
    case UIInterfaceOrientationLandscapeRight:
      ax = -acceleration.y;
      ay = +acceleration.x;
      break;
  }
  
  crossDelegate->handleAcceleration(glm::vec3(ax, ay, acceleration.z));
}

#pragma mark ---------------------------------------- TOUCH ----------------------------------------

- (uint32_t) addTouchToMap:(UITouch*)touch
{
  uint32_t candidateId = 0;
  bool found = true;

  while (found)
  {
     candidateId++;
     found = false;

     for (auto &element : touchIdMap)
     {
       if (element.second == candidateId)
       {
         found = true;
         break;
       }
     }
  }

  touchIdMap.insert(make_pair(touch, candidateId));
  return candidateId;
}

- (void) removeTouchFromMap:(UITouch*)touch
{
  auto found = touchIdMap.find(touch);

  if (found != touchIdMap.end())
  {
     touchIdMap.erase(found);
  }
}

- (uint32_t) findTouchInMap:(UITouch*)touch
{
  auto found = touchIdMap.find(touch);

  if (found != touchIdMap.end())
  {
    return found->second;
  }

  return 0;
}

- (void) updateActiveTouches
{
  auto view = viewController.view;
  float scale = view.contentScaleFactor; // TODO: TEST ON IPHONE 6+

  vector<TouchEvent> activeTouches;

  for (auto &element : touchIdMap)
  {
    auto &touch = element.first;
    auto &touchId = element.second;

    CGPoint pt = [touch locationInView:view];
    CGPoint prevPt = [touch previousLocationInView:view];
    activeTouches.emplace_back(pt.x * scale, pt.y * scale, touchId);
  }
}

- (void) touchesBegan:(NSSet*)touches withEvent:(UIEvent*)event
{
  auto view = viewController.view;
  float scale = view.contentScaleFactor; // TODO: TEST ON IPHONE 6+

  vector<TouchEvent> touchList;

  for (UITouch *touch in touches)
  {
    CGPoint pt = [touch locationInView:view];
    CGPoint prevPt = [touch previousLocationInView:view];
    touchList.emplace_back(pt.x * scale, pt.y * scale, [self addTouchToMap:touch]);
  }

  [self updateActiveTouches];

  if (!touchList.empty())
  {
    crossDelegate->touchesBegan(touchList);
  }
}

- (void) touchesMoved:(NSSet*)touches withEvent:(UIEvent*)event
{
  auto view = viewController.view;
  float scale = view.contentScaleFactor; // TODO: TEST ON IPHONE 6+

  vector<TouchEvent> touchList;

  for (UITouch *touch in touches)
  {
    CGPoint pt = [touch locationInView:view];
    CGPoint prevPt = [touch previousLocationInView:view];
    touchList.emplace_back(pt.x * scale, pt.y * scale, [self findTouchInMap:touch]);
  }

  [self updateActiveTouches];

  if (!touchList.empty())
  {
     crossDelegate->touchesMoved(touchList);
  }
}

- (void) touchesEnded:(NSSet*)touches withEvent:(UIEvent*)event
{
  auto view = viewController.view;
  float scale = view.contentScaleFactor; // TODO: TEST ON IPHONE 6+

  vector<TouchEvent> touchList;

  for (UITouch *touch in touches)
  {
    CGPoint pt = [touch locationInView:view];
    CGPoint prevPt = [touch previousLocationInView:view];
    touchList.emplace_back(pt.x * scale, pt.y * scale, [self findTouchInMap:touch]);

    [self removeTouchFromMap:touch];
  }

  [self updateActiveTouches];

  if (!touchList.empty())
  {
    crossDelegate->touchesEnded(touchList);
  }
}

- (void) touchesCancelled:(NSSet*)touches withEvent:(UIEvent*)event
{
  [self touchesEnded:touches withEvent:event];
}

#pragma mark ---------------------------------------- SKETCH <-> BRIDGE COMMUNICATION ----------------------------------------

- (void) sendMessageToSketch:(int)what
{
  crossDelegate->messageFromBridge(what);
}

- (void) sendMessageToSketch:(int)what json:(id)json
{
  NSString *body = [NSString stringWithJSONObject:json];
  crossDelegate->messageFromBridge(what, [body UTF8String]);
}

- (void) sendMessageToSketch:(int)what body:(NSString*)body
{
  crossDelegate->messageFromBridge(what, [body UTF8String]);
}

// ---

- (void) handleMessage:(int)what body:(NSString*)body
{}

- (void) handleEvent:(int)eventId
{}

#pragma mark ---------------------------------------- NOTIFICATIONS ----------------------------------------

- (void) applicationWillTerminate
{
  if (initialized)
  {
    DLOG(@"CrossBridge:applicationWillTerminate");
    [self performUninit];
  }
}


- (void) applicationDidReceiveMemoryWarning
{
  if (initialized)
  {
    DLOG(@"CrossBridge:applicationDidReceiveMemoryWarning");
    crossDelegate->handleEvent(CrossSketch::EVENT_MEMORY_WARNING);
  }
}

- (void) applicationWillResignActive
{
  if (setup && !viewController.appeared)
  {
    DLOG(@"CrossBridge:applicationWillResignActive");
    crossDelegate->handleEvent(CrossSketch::EVENT_BACKGROUND); // TODO: TEST
  }
}

- (void) applicationDidBecomeActive
{
  if (setup && !viewController.appeared)
  {
    DLOG(@"CrossBridge:applicationDidBecomeActive");
    crossDelegate->handleEvent(CrossSketch::EVENT_FOREGROUND); // TODO: TEST
  }
}

@end
