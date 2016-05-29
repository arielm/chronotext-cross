#include "path/ASPC.h"

using namespace std;

namespace chr
{
  namespace path
  {
    array<float, 256> ASPC::random;

    bool ASPC::randomGenerated = false;

    void ASPC::begin()
    {
      polyline.clear();

      if (!randomGenerated)
      {
        srand(1);

        for (int i = 0; i < random.size(); i++)
        {
          random[i] = rand() / float(RAND_MAX);
        }

        randomGenerated = true;
      }

      randomIndex = 0;
    }

    float ASPC::nextRandom()
    {
      return random[(randomIndex++) % random.size()];
    }

    void ASPC::segment(const glm::vec2 &p0, const glm::vec2 &p1, const glm::vec2 &p2)
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

    void ASPC::segment(const glm::vec2 &p0, const glm::vec2 &p1, const glm::vec2 &p2, const glm::vec2 &p3)
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

    void ASPC::sample(float t0, const glm::vec2 &p0, float t1, const glm::vec2 &p1)
    {
      float t = 0.45f + 0.1f * nextRandom();
      float rt = t0 + t * (t1 - t0);
      auto r = gamma(rt, in.data());

      float cross = (p0.x - r.x) * (p1.y - r.y) - (p0.y - r.y) * (p1.x - r.x);

      if (cross * cross < samplingTolerance) // XXX
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
