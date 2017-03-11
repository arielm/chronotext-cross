#include "Sketch.h"

#include "nfd/nfd.h"

using namespace std;
using namespace chr;

void Sketch::setup()
{
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

void Sketch::mousePressed(int button, float x, float y)
{
  nfdchar_t *outPath = NULL;
  nfdresult_t result = NFD_OpenDialog("png,jpg;pdf", NULL, &outPath);

  if (result == NFD_OKAY)
  {
    LOGI << outPath << endl;
    free(outPath);
  }
  else if (result == NFD_CANCEL)
  {
    LOGI << "CANCELLED" << endl;
  }
  else
  {
    LOGE << NFD_GetError() << endl;
  }
}
