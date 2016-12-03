cmake_minimum_required (VERSION 3.2.2)

project(TestingIO)

set(PROTOBUF_ROOT "$ENV{CROSS_PATH}/tree/protobuf/${PLATFORM}")

set(INCLUDE_DIRS
  "${PROTOBUF_ROOT}/include"
)

set(LIBRARIES
  "${PROTOBUF_ROOT}/lib/libprotobuf.a"
)

set(SRC_FILES
  "src/Sketch.cpp"
)

include("$ENV{CROSS_PATH}/core/cmake/sketch.cmake")
include("$ENV{CROSS_PATH}/core/cmake/test.cmake")
