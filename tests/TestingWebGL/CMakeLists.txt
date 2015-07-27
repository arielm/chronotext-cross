cmake_minimum_required (VERSION 3.2.2)

project(TestingWebGL)

find_package(Boost 1.58.0 REQUIRED COMPONENTS system filesystem)

set(INCLUDE_DIRS
  ${Boost_INCLUDE_DIRS}
)

set(LIBRARIES
  ${Boost_LIBRARIES}
)

set(SRC_FILES
  src/esUtil.c
  src/Hello_Triangle.c
)

include("${CROSS_ROOT}/cmake/test.cmake")
