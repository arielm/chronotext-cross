#pragma once

#include "gl/Buffer.h"
#include "gl/State.h"
#include "gl/Texture.h"

namespace chr
{
  namespace gl
  {
    class Batch
    {
    public:
      Batch() = default;

      virtual void clear() = 0;
      virtual bool empty() const = 0;
      virtual void bind(ShaderProgram &shader) = 0;
    };

    template<int V = XYZ>
    class VertexBatch : public Batch
    {
    public:
      GLenum primitive;
      Buffer<Vertex<V>> vertexBuffer;

      ShaderProgram shader;
      bool hasShader;

      glm::mat4 matrix;
      bool hasMatrix = false;

      glm::vec4 color;
      bool hasColor = false;

      Texture texture;
      bool hasTexture = false;

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

      // ---

      void clear() override
      {
        vertexBuffer.clear();
        vertexBuffer.requestUpload();
      }

      bool empty() const override
      {
        return vertexBuffer.empty();
      }

      void flush(State &state)
      {
        if (hasTexture)
        {
          texture.bind();
        }

        if (hasShader)
        {
          shader.bind();
          apply(shader);
          state.apply(shader);
          bind(shader);
        }
        else
        {
          apply(state.shader);
          bind(state.shader);
        }
      }

      template<typename... Args>
      inline void addVertex(Args&&... args)
      {
        vertexBuffer->storage.emplace_back(std::forward<Args>(args)...);
      }

      VertexBatch& setShader(const ShaderProgram &shader)
      {
        this->shader = shader;
        hasShader = true;
        return *this;
      }

      VertexBatch& setShaderMatrix(const glm::mat4 &matrix)
      {
        this->matrix = matrix;
        hasMatrix = true;
        return *this;
      }

      VertexBatch& setShaderColor(const glm::vec4 &color)
      {
        this->color = color;
        hasColor = true;
        return *this;
      }

      VertexBatch& setShaderColor(float r, float g, float b, float a)
      {
        color = { r, g, b, a };
        hasColor = true;
        return *this;
      }

      VertexBatch& setTexture(const Texture &texture)
      {
        this->texture = texture;
        hasTexture = true;
        return *this;
      }

      // ---

      inline VertexBatch& setShaderUniform(const std::string &name, int v0) { uniformi[name] = { v0 }; return *this; }
      inline VertexBatch& setShaderUniform(const std::string &name, int v0, int v1) { uniformi[name] = { v0, v1 }; return *this; }
      inline VertexBatch& setShaderUniform(const std::string &name, int v0, int v1, int v2) { uniformi[name] = { v0, v1, v2 }; return *this; }
      inline VertexBatch& setShaderUniform(const std::string &name, int v0, int v1, int v2, int v3) { uniformi[name] = { v0, v1, v2, v3 }; return *this; }

      inline VertexBatch& setShaderUniform(const std::string &name, const glm::ivec2 &v) { uniformi[name] = { v[0], v[1] }; return *this; }
      inline VertexBatch& setShaderUniform(const std::string &name, const glm::ivec3 &v) { uniformi[name] = { v[0], v[1], v[2] }; return *this; }
      inline VertexBatch& setShaderUniform(const std::string &name, const glm::ivec4 &v) { uniformi[name] = { v[0], v[1], v[2], v[3] }; return *this; }

      inline VertexBatch& setShaderUniform(const std::string &name, float v0) { uniformf[name] = { v0 }; return *this; }
      inline VertexBatch& setShaderUniform(const std::string &name, float v0, float v1) { uniformf[name] = { v0, v1 }; return *this; }
      inline VertexBatch& setShaderUniform(const std::string &name, float v0, float v1, float v2) { uniformf[name] = { v0, v1, v2 }; return *this; }
      inline VertexBatch& setShaderUniform(const std::string &name, float v0, float v1, float v2, float v3) { uniformf[name] = { v0, v1, v2, v3 }; return *this; }

      inline VertexBatch& setShaderUniform(const std::string &name, const glm::vec2 &v) { uniformf[name] = { v[0], v[1] }; return *this; }
      inline VertexBatch& setShaderUniform(const std::string &name, const glm::vec3 &v) { uniformf[name] = { v[0], v[1], v[2] }; return *this; }
      inline VertexBatch& setShaderUniform(const std::string &name, const glm::vec4 &v) { uniformf[name] = { v[0], v[1], v[2], v[3] }; return *this; }

      inline VertexBatch& setShaderUniform(const std::string &name, const glm::mat3 &m) { uniformm3[name] = m; return *this; }
      inline VertexBatch& setShaderUniform(const std::string &name, const glm::mat4 &m) { uniformm4[name] = m; return *this; }

    protected:
      std::map <std::string, std::vector<int>> uniformi;
      std::map <std::string, std::vector<float>> uniformf;
      std::map <std::string, glm::mat3> uniformm3;
      std::map <std::string, glm::mat4> uniformm4;

      void apply(ShaderProgram &shader)
      {
        if (hasColor)
        {
          shader.applyColor(color);
        }

        if (hasMatrix)
        {
          shader.applyMatrix(matrix);
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

      void bind(ShaderProgram &shader) override
      {
        vertexBuffer.bind(shader);
        vertexBuffer.draw(primitive);
        vertexBuffer.unbind(shader);
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
          indexBuffer->storage.emplace_back(index + offset);
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

      void bind(ShaderProgram &shader) override
      {
        if (VertexBatch<V>::hasTexture)
        {
          VertexBatch<V>::texture.bind();
        }

        VertexBatch<V>::vertexBuffer.bind(shader);
        indexBuffer.bind(shader);
        indexBuffer.draw(VertexBatch<V>::primitive);

        VertexBatch<V>::vertexBuffer.unbind(shader);
        indexBuffer.unbind(shader);
      }
    };
  }
}
