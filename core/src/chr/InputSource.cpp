#include "chr/InputSource.h"
#include "chr/FileSystem.h"

using namespace std;

namespace chr
{
  bool InputSource::isResource() const
  {
    return (type == TYPE_RESOURCE);
  }

  bool InputSource::isFile() const
  {
    return (type == TYPE_FILE);
  }

  fs::path InputSource::getFilePath() const
  {
    switch (type)
    {
      case TYPE_FILE:
        return filePath;

      case TYPE_RESOURCE:
        return getResourceFilePath(relativePath);

      default:
        return "";
    }
  }

  fs::path InputSource::getRelativePath() const
  {
    return relativePath;
  }

  const string& InputSource::getUri() const
  {
    return uri;
  }

  shared_ptr<istream> InputSource::getStream() const
  {
    switch (type)
    {
      case TYPE_FILE:
      {
        fs::ifstream *tmp = new fs::ifstream(filePath, fs::ifstream::binary);
        return shared_ptr<istream>(tmp);
      }

      case TYPE_RESOURCE:
        return getResourceStream(relativePath);

      default:
        return nullptr;
    }
  }

  InputSource InputSource::resource(const fs::path &relativePath)
  {
    InputSource inputSource;

    inputSource.type = TYPE_RESOURCE;
    inputSource.relativePath = relativePath;
    inputSource.uri = "res://" + relativePath.string();

    return inputSource;
  }

  InputSource InputSource::file(const fs::path &filePath)
  {
    InputSource inputSource;

    inputSource.type = TYPE_FILE;
    inputSource.filePath = filePath;
    inputSource.uri = "file://" + filePath.string();

    return inputSource;
  }
}
