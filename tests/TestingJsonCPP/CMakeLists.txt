cmake_minimum_required (VERSION 3.2.2)

project(TestingJsonCPP)

set(BOOST_ROOT "$ENV{CROSS_PATH}/deps/boost/dist/${PLATFORM}")
set(GTEST_ROOT "$ENV{CROSS_PATH}/deps/gtest/dist/${PLATFORM}")
set(JSONCPP_ROOT "$ENV{CROSS_PATH}/deps/jsoncpp/dist/${PLATFORM}")

find_package(Boost 1.58.0 REQUIRED COMPONENTS system filesystem)
find_package(GTest REQUIRED)

set(INCLUDE_DIRS
  ${Boost_INCLUDE_DIRS}
  ${GTEST_INCLUDE_DIRS}
  "${JSONCPP_ROOT}/include"
)

set(LIBRARIES
  ${Boost_LIBRARIES}
  ${GTEST_BOTH_LIBRARIES}
  "${JSONCPP_ROOT}/lib/libjsoncpp.a"
)

set(SRC_FILES
  src/main.cpp
)

include("$ENV{CROSS_PATH}/core/cmake/test.cmake")
