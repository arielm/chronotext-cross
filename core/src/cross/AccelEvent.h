/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012-2015, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE SIMPLIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

/*
 * BASED ON https://github.com/cinder/Cinder/blob/v0.8.4/include/cinder/app/AccelEvent.h
 */

#pragma once

#define GLM_SWIZZLE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

namespace chr
{
    class AccelEvent
    {
    public:
        AccelEvent(const glm::vec3 &data, const glm::vec3 &rawData, const glm::vec3 &prevData, const glm::vec3 &prevRawData)
        :
        data(data),
        rawData(rawData),
        prevData(prevData),
        prevRawData(prevRawData)
        {}
        
        glm::vec3 getData() const { return data; }
        glm::vec3 getRawData() const { return rawData; }
        glm::vec3 getPrevData() const { return prevData; }
        glm::vec3 getPrevRawData() const { return prevRawData; }
        
        bool isShaking(float shakeDelta = 2.2f) const
        {
            return glm::length(rawData - prevRawData) > shakeDelta;
        }
        
        class Filter
        {
        public:
            Filter()
            :
            factor(0.1f),
            lastAccel(glm::zero<glm::vec3>()),
            lastRawAccel(glm::zero<glm::vec3>())
            {}
            
            Filter(float factor)
            :
            factor(factor),
            lastAccel(glm::zero<glm::vec3>()),
            lastRawAccel(glm::zero<glm::vec3>())
            {}
            
            AccelEvent process(const glm::vec3 &acceleration)
            {
                glm::vec3 filtered = lastAccel * (1 - factor) + acceleration * factor;
                AccelEvent event(filtered, acceleration, lastAccel, lastRawAccel);
                
                lastAccel = filtered;
                lastRawAccel = acceleration;
                
                return event;
            }
            
        protected:
            float factor;
            glm::vec3 lastAccel, lastRawAccel;
        };
        
    protected:
        glm::vec3 data, prevData;
    	  glm::vec3 rawData, prevRawData;
    };
}
