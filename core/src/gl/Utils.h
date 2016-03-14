#pragma once

#include "Platform.h"

#define GLM_SWIZZLE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#if defined(CHR_PLATFORM_DESKTOP)
  #include <glad/glad.h>
#elif defined(CHR_PLATFORM_EMSCRIPTEN)
  #include <GLES2/gl2.h>
  #include <GLES2/gl2ext.h>
  #include <html5.h>
#elif defined(CHR_PLATFORM_IOS)
  #include <OpenGLES/ES2/gl.h>
  #include <OpenGLES/ES2/glext.h>
#elif defined(CHR_PLATFORM_ANDROID)
  #include <GLES2/gl2.h>
  #include <GLES2/gl2ext.h>
#endif

#if defined (CHR_PLATFORM_DESKTOP)
  #define GL_TEXTURE_MAX_ANISOTROPY_EXT 0x84FE
  #define GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT 0x84FF

  #define GL_GENERATE_MIPMAP 0x8191
  #define GL_GENERATE_MIPMAP_HINT 0x8192
#endif

#include <image/Utils.h>

namespace chr
{
  namespace gl
  {
    struct TextureHandle;

    TextureHandle loadTexture(const fs::path &relativePath, int flags = 0, bool useMipmap = true, GLenum wrapS = GL_CLAMP_TO_EDGE, GLenum wrapT = GL_CLAMP_TO_EDGE);
    TextureHandle uploadMaskedTexture(const image::ImageBuffer &image, const image::ImageBuffer &mask, bool useMipmap = true, GLenum wrapS = GL_CLAMP_TO_EDGE, GLenum wrapT = GL_CLAMP_TO_EDGE);
    void uploadTexture(GLenum format, GLsizei width, GLsizei height, const GLvoid *data, bool useMipmap = true, GLenum wrapS = GL_CLAMP_TO_EDGE, GLenum wrapT = GL_CLAMP_TO_EDGE);

    const glm::mat4 getPerspectiveMatrix(float fovy, float zNear, float zFar, float width, float height, float panX, float panY, float zoom);

    glm::vec3 transformPointAffine(const glm::mat4 &matrix, const glm::vec2 &input);
    glm::vec3 transformPointAffine(const glm::mat4 &matrix, const glm::vec3 &input);

    template<int MODE, int ORIENTATION = GL_CCW>
    void transformQuadAffine(const glm::mat4 &matrix, float x1, float y1, float x2, float y2, std::vector<glm::vec3> &output);
  }
}
