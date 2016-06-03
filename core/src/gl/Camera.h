#pragma once

#include "gl/Matrix.h"

namespace chr
{
  namespace gl
  {
    class Camera
    {
    public:
      Camera& setFov(float fov); // IN DEGREES
      Camera& setClip(float near, float far);
      Camera& setAspectRatio(float ratio);
      Camera& setSize(const glm::vec2 &size);

      glm::mat4 getProjectionMatrix();
      glm::mat4 getModelViewProjectionMatrix();
      glm::mat3 getNormalMatrix();
      Matrix& getModelViewMatrix();

    protected:
      float fovY = 60;
      float nearZ = 0.1;
      float farZ = 1000;
      float aspectRatio = 1;

      bool updateRequired = false;

      glm::mat4 projectionMatrix;
      Matrix modelViewMatrix;
    };
  }
}
