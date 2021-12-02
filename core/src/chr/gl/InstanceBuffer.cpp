#include "InstanceBuffer.h"

using namespace std;

namespace chr
{
  namespace gl
  {
    InstanceBuffer::InstanceBuffer(GLenum matricesUsage, GLenum colorsUsage)
    :
    element(new instancebuffer::Element())
    {
      element->matricesUsage = matricesUsage;
      element->colorsUsage = colorsUsage;
      element->useCount++;
    }

    InstanceBuffer::InstanceBuffer(const InstanceBuffer &other)
    :
    element(other.element)
    {
      element->useCount++;
    }

    InstanceBuffer& InstanceBuffer::operator=(const InstanceBuffer &other)
    {
      if (this != &other)
      {
        element = other.element;
        element->useCount++;
      }

      return *this;
    }

    InstanceBuffer::~InstanceBuffer()
    {
      if (--element->useCount == 0)
      {
        if (element->matricesVboId != 0)
        {
          glDeleteBuffers(1, &element->matricesVboId);
        }

        if (element->colorsVboId != 0)
        {
          glDeleteBuffers(1, &element->colorsVboId);
        }
      }
    }

    void InstanceBuffer::bind(const ShaderProgram &shader)
    {
      bindMatrices(shader);
      bindColors(shader);
    }  

    void InstanceBuffer::unbind(const ShaderProgram &shader)
    {
      for (int i = 0; i < 4; ++i)
      {
        auto location = shader->matrixLocation + i;
        glDisableVertexAttribArray(location);
        glVertexAttribDivisor(location, 0);
      }

      glDisableVertexAttribArray(shader->colorLocation);
      glVertexAttribDivisor(shader->colorLocation, 0);

      glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    void InstanceBuffer::clearMatrices()
    {
      element->matrices.clear();
      element->matricesUploadRequired = true;
    }

    void InstanceBuffer::clearColors()
    {
      element->colors.clear();
      element->colorsUploadRequired = true;
    }

    InstanceBuffer& InstanceBuffer::addMatrix(const glm::mat4 &matrix)
    {
      element->matrices.push_back(matrix);
      return *this;
    }

    InstanceBuffer& InstanceBuffer::addMatrix(const Matrix &matrix)
    {
      element->matrices.push_back(matrix.m);
      return *this;
    }

    InstanceBuffer& InstanceBuffer::addColor(const glm::vec4 &color)
    {
      element->colors.push_back(color);
      return *this;
    }

    InstanceBuffer& InstanceBuffer::addColor(float r, float g, float b, float a)
    {
      element->colors.emplace_back(r, g, b, a);
      return *this;
    }

    size_t InstanceBuffer::size() const
    {
      return element->matrices.size();
    }

    void InstanceBuffer::bindMatrices(const ShaderProgram &shader)
    {
      if (element->matricesVboId == 0)
      {
        glGenBuffers(1, &element->matricesVboId);
      }
      else if (element->matrices.size() > element->matricesAllocatedSize)
      {
        element->matricesUploadRequired = true;
        element->matricesAllocatedSize = 0;

        glDeleteBuffers(1, &element->matricesVboId);
        glGenBuffers(1, &element->matricesVboId);
      }

      glBindBuffer(GL_ARRAY_BUFFER, element->matricesVboId);

      for (int i = 0; i < 4; ++i)
      {
          auto location = shader->matrixLocation + i;
          glEnableVertexAttribArray(location);
          glVertexAttribPointer(location, 4, GL_FLOAT, GL_FALSE, 16 * sizeof(float), (GLvoid*)(i * 16));
          glVertexAttribDivisor(location, 1);
      }

      if (element->matricesUploadRequired)
      {
        element->matricesUploadRequired = false;

        switch (element->matricesUsage)
        {
          case GL_STATIC_DRAW:
          {
            if (element->matricesAllocatedSize == 0)
            {
              element->matricesAllocatedSize = element->matrices.size();
              glBufferData(GL_ARRAY_BUFFER, element->matrices.size() * sizeof(glm::mat4), element->matrices.data(), GL_STATIC_DRAW);
            }
          }
          break;

          case GL_DYNAMIC_DRAW:
          default:
          {
            if (element->matricesAllocatedSize < element->matrices.size())
            {
              element->matricesAllocatedSize = element->matrices.size();
              glBufferData(GL_ARRAY_BUFFER, element->matrices.size() * sizeof(glm::mat4), nullptr, GL_DYNAMIC_DRAW);
            }

            glBufferSubData(GL_ARRAY_BUFFER, 0, element->matrices.size() * sizeof(glm::mat4), element->matrices.data());
          }
          break;
        }
      }
    }

    void InstanceBuffer::bindColors(const ShaderProgram &shader)
    {
      if (!element->colors.empty())
      {
        if (element->colorsVboId == 0)
        {
          glGenBuffers(1, &element->colorsVboId);
        }
        else if (element->colors.size() > element->colorsAllocatedSize)
        {
          element->colorsUploadRequired = true;
          element->colorsAllocatedSize = 0;

          glDeleteBuffers(1, &element->colorsVboId);
          glGenBuffers(1, &element->colorsVboId);
        }

        glBindBuffer(GL_ARRAY_BUFFER, element->colorsVboId);
        glEnableVertexAttribArray(shader->colorLocation);
        glVertexAttribPointer(shader->colorLocation, 4, GL_FLOAT, GL_FALSE, 0, nullptr);
        glVertexAttribDivisor(shader->colorLocation, 1);

        if (element->colorsUploadRequired)
        {
          element->colorsUploadRequired = false;

          switch (element->colorsUsage)
          {
            case GL_STATIC_DRAW:
            {
              if (element->colorsAllocatedSize == 0)
              {
                element->colorsAllocatedSize = element->colors.size();
                glBufferData(GL_ARRAY_BUFFER, element->colors.size() * sizeof(glm::vec4), element->colors.data(), GL_STATIC_DRAW);
              }
            }
            break;

            case GL_DYNAMIC_DRAW:
            default:
            {
              if (element->colorsAllocatedSize < element->colors.size())
              {
                element->colorsAllocatedSize = element->colors.size();
                glBufferData(GL_ARRAY_BUFFER, element->colors.size() * sizeof(glm::vec4), nullptr, GL_DYNAMIC_DRAW);
              }

              glBufferSubData(GL_ARRAY_BUFFER, 0, element->colors.size() * sizeof(glm::vec4), element->colors.data());
            }
            break;
          }
        }
      }
    }
  }
}
