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
  "src/main.cpp"
  "src/cross/CrossSketch.cpp"
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

elseif (PLATFORM MATCHES ios)
  list(APPEND LIBRARIES
    "-framework GLKit"
    "-framework UIKit"
    "-framework OpenGLES"
    "-framework CoreGraphics"
    "-framework CoreVideo"
  )

  list(APPEND SRC_FILES
    "src/cross/CrossDelegateBase.cpp"
    "src/ios/Application.mm"
    "src/ios/GLViewController.mm"
    "src/ios/CrossBridge.mm"
    "src/ios/CrossDelegate.mm"
    "src/ios/system/DisplayHelper.mm"
    "src/ios/system/SystemManager.mm"
    "src/cocoa/NSDictionary+DefautValues.m"
    "src/cocoa/NSMutableDictionary+Merge.m"
    "src/cocoa/NSString+JSON.m"
    "src/system/SystemManagerBase.cpp"
    "src/system/DisplayHelperBase.cpp"
  )

  set_source_files_properties(
    "src/main.cpp"
    PROPERTIES COMPILE_FLAGS "-x objective-c++"
  )
endif()

# ---

include("${CROSS_ROOT}/cmake/test.cmake")
