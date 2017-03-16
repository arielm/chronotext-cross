#include "OutputTarget.h"

#include <sstream>

using namespace std;

namespace chr
{
  bool OutputTarget::isFile() const
  {
    return (type == TYPE_FILE);
  }

  bool OutputTarget::isBuffer() const
  {
    return (type == TYPE_BUFFER);
  }

  fs::path OutputTarget::getFilePath() const
  {
    switch (type)
    {
      case TYPE_FILE:
        return filePath;

      default:
        return "";
    }
  }

  const void *OutputTarget::getData() const
  {
    return storage.data();
  }

  size_t OutputTarget::getDataSize() const
  {
    return storage.size();
  }

  shared_ptr<ostream> OutputTarget::getStream() const
  {
    switch (type)
    {
      case TYPE_FILE:
      {
        auto *tmp = new fs::ofstream(filePath, ios::binary);
        return shared_ptr<ostream>(tmp);
      }

      case TYPE_BUFFER:
      {
        auto *tmp = new ostringstream(ios::out | ios::binary);
        return shared_ptr<ostream>(tmp);
      }

      default:
        return nullptr;
    }
  }

  void OutputTarget::setBuffer(ostringstream &stream)
  {
    storage = stream.str();
  }

  OutputTarget OutputTarget::file(const fs::path &filePath)
  {
    OutputTarget outputTarget;

    outputTarget.type = TYPE_FILE;
    outputTarget.filePath = filePath;

    return outputTarget;
  }

  OutputTarget OutputTarget::buffer()
  {
    OutputTarget outputTarget;

    outputTarget.type = TYPE_BUFFER;

    return outputTarget;
  }
}
