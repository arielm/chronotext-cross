#include "chr/gl/SyphonServer.h"

#import <Syphon/Syphon.h>

using namespace std;

namespace chr
{
  namespace gl
  {
    namespace syphon
    {
      void Server::start(const string &name)
      {
        NSString *n = [NSString stringWithCString:name.c_str() encoding:[NSString defaultCStringEncoding]];
        server = [[SyphonServer alloc] initWithName:n context:CGLGetCurrentContext() options:nil];
      }

      void Server::stop()
      {
        if (server)
        {
          [(SyphonServer*)server stop];
          [(SyphonServer*)server release];
          server = nullptr;
        }
      }

      bool Server::bind(const glm::vec2 &size)
      {
        if (server)
        {
          binded = [(SyphonServer*)server bindToDrawFrameOfSize:NSMakeSize(size.x, size.y)];
        }

        return binded;
      }

      void Server::unbind()
      {
        if (binded)
        {
          [(SyphonServer*)server unbindAndPublish];
          binded = false;
        }
      }
    }
  }
}
