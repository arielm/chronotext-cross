#include "chr/io/BinaryOutputStream.h"
#include "chr/glm.h"

#include <fcntl.h> // FOR MINGW

using namespace std;
using namespace google;

namespace chr
{
  namespace io
  {
    BinaryOutputStream::~BinaryOutputStream()
    {
      close();
    }

    void BinaryOutputStream::close()
    {
      if (fd > 0)
      {
        delete codedOutput;
        codedOutput = nullptr;

        delete rawOutput;
        rawOutput = nullptr;

        ::close(fd);
        fd = 0;
      }
    }

    BinaryOutputStream::BinaryOutputStream(const fs::path &filePath)
    {
      int fd = open(filePath.string().data(), O_WRONLY | O_CREAT | O_TRUNC, 0666);

      if (fd > 0)
      {
        rawOutput = new protobuf::io::FileOutputStream(fd);
        codedOutput = new protobuf::io::CodedOutputStream(rawOutput);
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

//    template<>
//    int32_t BinaryOutputStream::read()
//    {
//      int32_t result;
//
//      if (good())
//      {
//        if (!codedInput->ReadLittleEndian32(reinterpret_cast<uint32_t*>(&result)))
//        {
//          fail();
//        }
//      }
//
//      return result;
//    }

    template<>
    void BinaryOutputStream::write(float value)
    {
      if (good())
      {
        codedOutput->WriteLittleEndian32(encodeFloat(value));
      }
    }

//    template<>
//    glm::vec2 BinaryOutputStream::read()
//    {
//      glm::vec2 result;
//
//      if (good())
//      {
//        if (codedInput->ReadLittleEndian32(reinterpret_cast<uint32_t*>(&result.x)))
//        {
//          if (codedInput->ReadLittleEndian32(reinterpret_cast<uint32_t*>(&result.y)))
//          {
//            goto exit; // RVO-FRIENDLY
//          }
//        }
//
//        fail();
//      }
//
//      exit:
//      return result;
//    }

//    template<>
//    glm::vec3 BinaryOutputStream::read()
//    {
//      glm::vec3 result;
//
//      if (good())
//      {
//        if (codedInput->ReadLittleEndian32(reinterpret_cast<uint32_t*>(&result.x)))
//        {
//          if (codedInput->ReadLittleEndian32(reinterpret_cast<uint32_t*>(&result.y)))
//          {
//            if (codedInput->ReadLittleEndian32(reinterpret_cast<uint32_t*>(&result.z)))
//            {
//              goto exit; // RVO-FRIENDLY
//            }
//          }
//        }
//
//        fail();
//      }
//
//      exit:
//      return result;
//    }

    void BinaryOutputStream::writeVarint32(uint32_t value)
    {
      if (good())
      {
        codedOutput->WriteVarint32(value);
      }
    }

//    void BinaryOutputStream::readBytes(uint8_t *data, size_t size)
//    {
//      if (good())
//      {
//        if (!codedInput->ReadRaw(data, size))
//        {
//          fail();
//        }
//      }
//    }

//    string BinaryOutputStream::readString(size_t size)
//    {
//      string result;
//
//      if (good())
//      {
//        if (!codedInput->ReadString(&result, size))
//        {
//          fail();
//        }
//      }
//
//      return result;
//    }

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
