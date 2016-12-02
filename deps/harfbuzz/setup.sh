#!/bin/sh

ARCHIVE_ZIP="1.3.3.zip"
ARCHIVE_SRC="https://github.com/arielm/harfbuzz/archive/$ARCHIVE_ZIP"
ARCHIVE_DIR="harfbuzz-1.3.3"

# ---

rm -rf ../../tree/harfbuzz
mkdir -p ../../tree/harfbuzz
cd ../../tree/harfbuzz

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

# ---

cd src

./autogen.sh
./configure --with-glib=no
