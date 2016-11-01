cmake_minimum_required (VERSION 3.2.2)

project(TestingHarfbuzz)

set(BOOST_ROOT "$ENV{CROSS_PATH}/deps/boost/dist/${PLATFORM}")
set(GTEST_ROOT "$ENV{CROSS_PATH}/deps/gtest/dist/${PLATFORM}")
set(FREETYPE_ROOT "$ENV{CROSS_PATH}/deps/freetype/dist/${PLATFORM}")
set(HARFBUZZ_ROOT "$ENV{CROSS_PATH}/deps/harfbuzz/dist/${PLATFORM}")

find_package(Boost 1.58.0 REQUIRED COMPONENTS system filesystem)
find_package(GTest REQUIRED)

set(INCLUDE_DIRS
  ${Boost_INCLUDE_DIRS}
  ${GTEST_INCLUDE_DIRS}
  "${FREETYPE_ROOT}/include"
  "${HARFBUZZ_ROOT}/include"
)

set(LIBRARIES
  ${Boost_LIBRARIES}
  ${GTEST_BOTH_LIBRARIES}
  "${FREETYPE_ROOT}/lib/libfreetype.a"
  "${HARFBUZZ_ROOT}/lib/libharfbuzz.a"
)

set(SRC_FILES
  src/main.cpp
)

include("$ENV{CROSS_PATH}/core/cmake/test.cmake")
