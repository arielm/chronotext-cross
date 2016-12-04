#include "Sketch.h"

#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/io/zero_copy_stream_impl.h>

#include <fcntl.h> // FOR MINGW + O_RDONLY

using namespace std;
using namespace chr;
using namespace gl;
using namespace google::protobuf;

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
  int fd = open(filePath.string().data(), O_WRONLY | O_CREAT | O_TRUNC, 0666);

  if (fd > 0)
  {
    auto rawOutput = new io::FileOutputStream(fd);
    auto codedOutput = new io::CodedOutputStream(rawOutput);

    // ---

    string text = "Which way to the station?";
    codedOutput->WriteVarint32((uint32)text.size());
    codedOutput->WriteRaw(text.data(), (uint32)text.size());

    codedOutput->WriteLittleEndian32(123456789);
    codedOutput->WriteLittleEndian32(encodeFloat(123.456f));

    // ---

    delete codedOutput;
    delete rawOutput;
    close(fd);
  }
}

void Sketch::readFile(const fs::path &filePath)
{
  int fd = open(filePath.string().data(), O_RDONLY);

  if (fd > 0)
  {
    auto rawInput = new io::FileInputStream(fd);
    auto codedInput = new io::CodedInputStream(rawInput);

    // ---

    uint32 size;
    codedInput->ReadVarint32(&size);

    std::string text;
    text.resize(size);

    codedInput->ReadRaw(&text[0], size);

    //

    uint32_t i;
    codedInput->ReadLittleEndian32(&i);

    float f;
    codedInput->ReadLittleEndian32(reinterpret_cast<uint32_t*>(&f));

    //

    success = (text == "Which way to the station?") && (i = 123456789) && (f == 123.456f);

    // ---

    delete codedInput;
    delete rawInput;
    close(fd);
  }
}
