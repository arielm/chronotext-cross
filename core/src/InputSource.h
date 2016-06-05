#pragma once

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

    bool isResource() const
    {
      return (type == TYPE_RESOURCE);
    }

    bool isFile() const
    {
      return (type == TYPE_FILE);
    }

    fs::path getFilePath() const
    {
      return filePath;
    }

    fs::path getRelativePath() const
    {
      return relativePath;
    }

    static InputSource resource(const fs::path &relativePath)
    {
      InputSource inputSource;
      inputSource.type = TYPE_RESOURCE;
      inputSource.relativePath = relativePath;

      return inputSource;
    }

    static InputSource file(const fs::path &filePath)
    {
      InputSource inputSource;
      inputSource.type = TYPE_FILE;
      inputSource.filePath = filePath;

      return inputSource;
    }

  protected:
    Type type;
    fs::path filePath;
    fs::path relativePath;
  };
}
