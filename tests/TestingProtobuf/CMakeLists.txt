cmake_minimum_required (VERSION 3.2.2)

project(TestingProtobuf)

add_definitions(-DHAVE_ZLIB)

if (PLATFORM MATCHES rpi|linux)
  find_package(Boost COMPONENTS system filesystem REQUIRED)

  list(APPEND INCLUDE_DIRS
    ${Boost_INCLUDE_DIRS}
  )

  list(APPEND LIBRARIES
    ${Boost_LIBRARIES}
    -lpthread
  )
else()
  set(BOOST_ROOT "$ENV{CROSS_PATH}/tree/boost/${PLATFORM}")

  list(APPEND INCLUDE_DIRS
    "${BOOST_ROOT}/include"
  )

  list(APPEND LIBRARIES
    "${BOOST_ROOT}/lib/libboost_system.a"
    "${BOOST_ROOT}/lib/libboost_filesystem.a"
  )
endif()

set(GTEST_ROOT "$ENV{CROSS_PATH}/tree/gtest/${PLATFORM}")
set(PROTOBUF_ROOT "$ENV{CROSS_PATH}/tree/protobuf/${PLATFORM}")

list(APPEND INCLUDE_DIRS
  "${GTEST_ROOT}/include"
  "${PROTOBUF_ROOT}/include"
)

list(APPEND LIBRARIES
  "${GTEST_ROOT}/lib/libgtest.a"
  "${GTEST_ROOT}/lib/libgtest_main.a"
  "${PROTOBUF_ROOT}/lib/libprotobuf.a"
)

set(PROTOBUF_SRC "$ENV{CROSS_PATH}/tree/protobuf/src/src")

set(COMMON_TEST_FILES
  "${PROTOBUF_SRC}/google/protobuf/testing/file.cc"
  "${PROTOBUF_SRC}/google/protobuf/testing/googletest.cc"
)

set(SRC_FILES
  ${COMMON_TEST_FILES}
  "${PROTOBUF_SRC}/google/protobuf/io/coded_stream_unittest.cc"
  "${PROTOBUF_SRC}/google/protobuf/io/printer_unittest.cc"
  "${PROTOBUF_SRC}/google/protobuf/io/tokenizer_unittest.cc"
  "${PROTOBUF_SRC}/google/protobuf/io/zero_copy_stream_unittest.cc"
)

include("$ENV{CROSS_PATH}/core/cmake/test.cmake")
