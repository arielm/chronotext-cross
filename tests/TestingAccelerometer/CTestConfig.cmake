set(CTEST_PROJECT_NAME TestingAccelerometer)
set(CTEST_CONFIGURATION_TYPE Release)

if (PLATFORM MATCHES ios)
  set(ARGS
    -DRUN=APP
    -DIOS_ARCHS=arm64
  )

elseif (PLATFORM MATCHES android)
  set(ARGS
    -DRUN=APK
  )
endif()

include("$ENV{CROSS_PATH}/core/cmake/platforms.cmake")
