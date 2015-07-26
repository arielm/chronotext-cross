cmake_minimum_required (VERSION 3.2.2)

include(ExternalProject)

ExternalProject_Add(setup
  GIT_REPOSITORY "git://github.com/arielm/glfw.git"

  PREFIX ""
  SOURCE_DIR "src"
  CONFIGURE_COMMAND ""
  BUILD_COMMAND ""
  INSTALL_COMMAND ""
)

add_custom_target(osx
  COMMAND rm -rf "../dist/osx"
  COMMAND cmake -H"src" -B"osx" -DCMAKE_TOOLCHAIN_FILE="$ENV{CROSS_PATH}/core/cmake/toolchains/osx.cmake" -G "Ninja" -DCMAKE_BUILD_TYPE=Release -DLIBRARY_OUTPUT_PATH="../../../dist/osx/lib" -DGLFW_BUILD_EXAMPLES=OFF -DGLFW_BUILD_TESTS=OFF -DGLFW_BUILD_DOCS=OFF -DGLFW_INSTALL=OFF
  COMMAND cmake --build "osx"
  COMMAND cd "../dist/osx" && ln -s "../../build/src/include"

  DEPENDS setup
)

add_custom_target(mxe
  COMMAND rm -rf "../dist/mxe"
  COMMAND cmake -H"src" -B"mxe" -DCMAKE_TOOLCHAIN_FILE="$ENV{CROSS_PATH}/core/cmake/toolchains/mxe.cmake" -G "Ninja" -DCMAKE_BUILD_TYPE=Release -DLIBRARY_OUTPUT_PATH="../../../dist/mxe/lib" -DGLFW_BUILD_EXAMPLES=OFF -DGLFW_BUILD_TESTS=OFF -DGLFW_BUILD_DOCS=OFF -DGLFW_INSTALL=OFF
  COMMAND cmake --build "mxe"
  COMMAND cd "../dist/mxe" && ln -s "../../build/src/include"

  DEPENDS setup
)
