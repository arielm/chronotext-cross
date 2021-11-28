#!/bin/sh

ARCHIVE_TAR="boost_1_68_0.tar.gz"
ARCHIVE_SRC="https://boostorg.jfrog.io/artifactory/main/release/1.68.0/source/boost_1_68_0.tar.gz"
ARCHIVE_DIR="boost_1_68_0"

# ---

rm -rf ../../tree/boost
mkdir -p ../../tree/boost
cd ../../tree/boost

if [ ! -f $ARCHIVE_TAR ]; then
  echo "DOWNLOADING $ARCHIVE_SRC"
  curl -L -O $ARCHIVE_SRC

  if [ $? != 0 ] || [ ! -f $ARCHIVE_TAR ]; then
    echo "DOWNLOADING FAILED!"
    exit 1
  fi
fi

# ---

echo "UNPACKING $ARCHIVE_TAR..."
tar -xf $ARCHIVE_TAR

if [ $? != 0 ] || [ ! -d $ARCHIVE_DIR ]; then
  echo "UNPACKING FAILED!"
  exit 1
fi

rm $ARCHIVE_TAR
mv $ARCHIVE_DIR src
