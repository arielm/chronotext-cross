#include "chr/path/FollowablePath2D.h"

#include "chr/utils/Utils.h"
#include "chr/math/Utils.h"

using namespace std;
using namespace chr;
using namespace math;

namespace chr
{
  namespace path
  {
    FollowablePath2D::FollowablePath2D(size_t capacity)
    :
    mode(MODE_TANGENT)
    {
      if (capacity > 0)
      {
        points.reserve(capacity);
        lengths.reserve(capacity);
      }
    }

    void FollowablePath2D::clear()
    {
      points.clear();
      lengths.clear();
    }

    void FollowablePath2D::reserve(size_t n)
    {
      points.reserve(n);
      lengths.reserve(n);
    }

    size_t FollowablePath2D::size() const
    {
      return points.size();
    }

    bool FollowablePath2D::empty() const
    {
      return points.empty();
    }

    FollowablePath2D& FollowablePath2D::setMode(Mode mode)
    {
      this->mode = mode;
      return *this;
    }

    float FollowablePath2D::getLength() const
    {
      if (!empty())
      {
        return lengths.back();
      }
      else
      {
        return 0;
      }
    }

    const vector<FollowablePath2D::Point>& FollowablePath2D::getPoints() const
    {
      return points;
    }

    const vector<float>& FollowablePath2D::getLengths() const
    {
      return lengths;
    }

    vector<glm::vec2> FollowablePath2D::getPolyline() const
    {
      vector<glm::vec2> polyline;

      for (const auto &point : points)
      {
        polyline.push_back(point.position);
      }

      return polyline;
    }

    FollowablePath2D& FollowablePath2D::begin()
    {
      points.clear();
      lengths.clear();

      return *this;
    }

    FollowablePath2D& FollowablePath2D::end(bool close)
    {
      auto end = size();
      bool closed = (end > 2) && (points.front().position == points.back().position);

      if (!closed)
      {
        if (close && (end > 2))
        {
            add(points.front().position);
            points.back().forward = points.front().forward;
        }
        else
        {
          if (end > 1)
          {
            points[end - 1].forward = points[end - 2].forward;
          }
        }
      }

      return *this;
    }

    FollowablePath2D& FollowablePath2D::add(const vector<glm::vec2> &polyline)
    {
      points.reserve(points.size() + polyline.size());

      for (const auto &point : polyline)
      {
        add(point);
      }

      return *this;
    }

    FollowablePath2D& FollowablePath2D::add(const MatrixAffine &matrix, const vector<glm::vec2> &polyline)
    {
      points.reserve(points.size() + polyline.size());

      for (const auto &point : polyline)
      {
        add(matrix.transformPoint(point));
      }

      return *this;
    }

    FollowablePath2D& FollowablePath2D::add(const glm::vec2 &point)
    {
      if (!empty())
      {
        auto delta = point - points.back().position;
        auto length = glm::length(delta); // ASSERT: length > EPSILON

        lengths.push_back(lengths.back() + length);
        points.back().forward = delta / length;
      }
      else
      {
        lengths.push_back(0);
      }

      points.emplace_back(point);
      return *this;
    }

    glm::vec2 FollowablePath2D::offsetToPosition(float offset) const
    {
      float length = getLength();

      if (length > 0)
      {
        if ((mode == MODE_LOOP) || (mode == MODE_MODULO))
        {
          offset = math::boundf(offset, length);
        }
        else if (mode == MODE_PALINDROME)
        {
            int n = offset / length;
            if (n % 2 != 0)
            {
                offset = length - math::boundf(offset, length);
            }
            else
            {
                offset = math::boundf(offset, length);
            }
        }
        else if (offset <= 0)
        {
          if (mode == MODE_BOUNDED)
          {
            return points.front().position;
          }
        }
        else if (offset >= length)
        {
          if (mode == MODE_BOUNDED)
          {
            return points.back().position;
          }
        }

        auto index = utils::search(lengths, offset, 1, size());
        auto &p0 = points[index];
        auto &p1 = points[index + 1];

        float u = (offset - lengths[index]) / (lengths[index + 1] - lengths[index]);
        return p0.position * (1 - u) + p1.position * u;
      }

      return glm::zero<glm::vec2>();
    }

    glm::vec2 FollowablePath2D::offsetToTangent(float offset, float sampleSize) const
    {
      float length = getLength();

      if (length > 0)
      {
        if (sampleSize > 0)
        {
          float offset0, offset1;

          if (mode == MODE_MODULO)
          {
            offset = math::boundf(offset, length);
          }
          else if (mode == MODE_PALINDROME)
          {
              int n = offset / length;
              if (n % 2 != 0)
              {
                  offset = length - math::boundf(offset, length);
              }
              else
              {
                  offset = math::boundf(offset, length);
              }
          }

          if (mode == MODE_LOOP)
          {
            offset0 = math::boundf(offset - sampleSize * 0.5f, length);
            offset1 = math::boundf(offset + sampleSize * 0.5f, length);
          }
          else
          {
            offset0 = offset - sampleSize * 0.5f;
            offset1 = offset + sampleSize * 0.5f;
          }

          if (mode == MODE_BOUNDED)
          {
            if ((offset1 <= 0) || ((offset0 < 0) && (offset1 > 0)))
            {
              return points.front().forward;
            }
            else if ((offset0 >= length) || ((offset1 > length) && (offset0 < length)))
            {
              return points.back().forward;
            }
          }

          auto index0 = utils::search(lengths, offset0, 1, size());
          auto index1 = utils::search(lengths, offset1, 1, size());

          if (index0 == index1)
          {
            return points[index0].forward;
          }
          else
          {
            float u0 = (offset0 - lengths[index0]) / (lengths[index0 + 1] - lengths[index0]);
            auto p0 = points[index0].position * (1 - u0) + points[index0 + 1].position * u0;

            float u1 = (offset1 - lengths[index1]) / (lengths[index1 + 1] - lengths[index1]);
            auto p1 = points[index1].position * (1 - u1) + points[index1 + 1].position * u1;

            return glm::normalize(p1 - p0);
          }
        }
        else
        {
          if ((mode == MODE_LOOP) || (mode == MODE_MODULO))
          {
            offset = math::boundf(offset, length);
          }
          else if (offset <= 0)
          {
            if (mode == MODE_BOUNDED)
            {
              return points.front().forward;
            }
          }
          else if (offset >= length)
          {
            if (mode == MODE_BOUNDED)
            {
              return points.back().forward;
            }
          }

          auto index = utils::search(lengths, offset, 1, size());
          return points[index].forward;
        }
      }

      return glm::zero<glm::vec2>();
    }

    FollowablePath2D::Value FollowablePath2D::offsetToValue(float offset, float sampleSize) const
    {
      Value value;

      float length = getLength();

      if (length > 0)
      {
        if ((mode == MODE_LOOP) || (mode == MODE_MODULO))
        {
          offset = math::boundf(offset, length);
        }
        else if (mode == MODE_PALINDROME)
        {
            int n = offset / length;
            if (n % 2 != 0)
            {
                offset = length - math::boundf(offset, length);
            }
            else
            {
                offset = math::boundf(offset, length);
            }
        }
        else if (offset <= 0)
        {
          if (mode == MODE_BOUNDED)
          {
            value.position = points.front().position;
            value.forward = points.front().forward;
            value.up = value.forward.yx() * glm::vec2(-1, +1);
            value.offset = 0;
            value.index = 0;

            goto exit; // BECAUSE OF RVO
          }
        }
        else if (offset >= length)
        {
          if (mode == MODE_BOUNDED)
          {
            value.position = points.back().position;
            value.forward = points.back().forward;
            value.up = value.forward.yx() * glm::vec2(-1, +1);
            value.offset = length;
            value.index = points.size() - 1;

            goto exit; // BECAUSE OF RVO
          }
        }

        auto index = utils::search(lengths, offset, 1, size());
        auto &p0 = points[index];
        auto &p1 = points[index + 1];

        float u = (offset - lengths[index]) / (lengths[index + 1] - lengths[index]);

        value.position = p0.position * (1 - u) + p1.position * u;
        value.offset = offset;
        value.index = index;

        if (sampleSize > 0)
        {
          value.forward = offsetToTangent(offset, sampleSize);
        }
        else
        {
          value.forward = p0.forward;
        }

        value.up = value.forward.yx() * glm::vec2(-1, +1);
      }

      exit:
      return value;
    }

    /*
     * RETURNS false IF CLOSEST-POINT IS FARTHER THAN threshold DISTANCE
     *
     * REFERENCE: "Minimum Distance between a Point and a Line" BY Paul Bourke
     * http://paulbourke.net/geometry/pointlineplane
     */
    bool FollowablePath2D::findClosestPoint(const glm::vec2 &input, float threshold, ClosePoint &output) const
    {
      float min = threshold * threshold; // BECAUSE IT IS MORE EFFICIENT TO WORK WITH MAGNIFIED DISTANCES

      int end = size();
      int index = -1;
      glm::vec2 position;
      float offset;

      for (int i = 0; i < end; i++)
      {
        int i0, i1;

        if (i == end - 1)
        {
          i0 = i - 1;
          i1 = i;
        }
        else
        {
          i0 = i;
          i1 = i + 1;
        }

        auto &p0 = points[i0].position;
        auto &p1 = points[i1].position;

        glm::vec2 delta = p1 - p0;
        float length = lengths[i1] - lengths[i0];
        float u = glm::dot(delta, input - p0) / (length * length);

        if (u >= 0 && u <= 1)
        {
          glm::vec2 p = p0 + u * delta;
          float mag = glm::length2(p - input);

          if (mag < min)
          {
            min = mag;
            index = i0;

            position = p;
            offset = lengths[index] + u * length;
          }
        }
        else
        {
          float mag0 = glm::length2(p0 - input);
          float mag1 = glm::length2(p1 - input);

          if ((mag0 < min) && (mag0 < mag1))
          {
            min = mag0;
            index = i0;

            position = p0;
            offset = lengths[index];
          }
          else if ((mag1 < min) && (mag1 < mag0))
          {
            min = mag1;
            index = i1;

            position = p1;
            offset = lengths[index];
          }
        }
      }

      if (index != -1)
      {
        output.position = position;
        output.offset = offset;
        output.distance = sqrtf(min);

        return true;
      }

      return false;
    }

    /*
     * REFERENCE: "Minimum Distance between a Point and a Line" BY Paul Bourke
     * http://paulbourke.net/geometry/pointlineplane
     */
    FollowablePath2D::ClosePoint FollowablePath2D::closestPointFromSegment(const glm::vec2 &input, int segmentIndex) const
    {
      FollowablePath2D::ClosePoint output;

      if ((segmentIndex >= 0) && (segmentIndex + 1 < size()))
      {
        int i0 = segmentIndex;
        int i1 = segmentIndex + 1;

        auto &p0 = points[i0].position;
        auto &p1 = points[i1].position;

        glm::vec2 delta = p1 - p0;
        float length = lengths[i1] - lengths[i0];
        float u = glm::dot(delta, input - p0) / (length * length);

        if (u >= 0 && u <= 1)
        {
          glm::vec2 p = p0 + u * delta;
          float mag = glm::length2(p - input);

          output.position = p;
          output.offset = lengths[i0] + u * length;
          output.distance = sqrtf(mag);
        }
        else
        {
          float mag0 = glm::length2(p0 - input);
          float mag1 = glm::length2(p1 - input);

          if (mag0 < mag1)
          {
            output.position = p0;
            output.offset = lengths[i0];
            output.distance = sqrtf(mag0);
          }
          else
          {
            output.position = p1;
            output.offset = lengths[i1];
            output.distance = sqrtf(mag1);
          }
        }
      }
      else
      {
        output.distance = numeric_limits<float>::max();
      }

      return output;
    }

    // ---

    void FollowablePath2D::Value::applyToMatrix(glm::mat4 &m) const
    {
      m[0][0] = forward.x; m[1][0] = up.x; m[2][0] = 0; m[3][0] = position.x;
      m[0][1] = forward.y; m[1][1] = up.y; m[2][1] = 0; m[3][1] = position.y;
      m[0][2] = 0;         m[1][2] = 0;    m[2][2] = 1; m[3][2] = 0;
      m[0][3] = 0;         m[1][3] = 0;    m[2][3] = 0; m[3][3] = 1;
    }

    void FollowablePath2D::Value::applyToMatrix(array<float, 16> &m) const
    {
      m[0] = forward.x; m[4] = up.x; m[ 8] = 0; m[12] = position.x;
      m[1] = forward.y; m[5] = up.y; m[ 9] = 0; m[13] = position.y;
      m[2] = 0;         m[6] = 0;    m[10] = 1; m[14] = 0;
      m[3] = 0;         m[7] = 0;    m[11] = 0; m[15] = 1;
    }

    void FollowablePath2D::Value::applyToMatrix(array<float, 6> &m) const
    {
      m[0] = forward.x; m[2] = up.x; m[4] = position.x;
      m[1] = forward.y; m[3] = up.y; m[5] = position.y;
    }
  }
}
