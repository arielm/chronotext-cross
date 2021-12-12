#include "Frustum.h"

namespace chr
{
  namespace math
  {
    Frustum::Frustum(const glm::mat4 &mat)
    {
      set(mat);
    }

    /*
     * Based on:
     * https://github.com/cinder/Cinder/blob/master/src/cinder/Frustum.cpp
     * http://gamedevs.org/uploads/fast-extraction-viewing-frustum-planes-from-world-view-projection-matrix.pdf
     */
    void Frustum::set(const glm::mat4 &mat)
    {
      planes[PLANE_TOP   ].set(mat[0][3] - mat[0][1], mat[1][3] - mat[1][1], mat[2][3] - mat[2][1], -mat[3][3] + mat[3][1]);
      planes[PLANE_BOTTOM].set(mat[0][3] + mat[0][1], mat[1][3] + mat[1][1], mat[2][3] + mat[2][1], -mat[3][3] - mat[3][1]);
      planes[PLANE_LEFT  ].set(mat[0][3] + mat[0][0], mat[1][3] + mat[1][0], mat[2][3] + mat[2][0], -mat[3][3] - mat[3][0]);
      planes[PLANE_RIGHT ].set(mat[0][3] - mat[0][0], mat[1][3] - mat[1][0], mat[2][3] - mat[2][0], -mat[3][3] + mat[3][0]);
      planes[PLANE_NEAR  ].set(mat[0][3] + mat[0][2], mat[1][3] + mat[1][2], mat[2][3] + mat[2][2], -mat[3][3] - mat[3][2]);
      planes[PLANE_FAR   ].set(mat[0][3] - mat[0][2], mat[1][3] - mat[1][2], mat[2][3] - mat[2][2], -mat[3][3] + mat[3][2]);
    }

    bool Frustum::contains(const glm::vec3 &point) const
    {
      for (int i = 0; i < 6; ++i)
      {
        if (planes[i].signedDistance(point) < 0)
        {
          return false;
        }
      }

      return true;
    }

    /*
     * Based on:
     * https://www.iquilezles.org/www/articles/frustumcorrect/frustumcorrect.htm
     */
    bool Frustum::containsOrIntersectsWith(const Box &box) const
    {
      for (int i = 0; i < 6; i++)
      {
        int out = 0;
        out += (planes[i].signedDistance(glm::vec3(box.min.x, box.min.y, box.min.z)) < 0) ? 1 : 0;
        out += (planes[i].signedDistance(glm::vec3(box.max.x, box.min.y, box.min.z)) < 0) ? 1 : 0;
        out += (planes[i].signedDistance(glm::vec3(box.min.x, box.max.y, box.min.z)) < 0) ? 1 : 0;
        out += (planes[i].signedDistance(glm::vec3(box.max.x, box.max.y, box.min.z)) < 0) ? 1 : 0;
        out += (planes[i].signedDistance(glm::vec3(box.min.x, box.min.y, box.max.z)) < 0) ? 1 : 0;
        out += (planes[i].signedDistance(glm::vec3(box.max.x, box.min.y, box.max.z)) < 0) ? 1 : 0;
        out += (planes[i].signedDistance(glm::vec3(box.min.x, box.max.y, box.max.z)) < 0) ? 1 : 0;
        out += (planes[i].signedDistance(glm::vec3(box.max.x, box.max.y, box.max.z)) < 0) ? 1 : 0;

        if (out == 8)
        {
          return false;
        }
      }

      return true;
    }
  }
}
