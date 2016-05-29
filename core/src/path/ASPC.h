/*
 * BASED ON "Adaptive Sampling of Parametric Curves"
 * BY Luiz Henrique de Figueiredo
 *
 * COPY AVAILABLE AT:
 * http://ariel.chronotext.org/dd/defigueiredo93adaptive.pdf
 */

#pragma once

#include "glm.h"

#include <vector>
#include <array>
#include <functional>

namespace chr
{
  namespace path
  {
    class ASPC
    {
    protected:
      std::vector <glm::vec2> polyline;

      float samplingTolerance = 1;
      std::function<glm::vec2(float, glm::vec2 * )> gamma;
      std::array<glm::vec2, 4> in;

      static std::array<float, 256> random;
      static bool randomGenerated;
      int randomIndex = 0;

      void begin();
      float nextRandom();

      void segment(const glm::vec2 &p0, const glm::vec2 &p1, const glm::vec2 &p2);
      void segment(const glm::vec2 &p0, const glm::vec2 &p1, const glm::vec2 &p2, const glm::vec2 &p3);
      void sample(float t0, const glm::vec2 &p0, float t1, const glm::vec2 &p1);
    };
  }
}
