#pragma once

#include "chr/InputSource.h"
#include "chr/OutputTarget.h"

#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>

namespace fs = boost::filesystem;

namespace chr
{
  class ResourceBuffer;

  bool hasFileResources();
  bool hasMemoryResources();

  fs::path getResourceFilePath(const fs::path &relativePath);
  std::shared_ptr<ResourceBuffer> getResourceBuffer(const fs::path &relativePath);
  std::shared_ptr<std::istream> getResourceStream(const fs::path &relativePath);

  fs::path getDocumentsFolder();

  #if defined(CHR_PLATFORM_IOS)
    fs::path getLibraryFolder();
  #elif defined(CHR_PLATFORM_ANDROID)
    fs::path getInternalDataPath();
    fs::path getExternalDataPath();
  #elif defined(CHR_PLATFORM_RPI)
    fs::path getHomeFolder();
  #endif

  fs::path getFolderOrCreateIt(const fs::path &folderPath);
}
