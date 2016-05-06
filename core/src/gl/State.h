#pragma once

#include "gl/ShaderProgram.h"

namespace chr
{
  namespace gl
  {
    class State;

    namespace state
    {
      extern State current;

      struct Element
      {
        int useCount = 0;

        ShaderProgram shader;
        bool hasShader = false;

        glm::vec4 color;
        bool hasColor = false;

        glm::mat3 matrices3[1];
        bool hasMatrix3[1] = { false };

        glm::mat4 matrices4[3];
        bool hasMatrix4[3] = { false, false, false };

        std::map <std::string, std::vector<int>> uniformi;
        std::map <std::string, std::vector<float>> uniformf;
        std::map <std::string, glm::mat3> uniformm3;
        std::map <std::string, glm::mat4> uniformm4;

        std::map<int, std::vector<unsigned int>> propui;
        std::map<int, std::vector<float>> propf;

        std::map<int, bool> enabled;
      };
    }

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
        PROPERTY_GL_POINT_SIZE,
        PROPERTY_GL_POLYGON_OFFSET
      };

      int id;
      state::Element *element;

      State();
      State(const State &other);
      State& operator=(const State &other);

      ~State();

      state::Element* operator-> ()
      {
        return element;
      }

      inline State& setShaderUniform(const std::string &name, int v0) { element->uniformi[name] = { v0 }; return *this; }
      inline State& setShaderUniform(const std::string &name, int v0, int v1) { element->uniformi[name] = { v0, v1 }; return *this; }
      inline State& setShaderUniform(const std::string &name, int v0, int v1, int v2) { element->uniformi[name] = { v0, v1, v2 }; return *this; }
      inline State& setShaderUniform(const std::string &name, int v0, int v1, int v2, int v3) { element->uniformi[name] = { v0, v1, v2, v3 }; return *this; }

      inline State& setShaderUniform(const std::string &name, const glm::ivec2 &v) { element->uniformi[name] = { v[0], v[1] }; return *this; }
      inline State& setShaderUniform(const std::string &name, const glm::ivec3 &v) { element->uniformi[name] = { v[0], v[1], v[2] }; return *this; }
      inline State& setShaderUniform(const std::string &name, const glm::ivec4 &v) { element->uniformi[name] = { v[0], v[1], v[2], v[3] }; return *this; }

      inline State& setShaderUniform(const std::string &name, float v0) { element->uniformf[name] = { v0 }; return *this; }
      inline State& setShaderUniform(const std::string &name, float v0, float v1) { element->uniformf[name] = { v0, v1 }; return *this; }
      inline State& setShaderUniform(const std::string &name, float v0, float v1, float v2) { element->uniformf[name] = { v0, v1, v2 }; return *this; }
      inline State& setShaderUniform(const std::string &name, float v0, float v1, float v2, float v3) { element->uniformf[name] = { v0, v1, v2, v3 }; return *this; }

      inline State& setShaderUniform(const std::string &name, const glm::vec2 &v) { element->uniformf[name] = { v[0], v[1] }; return *this; }
      inline State& setShaderUniform(const std::string &name, const glm::vec3 &v) { element->uniformf[name] = { v[0], v[1], v[2] }; return *this; }
      inline State& setShaderUniform(const std::string &name, const glm::vec4 &v) { element->uniformf[name] = { v[0], v[1], v[2], v[3] }; return *this; }

      inline State& setShaderUniform(const std::string &name, const glm::mat3 &m) { element->uniformm3[name] = m; return *this; }
      inline State& setShaderUniform(const std::string &name, const glm::mat4 &m) { element->uniformm4[name] = m; return *this; }

      // ---

      State& setShader(const ShaderProgram &shader);
      State& setShaderColor(const glm::vec4 &color);
      State& setShaderColor(float r, float g, float b, float a);

      template<int T>
      State& setShaderMatrix(const glm::mat3 &matrix)
      {
        element->matrices3[T] = matrix;
        element->hasMatrix3[T] = true;
        return *this;
      }

      template<int T = MVP>
      State& setShaderMatrix(const glm::mat4 &matrix)
      {
        element->matrices4[T] = matrix;
        element->hasMatrix4[T] = true;
        return *this;
      }

      template<int T = MV>
      State& setShaderMatrix(float *values)
      {
        element->matrices4[T] = glm::make_mat4(values);
        element->hasMatrix4[T] = true;
        return *this;
      }

      State& glEnable(GLenum cap);
      State& glDisable(GLenum cap);
      State& glDepthMask(bool flag);
      State& glDepthFunc(GLenum func);
      State& glBlendFunc(GLenum sfactor, GLenum dfactor);
      State& glCullFace(GLenum mode);
      State& glFrontFace(GLenum mode);
      State& glLineWidth(float width);
      State& glPointSize(float size);
      State& glPolygonOffset(GLfloat factor, GLfloat units);

      void apply();
      void apply(ShaderProgram &shader);

    protected:
      static int usageCounter;
    };
  }
}
