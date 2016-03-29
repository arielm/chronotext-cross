#pragma once

#include "gl/ShaderProgram.h"
#include "gl/TextureHandle.h"
#include "math/Rect.h"

namespace chr
{
  namespace gl
  {
    class TextureBuffer
    {
    public:
      GLint mvpMatrixLocation;
      GLint positionLocation;
      GLint colorLocation;
      GLint coordLocation;

      TextureBuffer();
      void shutdown();

      void draw(const TextureHandle &texture, float x = 0, float y = 0, float scale = 1);
      void drawFromCenter(const TextureHandle &texture, float x = 0, float y = 0, float scale = 1);
      void drawInRect(const TextureHandle &texture, const math::Rectf &rect, float ox = 0, float oy = 0);

      void setShader(ShaderProgram &shader)
      {
        shader.use(); // XXX

        mvpMatrixLocation = shader.mvpMatrixLocation;
        positionLocation = shader.positionLocation;
        colorLocation = shader.colorLocation;
        coordLocation = shader.coordLocation;
      }

    protected:
      std::vector<Vertex<UV>> vertices;
      uint32_t vertexVBOId = 0;

      void apply(const TextureHandle &texture);
    };
  }
}
