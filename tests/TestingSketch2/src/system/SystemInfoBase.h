/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012-2014, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE SIMPLIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#pragma once

#include "cross/WindowInfo.h"

#include <boost/asio.hpp>

#include <array>

namespace chr
{
    namespace system
    {
        struct SetupInfo
        {
            boost::asio::io_service *io_service;
            WindowInfo windowInfo;
            
            SetupInfo() = default;
            
            SetupInfo(boost::asio::io_service &io_service, const WindowInfo &windowInfo)
            :
            io_service(&io_service),
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
