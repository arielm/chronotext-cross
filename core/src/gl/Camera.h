#pragma once

#include "gl/Matrix.h"
#include "math/Ray.h"

namespace chr
{
  namespace gl
  {
    class Camera
    {
    public:
      Camera& setFov(float fov); // IN DEGREES
      Camera& setClip(float near, float far);
      Camera& setWindowSize(const glm::vec2 &size);

      Matrix& getModelViewMatrix();
      glm::mat4 getProjectionMatrix();
      glm::mat4 getModelViewProjectionMatrix();
      glm::mat3 getNormalMatrix();

      inline Matrix& getMVMatrix() { return getModelViewMatrix(); }
      inline glm::mat4 getMVPMatrix() { return getModelViewProjectionMatrix(); }

      glm::vec3 getEyePosition();
      math::Ray getRay(const glm::vec2 &windowPosition);

    protected:
      float fovY = 60;
      float nearZ = 0.1f;
      float farZ = 1000.0f;
      glm::vec2 windowSize = { 1, 1 };

      glm::mat4 projectionMatrix;
      Matrix modelViewMatrix;

      float frustumWidth;
      float frustumHeight;

      bool updateRequired = false;

      void update();
    };
  }
}
