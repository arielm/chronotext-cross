cmake_minimum_required (VERSION 3.2.2)

project(TestingPugiXML)

set(BOOST_ROOT "$ENV{CROSS_PATH}/deps/boost/dist/${PLATFORM}")
set(GTEST_ROOT "$ENV{CROSS_PATH}/deps/gtest/dist/${PLATFORM}")
set(PUGIXML_ROOT "$ENV{CROSS_PATH}/deps/pugixml/dist/${PLATFORM}")

find_package(Boost 1.58.0 REQUIRED COMPONENTS system filesystem)
find_package(GTest REQUIRED)

set(INCLUDE_DIRS
  ${Boost_INCLUDE_DIRS}
  ${GTEST_INCLUDE_DIRS}
  "${PUGIXML_ROOT}/include"
)

set(LIBRARIES
  ${Boost_LIBRARIES}
  ${GTEST_BOTH_LIBRARIES}
  "${PUGIXML_ROOT}/lib/libpugixml.a"
)

set(SRC_FILES
  src/main.cpp
)

include("$ENV{CROSS_PATH}/core/cmake/test.cmake")
