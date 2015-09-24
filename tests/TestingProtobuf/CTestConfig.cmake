
set(CTEST_PROJECT_NAME TestingProtobuf)
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

list(APPEND ARGS
  "-DBOOST_ROOT=$ENV{CROSS_PATH}/deps/boost/dist/${PLATFORM}"
  "-DGTEST_ROOT=$ENV{CROSS_PATH}/deps/gtest/dist/${PLATFORM}"
  "-DPROTOBUF_ROOT=$ENV{CROSS_PATH}/deps/protobuf/dist/${PLATFORM}"
)

include("${CROSS_ROOT}/cmake/platforms.cmake")
