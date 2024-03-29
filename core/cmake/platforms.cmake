
#
# FOR "PACKAGES" TO BE FOUND WHEN CROSS-COMPILING (ANDROID, EMSCRIPTEN, MXE...)
# REQUIRED: ALL THE "PACKAGES" MUST BE LOCATED UNDER THE "HOME" PATH
#
set(STAGING_PREFIX "$ENV{HOME}")

# ---

if (PLATFORM MATCHES osx)
  set(CTEST_CMAKE_GENERATOR "Ninja")
  set(TOOLCHAIN_FILE "${CMAKE_CURRENT_LIST_DIR}/toolchains/osx.cmake")

elseif (PLATFORM MATCHES ios)
  set(CTEST_CMAKE_GENERATOR "Xcode")
  set(TOOLCHAIN_FILE "${CMAKE_CURRENT_LIST_DIR}/toolchains/ios.xcode.cmake")

elseif (PLATFORM MATCHES android64)
  set(CTEST_CMAKE_GENERATOR "Ninja")
  set(TOOLCHAIN_FILE "${CMAKE_CURRENT_LIST_DIR}/toolchains/android64.cmake")

elseif (PLATFORM MATCHES android)
  set(CTEST_CMAKE_GENERATOR "Ninja")
  set(TOOLCHAIN_FILE "${CMAKE_CURRENT_LIST_DIR}/toolchains/android.cmake")

elseif (PLATFORM MATCHES emscripten)
  set(CTEST_CMAKE_GENERATOR "Ninja")
  set(TOOLCHAIN_FILE "${CMAKE_CURRENT_LIST_DIR}/toolchains/emscripten.cmake")

elseif (PLATFORM MATCHES mxe)
  set(CTEST_CMAKE_GENERATOR "Ninja")
  set(TOOLCHAIN_FILE "${CMAKE_CURRENT_LIST_DIR}/toolchains/mxe.cmake")

elseif (PLATFORM MATCHES rpi64)
  set(CTEST_CMAKE_GENERATOR "Ninja")
  set(TOOLCHAIN_FILE "${CMAKE_CURRENT_LIST_DIR}/toolchains/rpi64.cmake")

elseif (PLATFORM MATCHES rpi)
  set(CTEST_CMAKE_GENERATOR "Ninja")
  set(TOOLCHAIN_FILE "${CMAKE_CURRENT_LIST_DIR}/toolchains/rpi.cmake")
  
elseif (PLATFORM MATCHES linux)
  set(CTEST_CMAKE_GENERATOR "Ninja")
  set(TOOLCHAIN_FILE "${CMAKE_CURRENT_LIST_DIR}/toolchains/linux.cmake")

else()
  message(FATAL_ERROR "UNSUPPORTED PLATFORM!")
endif()
