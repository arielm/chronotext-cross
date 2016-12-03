#if (!defined(__OBJC__))
  #error THIS FILE MUST BE COMPILED AS OBJECTIVE-C++
#endif

#import <Foundation/Foundation.h>

#include "chr/osx/system/SystemManager.h"
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

    fs::path Manager::getDocumentsFolder()
    {
      NSString *path = [NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES) objectAtIndex:0];
      return fs::path([path cStringUsingEncoding:NSUTF8StringEncoding]);
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
