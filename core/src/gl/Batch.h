#pragma once

#include "gl/Buffer.h"

namespace chr
{
  namespace gl
  {
    class Batch
    {
    public:
      Batch() = default;
    };

    template<int V = 0>
    class VertexBatch : public Batch
    {
    public:
      Buffer<Vertex<V>> vertexBuffer;

      VertexBatch()
      :
      Batch()
      {}

      VertexBatch(const Buffer<Vertex<V>> &vertexBuffer)
      :
      vertexBuffer(vertexBuffer),
      Batch()
      {}

      inline std::vector<Vertex<V>>& vertices() const { return vertexBuffer.storage; }
    };

    template<int V = 0, typename I = GLushort>
    class IndexedVertexBatch : public VertexBatch<V>
    {
    public:
      Buffer<I> indexBuffer;

      IndexedVertexBatch()
      :
      VertexBatch<V>()
      {}

      IndexedVertexBatch(const Buffer<Vertex<V>> &vertexBuffer)
      :
      VertexBatch<V>(vertexBuffer)
      {}

      IndexedVertexBatch(const Buffer<Vertex<V>> &vertexBuffer, const Buffer<I> &indexBuffer)
      :
      indexBuffer(indexBuffer),
      IndexedVertexBatch(vertexBuffer)
      {}

      inline std::vector<I>& indices() const { return indexBuffer.storage; }
    };
  }
}
