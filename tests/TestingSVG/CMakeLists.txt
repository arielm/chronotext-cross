cmake_minimum_required (VERSION 3.2.2)

project(TestingSVG)

set(SRC_FILES
  "src/Sketch.cpp"
)

include("$ENV{CROSS_PATH}/core/cmake/sketch.cmake")
include("$ENV{CROSS_PATH}/core/cmake/test.cmake")
