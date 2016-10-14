#!/bin/sh

ARCHIVE_ZIP="master.zip"
ARCHIVE_SRC="https://github.com/zaphoyd/websocketpp/archive/$ARCHIVE_ZIP"
ARCHIVE_DIR="websocketpp-master"

# ---

rm -rf build
mkdir -p build
cd build

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
