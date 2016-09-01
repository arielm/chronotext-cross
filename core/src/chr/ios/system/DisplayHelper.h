#pragma once

#include "chr/system/DisplayHelperBase.h"
#include "chr/system/SystemInfo.h"

namespace chr
{
  namespace display
  {
    class Helper : public HelperBase
    {
    public:
      /*
       * MUST BE CALLED AFTER (OR WITHIN) UIApplication::didFinishLaunchingWithOptions
       * OTHERWISE: SCREEN-ORIENTATION WON'T BE PROPERLY COMPUTED
       *
       * NOTES:
       * - IT IS (STILL) NOT HELPING ON iOS 5 (WHICH ALWAYS DETECTS "PORTRAIT" AT THIS STAGE)
       *   - PROBABLY SOLVABLE, BUT iOS 5 HAS ALMOST REACHED "END-OF-LIFE" THESE DAYS
       */
      static void setup(const system::InitInfo &initInfo);
      
      static void shutdown();
    };
  }
}
