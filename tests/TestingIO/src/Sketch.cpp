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

static uint32_t encodeFloat(float value)
{
  union {float f; uint32_t i;};
  f = value;
  return i;
}

void Sketch::writeFile(const fs::path &filePath)
{
  BinaryOutputStream outputStream(filePath);

  outputStream.writeString("Which way to the station?");
  outputStream.write<uint32_t>(123456789);
  outputStream.write<float>(123.456f);
}

void Sketch::readFile(const fs::path &filePath)
{
  BinaryInputStream inputStream(InputSource::file(filePath));

  string text = inputStream.readString();
  auto i = inputStream.read<uint32_t>();
  auto f = inputStream.read<float>();

  success = (text == "Which way to the station?") && (i = 123456789) && (f == 123.456f);
}
