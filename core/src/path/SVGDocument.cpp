#include "path/SVGDocument.h"

#include "nanosvg.h"

using namespace std;
using namespace chr;
using namespace math;

namespace chr
{
  namespace path
  {
    SVGDocument::SVGDocument()
    :
    size(0, 0)
    {}

    SVGDocument& SVGDocument::setOriginAtBottom(bool atBottom)
    {
      originAtBottom = atBottom;
      return *this;
    }

    SVGDocument& SVGDocument::setSamplingTolerance(float tolerance)
    {
      samplingTolerance = tolerance;
      return *this;
    }

    glm::vec2 SVGDocument::getSize() const
    {
      return size;
    }

    const vector<Shape>& SVGDocument::getShapes() const
    {
      return shapes;
    }

    bool SVGDocument::load(const fs::path &resourcePath)
    {
      auto text = utils::readTextFromResource<string>(resourcePath);

      if (!text.empty())
      {
        auto doc = nsvgParse(const_cast<char*>(text.data()), "px", 96);

        if (doc)
        {
          width = doc->width;
          height = doc->height;

          for (auto docShape = doc->shapes; docShape != NULL; docShape = docShape->next)
          {
            shapes.emplace_back();
            auto &shape = shapes.back();

            shape.id = docShape->id;
            shape.fillRule = convert(docShape->fillRule);
            shape.bounds = convert(docShape->bounds);

            for (auto shapePath = docShape->paths; shapePath != NULL; shapePath = shapePath->next)
            {
              auto &path = shape.addPath();
              path.setSamplingTolerance(samplingTolerance);

              for (auto i = 0; i < shapePath->npts - 1; i += 3)
              {
                float *p = &shapePath->pts[i * 2];

                if (i == 0)
                {
                  if (originAtBottom)
                  {
                    path.moveTo(p[0], height - p[1]);
                    path.bezierCurveTo(p[2], height - p[3], p[4], height - p[5], p[6], height - p[7]);
                  }
                  else
                  {
                    path.moveTo(p[0], p[1]);
                    path.bezierCurveTo(p[2], p[3], p[4], p[5], p[6], p[7]);
                  }
                }
                else
                {
                  if (originAtBottom)
                  {
                    path.bezierCurveTo(p[2], height - p[3], p[4], height - p[5], p[6], height - p[7]);
                  }
                  else
                  {
                    path.bezierCurveTo(p[2], p[3], p[4], p[5], p[6], p[7]);
                  }
                }
              }

              /*
               * NANOSVG HANDLES LINES VIA CUBIC-BEZIER-CURVES,
               * WHICH PRODUCE DUPLICATE POINTS
               */
              path.cleanup();
            }
          }

          nsvgDelete(doc);

          return true;
        }
      }

      return false;
    }

    Shape::FillRule SVGDocument::convert(char fillRule)
    {
      switch (fillRule)
      {
        default:
        case NSVG_FILLRULE_EVENODD:
          return Shape::FILLRULE_EVENODD;

        case NSVG_FILLRULE_NONZERO:
          return Shape::FILLRULE_NONZERO;
      }
    }

    Rectf SVGDocument::convert(float bounds[4])
    {
      return Rectf(glm::vec2(bounds[0], bounds[1]), glm::vec2(bounds[2], bounds[3]));
    }
  }
}
