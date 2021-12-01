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
    namespace batch
    {
      template<int V>
      class Element1;

      template<int V, class I>
      class Element2;
    }

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
      batch::Element1<V>* operator-> () const
      {
        return element1;
      }

      VertexBatch()
      :
      element1(new batch::Element1<V>()),
      Batch()
      {
        element1->useCount++;
      }

      VertexBatch(GLenum primitive)
      :
      VertexBatch()
      {
        element1->primitive = primitive;
      }

      VertexBatch(const Buffer<Vertex<V>> &vertexBuffer)
      :
      VertexBatch()
      {
        element1->vertexBuffer = vertexBuffer;
      }

      VertexBatch(GLenum primitive, const Buffer<Vertex<V>> &vertexBuffer)
      :
      VertexBatch()
      {
        element1->primitive = primitive;
        element1->vertexBuffer = vertexBuffer;
      }

      VertexBatch(const VertexBatch &other)
      :
      element1(other.element1)
      {
        element1->useCount++;
      }

      VertexBatch& operator=(const VertexBatch &other)
      {
        if (this != &other)
        {
          element1 = other.element1;
          element1->useCount++;
        }

        return *this;
      }

      ~VertexBatch()
      {
        element1->useCount--;

        if (element1->useCount == 0)
        {
          delete element1;
        }
      }

      // ---

      void clear() override
      {
        element1->vertexBuffer.clear();
        element1->vertexBuffer.requestUpload();
      }

      bool empty() const override
      {
        return element1->vertexBuffer.empty();
      }

      void extendVertexCapacity(size_t count)
      {
        element1->vertexBuffer.extendCapacity(count);
      }

      void flush()
      {
        if (element1->hasTexture)
        {
          element1->texture.bind();
        }

        if (element1->hasShader)
        {
          element1->shader.bind();
          state::current.apply(element1->shader);
          apply(element1->shader);
          bind(element1->shader);
        }
        else
        {
          apply(state::current->shader);
          bind(state::current->shader);
        }
      }

      void flush(InstanceBuffer &instanceBuffer)
      {
        if (element1->hasTexture)
        {
          element1->texture.bind();
        }

        if (element1->hasShader)
        {
          element1->shader.bind();
          state::current.apply(element1->shader);
          apply(element1->shader);
          bind(element1->shader, instanceBuffer);
        }
        else
        {
          apply(state::current->shader);
          bind(state::current->shader, instanceBuffer);
        }
      }

      inline Buffer<Vertex<V>>& vertexBuffer() const
      {
        return element1->vertexBuffer;
      }

      inline std::vector<Vertex<V>>& vertices() const
      {
        return element1->vertexBuffer->storage;
      }

      inline Vertex<V>& vertex(size_t i) const
      {
        return element1->vertexBuffer->storage[i];
      }

      inline size_t vertexCount() const
      {
        return element1->vertexBuffer->storage.size();
      }

      inline VertexBatch& addVertex(const Vertex<V> &vertex)
      {
        element1->vertexBuffer->storage.push_back(vertex);
        return *this;
      }

      template<typename... Args>
      inline VertexBatch& addVertex(Args&&... args)
      {
        element1->vertexBuffer->storage.emplace_back(std::forward<Args>(args)...);
        return *this;
      }

      template<typename... Args>
      inline VertexBatch& addVertices(Args&&... args)
      {
        for (Vertex<V>&& vertex : {args...})
        {
          element1->vertexBuffer->storage.emplace_back(vertex);
        }

        return *this;
      }

      inline VertexBatch& addVertices(const std::vector<Vertex<V>> &vertices)
      {
        element1->vertexBuffer->storage.insert(element1->vertexBuffer->storage.end(), vertices.begin(), vertices.end());
        return *this;
      }

      VertexBatch& setPrimitive(GLenum primitive)
      {
        element1->primitive = primitive;
        return *this;
      }

      VertexBatch& setShader(const ShaderProgram &shader)
      {
        if (!element1->hasShader)
        {
          element1->shader.purge();
        }

        element1->shader = shader;
        element1->hasShader = true;

        return *this;
      }

      VertexBatch& clearShader()
      {
        if (element1->hasShader)
        {
          element1->hasShader = false;
          element1->shader.purge();
        }

        return *this;
      }

      VertexBatch& setShaderColor(const glm::vec4 &color)
      {
        element1->color = color;
        element1->hasColor = true;
        return *this;
      }

      VertexBatch& setShaderColor(float r, float g, float b, float a)
      {
        element1->color = { r, g, b, a };
        element1->hasColor = true;
        return *this;
      }

      VertexBatch& clearShaderColor()
      {
        element1->hasColor = false;
        return *this;
      }

      VertexBatch& setTexture(const Texture &texture)
      {
        if (!element1->hasTexture)
        {
          element1->texture.purge();
        }

        element1->texture = texture;
        element1->hasTexture = true;

        return *this;
      }

      VertexBatch& clearTexture()
      {
        if (element1->hasTexture)
        {
          element1->hasTexture = false;
          element1->texture.purge();
        }

        return *this;
      }

      // ---

      inline VertexBatch& setShaderUniform(const std::string &name, int v0) { element1->uniformi[name] = { v0 }; return *this; }
      inline VertexBatch& setShaderUniform(const std::string &name, int v0, int v1) { element1->uniformi[name] = { v0, v1 }; return *this; }
      inline VertexBatch& setShaderUniform(const std::string &name, int v0, int v1, int v2) { element1->uniformi[name] = { v0, v1, v2 }; return *this; }
      inline VertexBatch& setShaderUniform(const std::string &name, int v0, int v1, int v2, int v3) { element1->uniformi[name] = { v0, v1, v2, v3 }; return *this; }

      inline VertexBatch& setShaderUniform(const std::string &name, const glm::ivec2 &v) { element1->uniformi[name] = { v[0], v[1] }; return *this; }
      inline VertexBatch& setShaderUniform(const std::string &name, const glm::ivec3 &v) { element1->uniformi[name] = { v[0], v[1], v[2] }; return *this; }
      inline VertexBatch& setShaderUniform(const std::string &name, const glm::ivec4 &v) { element1->uniformi[name] = { v[0], v[1], v[2], v[3] }; return *this; }

      inline VertexBatch& setShaderUniform(const std::string &name, float v0) { element1->uniformf[name] = { v0 }; return *this; }
      inline VertexBatch& setShaderUniform(const std::string &name, float v0, float v1) { element1->uniformf[name] = { v0, v1 }; return *this; }
      inline VertexBatch& setShaderUniform(const std::string &name, float v0, float v1, float v2) { element1->uniformf[name] = { v0, v1, v2 }; return *this; }
      inline VertexBatch& setShaderUniform(const std::string &name, float v0, float v1, float v2, float v3) { element1->uniformf[name] = { v0, v1, v2, v3 }; return *this; }

      inline VertexBatch& setShaderUniform(const std::string &name, const glm::vec2 &v) { element1->uniformf[name] = { v[0], v[1] }; return *this; }
      inline VertexBatch& setShaderUniform(const std::string &name, const glm::vec3 &v) { element1->uniformf[name] = { v[0], v[1], v[2] }; return *this; }
      inline VertexBatch& setShaderUniform(const std::string &name, const glm::vec4 &v) { element1->uniformf[name] = { v[0], v[1], v[2], v[3] }; return *this; }

      inline VertexBatch& setShaderUniform(const std::string &name, const glm::mat3 &m) { element1->uniformm3[name] = m; return *this; }
      inline VertexBatch& setShaderUniform(const std::string &name, const glm::mat4 &m) { element1->uniformm4[name] = m; return *this; }

    protected:
      batch::Element1<V> *element1 = nullptr;

      void apply(ShaderProgram &shader)
      {
        if (element1->hasColor)
        {
          shader.applyColor(element1->color);
        }

        for (auto it = element1->uniformi.begin(); it != element1->uniformi.end(); ++it)
        {
          shader.applyUniform(it->first, it->second);
        }

        for (auto it = element1->uniformf.begin(); it != element1->uniformf.end(); ++it)
        {
          shader.applyUniform(it->first, it->second);
        }

        for (auto it = element1->uniformm3.begin(); it != element1->uniformm3.end(); ++it)
        {
          shader.applyUniform(it->first, it->second);
        }

        for (auto it = element1->uniformm4.begin(); it != element1->uniformm4.end(); ++it)
        {
          shader.applyUniform(it->first, it->second);
        }
      }

      void bind(ShaderProgram &shader) override
      {
        element1->vertexBuffer.bind(shader);
        element1->vertexBuffer.draw(element1->primitive);
        element1->vertexBuffer.unbind(shader);
      }

      void bind(ShaderProgram &shader, InstanceBuffer &instanceBuffer) override
      {
        element1->vertexBuffer.bind(shader);
        instanceBuffer.bind(shader);

        element1->vertexBuffer.drawInstanced(element1->primitive, instanceBuffer.getCount());

        instanceBuffer.unbind(shader);
        element1->vertexBuffer.unbind(shader);
      }
    };

    template<int V = XYZ, typename I = GLuint>
    class IndexedVertexBatch : public VertexBatch<V>
    {
    public:
      IndexedVertexBatch()
      :
      element2(new batch::Element2<V, I>()),
      VertexBatch<V>()
      {}

      IndexedVertexBatch(GLenum primitive)
      :
      element2(new batch::Element2<V, I>()),
      VertexBatch<V>(primitive)
      {}

      IndexedVertexBatch(const Buffer<Vertex<V>> &vertexBuffer)
      :
      element2(new batch::Element2<V, I>()),
      VertexBatch<V>(vertexBuffer)
      {}

      IndexedVertexBatch(GLenum primitive, const Buffer<Vertex<V>> &vertexBuffer)
      :
      element2(new batch::Element2<V, I>()),
      VertexBatch<V>(primitive, vertexBuffer)
      {}

      IndexedVertexBatch(const Buffer<Vertex<V>> &vertexBuffer, const Buffer<I> &indexBuffer)
      :
      element2(new batch::Element2<V, I>()),
      VertexBatch<V>(vertexBuffer)
      {
        element2->indexBuffer = indexBuffer;
      }

      IndexedVertexBatch(GLenum primitive, const Buffer<Vertex<V>> &vertexBuffer, const Buffer<I> &indexBuffer)
      :
      element2(new batch::Element2<V, I>()),
      VertexBatch<V>(primitive, vertexBuffer)
      {
        element2->indexBuffer = indexBuffer;
      }

      IndexedVertexBatch(const IndexedVertexBatch &other)
      :
      element2(other.element2)
      {
        VertexBatch<V>::element1 = other.element1;
        VertexBatch<V>::element1->useCount++;
      }

      IndexedVertexBatch& operator=(const IndexedVertexBatch &other)
      {
        if (this != &other)
        {
          VertexBatch<V>::element1 = other.element1;
          element2 = other.element2;

          VertexBatch<V>::element1->useCount++;
        }

        return *this;
      }

      ~IndexedVertexBatch()
      {
        if (VertexBatch<V>::element1->useCount == 1)
        {
          delete element2;
        }
      }

      // ---

      inline Buffer<I>& indexBuffer() const
      {
        return element2->indexBuffer;
      }

      inline std::vector<I>& indices() const
      {
        return element2->indexBuffer->storage;
      }

      inline I index(size_t i) const
      {
        return element2->indexBuffer->storage[i];
      }

      inline size_t indexCount() const
      {
        return element2->indexBuffer->storage.size();
      }

      inline IndexedVertexBatch& addIndex(I offset)
      {
        element2->indexBuffer->storage.emplace_back(element2->idx + offset);
        return *this;
      }

      template<typename... Args>
      inline IndexedVertexBatch& addIndices(Args&&... args)
      {
        for (I offset : {args...})
        {
          element2->indexBuffer->storage.emplace_back(element2->idx + offset);
        }

        return *this;
      }

      inline IndexedVertexBatch& addIndices(const std::vector<I> &indices)
      {
        element2->indexBuffer->storage.insert(element2->indexBuffer->storage.end(), indices.begin(), indices.end());
        return *this;
      }

      inline IndexedVertexBatch& incrementIndices(I increment)
      {
        element2->idx += increment;
        return *this;
      }

      inline I currentIndex() const
      {
        return element2->idx;
      }

      void clearIndices()
      {
        element2->indexBuffer.clear();
        element2->indexBuffer.requestUpload();

        element2->idx = 0;
      }

      void clear() override
      {
        VertexBatch<V>::clear();
        clearIndices();
      }

      bool empty() const override
      {
        return VertexBatch<V>::element1->vertexBuffer.empty() && element2->indexBuffer.empty();
      }

      void extendCapacity(size_t vertexCount, size_t indexCount)
      {
        VertexBatch<V>::vertexBuffer.extendCapacity(vertexCount);
        element2->indexBuffer.extendCapacity(indexCount);
      }

      void extendIndexCapacity(size_t count)
      {
        element2->indexBuffer.extendCapacity(count);
      }

    protected:
      batch::Element2<V, I> *element2 = nullptr;

      void bind(ShaderProgram &shader) override
      {
        if (VertexBatch<V>::element1->hasTexture)
        {
          VertexBatch<V>::element1->texture.bind();
        }

        VertexBatch<V>::element1->vertexBuffer.bind(shader);
        element2->indexBuffer.bind(shader);

        element2->indexBuffer.draw(VertexBatch<V>::element1->primitive);

        VertexBatch<V>::element1->vertexBuffer.unbind(shader);
        element2->indexBuffer.unbind(shader);
      }

      void bind(ShaderProgram &shader, InstanceBuffer &instanceBuffer) override
      {
        if (VertexBatch<V>::element1->hasTexture)
        {
          VertexBatch<V>::element1->texture.bind();
        }

        VertexBatch<V>::element1->vertexBuffer.bind(shader);
        element2->indexBuffer.bind(shader);
        instanceBuffer.bind(shader);

        element2->indexBuffer.drawInstanced(VertexBatch<V>::element1->primitive, instanceBuffer.getCount());

        instanceBuffer.unbind(shader);
        VertexBatch<V>::element1->vertexBuffer.unbind(shader);
        element2->indexBuffer.unbind(shader);
      }
    };

    namespace batch
    {
      template<int V>
      class Element1
      {
      public:
        int useCount = 0;

        GLenum primitive = GL_TRIANGLES;
        Buffer<Vertex<V>> vertexBuffer;

        ShaderProgram shader;
        bool hasShader = false;

        glm::vec4 color;
        bool hasColor = false;

        Texture texture;
        bool hasTexture = false;

      protected:
        std::map <std::string, std::vector<int>> uniformi;
        std::map <std::string, std::vector<float>> uniformf;
        std::map <std::string, glm::mat3> uniformm3;
        std::map <std::string, glm::mat4> uniformm4;

        friend class VertexBatch<V>;
      };

      template<int V, class I>
      class Element2
      {
      protected:
        Buffer<I> indexBuffer;
        I idx = 0;

        friend class IndexedVertexBatch<V, I>;
      };
    }
  }
}
