#pragma once

#include "gl/Buffer.h"
#include "gl/ShaderProgram.h"
#include "gl/Texture.h"

namespace chr
{
  namespace gl
  {
    class Batch
    {
    public:
      Batch() = default;
      Batch(const Batch &other) = delete;
      Batch& operator=(const Batch &other) = delete;

      virtual void bind() = 0;
      virtual void clear() = 0;
      virtual bool empty() const = 0;
    };

    template<int V = XYZ>
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
      Texture texture;
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

      void setTexture(const Texture &texture)
      {
        this->texture = texture;
        hasTexture = true;
      }

      void setShader(const ShaderProgram &shader)
      {
        this->shader = shader;
        hasShader = true;
      }

      void setShaderMatrix(const glm::mat4 &matrix)
      {
        propm[PROPERTY_SHADER_MATRIX] = matrix;
      }

      void setShaderColor(const glm::vec4 &color)
      {
        propf[PROPERTY_SHADER_COLOR] = {color.r, color.g, color.b, color.a};
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

      void clear() override
      {
        vertexBuffer.clear();
        vertexBuffer.requestUpload();
      }

      bool empty() const override
      {
        return vertexBuffer.empty();
      }

      template<typename... Args>
      inline void addVertex(Args&&... args)
      {
        vertexBuffer.storage.emplace_back(std::forward<Args>(args)...);
      }

    protected:
      bool hasTexture = false;
      bool hasShader = false;

      std::map<std::string, std::vector<int>> uniformi;
      std::map<std::string, std::vector<float>> uniformf;

      std::map<int, std::vector<unsigned int>> propui;
      std::map<int, std::vector<float>> propf;
      std::map<int, glm::mat4> propm;

      void apply()
      {
        shader.bind();

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
              glUniformMatrix4fv(shader.element->matrixLocation, 1, GL_FALSE,  glm::value_ptr(it->second));
              break;
          }
        }
      }

      void bind() override
      {
        if (hasShader)
        {
          if (hasTexture)
          {
            texture.bind();
          }

          vertexBuffer.bind(shader);
          vertexBuffer.draw(primitive);
          vertexBuffer.unbind(shader);

          if (hasTexture)
          {
            texture.unbind();
          }
        }
      }
    };

    template<int V = XYZ, typename I = GLushort>
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

      template<typename... Args>
      inline void addIndices(Args&&... args)
      {
        for (I offset : {args...})
        {
          indexBuffer.storage.emplace_back(index + offset);
        }
      }

      inline void incrementIndices(I increment)
      {
        index += increment;
      }

      void clear() override
      {
        VertexBatch<V>::clear();

        indexBuffer.clear();
        indexBuffer.requestUpload();

        index = 0;
      }

      bool empty() const override
      {
        return VertexBatch<V>::vertexBuffer.empty() || indexBuffer.empty();
      }

    protected:
      I index = 0;

      void bind() override
      {
        if (VertexBatch<V>::hasShader)
        {
          if (VertexBatch<V>::hasTexture)
          {
            VertexBatch<V>::texture.bind();
          }

          VertexBatch<V>::vertexBuffer.bind(VertexBatch<V>::shader);
          indexBuffer.bind(VertexBatch<V>::shader);
          indexBuffer.draw(VertexBatch<V>::primitive);

          VertexBatch<V>::vertexBuffer.unbind(VertexBatch<V>::shader);
          indexBuffer.unbind(VertexBatch<V>::shader);

          if (VertexBatch<V>::hasTexture)
          {
            VertexBatch<V>::texture.unbind();
          }
        }
      }
    };
  }
}
