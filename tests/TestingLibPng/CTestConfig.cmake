set(CTEST_PROJECT_NAME TestingLibPng)
set(CTEST_CONFIGURATION_TYPE Release)

if (PLATFORM MATCHES osx|linux)
  set(ARGS
    -DRUN=EXE
  )

elseif (PLATFORM MATCHES ios)
  set(ARGS
    -DRUN=APP
    -DIOS_ARCHS=arm64
  )

elseif (PLATFORM MATCHES emscripten)
  set(ARGS
    -DRUN=BROWSER
  )
endif()

include("$ENV{CROSS_PATH}/core/cmake/platforms.cmake")
