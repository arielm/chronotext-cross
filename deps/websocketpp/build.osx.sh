#!/bin/sh

PLATFORM="osx"

SRC_DIR="build/src"
BUILD_DIR="build/$PLATFORM"
INSTALL_DIR="dist/$PLATFORM"

SRC_PATH="$(pwd)/$SRC_DIR"
INSTALL_PATH="$(pwd)/$INSTALL_DIR"

if [ ! -d "$SRC_PATH" ]; then
  echo "SOURCE NOT FOUND!"
  exit 1
fi

# ---

TOOLCHAIN_FILE="$CROSS_PATH/core/cmake/toolchains/osx.cmake"

cmake -H"$SRC_DIR" -B"$BUILD_DIR" \
  -DCMAKE_TOOLCHAIN_FILE="$TOOLCHAIN_FILE" -G Ninja \
  -DCMAKE_BUILD_TYPE=Release \
  -DLIBRARY_OUTPUT_PATH="$INSTALL_PATH/lib" \
  -DBOOST_INCLUDEDIR="/Users/arielm/Documents/DEV2/chronotext-cross/deps/boost/build/src" \
  -DBOOST_LIBRARYDIR="/Users/arielm/Documents/DEV2/chronotext-cross/deps/boost/dist/osx/lib" \
  -DBUILD_TESTS="ON"
  #-DWEBSOCKETPP_BOOST_LIBS="system thread" -DWEBSOCKETPP_PLATFORM_LIBS="pthread" -DWEBSOCKETPP_PLATFORM_TLS_LIBS="ssl crypto"

if [ $? != 0 ]; then
  echo "CONFIGURATION FAILED!"
  exit -1
fi

# ---

rm -rf "$INSTALL_PATH" # XXX: REQUIRED?
cmake --build "$BUILD_DIR"

if [ $? != 0 ]; then
  echo "BUILD FAILED!"
  exit -1
fi

cd "$INSTALL_PATH"
ln -s "$SRC_PATH/src"
mv src include
