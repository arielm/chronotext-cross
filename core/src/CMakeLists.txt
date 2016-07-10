cmake_minimum_required(VERSION 3.2.2)

project(chr_cross)

set(DEPS "../../deps")

set(BOOST_ROOT "${DEPS}/boost/dist/${PLATFORM}")
set(GLFW_ROOT "${DEPS}/glfw/dist/${PLATFORM}")
set(GLM_ROOT "${DEPS}/glm/dist")
set(JPEG_ROOT "${DEPS}/libjpeg-turbo")
set(PNG_ROOT "${DEPS}/libpng/dist/${PLATFORM}")
set(LIBTESS2_ROOT "${DEPS}/libtess2/dist/${PLATFORM}")

# ---

list(APPEND INCLUDE_DIRS
  "${CMAKE_CURRENT_SOURCE_DIR}"
)

list(APPEND SRC_FILES
  "Log.cpp"
  "InputSource.cpp"
  "FileSystem.cpp"
  "MemoryBuffer.cpp"
  "cross/CrossSketch.cpp"
  "cross/CrossDelegateBase.cpp"
  "system/SystemManagerBase.cpp"
  "system/DisplayHelperBase.cpp"
  "Random.cpp"
  "Timer.cpp"
  "time/Clock.cpp"
  "time/FrameClock.cpp"
  "utils/Utils.cpp"
  "gl/Utils.cpp"
  "gl/ShaderProgram.cpp"
  "gl/ShaderHelper.cpp"
  "gl/Camera.cpp"
  "gl/Matrix.cpp"
  "gl/Texture.cpp"
  "gl/State.cpp"
  "gl/Buffer.cpp"
  "gl/Vertex.cpp"
  "gl/Triangulator.cpp"
  "gl/Revolver.cpp"
  "gl/draw/Circle.cpp"
  "gl/draw/Rect.cpp"
  "gl/draw/Sprite.cpp"
  "gl/draw/Cube.cpp"
  "gl/draw/Box.cpp"
  "gl/shaders/ColorShader.cpp"
  "gl/shaders/LambertShader.cpp"
  "gl/shaders/TextureAlphaShader.cpp"
  "gl/shaders/TextureShader.cpp"
  "path/ASPC.cpp"
  "path/Path.cpp"
  "path/Shape.cpp"
  "path/FollowablePath2D.cpp"
  "path/FollowablePath3D.cpp"
  "path/SVGDocument.cpp"
  "math/MatrixAffine.cpp"
  "math/Utils.cpp"
  "math/Rect.cpp"
  "math/Box.cpp"
  "image/Utils.cpp"
)

if (PLATFORM MATCHES osx|mxe)
  list(APPEND SRC_FILES
    "desktop/CrossDelegate.cpp"
    "desktop/system/DisplayHelper.cpp"
    "desktop/system/SystemManager.cpp"
  )

elseif (PLATFORM MATCHES emscripten)
  list(APPEND SRC_FILES
    "emscripten/CrossDelegate.cpp"
    "emscripten/system/DisplayHelper.cpp"
    "emscripten/system/SystemManager.cpp"
  )

elseif (PLATFORM MATCHES ios)
  list(APPEND SRC_FILES
    "ios/CrossDelegate.mm"
    "ios/system/DisplayHelper.mm"
    "ios/system/SystemManager.mm"
    "ios/GLViewController.mm"
    "ios/CrossBridge.mm"
    "cocoa/NSDictionary+DefautValues.m"
    "cocoa/NSMutableDictionary+Merge.m"
    "cocoa/NSString+JSON.m"
  )

elseif (PLATFORM MATCHES android)
  list(APPEND SRC_FILES
    "android/JNI.cpp"
    "android/CrossBridge.cpp"
    "android/CrossDelegate.cpp"
    "android/system/DisplayHelper.cpp"
    "android/system/SystemManager.cpp"
  )
endif()

# ---

if (PLATFORM MATCHES osx|mxe)
  set(GLFW_SRC "${DEPS}/glfw/build/src")

  list(APPEND INCLUDE_DIRS
    "${GLFW_ROOT}/include"
    "${GLFW_SRC}/deps"
  )

  list(APPEND SRC_FILES "${GLFW_SRC}/deps/glad.c")
  list(APPEND LIBRARIES "${GLFW_ROOT}/lib/libglfw3.a")
endif()

if (PLATFORM MATCHES osx)
  add_definitions(-DCHR_RUN_EXE)

  list(APPEND LIBRARIES
  	"-framework Foundation"
    "-framework CoreFoundation"
    "-framework Cocoa"
    "-framework OpenGL"
    "-framework IOKit"
    "-framework CoreVideo"
  )

elseif (PLATFORM MATCHES mxe)
  add_definitions(-DCHR_RUN_EXE -DCHR_FS_RC)

  list(APPEND LIBRARIES
    gdi32
    opengl32
  )

elseif (PLATFORM MATCHES ios)
  add_definitions(-DCHR_RUN_APP -DCHR_FS_BUNDLE)
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
