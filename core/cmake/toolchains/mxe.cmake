
if (NOT DEFINED ENV{MXE_PATH})
  message(FATAL_ERROR "MXE_PATH MUST BE DEFINED!")
endif()

# ---

set(MXE_TARGET i686-w64-mingw32.static
  CACHE STRING "mxe_target"
)

#
# TODO: AVOID DOUBLE-INCLUSION OF FLAGS
#
set(CMAKE_CXX_FLAGS "-Wno-deprecated-declarations -std=c++11"
  CACHE STRING "cmake_cxx_flags/mxe"
)

include("$ENV{MXE_PATH}/usr/${MXE_TARGET}/share/cmake/mxe-conf.cmake")

# ---

if (DEFINED RUN)
  if (NOT PROJECT_NAME STREQUAL CMAKE_TRY_COMPILE)
    configure_file("${CROSS_ROOT}/cmake/mxe/run.sh.in" run.sh)
    configure_file("${CROSS_ROOT}/cmake/install.nop.sh.in" install.sh)

    install(CODE "
      execute_process(COMMAND ./install.sh RESULT_VARIABLE result)
      if (result)
        message(FATAL_ERROR)
      endif()
    ")
  endif()
endif()
