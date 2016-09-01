#include "chr/android/system/SystemManager.h"
#include "chr/cross/Context.h"

#include <sys/system_properties.h>
#include <sys/utsname.h>

using namespace std;

namespace chr
{
  namespace system
  {
    namespace android
    {
      /*
       * FIXME: WILL NOT WORK ANYMORE, STARTING FROM ANDROID LOLLIPOP
       *
       * HINT: http://stackoverflow.com/questions/26722040/replacement-for-system-property-get-in-android-l-ndk
       */
      const string getProperty(const char *name)
      {
        static char tmp[256];
        auto len = __system_property_get(name, tmp);
        
        return string(tmp, len);
      }
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
      
      info.model = getModel();
      info.manufacturer = getManufacturer();
      
      ManagerBase::updateInfo();
    }
    
    array<int, 3> Manager::getOsVersion()
    {
      auto releaseString = android::getProperty("ro.build.version.release");
      auto components = utils::split(releaseString, '.');
      
      int major = (components.size() > 0) ? atoi(components[0].data()) : 0;
      int minor = (components.size() > 1) ? atoi(components[1].data()) : 0;
      int patch = (components.size() > 2) ? atoi(components[2].data()) : 0;
      
      return {major, minor, patch};
    }
    
    string Manager::getOsVersionString()
    {
      auto releaseString = android::getProperty("ro.build.version.release");
      auto sdkVersion = getSdkVersion();
      
      if (sdkVersion)
      {
        return releaseString + " [" + utils::toString(sdkVersion) + "]";
      }
      else
      {
        releaseString;
      }
    }
    
    string Manager::getDeviceString()
    {
      return info.model + " [" + info.manufacturer + "]";
    }
    
    int Manager::getSdkVersion()
    {
      auto sdkString = android::getProperty("ro.build.version.sdk");
      return sdkString.empty() ? 0 : atoi(sdkString.data());
    }

    string Manager::getModel()
    {
      return android::getProperty("ro.product.model");
    }
    
    string Manager::getManufacturer()
    {
      return android::getProperty("ro.product.manufacturer");
    }
  }
}
