cmake_minimum_required (VERSION 3.2.2)

project(TestingDynamicInstancing)

set(SRC_FILES
  "src/Sketch.cpp"
  "src/SeaSurface.cpp"
  "src/SeaSurfaceSpiral2.cpp"
)

#
# For Mojave and up
#
add_compile_definitions(
  GL_SILENCE_DEPRECATION
)

include("$ENV{CROSS_PATH}/core/cmake/sketch.cmake")
include("$ENV{CROSS_PATH}/core/cmake/test.cmake")
