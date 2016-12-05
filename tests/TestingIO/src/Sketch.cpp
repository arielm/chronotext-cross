#include "Sketch.h"

#include "chr/io/BinaryInputStream.h"
#include "chr/io/BinaryOutputStream.h"

using namespace std;
using namespace chr;
using namespace io;
using namespace google;

void Sketch::setup()
{
  auto documentsFolder = systemManager().getDocumentsFolder();
  LOGI << documentsFolder << endl;

  writeFile(documentsFolder / "test.dat");
  readFile(documentsFolder / "test.dat");

  // ---

  glDisable(GL_DEPTH_TEST);
  glDepthMask(GL_FALSE);

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void Sketch::draw()
{
  if (success)
  {
    glClearColor(1, 0, 0, 1);
  }
  else
  {
    glClearColor(0.5f, 0.5f, 0.5f, 1);
  }

  glClear(GL_COLOR_BUFFER_BIT);
}

void Sketch::writeFile(const fs::path &filePath)
{
  BinaryOutputStream outputStream(filePath);

  outputStream.writeString("Which way to the station?");
  outputStream.write(-123456789);
  outputStream.write(123.456f);
  outputStream.write(M_PI);
}

void Sketch::readFile(const fs::path &filePath)
{
  BinaryInputStream inputStream(InputSource::file(filePath));

  string text = inputStream.readString();
  auto i1 = inputStream.read<int32_t>();
  auto f1 = inputStream.read<float>();
  auto f2 = inputStream.read<double>();

  success = (text == "Which way to the station?") &&
    (i1 == -123456789) &&
    (f1 == 123.456f) &&
    (f2 == M_PI);
}
