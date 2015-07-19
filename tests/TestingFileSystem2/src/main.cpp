#include "Log.h"
#include "Platform.h"
#include "MemoryBuffer.h"

#include <gtest/gtest.h>
#include <experimental/string_view>

#define STB_IMAGE_IMPLEMENTATION
#define STBI_ONLY_JPEG
#define STBI_ONLY_PNG

#include "stb_image.h"

using namespace std;
using string_view = experimental::string_view;

TEST(TestFileSystem2, TextResource)
{
  fs::path path = "sub/credits.txt";
  string expectedText = "https://commons.wikimedia.org/wiki/File:G%C3%A9r%C3%B4me-Black_Bashi-Bazouk-c._1869.jpg";

  if (chr::hasMemoryResources())
  {
    auto memoryBuffer = chr::getResourceBuffer(path);

    if (memoryBuffer)
    {
      string_view text(reinterpret_cast<const char*>(memoryBuffer->data()), memoryBuffer->size());
      EXPECT_EQ(expectedText, text);
    }
    else
    {
      ADD_FAILURE() << "chr::getResourceBuffer";
    }
  }
  else if (chr::hasFileResources())
  {
    auto resPath = chr::getResourcePath(path);
    fs::ifstream in(resPath, ios::in | ios::binary | ios::ate);
    
    if (in)
    {
      auto fileSize = in.tellg();
      in.seekg(0, ios::beg);

      string text(fileSize, 0);
      in.read(&text[0], fileSize);

      EXPECT_EQ(expectedText, text);
    }
    else
    {
      ADD_FAILURE() << "fs::ifstream";
    }
  }
}

TEST(TestFileSystem2, ImageResource)
{
  fs::path path = "2008.547.1crop_4.jpg";
  int expectedX = 850;
  int expectedY = 850;
  int expectedComp = 3;

  if (chr::hasMemoryResources())
  {
    auto memoryBuffer = chr::getResourceBuffer(path);

    if (memoryBuffer)
    {
      int x, y, comp;
      stbi_uc *data = stbi_load_from_memory(reinterpret_cast<const stbi_uc*>(memoryBuffer->data()), memoryBuffer->size(), &x, &y, &comp, 0);

      if (data)
      {
        EXPECT_EQ(expectedX, x);
        EXPECT_EQ(expectedY, y);
        EXPECT_EQ(expectedComp, comp);

        stbi_image_free(data);
      }
      else
      {
        ADD_FAILURE() << "stbi_load_from_memory";
      }
    }
    else
    {
      ADD_FAILURE() << "chr::getResourceBuffer";
    }
  }
  else if (chr::hasFileResources())
  {
    auto resPath = chr::getResourcePath(path);

    int x, y, comp;
    stbi_uc *data = stbi_load(resPath.string().data(), &x, &y, &comp, 0);

    if (data)
    {
      EXPECT_EQ(expectedX, x);
      EXPECT_EQ(expectedY, y);
      EXPECT_EQ(expectedComp, comp);

      stbi_image_free(data);
    }
    else
    {
      ADD_FAILURE() << "stbi_load";
    }
  }
}
