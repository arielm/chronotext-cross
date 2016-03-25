#pragma once

#include "gl/Utils.h"

#include <array>
#include <vector>

#include <glm/gtc/type_ptr.hpp>

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
      Matrix(const glm::mat3x3 &matrix);
      Matrix(const Matrix &other) = delete;
      
      operator float* () { return glm::value_ptr(m); }
      operator const float* () const { return glm::value_ptr(m); }

      glm::mat4 operator * (const glm::mat4 &matrix) { return matrix * m; }
      Matrix& operator *= (const glm::mat4 &matrix) { m *= matrix; return *this; }

      Matrix& load(const Matrix &matrix);
      Matrix& load(const glm::mat4 &matrix);
      Matrix& load(const glm::mat3x3 &matrix);

      void push();
      void pop();
      
      void setToIdentity();

      inline void setTranslation(const glm::vec2 &t) { setTranslation(t.x, t.y, 0); }
      inline void setTranslation(const glm::vec3 &t) { setTranslation(t.x, t.y, t.z); }
      void setTranslation(float x, float y, float z = 0);

      inline void translate(const glm::vec2 &t) { translate(t.x, t.y, 0); }
      inline void translate(const glm::vec3 &t) { translate(t.x, t.y, t.z); }
      void translate(float x, float y, float z = 0);

      inline void scale(const glm::vec2 &s) { scale(s.x, s.y); }
      inline void scale(const glm::vec3 &s) { scale(s.x, s.y, s.z); }
      inline void scale(float s) { scale(s, s, s); }
      void scale(float x, float y, float z = 1);
      
      void rotateX(float a);
      void rotateY(float a);
      void rotateZ(float a);
      void rotateXY(float sx, float sy);

      glm::quat getQuat() const;

      template<int T = 1>
      void applyQuat(const glm::quat &q);

      inline glm::vec3 transformPoint(const glm::vec2 &point) const { return transformPoint(point.x, point.y); }
      glm::vec3 transformPoint(float x, float y) const;

      inline glm::vec3 transformPoint(const glm::vec3 &point) const { return transformPoint(point.x, point.y, point.z); }
      glm::vec3 transformPoint(float x, float y, float z) const;

      template<int Primitive = GL_TRIANGLES, int Orientation = GL_CCW, int Type>
      void addTransformedQuad(const Quad<Type> &quad, std::vector<Vertex<Type>> &output) const;

    protected:
      std::vector<Values> stack;
    };
  }
}
