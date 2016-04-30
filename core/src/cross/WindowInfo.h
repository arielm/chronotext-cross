/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012-2015, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE SIMPLIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#pragma once

#include "glm.h"

#include <iostream>

namespace chr
{
    struct WindowInfo
    {
        union
        {
          glm::vec2 size;
          struct { float width, height; };
        };

        int aaSamples;
        int depthBits;

        static WindowInfo create(int width, int height, int aaSamples = 0, int depthBits = 0)
        {
            return WindowInfo(width, height, aaSamples, depthBits);
        }
        
        WindowInfo(const glm::ivec2 &size = glm::zero<glm::ivec2>(), int aaSamples = 0, int depthBits = 0)
        :
        size(size),
        aaSamples(aaSamples),
        depthBits(depthBits)
        {}
        
        WindowInfo(int width, int height, int aaSamples = 0, int depthBits = 0)
        :
        size(width, height),
        aaSamples(aaSamples),
        depthBits(depthBits)
        {}
        
        void rotate()
        {
            size = size.yx();
        }
        
        glm::vec4 bounds() const
        {
            return glm::vec4(0, 0, width, height);
        }

        glm::vec2 center() const
        {
            return size * 0.5f;
        }
        
        float aspectRatio() const
        {
            return (size.y > 0) ? (width / height) : 0;
        }
        
        friend std::ostream& operator<<(std::ostream &lhs, const WindowInfo &rhs)
        {
            lhs
            << "{"
            << "size: [" << rhs.width << ", " << rhs.height << "]"
            << ", anti-aliasing samples: " << rhs.aaSamples
            << ", depth-bits: " << rhs.depthBits
            << "}";
            
            return lhs;
        }
    };
}
