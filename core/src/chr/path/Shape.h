#pragma once

#include "chr/path/Path.h"
#include "chr/math/Rect.h"

#include <string>

namespace chr
{
  namespace path
  {
    class Shape
    {
    public:
      enum FillRule
      {
        FILLRULE_EVENODD,
        FILLRULE_NONZERO,
      };

      size_t size() const;
      bool empty() const;

      const std::string& getId() const;
      FillRule getFillRule() const;
      math::Rectf getBounds() const;

      const std::vector<Path>& getPaths() const;

      Path& addPath();
      Shape& addPath(const Path &path);

      inline Shape& addPath(std::vector<glm::vec2> &&points)
      {
        paths.emplace_back(std::forward<std::vector<glm::vec2>>(points));
        return *this;
      }

    protected:
      std::vector<Path> paths;

      std::string id;
      FillRule fillRule = FILLRULE_EVENODD;
      math::Rectf bounds;

      friend class SVGDocument;
    };
  }
}
