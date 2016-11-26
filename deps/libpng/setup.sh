#!/bin/sh

ARCHIVE_ZIP="libpng17.zip"
ARCHIVE_SRC="https://github.com/arielm/libpng/archive/$ARCHIVE_ZIP"
ARCHIVE_DIR="libpng-libpng17"

# ---

rm -rf ../../tree/libpng
mkdir -p ../../tree/libpng
cd ../../tree/libpng

if [ ! -f $ARCHIVE_ZIP ]; then
  echo "DOWNLOADING $ARCHIVE_SRC"
  curl -L -O $ARCHIVE_SRC

  if [ $? != 0 ] || [ ! -f $ARCHIVE_ZIP ]; then
    echo "DOWNLOADING FAILED!"
    exit 1
  fi
fi

# ---

echo "UNPACKING $ARCHIVE_ZIP..."
unzip -q $ARCHIVE_ZIP

if [ $? != 0 ] || [ ! -d $ARCHIVE_DIR ]; then
  echo "UNPACKING FAILED!"
  exit 1
fi

rm $ARCHIVE_ZIP
mv $ARCHIVE_DIR src
