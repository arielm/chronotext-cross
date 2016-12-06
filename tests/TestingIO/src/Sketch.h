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
  std::unique_ptr<uint8_t[]> data;

  void writeFile(const fs::path &filePath);
  void readFile(const fs::path &filePath);

  void populateData();
  bool checkData(uint8_t *d);
};
