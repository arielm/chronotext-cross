#include "gl/QuadBuffer.h"
#include "gl/ColorShader.h"

using namespace std;

namespace chr
{
  namespace gl
  {
    void QuadBuffer::setup()
    {
      vertices.resize(4);

      const GLushort indices[] =
      {
        0, 1, 2,
        2, 3, 0
      };

      glGenBuffers(2, vboIds);

      glBindBuffer(GL_ARRAY_BUFFER, vboIds[0]);
      glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 4 * 3, nullptr, GL_DYNAMIC_DRAW);

      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboIds[1]);
      glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLushort) * 6, indices, GL_STATIC_DRAW);
    }

    void QuadBuffer::shutdown()
    {
      glDeleteBuffers(2, vboIds);
    }

    void QuadBuffer::draw(float x1, float y1, float x2, float y2)
    {
      vertices[0] = { x1, y1, 0 };
      vertices[1] = { x1, y2, 0 };
      vertices[2] = { x2, y2, 0 };
      vertices[3] = { x2, y1, 0 };

      // ---

      glBindBuffer(GL_ARRAY_BUFFER, vboIds[0]);
      glEnableVertexAttribArray(colorShader.positionLocation);
      glVertexAttribPointer(colorShader.positionLocation, 3, GL_FLOAT, GL_FALSE, 0, 0);
      glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(GLfloat) * 4 * 3, vertices.data());

      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboIds[1]);
      glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, 0);

      // ---

      glDisableVertexAttribArray(colorShader.positionLocation);

      glBindBuffer(GL_ARRAY_BUFFER, 0);
      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }

    void QuadBuffer::setMatrix(const glm::mat4 &matrix)
    {
      glUniformMatrix4fv(colorShader.matrixLocation, 1, GL_FALSE, &matrix[0][0]);
    }

    void QuadBuffer::setColor(float r, float g, float b, float a)
    {
      glVertexAttrib4fv(colorShader.colorLocation, &glm::vec4(r, g, b, a)[0]);
    }

    void QuadBuffer::setColor(const glm::vec4 &color)
    {
      glVertexAttrib4fv(colorShader.colorLocation, &color[0]);
    }
  }
}
