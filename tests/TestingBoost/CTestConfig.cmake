
if (NOT DEFINED ENV{CROSS_PATH})
  message(FATAL_ERROR "CROSS_PATH MUST BE DEFINED!")
endif()

# ---

set(CTEST_PROJECT_NAME "TestingBoost")
set(CTEST_CONFIGURATION_TYPE Release)

if (PLATFORM MATCHES osx)
  set(CTEST_CMAKE_GENERATOR "Ninja")
  set(TOOLCHAIN_FILE osx.cmake)

elseif (PLATFORM MATCHES ios)
  set(CTEST_CMAKE_GENERATOR "Xcode")
  set(TOOLCHAIN_FILE ios.xcode.cmake)
# set(CONFIGURE_ARGS "-DIOS_DEPLOYMENT_TARGET=5.1.1 -DIOS_ARCHS=armv7")

elseif (PLATFORM MATCHES android)
  set(CTEST_CMAKE_GENERATOR "Ninja")
  set(TOOLCHAIN_FILE android.cmake)

elseif (PLATFORM MATCHES emscripten)
  set(CTEST_CMAKE_GENERATOR "Ninja")
  set(TOOLCHAIN_FILE emscripten.cmake)

elseif (PLATFORM MATCHES mxe)
  set(CTEST_CMAKE_GENERATOR "Ninja")
  set(TOOLCHAIN_FILE mxe.cmake)

else()
  message(FATAL_ERROR "UNSUPPORTED PLATFORM!")
endif()

set(CONFIGURE_ARGS "${CONFIGURE_ARGS} -DCROSS_ROOT=$ENV{CROSS_PATH}/core")
set(CONFIGURE_ARGS "${CONFIGURE_ARGS} -DBOOST_ROOT=$ENV{CROSS_PATH}/deps/boost/dist/${PLATFORM}")
set(CONFIGURE_ARGS "${CONFIGURE_ARGS} -DGTEST_ROOT=$ENV{CROSS_PATH}/deps/gtest/dist/${PLATFORM}")
