
set(CTEST_PROJECT_NAME TestingGLFW)
set(CTEST_CONFIGURATION_TYPE Release)

if (PLATFORM MATCHES osx)
  set(ARGS
    -DRUN=EXE
  )

elseif (PLATFORM MATCHES mxe)
  set(ARGS
    -DRUN=EXE
    -DFS=RC
  )
endif()

list(APPEND ARGS
  "-DBOOST_ROOT=$ENV{CROSS_PATH}/deps/boost/dist/${PLATFORM}"
  "-DGLFW_ROOT=$ENV{CROSS_PATH}/deps/glfw/dist/${PLATFORM}"
  "-DGLM_ROOT=$ENV{CROSS_PATH}/deps/glm/dist"
)

include("${CROSS_ROOT}/cmake/platforms.cmake")
