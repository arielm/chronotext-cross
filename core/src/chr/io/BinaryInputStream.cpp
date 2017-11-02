#include "chr/io/BinaryInputStream.h"
#include "chr/ResourceBuffer.h"
#include "chr/glm.h"

#include <google/protobuf/io/zero_copy_stream_impl.h>

#include <fcntl.h> // FOR MINGW

using namespace std;
using namespace google;

namespace chr
{
  namespace io
  {
    BinaryInputStream::~BinaryInputStream()
    {
      close();
    }

    void BinaryInputStream::close()
    {
      if (codedInput)
      {
        delete codedInput;
        codedInput = nullptr;
      }

      if (fd > 0)
      {
        delete rawInput;
        rawInput = nullptr;

        ::close(fd);
        fd = 0;
      }
    }

    BinaryInputStream::BinaryInputStream(const InputSource &inputSource)
    {
      if (inputSource.isFile() || (inputSource.isResource() && hasFileResources()))
      {
        fd = open(inputSource.getFilePath().string().data(), O_RDONLY);

        if (fd > 0)
        {
          rawInput = new protobuf::io::FileInputStream(fd);
          codedInput = new protobuf::io::CodedInputStream(rawInput);
        }
      }
      else if (inputSource.isResource() && hasMemoryResources())
      {
        memoryBuffer = chr::getResourceBuffer(inputSource.getRelativePath());

        if (memoryBuffer)
        {
          codedInput = new protobuf::io::CodedInputStream(reinterpret_cast<const uint8_t*>(memoryBuffer->data()), memoryBuffer->size());
        }
      }
      else if (inputSource.isBuffer())
      {
        codedInput = new protobuf::io::CodedInputStream(reinterpret_cast<const uint8_t*>(inputSource.getData()), inputSource.getDataSize());
      }

      #ifdef CHR_PLATFORM_DESKTOP
        if (codedInput)
        {
          /*
           * DEFAULT LIMIT IS 64MB
           *
           * TODO: LIMIT SHOULD BE A PARAMETER INSTEAD OF BEING HARD-CODED
           */
          codedInput->SetTotalBytesLimit(256 * 1024 * 1024, -1);
        }
      #endif
    }

    template<>
    uint32_t BinaryInputStream::read()
    {
      uint32_t result;

      if (good())
      {
        if (!codedInput->ReadLittleEndian32(&result))
        {
          fail();
        }
      }

      return result;
    }

    template<>
    int32_t BinaryInputStream::read()
    {
      int32_t result;

      if (good())
      {
        if (!codedInput->ReadLittleEndian32(reinterpret_cast<uint32_t*>(&result)))
        {
          fail();
        }
      }

      return result;
    }

    template<>
    uint64_t BinaryInputStream::read()
    {
      uint64_t result;

      if (good())
      {
        if (!codedInput->ReadLittleEndian64(&result))
        {
          fail();
        }
      }

      return result;
    }

    template<>
    int64_t BinaryInputStream::read()
    {
      int64_t result;

      if (good())
      {
        if (!codedInput->ReadLittleEndian64(reinterpret_cast<uint64_t*>(&result)))
        {
          fail();
        }
      }

      return result;
    }

    template<>
    float BinaryInputStream::read()
    {
      float result;

      if (good())
      {
        if (!codedInput->ReadLittleEndian32(reinterpret_cast<uint32_t*>(&result)))
        {
          fail();
        }
      }

      return result;
    }

    template<>
    double BinaryInputStream::read()
    {
      double result;

      if (good())
      {
        if (!codedInput->ReadLittleEndian64(reinterpret_cast<uint64_t*>(&result)))
        {
          fail();
        }
      }

      return result;
    }

    template<>
    bool BinaryInputStream::read()
    {
      bool result;

      if (good())
      {
        if (!codedInput->ReadRaw(&result, 1))
        {
          fail();
        }
      }

      return result;
    }

    template<>
    glm::vec2 BinaryInputStream::read()
    {
      glm::vec2 result;

      if (good())
      {
        if (codedInput->ReadLittleEndian32(reinterpret_cast<uint32_t*>(&result.x)))
        {
          if (codedInput->ReadLittleEndian32(reinterpret_cast<uint32_t*>(&result.y)))
          {
            goto exit; // RVO-FRIENDLY
          }
        }

        fail();
      }

      exit:
      return result;
    }

    template<>
    glm::vec3 BinaryInputStream::read()
    {
      glm::vec3 result;

      if (good())
      {
        if (codedInput->ReadLittleEndian32(reinterpret_cast<uint32_t*>(&result.x)))
        {
          if (codedInput->ReadLittleEndian32(reinterpret_cast<uint32_t*>(&result.y)))
          {
            if (codedInput->ReadLittleEndian32(reinterpret_cast<uint32_t*>(&result.z)))
            {
              goto exit; // RVO-FRIENDLY
            }
          }
        }

        fail();
      }

      exit:
      return result;
    }

    uint32_t BinaryInputStream::readVarint32()
    {
      uint32_t result = 0;

      if (good())
      {
        if (!codedInput->ReadVarint32(&result))
        {
          fail();
        }
      }

      return result;
    }

    uint64_t BinaryInputStream::readVarint64()
    {
      uint64_t result = 0;

      if (good())
      {
        if (!codedInput->ReadVarint64(&result))
        {
          fail();
        }
      }

      return result;
    }

    void BinaryInputStream::readBytes(uint8_t *data, size_t size)
    {
      if (good())
      {
        if (!codedInput->ReadRaw(data, size))
        {
          fail();
        }
      }
    }

    string BinaryInputStream::readString(size_t size)
    {
      string result;

      if (good())
      {
        if (!codedInput->ReadString(&result, size))
        {
          fail();
        }
      }

      return result;
    }

    string BinaryInputStream::readString()
    {
      string result;

      if (good())
      {
        uint32_t size;

        if (!codedInput->ReadVarint32(&size))
        {
          fail();
        }
        else
        {
          result.resize(size);

          if (!codedInput->ReadRaw(&result[0], size))
          {
            fail();
          }
        }
      }

      return result;
    }

    void BinaryInputStream::skip(int count)
    {
      if (good())
      {
        if (!codedInput->Skip(count))
        {
          fail();
        }
      }
    }

    bool BinaryInputStream::good() const
    {
      return codedInput && !failed;
    }

    void BinaryInputStream::fail()
    {
      failed = true;
    }
  }
}
