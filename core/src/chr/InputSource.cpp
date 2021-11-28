#include "chr/InputSource.h"
#include "chr/ResourceBuffer.h"
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

  string InputSource::getExtension() const
  {
    switch (type)
    {
      case TYPE_FILE:
        return filePath.extension().string();

      case TYPE_RESOURCE:
        return relativePath.extension().string();

      default:
        return "";
    }
  }

  const string& InputSource::getUri() const
  {
    return _uri;
  }

  const void* InputSource::getData() const
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
        auto *tmp = new fs::ifstream(filePath, fs::ifstream::binary);
        return shared_ptr<istream>(tmp);
      }

      case TYPE_RESOURCE:
        return getResourceStream(relativePath);

      case TYPE_BUFFER:
        return make_shared<imemstream>(data, dataSize);

      default:
        return nullptr;
    }
  }

  InputSource InputSource::resource(const fs::path &relativePath)
  {
    InputSource inputSource;

    inputSource.type = TYPE_RESOURCE;
    inputSource.relativePath = relativePath;
    inputSource._uri = "res://" + relativePath.string();

    return inputSource;
  }

  InputSource InputSource::file(const fs::path &filePath)
  {
    InputSource inputSource;

    inputSource.type = TYPE_FILE;
    inputSource.filePath = filePath;
    inputSource._uri = "file://" + filePath.string();

    return inputSource;
  }

  InputSource InputSource::buffer(const void *data, size_t size)
  {
    InputSource inputSource;

    inputSource.type = TYPE_BUFFER;
    inputSource.data = data;
    inputSource.dataSize = size;
    inputSource._uri = "mem://" + utils::toString((uint64_t)data); // XXX

    return inputSource;
  }

  InputSource InputSource::uri(const std::string uri)
  {
    auto found = uri.find("://");

    if (found != string::npos)
    {
      auto path = uri.substr(found + 3);

      if (!path.empty())
      {
        auto scheme = uri.substr(0, found);

        if (scheme == "res")
        {
          return InputSource::resource(path);
        }
        else if (scheme == "file")
        {
          return InputSource::file(path);
        }
      }
    }

    return InputSource();
  }
}
