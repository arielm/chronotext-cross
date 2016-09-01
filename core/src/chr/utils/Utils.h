#pragma once

#include "chr/FileSystem.h"
#include "chr/Log.h"

#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>

#include "utf8.h"

namespace chr
{
  namespace utils
  {
    template<typename TO, typename FROM>
    TO to(const FROM&);

    template <>
    inline std::string to(const std::u16string &in)
    {
      std::string out;
      utf8::unchecked::utf16to8(in.data(), in.data() + in.size(), back_inserter(out));
      return out;
    }

    template <>
    inline std::u16string to(const std::string &in)
    {
      std::u16string out;
      utf8::unchecked::utf8to16(in.data(), in.data() + in.size(), back_inserter(out));
      return out;
    }

    // ---

    template<typename T>
    T readText(const InputSource &inputSource);

    template<typename T>
    std::vector<T> readLines(const InputSource &inputSource);

    // ---

    inline int fromString(const std::string &in)
    {
      return std::atoi(in.data()); // std::stoi() DOESN'T WORK ON ANDROID
    }

    /*
     * toString() AND split() BASED ON:
     * https://github.com/cinder/Cinder/blob/master/include/cinder/Utilities.h
     */

    template<typename T>
    inline std::string toString(const T &t)
    {
      return boost::lexical_cast<std::string>(t);
    }

    std::vector<std::string> split(const std::string &str, char separator, bool compress = true);
    std::vector<std::string> split(const std::string &str, const std::string &separators, bool compress = true);

    // ---

    uint64_t millisSinceEpoch();

    // ---
    
    template<typename T>
    size_t search(T *array, T value, size_t min, size_t max)
    {
      size_t mid = (min + max) >> 1;
      
      while (min < mid)
      {
        if (array[mid - 1] < value)
        {
          min = mid;
        }
        else if (array[mid - 1] > value)
        {
          max = mid;
        }
        else
        {
          min = max = mid;
        }
        
        mid = (min + max) >> 1;
      }
      
      return mid - 1;
    }
    
    template<typename T>
    inline size_t search(const std::vector<T> &array, T value, size_t min, size_t max)
    {
      return search(const_cast<T*>(array.data()), value, min, max);
    }
  }
}
