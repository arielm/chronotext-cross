#pragma once

#include "chr/gl/ShaderProgram.h"
#include "chr/gl/Matrix.h"

namespace chr
{
  namespace gl
  {
    class InstanceBuffer
    {
    public:
  	  InstanceBuffer(GLenum matricesUsage = GL_DYNAMIC_DRAW, GLenum colorsUsage = GL_DYNAMIC_DRAW)
  	  :
  	  matricesUsage(matricesUsage),
  	  colorsUsage(colorsUsage)
  	  {}

      ~InstanceBuffer();

  	  void bind(const ShaderProgram &shader);
  	  void unbind(const ShaderProgram &shader);

  	  void clearMatrices();
  	  void clearColors();

  	  InstanceBuffer& addMatrix(const glm::mat4 &matrix);
  	  InstanceBuffer& addMatrix(const Matrix &matrix);

  	  InstanceBuffer& addColor(const glm::vec4 &color);
  	  InstanceBuffer& addColor(float r, float g, float b, float a);

  	  size_t getCount() const;

    protected:
      GLenum matricesUsage;
      GLenum colorsUsage;

      GLuint matricesVboId = 0;
      GLuint colorsVboId = 0;

      size_t matricesAllocatedSize = 0;
      size_t colorsAllocatedSize = 0;

      bool matricesUploadRequired = true;
      bool colorsUploadRequired = true;

  	  std::vector<glm::mat4> matrices;
   	  std::vector<glm::vec4> colors;

      void bindMatrices(const ShaderProgram &shader);
      void bindColors(const ShaderProgram &shader);
    };
  }
}
