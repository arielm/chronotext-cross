cmake_minimum_required (VERSION 3.2.2)

project(TestingWebGL)

find_package(Boost 1.58.0 REQUIRED COMPONENTS system filesystem)

#
# XXX
#
set(GLM_INCLUDE_DIR "${GLM_ROOT}/include")

set(INCLUDE_DIRS
  ${Boost_INCLUDE_DIRS}
  ${GLM_INCLUDE_DIR}
)

set(LIBRARIES
  ${Boost_LIBRARIES}
)

set(SRC_FILES
  src/main.cpp
)

include("${CROSS_ROOT}/cmake/test.cmake")
