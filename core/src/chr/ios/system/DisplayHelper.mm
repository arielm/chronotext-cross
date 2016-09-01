#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>

#include "chr/ios/system/DisplayHelper.h"
#include "chr/cross/Context.h"

using namespace std;

namespace chr
{
  namespace display
  {
    namespace intern
    {
      DisplayInfo displayInfo;
      bool setup = false;
    }
    
    // ---
    
    void Helper::setup(const system::InitInfo &initInfo)
    {
      if (!intern::setup)
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
        
        auto bounds = UIScreen.mainScreen.bounds;
        auto orientation = UIApplication.sharedApplication.statusBarOrientation;
        float contentScale;

        // ---
        
        glm::vec2 baseSize;
        
        if ([UIScreen.mainScreen respondsToSelector:@selector(nativeScale)]) // I.E. IOS 8+
        {
          /*
           * WARNING: SIMULATORS DO NOT HANDLE nativeScale LIKE REAL DEVICES
           *
           * E.G. ON IPHONE 6+:
           * - DEVICE: ~2.609 (3 * 1920 / 2208)
           * - SIMULATOR: 3
           */
          contentScale = UIScreen.mainScreen.nativeScale;

          baseSize.x = bounds.size.width;
          baseSize.y = bounds.size.height;
        }
        else
        {
          contentScale = UIScreen.mainScreen.scale;
          
          switch (orientation)
          {
            case UIDeviceOrientationUnknown:
            case UIInterfaceOrientationPortrait:
            case UIInterfaceOrientationPortraitUpsideDown:
              baseSize.x = bounds.size.width;
              baseSize.y = bounds.size.height;
              break;
              
            case UIInterfaceOrientationLandscapeLeft:
            case UIInterfaceOrientationLandscapeRight:
              baseSize.x = bounds.size.height;
              baseSize.y = bounds.size.width;
              break;
          }
        }
        
        // ---
        
        float diagonal = 0;
        int magSize = baseSize.x * baseSize.y;
        
        if (magSize == 320 * 480)
        {
          diagonal = 3.54f; // IPHONE 3GS OR 4
        }
        else if (magSize == 320 * 568)
        {
          if (contentScale == 2)
          {
            diagonal = 4.00f; // IPHONE 5
          }
          else
          {
            diagonal = 4.70f; // IPHONE 6
          }
        }
        else if (magSize == 375 * 667)
        {
          if (contentScale == 2)
          {
            diagonal = 4.70f; // IPHONE 6
          }
          else
          {
            diagonal = 5.50f; // IPHONE 6+
          }
        }
        else if (magSize == 414 * 736)
        {
          diagonal = 5.50f; // IPHONE 6+
        }
        else if (magSize == 1024 * 768)
        {
          if (getSystemInfo().isIPadMini)
          {
            diagonal = 7.90f; // IPAD MINI
          }
          else
          {
            diagonal = 9.70f; // IPAD
          }
        }
        
        intern::displayInfo = DisplayInfo::createWithDiagonal(baseSize.x, baseSize.y, diagonal, contentScale);
        
        // ---
        
        LOGI << "DISPLAY INFO: " << intern::displayInfo << endl; // LOG: VERBOSE
        
        intern::setup = true;
      }
    }
    
    void Helper::shutdown()
    {
      intern::setup = false;
    }
  }
  
  const DisplayInfo& getDisplayInfo()
  {
    assert(display::intern::setup);
    return display::intern::displayInfo;
  }
}
