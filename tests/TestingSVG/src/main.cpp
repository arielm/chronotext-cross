
#include "Sketch.h"

namespace chr
{
  CrossSketch* createSketch()
  {
    return new Sketch();
  }
}

#if defined(CHR_PLATFORM_DESKTOP)

  int main(int argc, char** argv)
  {
    chr::CrossSketch::run(1024, 768, 4);
    return 0;
  }

#elif defined(CHR_PLATFORM_EMSCRIPTEN)

  int main(int argc, char** argv)
  {
    chr::CrossSketch::run(4);
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
