#include "chr/io/GzipBinaryInputStream.h"
#include "chr/ResourceBuffer.h"
#include "chr/glm.h"

#include <fcntl.h> // FOR MINGW

using namespace std;
using namespace google;

namespace chr
{
  namespace io
  {
    GzipBinaryInputStream::~GzipBinaryInputStream()
    {
      close();
    }

    void GzipBinaryInputStream::close()
    {
      if (codedInput)
      {
        delete codedInput;
        codedInput = nullptr;

        delete zipInput;
        zipInput = nullptr;
      }

      if (arrayInput)
      {
        delete arrayInput;
        arrayInput = nullptr;
      }

      if (fileInput)
      {
        delete fileInput;
        fileInput = nullptr;

        ::close(fd);
        fd = 0;
      }
    }

    GzipBinaryInputStream::GzipBinaryInputStream(const InputSource &inputSource)
    {
      if (inputSource.isFile() || (inputSource.isResource() && hasFileResources()))
      {
        fd = open(inputSource.getFilePath().string().data(), O_RDONLY);

        if (fd > 0)
        {
          fileInput = new protobuf::io::FileInputStream(fd);
          zipInput = new protobuf::io::GzipInputStream(fileInput);
          codedInput = new protobuf::io::CodedInputStream(zipInput);
        }
      }
      else if (inputSource.isResource() && hasMemoryResources())
      {
        memoryBuffer = chr::getResourceBuffer(inputSource.getRelativePath());

        if (memoryBuffer)
        {
          arrayInput = new protobuf::io::ArrayInputStream(memoryBuffer->data(), memoryBuffer->size());
          zipInput = new protobuf::io::GzipInputStream(arrayInput);
          codedInput = new protobuf::io::CodedInputStream(zipInput);
        }
      }
      else if (inputSource.isBuffer())
      {
        arrayInput = new protobuf::io::ArrayInputStream(inputSource.getData(), inputSource.getDataSize());
        zipInput = new protobuf::io::GzipInputStream(arrayInput);
        codedInput = new protobuf::io::CodedInputStream(zipInput);
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
    uint32_t GzipBinaryInputStream::read()
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
    int32_t GzipBinaryInputStream::read()
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
    uint64_t GzipBinaryInputStream::read()
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
    int64_t GzipBinaryInputStream::read()
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
    float GzipBinaryInputStream::read()
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
    double GzipBinaryInputStream::read()
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
    bool GzipBinaryInputStream::read()
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
    glm::ivec2 GzipBinaryInputStream::read()
    {
      glm::ivec2 result;

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
    glm::vec2 GzipBinaryInputStream::read()
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
    glm::vec3 GzipBinaryInputStream::read()
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

    template<>
    glm::vec4 GzipBinaryInputStream::read()
    {
      glm::vec4 result;

      if (good())
      {
        if (codedInput->ReadLittleEndian32(reinterpret_cast<uint32_t*>(&result.x)))
        {
          if (codedInput->ReadLittleEndian32(reinterpret_cast<uint32_t*>(&result.y)))
          {
            if (codedInput->ReadLittleEndian32(reinterpret_cast<uint32_t*>(&result.z)))
            {
              if (codedInput->ReadLittleEndian32(reinterpret_cast<uint32_t*>(&result.w)))
              {
                goto exit; // RVO-FRIENDLY
              }
            }
          }
        }

        fail();
      }

      exit:
      return result;
    }

    uint32_t GzipBinaryInputStream::readVarint32()
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

    uint64_t GzipBinaryInputStream::readVarint64()
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

    void GzipBinaryInputStream::readBytes(uint8_t *data, size_t size)
    {
      if (good())
      {
        if (!codedInput->ReadRaw(data, size))
        {
          fail();
        }
      }
    }

    string GzipBinaryInputStream::readString(size_t size)
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

    string GzipBinaryInputStream::readString()
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

    void GzipBinaryInputStream::skip(int count)
    {
      if (good())
      {
        if (!codedInput->Skip(count))
        {
          fail();
        }
      }
    }

    bool GzipBinaryInputStream::good() const
    {
      return codedInput && !failed;
    }

    void GzipBinaryInputStream::fail()
    {
      failed = true;
    }
  }
}
