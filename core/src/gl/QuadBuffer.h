#pragma once

#include "gl/Utils.h"
#include "math/Rect.h"

namespace chr
{
  namespace gl
  {
    class QuadBuffer
    {
    public:
      void setup();
      void shutdown();

      void draw(float x1, float y1, float x2, float y2);
      inline void draw(const math::Rectf &rect) { draw(rect.x1, rect.y1, rect.x2, rect.y2); }

      void setMatrix(const glm::mat4 &matrix);
      void setColor(float r, float g, float b, float a);
      void setColor(const glm::vec4 &color);

    protected:
      std::vector<glm::vec3> vertices;
      GLuint vboIds[2];
    };
  }
}
