#pragma once

#include <memory>

namespace chr
{
  class TimeBase : public std::enable_shared_from_this<TimeBase>
  {
  public:
    virtual ~TimeBase() {}
    virtual double getTime() = 0;

    std::shared_ptr<TimeBase> ref() { return shared_from_this(); }
  };
}
