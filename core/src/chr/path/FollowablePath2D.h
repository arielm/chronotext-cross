#pragma once

#include "chr/math/MatrixAffine.h"

namespace chr
{
  namespace path
  {
    class FollowablePath2D
    {
    public:
      enum Mode
      {
        MODE_BOUNDED,
        MODE_LOOP,
        MODE_TANGENT,
        MODE_MODULO,
      };

      struct Point
      {
        glm::vec2 position;
        glm::vec2 forward;

        Point(const glm::vec2 &position)
        :
        position(position)
        {}
      };

      struct Value
      {
        glm::vec2 position;
        glm::vec2 forward;
        glm::vec2 up;
        float offset;
        int index;

        void applyToMatrix(glm::mat4 &m) const;
        void applyToMatrix(std::array<float, 16> &m) const;
        void applyToMatrix(std::array<float, 6> &m) const;
      };

      FollowablePath2D(size_t capacity = 0);

      void clear();
      void reserve(size_t n);
      size_t size() const;
      bool empty() const;

      FollowablePath2D& setMode(Mode mode);

      float getLength() const;
      const std::vector<Point>& getPoints() const;
      const std::vector<float>& getLengths() const;

      FollowablePath2D& begin();
      FollowablePath2D& end(bool close = false);

      FollowablePath2D& add(const std::vector<glm::vec2> &polyline);
      FollowablePath2D& add(const math::MatrixAffine &matrix, const std::vector<glm::vec2> &polyline);

      FollowablePath2D& add(const glm::vec2 &point);
      inline FollowablePath2D& add(float x, float y) { return add(glm::vec2(x, y)); }

      glm::vec2 offsetToPosition(float offset) const;
      glm::vec2 offsetToTangent(float offset, float sampleSize = 0) const;
      FollowablePath2D::Value offsetToValue(float offset, float sampleSize = 0) const;

    protected:
      Mode mode;

      std::vector<Point> points;
      std::vector<float> lengths;
    };
  }
}