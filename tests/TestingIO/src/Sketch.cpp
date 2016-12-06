#include "Sketch.h"

#include "chr/io/BinaryInputStream.h"
#include "chr/io/BinaryOutputStream.h"

using namespace std;
using namespace chr;
using namespace io;
using namespace google;

void Sketch::setup()
{
  data = make_unique<uint8_t[]>(100);
  populateData();

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
  outputStream.writeString("XMEN4", 5);
  outputStream.write<uint64_t>(999999999999999);
  outputStream.write<int32_t>(-123456789);
  outputStream.write(123.456f);
  outputStream.write(M_PI);
  outputStream.write(glm::vec3(0.5f, -1.0f, 0));
  outputStream.writeBytes(data.get(), 100);
}

void Sketch::readFile(const fs::path &filePath)
{
  BinaryInputStream inputStream(InputSource::file(filePath));

  auto s1 = inputStream.readString();
  auto s2 = inputStream.readString(5);
  auto l1 = inputStream.read<uint64_t>();
  auto i1 = inputStream.read<int32_t>();
  auto f1 = inputStream.read<float>();
  auto f2 = inputStream.read<double>();
  auto v1 = inputStream.read<glm::vec3>();

  uint8_t d1[100];
  inputStream.readBytes(&d1[0], 100);

  success = (s1 == "Which way to the station?") &&
    (s2 == "XMEN4") &&
    (l1 == 999999999999999L) &&
    (i1 == -123456789) &&
    (f1 == 123.456f) &&
    (f2 == M_PI) &&
    (v1 == glm::vec3(0.5f, -1.0f, 0)) &&
    checkData(&d1[0]);
}

void Sketch::populateData()
{
  for (int i = 0; i < 100; i++)
  {
    data[i] = i;
  }
}

bool Sketch::checkData(uint8_t *d)
{
  for (int i = 0; i < 100; i++)
  {
    if (*d++ != data[i])
    {
      return false;
    }
  }

  return true;
}
