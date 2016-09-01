#include "chr/gl/Vertex.h"

namespace chr
{
  namespace gl
  {
    template<>
    bool operator==(const Vertex<XYZ> &lhs, const Vertex<XYZ> &rhs)
    {
      return lhs.position == rhs.position;
    }

    template<>
    bool operator==(const Vertex<XYZ.UV> &lhs, const Vertex<XYZ.UV> &rhs)
    {
      return (lhs.position == rhs.position) && (lhs.coords == rhs.coords);
    }

    template<>
    bool operator==(const Vertex<XYZ.RGBA> &lhs, const Vertex<XYZ.RGBA> &rhs)
    {
      return (lhs.position == rhs.position) && (lhs.color == rhs.color);
    }

    template<>
    bool operator==(const Vertex<XYZ.UV.RGBA> &lhs, const Vertex<XYZ.UV.RGBA> &rhs)
    {
      return (lhs.position == rhs.position) && (lhs.coords == rhs.coords) && (lhs.color == rhs.color);
    }

    template<>
    bool operator==(const Vertex<XYZ.N> &lhs, const Vertex<XYZ.N> &rhs)
    {
      return (lhs.position == rhs.position) && (lhs.normal == rhs.normal);
    }

    template<>
    bool operator==(const Vertex<XYZ.N.UV> &lhs, const Vertex<XYZ.N.UV> &rhs)
    {
      return (lhs.position == rhs.position) && (lhs.normal == rhs.normal) && (lhs.coords == rhs.coords);
    }

    template<>
    bool operator==(const Vertex<XYZ.N.UV.RGBA> &lhs, const Vertex<XYZ.N.UV.RGBA> &rhs)
    {
      return (lhs.position == rhs.position) && (lhs.normal == rhs.normal) && (lhs.coords == rhs.coords) && (lhs.color == rhs.color);
    }

    template<>
    bool operator==(const Vertex<N> &lhs, const Vertex<N> &rhs)
    {
      return lhs.normal == rhs.normal;
    }

    template<>
    bool operator==(const Vertex<UV> &lhs, const Vertex<UV> &rhs)
    {
      return lhs.coords == rhs.coords;
    }

    template<>
    bool operator==(const Vertex<RGBA> &lhs, const Vertex<RGBA> &rhs)
    {
      return lhs.color == rhs.color;
    }
  }
}
