#include "gl/TextureBuffer.h"

using namespace std;

namespace chr
{
  namespace gl
  {
    void TextureBuffer::shutdown()
    {
      if (vertexVBOId > 0)
      {
        glDeleteBuffers(1, &vertexVBOId);
      }

      if (coordVBOId > 0)
      {
        glDeleteBuffers(1, &coordVBOId);
      }
    }

    void TextureBuffer::draw(const TextureHandle &texture, float x, float y, float scale)
    {
      float x1 = x;
      float y1 = y;
      float x2 = x + texture.width * texture.maxU * scale;
      float y2 = y + texture.height * texture.maxV * scale;

      vertices[0] = { x1, y1, 0 };
      vertices[1] = { x1, y2, 0 };
      vertices[2] = { x2, y1, 0 };
      vertices[3] = { x2, y2, 0 };

      // ---

      float u1 = 0;
      float v1 = 0;
      float u2 = texture.maxU;
      float v2 = texture.maxV;

      coords[0] = { u1, v1 };
      coords[1] = { u1, v2 };
      coords[2] = { u2, v1 };
      coords[3] = { u2, v2 };

      apply(texture);
    }

    void TextureBuffer::drawFromCenter(const TextureHandle &texture, float x, float y, float scale)
    {
      float halfWidth = texture.width * texture.maxU * scale * 0.5f;
      float halfHeight = texture.height * texture.maxV * scale * 0.5f;

      float x1 = x - halfWidth;
      float y1 = y - halfHeight;
      float x2 = x + halfWidth;
      float y2 = y + halfHeight;

      vertices[0] = { x1, y1, 0 };
      vertices[1] = { x1, y2, 0 };
      vertices[2] = { x2, y1, 0 };
      vertices[3] = { x2, y2, 0 };

      // ---

      float u1 = 0;
      float v1 = 0;
      float u2 = texture.maxU;
      float v2 = texture.maxV;

      coords[0] = { u1, v1 };
      coords[1] = { u1, v2 };
      coords[2] = { u2, v1 };
      coords[3] = { u2, v2 };

      apply(texture);
    }

    void TextureBuffer::drawInRect(const TextureHandle &texture, const math::Rectf &rect, float ox, float oy)
    {
      float x1 = rect.x1;
      float y1 = rect.y1;
      float x2 = rect.x2;
      float y2 = rect.y2;

      vertices[0] = { x1, y1, 0 };
      vertices[1] = { x1, y2, 0 };
      vertices[2] = { x2, y1, 0 };
      vertices[3] = { x2, y2, 0 };

      // ---

      float width = texture.width * texture.maxU;
      float height = texture.height * texture.maxV;

      float u1 = (rect.x1 - ox) / width;
      float v1 = (rect.y1 - oy) / height;
      float u2 = (rect.x2 - ox) / width;
      float v2 = (rect.y2 - oy) / height;

      coords[0] = { u1, v1 };
      coords[1] = { u1, v2 };
      coords[2] = { u2, v1 };
      coords[3] = { u2, v2 };

      apply(texture);
    }

    void TextureBuffer::apply(const TextureHandle &texture)
    {
      if (vertexVBOId == 0)
      {
        glGenBuffers(1, &vertexVBOId);

        glBindBuffer(GL_ARRAY_BUFFER, vertexVBOId);
        glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(glm::vec3), nullptr, GL_DYNAMIC_DRAW);
      }

      if (coordVBOId == 0)
      {
        glGenBuffers(1, &coordVBOId);

        glBindBuffer(GL_ARRAY_BUFFER, coordVBOId);
        glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(glm::vec2), nullptr, GL_DYNAMIC_DRAW);
      }

      // ---

      glEnable(GL_TEXTURE_2D);
      glBindTexture(GL_TEXTURE_2D, texture.id);

      glBindBuffer(GL_ARRAY_BUFFER, vertexVBOId);
      glEnableVertexAttribArray(positionLocation);
      glVertexAttribPointer(positionLocation, 3, GL_FLOAT, GL_FALSE, 0, 0);
      glBufferSubData(GL_ARRAY_BUFFER, 0, 4 * sizeof(glm::vec3), &vertices);

      glBindBuffer(GL_ARRAY_BUFFER, coordVBOId);
      glEnableVertexAttribArray(coordLocation);
      glVertexAttribPointer(coordLocation, 2, GL_FLOAT, GL_FALSE, 0, 0);
      glBufferSubData(GL_ARRAY_BUFFER, 0, 4 * sizeof(glm::vec2), &coords);

      glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

      glDisable(GL_TEXTURE_2D);
      glBindTexture(GL_TEXTURE_2D, 0);

      glBindBuffer(GL_ARRAY_BUFFER, 0);
      glDisableVertexAttribArray(positionLocation);
      glDisableVertexAttribArray(coordLocation);
    }

    void TextureBuffer::setMatrix(const glm::mat4 &matrix)
    {
      glUniformMatrix4fv(matrixLocation, 1, GL_FALSE, &matrix[0][0]);
    }

    void TextureBuffer::setColor(float r, float g, float b, float a)
    {
      glVertexAttrib4fv(colorLocation, &glm::vec4(r, g, b, a)[0]);
    }

    void TextureBuffer::setColor(const glm::vec4 &color)
    {
      glVertexAttrib4fv(colorLocation, &color[0]);
    }
  }
}
