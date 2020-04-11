#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>

#include "chr/ios/system/SystemManager.h"
#include "chr/cross/Context.h"

#include <sys/utsname.h>

#include <boost/algorithm/string.hpp>

using namespace std;

namespace chr
{
  namespace system
  {
    namespace manager
    {
      map<string, Info::Generation> machineGenerations;
      map<Info::Generation, string> generationNames;
      
      void updateGeneration();
    }
    
    // ---
    
    void Manager::setup(const InitInfo &initInfo)
    {
      updateInfo();

      LOGI << "SYSTEM INFO: " << info << endl; // LOG: VERBOSE
    }

    // ---

    void Manager::updateInfo()
    {
      /*
       * XXX: ORDER OF EXECUTION MATTERS!
       */
      
      manager::updateGeneration();
      
      info.model = getModel();
      info.machine = getMachine();
      info.generation = getGeneration();
      
      info.isPodTouch = isPodTouch();
      info.isIPhone = isIPhone();
      info.isIPhoneMax = isIPhoneMax();
      info.isIPad = isIPad();
      info.isIPadMini = isIPadMini();
      info.emulated = isSimulator();
      
      ManagerBase::updateInfo();
    }
    
    array<int, 3> Manager::getOsVersion()
    {
      NSArray *components = [UIDevice.currentDevice.systemVersion componentsSeparatedByString:@"."];
      
      int major = (components.count > 0) ? [components[0] intValue] : 0;
      int minor = (components.count > 1) ? [components[1] intValue] : 0;
      int patch = (components.count > 2) ? [components[2] intValue] : 0;
      
      return {major, minor, patch};
    }
    
    string Manager::getOsVersionString()
    {
      auto osVersion = getOsVersion();
      return utils::toString(osVersion[0]) + "." + utils::toString(osVersion[1]) + "." + utils::toString(osVersion[2]);
    }
    
    string Manager::getDeviceString()
    {
      auto it = manager::generationNames.find(info.generation);
      
      if (it != manager::generationNames.end())
      {
        return it->second;
      }
      
      return info.model + " [" + info.machine + "]";
    }
    
    /*
     * SHOULD RETURN ONE OF THE FOLLOWING:
     *
     * - iPod touch
     * - iPhone
     * - iPhone Simulator
     * - iPad
     * - iPad Simulator
     */
    string Manager::getModel()
    {
      NSString *model = UIDevice.currentDevice.model;
      return [model UTF8String];
    }
    
    string Manager::getMachine()
    {
      struct utsname systemInfo;
      uname(&systemInfo);
      
      return string(systemInfo.machine);
    }
    
    Info::Generation Manager::getGeneration()
    {
      auto it = manager::machineGenerations.find(info.machine);
      
      if (it != manager::machineGenerations.end())
      {
        return it->second;
      }
      
      return Info::GENERATION_UNDEFINED;
    }
    
    bool Manager::isPodTouch()
    {
      return boost::starts_with(info.model , "iPod touch");
    }
    
    bool Manager::isIPhone()
    {
      return boost::starts_with(info.model , "iPhone");
    }
    
    bool Manager::isIPhoneMax()
    {
      switch (info.generation)
      {
        case Info::GENERATION_IPHONE_XS_MAX:
        case Info::GENERATION_IPHONE_11_PRO_MAX:
          return true;
          
        default:
          return false;
      }
    }

    bool Manager::isIPad()
    {
      return boost::starts_with(info.model , "iPad");
    }

    bool Manager::isIPadMini()
    {
      switch (info.generation)
      {
        case Info::GENERATION_IPAD_MINI_1G:
        case Info::GENERATION_IPAD_MINI_2:
        case Info::GENERATION_IPAD_MINI_3:
        case Info::GENERATION_IPAD_MINI_4:
        case Info::GENERATION_IPAD_MINI_5:
          return true;
          
        default:
          return false;
      }
    }
    
    bool Manager::isSimulator()
    {
      return boost::ends_with(info.model , "Simulator");
    }
    
    // ---
    
    void manager::updateGeneration()
    {
      if (machineGenerations.empty())
      {
        machineGenerations["iPad1,1"] = Info::GENERATION_IPAD;
        
        machineGenerations["iPad2,1"] = Info::GENERATION_IPAD_2;
        machineGenerations["iPad2,2"] = Info::GENERATION_IPAD_2;
        machineGenerations["iPad2,3"] = Info::GENERATION_IPAD_2;
        machineGenerations["iPad2,4"] = Info::GENERATION_IPAD_2;
        
        machineGenerations["iPad3,1"] = Info::GENERATION_IPAD_3;
        machineGenerations["iPad3,2"] = Info::GENERATION_IPAD_3;
        machineGenerations["iPad3,3"] = Info::GENERATION_IPAD_3;
        
        machineGenerations["iPad3,4"] = Info::GENERATION_IPAD_4;
        machineGenerations["iPad3,5"] = Info::GENERATION_IPAD_4;
        machineGenerations["iPad3,6"] = Info::GENERATION_IPAD_4;

        machineGenerations["iPad6,11"] = Info::GENERATION_IPAD_5;
        machineGenerations["iPad6,12"] = Info::GENERATION_IPAD_5;

        machineGenerations["iPad7,5"] = Info::GENERATION_IPAD_6;
        machineGenerations["iPad7,6"] = Info::GENERATION_IPAD_6;

        machineGenerations["iPad7,11"] = Info::GENERATION_IPAD_7;
        machineGenerations["iPad7,12"] = Info::GENERATION_IPAD_7;
        
        machineGenerations["iPad4,1"] = Info::GENERATION_IPAD_AIR;
        machineGenerations["iPad4,2"] = Info::GENERATION_IPAD_AIR;
        machineGenerations["iPad4,3"] = Info::GENERATION_IPAD_AIR;
        
        machineGenerations["iPad5,3"] = Info::GENERATION_IPAD_AIR_2;
        machineGenerations["iPad5,4"] = Info::GENERATION_IPAD_AIR_2;

        machineGenerations["iPad11,3"] = Info::GENERATION_IPAD_AIR_3;
        machineGenerations["iPad11,4"] = Info::GENERATION_IPAD_AIR_3;
        
        // ---

        machineGenerations["iPad6,7"] = Info::GENERATION_IPAD_PRO_12_9;
        machineGenerations["iPad6,8"] = Info::GENERATION_IPAD_PRO_12_9;

        machineGenerations["iPad6,3"] = Info::GENERATION_IPAD_PRO_9_7;
        machineGenerations["iPad6,4"] = Info::GENERATION_IPAD_PRO_9_7;

        machineGenerations["iPad7,1"] = Info::GENERATION_IPAD_PRO_12_9_2;
        machineGenerations["iPad7,2"] = Info::GENERATION_IPAD_PRO_12_9_2;

        machineGenerations["iPad7,3"] = Info::GENERATION_IPAD_PRO_10_5;
        machineGenerations["iPad7,4"] = Info::GENERATION_IPAD_PRO_10_5;

        machineGenerations["iPad8,1"] = Info::GENERATION_IPAD_PRO_11;
        machineGenerations["iPad8,2"] = Info::GENERATION_IPAD_PRO_11;
        machineGenerations["iPad8,3"] = Info::GENERATION_IPAD_PRO_11;
        machineGenerations["iPad8,4"] = Info::GENERATION_IPAD_PRO_11;

        machineGenerations["iPad8,5"] = Info::GENERATION_IPAD_PRO_12_9_3;
        machineGenerations["iPad8,6"] = Info::GENERATION_IPAD_PRO_12_9_3;
        machineGenerations["iPad8,7"] = Info::GENERATION_IPAD_PRO_12_9_3;
        machineGenerations["iPad8,8"] = Info::GENERATION_IPAD_PRO_12_9_3;

        // ---
        
        machineGenerations["iPad2,5"] = Info::GENERATION_IPAD_MINI_1G;
        machineGenerations["iPad2,6"] = Info::GENERATION_IPAD_MINI_1G;
        machineGenerations["iPad2,7"] = Info::GENERATION_IPAD_MINI_1G;
        
        machineGenerations["iPad4,4"] = Info::GENERATION_IPAD_MINI_2;
        machineGenerations["iPad4,5"] = Info::GENERATION_IPAD_MINI_2;
        machineGenerations["iPad4,6"] = Info::GENERATION_IPAD_MINI_2;
        
        machineGenerations["iPad4,7"] = Info::GENERATION_IPAD_MINI_3;
        machineGenerations["iPad4,8"] = Info::GENERATION_IPAD_MINI_3;
        machineGenerations["iPad4,9"] = Info::GENERATION_IPAD_MINI_3;

        machineGenerations["iPad5,1"] = Info::GENERATION_IPAD_MINI_4;
        machineGenerations["iPad5,2"] = Info::GENERATION_IPAD_MINI_4;
        
        machineGenerations["iPad11,1"] = Info::GENERATION_IPAD_MINI_5;
        machineGenerations["iPad11,2"] = Info::GENERATION_IPAD_MINI_5;

        // ---
        
        machineGenerations["iPhone2,1"] = Info::GENERATION_IPHONE_3GS;
        
        machineGenerations["iPhone3,1"] = Info::GENERATION_IPHONE_4;
        machineGenerations["iPhone3,2"] = Info::GENERATION_IPHONE_4;
        machineGenerations["iPhone3,3"] = Info::GENERATION_IPHONE_4;
        machineGenerations["iPhone4,1"] = Info::GENERATION_IPHONE_4S;
        
        machineGenerations["iPhone5,1"] = Info::GENERATION_IPHONE_5;
        machineGenerations["iPhone5,2"] = Info::GENERATION_IPHONE_5;
        machineGenerations["iPhone5,3"] = Info::GENERATION_IPHONE_5C;
        machineGenerations["iPhone5,4"] = Info::GENERATION_IPHONE_5C;
        machineGenerations["iPhone6,1"] = Info::GENERATION_IPHONE_5S;
        machineGenerations["iPhone6,2"] = Info::GENERATION_IPHONE_5S;

        machineGenerations["iPhone7,2"] = Info::GENERATION_IPHONE_6;
        machineGenerations["iPhone7,1"] = Info::GENERATION_IPHONE_6_PLUS;
        machineGenerations["iPhone8,1"] = Info::GENERATION_IPHONE_6S;
        machineGenerations["iPhone8,2"] = Info::GENERATION_IPHONE_6S_PLUS;

        machineGenerations["iPhone8,4"] = Info::GENERATION_IPHONE_SE;

        machineGenerations["iPhone9,1"] = Info::GENERATION_IPHONE_7;
        machineGenerations["iPhone9,3"] = Info::GENERATION_IPHONE_7;
        machineGenerations["iPhone9,2"] = Info::GENERATION_IPHONE_7_PLUS;
        machineGenerations["iPhone9,4"] = Info::GENERATION_IPHONE_7_PLUS;

        machineGenerations["iPhone10,1"] = Info::GENERATION_IPHONE_8;
        machineGenerations["iPhone10,4"] = Info::GENERATION_IPHONE_8;
        machineGenerations["iPhone10,2"] = Info::GENERATION_IPHONE_8_PLUS;
        machineGenerations["iPhone10,5"] = Info::GENERATION_IPHONE_8_PLUS;

        machineGenerations["iPhone10,3"] = Info::GENERATION_IPHONE_X;
        machineGenerations["iPhone10,6"] = Info::GENERATION_IPHONE_X;
        machineGenerations["iPhone11,8"] = Info::GENERATION_IPHONE_XR;
        machineGenerations["iPhone11,2"] = Info::GENERATION_IPHONE_XS;
        machineGenerations["iPhone11,6"] = Info::GENERATION_IPHONE_XS_MAX;
        machineGenerations["iPhone11,4"] = Info::GENERATION_IPHONE_XS_MAX;

        machineGenerations["iPhone12,1"] = Info::GENERATION_IPHONE_11;
        machineGenerations["iPhone12,3"] = Info::GENERATION_IPHONE_11_PRO;
        machineGenerations["iPhone12,5"] = Info::GENERATION_IPHONE_11_PRO_MAX;
        
        // ---
        
        machineGenerations["iPod3,1"] = Info::GENERATION_IPOD_TOUCH_3G;
        machineGenerations["iPod4,1"] = Info::GENERATION_IPOD_TOUCH_4G;
        machineGenerations["iPod5,1"] = Info::GENERATION_IPOD_TOUCH_5G;
        machineGenerations["iPod7,1"] = Info::GENERATION_IPOD_TOUCH_6G;
        machineGenerations["iPod9,1"] = Info::GENERATION_IPOD_TOUCH_7G;
      }
      
      if (generationNames.empty())
      {
        generationNames[Info::GENERATION_IPAD] = "iPad";
        generationNames[Info::GENERATION_IPAD_2] = "iPad 2";
        generationNames[Info::GENERATION_IPAD_3] = "iPad 3";
        generationNames[Info::GENERATION_IPAD_4] = "iPad 4";
        generationNames[Info::GENERATION_IPAD_5] = "iPad 5";
        generationNames[Info::GENERATION_IPAD_6] = "iPad 6";
        generationNames[Info::GENERATION_IPAD_7] = "iPad 7";
        generationNames[Info::GENERATION_IPAD_AIR] = "iPad Air";
        generationNames[Info::GENERATION_IPAD_AIR_2] = "iPad Air 2";
        generationNames[Info::GENERATION_IPAD_AIR_3] = "iPad Air 3";

        generationNames[Info::GENERATION_IPAD_PRO_12_9] = "iPad Pro 12.9-inch";
        generationNames[Info::GENERATION_IPAD_PRO_9_7] = "iPad Pro 9.7-inch";
        generationNames[Info::GENERATION_IPAD_PRO_12_9_2] = "iPad Pro 12.9-inch 2";
        generationNames[Info::GENERATION_IPAD_PRO_10_5] = "iPad Pro 10.5-inch";
        generationNames[Info::GENERATION_IPAD_PRO_11] = "iPad Pro 11-inch";
        generationNames[Info::GENERATION_IPAD_PRO_12_9_3] = "iPad Pro 12.9-inch 3";
        
        generationNames[Info::GENERATION_IPAD_MINI_1G] = "iPad mini 1G";
        generationNames[Info::GENERATION_IPAD_MINI_2] = "iPad mini 2";
        generationNames[Info::GENERATION_IPAD_MINI_3] = "iPad mini 3";
        generationNames[Info::GENERATION_IPAD_MINI_4] = "iPad mini 4";
        generationNames[Info::GENERATION_IPAD_MINI_5] = "iPad mini 5";
        
        generationNames[Info::GENERATION_IPHONE_3GS] = "iPhone 3GS";
        generationNames[Info::GENERATION_IPHONE_4] = "iPhone 4";
        generationNames[Info::GENERATION_IPHONE_4S] = "iPhone 4S";
        generationNames[Info::GENERATION_IPHONE_5] = "iPhone 5";
        generationNames[Info::GENERATION_IPHONE_5C] = "iPhone 5c";
        generationNames[Info::GENERATION_IPHONE_5S] = "iPhone 5s";
        generationNames[Info::GENERATION_IPHONE_6] = "iPhone 6";
        generationNames[Info::GENERATION_IPHONE_6_PLUS] = "iPhone 6 Plus";
        generationNames[Info::GENERATION_IPHONE_6S] = "iPhone 6s";
        generationNames[Info::GENERATION_IPHONE_6S_PLUS] = "iPhone 6s Plus";
        generationNames[Info::GENERATION_IPHONE_SE] = "iPhone SE";
        generationNames[Info::GENERATION_IPHONE_7] = "iPhone 7";
        generationNames[Info::GENERATION_IPHONE_7_PLUS] = "iPhone 7 Plus";
        generationNames[Info::GENERATION_IPHONE_8] = "iPhone 8";
        generationNames[Info::GENERATION_IPHONE_8_PLUS] = "iPhone Plus";
        generationNames[Info::GENERATION_IPHONE_X] = "iPhone X";
        generationNames[Info::GENERATION_IPHONE_XR] = "iPhone XR";
        generationNames[Info::GENERATION_IPHONE_XS] = "iPhone XS";
        generationNames[Info::GENERATION_IPHONE_XS_MAX] = "iPhone XS Max";
        generationNames[Info::GENERATION_IPHONE_11] = "iPhone 11";
        generationNames[Info::GENERATION_IPHONE_11_PRO] = "iPhone Pro";
        generationNames[Info::GENERATION_IPHONE_11_PRO_MAX] = "iPhone Pro Max";
        
        generationNames[Info::GENERATION_IPOD_TOUCH_3G] = "iPod touch 3G";
        generationNames[Info::GENERATION_IPOD_TOUCH_4G] = "iPod touch 4G";
        generationNames[Info::GENERATION_IPOD_TOUCH_5G] = "iPod touch 5G";
        generationNames[Info::GENERATION_IPOD_TOUCH_6G] = "iPod touch 6G";
        generationNames[Info::GENERATION_IPOD_TOUCH_7G] = "iPod touch 7G";
      }
    }
  }
}
