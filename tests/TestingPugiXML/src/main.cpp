#include "Platform.h"
#include "MemoryBuffer.h"
#include "Log.h"

#include <gtest/gtest.h>

#include <pugixml.hpp>

using namespace std;

TEST(TestPugiXML, TestResourceLoading)
{
  fs::path path = "tree.xml";

  pugi::xml_document doc;
  pugi::xml_parse_result result;

  if (chr::hasMemoryResources())
  {
    auto memoryBuffer = chr::getResourceBuffer(path);

    if (memoryBuffer)
    {
      result = doc.load_buffer(memoryBuffer->data(), memoryBuffer->size());
      EXPECT_TRUE(result);
    }
    else
    {
      ADD_FAILURE();
    }
  }
  else if (chr::hasFileResources())
  {
    result = doc.load_file(chr::getResourceFilename(path).data());
    EXPECT_TRUE(result);
  }
  else
  {
    ADD_FAILURE();
  }

  EXPECT_STREQ("mesh_root", doc.child("mesh").attribute("name").value());
}
