#pragma once

#include "chr/time/TimeBase.h"
#include "chr/Timer.h"

namespace chr
{
  class DefaultTimeBase : public TimeBase
  {
  public:
    static std::shared_ptr<DefaultTimeBase> create()
    {
      return std::shared_ptr<DefaultTimeBase>(new DefaultTimeBase());
    }

    double getTime() final
    {
      return timer.getSeconds();
    }
    
  protected:
    Timer timer;

    DefaultTimeBase()
    {
      timer.start();
    }
  };
}
