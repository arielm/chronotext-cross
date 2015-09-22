#pragma once

#include "Platform.h"

/*
 * API INSPIRED BY https://github.com/cinder/Cinder/blob/master/include/cinder/Timer.h
 */

namespace chr
{
  class Timer
  {
  public:
      Timer(bool startOnConstruction = true);

      void start();
      void stop();

      double getSeconds();
      bool isStopped() const;

  protected:
      double startTime = -1;
      double endTime = -1;
      bool stopped = true;

      double getCurrent();
  };
}
