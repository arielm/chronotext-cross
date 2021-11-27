#include "Sketch.h"

#include "chr/path/SVGDocument.h"
#include "chr/gl/Triangulator.h"

using namespace std;
using namespace chr;
using namespace gl;
using namespace path;

Sketch::Sketch()
{}

void Sketch::setup()
{
  lysBatch
    .setPrimitive(GL_LINES)
    .setShader(colorShader)
    .setShaderColor(0, 0, 0, 0.75f);

  girlBatch
    .setShader(colorShader)
    .setShaderColor(1, 0.25f, 0.25f, 1);

  createLys();
  createGirl();

  // ---

  glDisable(GL_DEPTH_TEST);
  glDepthMask(GL_FALSE);

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void Sketch::draw()
{
  glClearColor(1, 1, 1, 1);
  glClear(GL_COLOR_BUFFER_BIT);

  // ---

  auto projectionMatrix = glm::ortho(0.0f, windowInfo.width, 0.0f, windowInfo.height);

  State()
    .setShaderMatrix(projectionMatrix)
    .apply();

  lysBatch.flush();
  girlBatch.flush();
}

void Sketch::createLys()
{
  SVGDocument doc;
  doc
    .setOriginAtBottom(true)
    .load(InputSource::resource("lys.svg"));

  for (auto &shape : doc.getShapes())
  {
    for (auto &path : shape.getPaths())
    {
      drawPolyline(lysBatch, path.getPolyline());
    }
  }
}

void Sketch::createGirl()
{
  SVGDocument doc;
  doc
    .setOriginAtBottom(true)
    .setSamplingTolerance(16)
    .load(InputSource::resource("bansky girl heart.svg"));

  Triangulator triangulator;

  for (auto &shape : doc.getShapes())
  {
    triangulator.add(shape);
  }

  triangulator.fill(girlBatch, Matrix().scale(0.33f));
}

void Sketch::drawPolyline(VertexBatch<XYZ> &batch, const vector<glm::vec2> &polyline)
{
  auto size = polyline.size();
  if (size > 1)
  {
    for (auto i = 0; i < size - 1; i++)
    {
      batch.addVertices(polyline[i], polyline[i + 1]);
    }
  }
}
