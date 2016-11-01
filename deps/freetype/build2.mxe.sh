#!/bin/sh

PLATFORM="mxe"

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

TOOLCHAIN_FILE="$CROSS_PATH/core/cmake/toolchains/mxe.cmake"

cmake -H"$SRC_DIR" -B"$BUILD_DIR" \
  -DCMAKE_TOOLCHAIN_FILE="$TOOLCHAIN_FILE" -G Ninja \
  -DCMAKE_BUILD_TYPE=Release \
  -DLIBRARY_OUTPUT_PATH="$INSTALL_PATH/lib" \
  -DWITH_ZLIB=ON -DWITH_BZip2=OFF -DWITH_PNG=OFF -DWITH_HarfBuzz=ON \
  -DHARFBUZZ_INCLUDE_DIRS="$CROSS_PATH/deps/harfbuzz/dist/$PLATFORM/include" -DHARFBUZZ_LIBRARIES="$CROSS_PATH/deps/harfbuzz/dist/$PLATFORM/lib"

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
ln -s "$SRC_PATH/include"
