
set(CTEST_PROJECT_NAME TestingWebGL)
set(CTEST_CONFIGURATION_TYPE Release)

if (PLATFORM MATCHES emscripten)
  set(ARGS
    -DRUN=BROWSER
  )
endif()

list(APPEND ARGS
  "-DBOOST_ROOT=$ENV{CROSS_PATH}/deps/boost/dist/${PLATFORM}"
  "-DGLM_ROOT=$ENV{CROSS_PATH}/deps/glm/dist"
)

include("${CROSS_ROOT}/cmake/platforms.cmake")
