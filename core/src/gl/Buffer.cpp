#include "gl/Buffer.h"

using namespace std;

namespace chr
{
  namespace gl
  {
    int TypeTraits<Vertex<>>::counter = 0;
    int TypeTraits<Vertex<UV>>::counter = 0;
    int TypeTraits<Vertex<N>>::counter = 0;
    int TypeTraits<Vertex<RGBA>>::counter = 0;
    int TypeTraits<Vertex<N|UV>>::counter = 0;
    int TypeTraits<Vertex<N|UV|RGBA>>::counter = 0;
    int TypeTraits<Vertex<UV|RGBA>>::counter = 0;
    int TypeTraits<Vertex<N|RGBA>>::counter = 0;
    int TypeTraits<glm::vec2>::counter = 0;
    int TypeTraits<glm::vec3>::counter = 0;
    int TypeTraits<glm::vec4>::counter = 0;
    int TypeTraits<GLushort>::counter = 0;
    int TypeTraits<GLuint>::counter = 0;

    unordered_map<int, Element<Vertex<>>> TypeTraits<Vertex<>>::map;
    unordered_map<int, Element<Vertex<UV>>> TypeTraits<Vertex<UV>>::map;
    unordered_map<int, Element<Vertex<N>>> TypeTraits<Vertex<N>>::map;
    unordered_map<int, Element<Vertex<RGBA>>> TypeTraits<Vertex<RGBA>>::map;
    unordered_map<int, Element<Vertex<N|UV>>> TypeTraits<Vertex<N|UV>>::map;
    unordered_map<int, Element<Vertex<N|UV|RGBA>>> TypeTraits<Vertex<N|UV|RGBA>>::map;
    unordered_map<int, Element<Vertex<UV|RGBA>>> TypeTraits<Vertex<UV|RGBA>>::map;
    unordered_map<int, Element<Vertex<N|RGBA>>> TypeTraits<Vertex<N|RGBA>>::map;
    unordered_map<int, Element<glm::vec2>> TypeTraits<glm::vec2>::map;
    unordered_map<int, Element<glm::vec3>> TypeTraits<glm::vec3>::map;
    unordered_map<int, Element<glm::vec4>> TypeTraits<glm::vec4>::map;
    unordered_map<int, Element<GLushort>> TypeTraits<GLushort>::map;
    unordered_map<int, Element<GLuint>> TypeTraits<GLuint>::map;
  }
}
