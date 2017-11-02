#include "Sketch.h"

#include "chr/io/BinaryInputStream.h"
#include "chr/io/BinaryOutputStream.h"

using namespace std;
using namespace chr;
using namespace io;
using namespace google;

static constexpr int DATA_SIZE = 100;

void Sketch::setup()
{
  data = make_unique<uint8_t[]>(DATA_SIZE);
  populateData();

  auto documentsFolder = getDocumentsFolder();
  fs::path filePath = documentsFolder / "test.dat";

  if (false)
  {
    auto outputTargetBuffer = OutputTarget::buffer();
    writeFile(outputTargetBuffer);

    auto inputSourceBuffer = InputSource::buffer(outputTargetBuffer.getData(), outputTargetBuffer.getDataSize());
    success = readFile(inputSourceBuffer);
  }
  else
  {
    auto outputTargetFile = OutputTarget::file(filePath);
    writeFile(outputTargetFile);

    auto inputSourceFile = InputSource::file(filePath);
    success = readFile(inputSourceFile);
  }

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
    glClearColor(0, 1, 0, 1);
  }
  else
  {
    glClearColor(1, 0, 0, 1);
  }

  glClear(GL_COLOR_BUFFER_BIT);
}

void Sketch::writeFile(OutputTarget &outputTarget)
{
  BinaryOutputStream outputStream(outputTarget);

  outputStream.writeString("Which way to the station?");
  outputStream.writeString("XMEN4", 5);
  outputStream.write<uint64_t>(999999999999999);
  outputStream.write<int32_t>(-123456789);
  outputStream.write(123.456f);
  outputStream.write(M_PI);
  outputStream.write(true);
  outputStream.write(glm::vec3(0.5f, -1.0f, 0));
  outputStream.writeBytes(data.get(), DATA_SIZE);
}

bool Sketch::readFile(const InputSource &inputSource)
{
  BinaryInputStream inputStream(inputSource);

  auto s1 = inputStream.readString();
  auto s2 = inputStream.readString(5);
  auto l1 = inputStream.read<uint64_t>();
  auto i1 = inputStream.read<int32_t>();
  auto f1 = inputStream.read<float>();
  auto f2 = inputStream.read<double>();
  auto b1 = inputStream.read<bool>();
  auto v1 = inputStream.read<glm::vec3>();

  uint8_t d1[DATA_SIZE];
  inputStream.readBytes(d1, DATA_SIZE);

  return (s1 == "Which way to the station?") &&
    (s2 == "XMEN4") &&
    (l1 == 999999999999999L) &&
    (i1 == -123456789) &&
    (f1 == 123.456f) &&
    (f2 == M_PI) &&
    b1 &&
    (v1 == glm::vec3(0.5f, -1.0f, 0)) &&
    checkData(d1);
}

void Sketch::populateData()
{
  for (int i = 0; i < DATA_SIZE; i++)
  {
    data[i] = i;
  }
}

bool Sketch::checkData(uint8_t *d)
{
  for (int i = 0; i < DATA_SIZE; i++)
  {
    if (*d++ != data[i])
    {
      return false;
    }
  }

  return true;
}
