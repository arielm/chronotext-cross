#pragma once

#include "chr/gl/Texture.h"
#include "chr/gl/Batch.h"

#include <map>

namespace chr
{
  namespace gl
  {
    template<int V = XYZ>
    class BatchMap
    {
    public:
      inline VertexBatch<V>& getBatch(const Texture &texture)
      {
        return map[texture];
      };

      void begin()
      {
        for (auto &it : map)
        {
          it.second.clear();
          it.second.setTexture(it.first);
        }
      }

      void flush()
      {
        for (auto &it : map)
        {
          it.second.flush();
        }
      }

    protected:
      std::map<Texture, VertexBatch<V>> map;
    };

    template<int V = XYZ, typename I = GLuint>
    class IndexedBatchMap
    {
    public:
      inline IndexedVertexBatch<V, I>& getBatch(const Texture &texture)
      {
        return map[texture];
      };

      void begin()
      {
        for (auto &it : map)
        {
          it.second.clear();
          it.second.setTexture(it.first);
        }
      }

      void flush()
      {
        for (auto &it : map)
        {
          it.second.flush();
        }
      }

    protected:
      std::map<Texture, IndexedVertexBatch<V, I>> map;
    };
  }
}
