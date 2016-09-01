#include "chr/android/system/DisplayHelper.h"
#include "chr/cross/Context.h"

using namespace std;

namespace chr
{
  namespace display
  {
    namespace intern
    {
      DisplayInfo displayInfo;
      bool setup = false;
    }
    
    // ---
    
    void Helper::setup(const system::InitInfo &initInfo)
    {
      if (!intern::setup)
      {
        intern::displayInfo = DisplayInfo::createWithDensity(initInfo.displaySize.x, initInfo.displaySize.y, initInfo.displayDensity);
        
        // ---
        
        LOGI << "DISPLAY INFO: " << intern::displayInfo << endl; // LOG: VERBOSE
        
        intern::setup = true;
      }
    }
    
    void Helper::shutdown()
    {
      intern::setup = false;
    }
  }
  
  const DisplayInfo& getDisplayInfo()
  {
    assert(display::intern::setup);
    return display::intern::displayInfo;
  }
}
