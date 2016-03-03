#pragma once

#include <memory>

namespace chr
{
  namespace image
  {
    enum Flags
    {
        FLAGS_NONE = 0,
        FLAGS_TRANSLUCENT = 1,
        FLAGS_TRANSLUCENT_INVERSE = 2,
        FLAGS_RBGA = 3,
        FLAGS_POT = 4,
    };

    struct ImageBuffer
    {
        int width = 0;
        int height = 0;
        int components = 0;

        std::shared_ptr<uint8_t> buffer;
    };
  }
}
