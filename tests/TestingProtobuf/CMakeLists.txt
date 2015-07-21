cmake_minimum_required (VERSION 3.2.2)

project(TestingProtobuf)

find_package(Boost 1.58.0 REQUIRED COMPONENTS system filesystem)
find_package(GTest REQUIRED)

set(INCLUDE_DIRS
  ${Boost_INCLUDE_DIRS}
  ${GTEST_INCLUDE_DIRS}
  "${PROTOBUF_ROOT}/include"
)

set(LIBRARIES
  ${Boost_LIBRARIES}
  ${GTEST_BOTH_LIBRARIES}
  "${PROTOBUF_ROOT}/lib/libprotobuf.a"
)

set(PROTOBUF_SRC "$ENV{CROSS_PATH}/deps/protobuf/build/src")

set(SRC_FILES
  "${PROTOBUF_SRC}/src/google/protobuf/io/tokenizer_unittest.cc"
)

include("${CROSS_ROOT}/cmake/test.cmake")

add_test(NAME AllTests COMMAND ./run.sh)
