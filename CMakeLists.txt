cmake_minimum_required(VERSION 3.2.2)

project(chr_cross)

set(BOOST_ROOT "deps/boost/dist/${PLATFORM}")
set(GLFW_ROOT "deps/glfw/dist/${PLATFORM}")
set(GLM_ROOT "deps/glm/dist")
set(JPEG_ROOT "deps/libjpeg-turbo")
set(PNG_ROOT "deps/libpng/dist/${PLATFORM}")
set(LIBTESS2_ROOT "deps/libtess2/dist/${PLATFORM}")

# ---

list(APPEND INCLUDE_DIRS
  "core/src"
)

list(APPEND SRC_FILES
  "core/src/Log.cpp"
  "core/src/InputSource.cpp"
  "core/src/FileSystem.cpp"
  "core/src/MemoryBuffer.cpp"
  "core/src/cross/CrossSketch.cpp"
  "core/src/cross/CrossDelegateBase.cpp"
  "core/src/system/SystemManagerBase.cpp"
  "core/src/system/DisplayHelperBase.cpp"
  "core/src/Random.cpp"
  "core/src/Timer.cpp"
  "core/src/time/Clock.cpp"
  "core/src/time/FrameClock.cpp"
  "core/src/utils/Utils.cpp"
  "core/src/gl/Utils.cpp"
  "core/src/gl/ShaderProgram.cpp"
  "core/src/gl/ShaderHelper.cpp"
  "core/src/gl/Camera.cpp"
  "core/src/gl/Matrix.cpp"
  "core/src/gl/Texture.cpp"
  "core/src/gl/State.cpp"
  "core/src/gl/Buffer.cpp"
  "core/src/gl/Vertex.cpp"
  "core/src/gl/Triangulator.cpp"
  "core/src/gl/Revolver.cpp"
  "core/src/gl/draw/Circle.cpp"
  "core/src/gl/draw/Rect.cpp"
  "core/src/gl/draw/Sprite.cpp"
  "core/src/gl/draw/Cube.cpp"
  "core/src/gl/draw/Box.cpp"
  "core/src/gl/shaders/ColorShader.cpp"
  "core/src/gl/shaders/LambertShader.cpp"
  "core/src/gl/shaders/TextureAlphaShader.cpp"
  "core/src/gl/shaders/TextureShader.cpp"
  "core/src/path/ASPC.cpp"
  "core/src/path/Path.cpp"
  "core/src/path/Shape.cpp"
  "core/src/path/FollowablePath2D.cpp"
  "core/src/path/FollowablePath3D.cpp"
  "core/src/path/SVGDocument.cpp"
  "core/src/math/MatrixAffine.cpp"
  "core/src/math/Utils.cpp"
  "core/src/math/Rect.cpp"
  "core/src/math/Box.cpp"
  "core/src/image/Utils.cpp"
)

if (PLATFORM MATCHES osx|mxe)
  list(APPEND SRC_FILES
    "core/src/desktop/CrossDelegate.cpp"
    "core/src/desktop/system/DisplayHelper.cpp"
    "core/src/desktop/system/SystemManager.cpp"
  )

elseif (PLATFORM MATCHES emscripten)
  list(APPEND SRC_FILES
    "core/src/emscripten/CrossDelegate.cpp"
    "core/src/emscripten/system/DisplayHelper.cpp"
    "core/src/emscripten/system/SystemManager.cpp"
  )

elseif (PLATFORM MATCHES ios)
  list(APPEND SRC_FILES
    "core/src/ios/CrossDelegate.mm"
    "core/src/ios/system/DisplayHelper.mm"
    "core/src/ios/system/SystemManager.mm"
    "core/src/ios/GLViewController.mm"
    "core/src/ios/CrossBridge.mm"
    "core/src/cocoa/NSDictionary+DefautValues.m"
    "core/src/cocoa/NSMutableDictionary+Merge.m"
    "core/src/cocoa/NSString+JSON.m"
  )

elseif (PLATFORM MATCHES android)
  list(APPEND SRC_FILES
    "core/src/android/JNI.cpp"
    "core/src/android/CrossBridge.cpp"
    "core/src/android/CrossDelegate.cpp"
    "core/src/android/system/DisplayHelper.cpp"
    "core/src/android/system/SystemManager.cpp"
  )
endif()

# ---

if (PLATFORM MATCHES osx|mxe)
  set(GLFW_SRC "deps/glfw/build/src")

  list(APPEND INCLUDE_DIRS
    "${GLFW_ROOT}/include"
    "${GLFW_SRC}/deps"
  )

  list(APPEND SRC_FILES "${GLFW_SRC}/deps/glad.c")
  list(APPEND LIBRARIES "${GLFW_ROOT}/lib/libglfw3.a")
endif()

if (PLATFORM MATCHES osx)
  list(APPEND LIBRARIES
  	"-framework Foundation"
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

elseif (PLATFORM MATCHES ios)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fembed-bitcode")

  list(APPEND LIBRARIES
  	"-framework Foundation"
    "-framework GLKit"
    "-framework UIKit"
    "-framework OpenGLES"
    "-framework CoreGraphics"
    "-framework CoreVideo"
  )

elseif (PLATFORM MATCHES android)
  add_definitions(-DCHR_RUN_APK -DCHR_FS_APK)

  list(APPEND LIBRARIES
    log
    android
    EGL
    GLESv2
  )
endif()

# ---

list(APPEND INCLUDE_DIRS "${BOOST_ROOT}/include")
list(APPEND LIBRARIES
  "${BOOST_ROOT}/lib/libboost_system.a"
  "${BOOST_ROOT}/lib/libboost_filesystem.a"
)

list(APPEND INCLUDE_DIRS "${GLM_ROOT}/include")

list(APPEND INCLUDE_DIRS "${JPEG_ROOT}/include")
list(APPEND LIBRARIES "${JPEG_ROOT}/lib/${PLATFORM}/libjpeg.a")

list(APPEND INCLUDE_DIRS "${PNG_ROOT}/include")
list(APPEND LIBRARIES "${PNG_ROOT}/lib/libpng17.a")

list(APPEND INCLUDE_DIRS "${LIBTESS2_ROOT}/include")
list(APPEND LIBRARIES "${LIBTESS2_ROOT}/lib/libtess2.a")

# ---

if (PLATFORM MATCHES emscripten)
  set(ZLIB_INCLUDE_DIRS "$ENV{HOME}/.emscripten_cache/ports-builds/zlib")
  set(ZLIB_LIBRARIES "$ENV{HOME}/.emscripten_cache/zlib.bc")

elseif (PLATFORM MATCHES ios)
  set(ZLIB_INCLUDE_DIRS "/Applications/Xcode.app/Contents/Developer/Platforms/iPhoneOS.platform/Developer/SDKs/iPhoneOS.sdk/usr/include")
  set(ZLIB_LIBRARIES "/Applications/Xcode.app/Contents/Developer/Platforms/iPhoneOS.platform/Developer/SDKs/iPhoneOS.sdk/usr/lib/libz.tbd")

else()
  find_package(ZLIB)
endif()

list(APPEND INCLUDE_DIRS ${ZLIB_INCLUDE_DIRS})
list(APPEND LIBRARIES ${ZLIB_LIBRARIES})

# ---

include_directories(${INCLUDE_DIRS})
add_library(${PROJECT_NAME} STATIC ${SRC_FILES})
target_link_libraries(${PROJECT_NAME} ${LIBRARIES})
