
if (NOT DEFINED ENV{EMSCRIPTEN_PATH})
  message(FATAL_ERROR "EMSCRIPTEN_PATH MUST BE DEFINED!")
endif()

# ---

#
# TODO: AVOID DOUBLE-INCLUSION OF FLAGS
#
set(CMAKE_CXX_FLAGS "-Wno-warn-absolute-paths -std=c++14 -s WASM=0"
  CACHE STRING "cmake_cxx_flags/emscripten"
)

include("$ENV{EMSCRIPTEN_PATH}/cmake/Modules/Platform/Emscripten.cmake")
