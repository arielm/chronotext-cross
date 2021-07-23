#!/bin/sh

PLATFORM="android"

TREE_DIR="../../tree/chr"
BUILD_DIR="build/$PLATFORM"
INSTALL_DIR="tmp/$PLATFORM/lib/Release"
INSTALL_PATH="$(pwd)/$INSTALL_DIR"

# ---

TOOLCHAIN_FILE="$CROSS_PATH/core/cmake/toolchains/android.cmake"

cmake . -B"$BUILD_DIR" \
  -DCMAKE_TOOLCHAIN_FILE="$TOOLCHAIN_FILE" -G Ninja -DCMAKE_MAKE_PROGRAM=/usr/local/bin/ninja \
  -DCMAKE_BUILD_TYPE=Release \
  -DLIBRARY_OUTPUT_PATH="$INSTALL_PATH" \
  -DPLATFORM="$PLATFORM" \
  -DCMAKE_ANDROID_ARCH_ABI="armeabi-v7a"

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
