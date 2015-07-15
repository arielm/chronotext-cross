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

emcmake cmake -H"$SRC_DIR" -B"$BUILD_DIR" \
  -G Ninja \
  -DCMAKE_BUILD_TYPE=Release \
  -DLIBRARY_OUTPUT_PATH="$INSTALL_PATH/lib" \
  -Dgtest_disable_pthreads=ON

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
