#pragma once

#include "gl/ShaderProgram.h"

namespace chr
{
  namespace gl
  {
    class State
    {
    public:
      enum
      {
        PROPERTY_GL_DEPTH_MASK,
        PROPERTY_GL_DEPTH_FUNC,
        PROPERTY_GL_BLEND_FUNC,
        PROPERTY_GL_CULL_FACE,
        PROPERTY_GL_FRONT_FACE,
        PROPERTY_GL_LINE_WIDTH,
        PROPERTY_GL_POLYGON_OFFSET
      };

      ShaderProgram shader;
      bool hasShader = false;

      glm::mat4 matrix;
      bool hasMatrix = false;

      glm::vec4 color;
      bool hasColor = false;

      State() = default;

      inline State& setShaderUniform(const std::string &name, int v0) { uniformi[name] = { v0 }; return *this; }
      inline State& setShaderUniform(const std::string &name, int v0, int v1) { uniformi[name] = { v0, v1 }; return *this; }
      inline State& setShaderUniform(const std::string &name, int v0, int v1, int v2) { uniformi[name] = { v0, v1, v2 }; return *this; }
      inline State& setShaderUniform(const std::string &name, int v0, int v1, int v2, int v3) { uniformi[name] = { v0, v1, v2, v3 }; return *this; }

      inline State& setShaderUniform(const std::string &name, const glm::ivec2 &v) { uniformi[name] = { v[0], v[1] }; return *this; }
      inline State& setShaderUniform(const std::string &name, const glm::ivec3 &v) { uniformi[name] = { v[0], v[1], v[2] }; return *this; }
      inline State& setShaderUniform(const std::string &name, const glm::ivec4 &v) { uniformi[name] = { v[0], v[1], v[2], v[3] }; return *this; }

      inline State& setShaderUniform(const std::string &name, float v0) { uniformf[name] = { v0 }; return *this; }
      inline State& setShaderUniform(const std::string &name, float v0, float v1) { uniformf[name] = { v0, v1 }; return *this; }
      inline State& setShaderUniform(const std::string &name, float v0, float v1, float v2) { uniformf[name] = { v0, v1, v2 }; return *this; }
      inline State& setShaderUniform(const std::string &name, float v0, float v1, float v2, float v3) { uniformf[name] = { v0, v1, v2, v3 }; return *this; }

      inline State& setShaderUniform(const std::string &name, const glm::vec2 &v) { uniformf[name] = { v[0], v[1] }; return *this; }
      inline State& setShaderUniform(const std::string &name, const glm::vec3 &v) { uniformf[name] = { v[0], v[1], v[2] }; return *this; }
      inline State& setShaderUniform(const std::string &name, const glm::vec4 &v) { uniformf[name] = { v[0], v[1], v[2], v[3] }; return *this; }

      // ---

      State& setShader(const ShaderProgram &shader);
      State& setShaderMatrix(const glm::mat4 &matrix);
      State& setShaderColor(const glm::vec4 &color);
      State& setShaderColor(float r, float g, float b, float a);

      State& glEnable(GLenum cap);
      State& glDisable(GLenum cap);
      State& glDepthMask(bool flag);
      State& glDepthFunc(GLenum func);
      State& glBlendFunc(GLenum sfactor, GLenum dfactor);
      State& glCullFace(GLenum mode);
      State& glFrontFace(GLenum mode);
      State& glLineWidth(float width);
      State& glPolygonOffset(GLfloat factor, GLfloat units);

      void apply();

    protected:
      std::map <std::string, std::vector<int>> uniformi;
      std::map <std::string, std::vector<float>> uniformf;

      std::map<int, std::vector<unsigned int>> propui;
      std::map<int, std::vector<float>> propf;

      std::map<int, bool> enabled;
    };
  }
}
