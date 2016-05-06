#pragma once

#include "gl/ShaderHelper.h"

namespace chr
{
  namespace gl
  {
    namespace buffer
    {
      enum TypeIndex : size_t
      {
        VERTEX_XYZ,
        VERTEX_XYZ_N,
        VERTEX_XYZ_N_UV,
        VERTEX_XYZ_N_RGBA,
        VERTEX_XYZ_N_UV_RGBA,
        VERTEX_XYZ_UV,
        VERTEX_XYZ_UV_RGBA,
        VERTEX_XYZ_RGBA,
        VERTEX_N,
        VERTEX_UV,
        VERTEX_RGBA,
        GLUSHORT,
        GLUINT,
      };

      template<class T>
      struct TypeTraits
      {};

      template<class T>
      struct Element
      {
        int useCount = 0;
        std::vector<T> storage;
        size_t allocatedSize;
        GLuint vboId;
        bool uploadRequired = true;
      };
    }

    template<typename T>
    class Buffer
    {
    public:
      static constexpr size_t typeIndex = buffer::TypeTraits<T>::typeIndex;
      static constexpr GLenum target = buffer::TypeTraits<T>::target;

      int id;
      buffer::Element<T> *element;
      GLenum usage;

      buffer::Element<T>* operator->()
      {
        return element;
      }

      Buffer(GLenum usage = GL_DYNAMIC_DRAW)
      :
      id(buffer::TypeTraits<T>::usageCounter++),
      element(new buffer::Element<T>()),
      usage(usage)
      {
        element->useCount++;
      }

      Buffer(const Buffer &other)
      :
      id(other.id),
      element(other.element),
      usage(other.usage)
      {
        element->useCount++;
      }

      Buffer& operator=(const Buffer &other)
      {
        if (this != &other)
        {
          id = other.id;
          element = other.element;
          usage = other.usage;

          element->useCount++;
        }

        return *this;
      }

      ~Buffer()
      {
        element->useCount--;

        if (element->useCount == 0)
        {
          if (element->vboId != 0)
          {
            glDeleteBuffers(1, &element->vboId);
          }

          delete element;
        }
      }

      void clear()
      {
        element->storage.clear();
      }

      void reserve(size_t n)
      {
        element->storage.reserve(n);
      }

      size_t size() const
      {
        return element->storage.size();
      }

      bool empty() const
      {
        return element->storage.empty();
      }

      int useCount() const
      {
        return element->useCount;
      }

      void bind(const ShaderProgram &shader, bool forceUpload = false)
      {
        if (element->vboId == 0)
        {
          glGenBuffers(1, &element->vboId);
        }
        else if (element->storage.size() > element->allocatedSize)
        {
          element->uploadRequired = true;
          element->allocatedSize = 0;

          glDeleteBuffers(1, &element->vboId);
          glGenBuffers(1, &element->vboId);
        }

        switch (typeIndex)
        {
          case buffer::GLUSHORT:
          case buffer::GLUINT:
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, element->vboId);
            break;

          default:
            glBindBuffer(target, element->vboId);
            ShaderHelper::bindAttributes<T>(shader);
            break;
        }

        if (forceUpload || element->uploadRequired)
        {
          upload();
          element->uploadRequired = false;
        }
      }

      void unbind(const ShaderProgram &shader)
      {
        switch (typeIndex)
        {
          case buffer::GLUSHORT:
          case buffer::GLUINT:
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
            break;

          default:
            ShaderHelper::unbindAttributes<T>(shader);
            glBindBuffer(target, 0);
            break;
        }
      }

      void draw(GLenum primitive, size_t count = 0)
      {
        switch (typeIndex)
        {
          case buffer::GLUSHORT:
          case buffer::GLUINT:
            glDrawElements(primitive, count ? count : element->storage.size(), (typeIndex == buffer::GLUSHORT) ? GL_UNSIGNED_SHORT : GL_UNSIGNED_INT, 0);
            break;

          default:
            glDrawArrays(primitive, 0, count ? count : element->storage.size());
            break;
        }
      }

      void requestUpload()
      {
        element->uploadRequired = true;
      }

      template<typename... Args>
      inline void add(Args&&... args)
      {
        element->storage.emplace_back(std::forward<Args>(args)...);
      }

      void upload()
      {
        switch (usage)
        {
          case GL_STATIC_DRAW:
          {
            if (element->allocatedSize == 0)
            {
              element->allocatedSize = element->storage.size();
              glBufferData(target, element->storage.size() * sizeof(T), element->storage.data(), GL_STATIC_DRAW);
            }
          }
          break;

          case GL_DYNAMIC_DRAW:
          default:
          {
            if (element->allocatedSize < element->storage.size())
            {
              element->allocatedSize = element->storage.size();
              glBufferData(target, element->storage.size() * sizeof(T), nullptr, GL_DYNAMIC_DRAW);
            }

            glBufferSubData(target, 0, element->storage.size() * sizeof(T), element->storage.data());
          }
          break;
        }
      }
    };

    // ---

    namespace buffer
    {
      template<>
      struct TypeTraits<Vertex<XYZ>>
      {
        static constexpr size_t typeIndex = TypeIndex::VERTEX_XYZ;
        static constexpr GLenum target = GL_ARRAY_BUFFER;

        static int usageCounter;
      };

      template<>
      struct TypeTraits<Vertex<XYZ.UV>>
      {
        static constexpr size_t typeIndex = TypeIndex::VERTEX_XYZ_UV;
        static constexpr GLenum target = GL_ARRAY_BUFFER;

        static int usageCounter;
      };

      template<>
      struct TypeTraits<Vertex<XYZ.RGBA>>
      {
        static constexpr size_t typeIndex = TypeIndex::VERTEX_XYZ_RGBA;
        static constexpr GLenum target = GL_ARRAY_BUFFER;

        static int usageCounter;
      };

      template<>
      struct TypeTraits<Vertex<XYZ.N>>
      {
        static constexpr size_t typeIndex = TypeIndex::VERTEX_XYZ_N;
        static constexpr GLenum target = GL_ARRAY_BUFFER;

        static int usageCounter;
      };

      template<>
      struct TypeTraits<Vertex<XYZ.UV.RGBA>>
      {
        static constexpr size_t typeIndex = TypeIndex::VERTEX_XYZ_UV_RGBA;
        static constexpr GLenum target = GL_ARRAY_BUFFER;

        static int usageCounter;
      };

      template<>
      struct TypeTraits<Vertex<XYZ.N.UV>>
      {
        static constexpr size_t typeIndex = TypeIndex::VERTEX_XYZ_N_UV;
        static constexpr GLenum target = GL_ARRAY_BUFFER;

        static int usageCounter;
      };

      template<>
      struct TypeTraits<Vertex<XYZ.N.RGBA>>
      {
        static constexpr size_t typeIndex = TypeIndex::VERTEX_XYZ_N_RGBA;
        static constexpr GLenum target = GL_ARRAY_BUFFER;

        static int usageCounter;
      };

      template<>
      struct TypeTraits<Vertex<XYZ.N.UV.RGBA>>
      {
      public:
        static constexpr size_t typeIndex = TypeIndex::VERTEX_XYZ_N_UV_RGBA;
        static constexpr GLenum target = GL_ARRAY_BUFFER;

        static int usageCounter;
      };

      template<>
      struct TypeTraits<Vertex<N>>
      {
        static constexpr size_t typeIndex = TypeIndex::VERTEX_N;
        static constexpr GLenum target = GL_ARRAY_BUFFER;

        static int usageCounter;
      };

      template<>
      struct TypeTraits<Vertex<UV>>
      {
        static constexpr size_t typeIndex = TypeIndex::VERTEX_UV;
        static constexpr GLenum target = GL_ARRAY_BUFFER;

        static int usageCounter;
      };

      template<>
      struct TypeTraits<Vertex<RGBA>>
      {
        static constexpr size_t typeIndex = TypeIndex::VERTEX_RGBA;
        static constexpr GLenum target = GL_ARRAY_BUFFER;

        static int usageCounter;
      };

      template<>
      struct TypeTraits<GLushort>
      {
        static constexpr size_t typeIndex = TypeIndex::GLUSHORT;
        static constexpr GLenum target = GL_ELEMENT_ARRAY_BUFFER;

        static int usageCounter;
      };

      template<>
      struct TypeTraits<GLuint>
      {
        static constexpr size_t typeIndex = TypeIndex::GLUINT;
        static constexpr GLenum target = GL_ELEMENT_ARRAY_BUFFER;

        static int usageCounter;
      };
    }
  }
}
