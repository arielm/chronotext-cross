#include "Sketch.h"

using namespace std;
using namespace chr;
using namespace gl;

void Sketch::setup()
{
  LOGI << systemManager().getDocumentsFolder() << endl;

  // ---
  glDisable(GL_DEPTH_TEST);
  glDepthMask(GL_FALSE);

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void Sketch::draw()
{
  glClearColor(0.5f, 0.5f, 0.5f, 1);
  glClear(GL_COLOR_BUFFER_BIT);
}
