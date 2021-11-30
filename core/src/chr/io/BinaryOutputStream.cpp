#include "chr/io/BinaryOutputStream.h"
#include "chr/glm.h"

#include <fcntl.h> // FOR MINGW
#include <sstream>

using namespace std;
using namespace google;

namespace chr
{
  namespace io
  {
    BinaryOutputStream::BinaryOutputStream(OutputTarget &outputTarget)
    :
    outputTarget(outputTarget)
    {
      stream = outputTarget.getStream();

      if (stream->good())
      {
        rawOutput = new protobuf::io::OstreamOutputStream(stream.get());
        codedOutput = new protobuf::io::CodedOutputStream(rawOutput);
      }
    }

    BinaryOutputStream::~BinaryOutputStream()
    {
      close();
    }

    void BinaryOutputStream::close()
    {
      if (rawOutput)
      {
        delete codedOutput;
        codedOutput = nullptr;

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
    void BinaryOutputStream::write(uint32_t value)
    {
      if (good())
      {
        codedOutput->WriteLittleEndian32(value);
      }
    }

    template<>
    void BinaryOutputStream::write(int32_t value)
    {
      if (good())
      {
        codedOutput->WriteLittleEndian32(value);
      }
    }

    template<>
    void BinaryOutputStream::write(uint64_t value)
    {
      if (good())
      {
        codedOutput->WriteLittleEndian64(value);
      }
    }

    template<>
    void BinaryOutputStream::write(int64_t value)
    {
      if (good())
      {
        codedOutput->WriteLittleEndian64(value);
      }
    }

    template<>
    void BinaryOutputStream::write(float value)
    {
      if (good())
      {
        codedOutput->WriteLittleEndian32(encodeFloat(value));
      }
    }

    template<>
    void BinaryOutputStream::write(double value)
    {
      if (good())
      {
        codedOutput->WriteLittleEndian64(encodeDouble(value));
      }
    }

    template<>
    void BinaryOutputStream::write(bool value)
    {
      if (good())
      {
        codedOutput->WriteRaw(&value, 1);
      }
    }

    template<>
    void BinaryOutputStream::write(glm::ivec2 value)
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
    void BinaryOutputStream::write(glm::vec2 value)
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
    void BinaryOutputStream::write(glm::vec3 value)
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
    void BinaryOutputStream::write(glm::vec4 value)
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

    void BinaryOutputStream::writeVarint32(uint32_t value)
    {
      if (good())
      {
        codedOutput->WriteVarint32(value);
      }
    }

    void BinaryOutputStream::writeVarint64(uint64_t value)
    {
      if (good())
      {
        codedOutput->WriteVarint64(value);
      }
    }

    void BinaryOutputStream::writeBytes(uint8_t *data, size_t size)
    {
      codedOutput->WriteRaw(data, size);
    }

    void BinaryOutputStream::writeString(const string &value, size_t size)
    {
      if (!value.empty())
      {
        if (good())
        {
          codedOutput->WriteRaw(value.data(), size);
        }
      }
    }

    void BinaryOutputStream::writeString(const string &value)
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

    bool BinaryOutputStream::good() const
    {
      return codedOutput && !codedOutput->HadError();
    }
  }
}
