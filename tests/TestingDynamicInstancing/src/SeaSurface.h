#pragma once

#include "chr/glm.h"

/*
 * BASED ON "Interactive Animation of Ocean Waves"
 * BY Damien Hinsinger, Fabrice Neyret AND Marie-Paule Cani
 */

class SeaSurface
{
public:
    struct Sample
    {
        float angleX;
        float radiusX;
        
        float angleY;
        float radiusY;
        
        float angleZ;
        float radiusZ;
    };
    
    SeaSurface(float period = 5.55555f, uint32_t seed = 123456789);
    ~SeaSurface();
    
    void sample(float x, float y, Sample &result) const;
    inline void sample(const glm::vec2 &point, Sample &result) const { sample(point.x, point.y, result); }

    glm::vec3 getDisplacement(const Sample &sample, float t, float swellFactor = 1) const;
    glm::vec3 getDisplacement(float x, float y, float t, float swellFactor = 1) const;
    inline glm::vec3 getDisplacement(const glm::vec2 &point, float t, float swellFactor = 1) const { return getDisplacement(point.x, point.y, t, swellFactor); }

protected:
    int N; // SAMPLE COUNT
    float *K; // WAVE NUMBER (PER SAMPLE)
    float *KX, *KY; // WAVE DIRECTION (PER SAMPLE, NORMALIZED)
    
    float AV; // ANGULAR VELOCITY (GLOBAL, IN RADIANS PER SECOND)
};
