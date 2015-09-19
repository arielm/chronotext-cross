/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012-2015, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE SIMPLIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#include "system/DisplayHelperBase.h"

namespace chr
{
    namespace display
    {
        HelperBase::SizeFactor HelperBase::getSizeFactor(const Info &displayInfo)
        {
            float diagonal = displayInfo.diagonal;
            
            if (diagonal > 0)
            {
                if (diagonal < 3.33f)
                {
                    return SIZE_FACTOR_PHONE_MINI;
                }
                if (diagonal < 4.5f)
                {
                    return SIZE_FACTOR_PHONE;
                }
                if (diagonal < 6.5f)
                {
                    return SIZE_FACTOR_PHONE_BIG;
                }
                if (diagonal < 9)
                {
                    return SIZE_FACTOR_TABLET_MINI;
                }
                if (diagonal < 11.5f)
                {
                    return SIZE_FACTOR_TABLET;
                }
                
                return SIZE_FACTOR_TABLET_BIG;
            }
            
            return SIZE_FACTOR_UNDEFINED;
        }
    }
}
