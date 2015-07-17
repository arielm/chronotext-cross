
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

if (DEFINED RUN AND NOT PROJECT_NAME STREQUAL CMAKE_TRY_COMPILE)
  set(CONFIG_INSTALL "${CROSS_ROOT}/cmake/install.nop.sh.in")

  if (RUN MATCHES NODE)
    set(CONFIG_RUN "${CROSS_ROOT}/cmake/emscripten/run.node.sh.in")
  elseif (RUN MATCHES SPIDERMONKEY)
    set(CONFIG_RUN "${CROSS_ROOT}/cmake/emscripten/run.js.sh.in")
  elseif (RUN MATCHES BROWSER)
    set(CONFIG_RUN "${CROSS_ROOT}/cmake/emscripten/run.browser.sh.in")
  else()
    message(FATAL_ERROR "UNSUPPORTED RUN-MODE!")
  endif()
endif()
