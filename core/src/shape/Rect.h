#pragma once

#include "math/Utils.h"

namespace chr
{
  namespace shape
  {
    class Rect
    {
    public:
      Rect& setSize(float width, float height)
      {
        this->width = width;
        this->height = height;
        return *this;
      }

      Rect& setSize(const glm::vec2 &size)
      {
        width = size.x;
        height = size.y;
        return *this;
      }

      std::vector<glm::vec2> get() const
      {
        float w2 = width * 0.5f;
        float h2 = height * 0.5f;

        std::vector<glm::vec2> output;
        output.reserve(4);

        output.emplace_back(-w2, -h2);
        output.emplace_back(-w2, +h2);
        output.emplace_back(+w2, +h2);
        output.emplace_back(+w2, -h2);

        return output;
      }

    protected:
      float width = 1;
      float height = 1;
    };
  }
}
