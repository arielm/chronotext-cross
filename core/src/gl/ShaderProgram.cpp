#include "gl/ShaderProgram.h"

#include "gl/Utils.h"
#include "Utils/Utils.h"

using namespace std;

namespace chr
{
  namespace gl
  {
    ShaderProgram::ShaderProgram(const string &vertexShaderResourcePath, const string &fragmentShaderResourcePath)
    :
    vertexShaderResourcePath(vertexShaderResourcePath),
    fragmentShaderResourcePath(fragmentShaderResourcePath)
    {}

    bool ShaderProgram::reload(experimental::string_view vertexShaderSource, experimental::string_view fragmentShaderSource)
    {
      unload();

      tie(programId, vertexShaderId, fragmentShaderId) = ShaderHelper::loadProgram(vertexShaderSource, fragmentShaderSource);
      mapLocations();

      return bool(programId);
    };

    void ShaderProgram::unload()
    {
      if (programId)
      {
       ShaderHelper::unloadProgram(programId, vertexShaderId, fragmentShaderId);

       programId = 0;
       vertexShaderId = 0;
       fragmentShaderId = 0;

       // ---

       map_uniform1i.clear();
       map_uniform2i.clear();
       map_uniform3i.clear();
       map_uniform4i.clear();
       map_uniform1f.clear();
       map_uniform2f.clear();
       map_uniform3f.clear();
       map_uniform4f.clear();
       map_uniform2fv.clear();
       map_uniform3fv.clear();
       map_uniform4fv.clear();
      }
    }

    bool ShaderProgram::bind()
    {
      if (load())
      {
        glUseProgram(programId);
        return true;
      }

      return false;
    }

    bool ShaderProgram::load()
    {
      if (!programId)
      {
        if (!vertexShaderResourcePath.empty() && !fragmentShaderResourcePath.empty())
        {
          auto vertexShaderSource = utils::readTextFromResource<string>(vertexShaderResourcePath);
          auto fragmentShaderSource = utils::readTextFromResource<string>(fragmentShaderResourcePath);

          reload(vertexShaderSource, fragmentShaderSource);
          mapLocations();
        }
      }

      return bool(programId);
    }

    void ShaderProgram::mapLocations()
    {
      if (programId)
      {
        matrixLocation = glGetUniformLocation(programId, "u_matrix");
        positionLocation = glGetAttribLocation(programId, "a_position");
        colorLocation = glGetAttribLocation(programId, "a_color");
        normalLocation = glGetAttribLocation(programId, "a_normal");
        coordLocation = glGetAttribLocation(programId, "a_coord");
        samplerLocation = glGetUniformLocation(programId, "u_sampler");
      }
    }

    void ShaderProgram::applyMatrix(const glm::mat4 &matrix)
    {
     glUniformMatrix4fv(matrixLocation, 1, GL_FALSE, &matrix[0][0]);
    }

    void ShaderProgram::applyColor(float r, float g, float b, float a)
    {
      glVertexAttrib4f(colorLocation, r, g, b, a);
    }

    void ShaderProgram::applyColor(const glm::vec4 &color)
    {
      glVertexAttrib4fv(colorLocation, &color[0]);
    }

    // ---

    GLuint ShaderProgram::getUniformLocation(std::unordered_map<std::string, GLuint> &map, const std::string &name)
    {
      GLuint location;
      auto found = map.find(name);

      if (found == map.end())
      {
        location = glGetUniformLocation(programId, name.data());
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
