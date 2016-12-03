#pragma once

#include "chr/system/SystemManagerBase.h"

namespace chr
{
  namespace system
  {
    namespace android
    {
      /*
       * THE SAME PROPERTIES QUERIABLE VIA ADB, E.G.
       * adb shell getprop ro.product.manufacturer
       */
      const std::string getProperty(const char *name);
    }
    
    class Manager : public ManagerBase
    {
    public:
      void setup(const InitInfo &initInfo) final;
      fs::path getDocumentsFolder() final;

    protected:
      void updateInfo() final;

      std::array<int, 3> getOsVersion() final;
      std::string getOsVersionString() final;
      std::string getDeviceString() final;
      
      int getSdkVersion();
      std::string getModel();
      std::string getManufacturer();
    };
  }
}
