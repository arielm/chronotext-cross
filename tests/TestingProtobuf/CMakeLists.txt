cmake_minimum_required (VERSION 3.2.2)

project(TestingProtobuf)

set(BOOST_ROOT "$ENV{CROSS_PATH}/tree/boost/${PLATFORM}")
set(GTEST_ROOT "$ENV{CROSS_PATH}/tree/gtest/${PLATFORM}")
set(PROTOBUF_ROOT "$ENV{CROSS_PATH}/deps/protobuf/dist/${PLATFORM}")

set(INCLUDE_DIRS
  "${BOOST_ROOT}/include"
  "${GTEST_ROOT}/include"
  "${PROTOBUF_ROOT}/include"
)

set(LIBRARIES
  "${BOOST_ROOT}/lib/libboost_system.a"
  "${BOOST_ROOT}/lib/libboost_filesystem.a"
  "${GTEST_ROOT}/lib/libgtest.a"
  "${GTEST_ROOT}/lib/libgtest_main.a"
  "${PROTOBUF_ROOT}/lib/libprotobuf.a"
)

set(PROTOBUF_SRC "$ENV{CROSS_PATH}/deps/protobuf/build/src")

set(COMMON_TEST_FILES
  "${PROTOBUF_SRC}/src/google/protobuf/testing/file.cc"
  "${PROTOBUF_SRC}/src/google/protobuf/testing/googletest.cc"
)

set(SRC_FILES
  ${COMMON_TEST_FILES}
  "${PROTOBUF_SRC}/src/google/protobuf/io/coded_stream_unittest.cc"
  "${PROTOBUF_SRC}/src/google/protobuf/io/printer_unittest.cc"
  "${PROTOBUF_SRC}/src/google/protobuf/io/tokenizer_unittest.cc"
  "${PROTOBUF_SRC}/src/google/protobuf/io/zero_copy_stream_unittest.cc"
)

include("$ENV{CROSS_PATH}/core/cmake/test.cmake")
