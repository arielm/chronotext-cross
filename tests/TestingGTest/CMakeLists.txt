cmake_minimum_required (VERSION 3.2.2)

project(TestingGTest)

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
  src/gtest-message_test.cc
  src/gtest-printers_test.cc
)

include("${CROSS_ROOT}/cmake/test.simple.cmake")

add_test(NAME AllTests COMMAND ./run.sh)
