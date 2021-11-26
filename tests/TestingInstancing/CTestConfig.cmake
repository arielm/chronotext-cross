
set(CTEST_PROJECT_NAME TestingInstancing)
set(CTEST_CONFIGURATION_TYPE Release)

if (PLATFORM MATCHES osx|rpi|linux)
  set(ARGS
    -DRUN=EXE
  )
elseif (PLATFORM MATCHES emscripten)
  set(ARGS
    -DRUN=BROWSER
  )
elseif (PLATFORM MATCHES ios)
  set(ARGS
    -DRUN=APP
    -DIOS_ARCHS=arm64
  )
elseif (PLATFORM MATCHES android)
  set(ARGS
    -DRUN=APK
  )
elseif (PLATFORM MATCHES mxe)
  set(ARGS
    -DRUN=EXE
    -DFS=RC
  )
endif()

include("$ENV{CROSS_PATH}/core/cmake/platforms.cmake")
