#pragma once

#include "chr/gl/Batch.h"
#include "chr/gl/shaders/TextureRectangleShader.h"

namespace chr
{
  namespace gl
  {
    namespace syphon
    {
      class Server
      {
      public:
        void start(int aaSamples = 0);
        void stop();

        bool bind(const glm::vec2 &size);
        void unbind();

        Texture getTexture();
        void draw();

      protected:
        void *server = nullptr;
        bool bound = false;

        glm::vec2 size;
        IndexedVertexBatch<XYZ.UV> batch;
        shaders::TextureRectangleShader shader;
      };
    }
  }
}
