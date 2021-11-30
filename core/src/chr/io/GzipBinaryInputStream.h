#pragma once

#include "chr/FileSystem.h"

#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/io/gzip_stream.h>
#include <google/protobuf/io/zero_copy_stream_impl.h>

namespace chr
{
  namespace io
  {
    class GzipBinaryInputStream
    {
    public:
      GzipBinaryInputStream() = default;
      GzipBinaryInputStream(const InputSource &inputSource);

      ~GzipBinaryInputStream();
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
      std::shared_ptr<ResourceBuffer> memoryBuffer;
      google::protobuf::io::ArrayInputStream *arrayInput = nullptr;
      google::protobuf::io::CodedInputStream *codedInput = nullptr;
      google::protobuf::io::GzipInputStream *zipInput = nullptr;
      google::protobuf::io::ZeroCopyInputStream *fileInput = nullptr;
      int fd = 0;

      bool failed = false;

      void fail();
    };
  }
}
