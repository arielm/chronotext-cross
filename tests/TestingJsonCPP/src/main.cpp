#include "Platform.h"
#include "MemoryBuffer.h"
#include "Log.h"

#include <gtest/gtest.h>

#include <json/value.h>
#include <json/reader.h>
#include <json/writer.h>

using namespace std;

TEST(TestJsonCPP, TestResourceLoading)
{
  fs::path path = "config_doc.json";

  Json::Value root;
  Json::Reader reader;

  if (chr::hasMemoryResources())
  {
    auto memoryBuffer = chr::getResourceBuffer(path);

    if (memoryBuffer)
    {
      const char *beginDoc = (const char*)memoryBuffer->data();
      const char *endDoc = beginDoc + memoryBuffer->size();

      EXPECT_TRUE(reader.parse(beginDoc, endDoc, root));
    }
    else
    {
      ADD_FAILURE();
    }
  }
  else if (chr::hasFileResources())
  {
    fs::ifstream stream(chr::getResourcePath(path), ifstream::binary);
    EXPECT_TRUE(reader.parse(stream, root));
  }
  else
  {
    ADD_FAILURE();
  }

  // ---

  string encoding = root.get("my-encoding", "???").asString();
  EXPECT_STREQ("UTF-8", encoding.data());

  int indentLength = root["my-indent"].get("length", -1).asInt();
  EXPECT_EQ(3, indentLength);

  bool indentUseSpace =root["my-indent"].get("use_space", false).asBool();
  EXPECT_TRUE(indentUseSpace);
}
