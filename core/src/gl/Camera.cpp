#include "gl/Camera.h"
#include "math/Utils.h"

using namespace std;

namespace chr
{
  namespace gl
  {
    Camera& Camera::setFov(float fov)
    {
      fovY = fov;
      updateRequired = true;

      return *this;
    }

    Camera& Camera::setClip(float near, float far)
    {
      nearZ = near;
      farZ = far;
      updateRequired = true;

      return *this;
    }

    Camera& Camera::setAspectRatio(float ratio)
    {
      aspectRatio = ratio;
      updateRequired = true;

      return *this;
    }

    Camera& Camera::setSize(const glm::vec2 &size)
    {
      aspectRatio = size.x / size.y;
      updateRequired = true;

      return *this;
    }

    glm::mat4 Camera::getProjectionMatrix()
    {
      if (updateRequired)
      {
        projectionMatrix = glm::perspective(fovY * D2R, aspectRatio, nearZ, farZ);
        updateRequired = false;
      }

      return projectionMatrix;
    }

    glm::mat4 Camera::getModelViewProjectionMatrix()
    {
      return modelViewMatrix * getProjectionMatrix();
    }

    glm::mat3 Camera::getNormalMatrix()
    {
      return modelViewMatrix.getNormalMatrix();
    }

    Matrix& Camera::getModelViewMatrix()
    {
      return modelViewMatrix;
    }
  }
}
