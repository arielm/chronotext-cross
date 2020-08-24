
set(CTEST_PROJECT_NAME TestingCPP14)
set(CTEST_CONFIGURATION_TYPE Release)

if (PLATFORM MATCHES osx|rpi)
  set(ARGS
    -DRUN=EXE
  )

elseif (PLATFORM MATCHES ios)
  set(ARGS
    -DRUN=TEST
    -DIOS_ARCHS=arm64
#   -DIOS_DEPLOYMENT_TARGET=5.1.1
#   -DIOS_ARCHS=armv7
  )

elseif (PLATFORM MATCHES android)
  set(ARGS
    -DRUN=EXE
  )

elseif (PLATFORM MATCHES emscripten)
  set(ARGS
    -DRUN=NODE
  )

elseif (PLATFORM MATCHES mxe)
  set(ARGS
    -DRUN=EXE
  )
endif()

include("$ENV{CROSS_PATH}/core/cmake/platforms.cmake")
