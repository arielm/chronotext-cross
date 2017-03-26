cmake_minimum_required(VERSION 3.2.2)

project(chr_cross)

set(TREE "../../tree")

set(BOOST_ROOT "${TREE}/boost/${PLATFORM}")
set(GLFW_ROOT "${TREE}/glfw/${PLATFORM}")
set(GLLOAD_ROOT "${TREE}/glload")
set(GLM_ROOT "${TREE}/glm")
set(JPEG_ROOT "${TREE}/libjpeg-turbo")
set(PNG_ROOT "${TREE}/libpng/${PLATFORM}")
set(LIBTESS2_ROOT "${TREE}/libtess2/${PLATFORM}")
set(PROTOBUF_ROOT "${TREE}/protobuf/${PLATFORM}")

# ---

list(APPEND INCLUDE_DIRS
  "${CMAKE_CURRENT_SOURCE_DIR}"
)

list(APPEND SRC_FILES
  "chr/Log.cpp"
  "chr/InputSource.cpp"
  "chr/OutputTarget.cpp"
  "chr/FileSystem.cpp"
  "chr/ResourceBuffer.cpp"
  "chr/cross/CrossSketch.cpp"
  "chr/cross/CrossDelegateBase.cpp"
  "chr/system/SystemManagerBase.cpp"
  "chr/system/DisplayHelperBase.cpp"
  "chr/Random.cpp"
  "chr/Timer.cpp"
  "chr/time/Clock.cpp"
  "chr/time/FrameClock.cpp"
  "chr/utils/Utils.cpp"
  "chr/gl/Utils.cpp"
  "chr/gl/ShaderProgram.cpp"
  "chr/gl/ShaderHelper.cpp"
  "chr/gl/Camera.cpp"
  "chr/gl/Matrix.cpp"
  "chr/gl/Texture.cpp"
  "chr/gl/State.cpp"
  "chr/gl/Buffer.cpp"
  "chr/gl/Vertex.cpp"
  "chr/gl/Triangulator.cpp"
  "chr/gl/Revolver.cpp"
  "chr/gl/TextureManager.cpp"
  "chr/gl/draw/Circle.cpp"
  "chr/gl/draw/Rect.cpp"
  "chr/gl/draw/Sprite.cpp"
  "chr/gl/draw/Cube.cpp"
  "chr/gl/draw/Box.cpp"
  "chr/gl/shaders/ColorShader.cpp"
  "chr/gl/shaders/LambertShader.cpp"
  "chr/gl/shaders/TextureAlphaShader.cpp"
  "chr/gl/shaders/TextureShader.cpp"
  "chr/path/ASPC.cpp"
  "chr/path/Path.cpp"
  "chr/path/Shape.cpp"
  "chr/path/FollowablePath2D.cpp"
  "chr/path/FollowablePath3D.cpp"
  "chr/path/SVGDocument.cpp"
  "chr/math/MatrixAffine.cpp"
  "chr/math/Utils.cpp"
  "chr/math/Rect.cpp"
  "chr/math/Box.cpp"
  "chr/math/Plane.cpp"
  "chr/math/Frustum.cpp"
  "chr/image/Utils.cpp"
  "chr/io/BinaryInputStream.cpp"
  "chr/io/BinaryOutputStream.cpp"
)

if (PLATFORM MATCHES osx)
  list(APPEND SRC_FILES
    "chr/glfw/CrossDelegate.cpp"
    "chr/glfw/system/DisplayHelper.cpp"
    "chr/osx/system/SystemManager.cpp"
  )

elseif (PLATFORM MATCHES mxe)
  list(APPEND SRC_FILES
    "chr/win/CrossDelegate.cpp"
    "chr/win/system/DisplayHelper.cpp"
    "chr/win/system/SystemManager.cpp"
  )

elseif (PLATFORM MATCHES emscripten)
  list(APPEND SRC_FILES
    "chr/emscripten/CrossDelegate.cpp"
    "chr/emscripten/system/DisplayHelper.cpp"
    "chr/emscripten/system/SystemManager.cpp"
  )

elseif (PLATFORM MATCHES ios)
  list(APPEND SRC_FILES
    "chr/ios/CrossDelegate.mm"
    "chr/ios/system/DisplayHelper.mm"
    "chr/ios/system/SystemManager.mm"
    "chr/ios/GLViewController.mm"
    "chr/ios/CrossBridge.mm"
    "chr/cocoa/NSDictionary+DefautValues.m"
    "chr/cocoa/NSMutableDictionary+Merge.m"
    "chr/cocoa/NSString+JSON.m"
  )

elseif (PLATFORM MATCHES android)
  list(APPEND SRC_FILES
    "chr/android/JNI.cpp"
    "chr/android/CrossBridge.cpp"
    "chr/android/CrossDelegate.cpp"
    "chr/android/system/DisplayHelper.cpp"
    "chr/android/system/SystemManager.cpp"
  )
endif()

# ---

if (PLATFORM MATCHES osx)
  set_source_files_properties(
    "chr/osx/system/SystemManager.cpp"
    PROPERTIES
    COMPILE_FLAGS "-x objective-c++"
  )
endif()

# ---

if (PLATFORM MATCHES osx)
  set(GLFW_SRC "${TREE}/glfw/src")
  set(NFD_SRC "nfd")

  list(APPEND INCLUDE_DIRS
    "${GLFW_ROOT}/include"
    "${GLFW_SRC}/deps"
    "${NFD_SRC}"
  )

  list(APPEND SRC_FILES
    "${GLFW_SRC}/deps/glad.c"
    "${NFD_SRC}/nfd_common.c"
    "${NFD_SRC}/nfd_cocoa.m"
  )

  list(APPEND LIBRARIES "${GLFW_ROOT}/lib/libglfw3.a")

elseif (PLATFORM MATCHES mxe)
  set(GLLOAD_SRC "${GLLOAD_ROOT}/source")
  set(NFD_SRC "nfd")

  list(APPEND INCLUDE_DIRS
    "${GLLOAD_ROOT}/include"
    "${NFD_SRC}"
  )

  list(APPEND SRC_FILES
    "${GLLOAD_SRC}/gl_load.c"
    "${GLLOAD_SRC}/wgl_load.c"
    "${GLLOAD_SRC}/gl_load_cpp.cpp"
    "${GLLOAD_SRC}/wgl_load_cpp.cpp"
    "${NFD_SRC}/nfd_common.c"
    "${NFD_SRC}/nfd_win.cpp"
  )
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
    opengl32
  )

elseif (PLATFORM MATCHES ios)
  add_definitions(-DCHR_RUN_APP -DCHR_FS_BUNDLE)
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fembed-bitcode")
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

list(APPEND INCLUDE_DIRS "${PROTOBUF_ROOT}/include")
list(APPEND LIBRARIES "${PROTOBUF_ROOT}/lib/libprotobuf.a")

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
