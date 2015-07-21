#include "Log.h"
#include "Platform.h"
#include "MemoryBuffer.h"

#include <gtest/gtest.h>

#include <google/protobuf/io/zero_copy_stream_impl.h>
#include <google/protobuf/io/coded_stream.h>

#include <fcntl.h> // FOR MINGW + O_RDONLY

using namespace std;
using namespace google::protobuf;

TEST(TestFileSystem3, ZeroCopyInputStream)
{
  fs::path path = "Georgia_Regular_64.fnt";

  io::CodedInputStream *codedInput = nullptr;
  io::ZeroCopyInputStream *rawInput = nullptr;
  shared_ptr<chr::MemoryBuffer> memoryBuffer;
  int fd = 0;

  if (chr::hasMemoryResources())
  {
    memoryBuffer = chr::getResourceBuffer(path);

    if (memoryBuffer)
    {
      codedInput = new io::CodedInputStream(reinterpret_cast<const uint8*>(memoryBuffer->data()), memoryBuffer->size());
    }
    else
    {
      ADD_FAILURE() << "chr::getResourceBuffer";
    }
  }
  else if (chr::hasFileResources())
  {
    auto resPath = chr::getResourcePath(path);
    fd = open(resPath.string().data(), O_RDONLY);

    if (fd > 0)
    {
      rawInput = new io::FileInputStream(fd);
      codedInput = new io::CodedInputStream(rawInput);
    }
    else
    {
      ADD_FAILURE() << "open";
    }
  }

  if (codedInput)
  {
    string version;

    if (codedInput->ReadString(&version, 9))
    {
      LOGI << "{" << version << "}" << endl;

      delete codedInput;

      if (rawInput)
      {
        delete rawInput;
        close(fd);  
      }
    }
    else
    {
      ADD_FAILURE() << "CodedInputStream::ReadString";
    }
  }
}
