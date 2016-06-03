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
      Camera& setAspectRatio(const glm::vec2 &size);

      Matrix& getModelViewMatrix();
      glm::mat4 getProjectionMatrix();
      glm::mat4 getModelViewProjectionMatrix();
      glm::mat3 getNormalMatrix();

      inline Matrix& getMVMatrix() { return getModelViewMatrix(); }
      inline glm::mat4 getMVPMatrix() { return getModelViewProjectionMatrix(); }

    protected:
      float fovY = 60;
      float nearZ = 0.1f;
      float farZ = 1000.0f;
      float aspectRatio = 1;

      glm::mat4 projectionMatrix;
      Matrix modelViewMatrix;

      bool updateRequired = false;
    };
  }
}
