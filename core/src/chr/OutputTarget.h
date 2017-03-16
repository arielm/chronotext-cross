#pragma once

#include "chr/Platform.h"

#include <memory>

#include <boost/filesystem/fstream.hpp>

namespace chr
{
  class OutputTarget
  {
  public:
    enum Type
    {
      TYPE_UNDEFINED,
      TYPE_FILE,
      TYPE_BUFFER
    };

    OutputTarget() = default;

    bool isFile() const;
    bool isBuffer() const;

    fs::path getFilePath() const;
    const void *getData() const;
    size_t getDataSize() const;

    std::shared_ptr<std::ostream> getStream() const;
    void setBuffer(std::ostringstream &stream);

    static OutputTarget file(const fs::path &filePath);
    static OutputTarget buffer();

  protected:
    Type type = TYPE_UNDEFINED;
    fs::path filePath;
    std::string storage;
  };
}
