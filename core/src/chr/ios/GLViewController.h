#import <GLKit/GLKit.h>

OBJC_EXPORT NSString* GLViewControllerRenderingAPIKey;
OBJC_EXPORT NSString* GLViewControllerPreferredFramesPerSecondKey;
OBJC_EXPORT NSString* GLViewControllerMultipleTouchEnabledKey;
OBJC_EXPORT NSString* GLViewControllerInterfaceOrientationMaskKey;
OBJC_EXPORT NSString* GLViewControllerColorFormatKey;
OBJC_EXPORT NSString* GLViewControllerDepthFormatKey;
OBJC_EXPORT NSString* GLViewControllerStencilFormatKey;
OBJC_EXPORT NSString* GLViewControllerMultisampleKey;

@class CrossBridge;

enum
{
  REASON_VIEW_WILL_APPEAR,
  REASON_VIEW_WILL_DISAPPEAR,
  REASON_APP_DID_BECOME_ACTIVE,
  REASON_APP_WILL_RESIGN_ACTIVE,
};

@interface GLViewController : GLKViewController
{
  CrossBridge *crossBridge;
  GLKView *glView;
}

@property (nonatomic, readonly) CrossBridge *crossBridge;
@property (nonatomic, readonly) GLKView *glView;
@property (nonatomic, readonly) BOOL appeared;

- (id) initWithBridge:(CrossBridge*)bridge properties:(NSDictionary*)properties;

@end
