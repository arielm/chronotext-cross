#pragma once

#include "chr/cross/Context.h"

class Sketch : public chr::CrossSketch
{
public:
  virtual ~Sketch() {}

  void setup() final;
  void draw() final;

protected:
  bool success = false;

  void writeFile(const fs::path &filePath);
  void readFile(const fs::path &filePath);

  static uint32_t encodeFloat(float value)
  {
    union {float f; uint32_t i;};
    f = value;
    return i;
  }

  static uint64_t encodeDouble(double value)
  {
    union {double f; uint64_t i;};
    f = value;
    return i;
  }
};
