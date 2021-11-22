
set(IOS_ARCHS
  arm64
  CACHE STRING "ios_archs"
)

set(IOS_DEPLOYMENT_TARGET 11.0
  CACHE STRING "ios_deployment_target"
)

# ---

set(CMAKE_OSX_SYSROOT iphoneos
  CACHE STRING "cmake_osx_sysroot/ios"
)

set(CMAKE_OSX_ARCHITECTURES ${IOS_ARCHS}
  CACHE STRING "cmake_osx_architectures/ios"
)

#
# TODO: AVOID DOUBLE-INCLUSION OF FLAGS
#
set(CMAKE_CXX_FLAGS "-miphoneos-version-min=${IOS_DEPLOYMENT_TARGET} -stdlib=libc++ -std=c++14"
  CACHE STRING "cmake_cxx_flags/ios"
)
