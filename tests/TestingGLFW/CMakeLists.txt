cmake_minimum_required (VERSION 3.2.2)

project(TestingGLFW)

find_package(Boost 1.58.0 REQUIRED COMPONENTS system filesystem)

#
# XXX
#
set(GLFW_INCLUDE_DIR "${GLFW_ROOT}/include")
set(GLFW_LIBRARY "${GLFW_ROOT}/lib/libglfw3.a")

set(INCLUDE_DIRS
  ${Boost_INCLUDE_DIRS}
  ${GLFW_INCLUDE_DIR}
)

set(LIBRARIES
  ${Boost_LIBRARIES}
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

list(APPEND INCLUDE_DIRS
  "${GLFW_SRC}/deps"
)

set(SRC_FILES
  "${GLFW_SRC}/deps/glad.c"
  "src/main.cpp"
)

# ---

include("${CROSS_ROOT}/cmake/test.cmake")
