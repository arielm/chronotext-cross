/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012-2015, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE SIMPLIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#pragma once

#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

namespace chr
{
    struct WindowInfo
    {
        glm::vec2 size;
        int aaLevel;

        static WindowInfo create(int width, int height, int aaLevel = 0)
        {
            return WindowInfo(width, height, aaLevel);
        }
        
        WindowInfo(const glm::ivec2 &size = glm::zero<glm::ivec2>(), int aaLevel = 0)
        :
        size(size),
        aaLevel(aaLevel)
        {}
        
        WindowInfo(int width, int height, int aaLevel = 0)
        :
        size(width, height),
        aaLevel(aaLevel)
        {}
        
        void rotate()
        {
            size = glm::vec2(size.y, size.x); // XXX
        }
        
        glm::vec4 bounds() const
        {
            return glm::vec4(0, 0, size.x, size.y);
        }

        glm::vec2 center() const
        {
            return size * 0.5f;
        }
        
        float aspectRatio() const
        {
            return (size.y > 0) ? (size.x / float(size.y)) : 0;
        }
        
        friend std::ostream& operator<<(std::ostream &lhs, const WindowInfo &rhs)
        {
            lhs
            << "{"
            << "size: [" << rhs.size.x << "," << rhs.size.y << "]"
            << ", anti-aliasing-level: " << rhs.aaLevel
            << "}";
            
            return lhs;
        }
    };
}
