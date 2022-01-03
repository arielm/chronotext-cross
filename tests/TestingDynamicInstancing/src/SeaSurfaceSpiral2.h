#pragma once

#include "SeaSurface.h"

#include "chr/path/FollowablePath3D.h"
#include "chr/glm.h"

class SeaSurfaceSpiral2
{
public:
  float r1;
  float r2;
  float turns;
  float DD1;
  float DD2;
  float length;

  chr::path::FollowablePath3D path;

  void setup(const SeaSurface &surface, float r1, float r2, float turns, float DD1, float DD2, float length, float sampleSize);
  void update(const SeaSurface &surface, float t, float swellFactor = 1);

protected:
  size_t size;

  std::vector<glm::vec2> points;
  std::vector<glm::vec2> pointsM;
  std::vector<glm::vec2> pointsP;

  std::vector<SeaSurface::Sample> samples;
  std::vector<SeaSurface::Sample> samplesM;
  std::vector<SeaSurface::Sample> samplesP;
};
