#!/bin/sh

PLATFORM="mxe"

SRC_DIR="../../tree/boost/src"
INSTALL_DIR="../../tree/boost/$PLATFORM"

SRC_PATH="$(pwd)/$SRC_DIR"
INSTALL_PATH="$(pwd)/$INSTALL_DIR"
JAM_CONFIG_PATH="$(pwd)/configs/$PLATFORM.jam"

if [ ! -d "$SRC_PATH" ]; then
  echo "SOURCE NOT FOUND!"
  exit 1
fi

# ---

LIBRARIES="--with-system --with-filesystem --with-iostreams"

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

export MXE_TARGET=i686-w64-mingw32.static
export NO_ZLIB=0  # REQUIRED MXE PACKAGE: zlib
export NO_BZIP2=1

./b2 -q -j$HOST_NCORES       \
  threadapi=win32            \
  architecture=x86           \
  target-os=windows          \
  toolset=gcc-mxe            \
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
ln -s "$SRC_PATH" include
