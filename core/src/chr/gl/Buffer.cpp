#include "chr/gl/Buffer.h"

using namespace std;

namespace chr
{
  namespace gl
  {
    namespace buffer
    {
      int TypeTraits<Vertex<XYZ>>::usageCounter = 0;
      int TypeTraits<Vertex<XYZ.UV>>::usageCounter = 0;
      int TypeTraits<Vertex<XYZ.RGBA>>::usageCounter = 0;
      int TypeTraits<Vertex<XYZ.N>>::usageCounter = 0;
      int TypeTraits<Vertex<XYZ.UV.RGBA>>::usageCounter = 0;
      int TypeTraits<Vertex<XYZ.N.UV>>::usageCounter = 0;
      int TypeTraits<Vertex<XYZ.N.RGBA>>::usageCounter = 0;
      int TypeTraits<Vertex<XYZ.N.UV.RGBA>>::usageCounter = 0;
      int TypeTraits<Vertex<N>>::usageCounter = 0;
      int TypeTraits<Vertex<UV>>::usageCounter = 0;
      int TypeTraits<Vertex<RGBA>>::usageCounter = 0;
      int TypeTraits<GLushort>::usageCounter = 0;
      int TypeTraits<GLuint>::usageCounter = 0;
    }
  }
}
