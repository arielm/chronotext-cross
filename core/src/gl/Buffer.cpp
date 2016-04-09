#include "gl/Buffer.h"

using namespace std;

namespace chr
{
  namespace gl
  {
    namespace buffer
    {
      int TypeTraits<Vertex<>>::usageCounter = 0;
      int TypeTraits<Vertex<UV>>::usageCounter = 0;
      int TypeTraits<Vertex<N>>::usageCounter = 0;
      int TypeTraits<Vertex<RGBA>>::usageCounter = 0;
      int TypeTraits<Vertex<N | UV>>::usageCounter = 0;
      int TypeTraits<Vertex<N | UV | RGBA>>::usageCounter = 0;
      int TypeTraits<Vertex<UV | RGBA>>::usageCounter = 0;
      int TypeTraits<Vertex<N | RGBA>>::usageCounter = 0;
      int TypeTraits<glm::vec2>::usageCounter = 0;
      int TypeTraits<glm::vec3>::usageCounter = 0;
      int TypeTraits<glm::vec4>::usageCounter = 0;
      int TypeTraits<GLushort>::usageCounter = 0;
      int TypeTraits<GLuint>::usageCounter = 0;
    }
  }
}
