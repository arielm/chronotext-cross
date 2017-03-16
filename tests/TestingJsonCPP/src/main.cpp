#include "chr/Platform.h"
#include "chr/ResourceBuffer.h"
#include "chr/Log.h"

#include <gtest/gtest.h>

#include <json/value.h>
#include <json/reader.h>
#include <json/writer.h>

using namespace std;

TEST(TestJsonCPP, TestResourceLoading)
{
  Json::Value root;

  auto stream = chr::getResourceStream("config_doc.json");

  if (stream)
  {
    Json::Reader reader;
    EXPECT_TRUE(reader.parse(*stream, root));
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
