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
      fs::path getDocumentsFolder() final;

    protected:
      void updateInfo() final;
      
      std::array<int, 3> getOsVersion() final;
      std::string getOsVersionString() final;
      std::string getDeviceString() final;
      
      std::string getModel();
      std::string getMachine();
      Info::Generation getGeneration();
      
      bool isPodTouch();
      bool isIPhone();
      bool isIPad();
      bool isIPadMini();
      bool isSimulator();
    };
  }
}
