#pragma once

#include "chr/Platform.h"

#include <memory>

#include <boost/filesystem/fstream.hpp>

namespace chr
{
  class InputSource
  {
  public:
    enum Type
    {
      TYPE_UNDEFINED,
      TYPE_RESOURCE,
      TYPE_FILE,
      TYPE_BUFFER
    };

    InputSource() = default;

    bool isResource() const;
    bool isFile() const;
    bool isBuffer() const;

    fs::path getFilePath() const;
    fs::path getRelativePath() const;
    const std::string& getUri() const;
    const void* getData() const;
    size_t getDataSize() const;

    std::shared_ptr<std::istream> getStream() const;

    static InputSource resource(const fs::path &relativePath);
    static InputSource file(const fs::path &filePath);
    static InputSource buffer(const void *data, size_t size);

  protected:
    Type type;
    fs::path filePath;
    fs::path relativePath;
    std::string uri;
    const void* data = nullptr;
    size_t dataSize = 0;
  };
}
