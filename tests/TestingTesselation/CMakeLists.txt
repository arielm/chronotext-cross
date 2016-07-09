cmake_minimum_required (VERSION 3.2.2)

project(TestingTesselation)

set(SRC_FILES
  "src/main.cpp"
  "src/Sketch.cpp"
)

if (PLATFORM MATCHES ios)
  set_source_files_properties(
    "src/main.cpp"
    PROPERTIES COMPILE_FLAGS "-x objective-c++"
  )
endif()

# ---

include("${CROSS_ROOT}/cmake/sketch.cmake")
include("${CROSS_ROOT}/cmake/test.cmake")
