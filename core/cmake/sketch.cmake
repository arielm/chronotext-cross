set(BOOST_ROOT "$ENV{CROSS_PATH}/tree/boost/${PLATFORM}")
set(GLFW_ROOT "$ENV{CROSS_PATH}/tree/glfw/${PLATFORM}")
set(GLLOAD_ROOT "$ENV{CROSS_PATH}/tree/glload")
set(GLM_ROOT "$ENV{CROSS_PATH}/tree/glm")
set(JPEG_ROOT "$ENV{CROSS_PATH}/tree/libjpeg-turbo")
set(PNG_ROOT "$ENV{CROSS_PATH}/tree/libpng/${PLATFORM}")
set(LIBTESS2_ROOT "$ENV{CROSS_PATH}/tree/libtess2/${PLATFORM}")
set(PROTOBUF_ROOT "$ENV{CROSS_PATH}/tree/protobuf/${PLATFORM}")

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

if (PLATFORM MATCHES rpi)
  find_package(Boost COMPONENTS system filesystem REQUIRED)
  find_package(OpenGL REQUIRED)

  list(APPEND INCLUDE_DIRS
    ${Boost_INCLUDE_DIRS}
    "${JPEG_ROOT}/rpi/include"
  )

  list(APPEND LIBRARIES
    ${Boost_LIBRARIES}
    OpenGL::GL
    -ldl
    -lpthread
    -lpng16
    "${JPEG_ROOT}/rpi/lib/libturbojpeg.a"
  )
else()
  list(APPEND INCLUDE_DIRS "${BOOST_ROOT}/include")
  list(APPEND LIBRARIES
    "${BOOST_ROOT}/lib/libboost_system.a"
    "${BOOST_ROOT}/lib/libboost_filesystem.a"
  )

  list(APPEND INCLUDE_DIRS "${JPEG_ROOT}/include")
  list(APPEND LIBRARIES "${JPEG_ROOT}/lib/${PLATFORM}/libjpeg.a")

  list(APPEND INCLUDE_DIRS "${PNG_ROOT}/include")
  list(APPEND LIBRARIES "${PNG_ROOT}/lib/libpng17.a")
endif()

list(APPEND INCLUDE_DIRS "${GLM_ROOT}/include")

list(APPEND INCLUDE_DIRS "${LIBTESS2_ROOT}/include")
list(APPEND LIBRARIES "${LIBTESS2_ROOT}/lib/libtess2.a")

list(APPEND INCLUDE_DIRS "${PROTOBUF_ROOT}/include")
list(APPEND LIBRARIES "${PROTOBUF_ROOT}/lib/libprotobuf.a")

# ---

if (PLATFORM MATCHES osx|rpi)
  list(APPEND INCLUDE_DIRS
    "${GLFW_ROOT}/include"
    "$ENV{CROSS_PATH}/tree/glfw/src/deps"
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
    "-framework AudioToolbox"
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
    "-framework AudioToolbox"
    "-framework AVFoundation"
  )

elseif (PLATFORM MATCHES android)
  list(APPEND LIBRARIES
    log
    android
    EGL
    GLESv2
  )
endif()
