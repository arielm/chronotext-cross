#include "chr/path/ASPC.h"

#include <cstdlib>

using namespace std;

namespace chr
{
  namespace path
  {
    array<double, 256> ASPCBase::randomBase;
    bool ASPCBase::randomBaseGenerated = false;

    void ASPCBase::generateRandomBase()
    {
      if (!randomBaseGenerated)
      {
        srand(1); // XXX

        for (auto i = 0; i < randomBase.size(); i++)
        {
          randomBase[i] = rand() / double(RAND_MAX);
        }

        randomBaseGenerated = true;
      }

      randomIndex = 0;
    }

    double ASPCBase::nextRandom()
    {
      return randomBase[(randomIndex++) % randomBase.size()];
    }

    // ---

    template<>
    void ASPC<glm::vec2>::sample(float t0, const glm::vec2 &p0, float t1, const glm::vec2 &p1)
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

    // ---

    template<>
    void ASPC<glm::vec3>::sample(float t0, const glm::vec3 &p0, float t1, const glm::vec3 &p1)
    {
      float t = 0.45f + 0.1f * nextRandom();
      float rt = t0 + t * (t1 - t0);
      auto r = gamma(rt, in.data());

      float x0 = p0.x - r.x, y0 = p0.y - r.y, z0 = p0.z - r.z;
      float x1 = p1.x - r.x, y1 = p1.y - r.y, z1 = p1.z - r.z;

      float x = y0 * z1 - y1 * z0;
      float y = z0 * z1 - x1 * z0;
      float z = x0 * y1 - x1 * y0;

      if (x * x + y * y + z * z < samplingTolerance)
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
