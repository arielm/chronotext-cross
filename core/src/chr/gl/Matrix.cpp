#include "chr/gl/Matrix.h"
#include "chr/gl/Batch.h"
#include "chr/gl/Euler.h"

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

    Matrix::Matrix(const array<float, 16> &values)
    :
    values(values)
    {}

    Matrix::Matrix(const glm::mat4 &matrix)
    :
    m(matrix)
    {}

    Matrix::Matrix(const glm::mat3 &matrix)
    :
    m(matrix)
    {}

    Matrix::Matrix(const glm::quat &quat)
    :
    m(glm::mat4_cast(quat))
    {}

    Matrix::Matrix(const glm::vec3 &right, const glm::vec3 &up, const glm::vec3 &back)
    :
    m00(right.x),
    m10(up.x),
    m20(back.x),
    m30(0),
    m01(right.y),
    m11(up.y),
    m21(back.y),
    m31(0),
    m02(right.z),
    m12(up.z),
    m22(back.z),
    m32(0),
    m03(0),
    m13(0),
    m23(0),
    m33(1)
    {}

    Matrix& Matrix::set(const Matrix &matrix)
    {
      values = matrix.values;
      return *this;
    }
    
    Matrix& Matrix::set(const glm::mat4 &matrix)
    {
      m = matrix;
      return *this;
    }
    
    Matrix& Matrix::set(const glm::mat3 &matrix)
    {
      m = glm::mat4(matrix);
      return *this;
    }

    Matrix& Matrix::set(const glm::quat &quat)
    {
      m = glm::mat4_cast(quat);
      return *this;
    }

    Matrix& Matrix::set(const glm::vec3 &right, const glm::vec3 &up, const glm::vec3 &back)
    {
      m00 = right.x, m10 = up.x, m20 = back.x;
      m01 = right.y, m11 = up.y, m21 = back.y;
      m02 = right.z, m12 = up.z, m22 = back.z;

      m30 = m31 = m32 = m03 = m13 = m23 = 0.0f;
      m33 = 1.0f;

      return *this;
    }

    Matrix Matrix::compose(const glm::vec3 &pos, const glm::quat &quat, const glm::vec3 &scale)
    {
      float x = quat.x, y = quat.y, z = quat.z, w = quat.w;
      float x2 = x + x, y2 = y + y, z2 = z + z;
      float xx = x * x2, xy = x * y2, xz = x * z2;
      float yy = y * y2, yz = y * z2, zz = z * z2;
      float wx = w * x2, wy = w * y2, wz = w * z2;
      float sx = scale.x, sy = scale.y, sz = scale.z;

      Matrix result;

      result.values[ 0 ] = ( 1 - ( yy + zz ) ) * sx;
      result.values[ 1 ] = ( xy + wz ) * sx;
      result.values[ 2 ] = ( xz - wy ) * sx;
      result.values[ 3 ] = 0;

      result.values[ 4 ] = ( xy - wz ) * sy;
      result.values[ 5 ] = ( 1 - ( xx + zz ) ) * sy;
      result.values[ 6 ] = ( yz + wx ) * sy;
      result.values[ 7 ] = 0;

      result.values[ 8 ] = ( xz + wy ) * sz;
      result.values[ 9 ] = ( yz - wx ) * sz;
      result.values[ 10 ] = ( 1 - ( xx + yy ) ) * sz;
      result.values[ 11 ] = 0;

      result.values[ 12 ] = pos.x;
      result.values[ 13 ] = pos.y;
      result.values[ 14 ] = pos.z;
      result.values[ 15 ] = 1;

      return result;
    }

    Matrix Matrix::extractRotation(const Matrix &matrix)
    {
        glm::vec3 scale;
        scale.x = glm::length(glm::vec3(matrix.m[0]));
        scale.y = glm::length(glm::vec3(matrix.m[1]));
        scale.z = glm::length(glm::vec3(matrix.m[2]));

        Matrix result;

        result.values[0] = matrix.values[0] / scale.x;
        result.values[1] = matrix.values[1] / scale.x;
        result.values[2] = matrix.values[2] / scale.x;
        result.values[3] = 0;

        result.values[4] = matrix.values[4] / scale.y;
        result.values[5] = matrix.values[5] / scale.y;
        result.values[6] = matrix.values[6] / scale.y;
        result.values[7] = 0;

        result.values[8] = matrix.values[8] / scale.z;
        result.values[9] = matrix.values[9] / scale.z;
        result.values[10] = matrix.values[10] / scale.z;
        result.values[11] = 0;

        return result;
    }

    Matrix& Matrix::push(Stack &stack)
    {
      stack.push_back(values);
      return *this;
    }

    Matrix& Matrix::pop(Stack &stack)
    {
      if (!stack.empty())
      {
        values = stack.back();
        stack.pop_back();
      }

      return *this;
    }

    Matrix& Matrix::setIdentity()
    {
      m00 = m11 = m22 = m33 = 1.0f;
      m01 = m02 = m03 = m10 = m12 = m13 = m20 = m21 = m23 = m30 = m31 = m32 = 0.0f;

      return *this;
    }

    Matrix& Matrix::inverse()
    {
      m = glm::inverse(m);
      return *this;
    }

    Matrix& Matrix::setTranslate(float x, float y, float z)
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

    /*
     * Based on http://www.songho.ca/opengl/gl_anglestoaxes.html
     */
    Matrix& Matrix::rotateXYZ(float ax, float ay, float az)
    {
      float sx = sinf(ax);
      float cx = cosf(ax);

      float sy = sinf(ay);
      float cy = cosf(ay);

      float sz = sinf(az);
      float cz = cosf(az);

      glm::vec3 left;
      left.x =  cy*cz;
      left.y =  sx*sy*cz + cx*sz;
      left.z = -cx*sy*cz + sx*sz;

      glm::vec3 up;
      up.x = -cy*sz;
      up.y = -sx*sy*sz + cx*cz;
      up.z =  cx*sy*sz + sx*cz;

      glm::vec3 forward;
      forward.x =  sy;
      forward.y = -sx*cy;
      forward.z =  cx*cy;

      glm::mat4 rotation;
      rotation[0][0] = left.x;
      rotation[1][0] = up.x;
      rotation[2][0] = forward.x;
      rotation[3][0] = 0;
      rotation[0][1] = left.y;
      rotation[1][1] = up.y;
      rotation[2][1] = forward.y;
      rotation[3][1] = 0;
      rotation[0][2] = left.z;
      rotation[1][2] = up.z;
      rotation[2][2] = forward.z;
      rotation[3][2] = 0;
      rotation[0][3] = 0;
      rotation[1][3] = 0;
      rotation[2][3] = 0;
      rotation[3][3] = 1;

      m *= rotation;

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

    glm::vec3 Matrix::right() const
    {
      return glm::vec3(m00, m01, m02);
    }

    glm::vec3 Matrix::up() const
    {
      return glm::vec3(m10, m11, m12);
    }

    glm::vec3 Matrix::back() const
    {
      return glm::vec3(m20, m21, m22);
    }

    void Matrix::clearTranslation()
    {
      m03 = m13 = m23 = m30 = m31 = m32 = 0.0f;
      m33 = 1;
    }

    void Matrix::clearRotation()
    {
      m00 = 1.0f;
      m01 = 0.0f;
      m02 = 0.0f;

      m10 = 0.0f;
      m11 = 1.0f;
      m12 = 0.0f;

      m20 = 0.0f;
      m21 = 0.0f;
      m22 = 1.0f;
    }

    const glm::mat4& Matrix::getMat4() const
    {
      return m;
    }

    glm::quat Matrix::getQuat() const
    {
      return glm::quat_cast(m);
    }

    glm::mat4 Matrix::getInverse() const
    {
      return glm::inverse(m);
    }

    float Matrix::getDeterminant3x3() const
    {
      return
        +m00 * (m11 * m22 - m21 * m12)
        -m01 * (m10 * m22 - m12 * m20)
        +m02 * (m10 * m21 - m11 * m20);
    }

    glm::mat3 Matrix::getNormalMatrix() const
    {
      return glm::inverseTranspose(glm::mat3(m));
    }

    tuple<glm::vec3, glm::vec3, glm::vec3> Matrix::decomposeEulerXYZ() const
    {
      glm::vec3 translation = glm::vec3(m[3]);

      glm::vec3 scale;
      scale.x = glm::length(glm::vec3(m[0]));
      scale.y = glm::length(glm::vec3(m[1]));
      scale.z = glm::length(glm::vec3(m[2]));

      glm::mat3 rotation(
        glm::vec3(m[0]) / scale[0],
        glm::vec3(m[1]) / scale[1],
        glm::vec3(m[2]) / scale[2]);

      glm::vec3 euler = Euler::fromRotationMatrix(Matrix(rotation));

      return make_tuple(translation, euler, scale);
    }

    tuple<glm::vec3, glm::quat, glm::vec3> Matrix::decomposeQuaternion() const
    {
      glm::vec3 translation = glm::vec3(m[3]);

      glm::vec3 scale;
      scale.x = glm::length(glm::vec3(m[0]));
      scale.y = glm::length(glm::vec3(m[1]));
      scale.z = glm::length(glm::vec3(m[2]));

      glm::mat3 rotation(
        glm::vec3(m[0]) / scale[0],
        glm::vec3(m[1]) / scale[1],
        glm::vec3(m[2]) / scale[2]);

      return make_tuple(translation, glm::quat_cast(rotation), scale);
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

    glm::vec3 Matrix::transformNormal(float x, float y, float z) const
    {
      return getNormalMatrix() * glm::vec3(x, y, z);
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
    void Matrix::addTransformedQuad<GL_TRIANGLES>(const Quad<XYZ> &quad, std::vector<Vertex<XYZ>> &output) const
    {
      TRANSFORM_QUAD_HEADER

      output.emplace_back(TRANSFORM_QUAD_X1_Y1); // x1, y1
      output.emplace_back(TRANSFORM_QUAD_X1_Y2); // x1, y2
      output.emplace_back(TRANSFORM_QUAD_X2_Y2); // x2, y2
      output.emplace_back(TRANSFORM_QUAD_X2_Y1); // x2, y1
    };

    template <>
    void Matrix::addTransformedQuad<GL_TRIANGLES>(const Quad<XYZ.UV> &quad, std::vector<Vertex<XYZ.UV>> &output) const
    {
      TRANSFORM_QUAD_HEADER

      output.emplace_back(TRANSFORM_QUAD_X1_Y1, quad.u1, quad.v1); // x1, y1
      output.emplace_back(TRANSFORM_QUAD_X1_Y2, quad.u1, quad.v2); // x1, y2
      output.emplace_back(TRANSFORM_QUAD_X2_Y2, quad.u2, quad.v2); // x2, y2
      output.emplace_back(TRANSFORM_QUAD_X2_Y1, quad.u2, quad.v1); // x2, y1
    }

    template <>
    void Matrix::addTransformedQuad<GL_TRIANGLES, CCW>(const Quad<XYZ> &quad, IndexedVertexBatch<XYZ, GLuint> &output) const
    {
      TRANSFORM_QUAD_HEADER

      output
        .addVertex(TRANSFORM_QUAD_X1_Y1)
        .addVertex(TRANSFORM_QUAD_X1_Y2)
        .addVertex(TRANSFORM_QUAD_X2_Y2)
        .addVertex(TRANSFORM_QUAD_X2_Y1);

      output
        .addIndices(0, 1, 2, 2, 3, 0)
        .incrementIndices(4);
    }

    template <>
    void Matrix::addTransformedQuad<GL_TRIANGLES, CW>(const Quad<XYZ> &quad, IndexedVertexBatch<XYZ, GLuint> &output) const
    {
      TRANSFORM_QUAD_HEADER

      output
        .addVertex(TRANSFORM_QUAD_X1_Y1)
        .addVertex(TRANSFORM_QUAD_X1_Y2)
        .addVertex(TRANSFORM_QUAD_X2_Y2)
        .addVertex(TRANSFORM_QUAD_X2_Y1);

      output
        .addIndices(0, 3, 2, 2, 1, 0)
        .incrementIndices(4);
    }

    template <>
    void Matrix::addTransformedQuad<GL_TRIANGLES, CCW>(const Quad<XYZ.N> &quad, IndexedVertexBatch<XYZ.N, GLuint> &output) const
    {
      TRANSFORM_QUAD_HEADER

      const auto &transformedNormal = transformNormal(quad.normal);

      output
        .addVertex(TRANSFORM_QUAD_X1_Y1, transformedNormal)
        .addVertex(TRANSFORM_QUAD_X1_Y2, transformedNormal)
        .addVertex(TRANSFORM_QUAD_X2_Y2, transformedNormal)
        .addVertex(TRANSFORM_QUAD_X2_Y1, transformedNormal);

      output
        .addIndices(0, 1, 2, 2, 3, 0)
        .incrementIndices(4);
    }

    template <>
    void Matrix::addTransformedQuad<GL_TRIANGLES, CW>(const Quad<XYZ.N> &quad, IndexedVertexBatch<XYZ.N, GLuint> &output) const
    {
      TRANSFORM_QUAD_HEADER

      const auto &transformedNormal = transformNormal(quad.normal);

      output
        .addVertex(TRANSFORM_QUAD_X1_Y1, transformedNormal)
        .addVertex(TRANSFORM_QUAD_X1_Y2, transformedNormal)
        .addVertex(TRANSFORM_QUAD_X2_Y2, transformedNormal)
        .addVertex(TRANSFORM_QUAD_X2_Y1, transformedNormal);

      output
        .addIndices(0, 3, 2, 2, 1, 0)
        .incrementIndices(4);
    }

    template <>
    void Matrix::addTransformedQuad<GL_TRIANGLES, CCW>(const Quad<XYZ.UV> &quad, IndexedVertexBatch<XYZ.UV, GLuint> &output) const
    {
      TRANSFORM_QUAD_HEADER

      output
        .addVertex(TRANSFORM_QUAD_X1_Y1, quad.u1, quad.v1)
        .addVertex(TRANSFORM_QUAD_X1_Y2, quad.u1, quad.v2)
        .addVertex(TRANSFORM_QUAD_X2_Y2, quad.u2, quad.v2)
        .addVertex(TRANSFORM_QUAD_X2_Y1, quad.u2, quad.v1);

      output
        .addIndices(0, 1, 2, 2, 3, 0)
        .incrementIndices(4);
    }

    template <>
    void Matrix::addTransformedQuad<GL_TRIANGLES, CW>(const Quad<XYZ.UV> &quad, IndexedVertexBatch<XYZ.UV, GLuint> &output) const
    {
      TRANSFORM_QUAD_HEADER

      output
        .addVertex(TRANSFORM_QUAD_X1_Y1, quad.u1, quad.v1)
        .addVertex(TRANSFORM_QUAD_X1_Y2, quad.u1, quad.v2)
        .addVertex(TRANSFORM_QUAD_X2_Y2, quad.u2, quad.v2)
        .addVertex(TRANSFORM_QUAD_X2_Y1, quad.u2, quad.v1);

      output
        .addIndices(0, 3, 2, 2, 1, 0)
        .incrementIndices(4);
    }

    template <>
    void Matrix::addTransformedQuad<GL_TRIANGLES, CCW>(const Quad<XYZ.RGBA> &quad, IndexedVertexBatch<XYZ.RGBA, GLuint> &output) const
    {
      TRANSFORM_QUAD_HEADER

      output
        .addVertex(TRANSFORM_QUAD_X1_Y1, quad.color)
        .addVertex(TRANSFORM_QUAD_X1_Y2, quad.color)
        .addVertex(TRANSFORM_QUAD_X2_Y2, quad.color)
        .addVertex(TRANSFORM_QUAD_X2_Y1, quad.color);

      output
        .addIndices(0, 1, 2, 2, 3, 0)
        .incrementIndices(4);
    }

    template <>
    void Matrix::addTransformedQuad<GL_TRIANGLES, CW>(const Quad<XYZ.RGBA> &quad, IndexedVertexBatch<XYZ.RGBA, GLuint> &output) const
    {
      TRANSFORM_QUAD_HEADER

      output
        .addVertex(TRANSFORM_QUAD_X1_Y1, quad.color)
        .addVertex(TRANSFORM_QUAD_X1_Y2, quad.color)
        .addVertex(TRANSFORM_QUAD_X2_Y2, quad.color)
        .addVertex(TRANSFORM_QUAD_X2_Y1, quad.color);

      output
        .addIndices(0, 3, 2, 2, 1, 0)
        .incrementIndices(4);
    }

    template <>
    void Matrix::addTransformedQuad<GL_TRIANGLES, CCW>(const Quad<XYZ.UV.RGBA> &quad, IndexedVertexBatch<XYZ.UV.RGBA, GLuint> &output) const
    {
      TRANSFORM_QUAD_HEADER

      output
        .addVertex(TRANSFORM_QUAD_X1_Y1, quad.u1, quad.v1, quad.color)
        .addVertex(TRANSFORM_QUAD_X1_Y2, quad.u1, quad.v2, quad.color)
        .addVertex(TRANSFORM_QUAD_X2_Y2, quad.u2, quad.v2, quad.color)
        .addVertex(TRANSFORM_QUAD_X2_Y1, quad.u2, quad.v1, quad.color);

      output
        .addIndices(0, 1, 2, 2, 3, 0)
        .incrementIndices(4);
    }

    template <>
    void Matrix::addTransformedQuad<GL_TRIANGLES, CW>(const Quad<XYZ.UV.RGBA> &quad, IndexedVertexBatch<XYZ.UV.RGBA, GLuint> &output) const
    {
      TRANSFORM_QUAD_HEADER

      output
        .addVertex(TRANSFORM_QUAD_X1_Y1, quad.u1, quad.v1, quad.color)
        .addVertex(TRANSFORM_QUAD_X1_Y2, quad.u1, quad.v2, quad.color)
        .addVertex(TRANSFORM_QUAD_X2_Y2, quad.u2, quad.v2, quad.color)
        .addVertex(TRANSFORM_QUAD_X2_Y1, quad.u2, quad.v1, quad.color);

      output
        .addIndices(0, 3, 2, 2, 1, 0)
        .incrementIndices(4);
    }

    template <>
    void Matrix::addTransformedQuad<GL_TRIANGLES, CCW>(const Quad<XYZ.N.RGBA> &quad, IndexedVertexBatch<XYZ.N.RGBA, GLuint> &output) const
    {
      TRANSFORM_QUAD_HEADER

      const auto &transformedNormal = transformNormal(quad.normal);

      output
        .addVertex(TRANSFORM_QUAD_X1_Y1, transformedNormal, quad.color)
        .addVertex(TRANSFORM_QUAD_X1_Y2, transformedNormal, quad.color)
        .addVertex(TRANSFORM_QUAD_X2_Y2, transformedNormal, quad.color)
        .addVertex(TRANSFORM_QUAD_X2_Y1, transformedNormal, quad.color);

      output
        .addIndices(0, 1, 2, 2, 3, 0)
        .incrementIndices(4);
    }

    template <>
    void Matrix::addTransformedQuad<GL_TRIANGLES, CW>(const Quad<XYZ.N.RGBA> &quad, IndexedVertexBatch<XYZ.N.RGBA, GLuint> &output) const
    {
      TRANSFORM_QUAD_HEADER

      const auto &transformedNormal = transformNormal(quad.normal);

      output
        .addVertex(TRANSFORM_QUAD_X1_Y1, transformedNormal, quad.color)
        .addVertex(TRANSFORM_QUAD_X1_Y2, transformedNormal, quad.color)
        .addVertex(TRANSFORM_QUAD_X2_Y2, transformedNormal, quad.color)
        .addVertex(TRANSFORM_QUAD_X2_Y1, transformedNormal, quad.color);

      output
        .addIndices(0, 3, 2, 2, 1, 0)
        .incrementIndices(4);
    }

    template <>
    void Matrix::addTransformedQuad<GL_TRIANGLES, CCW>(const Quad<XYZ.N.UV> &quad, IndexedVertexBatch<XYZ.N.UV, GLuint> &output) const
    {
      TRANSFORM_QUAD_HEADER

      const auto &transformedNormal = transformNormal(quad.normal);

      output
        .addVertex(TRANSFORM_QUAD_X1_Y1, transformedNormal, quad.u1, quad.v1)
        .addVertex(TRANSFORM_QUAD_X1_Y2, transformedNormal, quad.u1, quad.v2)
        .addVertex(TRANSFORM_QUAD_X2_Y2, transformedNormal, quad.u2, quad.v2)
        .addVertex(TRANSFORM_QUAD_X2_Y1, transformedNormal, quad.u2, quad.v1);

      output
        .addIndices(0, 1, 2, 2, 3, 0)
        .incrementIndices(4);
    }

    template <>
    void Matrix::addTransformedQuad<GL_TRIANGLES, CW>(const Quad<XYZ.N.UV> &quad, IndexedVertexBatch<XYZ.N.UV, GLuint> &output) const
    {
      TRANSFORM_QUAD_HEADER

      const auto &transformedNormal = transformNormal(quad.normal);

      output
        .addVertex(TRANSFORM_QUAD_X1_Y1, transformedNormal, quad.u1, quad.v1)
        .addVertex(TRANSFORM_QUAD_X1_Y2, transformedNormal, quad.u1, quad.v2)
        .addVertex(TRANSFORM_QUAD_X2_Y2, transformedNormal, quad.u2, quad.v2)
        .addVertex(TRANSFORM_QUAD_X2_Y1, transformedNormal, quad.u2, quad.v1);

      output
        .addIndices(0, 3, 2, 2, 1, 0)
        .incrementIndices(4);
    }
  }
}
