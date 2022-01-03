#include "SeaSurface.h"

#include "chr/Random.h"
#include "chr/math/Utils.h"

using namespace std;
using namespace chr;

/*
 * WAVE-LENGTH FOR EACH WAVE-TRAIN
 * (AS USED IN THE "PAPER")
 */
const float WL[] = {204, 242, 61, 68, 84, 87, 101, 109, 25, 28, 34, 34, 39, 42, 47, 51, 54, 59, 24, 26, 29};

/*
 * WAVE-AMPLITUDE FOR EACH WAVE-TRAIN
 * (MANUALLY CREATED SINCE WE DON'T HAVE ACCESS TO THE VALUES USED IN THE "PAPER")
 */
const float R[] = {20.0f, 15.0f, 5.0f, 5.0f, 10.0f, 10.0f, 15.0f, 15.0f, 3.0f, 3.0f, 4.0f, 4.0f, 5.0f, 5.0f, 6.0f, 6.0f, 7.0f, 7.0f, 3.0f, 2.5f, 3.5f};

SeaSurface::SeaSurface(float period, uint32_t seed)
{
    N = sizeof(R) / sizeof(float);
    K = new float[N];
    KX = new float[N];
    KY = new float[N];

    /*
     * DIRECTION FOR EACH WAVE-TRAIN
     * (GENERATED SINCE WE DON'T HAVE ACCESS TO THE VALUES USED IN THE "PAPER")
     */
    
    Random random(seed);
    
    for (int i = 0; i < N; i++)
    {
        K[i] = TWO_PI / WL[i];
        assert(R[i] <= 1.0f / K[i]);

        float orientation = random.nextFloat(TWO_PI);
        KX[i] = sinf(orientation);
        KY[i] = cosf(orientation);
    }
    
    /*
     * USING A GLOBAL VALUE INSTEAD OF ONE PER WAVE-TRAIN
     * ADVANTAGE: PERMITS PRECALCULATION
     * LIMITATION: CYCLIC ANIMATION
     */
    AV = TWO_PI / period;
}

SeaSurface::~SeaSurface()
{
    delete[] K;
    delete[] KX;
    delete[] KY;
}

void SeaSurface::sample(float x, float y, Sample &result) const
{
    glm::vec2 accuX;
    glm::vec2 accuY;
    glm::vec2 accuZ;
    
    for (int i = 0; i < N; i++)
    {
        float A = K[i] * (KX[i] * x + KY[i] * y);
        float ca = cosf(A);
        float sa = sinf(A);
        
        accuX.x += R[i] * KX[i] * ca;
        accuX.y += R[i] * KX[i] * sa;
        
        accuY.x += R[i] * KY[i] * ca;
        accuY.y += R[i] * KY[i] * sa;
        
        accuZ.x += R[i] * ca;
        accuZ.y += R[i] * sa;
    }
    
    accuX /= N;
    result.radiusX = glm::length(accuX);
    result.angleX = atan2f(accuX.y, accuX.x);
    
    accuY /= N;
    result.radiusY = glm::length(accuY);
    result.angleY = atan2f(accuY.y, accuY.x);
    
    accuZ /= N;
    result.radiusZ = glm::length(accuZ);
    result.angleZ = atan2f(accuZ.y, accuZ.x);
}

glm::vec3 SeaSurface::getDisplacement(const Sample &sample, float t, float swellFactor) const
{
    float aX = sample.angleX - AV * t;
    float aY = sample.angleY - AV * t;
    float aZ = sample.angleZ - AV * t;
   
    return glm::vec3(swellFactor * sample.radiusX * sinf(aX), swellFactor * sample.radiusY * sinf(aY), sample.radiusZ * cosf(aZ));
}

/*
 * NO PRECALCULATION
 * NOTE: WE COULD USE ONE ANGULAR-VELOCITY VALUE PER WAVE-TRAIN
 */
glm::vec3 SeaSurface::getDisplacement(float x, float y, float t, float swellFactor) const
{
    glm::vec3 accu;
    
    for (int i = 0; i < N; i++)
    {
        float A = K[i] * (KX[i] * x + KY[i] * y) - AV * t;
        float ca = cosf(A);
        float sa = sinf(A);

        accu.x += swellFactor * R[i] * KX[i] * sa;
        accu.y += swellFactor * R[i] * KY[i] * sa;
        accu.z += R[i] * ca;
    }
    
    return accu / float(N);
}
