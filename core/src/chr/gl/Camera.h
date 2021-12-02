#pragma once

#include "chr/gl/Matrix.h"
#include "chr/math/Ray.h"

namespace chr
{
  namespace gl
  {
    class Camera
    {
    public:
      Camera& setFov(float fovY); // IN DEGREES
      Camera& setClip(float nearZ, float farZ);
      Camera& setWindowSize(const glm::vec2 &size);

      Camera& setPan2D(const glm::vec2 &pan);
      Camera& setZoom2D(float zoom);

      Matrix& getViewMatrix();
      glm::mat4 getProjectionMatrix();
      glm::mat4 getViewProjectionMatrix();
      glm::mat3 getNormalMatrix();

      glm::vec3 getEyePosition();
      math::Ray getRay(const glm::vec2 &windowPosition); // IN PIXEL COORDINATES; TOP-LEFT CORNER AT 0,0
      math::Ray getRay(float u, float v); // IN VIEWPORT COORDINATES; TOP-LEFT CORNER AT 0,0

      inline Camera& setPan2D(float x, float y) { return setPan2D(glm::vec2(x, y)); }

    protected:
      bool updateRequired = false;

      float fovY = 60;
      float nearZ = 0.1f;
      float farZ = 1000.0f;
      glm::vec2 windowSize = { 1, 1 };

      glm::vec2 pan2D;
      float zoom2D = 1;
      glm::vec2 frustumSize;

      glm::mat4 projectionMatrix;
      Matrix viewMatrix;

      void update();
    };
  }
}
