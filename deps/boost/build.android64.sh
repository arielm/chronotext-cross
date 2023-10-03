#!/bin/sh

ANDROID_ABI=arm64-v8a

PLATFORM="android64"

SRC_DIR="../../tree/boost/src"
INSTALL_DIR="../../tree/boost/android/$ANDROID_ABI"

SRC_PATH="$(pwd)/$SRC_DIR"
INSTALL_PATH="$(pwd)/$INSTALL_DIR"
JAM_CONFIG_PATH="$(pwd)/configs/$PLATFORM.jam"

if [ ! -d "$SRC_PATH" ]; then
  echo "SOURCE NOT FOUND!"
  exit 1
fi

if [ -z "$NDK_PATH" ]; then
  echo "NDK_PATH MUST BE DEFINED!"
  exit -1  
fi

TOOLCHAINS="$(pwd)/toolchains"

# ---

LIBRARIES="--with-system --with-filesystem"

# ---

cd "$SRC_PATH"

rm bjam
rm b2
rm project-config.jam
rm bootstrap.log
rm -rf bin.v2

./bootstrap.sh 2>&1

if [ $? != 0 ]; then
  echo "ERROR: boostrap FAILED!"
  exit 1
fi

cat "$JAM_CONFIG_PATH" >> project-config.jam

# ---

rm -rf "$INSTALL_PATH"

HOST_NCORES=$(shell nproc 2>/dev/null || sysctl -n hw.ncpu 2>/dev/null || echo 1)

export TOOLCHAINS
export NO_BZIP2=1

./b2 -q -j$HOST_NCORES       \
  target-os=android          \
  toolset=clang-arm64_v8a    \
  link=static                \
  variant=release            \
  $LIBRARIES                 \
  stage                      \
  --stagedir="$INSTALL_PATH" \
  2>&1

if [ $? != 0 ]; then
  echo "ERROR: b2 FAILED!"
  exit 1
fi

# ---

cd "$INSTALL_PATH"
cd ..
ln -s "../src" include
