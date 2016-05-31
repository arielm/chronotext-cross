#pragma once

#include "ASPC.h"

namespace chr
{
  namespace path
  {
    class Path: public ASPC
    {
    public:
      const std::vector <glm::vec2> &getPolyline() const;

      size_t size() const;
      bool empty() const;
      bool isClosed() const;

      Path& setSamplingTolerance(float tolerance);

      Path& clear();
      Path& close();

      Path& moveTo(const glm::vec2 &p);
      Path& lineTo(const glm::vec2 &p);
      Path& quadraticCurveTo(const glm::vec2 &p1, const glm::vec2 &p2);
      Path& bezierCurveTo(const glm::vec2 &p1, const glm::vec2 &p2, const glm::vec2 &p3);

      inline Path& moveTo(float x, float y) { return moveTo(glm::vec2(x, y)); }
      inline Path& lineTo(float x, float y) { return lineTo(glm::vec2(x, y)); }
      inline Path& quadraticCurveTo(float x1, float y1, float x2, float y2) { return quadraticCurveTo(glm::vec2(x1, y1), glm::vec2(x2, y2)); }
      inline Path& bezierCurveTo(float x1, float y1, float x2, float y2, float x3, float y3) { return bezierCurveTo(glm::vec2(x1, y1), glm::vec2(x2, y2), glm::vec2(x3, y3)); }

      /*
       * REMOVES DUPLICATE CONSECUTIVE POINTS
       */
      Path& cleanup();

    protected:
      glm::vec2 current = {std::numeric_limits<float>::quiet_NaN(), std::numeric_limits<float>::quiet_NaN()};

      static glm::vec2 GammaQuadratic(float t, glm::vec2 *in)
      {
        float t1 = (1 - t);
        return in[0] * (t1 * t1) + in[1] * (2 * t * t1) + in[2] * (t * t);
      }

      static glm::vec2 GammaBezier(float t, glm::vec2 *in)
      {
        float t1 = (1 - t);
        return in[0] * (t1 * t1 * t1) + in[1] * (3 * t * t1 * t1) + in[2] * (3 * t * t * t1) + in[3] * (t * t * t);
      }
    };
  }
}