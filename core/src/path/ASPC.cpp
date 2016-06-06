#include "path/ASPC.h"

#include <cstdlib>

using namespace std;

namespace chr
{
  namespace path
  {
    array<float, 256> ASPC::randomBase;
    bool ASPC::randomBaseGenerated = false;

    void ASPC::generateRandomBase()
    {
      if (!randomBaseGenerated)
      {
        srand(1);

        for (int i = 0; i < randomBase.size(); i++)
        {
          randomBase[i] = rand() / float(RAND_MAX);
        }

        randomBaseGenerated = true;
      }

      randomIndex = 0;
    }

    float ASPC::nextRandom()
    {
      return randomBase[(randomIndex++) % randomBase.size()];
    }

    // ----------

    ASPC2D::ASPC2D(vector<glm::vec2> &&polyline)
    :
    polyline(polyline)
    {}

    void ASPC2D::begin()
    {
      polyline.clear();
      generateRandomBase();
    }

    void ASPC2D::segment(const glm::vec2 &p0, const glm::vec2 &p1, const glm::vec2 &p2)
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

    void ASPC2D::segment(const glm::vec2 &p0, const glm::vec2 &p1, const glm::vec2 &p2, const glm::vec2 &p3)
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

    void ASPC2D::sample(float t0, const glm::vec2 &p0, float t1, const glm::vec2 &p1)
    {
      float t = 0.45f + 0.1f * nextRandom();
      float rt = t0 + t * (t1 - t0);
      auto r = gamma(rt, in.data());

      float cross = (p0.x - r.x) * (p1.y - r.y) - (p1.x - r.x) * (p0.y - r.y);

      if (cross * cross < samplingTolerance)
      {
        polyline.push_back(p0);
      }
      else
      {
        sample(t0, p0, rt, r);
        sample(rt, r, t1, p1);
      }
    }
  }
}
