#pragma once

#include <sstream>
#include <iostream>

namespace chr
{
  static constexpr int _FATAL   = -3;
  static constexpr int _ERROR   = -2;
  static constexpr int _WARNING = -1;
  static constexpr int _INFO    =  0;
  static constexpr int _DEBUG   =  1;
  static constexpr int _VERBOSE =  2;

  class Log
  {
  public:
    Log(int level = _INFO);
    ~Log();

    std::stringstream& stream();

  protected:
    int level;
    std::stringstream ss;
  };
}

#define LOGF chr::Log(chr::_FATAL).stream()
#define LOGE chr::Log(chr::_ERROR).stream()
#define LOGW chr::Log(chr::_WARNING).stream()
#define LOGI chr::Log(chr::_INFO).stream()
#define LOGD chr::Log(chr::_DEBUG).stream()
