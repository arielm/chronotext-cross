#pragma once

#include "chr/gl.h"
#include "chr/gl/Quad.h"
#include "chr/math/Utils.h"

#include <array>
#include <vector>

namespace chr
{
  namespace gl
  {
    template<int V, typename I>
    class IndexedVertexBatch;

    class Matrix
    {
    public:
      typedef std::vector<std::array<float, 16>> Stack;

      union
      {
        glm::mat4 m;

        struct
        {
          float m00, m10, m20, m30;
          float m01, m11, m21, m31;
          float m02, m12, m22, m32;
          float m03, m13, m23, m33;
        };

        std::array<float, 16> values;
      };

      Matrix();
      Matrix(const glm::mat4 &matrix);
      Matrix(const glm::mat3 &matrix);
      Matrix(const glm::quat &quat);
      Matrix(const glm::vec3 &right, const glm::vec3 &up, const glm::vec3 &back);

      operator std::array<float, 16>& () { return values; }
      operator const std::array<float, 16>& () const { return values; }

      glm::mat4 operator * (const glm::mat4 &matrix) { return matrix * m; }
      Matrix& operator *= (const glm::mat4 &matrix) { m *= matrix; return *this; }

      Matrix& set(const Matrix &matrix);
      Matrix& set(const glm::mat4 &matrix);
      Matrix& set(const glm::mat3 &matrix);
      Matrix& set(const glm::quat &quat);
      Matrix& set(const glm::vec3 &right, const glm::vec3 &up, const glm::vec3 &back);

      Matrix& push(Stack &stack);
      Matrix& pop(Stack &stack);

      Matrix& setIdentity();
      Matrix& inverse();

      inline Matrix& setTranslate(const glm::vec2 &t) { return setTranslate(t.x, t.y); }
      inline Matrix& setTranslate(const glm::vec3 &t) { return setTranslate(t.x, t.y, t.z); }
      Matrix& setTranslate(float x, float y, float z = 0);

      inline Matrix& translate(const glm::vec2 &t) { return translate(t.x, t.y); }
      inline Matrix& translate(const glm::vec3 &t) { return translate(t.x, t.y, t.z); }
      Matrix& translate(float x, float y, float z = 0);

      inline Matrix& scale(const glm::vec2 &s) { return scale(s.x, s.y); }
      inline Matrix& scale(const glm::vec3 &s) { return scale(s.x, s.y, s.z); }
      inline Matrix& scale(float s) { return scale(s, s, s); }
      Matrix& scale(float x, float y, float z = 1);

      Matrix& rotateX(float a);
      Matrix& rotateY(float a);
      Matrix& rotateZ(float a);

      template<int T = 1>
      Matrix& applyQuat(const glm::quat &q);

      glm::vec3 right() const;
      glm::vec3 up() const;
      glm::vec3 back() const;

      void clearTranslation();
      void clearRotation();

      const glm::mat4& getMat4() const;
      glm::quat getQuat() const;
      glm::mat4 getInverse() const;

      float getDeterminant3x3() const;
      glm::mat3 getNormalMatrix() const;

      inline glm::vec3 transformPoint(const glm::vec2 &point) const { return transformPoint(point.x, point.y); }
      glm::vec3 transformPoint(float x, float y) const;

      inline glm::vec3 transformPoint(const glm::vec3 &point) const { return transformPoint(point.x, point.y, point.z); }
      glm::vec3 transformPoint(float x, float y, float z) const;

      inline glm::vec3 transformNormal(const glm::vec3 &normal) const { return transformNormal(normal.x, normal.y, normal.z); }
      glm::vec3 transformNormal(float x, float y, float z) const;

      template<int V>
      inline glm::vec3 transformPoint(Vertex<V> vertex) const { return transformPoint(vertex.x, vertex.y, vertex.z); }

      template<int Primitive = GL_TRIANGLES, int FrontFace = CCW, int V = XYZ>
      void addTransformedQuad(const Quad<V> &quad, std::vector<Vertex<V>> &output) const;

      template<int Primitive = GL_TRIANGLES, int FrontFace = CCW, int V = XYZ, typename I = GLushort>
      void addTransformedQuad(const Quad<V> &quad, IndexedVertexBatch<V, I> &output) const;
    };
  }
}
