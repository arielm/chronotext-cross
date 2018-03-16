#include "chr/gl/State.h"

namespace chr
{
  namespace gl
  {
    int State::usageCounter = 0;

    namespace state
    {
      State current;
    }

    State::State()
    :
    id(usageCounter++),
    element(new state::Element())
    {
      element->useCount++;
    }

    State::State(const State &other)
    :
    id(other.id),
    element(other.element)
    {
      element->useCount++;
    }

    State& State::operator=(const State &other)
    {
      if (this != &other)
      {
        id = other.id;
        element = other.element;

        element->useCount++;
      }

      return *this;
    }

    State::~State()
    {
      if (--element->useCount == 0)
      {
        delete element;
      }
    }

    State& State::setShader(const ShaderProgram &shader)
    {
      element->shader = shader;
      element->hasShader = true;
      return *this;
    }

    State& State::setShaderColor(const glm::vec4 &color)
    {
      element->color = color;
      element->hasColor = true;
      return *this;
    }

    State& State::setShaderColor(float r, float g, float b, float a)
    {
      element->color = { r, g, b, a };
      element->hasColor = true;
      return *this;
    }

    // ---

    State& State::glEnable(GLenum cap)
    {
      element->enabled[cap] = {true};
      return *this;
    }

    State& State::glDisable(GLenum cap)
    {
      element->enabled[cap] = {false};
      return *this;
    }

    State& State::glDepthMask(bool flag)
    {
      element->propui[PROPERTY_GL_DEPTH_MASK] = {flag};
      return *this;
    }

    State& State::glDepthFunc(GLenum func)
    {
      element->propui[PROPERTY_GL_DEPTH_FUNC] = {func};
      return *this;
    }

    State& State::glBlendFunc(GLenum sfactor, GLenum dfactor)
    {
      element->propui[PROPERTY_GL_BLEND_FUNC] = {sfactor, dfactor};
      return *this;
    }

    State& State::glCullFace(GLenum mode)
    {
      element->propui[PROPERTY_GL_CULL_FACE] = {mode};
      return *this;
    }

    State& State::glFrontFace(GLenum mode)
    {
      element->propui[PROPERTY_GL_FRONT_FACE] = {mode};
      return *this;
    }

    State& State::glLineWidth(float width)
    {
      element->propf[PROPERTY_GL_LINE_WIDTH] = {width};
      return *this;
    }

    State& State::glPointSize(float size)
    {
      element->propf[PROPERTY_GL_POINT_SIZE] = {size};
      return *this;
    }

    State& State::glPolygonOffset(GLfloat factor, GLfloat units)
    {
      element->propf[PROPERTY_GL_POLYGON_OFFSET] = {factor, units};
      return *this;
    }

    // ---

    void State::apply()
    {
      if (element->hasShader)
      {
        element->shader.bind();
        apply(element->shader);
      }

      for (auto it = element->enabled.begin(); it != element->enabled.end(); ++it)
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

      for (auto it = element->propui.begin(); it != element->propui.end(); ++it)
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
        }
      }

      for (auto it = element->propf.begin(); it != element->propf.end(); ++it)
      {
        switch (it->first)
        {
          case PROPERTY_GL_POLYGON_OFFSET:
            ::glPolygonOffset(it->second[0], it->second[1]);
            break;

          case PROPERTY_GL_LINE_WIDTH:
            ::glLineWidth(it->second[0]);
            break;

          case PROPERTY_GL_POINT_SIZE:
            #ifdef CHR_PLATFORM_DESKTOP
              ::glPointSize(it->second[0]);
            #endif
            break;
        }
      }

      if (--state::current->useCount == 0)
      {
        delete state::current.element;
      }

      state::current = *this;
    }

    void State::apply(ShaderProgram &shader)
    {
      if (element->hasColor)
      {
        shader.applyColor(element->color);
      }

      if (element->hasMatrix4[MV])
      {
        shader.applyMatrix<MV>(element->matrices4[MV]);
      }

      if (element->hasMatrix4[MVP])
      {
        shader.applyMatrix<MVP>(element->matrices4[MVP]);
      }

      if (element->hasMatrix4[PROJECTION])
      {
        shader.applyMatrix<PROJECTION>(element->matrices4[PROJECTION]);
      }

      if (element->hasMatrix3[NORMAL])
      {
        shader.applyMatrix<NORMAL>(element->matrices3[NORMAL]);
      }

      for (auto it = element->uniformi.begin(); it != element->uniformi.end(); ++it)
      {
        shader.applyUniform(it->first, it->second);
      }

      for (auto it = element->uniformf.begin(); it != element->uniformf.end(); ++it)
      {
        shader.applyUniform(it->first, it->second);
      }

      for (auto it = element->uniformm3.begin(); it != element->uniformm3.end(); ++it)
      {
        shader.applyUniform(it->first, it->second);
      }

      for (auto it = element->uniformm4.begin(); it != element->uniformm4.end(); ++it)
      {
        shader.applyUniform(it->first, it->second);
      }
    }
  }
}
