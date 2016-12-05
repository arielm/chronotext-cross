#pragma once

#include "chr/FileSystem.h"

#include <google/protobuf/io/coded_stream.h>

namespace chr
{
  namespace io
  {
    class BinaryInputStream
    {
    public:
      BinaryInputStream() = default;
      BinaryInputStream(const InputSource &inputSource);

      ~BinaryInputStream();
      void close();

      template<typename T>
      T read();

      uint32_t readVarint32();
      uint64_t readVarint64();

      void readBytes(uint8_t *data, size_t size);
      std::string readString(size_t size);
      std::string readString();

      void skip(int count);
      bool good() const;

    protected:
      std::shared_ptr<MemoryBuffer> memoryBuffer;
      google::protobuf::io::CodedInputStream *codedInput = nullptr;
      google::protobuf::io::ZeroCopyInputStream *rawInput = nullptr;
      int fd = 0;

      bool failed = false;

      void fail();
    };
  }
}
