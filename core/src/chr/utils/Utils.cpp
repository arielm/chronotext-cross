#include "chr/utils/Utils.h"

#include "chr/ResourceBuffer.h"

#include <chrono> // FOR MINGW

using namespace std;

namespace chr
{
  namespace utils
  {
    template <>
    string readText(const InputSource &inputSource)
    {
      string result;

      if (inputSource.isFile() || (inputSource.isResource() && hasFileResources()))
      {
        fs::ifstream in(inputSource.getFilePath(), ios::in | ios::binary | ios::ate);

        if (in)
        {
          auto fileSize = in.tellg();
          in.seekg(0, ios::beg);

          result.resize(fileSize, 0);
          in.read(&result[0], fileSize);
        }
      }
      else if (inputSource.isResource() && hasMemoryResources())
      {
        auto memoryBuffer = getResourceBuffer(inputSource.getRelativePath());

        if (memoryBuffer)
        {
          result.assign(reinterpret_cast<const char*>(memoryBuffer->data()), memoryBuffer->size());
        }
      }
      else if (inputSource.isBuffer())
      {
        result.assign(reinterpret_cast<const char*>(inputSource.getData()), inputSource.getDataSize());
      }

      return result;
    }

    template <>
    u16string readText(const InputSource &inputSource)
    {
      u16string result;

      if (inputSource.isFile() || (inputSource.isResource() && hasFileResources()))
      {
        fs::ifstream in(inputSource.getFilePath(), ios::in | ios::binary | ios::ate);

        if (in)
        {
          auto fileSize = in.tellg();
          in.seekg(0, ios::beg);

          string text(fileSize, 0);
          in.read(&text[0], fileSize);

          utf8::unchecked::utf8to16(text.data(), text.data() + text.size(), back_inserter(result));
        }
      }
      else if (inputSource.isResource() && hasMemoryResources())
      {
        auto memoryBuffer = getResourceBuffer(inputSource.getRelativePath());

        if (memoryBuffer)
        {
          const char *begin = reinterpret_cast<const char*>(memoryBuffer->data());
          const char *end = begin + memoryBuffer->size();

          utf8::unchecked::utf8to16(begin, end, back_inserter(result));
        }
      }
      else if (inputSource.isBuffer())
      {
        const char *begin = reinterpret_cast<const char*>(inputSource.getData());
        const char *end = begin + inputSource.getDataSize();

        utf8::unchecked::utf8to16(begin, end, back_inserter(result));
      }

      return result;
    }

    // ---

    template <>
    vector<string> readLines(const InputSource &inputSource)
    {
      auto stream = inputSource.getStream();

      string line;
      vector<string> result;

      while (std::getline(*stream, line))
      {
        result.push_back(line);
      }

      return result;
    }

    template <>
    vector<u16string> readLines(const InputSource &inputSource)
    {
      auto stream = inputSource.getStream();

      string line;
      vector<u16string> result;

      while (std::getline(*stream, line))
      {
        result.emplace_back(to<u16string>(line));
      }

      return result;
    }

    // ---

    vector<string> split(const string &str, char separator, bool compress)
    {
      return split(str, string(1, separator ), compress);
    }

    vector<string> split(const string &str, const string &separators, bool compress)
    {
      vector<string> result;
      boost::algorithm::split(result, str, boost::is_any_of(separators), compress ? boost::token_compress_on : boost::token_compress_off);
      return result;
    }

    // ---

    uint64_t millisSinceEpoch()
    {
      using namespace chrono;

      auto now = system_clock::now();
      auto diff = now.time_since_epoch();
      return duration_cast<milliseconds>(diff).count();
    }
  }
}
