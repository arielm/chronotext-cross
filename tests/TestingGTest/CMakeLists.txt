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

set(GTEST_SRC "$ENV{CROSS_PATH}/deps/gtest/build/src")

set(SRC_FILES
  "${GTEST_SRC}/test/gtest-message_test.cc"
  "${GTEST_SRC}/test/gtest-printers_test.cc"
)

include("${CROSS_ROOT}/cmake/test.cmake")

#add_test(NAME MessageTest  COMMAND ./run.sh "--gtest_filter=MessageTest.*")
#add_test(NAME PrintersTest COMMAND ./run.sh "--gtest_filter=Print*Test.*")
