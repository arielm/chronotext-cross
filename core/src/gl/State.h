#pragma once

#include "gl/ShaderProgram.h"
#include "gl/Texture.h"

namespace chr
{
  namespace gl
  {
    class State
    {
    public:
      enum
      {
        PROPERTY_SHADER_MATRIX,
        PROPERTY_SHADER_COLOR,
        PROPERTY_GL_ENABLE,
        PROPERTY_GL_DISABLE,
        PROPERTY_GL_DEPTH_MASK,
        PROPERTY_GL_DEPTH_FUNC,
        PROPERTY_GL_BLEND_FUNC,
        PROPERTY_GL_CULL_FACE,
        PROPERTY_GL_FRONT_FACE,
        PROPERTY_GL_LINE_WIDTH,
        PROPERTY_GL_POLYGON_OFFSET
      };

      Texture texture;
      ShaderProgram shader;

      State() = default;

      inline void setShaderUniform(const std::string &name, int v0) { uniformi[name] = { v0 }; }
      inline void setShaderUniform(const std::string &name, int v0, int v1) { uniformi[name] = { v0, v1 }; }
      inline void setShaderUniform(const std::string &name, int v0, int v1, int v2) { uniformi[name] = { v0, v1, v2 }; }
      inline void setShaderUniform(const std::string &name, int v0, int v1, int v2, int v3) { uniformi[name] = { v0, v1, v2, v3 }; }

      inline void setShaderUniform(const std::string &name, const glm::ivec2 &v) { uniformi[name] = { v[0], v[1] }; }
      inline void setShaderUniform(const std::string &name, const glm::ivec3 &v) { uniformi[name] = { v[0], v[1], v[2] }; }
      inline void setShaderUniform(const std::string &name, const glm::ivec4 &v) { uniformi[name] = { v[0], v[1], v[2], v[3] }; }

      inline void setShaderUniform(const std::string &name, float v0) { uniformf[name] = { v0 }; }
      inline void setShaderUniform(const std::string &name, float v0, float v1) { uniformf[name] = { v0, v1 }; }
      inline void setShaderUniform(const std::string &name, float v0, float v1, float v2) { uniformf[name] = { v0, v1, v2 }; }
      inline void setShaderUniform(const std::string &name, float v0, float v1, float v2, float v3) { uniformf[name] = { v0, v1, v2, v3 }; }

      inline void setShaderUniform(const std::string &name, const glm::vec2 &v) { uniformf[name] = { v[0], v[1] }; }
      inline void setShaderUniform(const std::string &name, const glm::vec3 &v) { uniformf[name] = { v[0], v[1], v[2] }; }
      inline void setShaderUniform(const std::string &name, const glm::vec4 &v) { uniformf[name] = { v[0], v[1], v[2], v[3] }; }

      // ---

      void setTexture(const Texture &texture);
      void setShader(const ShaderProgram &shader);
      void setShaderMatrix(const glm::mat4 &matrix);
      void setShaderColor(const glm::vec4 &color);
      void setShaderColor(float r, float g, float b, float a);

      void glEnable(GLenum cap);
      void glDisable(GLenum cap);
      void glDepthMask(bool flag);
      void glDepthFunc(GLenum func);
      void glBlendFunc(GLenum sfactor, GLenum dfactor);
      void glCullFace(GLenum mode);
      void glFrontFace(GLenum mode);
      void glLineWidth(float width);
      void glPolygonOffset(GLfloat factor, GLfloat units);

      void apply();

    protected:
      bool hasTexture = false;
      bool hasShader = false;

      std::map <std::string, std::vector<int>> uniformi;
      std::map <std::string, std::vector<float>> uniformf;

      std::map<int, std::vector<unsigned int>> propui;
      std::map<int, std::vector<float>> propf;
      std::map<int, glm::mat4> propm;
    };
  }
}
