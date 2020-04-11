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
          diagonal = 4.0f; // IPHONE 5, IPHONE 5S, IPHONE 5C, IPHONE SE
        }
        else if (magSize == 375 * 667)
        {
          diagonal = 4.7f; // IPHONE 6, IPHONE 6S, IPHONE 7, IPHONE 8
        }
        else if (magSize == 414 * 736)
        {
          diagonal = 5.5f; // IPHONE 6+, IPHONE 6S+, IPHONE 7+, IPHONE 8+
        }
        else if (magSize == 375 * 812)
        {
          diagonal = 5.8f; // IPHONE X, IPHONE XS, IPHONE 11 PRO
        }
        else if (magSize == 414 * 896)
        {
          if (getSystemInfo().isIPhoneMax)
          {
            diagonal = 6.5f; // IPHONE XS MAX, IPHONE 11 PRO MAX
          }
          else
          {
            diagonal = 6.1f; // IPHONE XR, IPHONE 11
          }
        }
        else if (magSize == 1024 * 768)
        {
          if (getSystemInfo().isIPadMini)
          {
            diagonal = 7.9f; // IPAD MINI
          }
          else
          {
            diagonal = 9.7f; // IPAD...
          }
        }
        else if (magSize == 1080 * 810)
        {
          diagonal = 10.2f; // IPAD 7
        }
        else if (magSize == 1112 * 834)
        {
          diagonal = 10.5f; // IPAD AIR 3, IPAD PRO 10.5"
        }
        else if (magSize == 1194 * 834)
        {
          diagonal = 11.0f; // IPAD PRO 11"
        }
        else if (magSize == 1366 * 1024)
        {
          diagonal = 12.9f; // IPAD PRO 12.9"
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
