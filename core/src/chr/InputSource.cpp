#include "chr/InputSource.h"
#include "chr/MemoryBuffer.h"
#include "chr/FileSystem.h"
#include "chr/utils/Utils.h"

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

  bool InputSource::isBuffer() const
  {
    return (type == TYPE_BUFFER);
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

  const char* InputSource::getData() const
  {
    return data;
  }

  size_t InputSource::getDataSize() const
  {
    return dataSize;
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
        return make_shared<imemstream>(data, dataSize);

      case TYPE_BUFFER:

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

  InputSource InputSource::buffer(const char *data, size_t size)
  {
    InputSource inputSource;

    inputSource.type = TYPE_BUFFER;
    inputSource.data = data;
    inputSource.dataSize = size;
    inputSource.uri = "mem://" + utils::toString((uint64_t)data);

    return inputSource;
  }
}
