set(BOOST_ROOT "$ENV{CROSS_PATH}/tree/boost")
set(GLFW_ROOT "$ENV{CROSS_PATH}/tree/glfw/${PLATFORM}")
set(GLLOAD_ROOT "$ENV{CROSS_PATH}/tree/glload")
set(GLM_ROOT "$ENV{CROSS_PATH}/tree/glm")
set(JPEG_ROOT "$ENV{CROSS_PATH}/tree/libjpeg-turbo")
set(PNG_ROOT "$ENV{CROSS_PATH}/tree/libpng")
set(LIBTESS2_ROOT "$ENV{CROSS_PATH}/tree/libtess2")
set(PROTOBUF_ROOT "$ENV{CROSS_PATH}/tree/protobuf")
set(STB_ROOT "$ENV{CROSS_PATH}/tree/stb")

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

if (PLATFORM MATCHES android|android64|rpi|rpi64|linux|emscripten)
  list(APPEND INCLUDE_DIRS "${JPEG_ROOT}/${PLATFORM}/include")
  list(APPEND LIBRARIES "${JPEG_ROOT}/${PLATFORM}/lib/libturbojpeg.a")
else()
  list(APPEND INCLUDE_DIRS "${JPEG_ROOT}/include")
  list(APPEND LIBRARIES "${JPEG_ROOT}/lib/${PLATFORM}/libjpeg.a")
  endif()

if (PLATFORM MATCHES rpi|rpi64|linux|mxe)
  find_package(Boost COMPONENTS system filesystem REQUIRED)

  list(APPEND INCLUDE_DIRS ${Boost_INCLUDE_DIRS})
  list(APPEND LIBRARIES ${Boost_LIBRARIES})

elseif (PLATFORM MATCHES android|android64)
  list(APPEND INCLUDE_DIRS "${BOOST_ROOT}/android/include")
  list(APPEND LIBRARIES
    "${BOOST_ROOT}/android/${ANDROID_ABI}/libboost_system.a"
    "${BOOST_ROOT}/android/${ANDROID_ABI}/libboost_filesystem.a"
  )

else()
  list(APPEND INCLUDE_DIRS "${BOOST_ROOT}/${PLATFORM}/include")
  list(APPEND LIBRARIES
    "${BOOST_ROOT}/${PLATFORM}/lib/libboost_system.a"
    "${BOOST_ROOT}/${PLATFORM}/lib/libboost_filesystem.a"
  )
endif()

if (PLATFORM MATCHES rpi|rpi64|linux)
  list(APPEND LIBRARIES -lpng16)

elseif (PLATFORM MATCHES android|android64)
  list(APPEND INCLUDE_DIRS "${PNG_ROOT}/android/include")
  list(APPEND LIBRARIES "${PNG_ROOT}/android/${ANDROID_ABI}/libpng17.a")

else()
  list(APPEND INCLUDE_DIRS "${PNG_ROOT}/${PLATFORM}/include")
  list(APPEND LIBRARIES "${PNG_ROOT}/${PLATFORM}/lib/libpng17.a")
endif()

list(APPEND INCLUDE_DIRS
  "${GLM_ROOT}/include"
  "${STB_ROOT}"
)

if (PLATFORM MATCHES android|android64)
  list(APPEND INCLUDE_DIRS
    "${LIBTESS2_ROOT}/android/include"
    "${PROTOBUF_ROOT}/android/include"
  )

  list(APPEND LIBRARIES "${LIBTESS2_ROOT}/android/${ANDROID_ABI}/libtess2.a")
  list(APPEND LIBRARIES "${PROTOBUF_ROOT}/android/${ANDROID_ABI}/libprotobuf.a")

else()
  list(APPEND INCLUDE_DIRS "${LIBTESS2_ROOT}/${PLATFORM}/include")
  list(APPEND LIBRARIES "${LIBTESS2_ROOT}/${PLATFORM}/lib/libtess2.a")

  list(APPEND INCLUDE_DIRS "${PROTOBUF_ROOT}/${PLATFORM}/include")
  list(APPEND LIBRARIES "${PROTOBUF_ROOT}/${PLATFORM}/lib/libprotobuf.a")
endif()

# ---

if (PLATFORM MATCHES linux)
  find_package(OpenGL REQUIRED)
  list(APPEND LIBRARIES
    OpenGL::GL
  )
elseif (PLATFORM MATCHES rpi|rpi64)
  find_package(OpenGL REQUIRED)
  list(APPEND LIBRARIES
    OpenGL::GLU
    OpenGL::GL
  )
endif()

if (PLATFORM MATCHES osx|rpi|rpi64|linux)
  list(APPEND INCLUDE_DIRS
    "${GLFW_ROOT}/include"
    "$ENV{CROSS_PATH}/tree/glfw/src/deps"
  )

  list(APPEND LIBRARIES
    "${GLFW_ROOT}/lib/libglfw3.a"
    -ldl
    -lpthread
  )

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
    "$ENV{CROSS_PATH}/tree/syphon/Syphon.framework"
  )

elseif (PLATFORM MATCHES mxe)
  list(APPEND LIBRARIES
    opengl32
  )

elseif (PLATFORM MATCHES emscripten)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -s ERROR_ON_UNDEFINED_SYMBOLS=0 -s TOTAL_MEMORY=67108864")

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
    GLESv3
  )
endif()
