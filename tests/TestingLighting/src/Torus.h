#pragma once

#include "chr/gl/Batch.h"

class Torus
{
public:
  Torus& setFrontFace(GLenum mode);
  Torus& setSliceCount(int count);
  Torus& setLoopCount(int count);
  Torus& setInnerRadius(float radius);
  Torus& setOuterRadius(float radius);

  void append(chr::gl::IndexedVertexBatch<chr::gl::XYZ.N.UV> &batch, const chr::gl::Matrix &matrix) const;

protected:
  GLenum frontFace = chr::CCW;
  int sliceCount = 5;
  int loopCount = 15;
  float innerRadius = 1;
  float outerRadius = 4;
};
