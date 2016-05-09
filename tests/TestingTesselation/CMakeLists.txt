cmake_minimum_required (VERSION 3.2.2)

project(TestingTesselation)

find_package(Boost 1.58.0 REQUIRED COMPONENTS system filesystem)

#
# XXX
#
set(LIBTESS2_INCLUDE_DIR "${LIBTESS2_ROOT}/include")
set(LIBTESS2_LIBRARY "${LIBTESS2_ROOT}/lib/libtess2.a")

set(INCLUDE_DIRS
  ${Boost_INCLUDE_DIRS}
  ${LIBTESS2_INCLUDE_DIR}
)

set(LIBRARIES
  ${Boost_LIBRARIES}
  ${LIBTESS2_LIBRARY}
)

set(SRC_FILES
  "src/main.cpp"
  "src/Sketch.cpp"
)

# ---

if (PLATFORM MATCHES ios)
  set_source_files_properties(
    "src/main.cpp"
    PROPERTIES COMPILE_FLAGS "-x objective-c++"
  )
endif()

# ---

include("${CROSS_ROOT}/cmake/sketch.cmake")
include("${CROSS_ROOT}/cmake/test.cmake")
