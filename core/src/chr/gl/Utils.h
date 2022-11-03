#pragma once

#include "chr/gl/Batch.h"
#include "chr/gl/Quad.h"
#include "chr/gl/ShaderHelper.h"
#include "chr/math/Utils.h"
#include "chr/image/Utils.h"

namespace chr
{
  namespace gl
  {
    Texture::Response loadAndUploadTexture(const Texture::ImageRequest &request);
    Texture::Response uploadTexture(const image::ImageBuffer &image, bool useMipmap = true, bool useAnisotropy = false, GLenum wrapS = GL_CLAMP_TO_EDGE, GLenum wrapT = GL_CLAMP_TO_EDGE, GLint minFilter = GL_LINEAR, GLint magFilter = GL_LINEAR);
    Texture::Response loadAndUploadMaskedTexture(const Texture::MaskedRequest &request);
    Texture::Response uploadMaskedTexture(const image::ImageBuffer &image, const image::ImageBuffer &mask, bool useMipmap = true, bool useAnisotropy = false, GLenum wrapS = GL_CLAMP_TO_EDGE, GLenum wrapT = GL_CLAMP_TO_EDGE, GLint minFilter = GL_LINEAR, GLint magFilter = GL_LINEAR);
    void uploadTexture(GLenum format, GLsizei width, GLsizei height, const GLvoid *data, bool useMipmap = true, bool useAnisotropy = false, GLenum wrapS = GL_CLAMP_TO_EDGE, GLenum wrapT = GL_CLAMP_TO_EDGE, GLint minFilter = GL_LINEAR, GLint magFilter = GL_LINEAR);

    const glm::mat4 getPerspectiveMatrix(float fovY, float nearZ, float farZ, float width, float height, float panX = 0, float panY = 0, float zoom = 1);

    bool clip(Quad<XYZ.UV> &quad, const math::Rectf &clipRect, const glm::vec2 &textureFactor);

    void addLineStrip(VertexBatch<XYZ> &batch, const std::vector<glm::vec2> &points);
    void addLines(VertexBatch<XYZ> &batch, const std::vector<glm::vec2> &points);
  }
}
