#include "gl/QuadBuffer.h"

using namespace std;

namespace chr
{
  namespace gl
  {
    QuadBuffer::QuadBuffer()
    {
      vertices.reserve(4);
    }

    void QuadBuffer::shutdown()
    {
      if (vertexVBOId > 0)
      {
        glDeleteBuffers(1, &vertexVBOId);
      }
    }

    void QuadBuffer::draw(float x1, float y1, float x2, float y2)
    {
      vertices.clear();
      vertices.emplace_back(x1, y1);
      vertices.emplace_back(x2, y1);
      vertices.emplace_back(x1, y2);
      vertices.emplace_back(x2, y2);

      apply();
    }

    void QuadBuffer::apply()
    {
      if (vertexVBOId == 0)
      {
        glGenBuffers(1, &vertexVBOId);

        glBindBuffer(GL_ARRAY_BUFFER, vertexVBOId);
        glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(glm::vec3), nullptr, GL_DYNAMIC_DRAW);
      }

      // ---

      glBindBuffer(GL_ARRAY_BUFFER, vertexVBOId);
      glEnableVertexAttribArray(positionLocation);
      glVertexAttribPointer(positionLocation, 3, GL_FLOAT, GL_FALSE, 0, 0);
      glBufferSubData(GL_ARRAY_BUFFER, 0, 4 * sizeof(glm::vec3), vertices.data());

      glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

      glBindBuffer(GL_ARRAY_BUFFER, 0);
      glDisableVertexAttribArray(positionLocation);
    }
  }
}
