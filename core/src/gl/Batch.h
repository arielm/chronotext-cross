#pragma once

#include "gl/Buffer.h"
#include "gl/ShaderProgram.h"

namespace chr
{
  namespace gl
  {
    class Batch
    {
    public:
      Batch() = default;

      virtual void bind() = 0;
    };

    template<int V = 0>
    class VertexBatch : public Batch
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

      GLenum primitive;
      ShaderProgram shader;
      Buffer<Vertex<V>> vertexBuffer;

      VertexBatch(GLenum primitive = GL_TRIANGLE_STRIP)
      :
      primitive(primitive),
      Batch()
      {}

      VertexBatch(GLenum primitive, const Buffer<Vertex<V>> &vertexBuffer)
      :
      primitive(primitive),
      vertexBuffer(vertexBuffer),
      Batch()
      {}

      inline std::vector<Vertex<V>>& vertices() const { return vertexBuffer.storage; }

      inline void setShaderUniform(const std::string &name, int v0) { uniformi[name] = { v0 }; }
      inline void setShaderUniform(const std::string &name, int v0, int v1) { uniformi[name] = { v0, v1 }; }
      inline void setShaderUniform(const std::string &name, int v0, int v1, int v2) { uniformi[name] = { v0, v1, v2 }; }
      inline void setShaderUniform(const std::string &name, int v0, int v1, int v2, int v3) { uniformi[name] = { v0, v1, v2, v3 }; }

      inline void setShaderUniform(const std::string &name, const glm::ivec2 &v) { uniformi[name] = std::vector<int>(&v[0], &v[2]); }
      inline void setShaderUniform(const std::string &name, const glm::ivec3 &v) { uniformi[name] = std::vector<int>(&v[0], &v[3]); }
      inline void setShaderUniform(const std::string &name, const glm::ivec4 &v) { uniformi[name] = std::vector<int>(&v[0], &v[4]); }

      inline void setShaderUniform(const std::string &name, float v0) { uniformf[name] = { v0 }; }
      inline void setShaderUniform(const std::string &name, float v0, float v1) { uniformf[name] = { v0, v1 }; }
      inline void setShaderUniform(const std::string &name, float v0, float v1, float v2) { uniformf[name] = { v0, v1, v2 }; }
      inline void setShaderUniform(const std::string &name, float v0, float v1, float v2, float v3) { uniformf[name] = { v0, v1, v2, v3 }; }

      inline void setShaderUniform(const std::string &name, const glm::vec2 &v) { uniformf[name] = std::vector<float>(&v[0], &v[2]); }
      inline void setShaderUniform(const std::string &name, const glm::vec3 &v) { uniformf[name] = std::vector<float>(&v[0], &v[3]); }
      inline void setShaderUniform(const std::string &name, const glm::vec4 &v) { uniformf[name] = std::vector<float>(&v[0], &v[4]); }

      // ---

      void setShader(const ShaderProgram &shader)
      {
        this->shader = shader;
      }

      void setShaderMatrix(const glm::mat4 &matrix)
      {
        propf[PROPERTY_SHADER_MATRIX] = std::vector<float>(&matrix[0][0], &matrix[4][4]);
      }

      void setShaderColor(const glm::vec4 &color)
      {
        propf[PROPERTY_SHADER_COLOR] = std::vector<int>(&color[0], &color[4]);
      }

      void setShaderColor(float r, float g, float b, float a)
      {
        propf[PROPERTY_SHADER_COLOR] = { r, g, b, a };
      }

      // ---

      void glEnable(GLenum cap)
      {
        propui[PROPERTY_GL_ENABLE] = { cap };
      }

      void glDisable(GLenum cap)
      {
        propui[PROPERTY_GL_DISABLE] = { cap };
      }

      void glDepthMask(bool flag)
      {
        propui[PROPERTY_GL_DEPTH_MASK] = { flag };
      }

      void glDepthFunc(GLenum func)
      {
        propui[PROPERTY_GL_DEPTH_FUNC] = { func };
      }

      void glBlendFunc(GLenum sfactor, GLenum dfactor)
      {
        propui[PROPERTY_GL_BLEND_FUNC] = { sfactor, dfactor };
      }

      void glCullFace(GLenum mode)
      {
        propui[PROPERTY_GL_CULL_FACE] = { mode };
      }

      void glFrontFace(GLenum mode)
      {
        propui[PROPERTY_GL_FRONT_FACE] = { mode };
      }

      void glLineWidth(float width)
      {
        propf[PROPERTY_GL_LINE_WIDTH] = { width };
      }

      void glPolygonOffset(GLfloat factor, GLfloat units)
      {
        propf[PROPERTY_GL_POLYGON_OFFSET] = { factor, units };
      }

      // ---

      void flush()
      {
        apply();
        bind();
      }

      template<typename... Args>
      inline void addVertex(Args&&... args)
      {
        vertexBuffer.storage.emplace_back(std::forward<Args>(args)...);
      }

    protected:
      std::map<std::string, std::vector<int>> uniformi;
      std::map<std::string, std::vector<float>> uniformf;

      std::map<int, std::vector<unsigned int>> propui;
      std::map<int, std::vector<float>> propf;

      void apply()
      {
        shader.bind();

        // ---

        for (auto it = uniformi.begin(); it != uniformi.end(); ++it)
        {
          shader.applyUniform(it->first.data(), it->second);
        }

        for (auto it = uniformf.begin(); it != uniformf.end(); ++it)
        {
          shader.applyUniform(it->first.data(), it->second);
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
              ::glEnable(it->second[0]);
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
            case PROPERTY_SHADER_MATRIX:
              glUniformMatrix4fv(shader.element->matrixLocation, 1, GL_FALSE, it->second.data());
              break;

            case PROPERTY_SHADER_COLOR:
              glVertexAttrib4fv(shader.element->colorLocation, it->second.data());
              break;

            case PROPERTY_GL_LINE_WIDTH:
              ::glLineWidth(it->second[0]);
              break;
          }
        }
      }

      void bind() override
      {
        vertexBuffer.bind(shader);
        vertexBuffer.draw(primitive);
        vertexBuffer.unbind(shader);
      }
    };

    template<int V = 0, typename I = GLushort>
    class IndexedVertexBatch : public VertexBatch<V>
    {
    public:
      Buffer<I> indexBuffer;

      IndexedVertexBatch(GLenum primitive = GL_TRIANGLES)
      :
      VertexBatch<V>(primitive)
      {}

      IndexedVertexBatch(GLenum primitive, const Buffer<Vertex<V>> &vertexBuffer)
      :
      VertexBatch<V>(primitive, vertexBuffer)
      {}

      IndexedVertexBatch(GLenum primitive, const Buffer<Vertex<V>> &vertexBuffer, const Buffer<I> &indexBuffer)
      :
      indexBuffer(indexBuffer),
      VertexBatch<V>(primitive, vertexBuffer)
      {}

      inline std::vector<I>& indices() const { return indexBuffer.storage; }

      inline void addIndex(I index)
      {
        indexBuffer.storage.emplace_back(index);
      }

      void bind() override
      {
        VertexBatch<V>::vertexBuffer.bind(VertexBatch<V>::shader);
        indexBuffer.bind();
        indexBuffer.draw(VertexBatch<V>::primitive);
        VertexBatch<V>::vertexBuffer.unbind(VertexBatch<V>::shader);
        indexBuffer.unbind();
      }
    };
  }
}
