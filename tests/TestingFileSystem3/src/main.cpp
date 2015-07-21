#include "Log.h"
#include "Platform.h"
#include "MemoryBuffer.h"

#include <gtest/gtest.h>

#include <google/protobuf/io/zero_copy_stream_impl.h>
#include <google/protobuf/io/coded_stream.h>

#include <fcntl.h> // FOR MINGW + O_RDONLY

using namespace std;
using namespace google::protobuf::io;

TEST(TestFileSystem3, ZeroCopyInputStream)
{
  fs::path path = "Georgia_Regular_64.fnt";

  ZeroCopyInputStream *rawInput = nullptr;
  shared_ptr<chr::MemoryBuffer> memoryBuffer;
  int fd = 0;

  if (chr::hasMemoryResources())
  {
    memoryBuffer = chr::getResourceBuffer(path);

    if (memoryBuffer)
    {
      rawInput = new ArrayInputStream(memoryBuffer->data(), memoryBuffer->size());
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
      rawInput = new FileInputStream(fd);
    }
    else
    {
      ADD_FAILURE() << "open";
    }
  }

  if (rawInput)
  {
    CodedInputStream *codedInput = new CodedInputStream(rawInput);

    string version;
    if (codedInput->ReadString(&version, 9))
    {
      LOGI << "{" << version << "}" << endl;

      delete codedInput;
      delete rawInput;

      if (fd)
      {
        close(fd);  
      }
    }
    else
    {
      ADD_FAILURE() << "CodedInputStream::ReadString";
    }
  }
}
