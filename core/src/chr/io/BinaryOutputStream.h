#pragma once

#include "chr/FileSystem.h"

#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/io/zero_copy_stream_impl.h>

namespace chr
{
  namespace io
  {
    class BinaryOutputStream
    {
    public:
      BinaryOutputStream(OutputTarget &outputTarget);

      ~BinaryOutputStream();
      void close();

      template<typename T>
      void write(T value);

      void writeVarint32(uint32_t value);
      void writeVarint64(uint64_t value);

      void writeBytes(uint8_t *data, size_t size);
      void writeString(const std::string &value, size_t size);
      void writeString(const std::string &value);

      bool good() const;

    protected:
      OutputTarget &outputTarget;
      std::shared_ptr<std::ostream> stream;
      google::protobuf::io::CodedOutputStream *codedOutput = nullptr;
      google::protobuf::io::OstreamOutputStream *rawOutput = nullptr;

      static uint32_t encodeFloat(float value)
      {
        union {float f; uint32_t i;};
        f = value;
        return i;
      }

      static uint64_t encodeDouble(double value)
      {
        union {double f; uint64_t i;};
        f = value;
        return i;
      }
    };
  }
}
