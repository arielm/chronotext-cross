#pragma once

#include "chr/math/Utils.h"

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

      inline std::vector<glm::vec2> append(const glm::vec2 &xy) const { return append(xy.x, xy.y); }

      std::vector<glm::vec2> append(float x = 0, float y = 0) const
      {
        float w2 = width * 0.5f;
        float h2 = height * 0.5f;

        std::vector<glm::vec2> output;
        output.reserve(5);

        output.emplace_back(x - w2, y - h2);
        output.emplace_back(x - w2, y + h2);
        output.emplace_back(x + w2, y + h2);
        output.emplace_back(x + w2, y - h2);
        output.push_back(output.front());

        return output;
      }

    protected:
      float width = 1;
      float height = 1;
    };
  }
}
