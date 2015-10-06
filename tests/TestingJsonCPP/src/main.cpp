#include "Platform.h"
#include "Log.h"

#include <gtest/gtest.h>

#include <json/value.h>
#include <json/reader.h>
#include <json/writer.h>

using namespace std;

TEST(TestJsonCPP, TestRead)
{
  Json::Value root;
  Json::Reader reader;

  fs::ifstream config_doc(chr::getResourcePath("config_doc.json"), ifstream::binary);
  EXPECT_TRUE(reader.parse(config_doc, root));

  Json::FastWriter writer;
  LOGI << writer.write(root) << endl;
}

TEST(TestJsonCPP, TestParse)
{
  const char *json = R"({"my-encoding":"UTF-8","my-indent":{"length":3,"use_space":true},"my-plug-ins":["python","c++","ruby"]})";

  Json::Value root;
  Json::Reader reader;
  EXPECT_TRUE(reader.parse(json, root));

  string encoding = root.get("my-encoding", "???").asString();
  EXPECT_STREQ("UTF-8", encoding.data());

  int indentLength = root["my-indent"].get("length", -1).asInt();
  EXPECT_EQ(3, indentLength);

  bool indentUseSpace =root["my-indent"].get("use_space", false).asBool();
  EXPECT_TRUE(indentUseSpace);
}
