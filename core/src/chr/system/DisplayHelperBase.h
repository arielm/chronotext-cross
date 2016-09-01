#pragma once

#include "chr/system/DisplayInfo.h"

namespace chr
{
  const DisplayInfo& getDisplayInfo();

  namespace display
  {
    class HelperBase
    {
    public:
      enum SizeFactor
      {
        SIZE_FACTOR_UNDEFINED,
        SIZE_FACTOR_PHONE_MINI,
        SIZE_FACTOR_PHONE,
        SIZE_FACTOR_PHONE_BIG,
        SIZE_FACTOR_TABLET_MINI,
        SIZE_FACTOR_TABLET,
        SIZE_FACTOR_TABLET_BIG,
      };
      
      static SizeFactor getSizeFactor(const Info &displayInfo);
    };
  }
}
