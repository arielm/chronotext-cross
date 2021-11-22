
if (NOT CMAKE_GENERATOR STREQUAL "Xcode")
  message(FATAL_ERROR "UNSUPPORTED GENERATOR!")
endif()

set(IOS_ARCHS
  arm64
  CACHE STRING "ios_archs"
)

set(IOS_DEPLOYMENT_TARGET 11.0
  CACHE STRING "ios_deployment_target"
)

#
# SAVING TIME AND UNNECESSARY TROUBLE
#
set(CMAKE_C_COMPILER_FORCED ON)
set(CMAKE_CXX_COMPILER_FORCED ON)

# ---

macro (SET_ARCHS archs)
  set(VALID_ARCHS "")

  foreach (arch ${archs})
    set(VALID_ARCHS "${arch} ${VALID_ARCHS}")
  endforeach()

  set(CMAKE_XCODE_ATTRIBUTE_VALID_ARCHS "${VALID_ARCHS}")
  set(CMAKE_XCODE_ATTRIBUTE_ONLY_ACTIVE_ARCH NO) # NECESSARY WHEN SEVERAL ARCHITECTURES MUST BE BUILT
endmacro()

# ---

set(CMAKE_MACOSX_BUNDLE YES) # NECESSARY, OTHERWISE add_executable() IN CMakeLists.txt WON'T HAVE ANY EFFECT

set(CMAKE_OSX_SYSROOT iphoneos)
set(CMAKE_XCODE_ATTRIBUTE_CODE_SIGN_IDENTITY "iPhone Developer")
set(CMAKE_XCODE_ATTRIBUTE_DEVELOPMENT_TEAM "$ENV{IOS_DEV_TEAM}")

SET_ARCHS("${IOS_ARCHS}")

set(CMAKE_XCODE_ATTRIBUTE_TARGETED_DEVICE_FAMILY "1,2")
set(CMAKE_XCODE_ATTRIBUTE_IPHONEOS_DEPLOYMENT_TARGET ${IOS_DEPLOYMENT_TARGET})
set(CMAKE_XCODE_ATTRIBUTE_CLANG_CXX_LIBRARY "libc++")
set(CMAKE_XCODE_ATTRIBUTE_CLANG_CXX_LANGUAGE_STANDARD "c++14")
