#include "Platform.h"
#include "Log.h"

#include <gtest/gtest.h>

#include <json/value.h>
#include <json/reader.h>

using namespace std;

TEST(TestJsonCPP, TestRead)
{
  Json::Value root;
  Json::Reader reader;

  fs::ifstream config_doc(chr::getResourcePath("config_doc.json"), ifstream::binary);
  EXPECT_TRUE(reader.parse(config_doc, root));
}
