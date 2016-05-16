#pragma once

#include "gl/Batch.h"
#include "gl/Matrix.h"

namespace chr
{
  namespace gl
  {
    namespace draw
    {
      class Cube
      {
      public:
        Cube& setSize(float size);
        Cube& setFrontFace(GLenum mode);

        Cube& setColor(const glm::vec4 &color);
        Cube& setColor(float r, float g, float b, float a);

        template<typename... Args>
        inline Cube& setColors(Args&&... args)
        {
          colors = {{ std::forward<Args>(args)... }};
          hasColors = true;

          return *this;
        }

        template<int V = XYZ, typename I = GLushort>
        void append(IndexedVertexBatch<V,I> &batch, Matrix &matrix) const;

      protected:
        float size = 1;
        GLenum frontFace = GL_CCW;

        glm::vec4 color = glm::vec4(1, 1, 1, 1);
        std::array<glm::vec4, 6> colors;
        bool hasColors;

        const glm::vec4& getFaceColor(int faceIndex) const;
      };
    }
  }
}
