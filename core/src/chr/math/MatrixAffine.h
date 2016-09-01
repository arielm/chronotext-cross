#pragma once

#include "chr/math/Rect.h"

#include <array>
#include <vector>

namespace chr
{
  namespace math
  {
    class MatrixAffine
    {
    public:
      typedef std::array<float, 6> Values;
      
      union
      {
        struct
        {
          float m00, m10;
          float m01, m11;
          float m02, m12;
        };
        
        Values m;
      };
      
      MatrixAffine();
      MatrixAffine(const MatrixAffine &other) = delete;
      
      operator Values& () { return m; }
      operator const Values& () const { return m; }

      MatrixAffine& load(const MatrixAffine &matrix);

      MatrixAffine& push();
      MatrixAffine& pop();

      MatrixAffine& setIdentity();

      inline MatrixAffine& setTranslate(const glm::vec2 &t) { return setTranslate(t.x, t.y); }
      MatrixAffine& setTranslate(float x, float y);

      inline MatrixAffine& translate(const glm::vec2 &t) { return translate(t.x, t.y); }
      MatrixAffine& translate(float x, float y);

      inline MatrixAffine& scale(const glm::vec2 &s) { return scale(s.x, s.y); }
      inline MatrixAffine& scale(float s) { return scale(s, s); }
      MatrixAffine& scale(float x, float y);

      MatrixAffine& rotate(float a);

      inline glm::vec2 transformPoint(const glm::vec2 &point) const { return transformPoint(point.x, point.y); }
      glm::vec2 transformPoint(float x, float y) const;

      inline std::vector<glm::vec2> transformRect(float left, float top, float width, float height) const { return transformRect(Rectf(left, top, width, height)); }
      std::vector<glm::vec2> transformRect(const Rectf &rect) const;

      std::vector<glm::vec2> transformPoints(const std::vector<glm::vec2> &points);

    protected:
      std::vector<Values> stack;
    };
  }
}
