/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012-2015, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE SIMPLIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#include "utils/Utils.h"

#include <chrono>

using namespace std;

namespace chr
{
  namespace utils
  {
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

    uint64_t millisSinceEpoch()
    {
      using namespace chrono;

      auto now = system_clock::now();
      auto diff = now.time_since_epoch();
      return duration_cast<milliseconds>(diff).count();
    }
  }
}
