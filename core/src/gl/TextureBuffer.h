#pragma once

#include "gl/Utils.h"
#include "gl/TextureHandle.h"

namespace chr
{
  namespace gl
  {
    class TextureBuffer
    {
    public:
      void setup();
      void shutdown();

      void draw(const TextureHandle &texture, float x = 0, float y = 0, float scale = 1);

      void setColor(float r, float g, float b, float a);
      void setColor(const glm::vec4 &color);

    protected:
      std::vector<glm::vec3> vertices;
      std::vector<glm::vec2> coords;
      GLuint vboIds[3];
    };
  }
}
