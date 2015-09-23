/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012-2015, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE SIMPLIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#include "emscripten/system/DisplayHelper.h"
#include "cross/Context.h"

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
                intern::displayInfo = DisplayInfo::createWithDensity(fullscreenWidth, fullscreenHeight, 96 * pixelRatio); // XXX

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
