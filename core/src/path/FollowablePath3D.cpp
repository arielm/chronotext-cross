#include "path/FollowablePath3D.h"

#include "utils/Utils.h"
#include "math/Utils.h"

using namespace std;
using namespace chr;

namespace chr
{
  namespace path
  {
    FollowablePath3D::FollowablePath3D(size_t capacity)
    {
      if (capacity > 0)
      {
        points.reserve(capacity);
        lengths.reserve(capacity);
      }
    }

    void FollowablePath3D::clear()
    {
      points.clear();
      lengths.clear();
    }

    void FollowablePath3D::reserve(size_t n)
    {
      points.reserve(n);
      lengths.reserve(n);
    }

    size_t FollowablePath3D::size() const
    {
      return points.size();
    }

    bool FollowablePath3D::empty() const
    {
      return points.empty();
    }

    float FollowablePath3D::getLength() const
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

    const vector<FollowablePath3D::Point>& FollowablePath3D::getPoints() const
    {
      return points;
    }

    const vector<float>& FollowablePath3D::getLengths() const
    {
      return lengths;
    }

    FollowablePath3D& FollowablePath3D::setMode(Mode mode)
    {
      this->mode = mode;
      return *this;
    }

    FollowablePath3D& FollowablePath3D::setSampling(Sampling sampling)
    {
      this->sampling = sampling;
      return *this;
    }

    FollowablePath3D& FollowablePath3D::begin()
    {
      points.clear();
      lengths.clear();

      return *this;
    }

    FollowablePath3D& FollowablePath3D::end()
    {
      auto end = size();

      if (end > 1)
      {
        points[end - 1].forward = points[end - 2].forward;
      }

      return *this;
    }

    FollowablePath3D& FollowablePath3D::add(const glm::vec3 &position, const glm::vec3 &left)
    {
      if (!empty())
      {
        auto delta = position - points.back().position;
        auto length = glm::length(delta); // ASSERT: length > EPSILON

        lengths.push_back(lengths.back() + length);
        points.back().forward = delta / length;
      }
      else
      {
        lengths.push_back(0);
      }

      points.emplace_back(position, left);
      return *this;
    }

    glm::quat FollowablePath3D::offsetToQuat(float offset, float sampleSize) const
    {
      float length = getLength();

      if (length > 0)
      {
        if ((sampling != SAMPLING_NONE) && (sampleSize > 0))
        {
          if ((mode == MODE_LOOP) || (mode == MODE_MODULO))
          {
            offset = math::boundf(offset, length);
          }

          float offset0 = offset - sampleSize * 0.5f;
          float offset1 = offset + sampleSize * 0.5f;

          if (mode == MODE_BOUNDED)
          {
            if ((offset1 <= 0) || ((offset0 < 0) && (offset1 > 0)))
            {
              return points.front().toQuat();
            }
            else if ((offset0 >= length) || ((offset1 > length) && (offset0 < length)))
            {
              return points.back().toQuat();
            }
          }

          auto index0 = utils::search(lengths, offset0, 1, size());
          auto index1 = utils::search(lengths, offset1, 1, size());

          if (index1 < index0)
          {
            return points[index0].toQuat();
          }
          else if (index0 > index1)
          {
            return points[index1].toQuat();
          }
          else
          {
            if (sampling == SAMPLING_CONTINUOUS)
            {
              float u0 = (offset0 - lengths[index0]) / (lengths[index0 + 1] - lengths[index0]);
              auto q0 = glm::slerp(points[index0].toQuat(), points[index0 + 1].toQuat(), u0);

              float u1 = (offset1 - lengths[index1]) / (lengths[index1 + 1] - lengths[index1]);
              auto q1 = glm::slerp(points[index1].toQuat(), points[index1 + 1].toQuat(), u1);

              return glm::slerp(q0, q1, 0.5f);
            }
            else if (sampling == SAMPLING_CORNERS)
            {
              float limit0 = lengths[index0 + 1] - sampleSize * 0.5f;
              float limit1 = lengths[index1] + sampleSize * 0.5f;

              if (offset < limit0)
              {
                return points[index0].toQuat();
              }
              else if (offset > limit1)
              {
                return points[index1].toQuat();
              }
              else
              {
                auto q0 = points[index0].toQuat();
                auto q1 = points[index1].toQuat();

                float ratio = (offset - limit0) / (limit1 - limit0);
                return glm::slerp(q0, q1, ratio);
              }
            }
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
              return points.front().toQuat();
            }
          }
          else if (offset >= length)
          {
            if (mode == MODE_BOUNDED)
            {
              return points.back().toQuat();
            }
          }

          auto index = utils::search(lengths, offset, 1, size());
          return points[index].toQuat();
        }
      }

      return glm::quat();
    }

    FollowablePath3D::Value FollowablePath3D::offsetToValue(float offset, float sampleSize) const
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
            value.left = points.front().left;
            value.forward = points.front().forward;
            value.up = glm::cross(-value.left, value.forward);
            value.offset = 0;

            goto exit;
          }
          else if (mode == MODE_CLAMPED)
          {
            goto exit;
          }
        }
        else if (offset >= length)
        {
          if (mode == MODE_BOUNDED)
          {
            value.position = points.back().position;
            value.left = points.front().left;
            value.forward = points.back().forward;
            value.up = glm::cross(-value.left, value.forward);
            value.offset = length;

            goto exit;
          }
          else if (mode == MODE_CLAMPED)
          {
            goto exit;
          }
        }

        auto index = utils::search(lengths, offset, 1, size());
        auto &p0 = points[index];
        auto &p1 = points[index + 1];

        float u = (offset - lengths[index]) / (lengths[index + 1] - lengths[index]);

        value.position = p0.position * (1 - u) + p1.position * u;
        value.offset = offset;

        if (sampleSize > 0)
        {
          value.fromQuat(offsetToQuat(offset, sampleSize));
        }
        else if (sampling == SAMPLING_CONTINUOUS)
        {
          value.fromQuat(glm::slerp(p0.toQuat(), p1.toQuat(), u));
        }
        else
        {
          value.forward = p0.forward;
          value.left = p0.left;
          value.up = glm::cross(-value.left, value.forward);
        }
      }

      exit:
      return value;
    }

    // ---

    glm::quat FollowablePath3D::Point::toQuat() const
    {
      auto up(glm::cross(-left, forward));

      glm::mat4 m;
      m[0][0] = forward.x; m[1][0] = left.x; m[2][0] = up.x;
      m[0][1] = forward.y; m[1][1] = left.y; m[2][1] = up.y;
      m[0][2] = forward.z; m[1][2] = left.z; m[2][2] = up.z;

      return glm::quat_cast(m);
    }

    void FollowablePath3D::Value::applyToMatrix(glm::mat4 &m) const
    {
      m[0][0] = forward.x; m[1][0] = left.x; m[2][0] = up.x; m[3][0] = position.x;
      m[0][1] = forward.y; m[1][1] = left.y; m[2][1] = up.y; m[3][1] = position.y;
      m[0][2] = forward.z; m[1][2] = left.z; m[2][2] = up.z; m[3][2] = position.z;
      m[0][3] = 0;         m[1][3] = 0;      m[2][3] = 0;    m[3][3] = 1;
    }

    void FollowablePath3D::Value::applyToMatrix(array<float, 16> &m) const
    {
      m[0] = forward.x; m[4] = left.x; m[ 8] = up.x; m[12] = position.x;
      m[1] = forward.y; m[5] = left.y; m[ 9] = up.y; m[13] = position.y;
      m[2] = forward.z; m[6] = left.z; m[10] = up.z; m[14] = position.z;
      m[3] = 0;         m[7] = 0;      m[11] = 0;    m[15] = 1;
    }

    void FollowablePath3D::Value::fromQuat(const glm::quat &q)
    {
      float xx = q.x * q.x;
      float xy = q.x * q.y;
      float xz = q.x * q.z;
      float xw = q.x * q.w;

      float yy = q.y * q.y;
      float yz = q.y * q.z;
      float yw = q.y * q.w;

      float zz = q.z * q.z;
      float zw = q.z * q.w;

      forward.x = 1 - 2 * (yy + zz);
      left.x = 2 * (xy - zw);
      up.x = 2 * (xz + yw);

      forward.y = 2 * (xy + zw);
      left.y = 1 - 2 * (xx + zz);
      up.y = 2 * (yz - xw);

      forward.z = 2 * (xz - yw);
      left.z = 2 * (yz + xw);
      up.z = 1 - 2 * (xx + yy);
    }
  }
}
