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
      if (out)
      {
        delete codedOutput;
        codedOutput = nullptr;

        delete rawOutput;
        rawOutput = nullptr;

        delete out;
        out = nullptr;
      }
    }

    BinaryOutputStream::BinaryOutputStream(const fs::path &filePath)
    {
      out = new fs::ofstream(filePath, ios::out | ios::binary);

      if (out->good())
      {
        rawOutput = new protobuf::io::OstreamOutputStream(out);
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

    void BinaryOutputStream::writeVarint64(uint64_t value)
    {
      if (good())
      {
        codedOutput->WriteVarint64(value);
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
