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
};
