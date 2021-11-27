#pragma once

#include "chr/cross/Context.h"

class Sketch : public chr::CrossSketch
{
public:
  virtual ~Sketch() {}

  void setup() final;
  void draw() final;

protected:
  void mousePressed(int button, float x, float y) final;
};
