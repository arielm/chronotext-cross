#include "chr/math/MatrixAffine.h"

using namespace std;

namespace chr
{
  namespace math
  {
    MatrixAffine::MatrixAffine()
    :
    m00(1),
    m10(0),
    m01(0),
    m11(1),
    m02(0),
    m12(0)
    {}

    MatrixAffine::MatrixAffine(const Values &values)
    :
    m(values)
    {}

    MatrixAffine& MatrixAffine::set(const Values &values)
    {
      m = values;
      return *this;
    }

    MatrixAffine& MatrixAffine::set(const MatrixAffine &matrix)
    {
      m = matrix.m;
      return *this;
    }

    MatrixAffine& MatrixAffine::push()
    {
      stack.push_back(m);
      return *this;
    }

    MatrixAffine& MatrixAffine::pop()
    {
      if (!stack.empty())
      {
        m = stack.back();
        stack.pop_back();
      }

      return *this;
    }

    MatrixAffine& MatrixAffine::setIdentity()
    {
      m00 = m11 = 1.0f;
      m01 = m02 = m10 = m12 = 0.0f;

      return *this;
    }

    MatrixAffine& MatrixAffine::setTranslate(float x, float y)
    {
      m00 = m11 = 1.0f;
      m01 = m10 = 0.0f;
      
      m02 = x;
      m12 = y;

      return *this;
    }

    MatrixAffine& MatrixAffine::translate(float x, float y)
    {
      m02 += x * m00 + y * m01;
      m12 += x * m10 + y * m11;

      return *this;
    }

    MatrixAffine& MatrixAffine::scale(float x, float y)
    {
      m00 *= x;
      m10 *= x;
      
      m01 *= y;
      m11 *= y;

      return *this;
    }

    MatrixAffine& MatrixAffine::rotate(float a)
    {
      float c = cosf(a);
      float s = sinf(a);

      float r00 = m00 * c + m01 * s;
      float r01 = m01 * c - m00 * s;

      float r10 = m10 * c + m11 * s;
      float r11 = m11 * c - m10 * s;

      m00 = r00;
      m01 = r01;
      m10 = r10;
      m11 = r11;

      return *this;
    }

    void MatrixAffine::invert()
    {
      float determinant = m00 * m11 - m01 * m10;

      if (fabsf(determinant) > 0.000001f) // XXX
      {
        float t00 = m00;
        float t01 = m01;
        float t02 = m02;
        float t10 = m10;
        float t11 = m11;
        float t12 = m12;

        m00 = +t11 / determinant;
        m10 = -t10 / determinant;
        m01 = -t01 / determinant;
        m11 = +t00 / determinant;
        m02 = (t01 * t12 - t11 * t02) / determinant;
        m12 = (t10 * t02 - t00 * t12) / determinant;
      }
    }

    MatrixAffine::Values MatrixAffine::getInverse() const
    {
      Values result;

      float determinant = m00 * m11 - m01 * m10;

      if (fabsf(determinant) > 0.000001f) // XXX
      {
        result[0] = +m11 / determinant;
        result[1] = -m10 / determinant;
        result[2] = -m01 / determinant;
        result[3] = +m00 / determinant;
        result[4] = (m01 * m12 - m11 * m02) / determinant;
        result[5] = (m10 * m02 - m00 * m12) / determinant;
      }

      return result;
    }

    glm::vec2 MatrixAffine::transformPoint(float x, float y) const
    {
      return glm::vec2(x * m00 + y * m01 + m02, x * m10 + y * m11 + m12);
    }

    vector<glm::vec2> MatrixAffine::transformRect(const Rectf &rect) const
    {
      float x100 = rect.x1 * m00 + m02;
      float x110 = rect.x1 * m10 + m12;

      float y101 = rect.y1 * m01;
      float y111 = rect.y1 * m11;

      float x200 = rect.x2 * m00 + m02;
      float x210 = rect.x2 * m10 + m12;

      float y201 = rect.y2 * m01;
      float y211 = rect.y2 * m11;

      vector<glm::vec2> output;
      output.reserve(4);

      output.emplace_back(x100 + y101, x110 + y111); // x1, y1
      output.emplace_back(x100 + y201, x110 + y211); // x1, y2
      output.emplace_back(x200 + y201, x210 + y211); // x2, y2
      output.emplace_back(x200 + y101, x210 + y111); // x2, y1

      return output;
    }

    vector<glm::vec2> MatrixAffine::transformPoints(const vector<glm::vec2> &points)
    {
      vector<glm::vec2> output;
      output.reserve(points.size());

      for (const auto &point : points)
      {
        output.emplace_back(transformPoint(point));
      }

      return output;
    }
  }
}
