#pragma once

#include "gl/gl.h"
#include "gl/Vertex.h"

#include <unordered_map>

namespace chr
{
  namespace gl
  {
    enum TypeIndex : size_t
    {
      VERTEX,
      VERTEX_UV,
      VERTEX_N,
      VERTEX_RGBA,
      VERTEX_N_UV,
      VERTEX_N_UV_RGBA,
      VERTEX_UV_RGBA,
      VERTEX_N_RGBA,
      VEC2,
      VEC3,
      VEC4,
      GLUSHORT,
      GLUINT,
    };

    template<class T>
    struct TypeTraits
    {};

    template<class T>
    struct Element
    {
      std::vector<T> storage;
      size_t allocatedSize;
      int useCount;
      GLuint vboId;
    };

    template<typename T>
    class Buffer
    {
    public:
      static constexpr size_t typeIndex = TypeTraits<T>::typeIndex;
      static constexpr GLenum target = TypeTraits<T>::target;

      GLenum usage;
      int bufferId;
      std::vector<T> &storage;

      Buffer(GLenum usage = GL_DYNAMIC_DRAW)
      :
      usage(usage),
      bufferId(TypeTraits<T>::counter),
      storage(TypeTraits<T>::map.emplace(TypeTraits<T>::counter++, Element<T>()).first->second.storage)
      {
        element().useCount++;
      }

      Buffer(const Buffer &other)
      :
      usage(other.usage),
      bufferId(other.bufferId),
      storage(other.storage)
      {
        element().useCount++;
      }

      Buffer& operator=(const Buffer &other)
      {
        if (this != &other)
        {
          usage = other.usage;
          bufferId = other.bufferId;
          storage = other.storage;

          element().useCount++;
        }

        return *this;
      }

      ~Buffer()
      {
        auto it = TypeTraits<T>::map.find(bufferId);
        it->second.useCount--;

        if (it->second.useCount == 0)
        {
          unbind();
          TypeTraits<T>::map.erase(it);
        }
      }

      void clear()
      {
        storage.clear();
      }

      void reserve(size_t n)
      {
        storage.reserve(n);
      }

      size_t size() const
      {
        return storage.size();
      }

      bool empty() const
      {
        return storage.empty();
      }

      int useCount() const
      {
        return element().useCount;
      }

      void bind()
      {
        auto &e = element();

        if (e.vboId == 0)
        {
          glGenBuffers(1, &e.vboId);
        }

        glBindBuffer(target, e.vboId);
      }

      void unbind()
      {
        auto &e = element();

        if (e.vboId != 0)
        {
          glDeleteBuffers(1, &e.vboId);
          e.vboId = 0;
        }

        glBindBuffer(target, 0);
      }

      void upload()
      {
        bind();

        auto &e = element();

        switch (usage)
        {
          case GL_STATIC_DRAW:
          {
            if (e.allocatedSize == 0)
            {
              e.allocatedSize = e.storage.size();
              glBufferData(target, e.storage.size() * sizeof(T), storage.data(), GL_DYNAMIC_DRAW);
            }
          }
          break;

          case GL_DYNAMIC_DRAW:
          default:
          {
            if (e.allocatedSize < e.storage.size())
            {
              e.allocatedSize = e.storage.size();
              glBufferData(target, e.storage.size() * sizeof(T), nullptr, GL_DYNAMIC_DRAW);
            }

            glBufferSubData(target, 0, e.storage.size() * sizeof(T), storage.data());
          }
          break;
        }
      }

      template<typename... Args>
      inline void add(Args&&... args)
      {
        storage.emplace_back(std::forward<Args>(args)...);
      }

    protected:
      inline Element<T>& element() const
      {
        return TypeTraits<T>::map[bufferId];
      }
    };

    // ---

    template <>
    struct TypeTraits<Vertex<>>
    {
      static constexpr size_t typeIndex = TypeIndex::VERTEX;
      static constexpr GLenum target = GL_ARRAY_BUFFER;

      static int counter;
      static std::unordered_map<int, Element<Vertex<>>> map;
    };

    template <>
    struct TypeTraits<Vertex<UV>>
    {
      static constexpr size_t typeIndex = TypeIndex::VERTEX_UV;
      static constexpr GLenum target = GL_ARRAY_BUFFER;

      static int counter;
      static std::unordered_map<int, Element<Vertex<UV>>> map;
    };

    template <>
    struct TypeTraits<Vertex<N>>
    {
      static constexpr size_t typeIndex = TypeIndex::VERTEX_N;
      static constexpr GLenum target = GL_ARRAY_BUFFER;

      static int counter;
      static std::unordered_map<int, Element<Vertex<N>>> map;
    };

    template <>
    struct TypeTraits<Vertex<RGBA>>
    {
      static constexpr size_t typeIndex = TypeIndex::VERTEX_RGBA;
      static constexpr GLenum target = GL_ARRAY_BUFFER;

      static int counter;
      static std::unordered_map<int, Element<Vertex<RGBA>>> map;
    };

    template <>
    struct TypeTraits<Vertex<N|UV>>
    {
      static constexpr size_t typeIndex = TypeIndex::VERTEX_N_UV;
      static constexpr GLenum target = GL_ARRAY_BUFFER;

      static int counter;
      static std::unordered_map<int, Element<Vertex<N|UV>>> map;
    };

    template <>
    struct TypeTraits<Vertex<N|UV|RGBA>>
    {
    public:
      static constexpr size_t typeIndex = TypeIndex::VERTEX_N_UV_RGBA;
      static constexpr GLenum target = GL_ARRAY_BUFFER;

      static int counter;
      static std::unordered_map<int, Element<Vertex<N|UV|RGBA>>> map;
    };

    template <>
    struct TypeTraits<Vertex<UV|RGBA>>
    {
      static constexpr size_t typeIndex = TypeIndex::VERTEX_UV_RGBA;
      static constexpr GLenum target = GL_ARRAY_BUFFER;

      static int counter;
      static std::unordered_map<int, Element<Vertex<UV|RGBA>>> map;
    };

    template <>
    struct TypeTraits<Vertex<N|RGBA>>
    {
      static constexpr size_t typeIndex = TypeIndex::VERTEX_N_RGBA;
      static constexpr GLenum target = GL_ARRAY_BUFFER;

      static int counter;
      static std::unordered_map<int, Element<Vertex<N|RGBA>>> map;
    };

    template <>
    struct TypeTraits<glm::vec2>
    {
      static constexpr size_t typeIndex = TypeIndex::VEC2;
      static constexpr GLenum target = GL_ARRAY_BUFFER;

      static int counter;
      static std::unordered_map<int, Element<glm::vec2>> map;
    };

    template <>
    struct TypeTraits<glm::vec3>
    {
      static constexpr size_t typeIndex = TypeIndex::VEC3;
      static constexpr GLenum target = GL_ARRAY_BUFFER;

      static int counter;
      static std::unordered_map<int, Element<glm::vec3>> map;
    };

    template <>
    struct TypeTraits<glm::vec4>
    {
      static constexpr size_t typeIndex = TypeIndex::VEC4;
      static constexpr GLenum target = GL_ARRAY_BUFFER;

      static int counter;
      static std::unordered_map<int, Element<glm::vec4>> map;
    };

    template <>
    struct TypeTraits<GLushort>
    {
      static constexpr size_t typeIndex = TypeIndex::GLUSHORT;
      static constexpr GLenum target = GL_ELEMENT_ARRAY_BUFFER;

      static int counter;
      static std::unordered_map<int, Element<GLushort>> map;
    };

    template <>
    struct TypeTraits<GLuint>
    {
      static constexpr size_t typeIndex = TypeIndex::GLUINT;
      static constexpr GLenum target = GL_ELEMENT_ARRAY_BUFFER;

      static int counter;
      static std::unordered_map<int, Element<GLuint>> map;
    };
  }
}
