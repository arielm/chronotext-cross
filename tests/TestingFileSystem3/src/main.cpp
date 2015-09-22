#include "Platform.h"
#include "Log.h"
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
    EXPECT_TRUE(codedInput->ReadString(&version, 9));
    EXPECT_EQ("XFONT.004", version);
    EXPECT_TRUE(codedInput->Skip(1)); // XXX

    uint32 glyphCount;
    EXPECT_TRUE(codedInput->ReadLittleEndian32(&glyphCount));
    EXPECT_EQ(191, glyphCount);

    float baseSize;
    EXPECT_TRUE(codedInput->ReadLittleEndian32(reinterpret_cast<uint32_t*>(&baseSize)));
    EXPECT_EQ(64, baseSize);

    /*
     * TODO: CHECK GetDirectBufferPointer()
     * https://github.com/google/protobuf/blob/master/src/google/protobuf/io/coded_stream.h#L183-190
     */

    // ---

    delete codedInput;

    if (rawInput)
    {
      delete rawInput;
      close(fd);  
    }
  }
}
