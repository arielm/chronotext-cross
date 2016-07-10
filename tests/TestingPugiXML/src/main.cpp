#include "Platform.h"
#include "MemoryBuffer.h"
#include "Log.h"

#include <gtest/gtest.h>

#include <pugixml.hpp>

using namespace std;

TEST(TestPugiXML, TestResourceLoading)
{
  pugi::xml_document doc;

  auto stream = chr::getResourceStream("tree.xml");

  if (stream)
  {
    pugi::xml_parse_result result = doc.load(*stream);
    EXPECT_TRUE(result.status == pugi::xml_parse_status::status_ok);
  }
  else
  {
    ADD_FAILURE();
  }

  EXPECT_STREQ("mesh_root", doc.child("mesh").attribute("name").value());
}
