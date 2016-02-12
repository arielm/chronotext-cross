cmake_minimum_required (VERSION 3.2.2)

project(TestingLibJpg)

find_package(Boost 1.58.0 REQUIRED COMPONENTS system filesystem)

set(LIBJPEG_INCLUDE_DIR "${LIBJPEG_ROOT}/include")
set(LIBJPEG_LIBRARY "${LIBJPEG_ROOT}/lib/libjpeg.a")

set(INCLUDE_DIRS
  ${Boost_INCLUDE_DIRS}
  ${LIBJPEG_INCLUDE_DIR}
)

set(LIBRARIES
  ${Boost_LIBRARIES}
  ${LIBJPEG_LIBRARY}
)

set(SRC_FILES
  "src/main.cpp"
  "src/Sketch.cpp"
  "${CROSS_ROOT}/src/gl/TextureShader.cpp"
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
