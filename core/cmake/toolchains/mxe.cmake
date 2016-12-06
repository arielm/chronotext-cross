
if (NOT DEFINED ENV{MXE_PATH})
  message(FATAL_ERROR "MXE_PATH MUST BE DEFINED!")
endif()

# ---

add_definitions(-DUNICODE -D_UNICODE)
add_definitions(-DWIN32 -D_WIN32)

set(MXE_TARGET i686-w64-mingw32.static
  CACHE STRING "mxe_target"
)

#
# TODO: AVOID DOUBLE-INCLUSION OF FLAGS
#
set(CMAKE_CXX_FLAGS "-Wno-deprecated-declarations -std=c++1y"
  CACHE STRING "cmake_cxx_flags/mxe"
)

include("$ENV{MXE_PATH}/usr/${MXE_TARGET}/share/cmake/mxe-conf.cmake")
