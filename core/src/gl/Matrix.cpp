#include "gl/Matrix.h"

using namespace std;

namespace chr
{
  namespace gl
  {
    Matrix::Matrix()
    :
    m00(1),
    m10(0),
    m20(0),
    m30(0),
    m01(0),
    m11(1),
    m21(0),
    m31(0),
    m02(0),
    m12(0),
    m22(1),
    m32(0),
    m03(0),
    m13(0),
    m23(0),
    m33(1)
    {}

    Matrix::Matrix(const glm::mat4 &matrix)
    :
    m(matrix)
    {}

    Matrix::Matrix(const glm::mat3x3 &matrix)
    :
    m00(matrix[0][0]),
    m10(matrix[0][1]),
    m20(matrix[0][2]),
    m30(0),
    m01(matrix[1][0]),
    m11(matrix[1][1]),
    m21(matrix[1][2]),
    m31(0),
    m02(0),
    m12(0),
    m22(1),
    m32(0),
    m03(matrix[2][0]),
    m13(matrix[2][1]),
    m23(0),
    m33(matrix[2][2])
    {}

    Matrix& Matrix::load(const Matrix &matrix)
    {
      values = matrix.values;
      return *this;
    }
    
    Matrix& Matrix::load(const glm::mat4 &matrix)
    {
      m = matrix;
      return *this;
    }
    
    Matrix& Matrix::load(const glm::mat3x3 &matrix)
    {
      m[0][0] = matrix[0][0];
      m[0][1] = matrix[0][1];
      m[0][2] = matrix[0][2];
      m[0][3] = 0;

      m[1][0] = matrix[1][0];
      m[1][1] = matrix[1][1];
      m[1][2] = matrix[1][2];
      m[1][3] = 0;

      m[2][0] = 0;
      m[2][1] = 0;
      m[2][2] = 1;
      m[2][3] = 0;

      m[3][0] = matrix[2][0];
      m[3][1] = matrix[2][1];
      m[3][2] = 0;
      m[3][3] = matrix[2][2];
      
      return *this;
    }

    Matrix& Matrix::push()
    {
      stack.push_back(values);
      return *this;
    }

    Matrix& Matrix::pop()
    {
      if (!stack.empty())
      {
        values = stack.back();
        stack.pop_back();
      }

      return *this;
    }

    Matrix& Matrix::setToIdentity()
    {
      m00 = m11 = m22 = m33 = 1.0f;
      m01 = m02 = m03 = m10 = m12 = m13 = m20 = m21 = m23 = m30 = m31 = m32 = 0.0f;

      return *this;
    }

    Matrix& Matrix::setTranslation(float x, float y, float z)
    {
      m00 = m11 = m22 = m33 = 1.0f;
      m01 = m02 = m10 = m12 = m20 = m21 = m30 = m31 = m32 = 0.0f;
      
      m03 = x;
      m13 = y;
      m23 = z;

      return *this;
    }

    Matrix& Matrix::translate(float x, float y, float z)
    {
      m03 += x * m00 + y * m01 + z * m02;
      m13 += x * m10 + y * m11 + z * m12;
      m23 += x * m20 + y * m21 + z * m22;
      m33 += x * m30 + y * m31 + z * m32;

      return *this;
    }

    Matrix& Matrix::scale(float x, float y, float z)
    {
      m00 *= x;
      m10 *= x;
      m20 *= x;
      m30 *= x;
      
      m01 *= y;
      m11 *= y;
      m21 *= y;
      m31 *= y;
      
      m02 *= z;
      m12 *= z;
      m22 *= z;
      m32 *= z;

      return *this;
    }

    Matrix& Matrix::rotateX(float a)
    {
      float c = cosf(a);
      float s = sinf(a);
      
      float r01 = m01 * c + m02 * s;
      float r02 = m02 * c - m01 * s;
      
      float r11 = m11 * c + m12 * s;
      float r12 = m12 * c - m11 * s;
      
      float r21 = m21 * c + m22 * s;
      float r22 = m22 * c - m21 * s;
      
      float r31 = m31 * c + m32 * s;
      float r32 = m32 * c - m31 * s;
      
      m01 = r01;
      m02 = r02;
      m11 = r11;
      m12 = r12;
      m21 = r21;
      m22 = r22;
      m31 = r31;
      m32 = r32;

      return *this;
    }

    Matrix& Matrix::rotateY(float a)
    {
      float c = cosf(a);
      float s = sinf(a);
      
      float r00 = m00 * c - m02 * s;
      float r02 = m00 * s + m02 * c;
      
      float r10 = m10 * c - m12 * s;
      float r12 = m10 * s + m12 * c;
      
      float r20 = m20 * c - m22 * s;
      float r22 = m20 * s + m22 * c;
      
      float r30 = m30 * c - m32 * s;
      float r32 = m30 * s + m32 * c;
      
      m00 = r00;
      m02 = r02;
      m10 = r10;
      m12 = r12;
      m20 = r20;
      m22 = r22;
      m30 = r30;
      m32 = r32;

      return *this;
    }

    Matrix& Matrix::rotateZ(float a)
    {
      float c = cosf(a);
      float s = sinf(a);
      
      float r00 = m00 * c + m01 * s;
      float r01 = m01 * c - m00 * s;
      
      float r10 = m10 * c + m11 * s;
      float r11 = m11 * c - m10 * s;
      
      float r20 = m20 * c + m21 * s;
      float r21 = m21 * c - m20 * s;
      
      float r30 = m30 * c + m31 * s;
      float r31 = m31 * c - m30 * s;
      
      m00 = r00;
      m01 = r01;
      m10 = r10;
      m11 = r11;
      m20 = r20;
      m21 = r21;
      m30 = r30;
      m31 = r31;

      return *this;
    }

    Matrix& Matrix::rotateXY(float sx, float sy)
    {
      float cx = sqrtf(1.0f - sx * sx);
      float cy = sqrtf(1.0f - sy * sy);
      
      float cxy = cx + cy;
      
      float r00 = m00 * cy - m02 * sy;
      float r01 = m01 * cx + m02 * sx;
      float r02 = m00 * sy + m02 * cxy - m01 * sx;
      
      float r10 = m10 * cy - m12 * sy;
      float r11 = m11 * cx + m12 * sx;
      float r12 = m10 * sy + m12 * cxy - m11 * sx;
      
      float r20 = m20 * cy - m22 * sy;
      float r21 = m21 * cx + m22 * sx;
      float r22 = m20 * sy + m22 * cxy - m21 * sx;
      
      float r30 = m30 * cy - m32 * sy;
      float r31 = m31 * cx + m32 * sx;
      float r32 = m30 * sy + m32 * cxy - m31 * sx;
      
      m00 = r00;
      m01 = r01;
      m02 = r02;
      m10 = r10;
      m11 = r11;
      m12 = r12;
      m20 = r20;
      m21 = r21;
      m22 = r22;
      m30 = r30;
      m31 = r31;
      m32 = r32;

      return *this;
    }

    template <>
    Matrix& Matrix::applyQuat<+1>(const glm::quat &q)
    {
      m *= glm::mat4_cast(q);
      return *this;
    }

    template <>
    Matrix& Matrix::applyQuat<-1>(const glm::quat &q)
    {
      m *= glm::inverse(glm::mat4_cast(q));
      return *this;
    }

    glm::quat Matrix::getQuat() const
    {
      return glm::quat_cast(m);
    }

    glm::vec3 Matrix::transformPoint(float x, float y) const
    {
      return glm::vec3(
        x * m00 + y * m01 + m03,
        x * m10 + y * m11 + m13,
        x * m20 + y * m21 + m23);
    }

    glm::vec3 Matrix::transformPoint(float x, float y, float z) const
    {
      return glm::vec3(
        x * m00 + y * m01 + z * m02 + m03,
        x * m10 + y * m11 + z * m12 + m13,
        x * m20 + y * m21 + z * m22 + m23);
    }

    #define TRANSFORM_QUAD_HEADER \
      float x100 = quad.x1 * m00 + m03; \
      float x110 = quad.x1 * m10 + m13; \
      float x120 = quad.x1 * m20 + m23; \
      \
      float y101 = quad.y1 * m01; \
      float y111 = quad.y1 * m11; \
      float y121 = quad.y1 * m21; \
      \
      float x200 = quad.x2 * m00 + m03; \
      float x210 = quad.x2 * m10 + m13; \
      float x220 = quad.x2 * m20 + m23; \
      \
      float y201 = quad.y2 * m01; \
      float y211 = quad.y2 * m11; \
      float y221 = quad.y2 * m21;

    #define TRANSFORM_QUAD_X1_Y1 x100 + y101, x110 + y111, x120 + y121
    #define TRANSFORM_QUAD_X1_Y2 x100 + y201, x110 + y211, x120 + y221
    #define TRANSFORM_QUAD_X2_Y2 x200 + y201, x210 + y211, x220 + y221
    #define TRANSFORM_QUAD_X2_Y1 x200 + y101, x210 + y111, x220 + y121

    template <>
    void Matrix::addTransformedQuad<GL_TRIANGLES>(const Quad<> &quad, std::vector<Vertex<>> &output) const
    {
      TRANSFORM_QUAD_HEADER

      output.emplace_back(TRANSFORM_QUAD_X1_Y1); // x1, y1
      output.emplace_back(TRANSFORM_QUAD_X1_Y2); // x1, y2
      output.emplace_back(TRANSFORM_QUAD_X2_Y2); // x2, y2
      output.emplace_back(TRANSFORM_QUAD_X2_Y1); // x2, y1
    };

    template <>
    void Matrix::addTransformedQuad<GL_TRIANGLES>(const Quad<UV> &quad, std::vector<Vertex<UV>> &output) const
    {
      TRANSFORM_QUAD_HEADER

      output.emplace_back(TRANSFORM_QUAD_X1_Y1, quad.u1, quad.v1); // x1, y1
      output.emplace_back(TRANSFORM_QUAD_X1_Y2, quad.u1, quad.v2); // x1, y2
      output.emplace_back(TRANSFORM_QUAD_X2_Y2, quad.u2, quad.v2); // x2, y2
      output.emplace_back(TRANSFORM_QUAD_X2_Y1, quad.u2, quad.v1); // x2, y1
    }

    template <>
    void Matrix::addTransformedQuad<GL_TRIANGLES, GL_CCW>(const Quad<> &quad, IndexedVertexBatch<0, GLushort> &output) const
    {
      TRANSFORM_QUAD_HEADER

      output.addVertex(TRANSFORM_QUAD_X1_Y1); // x1, y1
      output.addVertex(TRANSFORM_QUAD_X1_Y2); // x1, y2
      output.addVertex(TRANSFORM_QUAD_X2_Y2); // x2, y2
      output.addVertex(TRANSFORM_QUAD_X2_Y1); // x2, y1

      output.addIndices(0, 1, 2, 2, 3, 0);
      output.incrementIndices(4);
    }

    template <>
    void Matrix::addTransformedQuad<GL_TRIANGLES, GL_CW>(const Quad<> &quad, IndexedVertexBatch<0, GLushort> &output) const
    {
      TRANSFORM_QUAD_HEADER

      output.addVertex(TRANSFORM_QUAD_X1_Y1); // x1, y1
      output.addVertex(TRANSFORM_QUAD_X1_Y2); // x1, y2
      output.addVertex(TRANSFORM_QUAD_X2_Y2); // x2, y2
      output.addVertex(TRANSFORM_QUAD_X2_Y1); // x2, y1

      output.addIndices(0, 3, 2, 2, 1, 0);
      output.incrementIndices(4);
    }

    template <>
    void Matrix::addTransformedQuad<GL_TRIANGLES, GL_CCW>(const Quad<UV> &quad, IndexedVertexBatch<UV, GLushort> &output) const
    {
      TRANSFORM_QUAD_HEADER

      output.addVertex(TRANSFORM_QUAD_X1_Y1, quad.u1, quad.v1); // x1, y1
      output.addVertex(TRANSFORM_QUAD_X1_Y2, quad.u1, quad.v2); // x1, y2
      output.addVertex(TRANSFORM_QUAD_X2_Y2, quad.u2, quad.v2); // x2, y2
      output.addVertex(TRANSFORM_QUAD_X2_Y1, quad.u2, quad.v1); // x2, y1

      output.addIndices(0, 1, 2, 2, 3, 0);
      output.incrementIndices(4);
    }

    template <>
    void Matrix::addTransformedQuad<GL_TRIANGLES, GL_CW>(const Quad<UV> &quad, IndexedVertexBatch<UV, GLushort> &output) const
    {
      TRANSFORM_QUAD_HEADER

      output.addVertex(TRANSFORM_QUAD_X1_Y1, quad.u1, quad.v1); // x1, y1
      output.addVertex(TRANSFORM_QUAD_X1_Y2, quad.u1, quad.v2); // x1, y2
      output.addVertex(TRANSFORM_QUAD_X2_Y2, quad.u2, quad.v2); // x2, y2
      output.addVertex(TRANSFORM_QUAD_X2_Y1, quad.u2, quad.v1); // x2, y1

      output.addIndices(0, 3, 2, 2, 1, 0);
      output.incrementIndices(4);
    }

    template <>
    void Matrix::addTransformedQuad<GL_TRIANGLES, GL_CCW>(const Quad<RGBA> &quad, IndexedVertexBatch<RGBA, GLushort> &output) const
    {
      TRANSFORM_QUAD_HEADER

      output.addVertex(TRANSFORM_QUAD_X1_Y1, quad.color); // x1, y1
      output.addVertex(TRANSFORM_QUAD_X1_Y2, quad.color); // x1, y2
      output.addVertex(TRANSFORM_QUAD_X2_Y2, quad.color); // x2, y2
      output.addVertex(TRANSFORM_QUAD_X2_Y1, quad.color); // x2, y1

      output.addIndices(0, 1, 2, 2, 3, 0);
      output.incrementIndices(4);
    }

    template <>
    void Matrix::addTransformedQuad<GL_TRIANGLES, GL_CW>(const Quad<RGBA> &quad, IndexedVertexBatch<RGBA, GLushort> &output) const
    {
      TRANSFORM_QUAD_HEADER

      output.addVertex(TRANSFORM_QUAD_X1_Y1, quad.color); // x1, y1
      output.addVertex(TRANSFORM_QUAD_X1_Y2, quad.color); // x1, y2
      output.addVertex(TRANSFORM_QUAD_X2_Y2, quad.color); // x2, y2
      output.addVertex(TRANSFORM_QUAD_X2_Y1, quad.color); // x2, y1

      output.addIndices(0, 3, 2, 2, 1, 0);
      output.incrementIndices(4);
    }

    template <>
    void Matrix::addTransformedQuad<GL_TRIANGLES, GL_CCW>(const Quad<UV|RGBA> &quad, IndexedVertexBatch<UV|RGBA, GLushort> &output) const
    {
      TRANSFORM_QUAD_HEADER

      output.addVertex(TRANSFORM_QUAD_X1_Y1, quad.u1, quad.v1, quad.color); // x1, y1
      output.addVertex(TRANSFORM_QUAD_X1_Y2, quad.u1, quad.v2, quad.color); // x1, y2
      output.addVertex(TRANSFORM_QUAD_X2_Y2, quad.u2, quad.v2, quad.color); // x2, y2
      output.addVertex(TRANSFORM_QUAD_X2_Y1, quad.u2, quad.v1, quad.color); // x2, y1

      output.addIndices(0, 1, 2, 2, 3, 0);
      output.incrementIndices(4);
    }

    template <>
    void Matrix::addTransformedQuad<GL_TRIANGLES, GL_CW>(const Quad<UV|RGBA> &quad, IndexedVertexBatch<UV|RGBA, GLushort> &output) const
    {
      TRANSFORM_QUAD_HEADER

      output.addVertex(TRANSFORM_QUAD_X1_Y1, quad.u1, quad.v1, quad.color); // x1, y1
      output.addVertex(TRANSFORM_QUAD_X1_Y2, quad.u1, quad.v2, quad.color); // x1, y2
      output.addVertex(TRANSFORM_QUAD_X2_Y2, quad.u2, quad.v2, quad.color); // x2, y2
      output.addVertex(TRANSFORM_QUAD_X2_Y1, quad.u2, quad.v1, quad.color); // x2, y1

      output.addIndices(0, 3, 2, 2, 1, 0);
      output.incrementIndices(4);
    }
  }
}
