cmake_minimum_required (VERSION 3.2.2)

project(TestingInput)

set(SRC_FILES
  "src/Sketch.cpp"
)

include("${CROSS_ROOT}/cmake/sketch.cmake")
include("${CROSS_ROOT}/cmake/test.cmake")
