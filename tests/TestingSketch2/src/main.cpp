
#include "Sketch.h"

#if defined(CHR_PLATFORM_IOS)
  #include "ios/Application.h"
#endif

namespace chr
{
  CrossSketch* createSketch()
  {
    return new Sketch();
  }
}

int main(int argc, char** argv)
{
  #if defined(CHR_PLATFORM_DESKTOP) || defined(CHR_PLATFORM_EMSCRIPTEN)
    chr::CrossSketch::run(800, 600, 4);
  #elif defined(CHR_PLATFORM_IOS)
    @autoreleasepool
    {
      return UIApplicationMain(argc, argv, nil, NSStringFromClass([Application class]));
    }
  #endif

  return 0;
}
