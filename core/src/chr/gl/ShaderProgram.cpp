#include "chr/gl/ShaderProgram.h"

#include "chr/gl/Utils.h"
#include "chr/utils/Utils.h"

using namespace std;

namespace chr
{
  namespace gl
  {
    int ShaderProgram::usageCounter = 0;

    ShaderProgram::ShaderProgram(const string &vertexShaderSource, const string &fragmentShaderSource)
    :
    id(usageCounter++),
    element(new shader::Element())
    {
      element->vertexShaderSource = vertexShaderSource;
      element->fragmentShaderSource = fragmentShaderSource;
      element->useCount++;
    }    

    ShaderProgram::ShaderProgram(const InputSource &vertexShaderSource, const InputSource &fragmentShaderSource)
    :
    id(usageCounter++),
    element(new shader::Element())
    {
      element->vertexShaderSource = utils::readText<string>(vertexShaderSource);
      element->fragmentShaderSource = utils::readText<string>(fragmentShaderSource);
      element->useCount++;
    }

    ShaderProgram::ShaderProgram(const ShaderProgram &other)
    :
    id(other.id),
    element(other.element)
    {
      if (element)
      {
        element->useCount++;
      }
    }

    ShaderProgram& ShaderProgram::operator=(const ShaderProgram &other)
    {
      if (this != &other)
      {
        id = other.id;
        element = other.element;

        if (element)
        {
          element->useCount++;
        }
      }

      return *this;
    }

    bool ShaderProgram::operator==(const ShaderProgram& other) const
    {
      return id == other.id;
    }

    ShaderProgram::~ShaderProgram()
    {
      purge();
    }

    bool ShaderProgram::bind()
    {
      if (load())
      {
        glUseProgram(element->programId);
        return true;
      }

      return false;
    }

    void ShaderProgram::unbind()
    {
      if (element && element->programId)
      {
        glUseProgram(0);
      }
    }

    void ShaderProgram::applyColor(float r, float g, float b, float a)
    {
      glVertexAttrib4f(element->colorLocation, r, g, b, a);
    }

    void ShaderProgram::applyColor(const glm::vec4 &color)
    {
      glVertexAttrib4fv(element->colorLocation, &color[0]);
    }

    template <>
    void ShaderProgram::applyMatrix<MV>(const glm::mat4 &matrix)
    {
      glUniformMatrix4fv(element->mvMatrixLocation, 1, GL_FALSE, &matrix[0][0]);
    }

    template <>
    void ShaderProgram::applyMatrix<MVP>(const glm::mat4 &matrix)
    {
      glUniformMatrix4fv(element->mvpMatrixLocation, 1, GL_FALSE, &matrix[0][0]);
    }

    template <>
    void ShaderProgram::applyMatrix<PROJECTION>(const glm::mat4 &matrix)
    {
      glUniformMatrix4fv(element->projectionMatrixLocation, 1, GL_FALSE, &matrix[0][0]);
    }

    template <>
    void ShaderProgram::applyMatrix<MODEL>(const glm::mat4 &matrix)
    {
      glUniformMatrix4fv(element->modelMatrixLocation, 1, GL_FALSE, &matrix[0][0]);
    }

    template <>
    void ShaderProgram::applyMatrix<VIEW>(const glm::mat4 &matrix)
    {
      glUniformMatrix4fv(element->viewMatrixLocation, 1, GL_FALSE, &matrix[0][0]);
    }

    template <>
    void ShaderProgram::applyMatrix<NORMAL>(const glm::mat3 &matrix)
    {
      glUniformMatrix3fv(element->normalMatrixLocation, 1, GL_FALSE, &matrix[0][0]);
    }

    void ShaderProgram::applyUniform(const string &name, const vector<int> &v)
    {
      switch (v.size())
      {
        case 1:
          glUniform1iv(getUniformLocation(name), 1, v.data());
          break;

        case 2:
          glUniform2iv(getUniformLocation(name), 1, v.data());
          break;

        case 3:
          glUniform3iv(getUniformLocation(name), 1, v.data());
          break;

        case 4:
          glUniform4iv(getUniformLocation(name), 1, v.data());
          break;
      }
    }

    void ShaderProgram::applyUniform(const string &name, const vector<float> &v)
    {
      switch (v.size())
      {
        case 1:
          glUniform1fv(getUniformLocation(name), 1, v.data());
          break;

        case 2:
          glUniform2fv(getUniformLocation(name), 1, v.data());
          break;

        case 3:
          glUniform3fv(getUniformLocation(name), 1, v.data());
          break;

        case 4:
          glUniform4fv(getUniformLocation(name), 1, v.data());
          break;
      }
    }

    bool ShaderProgram::load()
    {
      if (element && !element->programId)
      {
        if (!element->vertexShaderSource.empty() && !element->fragmentShaderSource.empty())
        {
          tie(element->programId, element->vertexShaderId, element->fragmentShaderId) = ShaderHelper::loadProgram(element->vertexShaderSource, element->fragmentShaderSource);

          element->mvMatrixLocation = glGetUniformLocation(element->programId, "u_mv_matrix");
          element->mvpMatrixLocation = glGetUniformLocation(element->programId, "u_mvp_matrix");
          element->projectionMatrixLocation = glGetUniformLocation(element->programId, "u_projection_matrix");
          element->modelMatrixLocation = glGetUniformLocation(element->programId, "u_model_matrix");
          element->viewMatrixLocation = glGetUniformLocation(element->programId, "u_view_matrix");
          element->normalMatrixLocation = glGetUniformLocation(element->programId, "u_normal_matrix");
          element->samplerLocation = glGetUniformLocation(element->programId, "u_sampler"); // XXX
          element->positionLocation = glGetAttribLocation(element->programId, "a_position");
          element->colorLocation = glGetAttribLocation(element->programId, "a_color");
          element->normalLocation = glGetAttribLocation(element->programId, "a_normal");
          element->coordLocation = glGetAttribLocation(element->programId, "a_coord");
          element->matrixLocation = glGetAttribLocation(element->programId, "a_matrix");
        }
      }

      return bool(element->programId);
    }

    void ShaderProgram::unload()
    {
      if (element && element->programId)
      {
        ShaderHelper::unloadProgram(element->programId, element->vertexShaderId, element->fragmentShaderId);

        element->programId = 0;
        element->vertexShaderId = 0;
        element->fragmentShaderId = 0;

        element->map_uniform.clear();
      }
    }

    void ShaderProgram::purge()
    {
      if (element)
      {
        element->useCount--;

        if (element->useCount == 0)
        {
          unload();
          delete element;
          element = nullptr;
        }
      }
    }

    GLuint ShaderProgram::getUniformLocation(const string &name)
    {
      GLuint location;
      auto found = element->map_uniform.find(name);

      if (found == element->map_uniform.end())
      {
        location = glGetUniformLocation(element->programId, name.data());
        element->map_uniform.emplace(name, location);
      }
      else
      {
        location = found->second;
      }

      return location;
    }
  }
}
