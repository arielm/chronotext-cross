#include "chr/win/system/SystemManager.h"
#include "chr/cross/Context.h"

#include <windows.h>
#include <shlobj.h>

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

    fs::path Manager::getDocumentsFolder()
    {
      char buffer[MAX_PATH];
      SHGetFolderPathA(0, CSIDL_MYDOCUMENTS, NULL, SHGFP_TYPE_CURRENT, buffer);
      return fs::path(string(buffer));
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
