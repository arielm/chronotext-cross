#include "chr/gl/Camera.h"
#include "chr/math/Utils.h"

using namespace std;
using namespace chr;
using namespace math;

namespace chr
{
  namespace gl
  {
    Camera& Camera::setFov(float fovY)
    {
      this->fovY = fovY;
      updateRequired = true;

      return *this;
    }

    Camera& Camera::setClip(float nearZ, float farZ)
    {
      this->nearZ = nearZ;
      this->farZ = farZ;
      updateRequired = true;

      return *this;
    }

    Camera& Camera::setWindowSize(const glm::vec2 &size)
    {
      windowSize = size;
      updateRequired = true;

      return *this;
    }

    Camera& Camera::setPan2D(const glm::vec2 &pan)
    {
      pan2D = pan;
      updateRequired = true;

      return *this;
    }

    Camera& Camera::setZoom2D(float zoom)
    {
      zoom2D = zoom;
      updateRequired = true;

      return *this;
    }

    glm::mat4 Camera::getProjectionMatrix()
    {
      update();
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

    glm::vec3 Camera::getEyePosition()
    {
      auto tmp = glm::inverse(getModelViewProjectionMatrix()) * glm::vec4(0, 0, 0, 1);
      return glm::vec3(tmp) / tmp.w;
    }

    /*
     * BASED ON https://github.com/cinder/Cinder/blob/master/src/cinder/Camera.cpp
     */
    Ray Camera::getRay(const glm::vec2 &windowPosition)
    {
      float aspectRatio = windowSize.x / windowSize.y;
      float s = (windowPosition.x / windowSize.x - 0.5f) * aspectRatio;
      float t = (windowSize.y - windowPosition.y) / windowSize.y - 0.5f;
      float viewDistance = aspectRatio / frustumSize.x * nearZ;

      const auto &right = modelViewMatrix.right();
      const auto &up = modelViewMatrix.up();
      const auto &back = modelViewMatrix.back();

      return Ray(getEyePosition(), glm::normalize(right * s + up * t - back * viewDistance));
    }

    Ray Camera::getRay(float u, float v)
    {
      float aspectRatio = windowSize.x / windowSize.y;
      float s = (u - 0.5f) * aspectRatio;
      float t = 1 - v - 0.5f;
      float viewDistance = aspectRatio / frustumSize.x * nearZ;

      const auto &right = modelViewMatrix.right();
      const auto &up = modelViewMatrix.up();
      const auto &back = modelViewMatrix.back();

      return Ray(getEyePosition(), glm::normalize(right * s + up * t - back * viewDistance));
    }

    void Camera::update()
    {
      if (updateRequired)
      {
        updateRequired = false;

        float halfHeight = nearZ * tanf(fovY * PI / 360.0f) / zoom2D;
        float halfWidth = halfHeight * windowSize.x / windowSize.y;

        frustumSize.x = halfWidth * 2;
        frustumSize.y = halfHeight * 2;

        glm::vec2 offset(-pan2D * frustumSize / windowSize);

        projectionMatrix = glm::frustum(-halfWidth + offset.x, halfWidth + offset.x, -halfHeight + offset.y, halfHeight + offset.y, nearZ, farZ);
      }
    }
  }
}
