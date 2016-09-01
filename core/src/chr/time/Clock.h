/*
 * INSPIRED BY javax.media.Clock
 * http://docs.oracle.com/javame/config/cdc/opt-pkgs/api/jsr927/javax/media/Clock.html
 */

/*
 * PROBLEM: CREATING A Clock FROM A TimeBase WHICH IS NOT ENCLOSED IN A shared_ptr WOULD CAUSE A CRASH
 *
 * SOLUTION: ENFORCING CREATION AS shared_ptr
 *
 * REFERENCE: http://mortoray.com/2013/08/02/safely-using-enable_shared_from_this
 */

#pragma once

#include "chr/time/DefaultTimeBase.h"

namespace chr
{
  class Clock : public TimeBase
  {
  public:
    typedef std::shared_ptr<Clock> Ref;

    template<typename... T>
    static Ref create(T&&... args)
    {
      return Ref(new Clock(std::forward<T>(args)...));
    }

    enum State
    {
      STOPPED,
      STARTED
    };

    virtual void start();
    virtual void stop();
    
    virtual double getTime() override;
    virtual void setTime(double now);
    
    virtual double getRate();
    virtual void setRate(double factor);
    
    virtual State getState();
    
    virtual void restart();
    virtual void reset();
    
  protected:
    double mst;
    double rate;
    double tbst;
    State state;
    
    std::shared_ptr<TimeBase> timeBase;
    
    Clock();
    Clock(std::shared_ptr<TimeBase> timeBase, bool startOnConstruction = true);
  };
}
