
if (NOT DEFINED ENV{EMSCRIPTEN_PATH})
  message(FATAL_ERROR "EMSCRIPTEN_PATH MUST BE DEFINED!")
endif()

# ---

#
# TODO: AVOID DOUBLE-INCLUSION OF FLAGS
#
set(CMAKE_CXX_FLAGS "-Wno-warn-absolute-paths -std=c++11"
  CACHE STRING "cmake_cxx_flags/emscripten"
)

include("$ENV{EMSCRIPTEN_PATH}/cmake/Modules/Platform/Emscripten.cmake")

# ---

if (DEFINED RUN)
  if (NOT PROJECT_NAME STREQUAL CMAKE_TRY_COMPILE)
    configure_file("${CROSS_ROOT}/cmake/emscripten/run.node.sh.in" run.sh)
    configure_file("${CROSS_ROOT}/cmake/install.nop.sh.in" install.sh)

    install(CODE "
      execute_process(COMMAND ./install.sh RESULT_VARIABLE result)
      if (result)
        message(FATAL_ERROR)
      endif()
    ")
  endif()
endif()
