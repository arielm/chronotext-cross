#pragma once

#include "chr/system/SystemInfoBase.h"

/*
 * REFERENCE: http://theiphonewiki.com/wiki/Models
 */

namespace chr
{
  namespace system
  {
    struct InitInfo
    {};
    
    class Info : public InfoBase
    {
    public:
      enum Generation
      {
        GENERATION_UNDEFINED,
        
        GENERATION_IPAD,
        GENERATION_IPAD_2,
        GENERATION_IPAD_3,
        GENERATION_IPAD_4,
        GENERATION_IPAD_5,
        GENERATION_IPAD_6,
        GENERATION_IPAD_7,
        GENERATION_IPAD_8,
        GENERATION_IPAD_9,
        GENERATION_IPAD_10,
        GENERATION_IPAD_AIR,
        GENERATION_IPAD_AIR_2,
        GENERATION_IPAD_AIR_3,
        GENERATION_IPAD_AIR_4,
        GENERATION_IPAD_AIR_5,

        GENERATION_IPAD_PRO_12_9,
        GENERATION_IPAD_PRO_9_7,
        GENERATION_IPAD_PRO_12_9_2,
        GENERATION_IPAD_PRO_10_5,
        GENERATION_IPAD_PRO_11,
        GENERATION_IPAD_PRO_12_9_3,
        GENERATION_IPAD_PRO_11_4,
        GENERATION_IPAD_PRO_12_9_4,
        GENERATION_IPAD_PRO_11_5,
        GENERATION_IPAD_PRO_12_9_5,
        GENERATION_IPAD_PRO_11_6,
        GENERATION_IPAD_PRO_12_9_6,
        
        GENERATION_IPAD_MINI_1G,
        GENERATION_IPAD_MINI_2,
        GENERATION_IPAD_MINI_3,
        GENERATION_IPAD_MINI_4,
        GENERATION_IPAD_MINI_5,
        GENERATION_IPAD_MINI_6,
        
        GENERATION_IPHONE_3GS,
        GENERATION_IPHONE_4,
        GENERATION_IPHONE_4S,
        GENERATION_IPHONE_5,
        GENERATION_IPHONE_5C,
        GENERATION_IPHONE_5S,
        GENERATION_IPHONE_6,
        GENERATION_IPHONE_6_PLUS,
        GENERATION_IPHONE_6S,
        GENERATION_IPHONE_6S_PLUS,
        GENERATION_IPHONE_SE,
        GENERATION_IPHONE_7,
        GENERATION_IPHONE_7_PLUS,
        GENERATION_IPHONE_8,
        GENERATION_IPHONE_8_PLUS,
        GENERATION_IPHONE_X,
        GENERATION_IPHONE_XR,
        GENERATION_IPHONE_XS,
        GENERATION_IPHONE_XS_MAX,
        GENERATION_IPHONE_11,
        GENERATION_IPHONE_11_PRO,
        GENERATION_IPHONE_11_PRO_MAX,
        GENERATION_IPHONE_SE_2,
        GENERATION_IPHONE_12,
        GENERATION_IPHONE_12_PRO,
        GENERATION_IPHONE_12_PRO_MAX,
        GENERATION_IPHONE_12_MINI,
        GENERATION_IPHONE_13,
        GENERATION_IPHONE_13_PRO,
        GENERATION_IPHONE_13_PRO_MAX,
        GENERATION_IPHONE_13_MINI,
        GENERATION_IPHONE_SE_3,
        GENERATION_IPHONE_14,
        GENERATION_IPHONE_14_PRO,
        GENERATION_IPHONE_14_PRO_MAX,
        GENERATION_IPHONE_14_PLUS,
        GENERATION_IPHONE_15,
        GENERATION_IPHONE_15_PRO,
        GENERATION_IPHONE_15_PRO_MAX,
        GENERATION_IPHONE_15_PLUS,
        
        GENERATION_IPOD_TOUCH_3G,
        GENERATION_IPOD_TOUCH_4G,
        GENERATION_IPOD_TOUCH_5G,
        GENERATION_IPOD_TOUCH_6G,
        GENERATION_IPOD_TOUCH_7G
      };
      
      std::string model;
      std::string machine;
      Generation generation = GENERATION_UNDEFINED;
      
      bool isPodTouch = false;
      bool isIPhone = false;
      bool isIPhoneMax = false;
      bool isIPhoneMini = false;
      bool isIPad = false;
      bool isIPadMini = false;
      bool isSimulator = false;
    };
  }
}
