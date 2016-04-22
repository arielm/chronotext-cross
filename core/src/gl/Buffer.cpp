#include "gl/Buffer.h"

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
      int TypeTraits<Vertex<XYZ.UV.N>>::usageCounter = 0;
      int TypeTraits<Vertex<XYZ.RGBA.N>>::usageCounter = 0;
      int TypeTraits<Vertex<XYZ.UV.RGBA.N>>::usageCounter = 0;
      int TypeTraits<glm::vec2>::usageCounter = 0;
      int TypeTraits<glm::vec3>::usageCounter = 0;
      int TypeTraits<glm::vec4>::usageCounter = 0;
      int TypeTraits<GLushort>::usageCounter = 0;
      int TypeTraits<GLuint>::usageCounter = 0;
    }
  }
}
