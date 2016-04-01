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
    namespace display
    {
        class Info
        {
        public:
            static constexpr float REFERENCE_DENSITY = 160; // THE DENSITY-INDEPENDENT-PIXEL UNIT (DP) IS BASED ON THIS VALUE
            
            enum Orientation
            {
                ORIENTATION_DEFAULT,
                ORIENTATION_PORTRAIT,
                ORIENTATION_LANDSCAPE
            };
            
            static Info create(int width, int height, float contentScale = 1)
            {
                return Info(glm::vec2(width, height), contentScale, 0, 0);
            }
            
            static Info createWithDiagonal(int width, int height, float diagonal, float contentScale = 1)
            {
                return Info(glm::vec2(width, height), contentScale, diagonal, 0);
            }
            
            static Info createWithDensity(int width, int height, float density, float contentScale = 1)
            {
                return Info(glm::vec2(width, height), contentScale, 0, density);
            }
            
            float contentScale;
            float diagonal; // INCHES
            float density; // DPI
            glm::vec2 size; // PIXELS
            
            Info() = default;
            
            void rotate()
            {
                if (valid)
                {
                    baseSize = baseSize.yx();
                    size = size.yx();
                }
            }
            
            float aspectRatio() const
            {
                if (valid)
                {
                    return size.x / float(size.y);
                }
                
                return 0;
            }
            
            Orientation orientation() const
            {
                if (valid)
                {
                    return (aspectRatio() > 1) ? ORIENTATION_LANDSCAPE : ORIENTATION_PORTRAIT;
                }
                
                return ORIENTATION_DEFAULT;
            }
            
            friend std::ostream& operator<<(std::ostream &lhs, const Info &rhs)
            {
                lhs
                << "{"
                << "size: [" << rhs.size.x << ", " << rhs.size.y << "]"
                << ", content-scale: " << rhs.contentScale
                << ", diagonal: " << rhs.diagonal
                << ", density: " << rhs.density
                << "}";
                
                return lhs;
            }
            
        protected:
            glm::vec2 baseSize;
            bool valid;
            
            Info(const glm::vec2 &baseSize, float contentScale, float diagonal, float density)
            :
            baseSize(baseSize),
            contentScale(contentScale),
            diagonal(diagonal),
            density(density)
            {
                valid = update();
            }
            
            bool update()
            {
                if ((baseSize.x * baseSize.y == 0) || (contentScale < 1) || (diagonal < 0) || (density < 0))
                {
                    contentScale = 0;
                    diagonal = 0;
                    density = 0;
                    size = glm::zero<glm::vec2>();
                    
                    return false;
                }
                
                size = baseSize * contentScale;
                
                if ((diagonal == 0) && (density == 0))
                {
                    density = REFERENCE_DENSITY;
                }
                
                if (diagonal == 0)
                {
                    diagonal = glm::length(size) / density;
                }
                else if (density == 0)
                {
                    density = glm::length(size) / diagonal;
                }
                
                return true;
            }
        };
    }
    
    typedef display::Info DisplayInfo;
}
