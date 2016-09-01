#pragma once

#include "chr/path/Shape.h"
#include "chr/utils/Utils.h"

namespace chr
{
  namespace path
  {
    class SVGDocument
    {
    public:
      SVGDocument();

      SVGDocument& setOriginAtBottom(bool atBottom);
      SVGDocument& setSamplingTolerance(float tolerance);

      glm::vec2 getSize() const;
      const std::vector<Shape>& getShapes() const;

      bool load(const InputSource &inputSource);

    protected:
      std::vector<Shape> shapes;

      union
      {
        glm::vec2 size;
        struct { float width, height; };
      };

      bool originAtBottom = false;
      float samplingTolerance = 1;

      static Shape::FillRule convert(char fillRule);
      static math::Rectf convert(float bounds[4]);
    };
  }
}
