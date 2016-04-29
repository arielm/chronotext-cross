#include "gl/State.h"

namespace chr
{
  namespace gl
  {
    void State::setTexture(const Texture &texture)
    {
      this->texture = texture;
      hasTexture = true;
    }

    void State::setShader(const ShaderProgram &shader)
    {
      this->shader = shader;
      hasShader = true;
    }

    void State::setShaderMatrix(const glm::mat4 &matrix)
    {
      propm[PROPERTY_SHADER_MATRIX] = matrix;
    }

    void State::setShaderColor(const glm::vec4 &color)
    {
      propf[PROPERTY_SHADER_COLOR] = {color.r, color.g, color.b, color.a};
    }

    void State::setShaderColor(float r, float g, float b, float a)
    {
      propf[PROPERTY_SHADER_COLOR] = {r, g, b, a};
    }

    // ---

    void State::glEnable(GLenum cap)
    {
      propui[PROPERTY_GL_ENABLE] = {cap};
    }

    void State::glDisable(GLenum cap)
    {
      propui[PROPERTY_GL_DISABLE] = {cap};
    }

    void State::glDepthMask(bool flag)
    {
      propui[PROPERTY_GL_DEPTH_MASK] = {flag};
    }

    void State::glDepthFunc(GLenum func)
    {
      propui[PROPERTY_GL_DEPTH_FUNC] = {func};
    }

    void State::glBlendFunc(GLenum sfactor, GLenum dfactor)
    {
      propui[PROPERTY_GL_BLEND_FUNC] = {sfactor, dfactor};
    }

    void State::glCullFace(GLenum mode)
    {
      propui[PROPERTY_GL_CULL_FACE] = {mode};
    }

    void State::glFrontFace(GLenum mode)
    {
      propui[PROPERTY_GL_FRONT_FACE] = {mode};
    }

    void State::glLineWidth(float width)
    {
      propf[PROPERTY_GL_LINE_WIDTH] = {width};
    }

    void State::glPolygonOffset(GLfloat factor, GLfloat units)
    {
      propf[PROPERTY_GL_POLYGON_OFFSET] = {factor, units};
    }

    // ---

    void State::apply()
    {
      if (hasShader)
      {
        shader.bind();

        if (hasTexture)
        {
          texture.bind();
        }

        // ---

        for (auto it = uniformi.begin(); it != uniformi.end(); ++it)
        {
          shader.applyUniform(it->first, it->second);
        }

        for (auto it = uniformf.begin(); it != uniformf.end(); ++it)
        {
          shader.applyUniform(it->first, it->second);
        }

        // ---

        for (auto it = propui.begin(); it != propui.end(); ++it)
        {
          switch (it->first)
          {
            case PROPERTY_GL_ENABLE:
              ::glEnable(it->second[0]);
              break;

            case PROPERTY_GL_DISABLE:
              ::glDisable(it->second[0]);
              break;

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
            case PROPERTY_SHADER_COLOR:
              glVertexAttrib4fv(shader.element->colorLocation, it->second.data());
              break;

            case PROPERTY_GL_LINE_WIDTH:
              ::glLineWidth(it->second[0]);
              break;
          }
        }

        for (auto it = propm.begin(); it != propm.end(); ++it)
        {
          switch (it->first)
          {
            case PROPERTY_SHADER_MATRIX:
              glUniformMatrix4fv(shader.element->matrixLocation, 1, GL_FALSE, glm::value_ptr(it->second));
              break;
          }
        }
      }
    }
  }
}
