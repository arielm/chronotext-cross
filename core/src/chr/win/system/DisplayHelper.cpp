#include "chr/win/system/DisplayHelper.h"
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
        HWND desktopWindow = GetDesktopWindow();

        RECT desktopRect;
        GetWindowRect(desktopWindow, &desktopRect);
        int width = desktopRect.right;
        int height = desktopRect.bottom;

        HDC dc = GetDC(desktopWindow);
        int widthMM = GetDeviceCaps(dc, HORZSIZE);
        double dpi = width / (widthMM / 25.4);

        intern::displayInfo = DisplayInfo::createWithDensity(width, height, dpi);

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
