#pragma once

#include "gl/Quad.h"
#include "gl/Batch.h"

#include <array>
#include <vector>

namespace chr
{
  namespace gl
  {
    class Matrix
    {
    public:
      typedef std::array<float, 16> Values;
      
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
        
        Values values;
      };
      
      Matrix();
      Matrix(const glm::mat4 &matrix);
      Matrix(const glm::mat3 &matrix);
      Matrix(const Matrix &other) = delete;
      
      operator float* () { return glm::value_ptr(m); }
      operator const float* () const { return glm::value_ptr(m); }

      glm::mat4 operator * (const glm::mat4 &matrix) { return matrix * m; }
      Matrix& operator *= (const glm::mat4 &matrix) { m *= matrix; return *this; }

      Matrix& load(const Matrix &matrix);
      Matrix& load(const glm::mat4 &matrix);
      Matrix& load(const glm::mat3 &matrix);

      Matrix& push();
      Matrix& pop();

      Matrix& setToIdentity();

      inline Matrix& setTranslation(const glm::vec2 &t) { return setTranslation(t.x, t.y, 0); }
      inline Matrix& setTranslation(const glm::vec3 &t) { return setTranslation(t.x, t.y, t.z); }
      Matrix& setTranslation(float x, float y, float z = 0);

      inline Matrix& translate(const glm::vec2 &t) { return translate(t.x, t.y, 0); }
      inline Matrix& translate(const glm::vec3 &t) { return translate(t.x, t.y, t.z); }
      Matrix& translate(float x, float y, float z = 0);

      inline Matrix& scale(const glm::vec2 &s) { return scale(s.x, s.y); }
      inline Matrix& scale(const glm::vec3 &s) { return scale(s.x, s.y, s.z); }
      inline Matrix& scale(float s) { return scale(s, s, s); }
      Matrix& scale(float x, float y, float z = 1);

      Matrix& rotateX(float a);
      Matrix& rotateY(float a);
      Matrix& rotateZ(float a);
      Matrix& rotateXY(float sx, float sy);

      template<int T = 1>
      Matrix& applyQuat(const glm::quat &q);

      glm::quat getQuat() const;

      inline glm::vec3 transformPoint(const glm::vec2 &point) const { return transformPoint(point.x, point.y); }
      glm::vec3 transformPoint(float x, float y) const;

      inline glm::vec3 transformPoint(const glm::vec3 &point) const { return transformPoint(point.x, point.y, point.z); }
      glm::vec3 transformPoint(float x, float y, float z) const;

      inline glm::vec3 transformNormal(const glm::vec3 &normal) const { return transformNormal(normal.x, normal.y, normal.z); }
      glm::vec3 transformNormal(float x, float y, float z) const;

      template<int Primitive = GL_TRIANGLES, int Orientation = GL_CCW, int V = XYZ>
      void addTransformedQuad(const Quad<V> &quad, std::vector<Vertex<V>> &output) const;

      template<int Primitive = GL_TRIANGLES, int Orientation = GL_CCW, int V = XYZ, typename I = GLushort>
      void addTransformedQuad(const Quad<V> &quad, IndexedVertexBatch<V, I> &output) const;

    protected:
      std::vector<Values> stack;
    };
  }
}
