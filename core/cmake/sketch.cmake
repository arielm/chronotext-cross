set(BOOST_ROOT "$ENV{CROSS_PATH}/tree/boost/${PLATFORM}")
set(GLFW_ROOT "$ENV{CROSS_PATH}/deps/glfw/dist/${PLATFORM}")
set(GLLOAD_ROOT "$ENV{CROSS_PATH}/deps/glload")
set(GLM_ROOT "$ENV{CROSS_PATH}/deps/glm/dist")
set(JPEG_ROOT "$ENV{CROSS_PATH}/deps/libjpeg-turbo")
set(PNG_ROOT "$ENV{CROSS_PATH}/deps/libpng/dist/${PLATFORM}")
set(LIBTESS2_ROOT "$ENV{CROSS_PATH}/deps/libtess2/dist/${PLATFORM}")

# ---

set(SKETCH ON)

list(APPEND SRC_FILES "src/main.cpp")

if (PLATFORM MATCHES ios)
  list(APPEND SRC_FILES "src/ios/Application.mm")

  set_source_files_properties(
    "src/main.cpp"
    PROPERTIES COMPILE_FLAGS "-x objective-c++"
  )

elseif (PLATFORM MATCHES android)
  list(APPEND SRC_FILES "$ENV{CROSS_PATH}/core/src/chr/android/Stub.cpp")
endif()

# ---

list(APPEND INCLUDE_DIRS "$ENV{CROSS_PATH}/core/src")
list(APPEND LIBRARIES "$ENV{CROSS_PATH}/tree/chr/${PLATFORM}/lib/${CMAKE_BUILD_TYPE}/libchr_cross.a")

list(APPEND INCLUDE_DIRS "${BOOST_ROOT}/include")
list(APPEND LIBRARIES
  "${BOOST_ROOT}/lib/libboost_system.a"
  "${BOOST_ROOT}/lib/libboost_filesystem.a"
)

list(APPEND INCLUDE_DIRS "${GLM_ROOT}/include")

set(JPEG_INCLUDE_DIR "${JPEG_ROOT}/include")
set(JPEG_LIBRARY "${JPEG_ROOT}/lib/${PLATFORM}/libjpeg.a")

set(PNG_INCLUDE_DIRS "${PNG_ROOT}/include")
set(PNG_LIBRARIES "${PNG_ROOT}/lib/libpng17.a")

set(LIBTESS2_INCLUDE_DIR "${LIBTESS2_ROOT}/include")
set(LIBTESS2_LIBRARY "${LIBTESS2_ROOT}/lib/libtess2.a")

list(APPEND INCLUDE_DIRS
  ${JPEG_INCLUDE_DIR}
  ${PNG_INCLUDE_DIRS}
  ${LIBTESS2_INCLUDE_DIR}
)

list(APPEND LIBRARIES
  ${JPEG_LIBRARY}
  ${PNG_LIBRARIES}
  ${LIBTESS2_LIBRARY}
)

# ---

if (PLATFORM MATCHES osx)
  list(APPEND INCLUDE_DIRS
    "${GLFW_ROOT}/include"
    "$ENV{CROSS_PATH}/deps/glfw/build/src/deps"
  )

  list(APPEND LIBRARIES "${GLFW_ROOT}/lib/libglfw3.a")

elseif (PLATFORM MATCHES mxe)
  list(APPEND INCLUDE_DIRS
    "${GLLOAD_ROOT}/include"
  )
endif()

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
    opengl32
  )

elseif (PLATFORM MATCHES emscripten)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -s TOTAL_MEMORY=67108864")

elseif (PLATFORM MATCHES ios)
  list(APPEND LIBRARIES
    "-framework GLKit"
    "-framework UIKit"
    "-framework OpenGLES"
    "-framework CoreGraphics"
    "-framework CoreVideo"
  )

elseif (PLATFORM MATCHES android)
  list(APPEND LIBRARIES
    log
    android
    EGL
    GLESv2
  )
endif()
