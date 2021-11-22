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
  fillBatch
    .setShader(colorShader);

  strokeBatch
    .setPrimitive(GL_LINES)
    .setShader(colorShader)
    .setShaderColor(0, 0, 0, 0.75f);

  // ---

  SVGDocument doc1;
  doc1
    .setOriginAtBottom(false)
    .setSamplingTolerance(16)
    .load(InputSource::resource("256.svg"));

  SVGDocument doc2;
  doc2.load(InputSource::resource("lys.svg"));

  // ---

  for (auto &shape : doc2.getShapes())
  {
    for (auto &path : shape.getPaths())
    {
      drawPolyline(path.getPolyline());
    }
  }

  // ---

  Triangulator triangulator;

  for (auto &shape : doc1.getShapes())
  {
    if (shape.getId() != "rect")
    {
      triangulator.add(shape);
    }
  }

  triangulator
    .setColor(1, 0.25f, 0.25f, 1)
    .fill(fillBatch);

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

  Matrix modelViewMatrix;
  modelViewMatrix
    .translate(0, windowInfo.height)
    .scale(1, -1);

  State()
    .setShaderMatrix(modelViewMatrix * projectionMatrix)
    .apply();

  fillBatch.flush();
  strokeBatch.flush();
}

void Sketch::drawPolyline(const vector<glm::vec2> &polyline)
{
  auto size = polyline.size();
  if (size > 1)
  {
    for (auto i = 0; i < size - 1; i++)
    {
      strokeBatch.addVertices(polyline[i], polyline[i + 1]);
    }
  }
}
