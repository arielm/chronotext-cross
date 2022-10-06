set(CTEST_PROJECT_NAME TestingLighting)
set(CTEST_CONFIGURATION_TYPE Release)

if (PLATFORM MATCHES osx|linux)
  set(ARGS
    -DRUN=EXE
  )

elseif (PLATFORM MATCHES emscripten)
  set(ARGS
    -DRUN=BROWSER
  )
endif()

include("$ENV{CROSS_PATH}/core/cmake/platforms.cmake")
