cmake_minimum_required (VERSION 3.2.2)

project(TestingFileSystem2)

find_package(Boost 1.58.0 REQUIRED COMPONENTS system filesystem)
find_package(GTest REQUIRED)

set(INCLUDE_DIRS
  ${Boost_INCLUDE_DIRS}
  ${GTEST_INCLUDE_DIRS}
)

set(LIBRARIES
  ${Boost_LIBRARIES}
  ${GTEST_BOTH_LIBRARIES}
)

set(SRC_FILES
  src/main.cpp
)

include("${CROSS_ROOT}/cmake/test.cmake")

add_test(NAME AllTests COMMAND ./run.sh)
