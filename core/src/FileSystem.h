#pragma once

#include "InputSource.h"

#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>

namespace fs = boost::filesystem;

namespace chr
{
  class MemoryBuffer;

  bool hasFileResources();
  bool hasMemoryResources();

  fs::path getResourceFilePath(const fs::path &relativePath);
  std::shared_ptr<MemoryBuffer> getResourceBuffer(const fs::path &relativePath);
  std::shared_ptr<std::istream> getResourceStream(const fs::path &relativePath);
}
