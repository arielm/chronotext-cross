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
      SIZE
    };

    template<class T>
    struct TypeTraits
    {};

    template<class T>
    struct Element
    {
      std::vector<T> storage;
      int useCount;
      GLuint vboId;
    };

    template<typename T>
    class Buffer
    {
    public:
      static constexpr size_t typeIndex = TypeTraits<T>::typeIndex;

      int bufferId;
      std::vector<T> &storage;

      Buffer()
      :
      bufferId(TypeTraits<T>::counter),
      storage(TypeTraits<T>::map.emplace(TypeTraits<T>::counter++, Element<T>()).first->second.storage)
      {
        TypeTraits<T>::map[bufferId].useCount++;
      }

      Buffer(const Buffer &other)
      :
      bufferId(other.bufferId),
      storage(other.storage)
      {
        TypeTraits<T>::map[bufferId].useCount++;
      }

      Buffer& operator=(const Buffer &other)
      {
        TypeTraits<T>::map[bufferId].useCount++;
        return *this;
      }

      ~Buffer()
      {
        auto it = TypeTraits<T>::map.find(bufferId);
        it->second.useCount--;

        if (it->second.useCount == 0)
        {
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

      int useCount()
      {
        return TypeTraits<T>::map[bufferId].useCount;
      }

      template<typename... Args>
      inline void add(Args&&... args)
      {
        storage.emplace_back(std::forward<Args>(args)...);
      }
    };

    // ---

    template <>
    struct TypeTraits<Vertex<>>
    {
      static constexpr size_t typeIndex = TypeIndex::VERTEX;
      static int counter;
      static std::unordered_map<int, Element<Vertex<>>> map;
    };

    template <>
    struct TypeTraits<Vertex<UV>>
    {
      static constexpr size_t typeIndex = TypeIndex::VERTEX_UV;
      static int counter;
      static std::unordered_map<int, Element<Vertex<UV>>> map;
    };

    template <>
    struct TypeTraits<Vertex<N>>
    {
      static constexpr size_t typeIndex = TypeIndex::VERTEX_N;
      static int counter;
      static std::unordered_map<int, Element<Vertex<N>>> map;
    };

    template <>
    struct TypeTraits<Vertex<RGBA>>
    {
      static constexpr size_t typeIndex = TypeIndex::VERTEX_RGBA;
      static int counter;
      static std::unordered_map<int, Element<Vertex<RGBA>>> map;
    };

    template <>
    struct TypeTraits<Vertex<N|UV>>
    {
      static constexpr size_t typeIndex = TypeIndex::VERTEX_N_UV;
      static int counter;
      static std::unordered_map<int, Element<Vertex<N|UV>>> map;
    };

    template <>
    struct TypeTraits<Vertex<N|UV|RGBA>>
    {
    public:
      static constexpr size_t typeIndex = TypeIndex::VERTEX_N_UV_RGBA;
      static int counter;
      static std::unordered_map<int, Element<Vertex<N|UV|RGBA>>> map;
    };

    template <>
    struct TypeTraits<Vertex<UV|RGBA>>
    {
      static constexpr size_t typeIndex = TypeIndex::VERTEX_UV_RGBA;
      static int counter;
      static std::unordered_map<int, Element<Vertex<UV|RGBA>>> map;
    };

    template <>
    struct TypeTraits<Vertex<N|RGBA>>
    {
      static constexpr size_t typeIndex = TypeIndex::VERTEX_N_RGBA;
      static int counter;
      static std::unordered_map<int, Element<Vertex<N|RGBA>>> map;
    };

    template <>
    struct TypeTraits<glm::vec2>
    {
      static constexpr size_t typeIndex = TypeIndex::VEC2;
      static int counter;
      static std::unordered_map<int, Element<glm::vec2>> map;
    };

    template <>
    struct TypeTraits<glm::vec3>
    {
      static constexpr size_t typeIndex = TypeIndex::VEC3;
      static int counter;
      static std::unordered_map<int, Element<glm::vec3>> map;
    };

    template <>
    struct TypeTraits<glm::vec4>
    {
      static constexpr size_t typeIndex = TypeIndex::VEC4;
      static int counter;
      static std::unordered_map<int, Element<glm::vec4>> map;
    };

    template <>
    struct TypeTraits<GLushort>
    {
      static constexpr size_t typeIndex = TypeIndex::GLUSHORT;
      static int counter;
      static std::unordered_map<int, Element<GLushort>> map;
    };

    template <>
    struct TypeTraits<GLuint>
    {
      static constexpr size_t typeIndex = TypeIndex::GLUINT;
      static int counter;
      static std::unordered_map<int, Element<GLuint>> map;
    };
  }
}
