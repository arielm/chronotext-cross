#!/bin/sh

PLATFORM="emscripten"

TREE_DIR="../../tree/protobuf"
SRC_DIR="$TREE_DIR/src"
BUILD_DIR="build/$PLATFORM"
INSTALL_DIR="tmp/$PLATFORM"

SRC_PATH="$(pwd)/$SRC_DIR"
INSTALL_PATH="$(pwd)/$INSTALL_DIR"

if [ ! -d "$SRC_PATH" ]; then
  echo "SOURCE NOT FOUND!"
  exit 1
fi

# ---

TOOLCHAIN_FILE="$CROSS_PATH/core/cmake/toolchains/emscripten.cmake"

cmake -H"$SRC_DIR/cmake" -B"$BUILD_DIR" \
  -DCMAKE_TOOLCHAIN_FILE="$TOOLCHAIN_FILE" -G Ninja \
  -DCMAKE_BUILD_TYPE=Release \
  -DLIBRARY_OUTPUT_PATH="$INSTALL_PATH/lib" \
  -DPTHREADS=OFF \
  -DHAVE_ZLIB=ON -DZLIB_INCLUDE_DIRECTORIES="$HOME/.emscripten_cache/ports-builds/zlib"

if [ $? != 0 ]; then
  echo "CONFIGURATION FAILED!"
  exit 1
fi

# ---

rm -rf "$INSTALL_PATH"
cmake --build "$BUILD_DIR"

if [ $? != 0 ]; then
  echo "BUILD FAILED!"
  exit 1
fi

rm -rf   "$TREE_DIR/$PLATFORM"
mkdir -p "$TREE_DIR/$PLATFORM"
mv "tmp/$PLATFORM" "$TREE_DIR"

cd "$TREE_DIR/$PLATFORM"
ln -s "../src/src" include
