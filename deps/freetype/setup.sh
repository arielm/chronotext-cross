#!/bin/sh

ARCHIVE_ZIP="freetype-2.7.tar.gz"
ARCHIVE_SRC="http://download.savannah.gnu.org/releases/freetype/${ARCHIVE_ZIP}"
ARCHIVE_DIR="freetype-2.7"

# ---

rm -rf ../../tree/freetype
mkdir -p ../../tree/freetype
cd ../../tree/freetype

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
tar -zxvf $ARCHIVE_ZIP

if [ $? != 0 ] || [ ! -d $ARCHIVE_DIR ]; then
  echo "UNPACKING FAILED!"
  exit 1
fi

rm $ARCHIVE_ZIP
mv $ARCHIVE_DIR src
