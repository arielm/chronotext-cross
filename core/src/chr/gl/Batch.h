#pragma once

#include "chr/gl/Buffer.h"
#include "chr/gl/State.h"
#include "chr/gl/Texture.h"
#include "chr/gl/InstanceBuffer.h"

#include <array>

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
      virtual void bind(ShaderProgram &shader, InstanceBuffer &instanceBuffer) = 0;
    };

    template<int V = XYZ>
    class VertexBatch : public Batch
    {
    public:
      GLenum primitive = GL_TRIANGLES;
      Buffer<Vertex<V>> vertexBuffer;

      ShaderProgram shader;
      bool hasShader = false;

      glm::vec4 color;
      bool hasColor = false;

      Texture texture;
      bool hasTexture = false;

      VertexBatch()
      :
      Batch()
      {}

      VertexBatch(GLenum primitive)
      :
      primitive(primitive),
      Batch()
      {}

      VertexBatch(const Buffer<Vertex<V>> &vertexBuffer)
      :
      vertexBuffer(vertexBuffer),
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

      void extendVertexCapacity(size_t count)
      {
        vertexBuffer.extendCapacity(count);
      }

      void flush()
      {
        if (hasTexture)
        {
          texture.bind();
        }

        if (hasShader)
        {
          shader.bind();
          state::current.apply(shader);
          apply(shader);
          bind(shader);
        }
        else
        {
          apply(state::current->shader);
          bind(state::current->shader);
        }
      }

      void flush(InstanceBuffer &instanceBuffer)
      {
        if (hasTexture)
        {
          texture.bind();
        }

        if (hasShader)
        {
          shader.bind();
          state::current.apply(shader);
          apply(shader);
          bind(shader, instanceBuffer);
        }
        else
        {
          apply(state::current->shader);
          bind(state::current->shader, instanceBuffer);
        }
      }

      inline std::vector<Vertex<V>>& vertices() const
      {
        return vertexBuffer->storage;
      }

      inline Vertex<V>& vertex(size_t i) const
      {
        return vertexBuffer->storage[i];
      }

      inline size_t vertexCount() const
      {
        return vertexBuffer->storage.size();
      }

      inline VertexBatch& addVertex(const Vertex<V> &vertex)
      {
        vertexBuffer->storage.push_back(vertex);
        return *this;
      }

      template<typename... Args>
      inline VertexBatch& addVertex(Args&&... args)
      {
        vertexBuffer->storage.emplace_back(std::forward<Args>(args)...);
        return *this;
      }

      template<typename... Args>
      inline VertexBatch& addVertices(Args&&... args)
      {
        for (Vertex<V>&& vertex : {args...})
        {
          vertexBuffer->storage.emplace_back(vertex);
        }

        return *this;
      }

      inline VertexBatch& addVertices(const std::vector<Vertex<V>> &vertices)
      {
        vertexBuffer->storage.insert(vertexBuffer->storage.end(), vertices.begin(), vertices.end());
        return *this;
      }

      VertexBatch& setPrimitive(GLenum primitive)
      {
        this->primitive = primitive;
        return *this;
      }

      VertexBatch& setShader(const ShaderProgram &shader)
      {
        if (!hasShader)
        {
          this->shader.purge();
        }

        this->shader = shader;
        hasShader = true;

        return *this;
      }

      VertexBatch& clearShader()
      {
        if (hasShader)
        {
          hasShader = false;
          shader.purge();
        }

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

      VertexBatch& clearShaderColor()
      {
        hasColor = false;
        return *this;
      }

      VertexBatch& setTexture(const Texture &texture)
      {
        if (!hasTexture)
        {
          this->texture.purge();
        }

        this->texture = texture;
        hasTexture = true;

        return *this;
      }

      VertexBatch& clearTexture()
      {
        if (hasTexture)
        {
          hasTexture = false;
          texture.purge();
        }

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

      void bind(ShaderProgram &shader, InstanceBuffer &instanceBuffer) override
      {
        vertexBuffer.bind(shader);
        instanceBuffer.bind(shader);

        vertexBuffer.drawInstanced(primitive, instanceBuffer.getCount());

        instanceBuffer.unbind(shader);
        vertexBuffer.unbind(shader);
      }
    };

    template<int V = XYZ, typename I = GLuint>
    class IndexedVertexBatch : public VertexBatch<V>
    {
    public:
      Buffer<I> indexBuffer;

      IndexedVertexBatch()
      :
      VertexBatch<V>()
      {}

      IndexedVertexBatch(GLenum primitive)
      :
      VertexBatch<V>(primitive)
      {}

      IndexedVertexBatch(const Buffer<Vertex<V>> &vertexBuffer)
      :
      VertexBatch<V>(vertexBuffer)
      {}

      IndexedVertexBatch(GLenum primitive, const Buffer<Vertex<V>> &vertexBuffer)
      :
      VertexBatch<V>(primitive, vertexBuffer)
      {}

      IndexedVertexBatch(const Buffer<Vertex<V>> &vertexBuffer, const Buffer<I> &indexBuffer)
      :
      indexBuffer(indexBuffer),
      VertexBatch<V>(vertexBuffer)
      {}

      IndexedVertexBatch(GLenum primitive, const Buffer<Vertex<V>> &vertexBuffer, const Buffer<I> &indexBuffer)
      :
      indexBuffer(indexBuffer),
      VertexBatch<V>(primitive, vertexBuffer)
      {}

      inline std::vector<I>& indices() const
      {
        return indexBuffer->storage;
      }

      inline I index(size_t i) const
      {
        return indexBuffer->storage[i];
      }

      inline size_t indexCount() const
      {
        indexBuffer->storage.size();
      }

      inline IndexedVertexBatch& addIndex(I offset)
      {
        indexBuffer->storage.emplace_back(idx + offset);
        return *this;
      }

      template<typename... Args>
      inline IndexedVertexBatch& addIndices(Args&&... args)
      {
        for (I offset : {args...})
        {
          indexBuffer->storage.emplace_back(idx + offset);
        }

        return *this;
      }

      inline IndexedVertexBatch& addIndices(const std::vector<I> &indices)
      {
        indexBuffer->storage.insert(indexBuffer->storage.end(), indices.begin(), indices.end());
        return *this;
      }

      inline IndexedVertexBatch& incrementIndices(I increment)
      {
        idx += increment;
        return *this;
      }

      inline I currentIndex() const
      {
        return idx;
      }

      void clearIndices()
      {
        indexBuffer.clear();
        indexBuffer.requestUpload();

        idx = 0;
      }

      void clear() override
      {
        VertexBatch<V>::clear();
        clearIndices();
      }

      bool empty() const override
      {
        return VertexBatch<V>::vertexBuffer.empty() || indexBuffer.empty();
      }

      void extendCapacity(size_t vertexCount, size_t indexCount)
      {
        VertexBatch<V>::vertexBuffer.extendCapacity(vertexCount);
        indexBuffer.extendCapacity(indexCount);
      }

      void extendIndexCapacity(size_t count)
      {
        indexBuffer.extendCapacity(count);
      }

    protected:
      I idx = 0;

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

      void bind(ShaderProgram &shader, InstanceBuffer &instanceBuffer) override
      {
        if (VertexBatch<V>::hasTexture)
        {
          VertexBatch<V>::texture.bind();
        }

        VertexBatch<V>::vertexBuffer.bind(shader);
        indexBuffer.bind(shader);
        instanceBuffer.bind(shader);

        indexBuffer.drawInstanced(VertexBatch<V>::primitive, instanceBuffer.getCount());

        instanceBuffer.unbind(shader);
        VertexBatch<V>::vertexBuffer.unbind(shader);
        indexBuffer.unbind(shader);
      }
    };
  }
}
