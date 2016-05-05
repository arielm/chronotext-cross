#include "gl/State.h"

namespace chr
{
  namespace gl
  {
    State& State::setShader(const ShaderProgram &shader)
    {
      this->shader = shader;
      hasShader = true;
      return *this;
    }

    State& State::setShaderColor(const glm::vec4 &color)
    {
      this->color = color;
      hasColor = true;
      return *this;
    }

    State& State::setShaderColor(float r, float g, float b, float a)
    {
      color = { r, g, b, a };
      hasColor = true;
      return *this;
    }

    // ---

    State& State::glEnable(GLenum cap)
    {
      enabled[cap] = {true};
      return *this;
    }

    State& State::glDisable(GLenum cap)
    {
      enabled[cap] = {false};
      return *this;
    }

    State& State::glDepthMask(bool flag)
    {
      propui[PROPERTY_GL_DEPTH_MASK] = {flag};
      return *this;
    }

    State& State::glDepthFunc(GLenum func)
    {
      propui[PROPERTY_GL_DEPTH_FUNC] = {func};
      return *this;
    }

    State& State::glBlendFunc(GLenum sfactor, GLenum dfactor)
    {
      propui[PROPERTY_GL_BLEND_FUNC] = {sfactor, dfactor};
      return *this;
    }

    State& State::glCullFace(GLenum mode)
    {
      propui[PROPERTY_GL_CULL_FACE] = {mode};
      return *this;
    }

    State& State::glFrontFace(GLenum mode)
    {
      propui[PROPERTY_GL_FRONT_FACE] = {mode};
      return *this;
    }

    State& State::glLineWidth(float width)
    {
      propf[PROPERTY_GL_LINE_WIDTH] = {width};
      return *this;
    }

    State& State::glPolygonOffset(GLfloat factor, GLfloat units)
    {
      propf[PROPERTY_GL_POLYGON_OFFSET] = {factor, units};
      return *this;
    }

    // ---

    void State::apply()
    {
      if (hasShader)
      {
        shader.bind();
        apply(shader);
      }

      for (auto it = enabled.begin(); it != enabled.end(); ++it)
      {
        if (it->second)
        {
          ::glEnable(it->first);
        }
        else
        {
          ::glDisable(it->first);
        }
      }

      for (auto it = propui.begin(); it != propui.end(); ++it)
      {
        switch (it->first)
        {
          case PROPERTY_GL_DEPTH_MASK:
            ::glDepthMask(it->second[0]);
            break;

          case PROPERTY_GL_DEPTH_FUNC:
            ::glDepthFunc(it->second[0]);
            break;

          case PROPERTY_GL_BLEND_FUNC:
            ::glBlendFunc(it->second[0], it->second[1]);
            break;

          case PROPERTY_GL_CULL_FACE:
            ::glCullFace(it->second[0]);
            break;

          case PROPERTY_GL_FRONT_FACE:
            ::glFrontFace(it->second[0]);
            break;

          case PROPERTY_GL_POLYGON_OFFSET:
            ::glPolygonOffset(it->second[0], it->second[1]);
            break;
        }
      }

      for (auto it = propf.begin(); it != propf.end(); ++it)
      {
        switch (it->first)
        {
          case PROPERTY_GL_LINE_WIDTH:
            ::glLineWidth(it->second[0]);
            break;
        }
      }
    }

    void State::apply(ShaderProgram &shader)
    {
      if (hasColor)
      {
        shader.applyColor(color);
      }

      if (hasMatrix[MV])
      {
        shader.applyMatrix<MV>(matrices[MV]);
      }

      if (hasMatrix[MVP])
      {
        shader.applyMatrix<MVP>(matrices[MVP]);
      }

      if (hasMatrix[PROJECTION])
      {
        shader.applyMatrix<PROJECTION>(matrices[PROJECTION]);
      }

      if (hasMatrix[NORMAL])
      {
        shader.applyMatrix<NORMAL>(matrices[NORMAL]);
      }

      for (auto it = uniformi.begin(); it != uniformi.end(); ++it)
      {
        shader.applyUniform(it->first, it->second);
      }

      for (auto it = uniformf.begin(); it != uniformf.end(); ++it)
      {
        shader.applyUniform(it->first, it->second);
      }

      for (auto it = uniformm3.begin(); it != uniformm3.end(); ++it)
      {
        shader.applyUniform(it->first, it->second);
      }

      for (auto it = uniformm4.begin(); it != uniformm4.end(); ++it)
      {
        shader.applyUniform(it->first, it->second);
      }
    }
  }
}
