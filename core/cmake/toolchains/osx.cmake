
set(OSX_ARCHS
  x86_64
  CACHE STRING "osx_archs"
)

set(OSX_DEPLOYMENT_TARGET 10.7
  CACHE STRING "osx_deployment_target"
)

# ---

set(CMAKE_OSX_ARCHITECTURES ${OSX_ARCHS}
  CACHE STRING "cmake_osx_architectures/osx"
)

#
# TODO: AVOID DOUBLE-INCLUSION OF FLAGS
#
set(CMAKE_CXX_FLAGS "-mmacosx-version-min=${OSX_DEPLOYMENT_TARGET} -stdlib=libc++ -std=c++11"
  CACHE STRING "cmake_cxx_flags/osx"
)

# ---

if (DEFINED RUN AND NOT PROJECT_NAME STREQUAL CMAKE_TRY_COMPILE)
  set(CONFIG_INSTALL "${CROSS_ROOT}/cmake/install.nop.sh.in")
  set(CONFIG_RUN "${CROSS_ROOT}/cmake/osx/run.sh.in")
endif()
