#include "chr/emscripten/system/DisplayHelper.h"
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
        double pixelRatio = emscripten_get_device_pixel_ratio();
        double screenWidth = EM_ASM_DOUBLE_V("return screen.width");
        double screenHeight =  EM_ASM_DOUBLE_V("return screen.height");

        int fullscreenWidth = int(pixelRatio * screenWidth + 0.5);
        int fullscreenHeight = int(pixelRatio * screenHeight + 0.5);
        intern::displayInfo = DisplayInfo::createWithDensity(fullscreenWidth, fullscreenHeight, 96 * pixelRatio, pixelRatio); // XXX: No way to get the density on browsers

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
