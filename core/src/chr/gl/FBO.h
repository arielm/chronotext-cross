#pragma once

#include "chr/gl/Texture.h"

namespace chr
{
  namespace gl
  {
    namespace fbo
    {
      struct Element
      {
        int useCount = 0;
        GLuint fboId;
        GLuint rboId;
      };
    }

    class FBO
    {
    public:
      int id = -1;
      fbo::Element *element = nullptr;

      Texture colorTexture;

      FBO();
      FBO(int width, int height);

      ~FBO();

      void bind();
      void unbind();
      void deallocate();

    protected:
      GLint defaultFBOId;

      static int usageCounter;

      bool allocate(int width, int height);
    };
  }
}
