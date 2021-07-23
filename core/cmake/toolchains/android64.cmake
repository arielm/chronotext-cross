
if (NOT DEFINED ENV{NDK_PATH})
  message(FATAL_ERROR "NDK_PATH MUST BE DEFINED!")
endif()

set(ANDROID_NDK "$ENV{NDK_PATH}"
  CACHE STRING "android_ndk"
)

set(ANDROID_ABI "arm64-v8a"
  CACHE STRING "android_abi"
)

set(ANDROID_API android-21
  CACHE STRING "android_api"
)

# ---

set(ANDROID_NATIVE_API_LEVEL ${ANDROID_API})

#
# TODO: AVOID DOUBLE-INCLUSION OF FLAGS
#
set(CMAKE_CXX_FLAGS "-std=c++1y"
  CACHE STRING "cmake_cxx_flags/android"
)

include("$ENV{CROSS_PATH}/core/cmake/toolchains/android.toolchain.cmake")
