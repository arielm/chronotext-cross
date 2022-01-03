cmake_minimum_required (VERSION 3.2.2)

project(TestingStaticInstancing)

set(SRC_FILES
  "src/Sketch.cpp"
)

#
# For Mojave and up
#
add_compile_definitions(
  GL_SILENCE_DEPRECATION
)

include("$ENV{CROSS_PATH}/core/cmake/sketch.cmake")
include("$ENV{CROSS_PATH}/core/cmake/test.cmake")
