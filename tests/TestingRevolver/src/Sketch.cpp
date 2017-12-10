#include "Sketch.h"

#include "chr/math/MatrixAffine.h"
#include "chr/gl/draw/Sprite.h"
#include "chr/gl/Revolver.h"
#include "chr/path/SVGDocument.h"

using namespace std;
using namespace chr;
using namespace gl;
using namespace path;
using namespace math;

static constexpr float DOT_RADIUS_PIXELS = 56; // SPECIFIC TO "dot_112.png"

Sketch::Sketch()
:
lineBatch(GL_LINES),
contourBatch(GL_LINES)
{}

void Sketch::setup()
{
  initTextures();

  // ---

  state2D
    .glDisable(GL_CULL_FACE)
    .glDisable(GL_DEPTH_TEST)
    .glDepthMask(GL_FALSE);

  state3D
    .glEnable(GL_CULL_FACE)
    .glEnable(GL_DEPTH_TEST)
    .glDepthMask(GL_TRUE);

  contourBatch
    .setShader(colorShader)
    .setShaderColor(1, 1, 0.5f, 1);

  flatBatch
    .setShader(colorShader)
    .setShaderColor(0.25f, 0.75f, 0, 1);

  lightenBatch
    .setTexture(lysTexture);

  lineBatch
    .setShader(colorShader)
    .setShaderColor(1, 1, 1, 1);

  dotBatch
    .setShader(textureAlphaShader)
    .setShaderColor(1, 0, 0, 0.75f)
    .setTexture(dotTexture);

  // ---

  SVGDocument doc;
  doc
    .setOriginAtBottom(false)
    .setSamplingTolerance(16)
    .load(InputSource::resource("bullet.svg"));

  auto &path = doc.getShapes().front().getPaths().front();
  auto &contour1 = path.getPolyline();

  //

  float scale = windowInfo.height / doc.getSize().y;
  auto scaledContour1 = MatrixAffine().scale(scale).transformPoints(contour1);

  drawPolyline(scaledContour1);

  for (const auto &point : scaledContour1)
  {
    drawDot(point, 2);
  }

  //

  Revolver revolver1;

  Matrix matrix1;
  matrix1.translate(0, -200);

  revolver1
    .setCapBack(true)
    .setContourCapture(Revolver::CAPTURE_ALL)
    .process(flatBatch, matrix1, contour1);

  revolver1.exportContours(contourBatch);

  // ---

  auto contour2 = Path()
    .moveTo(10, 0)
    .lineTo(20, 20)
    .lineTo(15, 30)
    .lineTo(15, 144)
    .lineTo(50, 240)
    .lineTo(33, 267)
    .getPolyline();

  //

  Revolver revolver2;

  Matrix matrix2;
  matrix2.translate(0, -100, 100);

  revolver2
    .setCapFront(true)
    .setCapBack(true)
    .setContourCapture(Revolver::CAPTURE_ROWS)
    .setTextureScale(0.125f)
    .setColor(1.0f, 0.0f, 0.0f, 1.0f)
    .process(lightenBatch, matrix2, contour2);

  revolver2.exportContours(contourBatch);

  // ---

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void Sketch::draw()
{
  glClearColor(0.5f, 0.5f, 0.5f, 1);
  glDepthMask(GL_TRUE);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // ---

  auto projectionMatrix3D = glm::perspective(60 * D2R, windowInfo.width / windowInfo.height, 0.1f, 1000.0f);

  Matrix mvMatrix3D;
  mvMatrix3D
    .scale(1, -1, 1)
    .translate(0, 0, -400)
    .rotateY(clock()->getTime() * 0.5f)
    .rotateX(60 * D2R);

  state3D
    .setShaderMatrix<MVP>(mvMatrix3D * projectionMatrix3D)
    .setShaderMatrix<NORMAL>(mvMatrix3D.getNormalMatrix())
    .apply();

  glEnable(GL_POLYGON_OFFSET_FILL);
  glPolygonOffset(2, 1);

  lightenBatch
    .setShader(lambertShader)
    .flush();

  flatBatch.flush();

  glDepthMask(GL_FALSE);
  glDisable(GL_POLYGON_OFFSET_FILL);

  lightenBatch
    .setShader(textureAlphaShader)
    .flush();

  contourBatch.flush();

  // ---

  auto projectionMatrix2D = glm::ortho(0.0f, windowInfo.width, 0.0f, windowInfo.height);

  Matrix mvMatrix2D;
  mvMatrix2D
    .translate(0, windowInfo.height)
    .scale(1, -1);

  state2D
    .setShaderMatrix(mvMatrix2D * projectionMatrix2D)
    .apply();

  lineBatch.flush();
  dotBatch.flush();
}

void Sketch::drawPolyline(const vector<glm::vec2> &polyline)
{
  auto size = polyline.size();

  if (size > 1)
  {
    for (auto i = 0; i < size - 1; i++)
    {
      lineBatch.addVertices(polyline[i], polyline[i + 1]);
    }
  }
}

void Sketch::drawDot(const glm::vec2 &position, float radius)
{
  Matrix matrix;
  matrix
    .translate(position)
    .scale(radius / DOT_RADIUS_PIXELS);

  draw::Sprite()
    .setAnchor(0.5f, 0.5f)
    .append(dotBatch, matrix);
}

void Sketch::initTextures()
{
  dotTexture = Texture(
    Texture::ImageRequest("dot_112.png")
      .setFlags(image::FLAGS_TRANSLUCENT)
      .setMipmap(true));

  lysTexture = Texture(
    Texture::ImageRequest("lys_32.png")
      .setFlags(image::FLAGS_TRANSLUCENT_INVERSE)
      .setMipmap(true)
      .setWrap(GL_REPEAT, GL_REPEAT)
      .setAnisotropy(true));
}
