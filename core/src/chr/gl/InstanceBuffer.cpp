#include "InstanceBuffer.h"

using namespace std;

namespace chr
{
  namespace gl
  {
  	void InstanceBuffer::bind(const ShaderProgram &shader)
  	{
  	  bindMatrices(shader);
  	  bindColors(shader);
  	}

    void InstanceBuffer::unbind(const ShaderProgram &shader)
    {
      glDisableVertexAttribArray(shader->matrixLocation);
      glDisableVertexAttribArray(shader->colorLocation);
      glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    void InstanceBuffer::clearMatrices()
    {
      matrices.clear();
      matricesUploadRequired = true;
    }

    void InstanceBuffer::clearColors()
    {
      colors.clear();
      colorsUploadRequired = true;
    }

    InstanceBuffer& InstanceBuffer::addMatrix(const glm::mat4 &matrix)
    {
      matrices.push_back(matrix);
      return *this;
    }

    InstanceBuffer& InstanceBuffer::addMatrix(const Matrix &matrix)
    {
      matrices.push_back(matrix.m);
      return *this;
    }

    InstanceBuffer& InstanceBuffer::addColor(const glm::vec4 &color)
    {
      colors.push_back(color);
      return *this;
    }

    InstanceBuffer& InstanceBuffer::addColor(float r, float g, float b, float a)
    {
      colors.emplace_back(r, g, b, a);
      return *this;
    }

    size_t InstanceBuffer::getCount() const
    {
      return matrices.size();
    }

    void InstanceBuffer::bindMatrices(const ShaderProgram &shader)
    {
    	if (matricesVboId == 0)
      {
        glGenBuffers(1, &matricesVboId);
      }
      else if (matrices.size() > matricesAllocatedSize)
      {
        matricesUploadRequired = true;
        matricesAllocatedSize = 0;

        glDeleteBuffers(1, &matricesVboId);
        glGenBuffers(1, &matricesVboId);
      }

      glBindBuffer(GL_ARRAY_BUFFER, matricesVboId);

	    for (int i = 0; i < 4; ++i)
	    {
	        auto location = shader->matrixLocation + i;
	        glEnableVertexAttribArray(location);
	        glVertexAttribPointer(location, 4, GL_FLOAT, GL_FALSE, 16 * sizeof(float), (GLvoid*)(i * 16));
	        glVertexAttribDivisor(location, 1);
	    }

      if (matricesUploadRequired)
      {
        matricesUploadRequired = false;

        switch (matricesUsage)
        {
          case GL_STATIC_DRAW:
          {
            if (matricesAllocatedSize == 0)
            {
              matricesAllocatedSize = matrices.size();
              glBufferData(GL_ARRAY_BUFFER, matrices.size() * sizeof(glm::mat4), matrices.data(), GL_STATIC_DRAW);
            }
          }
          break;

          case GL_DYNAMIC_DRAW:
          default:
          {
            if (matricesAllocatedSize < matrices.size())
            {
              matricesAllocatedSize = matrices.size();
              glBufferData(GL_ARRAY_BUFFER, matrices.size() * sizeof(glm::mat4), nullptr, GL_DYNAMIC_DRAW);
            }

            glBufferSubData(GL_ARRAY_BUFFER, 0, matrices.size() * sizeof(glm::mat4), matrices.data());
          }
          break;
        }
      }
    }

    void InstanceBuffer::bindColors(const ShaderProgram &shader)
    {
    	if (!colors.empty())
    	{
	    	if (colorsVboId == 0)
	      {
	        glGenBuffers(1, &colorsVboId);
	      }
        else if (colors.size() > colorsAllocatedSize)
        {
          colorsUploadRequired = true;
          colorsAllocatedSize = 0;

          glDeleteBuffers(1, &colorsVboId);
          glGenBuffers(1, &colorsVboId);
        }

	      glBindBuffer(GL_ARRAY_BUFFER, colorsVboId);
	    	glEnableVertexAttribArray(shader->colorLocation);
	      glVertexAttribPointer(shader->colorLocation, 4, GL_FLOAT, GL_FALSE, 0, nullptr);
	      glVertexAttribDivisor(shader->colorLocation, 1);

        if (colorsUploadRequired)
        {
          colorsUploadRequired = false;

          switch (colorsUsage)
          {
            case GL_STATIC_DRAW:
            {
              if (colorsAllocatedSize == 0)
              {
                colorsAllocatedSize = colors.size();
                glBufferData(GL_ARRAY_BUFFER, colors.size() * sizeof(glm::vec4), colors.data(), GL_STATIC_DRAW);
              }
            }
            break;

            case GL_DYNAMIC_DRAW:
            default:
            {
              if (colorsAllocatedSize < colors.size())
              {
                colorsAllocatedSize = colors.size();
                glBufferData(GL_ARRAY_BUFFER, colors.size() * sizeof(glm::vec4), nullptr, GL_DYNAMIC_DRAW);
              }

              glBufferSubData(GL_ARRAY_BUFFER, 0, colors.size() * sizeof(glm::vec4), colors.data());
            }
            break;
          }
        }
      }
    }
  }
}
