
set(CTEST_PROJECT_NAME "TestingFileSystem2")
set(CTEST_CONFIGURATION_TYPE Release)

if (PLATFORM MATCHES osx)
  set(RUN "EXE")

elseif (PLATFORM MATCHES ios)
  set(RUN "APP")
# set(ARGS
#   "-DIOS_DEPLOYMENT_TARGET=5.1.1"
#   "-DIOS_ARCHS=armv7"
# )

elseif (PLATFORM MATCHES android)
  set(RUN "EXE")

elseif (PLATFORM MATCHES emscripten)
  set(RUN "NODE")

elseif (PLATFORM MATCHES mxe)
  set(RUN "EXE")
endif()

list(APPEND ARGS
  "-DBOOST_ROOT=$ENV{CROSS_PATH}/deps/boost/dist/${PLATFORM}"
)
