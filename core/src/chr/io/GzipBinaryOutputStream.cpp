#include "chr/io/GzipBinaryOutputStream.h"
#include "chr/glm.h"

#include <fcntl.h> // FOR MINGW
#include <sstream>

using namespace std;
using namespace google;

namespace chr
{
  namespace io
  {
    GzipBinaryOutputStream::GzipBinaryOutputStream(OutputTarget &outputTarget)
    :
    outputTarget(outputTarget)
    {
      stream = outputTarget.getStream();

      if (stream->good())
      {
        rawOutput = new protobuf::io::OstreamOutputStream(stream.get());
        zipOutput = new protobuf::io::GzipOutputStream(rawOutput);
        codedOutput = new protobuf::io::CodedOutputStream(zipOutput);
      }
    }

    GzipBinaryOutputStream::~GzipBinaryOutputStream()
    {
      close();
    }

    void GzipBinaryOutputStream::close()
    {
      if (rawOutput)
      {
        delete codedOutput;
        codedOutput = nullptr;

        delete zipOutput;
        zipOutput = nullptr;

        delete rawOutput;
        rawOutput = nullptr;

        if (outputTarget.isBuffer())
        {
          outputTarget.setBuffer(*static_cast<ostringstream*>(stream.get())); // XXX
        }

        stream.reset();
      }
    }

    template<>
    void GzipBinaryOutputStream::write(uint32_t value)
    {
      if (good())
      {
        codedOutput->WriteLittleEndian32(value);
      }
    }

    template<>
    void GzipBinaryOutputStream::write(int32_t value)
    {
      if (good())
      {
        codedOutput->WriteLittleEndian32(value);
      }
    }

    template<>
    void GzipBinaryOutputStream::write(uint64_t value)
    {
      if (good())
      {
        codedOutput->WriteLittleEndian64(value);
      }
    }

    template<>
    void GzipBinaryOutputStream::write(int64_t value)
    {
      if (good())
      {
        codedOutput->WriteLittleEndian64(value);
      }
    }

    template<>
    void GzipBinaryOutputStream::write(float value)
    {
      if (good())
      {
        codedOutput->WriteLittleEndian32(encodeFloat(value));
      }
    }

    template<>
    void GzipBinaryOutputStream::write(double value)
    {
      if (good())
      {
        codedOutput->WriteLittleEndian64(encodeDouble(value));
      }
    }

    template<>
    void GzipBinaryOutputStream::write(bool value)
    {
      if (good())
      {
        codedOutput->WriteRaw(&value, 1);
      }
    }

    template<>
    void GzipBinaryOutputStream::write(glm::ivec2 value)
    {
      if (good())
      {
        codedOutput->WriteLittleEndian32(value.x);

        if (good())
        {
          codedOutput->WriteLittleEndian32(value.y);
        }
      }
    }

    template<>
    void GzipBinaryOutputStream::write(glm::vec2 value)
    {
      if (good())
      {
        codedOutput->WriteLittleEndian32(encodeFloat(value.x));

        if (good())
        {
          codedOutput->WriteLittleEndian32(encodeFloat(value.y));
        }
      }
    }

    template<>
    void GzipBinaryOutputStream::write(glm::vec3 value)
    {
      if (good())
      {
        codedOutput->WriteLittleEndian32(encodeFloat(value.x));

        if (good())
        {
          codedOutput->WriteLittleEndian32(encodeFloat(value.y));

          if (good())
          {
            codedOutput->WriteLittleEndian32(encodeFloat(value.z));
          }
        }
      }
    }

    template<>
    void GzipBinaryOutputStream::write(glm::vec4 value)
    {
      if (good())
      {
        codedOutput->WriteLittleEndian32(encodeFloat(value.x));

        if (good())
        {
          codedOutput->WriteLittleEndian32(encodeFloat(value.y));

          if (good())
          {
            codedOutput->WriteLittleEndian32(encodeFloat(value.z));

            if (good())
            {
              codedOutput->WriteLittleEndian32(encodeFloat(value.w));
            }
          }
        }
      }
    }

    void GzipBinaryOutputStream::writeVarint32(uint32_t value)
    {
      if (good())
      {
        codedOutput->WriteVarint32(value);
      }
    }

    void GzipBinaryOutputStream::writeVarint64(uint64_t value)
    {
      if (good())
      {
        codedOutput->WriteVarint64(value);
      }
    }

    void GzipBinaryOutputStream::writeBytes(uint8_t *data, size_t size)
    {
      codedOutput->WriteRaw(data, size);
    }

    void GzipBinaryOutputStream::writeString(const string &value, size_t size)
    {
      if (!value.empty())
      {
        if (good())
        {
          codedOutput->WriteRaw(value.data(), size);
        }
      }
    }

    void GzipBinaryOutputStream::writeString(const string &value)
    {
      if (!value.empty())
      {
        if (good())
        {
          codedOutput->WriteVarint32(value.size());

          if (good())
          {
            codedOutput->WriteRaw(value.data(), value.size());
          }
        }
      }
    }

    bool GzipBinaryOutputStream::good() const
    {
      return codedOutput && !codedOutput->HadError();
    }
  }
}
