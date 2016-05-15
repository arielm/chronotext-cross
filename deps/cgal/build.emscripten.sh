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
  -DBUILD_SHARED_LIBS=OFF \
  -DWITH_CGAL_Core=OFF \
  -DWITH_CGAL_Qt5=OFF \
  -DWITH_CGAL_ImageIO=OFF \
  -DCGAL_DISABLE_GMP=ON \
  -DBoost_INCLUDE_DIR="$CROSS_PATH/deps/boost/dist/$PLATFORM/include" \
  -DBoost_LIBRARY_DIR="$CROSS_PATH/deps/boost/dist/$PLATFORM/lib" \
  -DCMAKE_CXX_FLAGS="-DCGAL_HAS_NO_THREADS -DCGAL_FORWARD -DBOOST_MATH_DISABLE_STD_FPCLASSIFY -DBOOST_NO_NATIVE_LONG_DOUBLE_FP_CLASSIFY -DBOOST_MATH_NO_LONG_DOUBLE_MATH_FUNCTIONS"
 
if [ $? != 0 ]; then
  echo "CONFIGURATION FAILED!"
  exit -1
fi

# ---

cmake --build "$BUILD_DIR"

if [ $? != 0 ]; then
  echo "BUILD FAILED!"
  exit -1
fi

# ---

rm -rf "$INSTALL_PATH"
mkdir -p "$INSTALL_PATH/lib"
mv "$BUILD_DIR/lib/libCGAL.a" "$INSTALL_PATH/lib"
cp -r "$BUILD_DIR/include" "$INSTALL_PATH"

cd "$INSTALL_PATH"
ln -s "$SRC_PATH" "src"
