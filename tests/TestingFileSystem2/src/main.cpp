#include "chr/Platform.h"
#include "chr/Log.h"
#include "chr/ResourceBuffer.h"

#include <gtest/gtest.h>
#include <string_view>

// ---

#define STB_IMAGE_IMPLEMENTATION
#define STBI_ONLY_JPEG
#define STBI_ONLY_PNG

#include "stb_image.h"

// ---

using namespace std;

TEST(TestFileSystem2, TextResource)
{
  fs::path path = "sub/credits.txt";
  string expectedText = "https://commons.wikimedia.org/wiki/File:G%C3%A9r%C3%B4me-Black_Bashi-Bazouk-c._1869.jpg";

  if (chr::hasMemoryResources())
  {
    auto resourceBuffer = chr::getResourceBuffer(path);

    if (resourceBuffer)
    {
      string_view text(reinterpret_cast<const char*>(resourceBuffer->data()), resourceBuffer->size());
      EXPECT_EQ(expectedText, text);
    }
    else
    {
      ADD_FAILURE() << "chr::getResourceBuffer";
    }
  }
  else if (chr::hasFileResources())
  {
    auto resPath = chr::getResourceFilePath(path);
    fs::ifstream in(resPath, ios::binary | ios::ate);
    
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

TEST(TestFileSystem2, BinaryResource)
{
  fs::path path = "2008.547.1crop_4.jpg";
  int expectedX = 850;
  int expectedY = 850;
  int expectedComp = 3;

  if (chr::hasMemoryResources())
  {
    auto resourceBuffer = chr::getResourceBuffer(path);

    if (resourceBuffer)
    {
      int x, y, comp;
      stbi_uc *imageData = stbi_load_from_memory(reinterpret_cast<const stbi_uc*>(resourceBuffer->data()), resourceBuffer->size(), &x, &y, &comp, 0);

      if (imageData)
      {
        EXPECT_EQ(expectedX, x);
        EXPECT_EQ(expectedY, y);
        EXPECT_EQ(expectedComp, comp);

        stbi_image_free(imageData);
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
    auto resPath = chr::getResourceFilePath(path);

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

TEST(TestFileSystem2, BinaryFile)
{
  if (chr::hasFileResources())
  {
    fs::path path = "2008.547.1crop_4.jpg";
    int expectedX = 850;
    int expectedY = 850;
    int expectedComp = 3;

    auto resPath = chr::getResourceFilePath(path);
    fs::ifstream in(resPath, ios::binary | ios::ate);

    if (in)
    {
      auto fileSize = in.tellg();
      in.seekg(0, ios::beg);
              
      char *data = new char[fileSize];
      in.read(data, fileSize);

      //

      int x, y, comp;
      stbi_uc *imageData = stbi_load_from_memory(reinterpret_cast<const stbi_uc*>(data), fileSize, &x, &y, &comp, 0);

      if (imageData)
      {
        EXPECT_EQ(expectedX, x);
        EXPECT_EQ(expectedY, y);
        EXPECT_EQ(expectedComp, comp);

        stbi_image_free(imageData);
      }
      else
      {
        ADD_FAILURE() << "stbi_load_from_memory";
      }
    }
    else
    {
      ADD_FAILURE() << "fs::ifstream";
    }
  }
}

TEST(TestFileSystem2, ForcedMemoryLoad)
{
  fs::path path = "2008.547.1crop_4.jpg";
  int expectedX = 850;
  int expectedY = 850;
  int expectedComp = 3;

  auto resourceBuffer = chr::getResourceBuffer(path);

  if (resourceBuffer)
  {
    int x, y, comp;
    stbi_uc *imageData = stbi_load_from_memory(reinterpret_cast<const stbi_uc*>(resourceBuffer->data()), resourceBuffer->size(), &x, &y, &comp, 0);

    if (imageData)
    {
      EXPECT_EQ(expectedX, x);
      EXPECT_EQ(expectedY, y);
      EXPECT_EQ(expectedComp, comp);

      stbi_image_free(imageData);
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
