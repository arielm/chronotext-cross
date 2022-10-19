/*
 * BASED ON "Adaptive Sampling of Parametric Curves"
 * BY Luiz Henrique de Figueiredo
 *
 * COPY AVAILABLE AT:
 * http://ariel.chronotext.org/dd/defigueiredo93adaptive.pdf
 */

#pragma once

#include "chr/glm.h"

#include <vector>
#include <array>
#include <functional>

namespace chr
{
  namespace path
  {
    class ASPCBase
    {
    public:
      ASPCBase() = default;

    protected:
      float samplingTolerance = 1;

      static std::array<float, 256> randomBase;
      static bool randomBaseGenerated;
      int randomIndex = 0;

      void generateRandomBase();
      float nextRandom();
    };

    // ---

    template <typename T>
    class ASPC : protected ASPCBase
    {
    public:
      ASPC<T>() = default;

      ASPC<T>(const std::vector<T> &polyline)
      :
      polyline(polyline)
      {}

      ASPC<T>(std::vector<T> &&polyline)
      :
      polyline(polyline)
      {}

    protected:
      std::vector<T> polyline;

      std::function<T(float, T *)> gamma;
      std::array<T, 4> in;

      void sample(float t0, const T &p0, float t1, const T &p1);

      void begin()
      {
        polyline.clear();
        generateRandomBase();
      }

      void segment(const T &p0, const T &p1, const T &p2)
      {
        in[0] = p0;
        in[1] = p1;
        in[2] = p2;

        float pt = 0;
        auto p = gamma(pt, in.data());

        float qt = 1;
        auto q = gamma(qt, in.data());

        sample(pt, p, qt, q);
      }

      void segment(const T &p0, const T &p1, const T &p2, const T &p3)
      {
        in[0] = p0;
        in[1] = p1;
        in[2] = p2;
        in[3] = p3;

        float pt = 0;
        auto p = gamma(pt, in.data());

        float qt = 1;
        auto q = gamma(qt, in.data());

        sample(pt, p, qt, q);
      }
    };
  }
}
