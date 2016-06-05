#pragma once

#include "Platform.h"

#include <memory>

#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>

namespace fs = boost::filesystem;

namespace chr
{
  class InputSource
  {
  public:
    enum Type
    {
      TYPE_UNDEFINED,
      TYPE_RESOURCE,
      TYPE_FILE
    };

    InputSource() = default;

    bool isResource() const;
    bool isFile() const;

    fs::path getFilePath() const;
    fs::path getRelativePath() const;
    const std::string& getUri() const;

    std::shared_ptr<std::istream> getStream() const;

    static InputSource resource(const fs::path &relativePath);
    static InputSource file(const fs::path &filePath);

  protected:
    Type type;
    fs::path filePath;
    fs::path relativePath;
    std::string uri;
  };
}
