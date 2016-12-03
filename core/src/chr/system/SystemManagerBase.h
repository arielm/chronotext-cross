#pragma once

#include "chr/system/SystemInfo.h"

namespace chr
{
  SystemInfo getSystemInfo();
  
  namespace system
  {
    const std::string& platformName();

    // ---
    
    class ManagerBase
    {
    public:
      virtual void setup(const InitInfo &initInfo) = 0;
      virtual void shutdown() {}

      const Info& getInfo() const;
      virtual fs::path getDocumentsFolder() = 0;

    protected:
      Info info;
      
      virtual void updateInfo();

      virtual std::array<int, 3> getOsVersion() = 0;
      virtual std::string getOsVersionString() = 0;
      virtual std::string getDeviceString() = 0;
    };
  }
}
