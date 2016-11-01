#!/bin/sh

PLATFORM="emscripten"

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

TOOLCHAIN_FILE="$CROSS_PATH/core/cmake/toolchains/emscripten.cmake"

cmake -H"$SRC_DIR" -B"$BUILD_DIR" \
  -DCMAKE_TOOLCHAIN_FILE="$TOOLCHAIN_FILE" -G Ninja \
  -DCMAKE_BUILD_TYPE=Release \
  -DLIBRARY_OUTPUT_PATH="$INSTALL_PATH/lib" \
  -DWITH_ZLIB=ON -DWITH_BZip2=OFF -DWITH_PNG=OFF -DWITH_HarfBuzz=OFF \
  -DZLIB_INCLUDE_DIR="$HOME/.emscripten_cache/ports-builds/zlib" -DZLIB_LIBRARY="$HOME/.emscripten_cache/zlib.bc"

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
