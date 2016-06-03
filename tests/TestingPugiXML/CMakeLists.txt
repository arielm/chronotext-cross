cmake_minimum_required (VERSION 3.2.2)

project(TestingPugiXML)

find_package(Boost 1.58.0 REQUIRED COMPONENTS system filesystem)
find_package(GTest REQUIRED)

#
# XXX
#
set(PUGIXML_INCLUDE_DIR "${PUGIXML_ROOT}/include")
set(PUGIXML_LIBRARY "${PUGIXML_ROOT}/lib/libpugixml.a")

set(INCLUDE_DIRS
  ${Boost_INCLUDE_DIRS}
  ${GTEST_INCLUDE_DIRS}
  ${PUGIXML_INCLUDE_DIR}
)

set(LIBRARIES
  ${Boost_LIBRARIES}
  ${GTEST_BOTH_LIBRARIES}
  ${PUGIXML_LIBRARY}
)

set(SRC_FILES
  src/main.cpp
)

include("${CROSS_ROOT}/cmake/test.cmake")
