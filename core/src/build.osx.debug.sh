#!/bin/sh

PLATFORM="osx"

TREE_DIR="../../tree/chr"
BUILD_DIR="build/$PLATFORM"
INSTALL_DIR="tmp/$PLATFORM/lib/Debug"
INSTALL_PATH="$(pwd)/$INSTALL_DIR"

# ---

TOOLCHAIN_FILE="$CROSS_PATH/core/cmake/toolchains/osx.cmake"

cmake . -B"$BUILD_DIR" \
  -DCMAKE_TOOLCHAIN_FILE="$TOOLCHAIN_FILE" -G Ninja \
  -DCMAKE_BUILD_TYPE=Debug \
  -DLIBRARY_OUTPUT_PATH="$INSTALL_PATH" \
  -DPLATFORM="$PLATFORM"

if [ $? != 0 ]; then
  echo "CONFIGURATION FAILED!"
  exit -1
fi

# ---

rm -rf "$INSTALL_PATH"
cmake --build "$BUILD_DIR"

if [ $? != 0 ]; then
  echo "BUILD FAILED!"
  exit -1
fi

rm -rf   "$TREE_DIR/$PLATFORM/lib/Debug"
mkdir -p "$TREE_DIR/$PLATFORM/lib/Debug"
mv "tmp/$PLATFORM/lib/Debug" "$TREE_DIR/$PLATFORM/lib"
