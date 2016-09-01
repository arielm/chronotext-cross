#pragma once

#include "chr/cross/WindowInfo.h"

#include <array>

namespace chr
{
  namespace system
  {
    struct SetupInfo
    {
      WindowInfo windowInfo;
      
      SetupInfo() = default;
      
      SetupInfo(const WindowInfo &windowInfo)
      :
      windowInfo(windowInfo)
      {}
    };
    
    class InfoBase
    {
    public:
      std::string platformString;

      std::array<int, 3> osVersion {};
      std::string osVersionString;
      
      std::string deviceString;
      bool emulated = false;

      friend std::ostream& operator<<(std::ostream &lhs, const InfoBase &rhs)
      {
        lhs << "{";
        
        lhs
        << "platform: " << rhs.platformString
        << ", os-version: " << rhs.osVersionString;

        if (!rhs.deviceString.empty())
        {
          lhs << ", device: " << rhs.deviceString;
        }

        return (lhs << "}");
      }
    };
  }
}
