#include "SeaSurfaceSpiral2.h"

#include "chr/math/Utils.h"

using namespace std;
using namespace chr;

void SeaSurfaceSpiral2::setup(const SeaSurface &surface, float r1, float r2, float turns, float DD1, float DD2, float length, float sampleSize)
{
  this->r1 = r1;
  this->r2 = r2;
  this->turns = turns;
  this->DD1 = DD1;
  this->DD2 = DD2;
  this->length = length;

  size = 0;
  
  float l = TWO_PI * turns;
  float dr = (r2 - r1) / l;
  float DD = (DD2 - DD1) / l;
  float D = 0;
  
  do
  {
    float r = sqrtf(r1 * r1 + 2 * dr * D);
    float d = (r - r1) / dr;
    D += DD1 + d * DD;

    float sx = -sinf(-d);
    float cy = +cosf(-d);

    points.emplace_back(sx * r, cy * r);

    float rm = r - sampleSize * 0.5f;
    pointsM.emplace_back(sx * rm, cy * rm);

    float rp = r + sampleSize * 0.5f;
    pointsP.emplace_back(sx * rp, cy * rp);
    
    size++;
  }
  while (D < length);

  // ---
  
  samples.reserve(size);
  samplesM.reserve(size);
  samplesP.reserve(size);
  
  for (auto i = 0; i < size; i++)
  {
    surface.sample(points[i], samples[i]);
    surface.sample(pointsM[i], samplesM[i]);
    surface.sample(pointsP[i], samplesP[i]);
  }
}

void SeaSurfaceSpiral2::update(const SeaSurface &surface, float t, float swellFactor)
{
  path.begin();

  for (auto i = 0; i < size; i++)
  {
    float x = points[i].x;
    float y = points[i].y;
    glm::vec3 p(glm::vec3(points[i], 0) + surface.getDisplacement(samples[i], t, swellFactor));

    float xm = pointsM[i].x;
    float ym = pointsM[i].y;
    glm::vec3 pm(glm::vec3(pointsM[i], 0) + surface.getDisplacement(samplesM[i], t, swellFactor));

    float xp = pointsP[i].x;
    float yp = pointsP[i].y;
    glm::vec3 pp(glm::vec3(pointsP[i], 0) + surface.getDisplacement(samplesP[i], t, swellFactor));

    glm::vec3 left = glm::normalize(pp - pm);
    path.add(p, left);
  }

  path.end();
}
