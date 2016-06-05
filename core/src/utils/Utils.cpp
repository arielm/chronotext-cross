#include "utils/Utils.h"

#include "MemoryBuffer.h"

#include <chrono> // FOR MINGW

using namespace std;

namespace chr
{
  namespace utils
  {
    template <>
    string readText(const fs::path &resourcePath)
    {
      string result;

      if (chr::hasMemoryResources())
      {
        auto memoryBuffer = getResourceBuffer(resourcePath);

        if (memoryBuffer)
        {
          result.assign(reinterpret_cast<const char*>(memoryBuffer->data()), memoryBuffer->size());
        }
      }
      else if (chr::hasFileResources())
      {
        auto resPath = getResourcePath(resourcePath);
        fs::ifstream in(resPath, ios::in | ios::binary | ios::ate);

        if (in)
        {
          auto fileSize = in.tellg();
          in.seekg(0, ios::beg);

          result.resize(fileSize, 0);
          in.read(&result[0], fileSize);
        }
      }

      return result;
    }

    template <>
    u16string readText(const fs::path &resourcePath)
    {
      u16string result;

      if (chr::hasMemoryResources())
      {
        auto memoryBuffer = getResourceBuffer(resourcePath);

        if (memoryBuffer)
        {
          const char *begin = reinterpret_cast<const char*>(memoryBuffer->data());
          const char *end = begin + memoryBuffer->size();

          utf8::unchecked::utf8to16(begin, end, back_inserter(result));
        }
      }
      else if (chr::hasFileResources())
      {
        auto resPath = getResourcePath(resourcePath);
        fs::ifstream in(resPath, ios::in | ios::binary | ios::ate);

        if (in)
        {
          auto fileSize = in.tellg();
          in.seekg(0, ios::beg);

          string text(fileSize, 0);
          in.read(&text[0], fileSize);

          utf8::unchecked::utf8to16(text.data(), text.data() + text.size(), back_inserter(result));
        }
      }

      return result;
    }

    // ---

    template <>
    vector<string> readLines(const fs::path &resourcePath)
    {
      auto stream = getResourceStream(resourcePath);

      string line;
      vector<string> result;

      while (std::getline(*stream, line))
      {
        result.push_back(line);
      }

      return result;
    }

    template <>
    vector<u16string> readLines(const fs::path &resourcePath)
    {
      auto stream = getResourceStream(resourcePath);

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
