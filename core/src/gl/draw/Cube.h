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
        inline Cube& setSize(float size) { return setSize(glm::vec3(size)); }
        Cube& setSize(const glm::vec3 &size);

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
        glm::vec4 color = glm::vec4(1, 1, 1, 1);
        std::array<glm::vec4, 6> colors;
        bool hasColors;

        glm::vec3 size = glm::vec3(1);

        const glm::vec4& getFaceColor(int faceIndex) const;
      };
    }
  }
}
