#pragma once

#include "chr/gl/Batch.h"
#include "chr/gl/Matrix.h"

namespace chr
{
  namespace gl
  {
    namespace draw
    {
      class Box
      {
      public:
        Box& setSize(float width, float height, float depth);
        inline Box& setSize(float size) { return setSize(size, size, size); }

        Box& setFrontFace(GLenum mode);

        Box& setColor(const glm::vec4 &color);
        Box& setColor(float r, float g, float b, float a);

        template<typename... Args>
        inline Box& setColors(Args&&... args)
        {
          colors = {{ std::forward<Args>(args)... }};
          hasColors = true;

          return *this;
        }

        template<int V = XYZ, typename I = GLuint>
        void append(IndexedVertexBatch<V,I> &batch, const Matrix &matrix) const;

      protected:
        float width = 1, height = 1, depth = 1;
        GLenum frontFace = CCW;

        glm::vec4 color = glm::vec4(1, 1, 1, 1);
        std::array<glm::vec4, 6> colors;
        bool hasColors;

        const glm::vec4& getFaceColor(int faceIndex) const;
      };
    }
  }
}
