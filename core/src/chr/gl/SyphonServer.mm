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
          this->size = size;
          bound = [(SyphonServer*)server bindToDrawFrameOfSize:NSMakeSize(size.x, size.y)];
        }

        return bound;
      }

      void Server::unbind()
      {
        if (bound)
        {
          [(SyphonServer*)server unbindAndPublish];
          bound = false;
        }
      }

      Texture Server::getTexture()
      {
        Texture texture;

        if (server)
        {
          SyphonImage *image = [(SyphonServer*)server newFrameImage];
          texture = Texture(image.textureName, image.textureSize.width, image.textureSize.height, GL_RGBA, GL_TEXTURE_RECTANGLE_ARB);
          [image release];
        }

        return texture;
      }

      void Server::draw()
      {
        if (server && !bound)
        {
          glDisable(GL_DEPTH_TEST);
          glDepthMask(GL_FALSE);

          batch
            .setShader(shader)
            .setShaderColor(1, 1, 1, 1)
            .setTexture(getTexture());

          State()
            .setShaderMatrix(glm::ortho(0.0f, size.x, 0.0f, size.y))
            .apply();

          batch.clear();

          batch
            .addVertex(0, 0, 0, 0, 0)
            .addVertex(0, size.y, 0, 0, size.y)
            .addVertex(size.x, size.y, 0, size.x, size.y)
            .addVertex(size.x, 0, 0, size.x, 0);

          batch.addIndices(0, 1, 2, 2, 3, 0);

          batch.flush();
        }
      }
    }
  }
}
