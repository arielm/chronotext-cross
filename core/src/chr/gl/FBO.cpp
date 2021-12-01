#include "FBO.h"

namespace chr
{
  namespace gl
  {
    int FBO::usageCounter = 0;

    FBO::FBO()
    {}

    FBO::FBO(int width, int height)
    :
    id(usageCounter++),
    element(new fbo::Element())
    {
      allocate(width, height);
    }

    FBO::~FBO()
    {
      if (element)
      {
        element->useCount--;

        if (element->useCount == 0)
        {
          deallocate();
          delete element;
        }
      }
    }

    void FBO::bind()
    {
      if (element && element->fboId)
      {
        defaultFBOId = 0;
        if (CHR_PLATFORM == PLATFORM_IOS) glGetIntegerv(GL_FRAMEBUFFER_BINDING, &defaultFBOId);

        glBindFramebuffer(GL_FRAMEBUFFER, element->fboId);
      }
    }

    void FBO::unbind()
    {
      if (element && element->fboId)
      {
        glBindFramebuffer(GL_FRAMEBUFFER, defaultFBOId);
      }
    }

    bool FBO::allocate(int width, int height)
    {
      bool complete = false;

      if (element && !element->fboId)
      {
        defaultFBOId = 0;
        if (CHR_PLATFORM == PLATFORM_IOS) glGetIntegerv(GL_FRAMEBUFFER_BINDING, &defaultFBOId);

        glGenFramebuffers(1, &element->fboId);
        glBindFramebuffer(GL_FRAMEBUFFER, element->fboId);

        //

        GLuint textureId;
        glGenTextures(1, &textureId);

        glBindTexture(GL_TEXTURE_2D, textureId);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
        glBindTexture(GL_TEXTURE_2D, 0);

        colorTexture = Texture(textureId, width, height, GL_RGBA);

        //

        glGenRenderbuffers(1, &element->rboId);
        glBindRenderbuffer(GL_RENDERBUFFER, element->rboId);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, width, height);
        glBindRenderbuffer(GL_RENDERBUFFER, 0);

        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureId, 0);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, element->rboId);

        complete = glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE;

        glBindFramebuffer(GL_FRAMEBUFFER, defaultFBOId);
      }

      return (element && complete);
    }

    void FBO::deallocate()
    {
      if (element && element->fboId)
      {
        glDeleteFramebuffers(1, &element->fboId);
        element->fboId = 0;

        glDeleteRenderbuffers(1, &element->rboId);
        element->rboId = 0;

        colorTexture.unload();
      }
    }
  }
}
