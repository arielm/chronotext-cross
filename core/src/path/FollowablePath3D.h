#pragma once

#include "glm.h"

#include <vector>

namespace chr
{
  namespace path
  {
    class FollowablePath3D
    {
    public:
      enum Mode
      {
        MODE_BOUNDED,
        MODE_LOOP,
        MODE_TANGENT,
        MODE_MODULO,
        MODE_CLAMPED
      };

      enum Sampling
      {
        SAMPLING_NONE,
        SAMPLING_CONTINUOUS,
        SAMPLING_CORNERS
      };

      struct Point
      {
        glm::vec3 left;
        glm::vec3 forward;
        glm::vec3 position;

        Point(const glm::vec3 &position)
          :
          position(position)
        { }

        Point(const glm::vec3 &position, const glm::vec3 &left)
          :
          position(position),
          left(left)
        { }

        glm::quat toQuat() const;
      };

      struct Value
      {
        glm::vec3 forward;
        glm::vec3 left;
        glm::vec3 up;
        glm::vec3 position;
        float offset = -1;

        void applyToMatrix(glm::mat4 &m) const;
        void applyToMatrix(std::array<float, 16> &m) const;

        glm::quat toQuat() const;
        void fromQuat(const glm::quat &q);
      };

      FollowablePath3D(size_t capacity = 0);

      void clear();
      void reserve(size_t n);
      size_t size() const;
      bool empty() const;

      FollowablePath3D& setMode(Mode mode);
      FollowablePath3D& setSampling(Sampling sampling);

      float getLength() const;
      const std::vector<Point>& getPoints() const;
      const std::vector<float>& getLengths() const;

      FollowablePath3D& begin();
      FollowablePath3D& end(bool close = false);

      FollowablePath3D& add(const glm::vec3 &position, const glm::vec3 &left);

      glm::quat offsetToQuat(float offset, float sampleSize = 0) const;
      FollowablePath3D::Value offsetToValue(float offset, float sampleSize = 0) const;

    protected:
      Mode mode = MODE_TANGENT;
      Sampling sampling = SAMPLING_CONTINUOUS;

      std::vector<Point> points;
      std::vector<float> lengths;
    };
  }
}
