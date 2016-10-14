
set(CTEST_PROJECT_NAME TestingWebSocket)
set(CTEST_CONFIGURATION_TYPE Release)

if (PLATFORM MATCHES osx)
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
    -DRUN=APK
  )

elseif (PLATFORM MATCHES emscripten)
  set(ARGS
    -DRUN=SPIDERMONKEY
#   -DRUN=BROWSER
  )

elseif (PLATFORM MATCHES mxe)
  set(ARGS
    -DRUN=EXE
    -DFS=RC
  )
endif()

list(APPEND ARGS
  "-DBOOST_ROOT=$ENV{CROSS_PATH}/deps/boost/dist/${PLATFORM}"
  "-DGTEST_ROOT=$ENV{CROSS_PATH}/deps/gtest/dist/${PLATFORM}"
  "-DWEBSOCKET_ROOT=$ENV{CROSS_PATH}/deps/websocketpp/build/src"
)

include("${CROSS_ROOT}/cmake/platforms.cmake")
