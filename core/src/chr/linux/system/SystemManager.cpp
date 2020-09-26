#include "chr/rpi64/system/SystemManager.h"
#include "chr/cross/Context.h"

using namespace std;

namespace chr
{
  namespace system
  {
    void Manager::setup(const InitInfo &initInfo)
    {
      updateInfo();

      LOGI << "SYSTEM INFO: " << info << endl; // LOG: VERBOSE
    }

    // ---

    void Manager::updateInfo()
    {
      ManagerBase::updateInfo();
    }
    
    array<int, 3> Manager::getOsVersion()
    {
      return {0, 0, 0}; // FIXME
    }
    
    string Manager::getOsVersionString()
    {
      return "0.0.0"; // FIXME
    }
    
    string Manager::getDeviceString()
    {
      return "???"; // FIXME
    }
  }
}
