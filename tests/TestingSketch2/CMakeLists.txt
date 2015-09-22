cmake_minimum_required (VERSION 3.2.2)

project(TestingSketch2)

find_package(Boost 1.58.0 REQUIRED COMPONENTS system filesystem)

#
# XXX
#
set(GLM_INCLUDE_DIR "${GLM_ROOT}/include")

set(INCLUDE_DIRS
  ${Boost_INCLUDE_DIRS}
  ${GLM_INCLUDE_DIR}
)

set(LIBRARIES
  ${Boost_LIBRARIES}
)

set(SRC_FILES
  "${CROSS_ROOT}/src/cross/CrossSketch.cpp"
  "${CROSS_ROOT}/src/cross/CrossDelegateBase.cpp"
  "${CROSS_ROOT}/src/system/SystemManagerBase.cpp"
  "${CROSS_ROOT}/src/system/DisplayHelperBase.cpp"
  "${CROSS_ROOT}/src/Timer.cpp"
  "src/main.cpp"
)

# ---

if (PLATFORM MATCHES osx|mxe)
  set(GLFW_INCLUDE_DIR "${GLFW_ROOT}/include")
  set(GLFW_LIBRARY "${GLFW_ROOT}/lib/libglfw3.a")
  set(GLFW_SRC "$ENV{CROSS_PATH}/deps/glfw/build/src")

  list(APPEND INCLUDE_DIRS
    ${GLFW_INCLUDE_DIR}
    "${GLFW_SRC}/deps"
  )

  list(APPEND LIBRARIES
    ${GLFW_LIBRARY}
  )

  list(APPEND SRC_FILES
    "${GLFW_SRC}/deps/glad.c"
    "${CROSS_ROOT}/src/desktop/CrossDelegate.cpp"
    "${CROSS_ROOT}/src/desktop/system/DisplayHelper.cpp"
    "${CROSS_ROOT}/src/desktop/system/SystemManager.cpp"
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
    gdi32
    opengl32
  )

elseif (PLATFORM MATCHES emscripten)
  list(APPEND SRC_FILES
    "${CROSS_ROOT}/src/emscripten/CrossDelegate.cpp"
    "${CROSS_ROOT}/src/emscripten/system/DisplayHelper.cpp"
    "${CROSS_ROOT}/src/emscripten/system/SystemManager.cpp"
  )

elseif (PLATFORM MATCHES ios)
  list(APPEND LIBRARIES
    "-framework GLKit"
    "-framework UIKit"
    "-framework OpenGLES"
    "-framework CoreGraphics"
    "-framework CoreVideo"
  )

  list(APPEND SRC_FILES
    "src/ios/Application.mm"
    "${CROSS_ROOT}/src/ios/CrossDelegate.mm"
    "${CROSS_ROOT}/src/ios/system/DisplayHelper.mm"
    "${CROSS_ROOT}/src/ios/system/SystemManager.mm"
    "${CROSS_ROOT}/src/ios/GLViewController.mm"
    "${CROSS_ROOT}/src/ios/CrossBridge.mm"
    "${CROSS_ROOT}/src/cocoa/NSDictionary+DefautValues.m"
    "${CROSS_ROOT}/src/cocoa/NSMutableDictionary+Merge.m"
    "${CROSS_ROOT}/src/cocoa/NSString+JSON.m"
  )

  set_source_files_properties(
    "src/main.cpp"
    PROPERTIES COMPILE_FLAGS "-x objective-c++"
  )

elseif (PLATFORM MATCHES android)
  list(APPEND SRC_FILES
    "${CROSS_ROOT}/src/android/JNI.cpp"
    "${CROSS_ROOT}/src/android/CrossDelegate.cpp"
    "${CROSS_ROOT}/src/android/system/DisplayHelper.cpp"
    "${CROSS_ROOT}/src/android/system/SystemManager.cpp"
  )
endif()

# ---

include("${CROSS_ROOT}/cmake/test.cmake")
