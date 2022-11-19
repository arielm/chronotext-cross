#pragma once

#include "chr/glm.h"

namespace chr
{
  namespace gl
  {
    namespace syphon
    {
      class Server
      {
      public:
        void start(const std::string &name = "");
        void stop();

        bool bind(const glm::vec2 &size);
        void unbind();
        
      protected:
        void *server = nullptr;
        bool binded = false;
      };
    }
  }
}
