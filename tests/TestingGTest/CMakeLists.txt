cmake_minimum_required (VERSION 3.2.2)

project(TestingGTest)

set(BOOST_ROOT "$ENV{CROSS_PATH}/tree/boost/${PLATFORM}")
set(GTEST_ROOT "$ENV{CROSS_PATH}/tree/gtest/${PLATFORM}")

set(INCLUDE_DIRS
  "${BOOST_ROOT}/include"
  "${GTEST_ROOT}/include"
)

set(LIBRARIES
  "${BOOST_ROOT}/lib/libboost_system.a"
  "${BOOST_ROOT}/lib/libboost_filesystem.a"
  "${GTEST_ROOT}/lib/libgtest.a"
  "${GTEST_ROOT}/lib/libgtest_main.a"
)

set(GTEST_SRC "$ENV{CROSS_PATH}/tree/gtest/src")

set(SRC_FILES
  "${GTEST_SRC}/test/gtest-message_test.cc"
  "${GTEST_SRC}/test/gtest-printers_test.cc"
)

include("$ENV{CROSS_PATH}/core/cmake/test.cmake")

#add_test(NAME MessageTest  COMMAND ./run.sh "--gtest_filter=MessageTest.*")
#add_test(NAME PrintersTest COMMAND ./run.sh "--gtest_filter=Print*Test.*")
