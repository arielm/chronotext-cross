#!/bin/sh

PLATFORM="android64"

TREE_DIR="../../tree/chr"
BUILD_DIR="build/$PLATFORM"
INSTALL_DIR="tmp/$PLATFORM/lib/Release"
INSTALL_PATH="$(pwd)/$INSTALL_DIR"

# ---

TOOLCHAIN_FILE="$NDK_PATH/build/cmake/android.toolchain.cmake"

cmake . -B"$BUILD_DIR" \
  -DCMAKE_TOOLCHAIN_FILE="$TOOLCHAIN_FILE" -G Ninja -DCMAKE_MAKE_PROGRAM=/usr/local/bin/ninja \
  -DCMAKE_BUILD_TYPE=Release \
  -DLIBRARY_OUTPUT_PATH="$INSTALL_PATH" \
  -DPLATFORM="$PLATFORM" \
  -DANDROID_ABI="arm64-v8a" \
  -DCMAKE_CXX_FLAGS="-std=c++14"

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

rm -rf   "$TREE_DIR/$PLATFORM/lib/Release"
mkdir -p "$TREE_DIR/$PLATFORM/lib/Release"
mv "tmp/$PLATFORM/lib/Release" "$TREE_DIR/$PLATFORM/lib"
