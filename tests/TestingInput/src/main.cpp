
#include "Sketch.h"

namespace chr
{
  CrossSketch* createSketch()
  {
    return new Sketch();
  }
}

#if defined(CHR_PLATFORM_DESKTOP) || defined(CHR_PLATFORM_EMSCRIPTEN)

  int main(int argc, char** argv)
  {
    chr::CrossSketch::run(640, 480, 4);
    return 0;
  }

#elif defined(CHR_PLATFORM_IOS)

  #include "ios/Application.h"

  int main(int argc, char** argv)
  {
    @autoreleasepool
    {
      return UIApplicationMain(argc, argv, nil, NSStringFromClass([Application class]));
    }
  }

#endif
