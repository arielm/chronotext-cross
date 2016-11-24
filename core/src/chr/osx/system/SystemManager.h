#pragma once

#include "chr/system/SystemManagerBase.h"

namespace chr
{
  namespace system
  {
    class Manager : public ManagerBase
    {
    public:
      void setup(const InitInfo &initInfo) final;

    protected:
      void updateInfo() final;
      
      std::array<int, 3> getOsVersion() final;
      std::string getOsVersionString() final;
      std::string getDeviceString() final;
    };
  }
}
