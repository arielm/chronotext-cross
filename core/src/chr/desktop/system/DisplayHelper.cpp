#include "chr/desktop/system/DisplayHelper.h"
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
        /*
         * WARNING: THE PRIMARY-MONITOR STRATEGY CURRENTLY USED IS NOT ALWAYS PICKING THE RIGHT MONITOR
         * E.G. WHEN A RETINA MACBOOK IS CONNECTED TO AN ADDITIONAL NON-RETINA MONITOR
         *
         * IT WOULD HAVE BEEN BETTER TO RELY ON glfwGetWindowMonitor(initInfo.window), BUT IT CRASHES
         */
        GLFWmonitor* monitor = glfwGetPrimaryMonitor();
        const GLFWvidmode *mode = glfwGetVideoMode(monitor);

        int widthMM, heightMM;
        glfwGetMonitorPhysicalSize(monitor, &widthMM, &heightMM);
        const double dpi = mode->width / (widthMM / 25.4);

        intern::displayInfo = DisplayInfo::createWithDensity(mode->width, mode->height, dpi);

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
