#include "gl/ShaderProgram.h"

#include "gl/Utils.h"
#include "Utils/Utils.h"

using namespace std;

namespace chr
{
  namespace gl
  {
    int ShaderProgram::usageCounter = 0;

    ShaderProgram::ShaderProgram()
    :
    id(usageCounter++),
    element(new shader::Element())
    {
      element->useCount++;
    }

    ShaderProgram::ShaderProgram(const string &vertexShaderResourcePath, const string &fragmentShaderResourcePath)
    :
    ShaderProgram()
    {
      element->vertexShaderSource = utils::readTextFromResource<string>(vertexShaderResourcePath);
      element->fragmentShaderSource = utils::readTextFromResource<string>(fragmentShaderResourcePath);
    }

    ShaderProgram::ShaderProgram(const ShaderProgram &other)
    :
    id(other.id),
    element(other.element)
    {
      element->useCount++;
    }

    ShaderProgram& ShaderProgram::operator=(const ShaderProgram &other)
    {
      if (this != &other)
      {
        id = other.id;
        element = other.element;

        element->useCount++;
      }

      return *this;
    }

    ShaderProgram::~ShaderProgram()
    {
      element->useCount--;

      if (element->useCount == 0)
      {
        if (element->programId != 0)
        {
          ShaderHelper::unloadProgram(element->programId, element->vertexShaderId, element->fragmentShaderId);
        }

        delete element;
      }
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

    void ShaderProgram::applyMatrix(const glm::mat4 &matrix)
    {
      glUniformMatrix4fv(element->matrixLocation, 1, GL_FALSE, &matrix[0][0]);
    }

    void ShaderProgram::applyColor(float r, float g, float b, float a)
    {
      glVertexAttrib4f(element->colorLocation, r, g, b, a);
    }

    void ShaderProgram::applyColor(const glm::vec4 &color)
    {
      glVertexAttrib4fv(element->colorLocation, &color[0]);
    }

    bool ShaderProgram::load()
    {
      if (!element->programId)
      {
        if (!element->vertexShaderSource.empty() && !element->fragmentShaderSource.empty())
        {
          tie(element->programId, element->vertexShaderId, element->fragmentShaderId) = ShaderHelper::loadProgram(element->vertexShaderSource, element->fragmentShaderSource);

          element->matrixLocation = glGetUniformLocation(element->programId, "u_matrix");
          element->positionLocation = glGetAttribLocation(element->programId, "a_position");
          element->colorLocation = glGetAttribLocation(element->programId, "a_color");
          element->normalLocation = glGetAttribLocation(element->programId, "a_normal");
          element->coordLocation = glGetAttribLocation(element->programId, "a_coord");
          element->samplerLocation = glGetUniformLocation(element->programId, "u_sampler");
        }
      }

      return bool(element->programId);
    }

    void ShaderProgram::unload()
    {
      if (element->programId)
      {
        ShaderHelper::unloadProgram(element->programId, element->vertexShaderId, element->fragmentShaderId);

        element->programId = 0;
        element->vertexShaderId = 0;
        element->fragmentShaderId = 0;

        // ---

        element->map_uniform1i.clear();
        element->map_uniform2i.clear();
        element->map_uniform3i.clear();
        element->map_uniform4i.clear();
        element->map_uniform1f.clear();
        element->map_uniform2f.clear();
        element->map_uniform3f.clear();
        element->map_uniform4f.clear();
        element->map_uniform2fv.clear();
        element->map_uniform3fv.clear();
        element->map_uniform4fv.clear();
      }
    }

    GLuint ShaderProgram::getUniformLocation(std::unordered_map<std::string, GLuint> &map, const std::string &name)
    {
      GLuint location;
      auto found = map.find(name);

      if (found == map.end())
      {
        location = glGetUniformLocation(element->programId, name.data());
        map.emplace(name, location);
      }
      else
      {
        location = found->second;
      }

      return location;
    }
  }
}
