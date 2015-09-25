/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012-2015, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE SIMPLIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#pragma once

#include <vector>
 
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>

namespace chr
{
  namespace utils
  {
    /*
     * toString() AND split() BORROWED FROM:
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
