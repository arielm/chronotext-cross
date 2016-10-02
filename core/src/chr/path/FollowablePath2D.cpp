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

    FollowablePath2D& FollowablePath2D::begin()
    {
      points.clear();
      lengths.clear();

      return *this;
    }

    FollowablePath2D& FollowablePath2D::end(bool close)
    {
      auto end = size();

      if (close)
      {
        if ((end > 2) && (points.front().position != points.back().position))
        {
          add(points.front().position);
          points.back().forward = points.front().forward;

          return *this;
        }
      }

      if (end > 1)
      {
        points[end - 1].forward = points[end - 2].forward;
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

          if (mode == MODE_LOOP)
          {
            offset0 = math::boundf(offset - sampleSize * 0.5f, length);
            offset1 = math::boundf(offset + sampleSize * 0.5f, length);

            if (offset1 < offset0)
            {
              std::swap(offset0, offset1);
            }
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

          if ((index0 == index1) || (index1 < index0))
          {
            return points[index0].forward;

          }
          else if (index0 > index1)
          {
            return points[index1].forward;
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
