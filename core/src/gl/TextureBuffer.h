#pragma once

#include "gl/Utils.h"
#include "gl/TextureHandle.h"
#include "math/Rect.h"

namespace chr
{
  namespace gl
  {
    class TextureBuffer
    {
    public:
      GLint positionLocation;
      GLint coordLocation;
      GLint colorLocation;
      GLint matrixLocation;

      TextureBuffer();
      void shutdown();

      void draw(const TextureHandle &texture, float x = 0, float y = 0, float scale = 1);
      void drawFromCenter(const TextureHandle &texture, float x = 0, float y = 0, float scale = 1);
      void drawInRect(const TextureHandle &texture, const math::Rectf &rect, float ox = 0, float oy = 0);

      void setMatrix(const glm::mat4 &matrix);
      void setColor(float r, float g, float b, float a);
      void setColor(const glm::vec4 &color);

      template <typename T>
      void setShader(T &shader)
      {
        shader.use();

        positionLocation = shader.positionLocation;
        coordLocation = shader.coordLocation;
        colorLocation = shader.colorLocation;
        matrixLocation = shader.matrixLocation;
      }

    protected:
      std::vector<Vertex<UV>> vertices;
      uint32_t vertexVBOId = 0;

      void apply(const TextureHandle &texture);
    };
  }
}
