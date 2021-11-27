
#include "Sketch.h"

namespace chr
{
  CrossSketch* createSketch()
  {
    return new Sketch();
  }
}

#if defined(CHR_PLATFORM_IOS)

  #include "ios/Application.h"

  int main(int argc, char** argv)
  {
    @autoreleasepool
    {
      return UIApplicationMain(argc, argv, nil, NSStringFromClass([Application class]));
    }
  }

#endif
