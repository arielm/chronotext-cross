cmake_minimum_required (VERSION 3.2.2)

project(TestingGLFW)

find_package(Boost 1.58.0 REQUIRED COMPONENTS system filesystem)

#
# BECAUSE find_package(Protobuf REQUIRED) IS NOT WORKING AS INTENDED
#
set(PROTOBUF_INCLUDE_DIRS "${PROTOBUF_ROOT}/include")
set(PROTOBUF_LIBRARY "${PROTOBUF_ROOT}/lib/libprotobuf.a")

#
# XXX
#
set(GLFW_INCLUDE_DIR "${GLFW_ROOT}/include")
set(GLFW_LIBRARY "${GLFW_ROOT}/lib/libglfw3.a")

set(INCLUDE_DIRS
  ${Boost_INCLUDE_DIRS}
  ${PROTOBUF_INCLUDE_DIRS}
  ${GLFW_INCLUDE_DIR}
)

set(LIBRARIES
  ${Boost_LIBRARIES}
  ${PROTOBUF_LIBRARY}
  ${GLFW_LIBRARY}
)

if (PLATFORM MATCHES osx)
  list(APPEND LIBRARIES
    "-framework CoreFoundation"
    "-framework Cocoa"
    "-framework OpenGL"
    "-framework IOKit"
    "-framework CoreVideo"
  )

elseif (PLATFORM MATCHES mxe)
  list(APPEND LIBRARIES
    gdi32
    opengl32
  )
endif()

# ---

set(GLFW_SRC "$ENV{CROSS_PATH}/deps/glfw/build/src")

set(SRC_FILES
  "${GLFW_SRC}/examples/simple.c"
)

# ---

include("${CROSS_ROOT}/cmake/test.cmake")
