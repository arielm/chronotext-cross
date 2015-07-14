#pragma once

#include <sstream>
#include <iostream>

namespace chr
{
  static constexpr int FATAL   = -3;
  static constexpr int ERROR   = -2;
  static constexpr int WARNING = -1;
  static constexpr int INFO    =  0;
  static constexpr int DEBUG   =  1;
  static constexpr int VERBOSE =  2;

  class Log
  {
  public:
    Log(int level = INFO);
    ~Log();

    std::stringstream& stream();

  protected:
    int level;
    std::stringstream ss;
  };
}

#define LOGF chr::Log(chr::FATAL).stream()
#define LOGE chr::Log(chr::ERROR).stream()
#define LOGW chr::Log(chr::WARNING).stream()
#define LOGI chr::Log(chr::INFO).stream()
#define LOGD chr::Log(chr::DEBUG).stream()
