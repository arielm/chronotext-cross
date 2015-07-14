cmake_minimum_required (VERSION 3.2.2)

project(TestingFileSystem1)

find_package(Boost 1.58.0 REQUIRED COMPONENTS system filesystem)

set(INCLUDE_DIRS
  ${Boost_INCLUDE_DIRS}
)

set(LIBRARIES
  ${Boost_LIBRARIES}
)

set(SRC_FILES
  src/main.cpp
)

include("${CROSS_ROOT}/cmake/test.simple.cmake")

add_test(NAME AllTests COMMAND ./run.sh)
#add_test(NAME MessageTest  COMMAND ./run.sh --gtest_filter=MessageTest.*)
#add_test(NAME PrintersTest COMMAND ./run.sh --gtest_filter=Print*Test.*)
