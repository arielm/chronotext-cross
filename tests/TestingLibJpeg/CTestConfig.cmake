
set(CTEST_PROJECT_NAME TestingLibJpeg)
set(CTEST_CONFIGURATION_TYPE Release)

if (PLATFORM MATCHES osx)
  set(ARGS
    -DRUN=EXE
  )

elseif (PLATFORM MATCHES ios)
  set(ARGS
    -DRUN=APP
    -DIOS_ARCHS=arm64
#   -DIOS_DEPLOYMENT_TARGET=5.1.1
#   -DIOS_ARCHS=armv7
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

elseif (PLATFORM MATCHES emscripten)
  set(ARGS
    -DRUN=BROWSER
  )
endif()

list(APPEND ARGS
  "-DBOOST_ROOT=$ENV{CROSS_PATH}/deps/boost/dist/${PLATFORM}"
  "-DGLFW_ROOT=$ENV{CROSS_PATH}/deps/glfw/dist/${PLATFORM}"
  "-DGLM_ROOT=$ENV{CROSS_PATH}/deps/glm/dist"
  "-DJPEG_ROOT=$ENV{CROSS_PATH}/deps/libjpeg-turbo/dist/${PLATFORM}"
)

include("${CROSS_ROOT}/cmake/platforms.cmake")