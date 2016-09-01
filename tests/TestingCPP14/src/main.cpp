#include <gtest/gtest.h>

#include <experimental/string_view>

using namespace std;
using string_view = experimental::string_view;

static int fooCount = 0;

class Foo
{
public:
  Foo()
  {
    fooCount++;
  }

  ~Foo()
  {
    fooCount--;
  }
};

TEST(TestCPP14, TestStringView)
{
  string cppstr = "foo";
  string_view cppstr_v(cppstr);

  char array[3] = {'f', 'o', 'o'};
  string_view array_v(array, sizeof array);

  EXPECT_TRUE(cppstr_v == array_v);
}

TEST(TestCPP14, TestMakeUnique)
{
  {
    auto foo = make_unique<Foo>();
    EXPECT_EQ(1, fooCount);
  }

  EXPECT_EQ(0, fooCount);
}
